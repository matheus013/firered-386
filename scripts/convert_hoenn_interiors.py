"""Stage graphics and attributes for the five Littleroot interiors (Building + BrendansMaysHouse/Lab).

Emerald secondary sheets hold up to 512 tiles; FireRed has room for 384 after its 640-tile
primary. Shifting by 128 would push used tiles past 1023, so the secondary sheets are packed:
only tiles referenced by audited metatiles are kept, in source order. Every remapped entry is
compared pixel by pixel (color and transparency, flips applied) against the Emerald source.
Unused metatiles are zeroed: the output only serves these five maps.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from PIL import Image
from convert_hoenn_graphics import palette, pad_sheet, rendered_entry
from normalize_region_layers import tile_pixels
from region_tilesets import behavior_names, convert_attributes, convert_entries, words

INTERIORS = {'LittlerootTown_BrendansHouse_1F', 'LittlerootTown_BrendansHouse_2F', 'LittlerootTown_MaysHouse_1F',
             'LittlerootTown_MaysHouse_2F', 'LittlerootTown_ProfessorBirchsLab'}
# Source folder and FireRed install folder for each Emerald tileset.
TILESETS = {
    'gTileset_Building': ('primary/building', 'primary/hoenn_pilot_building'),
    'gTileset_BrendansMaysHouse': ('secondary/brendans_mays_house', 'secondary/hoenn_pilot_brendans_mays_house'),
    'gTileset_Lab': ('secondary/lab', 'secondary/hoenn_pilot_lab'),
}
# FireRed behavior values (include/constants/metatile_behaviors.h). Layer bits are added later.
RULES = {
    'MB_NORMAL': (0x00, 'normal'),
    'MB_TELEVISION': (0x86, 'same name in FireRed'),
    'MB_REGION_MAP': (0x85, 'same name in FireRed; shows the FireRed region map'),
    'MB_SOUTH_ARROW_WARP': (0x65, 'same name in FireRed: doormat exit'),
    'MB_NON_ANIMATED_DOOR': (0x60, 'FR MB_CAVE_DOOR: the value FireRed MetatileBehavior_IsNonAnimDoor accepts (step-on warp, no animation)'),
    'MB_IMPASSABLE_SOUTH_AND_NORTH': (0xD2, 'added to FireRed as 0xD2 and to IsNorthBlocked/IsSouthBlocked, as in Emerald'),
    'MB_HOLDS_SMALL_DECORATION': (0x00, 'secret-base decoration slot; FireRed has no decorations, walkability comes from collision'),
    'MB_HOLDS_LARGE_DECORATION': (0x00, 'secret-base decoration slot; FireRed has no decorations, walkability comes from collision'),
}
SPLIT_SOURCE, SPLIT_TARGET, SECONDARY_ROOM = 512, 640, 384


def palettes(folder):
    return {i: palette(folder / f'palettes/{i:02}.pal') for i in range(16)}


def write_palettes(directory, colors):
    (directory / 'palettes').mkdir(parents=True)
    for i in range(16):
        (directory / 'palettes' / f'{i:02}.pal').write_text(
            'JASC-PAL\n0100\n16\n' + '\n'.join(' '.join(map(str, c)) for c in colors.get(i, [(0, 0, 0)] * 16)) + '\n')


def packed_sheet(source, tiles):
    """Copy the listed source tiles, in order, into a new 128-pixel-wide sheet."""
    rows = max(1, -(-len(tiles) // 16))
    sheet = Image.new('P', (128, rows * 8), 0)
    sheet.putpalette(source.getpalette())
    for slot, tile in enumerate(tiles):
        image = Image.new('P', (8, 8))
        image.putdata(tile_pixels(source, tile))
        sheet.paste(image, (slot % 16 * 8, slot // 16 * 8))
    return sheet


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True, help='pokeemerald checkout')
    p.add_argument('--audit', type=Path, required=True, help='docs/map-audits/hoenn-interior-tilesets.json')
    p.add_argument('--out', type=Path, required=True, help='new staging directory')
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    root = a.source / 'data/tilesets'
    names = behavior_names(a.source / 'include/constants/metatile_behaviors.h')
    used = {symbol: set() for symbol in TILESETS}
    pairs = {}
    for entry in json.loads(a.audit.read_text()):
        if entry['map'] not in INTERIORS:
            continue
        symbols = [s['symbol'] for s in entry['tilesets']]
        pairs[entry['map']] = symbols
        for s in entry['tilesets']:
            if s['symbol'] not in TILESETS:
                raise ValueError(f'Unexpected tileset {s["symbol"]}')
            used[s['symbol']].update(s['used_indices'])
    if set(pairs) != INTERIORS:
        raise ValueError('Audit does not cover exactly the five interiors')

    primary_src = Image.open(root / 'primary/building/tiles.png')
    primary_new = pad_sheet(primary_src, SPLIT_TARGET)
    primary_pals = palettes(root / 'primary/building')
    primary_meta = (root / 'primary/building/metatiles.bin').read_bytes()
    primary_attr = (root / 'primary/building/metatile_attributes.bin').read_bytes()
    a.out.mkdir(parents=True, exist_ok=False)
    report = {'status': 'INTERIOR_PILOT_STAGED_NOT_GENERAL', 'tilesets': {}, 'behavior_rules':
              {k: {'firered_behavior': hex(v), 'reason': r} for k, (v, r) in RULES.items()}}
    compared = 0

    def convert(symbol, raw_meta, raw_attr, slots, tile_map, palette_map, old_sheets, new_sheets, old_pals, new_pals):
        nonlocal compared
        meta, attr, behaviors = bytearray(slots * 16), bytearray(slots * 4), {}
        for index in sorted(used[symbol]):
            part = raw_meta[index * 16:index * 16 + 16]
            if len(part) != 16 or index >= slots:
                raise ValueError(f'{symbol}: metatile {index} outside source or target')
            converted = convert_entries(part, tile_map, palette_map)
            for before, after in zip(words(part), words(converted)):
                if rendered_entry(before, *old_sheets, SPLIT_SOURCE, old_pals) != rendered_entry(after, *new_sheets, SPLIT_TARGET, new_pals):
                    raise ValueError(f'{symbol}: pixel mismatch in metatile {index}')
                compared += 64
            meta[index * 16:index * 16 + 16] = converted
            source = struct.unpack_from('<H', raw_attr, index * 2)[0]
            name = names.get(source & 255)
            if name not in RULES:
                raise ValueError(f'{symbol}: behavior requires review: {name or source & 255}')
            attr[index * 4:index * 4 + 4] = convert_attributes(struct.pack('<H', source), {source & 255: RULES[name][0]})
            behaviors[name] = behaviors.get(name, 0) + 1
        return bytes(meta), bytes(attr), behaviors

    # Primary: tiles keep their numbers (512 fit in 640); palettes 0-5 keep theirs, slot 6 stays black.
    identity = {i: i for i in range(SPLIT_SOURCE)}
    primary_palette_map = {i: i for i in range(6)}
    new_primary_pals = {i: primary_pals[i] for i in range(6)}
    meta, attr, behaviors = convert('gTileset_Building', primary_meta, primary_attr, SPLIT_TARGET, identity, primary_palette_map,
                                    (primary_src, primary_src), (primary_new, primary_new), primary_pals, new_primary_pals)
    outputs = {'primary/hoenn_pilot_building': (primary_new, meta, attr, new_primary_pals)}
    report['tilesets']['gTileset_Building'] = {'metatiles_populated': len(used['gTileset_Building']), 'tiles': SPLIT_TARGET,
                                               'behaviors': behaviors, 'tile_partition': 'identity (512 in 640)', 'animation': 'none: TV animation tiles 496-499 unused by these metatiles'}

    for symbol in ('gTileset_BrendansMaysHouse', 'gTileset_Lab'):
        folder, target = TILESETS[symbol]
        sheet = Image.open(root / folder / 'tiles.png')
        raw_meta = (root / folder / 'metatiles.bin').read_bytes()
        raw_attr = (root / folder / 'metatile_attributes.bin').read_bytes()
        referenced = sorted({value & 1023 for index in used[symbol] for value in words(raw_meta[index * 16:index * 16 + 16])
                             if value & 1023 >= SPLIT_SOURCE})
        if len(referenced) > SECONDARY_ROOM:
            raise ValueError(f'{symbol}: {len(referenced)} tiles exceed the {SECONDARY_ROOM} FireRed secondary slots')
        packed = packed_sheet(sheet, [t - SPLIT_SOURCE for t in referenced])
        tile_map = dict(identity)
        tile_map.update({tile: SPLIT_TARGET + slot for slot, tile in enumerate(referenced)})
        source_pals = palettes(root / folder)
        old_pals = {**{i: primary_pals[i] for i in range(6)}, **{i: source_pals[i] for i in range(6, 13)}}
        palette_map = {**{i: i for i in range(6)}, **{i: i + 1 for i in range(6, 12)}}
        new_pals = {**new_primary_pals, **{i + 1: source_pals[i] for i in range(6, 12)}}
        slots = len(raw_meta) // 16
        meta, attr, behaviors = convert(symbol, raw_meta, raw_attr, slots, tile_map, palette_map,
                                        (primary_src, sheet), (primary_new, packed), old_pals, new_pals)
        outputs[target] = (packed, meta, attr, {i + 1: source_pals[i] for i in range(6, 12)})
        report['tilesets'][symbol] = {'metatiles_populated': len(used[symbol]), 'metatile_slots': slots,
                                      'source_tiles': sheet.width * sheet.height // 64, 'packed_tiles': len(referenced),
                                      'behaviors': behaviors, 'tile_partition': f'packed {SPLIT_SOURCE}+n -> {SPLIT_TARGET}+rank',
                                      'palette_partition': '6..11 -> 7..12', 'animation': 'none (NULL in Emerald)'}

    for target, (sheet, meta, attr, colors) in outputs.items():
        directory = a.out / target
        write_palettes(directory, colors)
        sheet.save(directory / 'tiles.png')
        (directory / 'metatiles.bin').write_bytes(meta)
        (directory / 'metatile_attributes.bin').write_bytes(attr)
        report['tilesets'][next(s for s, f in TILESETS.items() if f[1] == target)]['sha256'] = {
            name: hashlib.sha256((directory / name).read_bytes()).hexdigest()
            for name in ('tiles.png', 'metatiles.bin', 'metatile_attributes.bin')}
    report['map_tilesets'] = pairs
    report['rgba_pixel_comparisons'] = compared
    report['pixel_mismatches'] = 0
    report['limits'] = ('Static comparison against Emerald source sheets; not a capture of either engine. '
                        'Zeroed unused metatiles: do not reuse outside these five maps.')
    a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v.get('packed_tiles') or v.get('tiles') for k, v in report['tilesets'].items()}), compared, 'pixels compared')


if __name__ == '__main__':
    main()
