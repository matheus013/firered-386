"""Install the first Hoenn geography batch: Oldale Town, Route 102, Route 103 and Petalburg City.

Terrain comes from the staged blocks of scripts/prepare_region_pilot.py (Emerald's 512-metatile split
remapped to FireRed's 640) and uses the already converted pilot tilesets (General + Petalburg).
Pokemon Centers and Marts are FireRed's own (LAYOUT_POKEMON_CENTER_1F / LAYOUT_MART) with a nurse and
a clerk selling Emerald's expanded list; their upstairs link rooms are not part of the batch. Houses,
Wally's house and the Petalburg Gym keep their doors closed until their tilesets are converted; the
Route 103 warp to Altering Cave is dropped (FireRed's Altering Cave is on Six Island). Connections to
maps outside the installed set are left out. Idempotent.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from PIL import Image

EXTERIORS = {
    # map: (FireRed name, music, kept warps (door x, y, interior))
    'OldaleTown': ('Hoenn_OldaleTown', 'MUS_PEWTER', [(6, 16, 'PokemonCenter_1F'), (14, 6, 'Mart')]),
    'Route102': ('Hoenn_Route102', 'MUS_ROUTE3', []),
    'Route103': ('Hoenn_Route103', 'MUS_ROUTE3', []),
    'PetalburgCity': ('Hoenn_PetalburgCity', 'MUS_CELADON', [(20, 16, 'PokemonCenter_1F'), (25, 12, 'Mart')]),
}
SECTION_NAMES = {'MAPSEC_OLDALE_TOWN': 'OLDALE TOWN', 'MAPSEC_ROUTE_102': 'ROUTE 102',
                 'MAPSEC_ROUTE_103': 'ROUTE 103', 'MAPSEC_PETALBURG_CITY': 'PETALBURG CITY'}
# Door animations for the two FireRed-backed buildings (Emerald graphics/door_anims, palettes from
# Emerald's field_door.c; primary palettes 0-5 keep their numbers).
DOORS = [('poke_center', 'PokeCenter', 0x061, 'DOOR_SOUND_SLIDING', [1] * 8),
         ('poke_mart', 'PokeMart', 0x041, 'DOOR_SOUND_SLIDING', [0, 0, 1, 1, 1, 1, 1, 1])]


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--references', type=Path, required=True)
    p.add_argument('--staged', type=Path, required=True, help='prepare_region_pilot.py output folder')
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    g, em = a.game, a.references / 'pokeemerald'
    report = {'maps': {}, 'doors': [], 'closed_doors': {}}

    # --- door animations ---------------------------------------------------------------------------
    doors_h = g / 'src/data/hoenn_pilot_doors.h'
    text = doors_h.read_text(encoding='utf-8')
    for name, symbol, metatile, sound, pals in DOORS:
        src = Image.open(em / 'graphics/door_anims' / f'{name}.png')
        assert src.size == (16, 96) and src.mode == 'P', name
        out = Image.new('P', src.size)
        out.putpalette(src.getpalette()[:48])
        out.putdata([v & 15 for v in src.tobytes()])  # upstream gbagfx keeps the low nibble
        out.save(g / f'graphics/door_anims/hoenn/{name}.png')
        decl = (f'static const u8 sHoennDoorTiles_{symbol}[] = INCBIN_U8("graphics/door_anims/hoenn/{name}.4bpp");\n'
                f'static const u8 sHoennDoorPalettes_{symbol}[] = {{{", ".join(map(str, pals))}}};\n')
        row = f'    {{0x{metatile:03X}, {sound}, DOOR_SIZE_1x2, sHoennDoorTiles_{symbol}, sHoennDoorPalettes_{symbol}}},\n'
        if decl not in text:
            text = text.replace('static const struct DoorGraphics sHoennPilotDoors[] = {\n', decl + 'static const struct DoorGraphics sHoennPilotDoors[] = {\n')
            text = text.replace('    {}\n};', row + '    {}\n};')
        report['doors'].append({'door': name, 'metatile': hex(metatile), 'palettes': pals, 'source': f'pokeemerald graphics/door_anims/{name}.png'})
    doors_h.write_text(text, encoding='utf-8', newline='\n')

    layouts_path = g / 'data/layouts/layouts.json'
    layouts = json.loads(layouts_path.read_text(encoding='utf-8'))
    groups_path = g / 'data/maps/map_groups.json'
    groups = json.loads(groups_path.read_text(encoding='utf-8'))
    staged = json.loads((a.staged.parent / (a.staged.name + '.json')).read_text(encoding='utf-8'))
    staged_maps = {m['map']: m for m in staged['maps']}

    def add_layout(lid, name, w, h, folder):
        entry = {'id': lid, 'name': f'{name}_Layout', 'width': w, 'height': h, 'border_width': 2, 'border_height': 2,
                 'primary_tileset': 'gTileset_HoennPilotGeneral', 'secondary_tileset': 'gTileset_HoennPilotPetalburg',
                 'border_filepath': f'data/layouts/{folder}/border.bin', 'blockdata_filepath': f'data/layouts/{folder}/map.bin'}
        if not any(l.get('id') == lid for l in layouts['layouts']):
            layouts['layouts'].append(entry)

    def add_to_group(name):
        if name not in groups['gMapGroup_Hoenn']:
            groups['gMapGroup_Hoenn'].append(name)

    new_maps = []
    for src, (name, music, kept) in EXTERIORS.items():
        sm = staged_maps[src]
        folder = g / 'data/layouts' / name
        folder.mkdir(parents=True, exist_ok=True)
        for f in ('map.bin', 'border.bin'):
            shutil.copyfile(a.staged / src / f, folder / f)
        lid = 'LAYOUT_HOENN_' + re.sub(r'(?<=[a-z])(?=[A-Z])', '_', src).upper()
        add_layout(lid, name, sm['width'], sm['height'], name)
        emh = json.loads((em / f'data/maps/{src}/map.json').read_text())
        mid = 'MAP_HOENN_' + re.sub(r'(?<=[a-z])(?=[A-Z])', '_', src).upper()
        warps = []
        closed = []
        for w in emh.get('warp_events') or []:
            keep = next((k for k in kept if (k[0], k[1]) == (w['x'], w['y'])), None)
            if keep:
                warps.append({'x': w['x'], 'y': w['y'], 'elevation': w['elevation'],
                              'dest_map': f'{mid}_{re.sub(r"(?<=[a-z])(?=[A-Z])", "_", keep[2]).upper()}', 'dest_warp_id': '1'})  # middle of the doormat, like FireRed
            else:
                closed.append(w['dest_map'])
        report['closed_doors'][name] = closed
        header = {'id': mid, 'name': name, 'layout': lid, 'music': music, 'region_map_section': emh['region_map_section'],
                  'requires_flash': False, 'weather': emh['weather'], 'map_type': emh['map_type'], 'allow_cycling': True,
                  'allow_escaping': False, 'allow_running': True, 'show_map_name': True, 'battle_scene': 'MAP_BATTLE_SCENE_NORMAL',
                  'connections': [], 'object_events': [], 'warp_events': warps, 'coord_events': [], 'bg_events': [], 'floor_number': 0}
        mdir = g / 'data/maps' / name
        mdir.mkdir(parents=True, exist_ok=True)
        path = mdir / 'map.json'
        if path.exists():  # keep events written by later installers
            old = json.loads(path.read_text(encoding='utf-8'))
            for k in ('object_events', 'bg_events', 'coord_events'):
                header[k] = old.get(k, [])
        header['_emerald_connections'] = emh.get('connections') or []
        new_maps.append((src, name, mid, header))
        if not (mdir / 'scripts.inc').exists():
            (mdir / 'scripts.inc').write_text(f'@ Hoenn batch 1 (scripts/install_hoenn_batch1.py).\n{name}_MapScripts::\n\t.byte 0\n', encoding='utf-8', newline='\n')
        add_to_group(name)
        report['maps'][name] = {'source': f'pokeemerald data/maps/{src}', 'layout': lid, 'size': [sm['width'], sm['height']],
                                'warps_kept': [k[2] for k in kept], 'music': music}
    # Connections between installed maps (both directions are listed in each Emerald header).
    ids = {json.loads((em / f'data/maps/{src}/map.json').read_text())['id']: mid for src, _n, mid, _h in new_maps}
    ids['MAP_LITTLEROOT_TOWN'] = 'MAP_HOENN_LITTLEROOT_TOWN'
    ids['MAP_ROUTE101'] = 'MAP_HOENN_ROUTE101'
    for src, name, mid, header in new_maps:
        header['connections'] = [{'map': ids[c['map']], 'offset': c['offset'], 'direction': c['direction']}
                                 for c in header.pop('_emerald_connections') if c['map'] in ids]
        (g / 'data/maps' / name / 'map.json').write_text(json.dumps(header, indent=2) + '\n', encoding='utf-8', newline='\n')
        report['maps'][name]['connections'] = [c['map'] for c in header['connections']]
        if name not in groups['connections_include_order']:
            groups['connections_include_order'].append(name)
    # Route 101 now continues north into Oldale.
    r101 = g / 'data/maps/Hoenn_Route101/map.json'
    h = json.loads(r101.read_text(encoding='utf-8'))
    if not any(c['map'] == 'MAP_HOENN_OLDALE_TOWN' for c in h['connections']):
        h['connections'].append({'map': 'MAP_HOENN_OLDALE_TOWN', 'offset': 0, 'direction': 'up'})
        r101.write_text(json.dumps(h, indent=2) + '\n', encoding='utf-8', newline='\n')

    # --- Pokemon Centers and Marts (FireRed layouts) ------------------------------------------------
    marts = {}
    for town in ('OldaleTown', 'PetalburgCity'):
        body = (em / f'data/maps/{town}_Mart/scripts.inc').read_text(encoding='utf-8')
        block = body.split(f'{town}_Mart_Pokemart_Expanded:')[1].split('pokemartlistend')[0]
        marts[town] = re.findall(r'\.2byte (ITEM_\w+)', block)
    for town, heal in (('OldaleTown', 'HEAL_LOCATION_OLDALE_TOWN'), ('PetalburgCity', 'HEAL_LOCATION_PETALBURG_CITY')):
        ext = 'Hoenn_' + town
        ext_id = 'MAP_HOENN_' + re.sub(r'(?<=[a-z])(?=[A-Z])', '_', town).upper()
        section = json.loads((em / f'data/maps/{town}/map.json').read_text())['region_map_section']
        for kind, layout, warp_cells, door_index in (('PokemonCenter_1F', 'LAYOUT_POKEMON_CENTER_1F', [(6, 8), (7, 8), (8, 8)], 0),
                                                     ('Mart', 'LAYOUT_MART', [(3, 7), (4, 7), (5, 7)], 1)):
            name = f'{ext}_{kind}'
            mid = f'{ext_id}_{re.sub(r"(?<=[a-z])(?=[A-Z])", "_", kind).upper()}'  # PokemonCenter_1F -> POKEMON_CENTER_1F
            npc = ('OBJ_EVENT_GFX_NURSE', 7, 2, f'{name}_EventScript_Nurse') if kind.startswith('Pokemon') else ('OBJ_EVENT_GFX_CLERK', 2, 3, f'{name}_EventScript_Clerk')
            header = {'id': mid, 'name': name, 'layout': layout, 'music': 'MUS_POKE_CENTER', 'region_map_section': section,
                      'requires_flash': False, 'weather': 'WEATHER_NONE', 'map_type': 'MAP_TYPE_INDOOR', 'allow_cycling': False,
                      'allow_escaping': False, 'allow_running': False, 'show_map_name': False, 'floor_number': 0,
                      'battle_scene': 'MAP_BATTLE_SCENE_NORMAL', 'connections': None,
                      'object_events': [{'type': 'object', 'graphics_id': npc[0], 'x': npc[1], 'y': npc[2], 'elevation': 3,
                                         'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
                                         'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0', 'script': npc[3], 'flag': '0'}],
                      'warp_events': [{'x': x, 'y': y, 'elevation': 3, 'dest_map': ext_id, 'dest_warp_id': str(door_index)} for x, y in warp_cells],
                      'coord_events': [], 'bg_events': []}
            mdir = g / 'data/maps' / name
            mdir.mkdir(parents=True, exist_ok=True)
            (mdir / 'map.json').write_text(json.dumps(header, indent=2) + '\n', encoding='utf-8', newline='\n')
            if kind.startswith('Pokemon'):
                script = (f'@ Hoenn batch 1: FireRed Pokemon Center layout (scripts/install_hoenn_batch1.py).\n{name}_MapScripts::\n'
                          f'\tmap_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition\n\tmap_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume\n\t.byte 0\n\n'
                          f'{name}_OnTransition::\n\tsetrespawn {heal}\n\tend\n\n'
                          f'{name}_EventScript_Nurse::\n\tlock\n\tfaceplayer\n\tcall EventScript_PkmnCenterNurse\n\trelease\n\tend\n')
            else:
                items = ''.join(f'\t.2byte {i}\n' for i in marts[town])
                script = (f'@ Hoenn batch 1: FireRed Mart layout; pokeemerald {town}_Mart expanded list.\n{name}_MapScripts::\n\t.byte 0\n\n'
                          f'{name}_EventScript_Clerk::\n\tgoto_if_questlog EventScript_ReleaseEnd\n\tlock\n\tfaceplayer\n\tmessage Text_MayIHelpYou\n\twaitmessage\n'
                          f'\tpokemart {name}_Items\n\tmsgbox Text_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n{name}_Items::\n{items}\t.2byte ITEM_NONE\n\trelease\n\tend\n')
            (mdir / 'scripts.inc').write_text(script, encoding='utf-8', newline='\n')
            add_to_group(name)
            report['maps'][name] = {'layout': layout, 'npc': npc[0], **({'items': marts[town]} if kind == 'Mart' else {'heal_location': heal})}

    # --- section names, heal locations ---------------------------------------------------------------
    secs_path = g / 'src/data/region_map/region_map_sections.json'
    secs = json.loads(secs_path.read_text(encoding='utf-8'))
    for e in secs['map_sections']:
        if e['id'] in SECTION_NAMES:
            e['hoenn_name'] = SECTION_NAMES[e['id']]
    secs_path.write_text(json.dumps(secs, indent=2, ensure_ascii=False) + '\n', encoding='utf-8', newline='\n')
    heal_path = g / 'src/data/heal_locations.json'
    heals = json.loads(heal_path.read_text(encoding='utf-8'))
    have = {h['id'] for h in heals['heal_locations']}
    for hid, mid, x, y, respawn in (('HEAL_LOCATION_OLDALE_TOWN', 'MAP_HOENN_OLDALE_TOWN', 6, 17, 'MAP_HOENN_OLDALE_TOWN_POKEMON_CENTER_1F'),
                                    ('HEAL_LOCATION_PETALBURG_CITY', 'MAP_HOENN_PETALBURG_CITY', 20, 17, 'MAP_HOENN_PETALBURG_CITY_POKEMON_CENTER_1F')):
        if hid not in have:
            heals['heal_locations'].append({'id': hid, 'map': mid, 'x': x, 'y': y, 'respawn_map': respawn, 'respawn_npc': 1})
    heal_path.write_text(json.dumps(heals, indent=2, ensure_ascii=False) + '\n', encoding='utf-8', newline='\n')

    layouts_path.write_text(json.dumps(layouts, indent=2) + '\n', encoding='utf-8', newline='\n')
    groups_path.write_text(json.dumps(groups, indent=2) + '\n', encoding='utf-8', newline='\n')
    # event_scripts.s includes
    es = g / 'data/event_scripts.s'
    t = es.read_text(encoding='utf-8')
    anchor = '\t.include "data/maps/Hoenn_LittlerootTown_ProfessorBirchsLab/scripts.inc"\n'
    assert anchor in t
    add = ''.join(f'\t.include "data/maps/{n}/scripts.inc"\n' for n in groups['gMapGroup_Hoenn'] if f'data/maps/{n}/scripts.inc' not in t)
    t = t.replace(anchor, anchor + add)
    es.write_text(t, encoding='utf-8', newline='\n')
    a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: len(v) for k, v in report.items()}))


if __name__ == '__main__':
    main()
