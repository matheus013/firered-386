"""Johto batch 1: Cherrygrove City's Pokemon Center and Mart, its Fly destination and respawn. Idempotent.

Run after scripts/install_johto_pilot.py, which installs the Cherrygrove City, Route 30 and Route 31
exteriors from HNS_modern (and drops every warp to a map outside the installed set). This script puts
back the two Cherrygrove doors that lead to FireRed's own Pokemon Center and Mart layouts
(LAYOUT_POKEMON_CENTER_1F / LAYOUT_MART), like the Hoenn batch: a nurse who sets the respawn, and a
clerk with the HGSS Cherrygrove Mart list (HNS only stocks Potion and Antidote before the Pokedex).
The houses, Mr. Pokemon's house, Route 30's house, Dark Cave and the Violet gate stay closed.
"""
import argparse
import json
from pathlib import Path
import re

TOWN, TOWN_ID = 'Johto_CherrygroveCity', 'MAP_JOHTO_CHERRYGROVE_CITY'
# door (x, y) on the exterior -> interior; the interior warps back to this door's index.
DOORS = [((47, 7), 'PokemonCenter_1F'), ((41, 7), 'Mart')]
MART = ['ITEM_POKE_BALL', 'ITEM_POTION', 'ITEM_ANTIDOTE', 'ITEM_PARALYZE_HEAL', 'ITEM_AWAKENING']
HEAL = ('HEAL_LOCATION_CHERRYGROVE_CITY', 47, 8)
FLAG = ('FLAG_WORLD_MAP_CHERRYGROVE_CITY', 'FLAG_0x8C7')
SECTION = 'MAPSEC_CHERRYGROVE_CITY'


def write_json(path, data):
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n', encoding='utf-8', newline='\n')


def replace_once(text, anchor, new, marker):
    """Insert `new` after `anchor` unless `marker` is already in the text."""
    if marker in text:
        return text
    assert text.count(anchor) == 1, anchor
    return text.replace(anchor, anchor + new)


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    g = a.game
    report = {'maps': {}, 'closed_doors': []}

    # Exterior: the two doors, to the middle of each doormat (warp 1) like FireRed's towns.
    ext_path = g / f'data/maps/{TOWN}/map.json'
    ext = json.loads(ext_path.read_text(encoding='utf-8'))
    ext['warp_events'] = [{'x': x, 'y': y, 'elevation': 0, 'dest_map': f'{TOWN_ID}_{re.sub(r"(?<=[a-z])(?=[A-Z])", "_", kind).upper()}',
                           'dest_warp_id': '1'} for (x, y), kind in DOORS]
    write_json(ext_path, ext)

    groups_path = g / 'data/maps/map_groups.json'
    groups = json.loads(groups_path.read_text(encoding='utf-8'))
    for door_index, ((_x, _y), kind) in enumerate(DOORS):
        name = f'{TOWN}_{kind}'
        mid = f'{TOWN_ID}_{re.sub(r"(?<=[a-z])(?=[A-Z])", "_", kind).upper()}'
        center = kind.startswith('Pokemon')
        layout, cells = ('LAYOUT_POKEMON_CENTER_1F', [(6, 8), (7, 8), (8, 8)]) if center else ('LAYOUT_MART', [(3, 7), (4, 7), (5, 7)])
        npc = ('OBJ_EVENT_GFX_NURSE', 7, 2, f'{name}_EventScript_Nurse') if center else ('OBJ_EVENT_GFX_CLERK', 2, 3, f'{name}_EventScript_Clerk')
        header = {'id': mid, 'name': name, 'layout': layout, 'music': 'MUS_POKE_CENTER', 'region_map_section': SECTION,
                  'requires_flash': False, 'weather': 'WEATHER_NONE', 'map_type': 'MAP_TYPE_INDOOR', 'allow_cycling': False,
                  'allow_escaping': False, 'allow_running': False, 'show_map_name': False, 'floor_number': 0,
                  'battle_scene': 'MAP_BATTLE_SCENE_NORMAL', 'connections': None,
                  'object_events': [{'type': 'object', 'graphics_id': npc[0], 'x': npc[1], 'y': npc[2], 'elevation': 3,
                                     'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
                                     'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0', 'script': npc[3], 'flag': '0'}],
                  'warp_events': [{'x': x, 'y': y, 'elevation': 3, 'dest_map': TOWN_ID, 'dest_warp_id': str(door_index)} for x, y in cells],
                  'coord_events': [], 'bg_events': []}
        folder = g / 'data/maps' / name
        folder.mkdir(parents=True, exist_ok=True)
        write_json(folder / 'map.json', header)
        if center:
            script = (f'@ Johto batch 1: FireRed Pokemon Center layout (scripts/install_johto_batch1.py).\n{name}_MapScripts::\n'
                      f'\tmap_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition\n\tmap_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume\n\t.byte 0\n\n'
                      f'{name}_OnTransition::\n\tsetrespawn {HEAL[0]}\n\tend\n\n'
                      f'{name}_EventScript_Nurse::\n\tlock\n\tfaceplayer\n\tcall EventScript_PkmnCenterNurse\n\trelease\n\tend\n')
        else:
            items = ''.join(f'\t.2byte {i}\n' for i in MART)
            script = (f'@ Johto batch 1: FireRed Mart layout; HGSS Cherrygrove Mart list.\n{name}_MapScripts::\n\t.byte 0\n\n'
                      f'{name}_EventScript_Clerk::\n\tgoto_if_questlog EventScript_ReleaseEnd\n\tlock\n\tfaceplayer\n\tmessage Text_MayIHelpYou\n\twaitmessage\n'
                      f'\tpokemart {name}_Items\n\tmsgbox Text_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n{name}_Items::\n{items}\t.2byte ITEM_NONE\n\trelease\n\tend\n')
        (folder / 'scripts.inc').write_text(script, encoding='utf-8', newline='\n')
        if name not in groups['gMapGroup_Johto']:
            groups['gMapGroup_Johto'].append(name)
        report['maps'][name] = {'layout': layout, 'npc': npc[0], **({'heal_location': HEAL[0]} if center else {'items': MART})}
    write_json(groups_path, groups)

    # event_scripts.s includes, after the last Johto map.
    es = g / 'data/event_scripts.s'
    t = es.read_text(encoding='utf-8')
    for name in groups['gMapGroup_Johto']:
        line = f'\t.include "data/maps/{name}/scripts.inc"\n'
        if line not in t:
            last = max((l for l in t.splitlines(True) if l.startswith('\t.include "data/maps/Johto_')), key=t.index)
            t = t.replace(last, last + line, 1)
    es.write_text(t, encoding='utf-8', newline='\n')

    # Heal location in front of the Pokemon Center; respawn by the nurse.
    heal_path = g / 'src/data/heal_locations.json'
    heals = json.loads(heal_path.read_text(encoding='utf-8'))
    if HEAL[0] not in {h['id'] for h in heals['heal_locations']}:
        heals['heal_locations'].append({'id': HEAL[0], 'map': TOWN_ID, 'x': HEAL[1], 'y': HEAL[2],
                                        'respawn_map': f'{TOWN_ID}_POKEMON_CENTER_1F', 'respawn_npc': 1})
    write_json(heal_path, heals)

    # World map flag (set on entry by install_pilot_events.py) and the Fly destination.
    flags = g / 'include/constants/flags.h'
    t = flags.read_text(encoding='utf-8')
    t = replace_once(t, '#define FLAG_WORLD_MAP_PETALBURG_CITY                              FLAG_0x8C6\n',
                     f'#define {FLAG[0]:<58} {FLAG[1]}\n', f'#define {FLAG[0]} ')
    flags.write_text(t, encoding='utf-8', newline='\n')
    rm = g / 'src/region_map.c'
    t = rm.read_text(encoding='utf-8')
    t = replace_once(t, '    [MAPSEC_ROUTE_29            - KANTO_MAPSEC_START] = {MAP(MAP_JOHTO_ROUTE29),                         HEAL_LOCATION_NONE},\n',
                     '    [MAPSEC_CHERRYGROVE_CITY    - KANTO_MAPSEC_START] = {MAP(MAP_JOHTO_CHERRYGROVE_CITY),                HEAL_LOCATION_CHERRYGROVE_CITY},\n'
                     '    [MAPSEC_ROUTE_30            - KANTO_MAPSEC_START] = {MAP(MAP_JOHTO_ROUTE30),                         HEAL_LOCATION_NONE},\n'
                     '    [MAPSEC_ROUTE_31            - KANTO_MAPSEC_START] = {MAP(MAP_JOHTO_ROUTE31),                         HEAL_LOCATION_NONE},\n',
                     '[MAPSEC_CHERRYGROVE_CITY    - KANTO_MAPSEC_START]')
    t = replace_once(t, '    case MAPSEC_NEW_BARK_TOWN:\n        return FlagGet(FLAG_WORLD_MAP_NEW_BARK_TOWN) ? MAPSECTYPE_VISITED : MAPSECTYPE_NOT_VISITED;\n',
                     f'    case {SECTION}:\n        return FlagGet({FLAG[0]}) ? MAPSECTYPE_VISITED : MAPSECTYPE_NOT_VISITED;\n',
                     f'    case {SECTION}:\n')
    rm.write_text(t, encoding='utf-8', newline='\n')
    report['heal_location'] = dict(zip(('id', 'x', 'y'), HEAL))
    report['world_map_flag'] = FLAG[0]
    report['closed_doors'] = ['CherrygroveCity_House1', 'CherrygroveCity_House2', 'CherrygroveCity_House3', 'Route30_House',
                              'Route30_MrPokemonsHouse', 'DarkCave_SouthSide', 'Gate_Route31_VioletCity']
    write_json(a.report, report)
    print(json.dumps({k: len(v) if isinstance(v, (list, dict)) else v for k, v in report.items()}))


if __name__ == '__main__':
    main()
