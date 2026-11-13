"""Fill in the Hoenn pilot metatile attributes used by batch 1 (Oldale, Routes 102/103, Petalburg).

convert_hoenn_attributes.py only converted the metatiles Littleroot and Route 101 use; the rest of
the tables stayed zero, so batch 1's doors, water and east ledges had no behavior. This patches
only entries that are still zero and used by a batch-1 layout, from the Emerald source, with the
same FireRed values the stock tilesets use for each behavior. Layer bits come from the source.

Run from the repository root:
  python scripts/convert_hoenn_batch1_attributes.py --source ../map-references/pokeemerald
"""
import argparse
import json
from pathlib import Path
import struct

from region_tilesets import behavior_names

GAME = Path('games/super-fire-red')
LAYOUTS = ['LAYOUT_HOENN_OLDALE_TOWN', 'LAYOUT_HOENN_ROUTE102', 'LAYOUT_HOENN_ROUTE103',
           'LAYOUT_HOENN_PETALBURG_CITY']
# Emerald behavior -> FireRed attribute (without layer bits). Values are the ones FireRed's own
# tilesets use most for that behavior (water: surf terrain bit 10 and water encounters bit 25).
RULES = {
    # batch 2 (Route 104, Petalburg Woods, Rustboro)
    'MB_TALL_GRASS': (0x01000202, 'tall grass, land encounters'),
    'MB_JUMP_SOUTH': (0x0000003B, 'south ledge'), 'MB_JUMP_WEST': (0x00000039, 'west ledge'),
    'MB_SAND': (0x00000021, 'same name'), 'MB_PUDDLE': (0x00000016, 'same name'),
    'MB_NON_ANIMATED_DOOR': (0x00000060, 'FR MB_CAVE_DOOR (Petalburg Woods entrances)'),
    'MB_NORTH_ARROW_WARP': (0x00000064, 'same name'), 'MB_SOUTH_ARROW_WARP': (0x00000065, 'same name'),
    'MB_DEEP_SOUTH_WARP': (0x00000065, 'FireRed has no deep south warp; south arrow warp'),
    'MB_REFLECTION_UNDER_BRIDGE': (0x02000410, 'water under the Route 104 bridge: surfable pond water'),
    'MB_ANIMATED_DOOR': (0x00000069, 'FR MB_WARP_DOOR'),
    'MB_JUMP_EAST': (0x00000038, 'east ledge'),
    'MB_MOUNTAIN_TOP': (0x0000000C, 'FR MB_MOUNTAIN_TOP'),
    'MB_OCEAN_WATER': (0x02000415, 'FR ocean water: surfable, water encounters'),
    'MB_POND_WATER': (0x02000410, 'FR pond water: surfable, water encounters'),
    # No FireRed counterpart: left as normal terrain (their cells keep the map's collision).
    'MB_BERRY_TREE_SOIL': (0, 'berry trees are left out of batch 1'),
    'MB_SECRET_BASE_WALL': (0, 'FireRed has no secret bases'),
    'MB_JUMP_SOUTHEAST': (0, 'source has no movement for diagonal jumps'),
    'MB_JUMP_SOUTHWEST': (0, 'source has no movement for diagonal jumps'),
}
LAYOUTS_2 = ['LAYOUT_HOENN_ROUTE104', 'LAYOUT_HOENN_PETALBURG_WOODS', 'LAYOUT_HOENN_RUSTBORO_CITY']
TABLES_2 = [
    (0, 'data/tilesets/primary/general/metatile_attributes.bin', 'data/tilesets/primary/hoenn_pilot/metatile_attributes.bin'),
    (640, 'data/tilesets/secondary/rustboro/metatile_attributes.bin', 'data/tilesets/secondary/hoenn_pilot_rustboro/metatile_attributes.bin'),
]
TABLES = [  # (offset, source table, installed table)
    (0, 'data/tilesets/primary/general/metatile_attributes.bin', 'data/tilesets/primary/hoenn_pilot/metatile_attributes.bin'),
    (640, 'data/tilesets/secondary/petalburg/metatile_attributes.bin', 'data/tilesets/secondary/hoenn_pilot/metatile_attributes.bin'),
]


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--batch', type=int, choices=(1, 2), default=1)
    p.add_argument('--report', type=Path)
    a = p.parse_args()
    layouts_used, tables = (LAYOUTS, TABLES) if a.batch == 1 else (LAYOUTS_2, TABLES_2)
    report_path = a.report or Path(f'docs/map-audits/hoenn-batch{a.batch}-attributes.json')
    names = behavior_names(a.source / 'include/constants/metatile_behaviors.h')
    layouts = {l['id']: l for l in json.loads((GAME / 'data/layouts/layouts.json').read_text())['layouts'] if 'id' in l}
    used = set()
    for lid in layouts_used:
        used |= {v & 0x3FF for (v,) in struct.iter_unpack('<H', (GAME / layouts[lid]['blockdata_filepath']).read_bytes())}
    report = []
    for offset, source, installed in tables:
        src = [v for (v,) in struct.iter_unpack('<H', (a.source / source).read_bytes())]
        path = GAME / installed
        if not path.exists():  # a new secondary (Rustboro): start from an all-normal table
            path.write_bytes(bytes(4 * len(src)))
        out = [v for (v,) in struct.iter_unpack('<I', path.read_bytes())]
        for index in sorted(m - offset for m in used if offset <= m < offset + len(out)):
            behavior, layer = src[index] & 0xFF, src[index] >> 12
            if out[index]:
                continue
            if not behavior:  # normal ground still needs its layer type (tree tops, roofs drawn over the player)
                if layer:
                    out[index] = layer << 29
                    report.append({'metatile': f'{index + offset:#05x}', 'source_behavior': 'MB_NORMAL',
                                   'attribute': f'{out[index]:#010x}', 'reason': 'layer type only'})
                continue
            name = names.get(behavior, str(behavior))
            if name not in RULES:
                raise ValueError(f'metatile {index + offset:#x}: behavior {name} needs a reviewed rule')
            value, reason = RULES[name]
            if value:
                out[index] = value | layer << 29
            report.append({'metatile': f'{index + offset:#05x}', 'source_behavior': name,
                           'attribute': f'{out[index]:#010x}', 'reason': reason})
        path.write_bytes(b''.join(struct.pack('<I', v) for v in out))
    report_path.write_text(json.dumps({'layouts': layouts_used, 'entries': report}, indent=2) + '\n', encoding='utf-8')
    print(f'{sum(1 for e in report if e["attribute"] != "0x00000000")} attributes set, '
          f'{sum(1 for e in report if e["attribute"] == "0x00000000")} left normal')


if __name__ == '__main__':
    main()
