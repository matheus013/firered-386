"""Build pinned libntr and libntrsystem together in a new private Linux folder.

Builds libraries only. Does not distribute their sources or launch HGSS.
"""
import argparse
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from prepare_hgss_heap import prepare_sdk_heap
from prepare_sdk_texture_upload import prepare as prepare_texture_upload
from prepare_sdk_palette_loader import prepare as prepare_palette_loader
from prepare_sdk_animation_loader import prepare as prepare_animation_loader

PINS = {'libntr': '380a3d7e60de82aced82075e7b3b9acf5f68b1cd',
        'libntrsystem': '97fa995ed33fe2ab8c83ec7068aa3aa5b4c3e12f'}


def run(command, cwd=None):
    subprocess.run(list(map(str, command)), cwd=cwd, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--libntr', type=Path, required=True)
    parser.add_argument('--libntrsystem', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--jobs', type=int, default=6)
    args = parser.parse_args()
    root = args.output.resolve()
    root.mkdir(parents=True, exist_ok=False)
    subprojects = root / 'subprojects'
    subprojects.mkdir()
    for name, revision in PINS.items():
        destination = subprojects / name
        run(['git', 'clone', '--no-hardlinks', '--no-checkout', getattr(args, name).resolve(), destination])
        run(['git', 'checkout', '--detach', revision], destination)
    tools = Path(__file__).resolve().parent
    sdk = subprojects / 'libntr'
    patches = []
    for name in ('libntr-clip-matrix.patch', 'libntr-single-screen-prototype.patch'):
        patch = tools / 'patches' / name
        run(['git', 'apply', '--check', patch], sdk)
        run(['git', 'apply', patch], sdk)
        patches.append({'name': name, 'sha256': hashlib.sha256(patch.read_bytes()).hexdigest()})
    shutil.copy2(tools / 'reference_compat/single_screen_touch.h',
                 sdk / 'include/simulator/hg_single_screen_touch.h')
    expheap = subprojects / 'libntrsystem/libraries/fnd/src/expheap.c'
    prepare_sdk_heap(expheap, expheap)
    texture_upload = sdk / 'libraries/gx/src/gx_load3d.c'
    prepare_texture_upload(texture_upload, texture_upload)
    palette_loader = subprojects / 'libntrsystem/libraries/g2d/src/load/g2d_NCL_load.c'
    prepare_palette_loader(palette_loader, palette_loader)
    animation_loader = subprojects / 'libntrsystem/libraries/g2d/src/load/g2d_NAN_load.c'
    prepare_animation_loader(animation_loader, animation_loader)
    (root / 'meson.build').write_text(
        "project('hgss-sdk-stack', 'c', 'cpp')\n"
        "subproject('libntr', default_options: ['build_target=linux', 'standalone=false'])\n"
        "subproject('libntrsystem', default_options: ['build_target=linux', 'standalone=false'])\n")
    # The upstream SDK's metadata generator requires a parent project commit.
    run(['git', 'init', '-q'], root)
    run(['git', 'add', 'meson.build'], root)
    run(['git', '-c', 'user.name=Codex', '-c', 'user.email=codex@localhost',
         'commit', '-qm', 'Local native SDK build harness'], root)
    build = root / 'build'
    for label, command in (
            ('configure', ['meson', 'setup', str(build), str(root)]),
            ('build', ['meson', 'compile', '-C', str(build), '-j', str(args.jobs)])):
        with (root / f'{label}.log').open('w') as log:
            result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT)
        if result.returncode:
            print((root / f'{label}.log').read_text()[-10000:])
            raise SystemExit(result.returncode)
    libraries = sorted((build / 'subprojects/libntrsystem/libraries').rglob('*.a'))
    expected = {'libnnsfnd.a', 'libnnsg2d.a', 'libnnsg3d.a', 'libnnsgfd.a',
                'libnnsmcs.a', 'libnnssnd.a'}
    if {path.name for path in libraries} != expected:
        raise SystemExit('Expected six NitroSystem libraries')
    report = {'passed': True, 'pins': PINS, 'patches': patches, 'platform': 'linux-x86_64',
              'native_expheap_sha256': hashlib.sha256(expheap.read_bytes()).hexdigest(),
              'native_texture_upload_sha256': hashlib.sha256(texture_upload.read_bytes()).hexdigest(),
              'native_palette_loader_sha256': hashlib.sha256(palette_loader.read_bytes()).hexdigest(),
              'native_animation_loader_sha256': hashlib.sha256(animation_loader.read_bytes()).hexdigest(),
              'runtime_tested': False, 'libraries': [
                  {'file': str(path.relative_to(build)), 'bytes': path.stat().st_size,
                   'sha256': hashlib.sha256(path.read_bytes()).hexdigest()} for path in libraries]}
    (root / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
