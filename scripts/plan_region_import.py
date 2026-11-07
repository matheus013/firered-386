"""Plan append-only pilot map IDs and connected interiors; no engine writes."""
import argparse
import hashlib
import json
from pathlib import Path


def allocate(groups, names, prefix):
    order = groups['group_order']
    if len(order) >= 256 or len(names) > 256:
        raise ValueError('Map group/number exceeds u8 storage')
    group = 'gMapGroup_' + prefix
    if group in groups:
        raise ValueError('Import group already exists')
    existing = {name for key in order for name in groups[key]}
    result = []
    for number, name in enumerate(sorted(names)):
        new = prefix + '_' + name
        if new in existing:
            raise ValueError('Map name collision')
        result.append({'source_name': name, 'target_name': new,
                       'group': len(order), 'number': number})
    return result


def plan(source, target, seeds, prefix):
    maps = {p.parent.name: json.loads(p.read_text()) for p in (source / 'data/maps').glob('*/map.json')}
    by_id = {m['id']: name for name, m in maps.items()}
    selected = set(seeds)
    if selected - maps.keys():
        raise ValueError('Unknown seed maps')
    pending = list(seeds)
    while pending:
        name = pending.pop()
        for warp in maps[name].get('warp_events', []):
            dest = by_id.get(warp['dest_map'])
            if dest and dest not in selected and maps[dest]['map_type'] not in (
                    'MAP_TYPE_TOWN', 'MAP_TYPE_CITY', 'MAP_TYPE_ROUTE', 'MAP_TYPE_OCEAN_ROUTE'):
                selected.add(dest)
                pending.append(dest)
    boundaries, issues, scripts = [], [], set()
    for name in sorted(selected):
        m = maps[name]
        for event in sum([m.get(k, []) or [] for k in ('object_events', 'coord_events', 'bg_events')], []):
            if event.get('script'):
                scripts.add(event['script'])
        for warp in m.get('warp_events', []):
            dest = by_id.get(warp['dest_map'])
            index = warp['dest_warp_id']
            if dest and type(index) is int and not 0 <= index < len(maps[dest].get('warp_events', [])):
                issues.append({'map': name, 'destination': dest, 'bad_warp_index': index})
            if dest not in selected:
                boundaries.append({'map': name, 'kind': 'warp', 'destination': warp['dest_map']})
        for edge in m.get('connections') or []:
            if by_id.get(edge['map']) not in selected:
                boundaries.append({'map': name, 'kind': 'connection', 'destination': edge['map']})
    raw = (target / 'data/maps/map_groups.json').read_bytes()
    groups = json.loads(raw)
    return {'status': 'PLAN_ONLY_NOT_REGISTERED', 'prefix': prefix,
            'base_groups_sha256': hashlib.sha256(raw).hexdigest(),
            'allocation': allocate(groups, selected, prefix),
            'boundary_links_requiring_resolution': boundaries, 'warp_index_issues': issues,
            'scripts_requiring_review': sorted(scripts),
            'limits': 'One pilot group per plan. Recalculate after each registered group. Script-only destinations not traversed; dynamic warp indices require runtime review.'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--target', type=Path, required=True)
    p.add_argument('--seeds', nargs='+', required=True)
    p.add_argument('--prefix', choices=['Johto', 'Hoenn'], required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    report = plan(a.source, a.target, a.seeds, a.prefix)
    a.out.write_text(json.dumps(report, indent=2) + '\n')
    print(f"{len(report['allocation'])} maps, {len(report['scripts_requiring_review'])} scripts, "
          f"{len(report['boundary_links_requiring_resolution'])} boundary links, "
          f"{len(report['warp_index_issues'])} invalid numeric warp indices")


if __name__ == '__main__':
    main()
