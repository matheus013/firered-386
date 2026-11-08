"""Install the five Littleroot interiors into Super FireRed and wire their warps. Idempotent.

Inputs: staged tilesets from convert_hoenn_interiors.py, staged blocks from prepare_region_pilot.py
(.build/region-pilots/hoenn-interiors/<map>/) and the pokeemerald map headers. Terrain and warps
only: Emerald object events, scripts and coord events are not imported.
"""
import argparse
import json
from pathlib import Path
import shutil

INTERIORS = ['LittlerootTown_BrendansHouse_1F', 'LittlerootTown_BrendansHouse_2F', 'LittlerootTown_MaysHouse_1F',
             'LittlerootTown_MaysHouse_2F', 'LittlerootTown_ProfessorBirchsLab']
WARPED = ['LittlerootTown'] + INTERIORS
# Emerald songs missing from FireRed, replaced by the FireRed equivalent.
MUSIC = {'MUS_LITTLEROOT': 'MUS_PALLET', 'MUS_BIRCH_LAB': 'MUS_OAK_LAB'}
TILESETS = {  # Emerald symbol -> (FireRed symbol, install folder, primary?)
    'gTileset_Building': ('HoennPilotBuilding', 'primary/hoenn_pilot_building', True),
    'gTileset_BrendansMaysHouse': ('HoennPilotBrendansMaysHouse', 'secondary/hoenn_pilot_brendans_mays_house', False),
    'gTileset_Lab': ('HoennPilotLab', 'secondary/hoenn_pilot_lab', False),
}
MARK = '// Hoenn pilot interiors (scripts/install_hoenn_interiors.py)'


def hoenn_map(constant):
    if not constant.startswith('MAP_LITTLEROOT_TOWN'):
        raise ValueError(f'Warp leaves the pilot: {constant}')
    return 'MAP_HOENN_' + constant[4:]


def append_once(path, text):
    content = path.read_text(encoding='utf-8')
    if MARK not in content:
        path.write_text(content.rstrip('\n') + '\n\n' + MARK + '\n' + text, encoding='utf-8', newline='\n')


def c_sources():
    graphics, metatiles, headers = [], [], []
    for name, folder, primary in TILESETS.values():
        base = f'data/tilesets/{folder}'
        graphics.append(f'const u32 gTilesetTiles_{name}[] = INCBIN_U32("{base}/tiles.4bpp.lz");\n'
                        f'const u16 gTilesetPalettes_{name}[][16] = {{\n'
                        + ''.join(f'    INCBIN_U16("{base}/palettes/{i:02}.gbapal"),\n' for i in range(16)) + '};\n')
        metatiles.append(f'const u16 gMetatiles_{name}[] = INCBIN_U16("{base}/metatiles.bin");\n'
                         f'const u32 gMetatileAttributes_{name}[] = INCBIN_U32("{base}/metatile_attributes.bin");\n')
        headers.append(f'const struct Tileset gTileset_{name} =\n{{\n    .isCompressed = TRUE,\n'
                       f'    .isSecondary = {"FALSE" if primary else "TRUE"},\n    .tiles = gTilesetTiles_{name},\n'
                       f'    .palettes = gTilesetPalettes_{name},\n    .metatiles = gMetatiles_{name},\n'
                       f'    .metatileAttributes = gMetatileAttributes_{name},\n    .callback = NULL,\n}};\n')
    return '\n'.join(graphics), '\n'.join(metatiles), '\n'.join(headers)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--emerald', type=Path, required=True)
    p.add_argument('--staging', type=Path, required=True, help='.build/region-pilots')
    p.add_argument('--game', type=Path, required=True, help='games/super-fire-red')
    a = p.parse_args()
    g = a.game
    for _, folder, _ in TILESETS.values():
        target = g / 'data/tilesets' / folder
        if target.exists():
            shutil.rmtree(target)
        shutil.copytree(a.staging / 'hoenn-interior-graphics' / folder, target)
    graphics, metatiles, headers = c_sources()
    append_once(g / 'src/data/tilesets/graphics.h', graphics)
    append_once(g / 'src/data/tilesets/metatiles.h', metatiles)
    append_once(g / 'src/data/tilesets/headers.h', headers)

    emerald_layouts = {l['id']: l for l in json.loads((a.emerald / 'data/layouts/layouts.json').read_text())['layouts'] if 'id' in l}
    layouts_path = g / 'data/layouts/layouts.json'
    layouts = json.loads(layouts_path.read_text(encoding='utf-8'))
    known = {l.get('id') for l in layouts['layouts']}
    groups_path = g / 'data/maps/map_groups.json'
    groups = json.loads(groups_path.read_text(encoding='utf-8'))
    scripts_path = g / 'data/event_scripts.s'
    scripts = scripts_path.read_text(encoding='utf-8')
    for source in WARPED:
        header = json.loads((a.emerald / f'data/maps/{source}/map.json').read_text())
        name = 'Hoenn_' + source
        warps = [{'x': w['x'], 'y': w['y'], 'elevation': w['elevation'], 'dest_map': hoenn_map(w['dest_map']),
                  'dest_warp_id': w['dest_warp_id']} for w in header['warp_events']]
        map_path = g / f'data/maps/{name}/map.json'
        if source == 'LittlerootTown':  # already installed: only add its door warps
            current = json.loads(map_path.read_text(encoding='utf-8'))
            current['warp_events'] = warps
            map_path.write_text(json.dumps(current, indent=2) + '\n', encoding='utf-8', newline='\n')
            continue
        layout = emerald_layouts[header['layout']]
        layout_id = 'LAYOUT_HOENN_' + header['layout'][len('LAYOUT_'):]
        (g / f'data/layouts/{name}').mkdir(parents=True, exist_ok=True)
        for f in ('map.bin', 'border.bin'):
            shutil.copyfile(a.staging / 'hoenn-interiors' / source / f, g / f'data/layouts/{name}/{f}')
        if layout_id not in known:
            layouts['layouts'].append({
                'id': layout_id, 'name': name + '_Layout', 'width': layout['width'], 'height': layout['height'],
                'border_width': 2, 'border_height': 2,
                'primary_tileset': 'gTileset_' + TILESETS[layout['primary_tileset']][0],
                'secondary_tileset': 'gTileset_' + TILESETS[layout['secondary_tileset']][0],
                'border_filepath': f'data/layouts/{name}/border.bin', 'blockdata_filepath': f'data/layouts/{name}/map.bin'})
        out = {'id': 'MAP_HOENN_' + header['id'][4:], 'name': name, 'layout': layout_id,
               'music': MUSIC.get(header['music'], header['music']), 'region_map_section': header['region_map_section'],
               'requires_flash': header['requires_flash'], 'weather': header['weather'], 'map_type': header['map_type'],
               'allow_cycling': header['allow_cycling'], 'allow_escaping': header['allow_escaping'],
               'allow_running': header['allow_running'], 'show_map_name': False, 'floor_number': 0,
               'battle_scene': header['battle_scene'], 'connections': None,
               'object_events': [], 'warp_events': warps, 'coord_events': [], 'bg_events': []}
        # Events come from install_pilot_events.py; a rerun of this installer keeps them.
        if (map_path).exists():
            previous = json.loads((map_path).read_text(encoding='utf-8'))
            for key in ('object_events', 'coord_events', 'bg_events'):
                out[key] = previous.get(key) or []
        map_path.parent.mkdir(parents=True, exist_ok=True)
        map_path.write_text(json.dumps(out, indent=2) + '\n', encoding='utf-8', newline='\n')
        if not (map_path.parent / 'scripts.inc').exists(): (map_path.parent / 'scripts.inc').write_text(
            f'@ Terrain-only pilot. Emerald story events are not imported.\n{name}_MapScripts::\n\t.byte 0\n',
            encoding='utf-8', newline='\n')
        if name not in groups['gMapGroup_Hoenn']:
            groups['gMapGroup_Hoenn'].append(name)
        line = f'\t.include "data/maps/{name}/scripts.inc"\n'
        if line not in scripts:
            anchor = '\t.include "data/maps/Hoenn_Route101/scripts.inc"\n'
            last = max((l for l in scripts.splitlines(True) if l.startswith('\t.include "data/maps/Hoenn_') and l.endswith('scripts.inc"\n')),
                       key=scripts.index, default=anchor)
            scripts = scripts.replace(last, last + line, 1)
    layouts_path.write_text(json.dumps(layouts, indent=2) + '\n', encoding='utf-8', newline='\n')
    groups_path.write_text(json.dumps(groups, indent=2) + '\n', encoding='utf-8', newline='\n')
    scripts_path.write_text(scripts, encoding='utf-8', newline='\n')
    print('gMapGroup_Hoenn =', groups['gMapGroup_Hoenn'])


if __name__ == '__main__':
    main()
