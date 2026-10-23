"""Pinned source archive and file inventory for flat, complete project checkouts."""
from pathlib import Path
import argparse, hashlib, json, os, subprocess, tarfile, tempfile

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT / 'game'
BASE_REF = 'ce6406d8c2eaf46d543c8a2606de0c3cb1951982'
ARCHIVE = ROOT / 'data/upstream/pokefirered-base.tar.gz'

def base_tar():
    manifest=json.loads(ARCHIVE.with_suffix('').with_suffix('.json').read_text())
    assert manifest['commit']==BASE_REF
    assert hashlib.sha256(ARCHIVE.read_bytes()).hexdigest()==manifest['sha256'], 'Base archive checksum mismatch'
    return tarfile.open(ARCHIVE, 'r:gz')

def base_bytes(path):
    with base_tar() as archive:
        return archive.extractfile(path).read()

def extract_base(destination):
    with base_tar() as archive:
        archive.extractall(destination, filter='data')

def game_files():
    if (GAME/'.git').exists():
        args=['git','-C',str(GAME),'ls-files','--cached','--others','--exclude-standard','-z']
        prefix=''
    elif (ROOT/'.git').exists():
        args=['git','-C',str(ROOT),'ls-files','--cached','--others','--exclude-standard','-z','--','game']
        prefix='game/'
    else:
        return json.loads((ROOT/'data/vendor_sources.json').read_text())['game']['files']
    names=subprocess.check_output(args).decode('utf-8').split('\0')
    return sorted(set(n[len(prefix):] for n in names if n and (ROOT/n if prefix else GAME/n).is_file()))

def check_patch():
    with tempfile.TemporaryDirectory(prefix='firered-base-check-') as folder:
        extract_base(folder)
        subprocess.run(['git','apply','--check',str(ROOT/'patches/current.patch')],cwd=folder,check=True,capture_output=True)
    return 'Cumulative patch applies to the checked pinned archive.'

def changed_files():
    nested=(GAME/'.git').exists()
    directory=GAME if nested else ROOT
    scope=[] if nested else ['--','game']
    changed=subprocess.check_output(['git','-C',str(directory),'diff','HEAD','--name-only',*scope],text=True).splitlines()
    changed+=subprocess.check_output(['git','-C',str(directory),'ls-files','--others','--exclude-standard',*scope],text=True).splitlines()
    return sorted(set(n if nested else n.removeprefix('game/') for n in changed))

if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--extract',type=Path)
    parser.add_argument('--files',action='store_true')
    parser.add_argument('--check-patch',action='store_true')
    parser.add_argument('--changed-files',action='store_true')
    args=parser.parse_args()
    if args.extract: extract_base(args.extract)
    elif args.files: print('\n'.join(game_files()))
    elif args.check_patch: print(check_patch())
    elif args.changed_files: print('\n'.join(changed_files()))
    else:
        with base_tar(): pass
        print(ARCHIVE)
