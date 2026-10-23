"""Package the pinned upstream sources plus local changes, without ROMs or .git."""
from pathlib import Path
import difflib
import hashlib
import io
import json
import subprocess
import zipfile

root = Path(__file__).resolve().parent
repo = root / 'vba-link'
base = subprocess.check_output(['git', '-C', str(repo), 'rev-parse', 'HEAD'], text=True).strip()
assert base == 'a4f4ff94a9e5ba651b6258b3b4d8319813341577'
changed = ['src/GBALink.cpp', 'src/LinkTransport.h']
patch = []
for name in changed:
    if name.endswith('LinkTransport.h'):
        before, old_name = '', '/dev/null'
    else:
        before = subprocess.check_output(['git', '-C', str(repo), 'show', f'{base}:{name}']).decode()
        old_name = 'a/' + name
    after = (repo / name).read_text()
    patch.extend(difflib.unified_diff(before.splitlines(True), after.splitlines(True),
                                      fromfile=old_name, tofile='b/' + name))
(root / 'vbalink-1.8-transport.patch').write_text(''.join(patch), newline='\n')
(root / 'improved-server/COPYING').write_bytes((repo / 'COPYING').read_bytes())

files = changed + ['COPYING']
hashes = {f'vba-link/{name}': hashlib.sha256((repo / name).read_bytes()).hexdigest() for name in files}
for relative in ['downloads/V172lsrc.zip', 'improved-server/src/main.cpp',
                 'improved-server/bin/vbalink-server.exe', 'improved-server/bin/vbalink-server-linux',
                 'vbalink-1.8-transport.patch', 'test_transport18.cpp', 'test_integration18.cpp']:
    hashes[relative] = hashlib.sha256((root / relative).read_bytes()).hexdigest()
manifest = {
    'upstream_18': 'https://github.com/luckytyphlosion/vba-link',
    'upstream_commit': base,
    'original_172': 'https://www.vbalink.info/downloads/V172lsrc.zip',
    'target': 'Experimental Linux 1.8 TCP source corrections; not a Windows 1.8 release',
    'validation': ['265 helper scenarios: PASS (ASan/UBSan)',
                   'Actual 1.8 LAN client/host method integration: PASS (ASan/UBSan)',
                   'GBALink.cpp C++17 object compilation: PASS (legacy warnings)',
                   '1.72 synthetic relay protocol suite: PASS on five build variants'],
    'not_validated': ['complete GUI build', 'Windows 1.8', 'RFU/WiFi', 'real Pokemon trades/battles', 'internet play'],
    'sha256': hashes,
}
(root / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')

archive_data = subprocess.check_output(['git', '-C', str(repo), 'archive', '--format=zip', base])
with zipfile.ZipFile(io.BytesIO(archive_data)) as upstream, zipfile.ZipFile(
        root / 'vbalink-1.8-source-improved-dev.zip', 'w', zipfile.ZIP_DEFLATED) as output:
    for member in upstream.infolist():
        if member.is_dir():
            continue
        data = (repo / member.filename).read_bytes() if member.filename in changed else upstream.read(member)
        output.writestr('vba-link/' + member.filename, data)
    output.writestr('vba-link/src/LinkTransport.h', (repo / 'src/LinkTransport.h').read_bytes())
    for relative in ['README.md', 'AUDIT.md', 'manifest.json', 'vbalink-1.8-transport.patch',
                     'test_transport18.cpp', 'test_integration18.cpp', 'test-18.sh']:
        output.write(root / relative, relative)
    for path in sorted((root / 'test-results').glob('*.txt')):
        output.write(path, path.relative_to(root))

with zipfile.ZipFile(root / 'vbalink-1.8-source-improved-dev.zip') as check:
    assert check.testzip() is None
print('Packaged patched 1.8 sources, patch, tests, validation logs, license and provenance.')
