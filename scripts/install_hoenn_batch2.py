"""Install the second Hoenn geography batch: Route 104, Petalburg Woods and Rustboro City. Idempotent.

Run after scripts/convert_hoenn_tilesets.py (General + Rustboro; also fills every General/Petalburg
metatile) and scripts/prepare_region_pilot.py --maps Route104 PetalburgWoods RustboroCity
--out .build/region-pilots/hoenn-batch2. The three maps use Emerald's General + Rustboro pair, installed
as gTileset_HoennPilotGeneral + gTileset_HoennPilotRustboro.

Warps between installed maps (Route 104 <-> Petalburg Woods) are kept and renumbered; Rustboro's Pokemon
Center and Mart lead to FireRed's own layouts (nurse with respawn, clerk with Emerald's expanded list).
Mr. Briney's house, the Pretty Petal flower shop, the Gym, Devon Corp., the school, flats and houses stay
closed. Connections of every installed Hoenn exterior are recomputed from the Emerald headers, so
Petalburg City gains its west link to Route 104.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

GROUP = 'gMapGroup_Hoenn'
EXTERIORS = {  # Emerald map: (music, secondary tileset)
    'Route104': ('MUS_ROUTE24', 'gTileset_HoennPilotRustboro'),
    'PetalburgWoods': ('MUS_VIRIDIAN_FOREST', 'gTileset_HoennPilotRustboro'),
    'RustboroCity': ('MUS_VERMILLION', 'gTileset_HoennPilotRustboro'),
}
# Every installed Hoenn exterior, for connections (Emerald name -> FireRed map name).
INSTALLED = ['LittlerootTown', 'Route101', 'OldaleTown', 'Route102', 'Route103', 'PetalburgCity',
             'Route104', 'PetalburgWoods', 'RustboroCity']
TOWNS = {  # town: (doors {(x, y): interior}, heal cell, world map flag slot)
    'RustboroCity': ({(16, 38): 'PokemonCenter_1F', (16, 45): 'Mart'}, (16, 39), 'FLAG_0x8CB'),
}
SECTION_NAMES = {'MAPSEC_ROUTE_104': 'ROUTE 104', 'MAPSEC_PETALBURG_WOODS': 'PETALBURG WOODS', 'MAPSEC_RUSTBORO_CITY': 'RUSTBORO CITY'}
DOORS = [  # metatile, symbol in hoenn_pilot_doors.h (already defined for the Petalburg pair)
    (0x061, 'PokeCenter', 'DOOR_SOUND_SLIDING'), (0x041, 'PokeMart', 'DOOR_SOUND_SLIDING')]


def snake(name):
    return re.sub(r'(?<=[a-z])(?=[A-Z])', '_', name).upper()


def mid(name):
    return 'MAP_HOENN_' + snake(name)


def write_json(path, data):
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n', encoding='utf-8', newline='\n')


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument('--references', type=Path, required=True)
    p.add_argument('--staged', type=Path, required=True)
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    g, em = a.game, a.references / 'pokeemerald'
    report = {'maps': {}, 'closed_doors': {}}
    staged = {m['map']: m for m in json.loads((a.staged.parent / (a.staged.name + '.json')).read_text(encoding='utf-8'))['maps']}
    layouts = json.loads((g / 'data/layouts/layouts.json').read_text(encoding='utf-8'))
    groups = json.loads((g / 'data/maps/map_groups.json').read_text(encoding='utf-8'))
    headers = {m: json.loads((em / f'data/maps/{m}/map.json').read_text()) for m in INSTALLED}
    em_ids = {headers[m]['id']: m for m in INSTALLED}

    # Warps kept per new exterior: to another installed exterior, or a town door to a FireRed interior.
    kept, renumber = {}, {}
    for src in EXTERIORS:
        doors = TOWNS.get(src, ({},))[0]
        kept[src], renumber[src], closed = [], {}, []
        for i, w in enumerate(headers[src]['warp_events']):
            if w['dest_map'] in em_ids or (w['x'], w['y']) in doors:
                renumber[src][i] = len(kept[src])
                kept[src].append(w)
            else:
                closed.append(w['dest_map'])
        report['closed_doors'][src] = closed

    for src, (music, secondary) in EXTERIORS.items():
        name, lid = 'Hoenn_' + src, 'LAYOUT_HOENN_' + snake(src)
        folder = g / 'data/layouts' / name
        folder.mkdir(parents=True, exist_ok=True)
        for f in ('map.bin', 'border.bin'):
            shutil.copyfile(a.staged / src / f, folder / f)
        if not any(l.get('id') == lid for l in layouts['layouts']):
            layouts['layouts'].append({'id': lid, 'name': f'{name}_Layout', 'width': staged[src]['width'], 'height': staged[src]['height'],
                                       'border_width': 2, 'border_height': 2, 'primary_tileset': 'gTileset_HoennPilotGeneral',
                                       'secondary_tileset': secondary, 'border_filepath': f'data/layouts/{name}/border.bin',
                                       'blockdata_filepath': f'data/layouts/{name}/map.bin'})
        h = headers[src]
        doors = TOWNS.get(src, ({},))[0]
        warps = []
        for w in kept[src]:
            if (w['x'], w['y']) in doors:
                dest, dest_id = f'{mid(src)}_{snake(doors[(w["x"], w["y"])])}', '1'  # middle of the doormat
            else:
                target = em_ids[w['dest_map']]
                dest, dest_id = mid(target), str(renumber.get(target, {}).get(int(w['dest_warp_id']), int(w['dest_warp_id'])))
            warps.append({'x': w['x'], 'y': w['y'], 'elevation': w['elevation'], 'dest_map': dest, 'dest_warp_id': dest_id})
        header = {'id': mid(src), 'name': name, 'layout': lid, 'music': music, 'region_map_section': h['region_map_section'],
                  'requires_flash': False, 'weather': h['weather'], 'map_type': h['map_type'], 'allow_cycling': True,
                  'allow_escaping': False, 'allow_running': True, 'show_map_name': True, 'battle_scene': 'MAP_BATTLE_SCENE_NORMAL',
                  'connections': [], 'object_events': [], 'warp_events': warps, 'coord_events': [], 'bg_events': [], 'floor_number': 0}
        mdir = g / 'data/maps' / name
        mdir.mkdir(parents=True, exist_ok=True)
        if (mdir / 'map.json').exists():  # keep events written by install_pilot_events.py
            old = json.loads((mdir / 'map.json').read_text(encoding='utf-8'))
            for k in ('object_events', 'bg_events', 'coord_events'):
                header[k] = old.get(k, [])
        write_json(mdir / 'map.json', header)
        if not (mdir / 'scripts.inc').exists():
            (mdir / 'scripts.inc').write_text(f'@ Hoenn batch 2 (scripts/install_hoenn_batch2.py).\n{name}_MapScripts::\n\t.byte 0\n', encoding='utf-8', newline='\n')
        if name not in groups[GROUP]:
            groups[GROUP].append(name)
        report['maps'][name] = {'layout': lid, 'secondary': secondary, 'music': music, 'warps': len(warps)}

    # Connections of every installed exterior, both directions from the Emerald headers.
    for src in INSTALLED:
        path = g / 'data/maps' / ('Hoenn_' + src) / 'map.json'
        header = json.loads(path.read_text(encoding='utf-8'))
        header['connections'] = [{'map': mid(em_ids[c['map']]), 'offset': c['offset'], 'direction': c['direction']}
                                 for c in headers[src].get('connections') or [] if c['map'] in em_ids] or None
        write_json(path, header)
        if header['connections'] and 'Hoenn_' + src not in groups['connections_include_order']:
            groups['connections_include_order'].append('Hoenn_' + src)

    # Pokemon Center and Mart (FireRed layouts), heal location, Fly.
    heals_path = g / 'src/data/heal_locations.json'
    heals = json.loads(heals_path.read_text(encoding='utf-8'))
    rm_path, flags_path = g / 'src/region_map.c', g / 'include/constants/flags.h'
    rm, flags = rm_path.read_text(encoding='utf-8'), flags_path.read_text(encoding='utf-8')
    for town, (doors, heal, flag_slot) in TOWNS.items():
        ext, ext_id = 'Hoenn_' + town, mid(town)
        door_index = {kind: next(i for i, w in enumerate(kept[town]) if (w['x'], w['y']) == cell) for cell, kind in doors.items()}
        mart_block = (em / f'data/maps/{town}_Mart/scripts.inc').read_text(encoding='utf-8')
        items = re.findall(r'\.2byte (ITEM_\w+)', mart_block.split(f'{town}_Mart_Pokemart_Expanded:')[1].split('pokemartlistend')[0])
        heal_id = 'HEAL_LOCATION_' + snake(town)
        section = headers[town]['region_map_section']
        for kind in doors.values():
            name, interior = f'{ext}_{kind}', f'{ext_id}_{snake(kind)}'
            center = kind.startswith('Pokemon')
            layout, cells = ('LAYOUT_POKEMON_CENTER_1F', [(6, 8), (7, 8), (8, 8)]) if center else ('LAYOUT_MART', [(3, 7), (4, 7), (5, 7)])
            npc = ('OBJ_EVENT_GFX_NURSE', 7, 2, f'{name}_EventScript_Nurse') if center else ('OBJ_EVENT_GFX_CLERK', 2, 3, f'{name}_EventScript_Clerk')
            header = {'id': interior, 'name': name, 'layout': layout, 'music': 'MUS_POKE_CENTER', 'region_map_section': section,
                      'requires_flash': False, 'weather': 'WEATHER_NONE', 'map_type': 'MAP_TYPE_INDOOR', 'allow_cycling': False,
                      'allow_escaping': False, 'allow_running': False, 'show_map_name': False, 'floor_number': 0,
                      'battle_scene': 'MAP_BATTLE_SCENE_NORMAL', 'connections': None,
                      'object_events': [{'type': 'object', 'graphics_id': npc[0], 'x': npc[1], 'y': npc[2], 'elevation': 3,
                                         'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
                                         'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0', 'script': npc[3], 'flag': '0'}],
                      'warp_events': [{'x': x, 'y': y, 'elevation': 3, 'dest_map': ext_id, 'dest_warp_id': str(door_index[kind])} for x, y in cells],
                      'coord_events': [], 'bg_events': []}
            mdir = g / 'data/maps' / name
            mdir.mkdir(parents=True, exist_ok=True)
            write_json(mdir / 'map.json', header)
            if center:
                script = (f'@ Hoenn batch 2: FireRed Pokemon Center layout (scripts/install_hoenn_batch2.py).\n{name}_MapScripts::\n'
                          f'\tmap_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition\n\tmap_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume\n\t.byte 0\n\n'
                          f'{name}_OnTransition::\n\tsetrespawn {heal_id}\n\tend\n\n'
                          f'{name}_EventScript_Nurse::\n\tlock\n\tfaceplayer\n\tcall EventScript_PkmnCenterNurse\n\trelease\n\tend\n')
            else:
                rows = ''.join(f'\t.2byte {i}\n' for i in items)
                script = (f'@ Hoenn batch 2: FireRed Mart layout; pokeemerald {town}_Mart list.\n{name}_MapScripts::\n\t.byte 0\n\n'
                          f'{name}_EventScript_Clerk::\n\tgoto_if_questlog EventScript_ReleaseEnd\n\tlock\n\tfaceplayer\n\tmessage Text_MayIHelpYou\n\twaitmessage\n'
                          f'\tpokemart {name}_Items\n\tmsgbox Text_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n{name}_Items::\n{rows}\t.2byte ITEM_NONE\n\trelease\n\tend\n')
            (mdir / 'scripts.inc').write_text(script, encoding='utf-8', newline='\n')
            if name not in groups[GROUP]:
                groups[GROUP].append(name)
            report['maps'][name] = {'layout': layout, **({'heal_location': heal_id} if center else {'items': items})}
        if heal_id not in {h['id'] for h in heals['heal_locations']}:
            heals['heal_locations'].append({'id': heal_id, 'map': ext_id, 'x': heal[0], 'y': heal[1],
                                            'respawn_map': f'{ext_id}_POKEMON_CENTER_1F', 'respawn_npc': 1})
        flag = 'FLAG_WORLD_MAP_' + snake(town)
        if f'#define {flag} ' not in flags:
            last = max(re.findall(r'#define FLAG_WORLD_MAP_\w+ +FLAG_0x8C\w\n', flags), key=flags.index)
            flags = flags.replace(last, last + f'#define {flag:<58} {flag_slot}\n', 1)
        row = f'    [{section}]'
        if row not in rm:
            anchor = '    [MAPSEC_PETALBURG_CITY]  = {MAP(MAP_HOENN_PETALBURG_CITY),  HEAL_LOCATION_PETALBURG_CITY},\n'
            assert rm.count(anchor) == 1
            rm = rm.replace(anchor, anchor + f'{row + " " * max(1, 25 - len(row))}= {{MAP({ext_id}), {heal_id}}},\n')
        case = f'    case {section}:\n'
        if case not in rm:
            anchor = '    case MAPSEC_PETALBURG_CITY:\n        return FlagGet(FLAG_WORLD_MAP_PETALBURG_CITY) ? MAPSECTYPE_VISITED : MAPSECTYPE_NOT_VISITED;\n'
            assert rm.count(anchor) == 1
            rm = rm.replace(anchor, anchor + case + f'        return FlagGet({flag}) ? MAPSECTYPE_VISITED : MAPSECTYPE_NOT_VISITED;\n')
    write_json(heals_path, heals)
    rm_path.write_text(rm, encoding='utf-8', newline='\n')
    flags_path.write_text(flags, encoding='utf-8', newline='\n')

    # Doors on the General + Rustboro pair.
    doors_h = g / 'src/data/hoenn_pilot_doors.h'
    t = doors_h.read_text(encoding='utf-8')
    if 'sHoennPilotDoors_Rustboro' not in t:
        t += ('static const struct DoorGraphics sHoennPilotDoors_Rustboro[] = {\n'
              + ''.join(f'    {{0x{m:03X}, {snd}, DOOR_SIZE_1x2, sHoennDoorTiles_{sym}, sHoennDoorPalettes_{sym}}},\n' for m, sym, snd in DOORS)
              + '    {}\n};\n')
        doors_h.write_text(t, encoding='utf-8', newline='\n')
    dt = g / 'src/data/door_tilesets.h'
    t = dt.read_text(encoding='utf-8')
    if 'sHoennPilotDoors_Rustboro}' not in t:
        t = t.replace('#include "hoenn_pilot_doors.h"\n', 'extern const struct Tileset gTileset_HoennPilotRustboro;\n#include "hoenn_pilot_doors.h"\n')
        t = t.replace('    {NULL, NULL, NULL}', '    {&gTileset_HoennPilotGeneral, &gTileset_HoennPilotRustboro, sHoennPilotDoors_Rustboro},\n    {NULL, NULL, NULL}')
        dt.write_text(t, encoding='utf-8', newline='\n')

    secs_path = g / 'src/data/region_map/region_map_sections.json'
    secs = json.loads(secs_path.read_text(encoding='utf-8'))
    for e in secs['map_sections']:
        if e['id'] in SECTION_NAMES:
            e['hoenn_name'] = SECTION_NAMES[e['id']]
    write_json(secs_path, secs)
    (g / 'data/layouts/layouts.json').write_text(json.dumps(layouts, indent=2) + '\n', encoding='utf-8', newline='\n')
    write_json(g / 'data/maps/map_groups.json', groups)
    es = g / 'data/event_scripts.s'
    t = es.read_text(encoding='utf-8')
    for name in groups[GROUP]:
        line = f'\t.include "data/maps/{name}/scripts.inc"\n'
        if line not in t:
            last = max((l for l in t.splitlines(True) if l.startswith('\t.include "data/maps/Hoenn_')), key=t.index)
            t = t.replace(last, last + line, 1)
    es.write_text(t, encoding='utf-8', newline='\n')
    write_json(a.report, report)
    print(json.dumps({k: len(v) for k, v in report.items()}))


if __name__ == '__main__':
    main()
