"""Reduced Elm research request, using HNS_modern's original dialogue.

No Mr. Pokemon/egg errand until those maps exist. State: 0 not accepted,
1 choose a starter, 2 starter delivered, 3 Silver defeated.
Gift progress is committed only after givemon succeeds, including PC delivery.
"""

VARS = {'VAR_JOHTO_STORY': 'VAR_0x402B', 'VAR_JOHTO_STARTER': 'VAR_0x402C'}
FLAGS = {
    'FLAG_HIDE_JOHTO_CHIKORITA_BALL': 'FLAG_0x024',
    'FLAG_HIDE_JOHTO_CYNDAQUIL_BALL': 'FLAG_0x025',
    'FLAG_HIDE_JOHTO_TOTODILE_BALL': 'FLAG_0x026',
}
STARTERS = ('CHIKORITA', 'CYNDAQUIL', 'TOTODILE')
OBJECTS = {'NewBarkTown_Lab': {1: '0', **{
    i + 3: f'FLAG_HIDE_JOHTO_{species}_BALL' for i, species in enumerate(STARTERS)}},
    'NewBarkTown': {2: '0'}}


def object_script(source_map, index, name, text, local_id):
    if source_map == 'NewBarkTown' and index == 2:
        s = f'{name}_EventScript_Object2'
        before = text('NewBarkTown_Text_Silver1')
        second = text('NewBarkTown_Text_Silver2')
        intro = text('CherrygroveCity_Text_RivalSeen')
        beaten = text('CherrygroveCity_Text_SilverWin')
        after = text('CherrygroveCity_Text_RivalAfter')
        out = [f'{s}::\n\tlock\n\tgoto_if_ge VAR_JOHTO_STORY, 3, {s}_After\n'
               f'\tgoto_if_eq VAR_JOHTO_STORY, 2, {s}_Battle\n'
               f'\tmsgbox {before}, MSGBOX_DEFAULT\n\tdelay 60\n\tplayse SE_PIN\n'
               f'\tapplymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark\n\tfaceplayer\n\twaitmovement 0\n'
               f'\tmsgbox {second}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
               f'{s}_Battle::\n\tfaceplayer\n\tmsgbox {intro}, MSGBOX_DEFAULT\n\tclosemessage\n'
               f'\tgoto_if_eq VAR_JOHTO_STARTER, 0, {s}_VsChikorita\n'
               f'\tgoto_if_eq VAR_JOHTO_STARTER, 1, {s}_VsCyndaquil\n'
               f'\tgoto {s}_VsTotodile\n\tend\n']
        for starter, trainer in zip(STARTERS, ('TRAINER_BRENDAN','TRAINER_BRENDAN_2','TRAINER_BRENDAN_3')):
            out.append(f'{s}_Vs{starter.title()}::\n\ttrainerbattle_no_intro {trainer}, {beaten}\n'
                       f'\tsetvar VAR_JOHTO_STORY, 3\n\tgoto {s}_After\n\tend\n')
        out.append(f'{s}_After::\n\tfaceplayer\n\tmsgbox {after}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
        return '\n'.join(out)
    if source_map != 'NewBarkTown_Lab':
        return None
    s = f'{name}_EventScript_Object{index}'
    def line(label):
        return text('NewBarkTown_Lab_Text_' + label)
    if index == 1:
        return (f'{s}::\n\tlock\n\tfaceplayer\n'
            f'\tgoto_if_ge VAR_JOHTO_STORY, 2, {s}_Research\n'
            f'\tgoto_if_eq VAR_JOHTO_STORY, 1, {s}_Choose\n'
            f'\tmsgbox {line("ElmIntro")}, MSGBOX_YESNO\n'
            f'\tgoto_if_eq VAR_RESULT, NO, {s}_Refused\n'
            f'\tmsgbox {line("ElmAccepted")}, MSGBOX_DEFAULT\n'
            f'\tsetvar VAR_JOHTO_STORY, 1\n'
            f'{s}_Choose::\n\tmsgbox {line("ElmChooseAPokemon")}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            f'{s}_Research::\n\tmsgbox {line("ElmResearchAmbitions")}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            f'{s}_Refused::\n\tmsgbox {line("ElmRefused")}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
    if index not in (3, 4, 5):
        raise ValueError(f'Unsupported Johto story object: {index}')
    choice = index - 3
    species = STARTERS[choice]
    return (f'{s}::\n\tlock\n'
        f'\tgoto_if_ne VAR_JOHTO_STORY, 1, {s}_Unavailable\n'
        f'\tbufferspeciesname STR_VAR_1, SPECIES_{species}\n'
        f'\tsetvar VAR_0x8004, SPECIES_{species}\n\tspecial BufferMonCategory\n'
        f'\tmsgbox {line("Choose" + species.title())}, MSGBOX_YESNO\n'
        f'\tgoto_if_eq VAR_RESULT, NO, {s}_Declined\n'
        f'\tsetvar VAR_TEMP_1, SPECIES_{species}\n\tgivemon SPECIES_{species}, 5\n'
        f'\tgoto_if_eq VAR_RESULT, 2, EventScript_NoMoreRoomForPokemon\n'
        f'\tcopyvar VAR_0x8008, VAR_RESULT\n'
        f'\tsetvar VAR_JOHTO_STARTER, {choice}\n\tsetvar VAR_JOHTO_STORY, 2\n'
        f'\tsetflag FLAG_HIDE_JOHTO_{species}_BALL\n\tremoveobject {local_id(index)}\n'
        f'\tbufferspeciesname STR_VAR_1, SPECIES_{species}\n\tplayfanfare MUS_LEVEL_UP\n'
        f'\tmessage {line("ReceivedStarter")}\n\twaitmessage\n\twaitfanfare\n'
        f'\tcall_if_eq VAR_0x8008, 1, EventScript_TransferredToPC\n'
        f'\tmsgbox {line("ChoseStarter")}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
        f'{s}_Declined::\n\tmsgbox {line("DidntChooseStarter")}, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
        f'{s}_Unavailable::\n\tmsgbox {line("ShouldntTouch")}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
