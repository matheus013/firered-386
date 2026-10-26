"""Combine compiled HGSS objects and inventory remaining native dependencies.

An SDK symbol match is only a candidate provider, not proof of a working link.
"""
import argparse
import json
import subprocess
from pathlib import Path


def symbols(path, defined):
    result = subprocess.run(['nm', '-g', '--defined-only' if defined else '--undefined-only',
                             str(path)], capture_output=True, text=True, check=True)
    found = set()
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 2 and len(parts[-2]) == 1:
            # Weak undefined references are optional and not link blockers.
            if defined or parts[-2] == 'U':
                found.add(parts[-1])
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--objects', type=Path, required=True)
    parser.add_argument('--sdk-build', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    modules = json.loads((args.objects / 'report.json').read_text())
    objects = [args.objects / (m['module'] + '.o') for m in modules if m['compiled']]
    if not objects:
        raise SystemExit('No successfully compiled modules')
    args.output.mkdir(parents=True, exist_ok=False)
    combined = args.output / 'hgss-combined.o'
    subprocess.run(['ld', '-r', '-o', str(combined), *map(str, objects)], check=True)
    unresolved = symbols(combined, False)
    providers = {}
    for archive in sorted(args.sdk_build.rglob('*.a')):
        for symbol in symbols(archive, True) & unresolved:
            providers.setdefault(symbol, []).append(str(archive.relative_to(args.sdk_build)))
    missing = sorted(unresolved - providers.keys())
    report = {'module_count': len(objects), 'relocatable_link_passed': True,
              'executable_link_tested': False, 'unresolved_count': len(unresolved),
              'sdk_candidate_count': len(providers), 'without_sdk_provider_count': len(missing),
              'sdk_candidates': providers, 'without_sdk_provider': missing,
              'by_module': {obj.stem: sorted(symbols(obj, False) & set(missing)) for obj in objects}}
    (args.output / 'report.json').write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')
    print(json.dumps({key: value for key, value in report.items()
                      if key not in ('sdk_candidates', 'without_sdk_provider', 'by_module')}, indent=2))


if __name__ == '__main__':
    main()
