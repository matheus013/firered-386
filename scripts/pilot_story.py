"""Region pilot story, phase 3 (used by install_pilot_events.py).

Step 2, Hoenn: Prof. Birch's rescue on Route 101, the starter gift from his bag, Birch and his aide
following the story, and a battle with May in her room. The scenes follow pokeemerald's scripts
(data/maps/Route101, LittlerootTown_ProfessorBirchsLab, Route103) with their texts; what differs is
written down next to each piece:

- FireRed has no graphic starter chooser (Emerald's special ChooseStarter), so the bag asks about each
  starter with a yes/no box. That question and the "received" line are new texts.
- The Zigzagoon battle and May use post-League levels (the pilot is reached after the first Hall of
  Fame). The player battles with their own team; the starter is level 5 as in Emerald.
- Emerald's chase movements need a fresh scene; here Birch and the Zigzagoon jog in place where the
  source map places them, and the player steps up into the scene as in Route101_Movement_EnterScene.
- {RIVAL} in Emerald is the neighbour; FireRed's {RIVAL} is the Kanto rival, so it is written as MAY.

Story state, VAR_HOENN_STORY: 0 nothing, 1 rescue started, 2 starter chosen (Zigzagoon battle pending,
also after a blackout), 3 Birch rescued, 4 May beaten. VAR_HOENN_STARTER: 0 Treecko, 1 Torchic, 2 Mudkip.
"""
import re
from pathlib import Path

VARS = {'VAR_HOENN_STORY': 'VAR_0x4029', 'VAR_HOENN_STARTER': 'VAR_0x402A'}
FLAGS = {'FLAG_HIDE_HOENN_ROUTE101_RESCUE': 'FLAG_0x022', 'FLAG_HIDE_HOENN_BIRCH_LAB_BIRCH': 'FLAG_0x023'}
ZIGZAGOON_LEVEL = 84  # post-League band (provisional until the expansion rebalance)
STARTERS = [('TREECKO', 'TRAINER_MAY_2'), ('TORCHIC', 'TRAINER_MAY_3'), ('MUDKIP', 'TRAINER_MAY')]  # May's team per player starter
SUBSTITUTIONS = {'{RIVAL}': 'MAY'}
NEW_TEXTS = {
    'ChooseStarter': ['Do you choose {STR_VAR_1}?$'],
    'ReceivedStarter': ['{PLAYER} received the {STR_VAR_1}!$'],
}
# Objects from the source maps whose script is written here (source index -> flag).
OBJECTS = {
    'Route101': {1: 'FLAG_HIDE_HOENN_ROUTE101_RESCUE', 2: 'FLAG_HIDE_HOENN_ROUTE101_RESCUE', 3: 'FLAG_HIDE_HOENN_ROUTE101_RESCUE'},
    'LittlerootTown_ProfessorBirchsLab': {0: '0', 1: 'FLAG_HIDE_HOENN_BIRCH_LAB_BIRCH'},
    'LittlerootTown_MaysHouse_2F': {0: '0'},
}
# Emerald Route101 triggers: start the rescue, then keep the player near Birch until it is over.
COORDS = {'Route101': [(10, 19, 0, 'StartRescue'), (11, 19, 0, 'StartRescue')]
          + [(x, y, v, side) for v in (1, 2) for x, y, side in
             [(10, 18, 'PreventExitSouth'), (11, 18, 'PreventExitSouth'), (6, 15, 'PreventExitWest'), (6, 16, 'PreventExitWest'),
              (6, 17, 'PreventExitWest'), (6, 18, 'PreventExitWest'), (7, 13, 'PreventExitNorth')]]}


def install_constants(game: Path):
    for header, names in ((game / 'include/constants/vars.h', VARS), (game / 'include/constants/flags.h', FLAGS)):
        text = header.read_text(encoding='utf-8')
        added = ''.join(f'#define {alias} {real} // Region pilot story (scripts/pilot_story.py)\n'
                        for alias, real in names.items() if f'#define {alias} ' not in text)
        if added:
            anchor = '#define FLAG_HIDE_JOHTO_ROUTE29_POTION' if 'flags' in header.name else '#define VAR_0x4029 '
            line = next(l for l in text.splitlines(keepends=True) if l.startswith(anchor))
            header.write_text(text.replace(line, line + added if 'flags' in header.name else added + line, 1), encoding='utf-8', newline='\n')


def object_script(source_map, index, name, text, local_id):
    """Script for a story object; text(label) brings in a source text, local_id(source index) its local id."""
    n = name
    if source_map == 'Route101':
        if index != 2:
            return None  # Birch and the Zigzagoon only take part in the scene
        birch, zigzagoon, bag = local_id(1), local_id(3), local_id(2)
        out = [f'{n}_EventScript_Object2::\n\tlock\n\tgoto_if_eq VAR_HOENN_STORY, 2, {n}_EventScript_ZigzagoonBattle\n']
        for i, (starter, _) in enumerate(STARTERS):
            out.append(f'\tbufferspeciesname STR_VAR_1, SPECIES_{starter}\n\tmsgbox {n}_Text_ChooseStarter, MSGBOX_YESNO\n'
                       f'\tgoto_if_eq VAR_RESULT, YES, {n}_EventScript_Take{starter.title()}\n')
        out.append('\trelease\n\tend\n')
        for i, (starter, _) in enumerate(STARTERS):
            out.append(f'\n{n}_EventScript_Take{starter.title()}::\n\tsetvar VAR_HOENN_STARTER, {i}\n\tsetvar VAR_TEMP_1, SPECIES_{starter}\n'
                       f'\tgivemon SPECIES_{starter}, 5\n\tgoto {n}_EventScript_ReceivedStarter\n\tend\n')
        out.append(f'\n{n}_EventScript_ReceivedStarter::\n\tgoto_if_eq VAR_RESULT, 2, EventScript_NoMoreRoomForPokemon\n'
                   f'\tcopyvar VAR_0x8008, VAR_RESULT\n\tbufferspeciesname STR_VAR_1, VAR_TEMP_1\n\tplayfanfare MUS_LEVEL_UP\n'
                   f'\tmessage {n}_Text_ReceivedStarter\n\twaitmessage\n\twaitfanfare\n'
                   f'\tcall_if_eq VAR_0x8008, 1, EventScript_TransferredToPC\n\tsetvar VAR_HOENN_STORY, 2\n'
                   f'\tgoto {n}_EventScript_ZigzagoonBattle\n\tend\n')
        out.append(f'\n{n}_EventScript_ZigzagoonBattle::\n\tclosemessage\n\tsetwildbattle SPECIES_ZIGZAGOON, {ZIGZAGOON_LEVEL}\n'
                   f'\tdowildbattle\n\tapplymovement {birch}, Common_Movement_FacePlayer\n\twaitmovement 0\n'
                   f'\tmsgbox {text("Route101_Text_YouSavedMe")}, MSGBOX_DEFAULT\n\tclosemessage\n\tfadescreen FADE_TO_BLACK\n'
                   f'\tsetflag FLAG_HIDE_HOENN_ROUTE101_RESCUE\n\tremoveobject {birch}\n\tremoveobject {zigzagoon}\n\tremoveobject {bag}\n'
                   f'\tsetvar VAR_HOENN_STORY, 3\n\tspecial HealPlayerParty\n\tfadescreen FADE_FROM_BLACK\n\trelease\n\tend\n')
        return ''.join(out)
    if source_map == 'LittlerootTown_ProfessorBirchsLab':
        # aide: away on fieldwork until the rescue; Birch (shown from state 3): go see May, then the closing line
        before, after, later = (('BirchAwayOnFieldwork', 'BirchIsntOneForDeskWork', 3) if index == 0 else
                                ('MightBeGoodIdeaToGoSeeRival', 'CountlessPokemonAwait', 4))
        before, after = (f'LittlerootTown_ProfessorBirchsLab_Text_{t}' for t in (before, after))
        s = f'{n}_EventScript_Object{index}'
        return (f'{s}::\n\tlock\n\tfaceplayer\n\tgoto_if_ge VAR_HOENN_STORY, {later}, {s}_Later\n'
                f'\tmsgbox {text(before)}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                f'{s}_Later::\n\tmsgbox {text(after)}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
    if source_map == 'LittlerootTown_MaysHouse_2F':
        s = f'{n}_EventScript_Object0'
        out = [f'{s}::\n\tlock\n\tfaceplayer\n\tgoto_if_eq VAR_HOENN_STORY, 3, {s}_Battle\n\tgoto_if_ge VAR_HOENN_STORY, 4, {s}_After\n'
               f'\tmsgbox {text("RivalsHouse_2F_Text_MayGettingReady")}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
               f'{s}_Battle::\n\tmsgbox {text("Route103_Text_MayLetsBattle")}, MSGBOX_DEFAULT\n\tclosemessage\n']
        for i, (starter, _) in enumerate(STARTERS[:-1]):
            out.append(f'\tgoto_if_eq VAR_HOENN_STARTER, {i}, {s}_Vs{starter.title()}\n')
        out.append(f'\tgoto {s}_Vs{STARTERS[-1][0].title()}\n\tend\n')
        for starter, trainer in STARTERS:
            out.append(f'\n{s}_Vs{starter.title()}::\n\ttrainerbattle_no_intro {trainer}, {text("Route103_Text_MayDefeated")}\n\tgoto {s}_Beaten\n\tend\n')
        out.append(f'\n{s}_Beaten::\n\tsetvar VAR_HOENN_STORY, 4\n\tgoto {s}_After\n\tend\n\n'
                   f'{s}_After::\n\tmsgbox {text("Route103_Text_MayTimeToHeadBack")}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
        return ''.join(out)
    return None


def map_extras(source_map, name, text, add_text):
    """(map script header lines, extra scripts, coord events) for a pilot map."""
    if source_map == 'Route101':
        n = name
        for key, lines in NEW_TEXTS.items():
            add_text(f'{n}_Text_{key}', lines)
        scripts = [f'{n}_EventScript_StartRescue::\n\tlockall\n\tmsgbox {text("Route101_Text_HelpMe")}, MSGBOX_DEFAULT\n\tclosemessage\n'
                   f'\tapplymovement OBJ_EVENT_ID_PLAYER, {n}_Movement_EnterScene\n\twaitmovement 0\n'
                   f'\tmsgbox {text("Route101_Text_PleaseHelp")}, MSGBOX_DEFAULT\n\tclosemessage\n\tsetvar VAR_HOENN_STORY, 1\n\treleaseall\n\tend\n']
        for side, step in (('South', 'walk_up'), ('West', 'walk_right'), ('North', 'walk_down')):
            scripts.append(f'{n}_EventScript_PreventExit{side}::\n\tlockall\n\tmsgbox {text("Route101_Text_DontLeaveMe")}, MSGBOX_DEFAULT\n'
                           f'\tclosemessage\n\tapplymovement OBJ_EVENT_ID_PLAYER, {n}_Movement_PreventExit{side}\n\twaitmovement 0\n\treleaseall\n\tend\n')
            scripts.append(f'{n}_Movement_PreventExit{side}:\n\t{step}\n\tstep_end\n')
        scripts.append(f'{n}_Movement_EnterScene:\n' + '\twalk_up\n' * 4 + '\twalk_in_place_faster_left\n\tstep_end\n')
        coords = [{'type': 'trigger', 'x': x, 'y': y, 'elevation': 3, 'var': 'VAR_HOENN_STORY', 'var_value': str(v),
                   'script': f'{n}_EventScript_{script}'} for x, y, v, script in COORDS['Route101']]
        return None, scripts, coords
    if source_map == 'LittlerootTown_ProfessorBirchsLab':
        header = [f'{name}_MapScripts::', f'\tmap_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition', '\t.byte 0', '',
                  f'{name}_OnTransition:', '\tsetflag FLAG_HIDE_HOENN_BIRCH_LAB_BIRCH',
                  f'\tgoto_if_lt VAR_HOENN_STORY, 3, {name}_OnTransitionEnd', '\tclearflag FLAG_HIDE_HOENN_BIRCH_LAB_BIRCH',
                  f'{name}_OnTransitionEnd:', '\tend', '']
        return header, [], []
    return None, [], []


def substitute(lines):
    out = []
    for line in lines:
        for old, new in SUBSTITUTIONS.items():
            line = line.replace(old, new)
        out.append(line)
    return out
