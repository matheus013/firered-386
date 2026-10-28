"""Validate scene 4 tiles/maps/cells/animations with complete SDK loader units."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

from prepare_hgss_message_headers import read_banks
from prepare_hgss_opening_headers import ARCHIVE_SHA256, decompress_lz10
from prepare_sdk_animation_loader import prepare

MEMBERS = [54, 55, 56, 57, 58, 82, 84, 83, 86, 88, 87, 94, 96, 95, 90, 92, 91]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--stack', type=Path, required=True)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    raw = args.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ARCHIVE_SHA256:
        raise ValueError('Opening archive hash differs')
    args.output.mkdir(parents=True, exist_ok=False)
    files = []
    banks = read_banks(raw)
    for member in MEMBERS:
        path = args.output / f'{member}.bin'
        path.write_bytes(decompress_lz10(banks[member]))
        files.append(str(path))
    sdk = args.stack / 'subprojects/libntr'
    nns = args.stack / 'subprojects/libntrsystem'
    sources = [nns / f'libraries/g2d/src/load/g2d_{name}_load.c'
               for name in ('NCG', 'NSC', 'NOB', 'NAN')]
    sources.append(nns / 'libraries/g2d/src/load/g2d_Load.c')
    harness = Path(__file__).with_name('probe_sdk_scene_loaders.c')
    flags = ['gcc', '-g', '-DSDK_PORT', '-DSDK_X86', '-DSDK_BUILD_LINUX',
             '-DSDK_64BIT', '-DSDK_VERSION_MAJOR=4', '-DSDK_FINALROM', '-DNNS_FINALROM',
             '-ffunction-sections', '-fdata-sections', '-fsanitize=address,undefined',
             '-fno-sanitize-recover=all', '-Wl,--gc-sections']
    for include in (sdk / 'include', nns / 'include', nns / 'libraries/g2d/include',
                    args.stack / 'build/subprojects/libntr/gen',
                    args.stack / 'build/subprojects/libntr/gen/nitro/fx'):
        flags.append('-I' + str(include))
    adapted = args.output / 'g2d_NAN_native.c'
    prepare(sources[3], adapted)
    for label in ('original', 'adapted'):
        units = list(sources)
        if label == 'adapted':
            units[3] = adapted
        binary = args.output / ('scene-loaders-' + label)
        result = subprocess.run(flags + [str(harness), *map(str, units), '-o', str(binary)],
                                capture_output=True, text=True)
        (args.output / (label + '-build.log')).write_text(result.stdout + result.stderr)
        result.check_returncode()
        result = subprocess.run([str(binary), *files], capture_output=True, text=True)
        (args.output / (label + '.log')).write_text(result.stdout + result.stderr)
        if label == 'original':
            if result.returncode != 42 or 'out->pStringBank == NULL' not in result.stderr:
                raise ValueError('Expected uninitialized optional animation pointers')
        else:
            result.check_returncode()
    report = {'passed': True, 'members': MEMBERS, 'cycles_per_member': 512,
              'input_unchanged': True, 'gpu_tested': False,
              'sanitizers': ['address', 'undefined'],
              'original_optional_pointer_failure_reproduced': True,
              'adapted_animation_sha256': hashlib.sha256(adapted.read_bytes()).hexdigest(),
              'source_sha256': {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                                for p in [harness, *sources]}}
    (args.output / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
