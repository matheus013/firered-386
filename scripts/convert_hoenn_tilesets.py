"""Convert every metatile of an Emerald General + secondary tileset pair to FireRed's layout.

convert_hoenn_graphics.py only filled the metatiles that Littleroot and Route 101 use and left the rest
zeroed, so later Hoenn maps drew blank (grey/black) squares wherever they used another metatile. This
converts the whole tables with the same rules and the same pixel check:

- tiles: primary 0-511 stay put; secondary tiles move after FireRed's 640 primary tiles. A secondary
  sheet that fits FireRed's 384 slots keeps its order (+128); a larger one (Rustboro: 512 tiles, 276 of
  them referenced) is compacted to the tiles its metatiles reference, in source order;
- palettes: primary 0-5 stay, secondary 6-11 move to 7-12, slot 6 is unused (black), like the pilot;
- metatiles: every entry is remapped; lower quadrants with an invalid tile/palette that an opaque upper
  tile hides are cleared (normalize_region_layers.normalize). A metatile that cannot be normalized is left
  empty and listed in the report;
- every converted quadrant is rendered before and after and compared pixel by pixel.

Attributes are not touched (convert_hoenn_attributes.py / convert_hoenn_batch1_attributes.py and the
batch installers convert the ones the installed maps use). Tile animations are not converted.

Run from the repository root, e.g.:
  python scripts/convert_hoenn_tilesets.py --source ../map-references/pokeemerald --secondary petalburg \
      --primary-out games/super-fire-red/data/tilesets/primary/hoenn_pilot \
      --secondary-out games/super-fire-red/data/tilesets/secondary/hoenn_pilot --report docs/map-audits/hoenn-tilesets-petalburg.json
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from PIL import Image

from convert_hoenn_graphics import palette, rendered_entry, pad_sheet
from normalize_region_layers import normalize
from region_tilesets import convert_entries, words

SPLIT, FR_PRIMARY, FR_SECONDARY = 512, 640, 384


def write_palettes(folder, palettes):
    (folder / 'palettes').mkdir(parents=True, exist_ok=True)
    for i in range(13):
        (folder / 'palettes' / f'{i:02}.pal').write_text(
            'JASC-PAL\n0100\n16\n' + '\n'.join(' '.join(map(str, c)) for c in palettes[i]) + '\n', newline='\n')


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument('--source', type=Path, required=True, help='pokeemerald checkout')
    p.add_argument('--secondary', required=True, help='Emerald secondary tileset folder name, e.g. petalburg')
    p.add_argument('--primary-out', type=Path, required=True)
    p.add_argument('--secondary-out', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    p.add_argument('--maps', nargs='*', default=[],
                   help='Emerald maps using this pair: when the referenced tiles do not fit, keep only the tiles of the '
                        'metatiles these maps use (other metatiles stay empty; test_region_metatiles.py guards it)')
    a = p.parse_args()
    base = a.source / 'data/tilesets'
    sec = base / 'secondary' / a.secondary
    primary_img = Image.open(base / 'primary/general/tiles.png')
    secondary_img = Image.open(sec / 'tiles.png')
    sec_count = secondary_img.width * secondary_img.height // 64
    sec_raw = (sec / 'metatiles.bin').read_bytes()
    sec_attrs = (sec / 'metatile_attributes.bin').read_bytes()

    # Secondary tile order: unchanged if it fits, else only the referenced tiles, in source order.
    referenced = sorted({v & 1023 for v in words(sec_raw) if SPLIT <= (v & 1023) < SPLIT + sec_count})
    limited_to = None
    used = set()
    if a.maps:
        layouts = {l['id']: l for l in json.loads((a.source / 'data/layouts/layouts.json').read_text())['layouts'] if 'id' in l}
        for m in a.maps:
            layout = layouts[json.loads((a.source / f'data/maps/{m}/map.json').read_text())['layout']]
            for f in ('blockdata_filepath', 'border_filepath'):
                used |= {v & 1023 for (v,) in struct.iter_unpack('<H', (a.source / layout[f]).read_bytes()) if (v & 1023) >= SPLIT}
    # Secondary metatiles: FireRed has 384 ids (640-1023). A larger table keeps only the listed maps' metatiles,
    # in source order; the report's metatile_order lets the installers remap blocks and attributes.
    sec_metatiles = len(sec_raw) // 16
    mt_order = None
    if sec_metatiles > FR_SECONDARY:
        if not used:
            raise ValueError(f'{a.secondary}: {sec_metatiles} metatiles do not fit; pass --maps')
        mt_order = sorted(m - SPLIT for m in used)
    if len(referenced) > FR_SECONDARY and a.maps:
        entries = words(sec_raw)
        referenced = sorted({entries[(m - SPLIT) * 8 + q] & 1023 for m in used if (m - SPLIT) * 8 < len(entries) for q in range(8)
                             if SPLIT <= (entries[(m - SPLIT) * 8 + q] & 1023) < SPLIT + sec_count})
        limited_to = a.maps
    compact = sec_count > FR_SECONDARY
    order = [t - SPLIT for t in referenced] if compact else list(range(sec_count))
    if len(order) > FR_SECONDARY:
        raise ValueError(f'{a.secondary}: {len(order)} secondary tiles do not fit in {FR_SECONDARY}')
    tile_map = {i: i for i in range(SPLIT)}
    tile_map.update({SPLIT + old: FR_PRIMARY + new for new, old in enumerate(order)})
    palette_map = {i: i if i < 6 else i + 1 for i in range(12)}

    dest_primary = pad_sheet(primary_img, FR_PRIMARY)
    if compact:
        dest_secondary = Image.new('P', (128, (len(order) + 15) // 16 * 8), 0)
        dest_secondary.putpalette(secondary_img.getpalette())
        for new, old in enumerate(order):
            box = ((old % 16) * 8, (old // 16) * 8, (old % 16) * 8 + 8, (old // 16) * 8 + 8)
            dest_secondary.paste(secondary_img.crop(box), ((new % 16) * 8, (new // 16) * 8))
    else:
        dest_secondary = pad_sheet(secondary_img, len(order) // 16 * 16 if len(order) % 16 == 0 else (len(order) // 16 + 1) * 16)
    old_palettes = {i: palette((base / 'primary/general' if i < 6 else sec) / f'palettes/{i:02}.pal') for i in range(12)}
    new_palettes = {palette_map[i]: c for i, c in old_palettes.items()}
    new_palettes[6] = [(0, 0, 0)] * 16

    report = {'secondary': a.secondary, 'secondary_tiles': {'source': sec_count, 'installed': len(order), 'compacted': compact},
              'empty_metatiles': {}, 'normalized_quadrants': 0, 'rgba_pixel_comparisons': 0}
    outputs = {}
    for name, raw, attrs, slots in (('primary', (base / 'primary/general/metatiles.bin').read_bytes(),
                                     (base / 'primary/general/metatile_attributes.bin').read_bytes(), FR_PRIMARY),
                                    ('secondary', sec_raw, sec_attrs, len(mt_order) if mt_order else len(sec_raw) // 16)):
        count = len(raw) // 16
        dest, empty = bytearray(slots * 16), []
        sources = mt_order if (name == 'secondary' and mt_order) else range(count)
        for slot, index in enumerate(sources):
            part = raw[index * 16:index * 16 + 16]
            try:
                fixed, proof = normalize(part, attrs[index * 2:index * 2 + 2], [0], primary_img, secondary_img, SPLIT)
                if any((v & 1023) >= SPLIT + sec_count for v in words(fixed) if v):
                    raise ValueError('tile outside the secondary sheet')
                if any(SPLIT <= (v & 1023) and (v & 1023) not in tile_map for v in words(fixed)):
                    raise ValueError('uses a tile left out to fit FireRed (not used by the listed maps)')
                converted = convert_entries(fixed, tile_map, palette_map)
            except (ValueError, KeyError) as e:
                empty.append({'metatile': index + (0 if name == 'primary' else FR_PRIMARY), 'reason': str(e)})
                continue
            for before, after in zip(words(fixed), words(converted)):
                if rendered_entry(before, primary_img, secondary_img, SPLIT, old_palettes) != \
                   rendered_entry(after, dest_primary, dest_secondary, FR_PRIMARY, new_palettes):
                    raise ValueError(f'{name} metatile {index}: pixel mismatch after remapping')
                report['rgba_pixel_comparisons'] += 64
            report['normalized_quadrants'] += len(proof)
            dest[slot * 16:slot * 16 + 16] = converted
        outputs[name] = bytes(dest)
        report['empty_metatiles'][name] = empty

    a.primary_out.mkdir(parents=True, exist_ok=True)
    a.secondary_out.mkdir(parents=True, exist_ok=True)
    (a.primary_out / 'metatiles.bin').write_bytes(outputs['primary'])
    (a.secondary_out / 'metatiles.bin').write_bytes(outputs['secondary'])
    if compact or not (a.secondary_out / 'tiles.png').exists():
        dest_secondary.save(a.secondary_out / 'tiles.png')
        write_palettes(a.secondary_out, new_palettes)
    if not (a.primary_out / 'tiles.png').exists():
        dest_primary.save(a.primary_out / 'tiles.png')
        write_palettes(a.primary_out, new_palettes)
    report['tile_order'] = order if compact else 'unchanged (+128)'
    report['tiles_limited_to_maps'] = limited_to
    report['metatile_order'] = mt_order
    report['sha256'] = {f'{k}/metatiles.bin': hashlib.sha256(v).hexdigest() for k, v in outputs.items()}
    a.report.write_text(json.dumps(report, indent=1) + '\n', encoding='utf-8')
    print(json.dumps({k: (len(v) if isinstance(v, (list, dict)) else v) for k, v in report.items() if k not in ('tile_order', 'metatile_order')}))


if __name__ == '__main__':
    main()
