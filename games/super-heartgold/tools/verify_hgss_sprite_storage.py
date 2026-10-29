"""Compile actual HGSS/SDK types and verify native animation storage boundaries."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from prepare_hgss_sprite_storage import prepare, HASH


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('hgss', 'probe', 'stack', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    a = parser.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    sdk = a.stack/'subprojects/libntr'
    nns = a.stack/'subprojects/libntrsystem'
    harness = Path(__file__).with_name('probe_hgss_sprite_storage.c')
    source = a.hgss/'include/sprite.h'
    logs = {}
    for label in ('original', 'native'):
        folder = a.output/label
        folder.mkdir()
        if label == 'native': prepare(source, folder/'sprite.h')
        else: (folder/'sprite.h').write_bytes(source.read_bytes())
        flags = ['gcc', '-std=gnu11', '-DSDK_PORT', '-DSDK_X86', '-DSDK_BUILD_LINUX',
                 '-DSDK_64BIT', '-DSDK_VERSION_MAJOR=4', '-DSOULSILVER', '-DENGLISH',
                 '-DSDK_FINALROM', '-DNNS_FINALROM', '-fsanitize=address,undefined',
                 '-fno-sanitize-recover=all', '-include', str(a.probe/'hgss_backup.h')]
        for inc in (folder, a.probe, a.probe/'hgss-include', a.probe/'hgss-include/library',
                    sdk/'include', nns/'include', a.stack/'build/subprojects/libntr/gen',
                    a.stack/'build/subprojects/libntr/gen/nitro/fx'):
            flags.append('-I'+str(inc))
        result = subprocess.run(flags+[str(harness), '-o', str(folder/'probe')], capture_output=True, text=True)
        (folder/'build.log').write_text(result.stdout+result.stderr)
        result.check_returncode()
        result = subprocess.run([str(folder/'probe')], capture_output=True, text=True)
        logs[label] = result.stdout+result.stderr
        (folder/'run.log').write_text(logs[label])
        if label == 'original':
            if result.returncode != 42 or 'storage too small' not in result.stderr:
                raise ValueError('Expected original inline animation capacity failure')
        else: result.check_returncode()
        if label == 'native':
            result = subprocess.run(flags+['-c', str(a.hgss/'src/sprite.c'), '-o', str(folder/'sprite.o')],
                                    capture_output=True, text=True)
            (folder/'sprite-build.log').write_text(result.stdout+result.stderr)
            result.check_returncode()
    report = {'passed': True, 'platform':'linux-x86_64', 'original_capacity_failure_reproduced':True,
              'source_sha256':HASH, 'adapted_sha256':hashlib.sha256((a.output/'native/sprite.h').read_bytes()).hexdigest(),
              'sprite_translation_unit_compiled':True,
              'scope':'actual_type_storage_size_alignment_and_adjacent_fields_not_sprite_manager_runtime', 'logs':logs}
    (a.output/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__': main()
