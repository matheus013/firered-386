"""Johto geography batches: each town's Pokemon Center and Mart, Fly destination and respawn. Idempotent.

Run after scripts/install_johto_pilot.py, which installs the exteriors from HNS_modern and drops every
warp to a map outside the installed set. This script puts back each town's two doors to FireRed's own
Pokemon Center and Mart layouts (LAYOUT_POKEMON_CENTER_1F / LAYOUT_MART), like the Hoenn batch: a nurse
who sets the respawn, and a clerk with the HGSS Mart list of that town. Warps the pilot installer kept
(e.g. Violet's gate to Route 31) stay first, so their indices do not move.

Batch 1: Cherrygrove City (HNS only stocks Potion and Antidote there before the Pokedex; HGSS list used).
Batch 2: Violet City (HGSS list without Flower Mail, which FireRed does not have).
Houses, gyms, schools, towers, Mr. Pokemon's house and Dark Cave stay closed.
"""
import argparse
import json
from pathlib import Path
import re

TOWNS = [
    {'town': 'CherrygroveCity', 'doors': [((47, 7), 'PokemonCenter_1F'), ((41, 7), 'Mart')],
     'mart': ['ITEM_POKE_BALL', 'ITEM_POTION', 'ITEM_ANTIDOTE', 'ITEM_PARALYZE_HEAL', 'ITEM_AWAKENING'],
     'heal': (47, 8), 'flag': 'FLAG_0x8C7',
     'closed': ['CherrygroveCity_House1', 'CherrygroveCity_House2', 'CherrygroveCity_House3', 'Route30_House',
                'Route30_MrPokemonsHouse', 'DarkCave_SouthSide']},
    {'town': 'VioletCity', 'doors': [((39, 45), 'PokemonCenter_1F'), ((16, 35), 'Mart')],
     'mart': ['ITEM_POKE_BALL', 'ITEM_POTION', 'ITEM_ESCAPE_ROPE', 'ITEM_ANTIDOTE', 'ITEM_PARALYZE_HEAL',
              'ITEM_AWAKENING', 'ITEM_X_DEFEND', 'ITEM_X_ATTACK', 'ITEM_X_SPEED'],
     'heal': (39, 46), 'flag': 'FLAG_0x8C8',
     'closed': ['SproutTower_1F', 'VioletCity_House1', 'VioletCity_House2', 'VioletCity_TrainerSchool', 'VioletCity_Gym']},
    # Batch 3: the Pokemon Center at the south end of Route 32 (no Mart, no Fly, like FireRed's route centers).
    {'town': 'Route32', 'doors': [((25, 84), 'PokemonCenter_1F')], 'mart': None, 'heal': (25, 85), 'flag': None, 'section': 'MAPSEC_ROUTE_32',
     'closed': ['Gate_RuinsOfAlph_Route32', 'UnionCave_1F']},
    # Batch 4: Azalea Town (Johto South primary tileset).
    {'town': 'AzaleaTown', 'doors': [((31, 15), 'PokemonCenter_1F'), ((40, 11), 'Mart')],
     'mart': ['ITEM_POKE_BALL', 'ITEM_POTION', 'ITEM_SUPER_POTION', 'ITEM_ESCAPE_ROPE', 'ITEM_REPEL', 'ITEM_ANTIDOTE',
              'ITEM_PARALYZE_HEAL'],
     'heal': (31, 16), 'flag': 'FLAG_0x8C9',
     'closed': ['SlowpokeWell_B1F', 'Gate_AzaleaTown_IlexForest', 'AzaleaTown_KurtsHouse', 'AzaleaTown_House1',
                'AzaleaTown_Gym', 'UnionCave_1F']},
    # Batch 6: Goldenrod City. The Dept. Store door leads to FireRed's Mart layout with the HGSS 2F list
    # (a one-floor stand-in for the five-floor store).
    {'town': 'GoldenrodCity', 'doors': [((28, 36), 'PokemonCenter_1F'), ((39, 36), 'Mart')],
     'mart': ['ITEM_POKE_BALL', 'ITEM_GREAT_BALL', 'ITEM_ULTRA_BALL', 'ITEM_POTION', 'ITEM_SUPER_POTION', 'ITEM_HYPER_POTION',
              'ITEM_REVIVE', 'ITEM_ANTIDOTE', 'ITEM_PARALYZE_HEAL', 'ITEM_AWAKENING', 'ITEM_BURN_HEAL', 'ITEM_ICE_HEAL',
              'ITEM_ESCAPE_ROPE', 'ITEM_REPEL', 'ITEM_SUPER_REPEL'],
     'heal': (28, 37), 'flag': 'FLAG_0x8CA',
     'closed': ['Gate_GoldenrodCity_Route35', 'GoldenrodCity_UndergroundEntrance', 'GoldenrodCity_House1', 'GoldenrodCity_Gym',
                'GoldenrodCity_FlowerShop', 'GoldenrodCity_House2', 'GoldenrodCity_RadioTower_1F', 'GoldenrodCity_TrainStation',
                'GoldenrodCity_House3', 'GoldenrodCity_GameCorner', 'GoldenrodCity_BillsHouse', 'GoldenrodCity_BikeShop']},
]


def snake(name):
    return re.sub(r'(?<=[a-z])(?=[A-Z])', '_', name).upper()


def write_json(path, data):
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n', encoding='utf-8', newline='\n')


def replace_once(text, anchor, new, marker):
    """Insert `new` after `anchor` unless `marker` is already in the text."""
    if marker in text:
        return text
    assert text.count(anchor) == 1, anchor
    return text.replace(anchor, anchor + new)


def install_town(g, cfg, groups, report):
    town = 'Johto_' + cfg['town']
    town_id = 'MAP_JOHTO_' + snake(cfg['town'])
    section = cfg.get('section') or 'MAPSEC_' + snake(cfg['town'])
    heal_id = 'HEAL_LOCATION_' + snake(cfg['town'])
    interiors = {kind: f'{town_id}_{snake(kind)}' for _cell, kind in cfg['doors']}

    # Exterior: keep the other warps first, then the two doors (to the middle of each doormat, warp 1).
    ext_path = g / f'data/maps/{town}/map.json'
    ext = json.loads(ext_path.read_text(encoding='utf-8'))
    kept = [w for w in ext['warp_events'] if w['dest_map'] not in interiors.values()]
    door_index = {}
    for (x, y), kind in cfg['doors']:
        door_index[kind] = len(kept)
        kept.append({'x': x, 'y': y, 'elevation': 0, 'dest_map': interiors[kind], 'dest_warp_id': '1'})
    ext['warp_events'] = kept
    write_json(ext_path, ext)

    for _cell, kind in cfg['doors']:
        name, mid = f'{town}_{kind}', interiors[kind]
        center = kind.startswith('Pokemon')
        layout, cells = ('LAYOUT_POKEMON_CENTER_1F', [(6, 8), (7, 8), (8, 8)]) if center else ('LAYOUT_MART', [(3, 7), (4, 7), (5, 7)])
        npc = ('OBJ_EVENT_GFX_NURSE', 7, 2, f'{name}_EventScript_Nurse') if center else ('OBJ_EVENT_GFX_CLERK', 2, 3, f'{name}_EventScript_Clerk')
        header = {'id': mid, 'name': name, 'layout': layout, 'music': 'MUS_POKE_CENTER', 'region_map_section': section,
                  'requires_flash': False, 'weather': 'WEATHER_NONE', 'map_type': 'MAP_TYPE_INDOOR', 'allow_cycling': False,
                  'allow_escaping': False, 'allow_running': False, 'show_map_name': False, 'floor_number': 0,
                  'battle_scene': 'MAP_BATTLE_SCENE_NORMAL', 'connections': None,
                  'object_events': [{'type': 'object', 'graphics_id': npc[0], 'x': npc[1], 'y': npc[2], 'elevation': 3,
                                     'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
                                     'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0', 'script': npc[3], 'flag': '0'}],
                  'warp_events': [{'x': x, 'y': y, 'elevation': 3, 'dest_map': town_id, 'dest_warp_id': str(door_index[kind])} for x, y in cells],
                  'coord_events': [], 'bg_events': []}
        folder = g / 'data/maps' / name
        folder.mkdir(parents=True, exist_ok=True)
        write_json(folder / 'map.json', header)
        if center:
            script = (f'@ Johto geography: FireRed Pokemon Center layout (scripts/install_johto_batch1.py).\n{name}_MapScripts::\n'
                      f'\tmap_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition\n\tmap_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume\n\t.byte 0\n\n'
                      f'{name}_OnTransition::\n\tsetrespawn {heal_id}\n\tend\n\n'
                      f'{name}_EventScript_Nurse::\n\tlock\n\tfaceplayer\n\tcall EventScript_PkmnCenterNurse\n\trelease\n\tend\n')
        else:
            items = ''.join(f'\t.2byte {i}\n' for i in cfg['mart'])
            script = (f'@ Johto geography: FireRed Mart layout; HGSS {cfg["town"]} Mart list.\n{name}_MapScripts::\n\t.byte 0\n\n'
                      f'{name}_EventScript_Clerk::\n\tgoto_if_questlog EventScript_ReleaseEnd\n\tlock\n\tfaceplayer\n\tmessage Text_MayIHelpYou\n\twaitmessage\n'
                      f'\tpokemart {name}_Items\n\tmsgbox Text_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n{name}_Items::\n{items}\t.2byte ITEM_NONE\n\trelease\n\tend\n')
        (folder / 'scripts.inc').write_text(script, encoding='utf-8', newline='\n')
        if name not in groups['gMapGroup_Johto']:
            groups['gMapGroup_Johto'].append(name)
        report['maps'][name] = {'layout': layout, 'npc': npc[0], **({'heal_location': heal_id} if center else {'items': cfg['mart']})}

    # Heal location in front of the Pokemon Center; respawn by the nurse.
    heal_path = g / 'src/data/heal_locations.json'
    heals = json.loads(heal_path.read_text(encoding='utf-8'))
    if heal_id not in {h['id'] for h in heals['heal_locations']}:
        heals['heal_locations'].append({'id': heal_id, 'map': town_id, 'x': cfg['heal'][0], 'y': cfg['heal'][1],
                                        'respawn_map': f'{town_id}_POKEMON_CENTER_1F', 'respawn_npc': 1})
    write_json(heal_path, heals)

    # World map flag (set on entry by install_pilot_events.py) and the Fly destination. A route with a
    # Pokemon Center (flag None) only gets its map section row, like Routes 30/31: no Fly, as in HGSS.
    flag = 'FLAG_WORLD_MAP_' + snake(cfg['town']) if cfg.get('flag') else None
    if flag:
        flags = g / 'include/constants/flags.h'
        t = flags.read_text(encoding='utf-8')
        if f'#define {flag} ' not in t:
            last = max(re.findall(r'#define FLAG_WORLD_MAP_\w+ +FLAG_0x8C\w\n', t), key=t.index)
            t = t.replace(last, last + f'#define {flag:<58} {cfg["flag"]}\n', 1)
        flags.write_text(t, encoding='utf-8', newline='\n')
    rm = g / 'src/region_map.c'
    t = rm.read_text(encoding='utf-8')
    row = f'    [{section:<27}- KANTO_MAPSEC_START] = {{MAP({town_id}),'
    if row not in t:
        anchor = '    [MAPSEC_ROUTE_29            - KANTO_MAPSEC_START] = {MAP(MAP_JOHTO_ROUTE29),                         HEAL_LOCATION_NONE},\n'
        assert t.count(anchor) == 1
        t = t.replace(anchor, anchor + f'{row:<93} {heal_id if flag else "HEAL_LOCATION_NONE"}}},\n')
    if flag:
        t = replace_once(t, '    case MAPSEC_NEW_BARK_TOWN:\n        return FlagGet(FLAG_WORLD_MAP_NEW_BARK_TOWN) ? MAPSECTYPE_VISITED : MAPSECTYPE_NOT_VISITED;\n',
                         f'    case {section}:\n        return FlagGet({flag}) ? MAPSECTYPE_VISITED : MAPSECTYPE_NOT_VISITED;\n', f'    case {section}:\n')
    rm.write_text(t, encoding='utf-8', newline='\n')
    report['towns'][cfg['town']] = {'heal_location': {'id': heal_id, 'x': cfg['heal'][0], 'y': cfg['heal'][1]},
                                    'world_map_flag': flag, 'closed_doors': cfg['closed']}


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    g = a.game
    report = {'maps': {}, 'towns': {}}
    groups_path = g / 'data/maps/map_groups.json'
    groups = json.loads(groups_path.read_text(encoding='utf-8'))
    for cfg in TOWNS:
        install_town(g, cfg, groups, report)
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
    write_json(a.report, report)
    print(json.dumps({k: list(v) for k, v in report.items()}))


if __name__ == '__main__':
    main()
