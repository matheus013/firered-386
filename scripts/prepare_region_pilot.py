"""Convert pilot map blocks in isolation; does not install maps or build the game."""
import argparse
import hashlib
import json
import re
import struct
import subprocess
from pathlib import Path


def remap_blocks(raw, source_split, target_split=640):
    if len(raw) % 2:
        raise ValueError('Truncated 16-bit map block')
    result = bytearray()
    for (block,) in struct.iter_unpack('<H', raw):
        tile = block & 1023
        if block == 1023:  # Preserve the exact MAPGRID_UNDEFINED sentinel.
            converted = block
        else:
            mapped = tile if tile < source_split else target_split + tile - source_split
            if mapped >= 1023 or (tile < source_split and mapped >= target_split):
                raise ValueError(f'Metatile {tile} cannot fit safely in target partition')
            converted = (block & 0xFC00) | mapped
        result.extend(struct.pack('<H', converted))
    return bytes(result)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--maps', nargs='+', required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    args = p.parse_args()
    root = args.source.resolve()
    if args.out.resolve() == root or root in args.out.resolve().parents:
        raise ValueError('Output must be outside source checkout')
    header = (root / 'include/fieldmap.h').read_text()
    split = int(re.search(r'#define NUM_METATILES_IN_PRIMARY\s+(\d+)', header)[1])
    layouts = {v['id']: v for v in json.loads((root / 'data/layouts/layouts.json').read_text())['layouts']}
    results, writes = [], []
    for name in args.maps:
        if not re.fullmatch(r'[A-Za-z0-9_]+', name):
            raise ValueError('Invalid map name')
        m = json.loads((root / 'data/maps' / name / 'map.json').read_text())
        layout = layouts[m['layout']]
        record = {'map': name, 'id': m['id'], 'width': layout['width'], 'height': layout['height'],
                  'tilesets': [layout['primary_tileset'], layout['secondary_tileset']],
                  'connections': m.get('connections'), 'warps': m.get('warp_events'),
                  'blocks': []}
        for field, filename in [('blockdata_filepath', 'map.bin'), ('border_filepath', 'border.bin')]:
            source = (root / layout[field]).resolve()
            if root not in source.parents:
                raise ValueError('Asset path escapes source checkout')
            raw = source.read_bytes()
            if filename == 'map.bin' and len(raw) != layout['width'] * layout['height'] * 2:
                raise ValueError('Layout dimensions do not match block data')
            converted = remap_blocks(raw, split)
            writes.append((name, filename, converted))
            record['blocks'].append({'file': filename, 'bytes': len(raw),
                                     'source_sha256': hashlib.sha256(raw).hexdigest(),
                                     'converted_sha256': hashlib.sha256(converted).hexdigest(),
                                     'changed_blocks': sum(a != b for a, b in zip(
                                         struct.iter_unpack('<H', raw), struct.iter_unpack('<H', converted)))})
        results.append(record)
    # All conversions must succeed before creating any output.
    args.out.mkdir(parents=True, exist_ok=False)
    for name, filename, content in writes:
        destination = args.out / name / filename
        destination.parent.mkdir(exist_ok=True)
        destination.write_bytes(content)
    report = {'source_commit': subprocess.check_output(['git', '-C', str(root), 'rev-parse', 'HEAD'], text=True).strip(),
              'source_partition': split, 'target_partition': 640, 'maps': results,
              'status': 'BLOCKS_ONLY_NOT_INSTALLED',
              'pending': ['tiles/palettes', 'metatile attributes/behaviors', 'animations',
                          'scripts/events', 'map registration', 'playtesting']}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(f'{len(results)} maps converted in isolation; not installed or playable')


if __name__ == '__main__':
    main()
