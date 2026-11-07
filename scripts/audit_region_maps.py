"""Inventory decomp maps without importing assets or building a ROM."""
import argparse
import json
import subprocess
from pathlib import Path


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def audit(root, target):
    layouts = {x['id']: x for x in read(root / 'data/layouts/layouts.json')['layouts']}
    entries = [(p, read(p)) for p in sorted((root / 'data/maps').glob('*/map.json'))]
    ids = {m['id'] for _, m in entries}
    target_ids = {read(p)['id'] for p in (target / 'data/maps').glob('*/map.json')}
    records, problems = [], []
    for path, m in entries:
        issues = []
        layout = layouts.get(m['layout'])
        files = []
        if layout is None:
            issues.append('missing layout')
        else:
            for key in ('border_filepath', 'blockdata_filepath'):
                name = layout[key]
                file = root / name
                files.append(name)
                if not file.is_file():
                    issues.append(f'missing {name}')
                elif key == 'blockdata_filepath':
                    expected = layout['width'] * layout['height'] * 2
                    if file.stat().st_size != expected:
                        issues.append(f'block size {file.stat().st_size} != {expected}')
        links = []
        for kind, events in (('connection', m.get('connections') or []),
                             ('warp', m.get('warp_events') or [])):
            for event in events:
                dest = event.get('map', event.get('dest_map'))
                links.append({'kind': kind, 'destination': dest})
                # Dynamic sentinels are listed rather than treated as static destinations.
                if dest not in ids and dest not in ('MAP_NONE', 'MAP_UNDEFINED', 'MAP_DYNAMIC', None):
                    issues.append(f'unresolved {kind}: {dest}')
        record = {'id': m['id'], 'path': str(path.relative_to(root)).replace('\\', '/'),
                  'section': m.get('region_map_section'), 'layout': m['layout'],
                  'tilesets': [layout.get('primary_tileset'), layout.get('secondary_tileset')] if layout else [],
                  'files': files, 'links': links, 'conflicts_with_firered_id': m['id'] in target_ids,
                  'issues': issues}
        records.append(record)
        if issues:
            problems.append({'map': m['id'], 'issues': issues})
    pin = subprocess.check_output(['git', '-C', str(root), 'rev-parse', 'HEAD'], text=True).strip()
    return {'source_commit': pin, 'map_count': len(records),
            'conflicting_ids': sum(r['conflicts_with_firered_id'] for r in records),
            'maps_with_issues': len(problems), 'problems': problems, 'maps': records,
            'limits': 'Entire source inventory, not a region completeness or playability test. Scripts, graphics, permissions and warp indices not validated.'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--target', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    args = p.parse_args()
    report = audit(args.source, args.target)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('source_commit', 'map_count', 'conflicting_ids', 'maps_with_issues')}))


if __name__ == '__main__':
    main()
