"""Install the ambient events of the region pilot maps (phase 1): signs, townspeople, an item, a Cut tree
and the Elm lab healing machine. Idempotent; rewrites each pilot map's scripts.inc and event lists.

Only events whose effect does not depend on the source game's story are brought over. Where a source
script branches on story progress, the branch that fits a trainer arriving from elsewhere is fixed and
named below. Story events, the player's own house/PC/clock, starters, professors, mothers, rivals and
HNS overworld Pokemon belong to later phases. Every text is copied verbatim from the source script
files and checked against FireRed's charmap; labels are prefixed with the FireRed map name so the
Hoenn and HNS copies of the same Emerald labels cannot collide.
"""
import argparse
import json
from pathlib import Path
import re

EMERALD, HNS = 'pokeemerald', 'HNS_modern'
FLAG_POTION = 'FLAG_HIDE_JOHTO_ROUTE29_POTION'
# FireRed sprite for each source sprite. Imported ones come from import_npc_sprite.py.
SPRITES = {
    'OBJ_EVENT_GFX_TWIN': 'OBJ_EVENT_GFX_TWIN', 'OBJ_EVENT_GFX_SCIENTIST_F': 'OBJ_EVENT_GFX_SCIENTIST_F',
    'OBJ_EVENT_GFX_FAT_MAN': 'OBJ_EVENT_GFX_FAT_MAN', 'OBJ_EVENT_GFX_BOY_2': 'OBJ_EVENT_GFX_BOY',
    'OBJ_EVENT_GFX_BOY': 'OBJ_EVENT_GFX_BOY', 'OBJ_EVENT_GFX_YOUNGSTER': 'OBJ_EVENT_GFX_YOUNGSTER',
    'OBJ_EVENT_GFX_LASS': 'OBJ_EVENT_GFX_LASS', 'OBJ_EVENT_GFX_OLD_MAN_2': 'OBJ_EVENT_GFX_OLD_MAN_2',
    'OBJ_EVENT_GFX_WOMAN_1': 'OBJ_EVENT_GFX_WOMAN_1', 'OBJ_EVENT_GFX_WOMAN_2': 'OBJ_EVENT_GFX_WOMAN_2',
    'OBJ_EVENT_GFX_WOMAN_3': 'OBJ_EVENT_GFX_WOMAN_3', 'OBJ_EVENT_GFX_LITTLE_BOY': 'OBJ_EVENT_GFX_LITTLE_BOY',
    'OBJ_EVENT_GFX_POLICEMAN': 'OBJ_EVENT_GFX_POLICEMAN', 'OBJ_EVENT_GFX_SCIENTIST_1': 'OBJ_EVENT_GFX_SCIENTIST',
    'OBJ_EVENT_GFX_SCIENTIST_M': 'OBJ_EVENT_GFX_SCIENTIST', 'OBJ_EVENT_GFX_CUTTABLE_TREE': 'OBJ_EVENT_GFX_CUT_TREE',
    'OBJ_EVENT_GFX_POKE_BALL': 'OBJ_EVENT_GFX_ITEM_BALL',
    # Phase 2: main characters. FireRed already has the Ruby/Sapphire Brendan and May.
    'OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL': 'OBJ_EVENT_GFX_RS_BRENDAN', 'OBJ_EVENT_GFX_RIVAL_MAY_NORMAL': 'OBJ_EVENT_GFX_RS_MAY',
    'OBJ_EVENT_GFX_WOMAN_4': 'OBJ_EVENT_GFX_WOMAN_4', 'OBJ_EVENT_GFX_NINJA_BOY': 'OBJ_EVENT_GFX_NINJA_BOY',
    'OBJ_EVENT_GFX_PROF_BIRCH': 'OBJ_EVENT_GFX_PROF_BIRCH', 'OBJ_EVENT_GFX_PROF_ELM': 'OBJ_EVENT_GFX_PROF_ELM',
    'OBJ_EVENT_GFX_SILVER': 'OBJ_EVENT_GFX_SILVER',
    'OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX': 'OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX',
    'OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY': 'OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY',
}
# Positions the source map script sets on load (setobjectxyperm/setobjectmovementtype); the map.json
# coordinates are the cutscene entry point, on top of the stair warp.
# Objects that do not come from a source game: the SEAGALLOP sailors that take the player back to
# Vermilion City (access chosen by the maintainer: ship from Vermilion after the first Hall of Fame).
# Their text is new; region_pilot_ptbr.py translates it.
FERRY_TEXT = ['The SEAGALLOP can take you back\\n', 'to VERMILION CITY in KANTO.\\p', 'Do you want to set sail?$']
FERRIES = {'LittlerootTown': ('SEAGALLOP_LITTLEROOT', 10, 13), 'NewBarkTown': ('SEAGALLOP_NEW_BARK', 24, 12)}
POSITIONS = {
    ('LittlerootTown_BrendansHouse_2F', 0): (0, 2, 'MOVEMENT_TYPE_FACE_UP', 'LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos'),
    ('LittlerootTown_MaysHouse_2F', 0): (8, 2, 'MOVEMENT_TYPE_FACE_UP', 'LittlerootTown_MaysHouse_2F_EventScript_TryUpdateMayPos'),
}
# Same source name, different person: Emerald's mother and HNS's Johto mother.
REPO_SPRITES = {('pokeemerald', 'OBJ_EVENT_GFX_MOM'): 'OBJ_EVENT_GFX_EM_MOM', ('HNS_modern', 'OBJ_EVENT_GFX_MOM'): 'OBJ_EVENT_GFX_JOHTO_MOM'}
# map -> (source repo, objects {index: (kind, text label or None, note)}, signs {index: (kind, text label)})
# kinds: npc, sign, item (Potion), cuttree, heal (Elm's machine: heals once the player has a Pokemon)
EVENTS = {
    'LittlerootTown': (EMERALD, {
        0: ('npc', 'LittlerootTown_Text_GoodLuckCatchingPokemon', 'Twin: post-adventure branch (FLAG_ADVENTURE_STARTED)'),
        1: ('npc', 'LittlerootTown_Text_CanUsePCToStoreItems', ''),
        2: ('npc', 'LittlerootTown_Text_BirchSpendsDaysInLab', '')},
        {0: ('sign', 'LittlerootTown_Text_TownSign'), 1: ('sign', 'LittlerootTown_Text_ProfBirchsLab')}),
    'Route101': (EMERALD, {
        0: ('npc', 'Route101_Text_TakeTiredPokemonToPokeCenter', ''),
        5: ('npc', 'Route101_Text_WildPokemonInTallGrass', 'source hides him until Birch is rescued; always shown')},
        {0: ('sign', 'Route101_Text_RouteSign')}),
    'LittlerootTown_ProfessorBirchsLab': (EMERALD, {
        0: ('npc', 'LittlerootTown_ProfessorBirchsLab_Text_BirchIsntOneForDeskWork', 'aide: AideAlreadyMet branch, consistent with Birch in the lab'),
        1: ('npc', 'LittlerootTown_ProfessorBirchsLab_Text_CountlessPokemonAwait', 'Birch: post-starter branch (VAR_BIRCH_LAB_STATE 5)')},
        'lab'),
    # Family choice (user, 08/11/2026): May is Birch's daughter. May's house = Birch family,
    # Brendan's house = Norman family, as in Emerald with a male player. Cutscene-only objects stay out.
    'LittlerootTown_BrendansHouse_1F': (EMERALD, {
        0: ('silent', None, 'Brendan\'s mother: every source line speaks to her own child'),
        1: ('cry', ('SPECIES_VIGOROTH', 'PlayersHouse_1F_Text_Vigoroth2'), 'mover'),
        2: ('cry', ('SPECIES_VIGOROTH', 'PlayersHouse_1F_Text_Vigoroth1'), 'mover')}, {}),
    'LittlerootTown_BrendansHouse_2F': (EMERALD, {
        0: ('npc', 'RivalsHouse_2F_Text_BrendanGettingReady', 'Brendan: getting-ready branch')},
        {1: ('sign', 'PlayersHouse_2F_Text_Notebook'), 3: ('sign', 'PlayersHouse_2F_Text_ItsAGameCube')}),
    'LittlerootTown_MaysHouse_1F': (EMERALD, {
        3: ('npc', 'RivalsHouse_1F_Text_ShouldGoHomeEverySoOften', 'Birch\'s wife: branch for a travelling trainer'),
        5: ('npc', 'RivalsHouse_1F_Text_DoYouHavePokemon', 'May\'s little brother')}, {}),
    'LittlerootTown_MaysHouse_2F': (EMERALD, {
        0: ('npc', 'RivalsHouse_2F_Text_MayGettingReady', 'May: getting-ready branch')},
        {0: ('sign', 'PlayersHouse_2F_Text_ItsAGameCube'), 1: ('sign', 'PlayersHouse_2F_Text_Notebook')}),
    'NewBarkTown': (HNS, {
        0: ('npc', 'NewBarkTown_Text_FatManState3', 'default branch'),
        1: ('npc', 'NewBarkTown_Text_LassState3', 'branch for a player who already has Pokemon'),
        2: ('silver', ('NewBarkTown_Text_Silver1', 'NewBarkTown_Text_Silver2'), 'Silver: both lines and the exclamation; the push movement is left out because it only fits one approach cell')},
        {0: ('sign', 'NewBarkTown_Text_Sign'), 1: ('sign', 'NewBarkTown_Text_Door'),
         3: ('sign', 'NewBarkTown_Text_ElmsHouse'), 4: ('sign', 'NewBarkTown_Text_LabSign')}),
    'Route29': (HNS, {
        0: ('npc', 'Route29_Text_OldMan1', 'branch before the source adventure flag'),
        1: ('npc', 'Route29_Text_Youngster', ''),
        2: ('npc', 'Route29_Text_Boy_WaitingForDay', 'day branch; FireRed has no day/night'),
        4: ('npc', 'Route29_Text_Girl', ''),
        5: ('cuttree', None, 'FireRed EventScript_CutTree'),
        6: ('npc', 'Route29_Text_Lass', ''),
        7: ('item', None, 'Potion')},
        {0: ('sign', 'Route29_Text_Sign1'), 1: ('sign', 'Route29_Text_Sign1')}),
    'NewBarkTown_House1': (HNS, {
        0: ('npc', 'NewBarkTown_House1_Text_NeighborsDaughter', ''),
        1: ('npc', 'NewBarkTown_House1_Text_Neighbor', 'source hides her during the intro; always shown')},
        {0: ('sign', 'Text_BookShelf'), 1: ('sign', 'Text_BookShelf')}),
    'NewBarkTown_House2': (HNS, {
        0: ('npc', 'NewBarkTown_House2_Text_ElmsSon', ''), 1: ('npc', 'NewBarkTown_House2_Text_ElmsWife', '')},
        {0: ('sign', 'Text_BookShelf'), 1: ('sign', 'Text_BookShelf')}),
    'Gate_Route29_Route46': (HNS, {
        0: ('npc', 'Gate_Route29_Route46_Text_Officer', ''), 1: ('npc', 'Gate_Route29_Route46_Text_Youngster', '')}, {}),
    'NewBarkTown_Lab': (HNS, {
        0: ('npc', 'NewBarkTown_Lab_Text_AideAlwaysBusy', 'source hides him during the story; always shown'),
        1: ('silent', None, 'Prof. Elm: every source branch depends on the story (eggs, phone, gifts)'),
        2: ('npc', 'NewBarkTown_Lab_Text_AideSave', 'default branch')},
        'lab'),
    'NewBarkTown_PlayersHouse_1F': (HNS, {
        0: ('silent', None, 'mother: every source line speaks to her own child'),
        1: ('npc', 'NewBarkTown_PlayersHouse_1F_Text_MomsFriend', 'source shows her only during the intro; always shown')}, {}),
    'NewBarkTown_PlayersHouse_2F': (HNS, {}, {2: ('sign', 'NewBarkTown_PlayersHouse_2F_Text_NES'),
                                             3: ('sign', 'Text_BookShelf'), 4: ('sign', 'Text_BookShelf')}),
}
# Lab furniture scripts (Emerald's labels, also reused by HNS) -> text shown.
LAB_SIGNS = {
    'LittlerootTown_ProfessorBirchsLab_EventScript_Machine': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine'),
    'LittlerootTown_ProfessorBirchsLab_EventScript_Bookshelf': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon'),
    'LittlerootTown_ProfessorBirchsLab_EventScript_PC': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch'),
    'LittlerootTown_ProfessorBirchsLab_EventScript_Book': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead'),
    'NewBarkTown_Lab_EventScript_HealingMachine1': ('heal', 'LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine'),
    'EventScript_EmptyTrashCan': ('sign', 'Text_EmptyTrashCan'),
}


def source_text(repo, label):
    """The .string lines of a text label (single or double colon), anywhere in the repo's data folder."""
    pattern = re.compile(r'^' + re.escape(label) + r'::?\s*\n((?:[ \t]*\.string "[^"]*"[ \t]*\n?)+)', re.M)
    for path in sorted((repo / 'data').rglob('*.inc')):
        match = pattern.search(path.read_text(encoding='utf-8'))
        if match:
            return re.findall(r'\.string "([^"]*)"', match[1]), path.relative_to(repo).as_posix()
    raise KeyError(f'{repo.name}: text {label} not found')


def check_charmap(text, charmap, label):
    tokens = set(re.findall(r'\{([^}]+)\}', text))
    plain = re.sub(r'\{[^}]+\}|\\[npl]|\$', '', text)
    missing = {t for t in tokens if t not in charmap['tokens']} | {c for c in plain if c not in charmap['chars']}
    if missing:
        raise ValueError(f'{label}: characters not in FireRed charmap: {sorted(missing)}')


def load_charmap(game):
    chars, tokens = set(), set()
    for line in (game / 'charmap.txt').read_text(encoding='utf-8').splitlines():
        m = re.match(r"^'((?:\\.|[^'])*)'\s*=", line)
        if m:
            chars.add(m[1].replace("\\'", "'"))
        m = re.match(r'^([A-Z_0-9]+)\s*=', line)
        if m:
            tokens.add(m[1])
    return {'chars': chars, 'tokens': tokens}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--references', type=Path, required=True)
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    g, charmap = a.game, load_charmap(a.game)
    flags = g / 'include/constants/flags.h'
    text = flags.read_text(encoding='utf-8')
    if FLAG_POTION not in text:
        text = text.replace('#define FLAG_EXP_SHARE_DISABLED  FLAG_0x021\n',
                            '#define FLAG_EXP_SHARE_DISABLED  FLAG_0x021\n'
                            f'#define {FLAG_POTION} FLAG_0x077 // Region pilot item (scripts/install_pilot_events.py)\n', 1)
        flags.write_text(text, encoding='utf-8', newline='\n')
    report = {'status': 'AMBIENT_EVENTS_PHASE_1', 'maps': {}}
    for source_map, (repo_name, objects, signs) in EVENTS.items():
        repo = a.references / repo_name
        region = 'Hoenn' if repo_name == EMERALD else 'Johto'
        name = f'{region}_{source_map}'
        header = json.loads((repo / f'data/maps/{source_map}/map.json').read_text(encoding='utf-8'))
        source_bgs = header.get('bg_events') or []
        if signs == 'lab':
            signs = {i: LAB_SIGNS[b['script']] for i, b in enumerate(source_bgs) if b['script'] in LAB_SIGNS}
        scripts, texts, obj_events, bg_events, rows = [], {}, [], [], []

        def text_label(label):
            local = f'{name}_Text_{label.split("_Text_")[-1] if "_Text_" in label else label.removeprefix("Text_")}'
            if local in texts and texts[local][2] != label:
                raise ValueError(f'{local}: two source texts share a label ({texts[local][2]}, {label})')
            if local not in texts:
                lines, where = source_text(repo, label)
                check_charmap(''.join(lines), charmap, label)
                texts[local] = (lines, where, label)
            return local

        for index, (kind, label, note) in sorted(objects.items()):
            o = header['object_events'][index]
            gfx = REPO_SPRITES.get((repo_name, o['graphics_id'])) or SPRITES[o['graphics_id']]
            script, flag = f'{name}_EventScript_Object{index}', '0'
            if kind == 'npc':
                scripts.append(f'{script}::\n\tmsgbox {text_label(label)}, MSGBOX_NPC\n\tend\n')
            elif kind == 'silent':
                script = '0x0'
            elif kind == 'cry':  # Vigoroth: cry, then its line
                species, line = label
                scripts.append(f'{script}::\n\tlock\n\tfaceplayer\n\twaitse\n\tplaymoncry {species}, CRY_MODE_NORMAL\n'
                               f'\tmsgbox {text_label(line)}, MSGBOX_DEFAULT\n\twaitmoncry\n\trelease\n\tend\n')
            elif kind == 'silver':  # HNS NewBarkTown_EventScript_Silver without the push
                first, second = label
                scripts.append(f'{script}::\n\tlock\n\tmsgbox {text_label(first)}, MSGBOX_DEFAULT\n\tdelay 60\n\tplayse SE_PIN\n'
                               f'\tapplymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark\n\tfaceplayer\n\twaitmovement 0\n'
                               f'\tmsgbox {text_label(second)}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
            elif kind == 'item':
                scripts.append(f'{script}::\n\tfinditem ITEM_POTION\n\tend\n')
                flag = FLAG_POTION
            elif kind == 'cuttree':
                script, flag = 'EventScript_CutTree', o['flag']
            x, y, movement, placed_by = POSITIONS.get((source_map, index), (o['x'], o['y'], o['movement_type'], None))
            obj_events.append({'type': 'object', 'graphics_id': gfx, 'x': x, 'y': y, 'elevation': o['elevation'],
                               'movement_type': movement, 'movement_range_x': o['movement_range_x'],
                               'movement_range_y': o['movement_range_y'], 'trainer_type': 'TRAINER_TYPE_NONE',
                               'trainer_sight_or_berry_tree_id': '0', 'script': script, 'flag': flag})
            rows.append({'kind': kind, 'source_index': index, 'source_script': o.get('script'), 'source_sprite': o['graphics_id'],
                         'firered_sprite': gfx, 'text': label, 'note': note, 'x': x, 'y': y,
                         **({'placed_by': placed_by} if placed_by else {})})
        if source_map in FERRIES:
            port, x, y = FERRIES[source_map]
            script = f'{name}_EventScript_Ferry'
            scripts.append(f'{script}::\n\tlock\n\tfaceplayer\n\tmsgbox {name}_Text_Ferry, MSGBOX_YESNO\n'
                           f'\tgoto_if_eq VAR_RESULT, NO, {script}_Stay\n\tsetvar VAR_0x8004, {port}\n'
                           f'\tsetvar VAR_0x8006, SEAGALLOP_VERMILION_CITY\n\tgoto EventScript_SailToDest\n\tend\n\n'
                           f'{script}_Stay::\n\trelease\n\tend\n')
            texts[f'{name}_Text_Ferry'] = (FERRY_TEXT, 'new text (no source)', 'Ferry')
            obj_events.append({'type': 'object', 'graphics_id': 'OBJ_EVENT_GFX_SAILOR', 'x': x, 'y': y, 'elevation': 3,
                               'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 1, 'movement_range_y': 1,
                               'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0',
                               'script': script, 'flag': '0'})
            rows.append({'kind': 'ferry', 'source_index': None, 'firered_sprite': 'OBJ_EVENT_GFX_SAILOR', 'x': x, 'y': y,
                         'note': f'new: SEAGALLOP back to Vermilion City ({port})'})
        for index, (kind, label) in sorted(signs.items()):
            b = source_bgs[index]
            script = f'{name}_EventScript_Sign{index}'
            if kind == 'heal':
                scripts.append(f'{script}::\n\tlockall\n\tgoto_if_set FLAG_SYS_POKEMON_GET, {script}_Heal\n'
                               f'\tmsgbox {text_label(label)}, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                               f'{script}_Heal::\n\tfadescreen FADE_TO_BLACK\n\tplayfanfare MUS_HEAL\n\twaitfanfare\n'
                               f'\tspecial HealPlayerParty\n\tfadescreen FADE_FROM_BLACK\n\treleaseall\n\tend\n')
            else:
                scripts.append(f'{script}::\n\tmsgbox {text_label(label)}, MSGBOX_SIGN\n\tend\n')
            bg_events.append({'type': 'sign', 'x': b['x'], 'y': b['y'], 'elevation': b['elevation'],
                              'player_facing_dir': b.get('player_facing_dir', 'BG_EVENT_PLAYER_FACING_ANY'), 'script': script})
            rows.append({'kind': kind, 'source_index': index, 'source_script': b['script'], 'text': label, 'x': b['x'], 'y': b['y']})
        map_path = g / f'data/maps/{name}/map.json'
        current = json.loads(map_path.read_text(encoding='utf-8'))
        current['object_events'], current['bg_events'] = obj_events, bg_events
        map_path.write_text(json.dumps(current, indent=2) + '\n', encoding='utf-8', newline='\n')
        body = [f'@ Region pilot, phase 1 (ambient). Generated by scripts/install_pilot_events.py from {repo_name}.',
                f'{name}_MapScripts::', '\t.byte 0', '']
        body += scripts
        for local, (lines, where, label) in texts.items():
            origin = where if where.startswith('new text') else f'{repo_name} {where}: {label}'
            body.append(f'@ {origin}\n{local}::\n' + ''.join(f'\t.string "{l}"\n' for l in lines))
        (map_path.parent / 'scripts.inc').write_text('\n'.join(body), encoding='utf-8', newline='\n')
        report['maps'][name] = {'source': f'{repo_name}/{source_map}', 'events': rows}
    a.report.write_text(json.dumps(report, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    print({k: len(v['events']) for k, v in report['maps'].items()})


if __name__ == '__main__':
    main()
