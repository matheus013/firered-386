"""Stage explicitly reviewed attributes for Littleroot/Route101 only."""
import argparse
import json
from pathlib import Path
import struct
from region_tilesets import behavior_names, convert_attributes, words

# FireRed fieldmap.c: behavior bits 0..8, terrain bits 9..13,
# encounter bits 24..26, layer bits 29..30 (added by convert_attributes).
RULES = {
    'MB_NORMAL': (0x00000000, 'normal terrain, no encounter'),
    'MB_TALL_GRASS': (0x01000202, 'MB_TALL_GRASS, grass terrain, land encounters'),
    'MB_ANIMATED_DOOR': (0x00000069, 'FR MB_WARP_DOOR; animation graphics still pending'),
    'MB_JUMP_SOUTH': (0x0000003B, 'south ledge; not an encounter tile in source'),
    'MB_JUMP_SOUTHEAST': (0x00000000, 'source has no movement predicate for this unused behavior; do not invent diagonal jumping'),
}


def convert_selected(raw, selected, names, slots):
    source = words(raw)
    output = bytearray(slots * 4)
    reviewed = []
    for index in sorted(set(selected)):
        if not 0 <= index < len(source) or index >= slots:
            raise ValueError('Metatile outside source or target array')
        behavior = source[index] & 255
        name = names.get(behavior)
        if name not in RULES:
            raise ValueError(f'Behavior requires review: {name or behavior}')
        value, reason = RULES[name]
        converted = convert_attributes(struct.pack('<H', source[index]), {behavior: value})
        output[index*4:index*4+4] = converted
        reviewed.append({'index': index, 'source_behavior': name,
                         'target_attribute': struct.unpack('<I', converted)[0], 'reason': reason})
    return bytes(output), reviewed


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--audit', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    audit = json.loads(a.audit.read_text())
    if {v['map'] for v in audit} != {'LittlerootTown', 'Route101'}:
        raise ValueError('Only pilot exteriors have reviewed rules')
    names = behavior_names(a.source / 'include/constants/metatile_behaviors.h')
    selected, paths = {}, {}
    for m in audit:
        for s in m['tilesets']:
            selected.setdefault(s['symbol'], set()).update(s['used_indices'])
            paths[s['symbol']] = s['paths']['metatileAttributes']
    outputs, report = {}, {}
    for symbol, folder, slots in [('gTileset_General', 'primary', 640), ('gTileset_Petalburg', 'secondary', 144)]:
        raw = (a.source / paths[symbol]).read_bytes()
        outputs[folder], report[folder] = convert_selected(raw, selected[symbol], names, slots)
    a.out.mkdir(parents=True, exist_ok=False)
    for folder, data in outputs.items():
        (a.out / folder).mkdir()
        (a.out / folder / 'metatile_attributes.bin').write_bytes(data)
    a.report.write_text(json.dumps({'status': 'PILOT_ATTRIBUTES_ONLY_NOT_INSTALLED', 'tables': report,
                                   'limits': 'Unused entries zeroed. Source-grounded rules, not runtime-tested. No encounter tables or door animations installed.'}, indent=2) + '\n')
    print(f'{sum(map(len, report.values()))} pilot metatile attributes staged')


if __name__ == '__main__':
    main()
