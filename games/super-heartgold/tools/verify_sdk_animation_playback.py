"""Check native animation frame timing, loop and stop against real intro sequences."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from prepare_hgss_message_headers import read_banks
from prepare_hgss_opening_headers import ARCHIVE_SHA256, decompress_lz10
from prepare_sdk_animation_controller import prepare, HASH


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('stack', 'archive', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    raw = args.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ARCHIVE_SHA256:
        raise ValueError('Opening archive differs')
    args.output.mkdir(parents=True, exist_ok=False)
    banks = read_banks(raw)
    files = []
    for member in (83,87,95,91):
        path = args.output / f'{member}.bin'
        path.write_bytes(decompress_lz10(banks[member]))
        files.append(str(path))
    sdk = args.stack / 'subprojects/libntr'
    nns = args.stack / 'subprojects/libntrsystem'
    source = nns / 'libraries/g2d/src/g2d_Animation.c'
    flags = ['gcc', '-g', '-DSDK_PORT', '-DSDK_X86', '-DSDK_BUILD_LINUX', '-DSDK_64BIT',
             '-DSDK_VERSION_MAJOR=4', '-DSDK_FINALROM', '-DNNS_FINALROM',
             '-ffunction-sections', '-fdata-sections', '-fsanitize=address,undefined',
             '-fno-sanitize-recover=all', '-Wl,--gc-sections']
    for inc in (sdk/'include', nns/'include', nns/'libraries/g2d/include',
                args.stack/'build/subprojects/libntr/gen', args.stack/'build/subprojects/libntr/gen/nitro/fx'):
        flags.append('-I' + str(inc))
    harness = Path(__file__).with_name('probe_sdk_animation_playback.c')
    adapted = args.output / 'g2d_Animation_native.c'
    prepare(source, adapted)
    for label, unit in [('original', source), ('adapted', adapted)]:
        binary = args.output / label
        result = subprocess.run(flags + [str(harness), str(unit), str(sdk/'libraries/fx/src/fx.c'),
            str(nns/'libraries/g2d/src/load/g2d_NAN_load.c'),
            str(nns/'libraries/g2d/src/load/g2d_Load.c'), '-o', str(binary)], capture_output=True, text=True)
        (args.output/(label+'-build.log')).write_text(result.stdout + result.stderr)
        result.check_returncode()
        result = subprocess.run([str(binary), *files], capture_output=True, text=True)
        (args.output/(label+'.log')).write_text(result.stdout + result.stderr)
        if label == 'original':
            if result.returncode == 0 or 'heap-buffer-overflow' not in result.stderr:
                raise ValueError('Expected original sequence edge overflow')
        else:
            result.check_returncode()
    report = {'passed': True, 'scope': 'animation_controller_not_rendering',
              'sanitizers': ['address','undefined'], 'log': result.stdout,
              'original_overflow_reproduced': True, 'original_sha256': HASH,
              'adapted_sha256': hashlib.sha256(adapted.read_bytes()).hexdigest()}
    (args.output/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__': main()
