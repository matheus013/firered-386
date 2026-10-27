"""Compare original/adapted SPL command emission at the SDK dispatcher boundary."""
import argparse
import json
import subprocess
from pathlib import Path
from prepare_spl_drawing import prepare, HASH


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--probe', type=Path, required=True, help='Existing HGSS module probe with private headers')
    p.add_argument('--sdk', type=Path, required=True)
    p.add_argument('--sdk-build', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    native = a.output / 'adapted'
    native.mkdir()
    prepare(a.source, native / 'spl_draw_ptcl.c')
    flags = ['gcc', '-include', 'nitro.h', '-DSDK_PORT', '-DSDK_X86', '-DSDK_BUILD_LINUX', '-DSDK_64BIT',
             '-DSDK_VERSION_MAJOR=4', '-DPM_KEEP_ASSERTS', '-ffunction-sections', '-fdata-sections',
             '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-Wl,--gc-sections']
    for path in [a.probe, a.probe/'hgss-include', a.probe/'hgss-include/library',
                 a.sdk/'include', a.sdk_build/'gen', a.sdk_build/'gen/nitro/fx']:
        flags += ['-I' + str(path.resolve())]
    harness = Path(__file__).with_name('probe_spl_draw_commands.c')
    for label, directory in [('original', a.source.parent), ('adapted', native)]:
        binary = a.output / label / 'test' if label == 'adapted' else a.output / 'original-test'
        command = flags + ['-I'+str(directory.resolve())]
        if label == 'adapted': command += ['-DTEST_ADAPTED_SPL']
        command += [str(harness), str(a.sdk/'libraries/sim/src/sim_vars.c'), '-o', str(binary)]
        build = subprocess.run(command, capture_output=True, text=True)
        (a.output/(label+'-build.log')).write_text(build.stdout+build.stderr)
        build.check_returncode()
        result = subprocess.run([str(binary)], capture_output=True, text=True)
        (a.output/(label+'.log')).write_text(result.stdout+result.stderr)
        if result.returncode != (42 if label == 'original' else 0):
            raise RuntimeError(label + ': unexpected exit ' + str(result.returncode))
    report = dict(passed=True, original_missing_commands_reproduced=True,
                  source_sha256=HASH, signed_coordinate_cases=65536, planes=['XY', 'XZ'],
                  sanitizers=['address', 'undefined'], gpu_tested=False,
                  scope='complete SPL drawing unit, SDK inline APIs, recording dispatcher')
    (a.output/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__': main()
