"""Inspect pilot tileset dependencies and convert entries with explicit mappings."""
import argparse
from collections import Counter, defaultdict
import json
from pathlib import Path
import re
import struct


def words(data, width=2):
    if len(data) % width:
        raise ValueError('Truncated tile data')
    return [v[0] for v in struct.iter_unpack('<H' if width == 2 else '<I', data)]


def convert_entries(data, tile_map, palette_map):
    """Preserve horizontal/vertical flip bits. Never guess missing mappings."""
    output = bytearray()
    for value in words(data):
        tile, palette = value & 1023, value >> 12
        if tile not in tile_map or palette not in palette_map:
            raise ValueError(f'Missing mapping for tile {tile}, palette {palette}')
        target_tile, target_palette = tile_map[tile], palette_map[palette]
        if type(target_tile) is not int or not 0 <= target_tile < 1024:
            raise ValueError('Invalid target tile')
        if type(target_palette) is not int or not 0 <= target_palette < 13:
            raise ValueError('Invalid target palette')
        output.extend(struct.pack('<H', (value & 0x0C00) | target_tile | target_palette << 12))
    return bytes(output)


def convert_attributes(data, behavior_rules):
    """Rules map source behavior IDs to reviewed FR attributes excluding layer bits."""
    output = bytearray()
    for value in words(data):
        behavior, layer = value & 255, value >> 12
        if value & 0x0F00 or layer > 2:
            raise ValueError('Unsupported source attribute bits/layer')
        if behavior not in behavior_rules:
            raise ValueError(f'Unreviewed behavior {behavior}')
        rule = behavior_rules[behavior]
        if type(rule) is not int or not 0 <= rule < (1 << 32) or rule & 0x60000000:
            raise ValueError('Rule must be a u32 without layer bits')
        output.extend(struct.pack('<I', rule | layer << 29))
    return bytes(output)


def behavior_names(path):
    text = re.sub(r'/\*.*?\*/|//[^\n]*', '', path.read_text(), flags=re.S)
    pairs = re.findall(r'#define\s+(MB_\w+)\s+(0x[0-9a-fA-F]+|\d+)\b', text)
    if pairs:
        return {int(value, 0): name for name, value in pairs}
    names, current = {}, 0
    for name, explicit in re.findall(r'\b(MB_\w+)\s*(?:=\s*(0x[0-9a-fA-F]+|\d+))?\s*[,}]', text):
        if explicit:
            current = int(explicit, 0)
        names[current] = name
        current += 1
    return names


def inspect(root, map_name, target):
    load = lambda p: json.loads(p.read_text())
    m = load(root / 'data/maps' / map_name / 'map.json')
    layout = next(v for v in load(root / 'data/layouts/layouts.json')['layouts'] if v['id'] == m['layout'])
    fieldmap = (root / 'include/fieldmap.h').read_text()
    split = int(re.search(r'#define NUM_METATILES_IN_PRIMARY\s+(\d+)', fieldmap)[1])
    headers = (root / 'src/data/tilesets/headers.h').read_text()
    definitions = (root / 'src/data/tilesets/metatiles.h').read_text()
    names = behavior_names(root / 'include/constants/metatile_behaviors.h')
    target_names = set(behavior_names(target / 'include/constants/metatile_behaviors.h').values())
    blocks = words((root / layout['blockdata_filepath']).read_bytes()) + words((root / layout['border_filepath']).read_bytes())
    used = set(v & 1023 for v in blocks if v != 1023)
    sections = []
    for secondary, key in ((False, 'primary_tileset'), (True, 'secondary_tileset')):
        symbol = layout[key]
        body = re.search(r'\b' + re.escape(symbol) + r'\s*=\s*\{(.*?)\};', headers, re.S)[1]
        paths = {}
        for prop in ('metatiles', 'metatileAttributes'):
            resource = re.search(r'\.' + prop + r'\s*=\s*(\w+)', body)[1]
            match = re.search(r'\b' + resource + r'\[\]\s*=\s*INCBIN_U(16|32)\("([^"]+)"\)', definitions)
            if match is None or match[1] != '16':
                raise ValueError(f'Unsupported source table: {resource}')
            paths[prop] = match[2]
        entries = words((root / paths['metatiles']).read_bytes())
        attributes = words((root / paths['metatileAttributes']).read_bytes())
        if len(entries) != len(attributes) * 8:
            raise ValueError('Metatile/attribute count mismatch')
        selected = sorted(v - split if secondary else v for v in used if (v >= split) == secondary)
        if selected and selected[-1] >= len(attributes):
            raise ValueError('Layout refers beyond metatile array')
        tile_ids, palettes, behaviors, layers = set(), set(), Counter(), Counter()
        for index in selected:
            for entry in entries[index*8:index*8+8]:
                tile_ids.add(entry & 1023)
                palettes.add(entry >> 12)
            behaviors[names.get(attributes[index] & 255, f'UNKNOWN_{attributes[index] & 255}')] += 1
            layers[attributes[index] >> 12] += 1
        sections.append({'symbol': symbol, 'paths': paths, 'metatiles_total': len(attributes),
                         'metatiles_used': len(selected), 'used_indices': selected,
                         'tile_ids_used': sorted(tile_ids), 'palettes_used': sorted(palettes),
                         'behaviors_used': dict(behaviors), 'layer_types': dict(layers),
                         'behaviors_without_same_name_in_firered': sorted(set(behaviors) - target_names),
                         'animation_callback': re.search(r'\.callback\s*=\s*(\w+)', body)[1]})
    return {'map': map_name, 'tilesets': sections,
            'limits': 'Same behavior name does not prove semantic equivalence; callbacks, palette images and terrain/encounter attributes still need review.'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--target', type=Path, required=True)
    p.add_argument('--maps', nargs='+', required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    report = [inspect(a.source, name, a.target) for name in a.maps]
    a.out.parent.mkdir(parents=True, exist_ok=True)
    a.out.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({v['map']: [s['behaviors_without_same_name_in_firered'] for s in v['tilesets']] for v in report}))


if __name__ == '__main__':
    main()
