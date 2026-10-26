"""Run complete GX upload/bank units with real MI CPU copies, without a GPU."""
import argparse
import hashlib
import json
import shlex
import subprocess
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.build = args.build.resolve()
    args.output = args.output.resolve()
    args.output.mkdir(parents=True, exist_ok=False)
    commands = json.loads((args.build / 'compile_commands.json').read_text())
    names = ['gx_load3d.c', 'gx_vramcnt.c', 'gxstate.c', 'mi_memory.c', 'mi_dma.c', 'sim_vars.c']
    objects = []
    hashes = {}
    common = None
    for name in names:
        entry = next(c for c in commands if Path(c['file']).name == name)
        hashes[name] = hashlib.sha256((args.build / entry['file']).read_bytes()).hexdigest()
        parts = shlex.split(entry['command'])
        flags = []
        i = 1
        while i < len(parts):
            item = parts[i]
            if item in ('-o', '-MF', '-MQ', '-include'):
                i += 2
                continue
            if item in ('-c', '-MD', '-fpch-preprocess') or item == entry['file']:
                i += 1
                continue
            flags.append(item)
            i += 1
        flags += ['-include', 'nitro.h']
        if common is None:
            common = flags
        obj = args.output / (name + '.o')
        subprocess.run(['cc', *flags, '-ffunction-sections', '-fdata-sections',
                        '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                        '-c', entry['file'], '-o', str(obj)], cwd=args.build, check=True)
        objects.append(str(obj))
    binary = args.output / 'upload-runtime'
    harness = Path(__file__).with_name('probe_sdk_upload_runtime.c').resolve()
    subprocess.run(['cc', *common, '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                    '-Wl,--gc-sections', str(harness), *objects, '-o', str(binary)],
                   cwd=args.build, check=True)
    result = subprocess.run([str(binary)], capture_output=True, text=True)
    (args.output / 'runtime.log').write_text(result.stdout + result.stderr)
    result.check_returncode()
    report = {'passed': True, 'sdk_units': names, 'source_sha256': hashes,
              'texture_bank_combinations': 15, 'texture_upload_paths': 2,
              'platform_hooks': 'headless globals; assertion, terminate and DMA callback abort',
              'cpu_copy_tested': True,
              'bank_restore_tested': True, 'dma_tested': False, 'gpu_tested': False,
              'build_configuration': 'SDK_FINALROM, matching SDK build',
              'sanitizers': ['address', 'undefined'], 'output': result.stdout.strip()}
    (args.output / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(result.stdout, end='')


if __name__ == '__main__':
    main()
