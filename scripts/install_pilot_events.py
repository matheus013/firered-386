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

import pilot_story

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
    'OBJ_EVENT_GFX_POKE_BALL': 'OBJ_EVENT_GFX_ITEM_BALL', 'OBJ_EVENT_GFX_ITEM_BALL': 'OBJ_EVENT_GFX_ITEM_BALL',
    # Phase 2: main characters. FireRed already has the Ruby/Sapphire Brendan and May.
    'OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL': 'OBJ_EVENT_GFX_RS_BRENDAN', 'OBJ_EVENT_GFX_RIVAL_MAY_NORMAL': 'OBJ_EVENT_GFX_RS_MAY',
    'OBJ_EVENT_GFX_WOMAN_4': 'OBJ_EVENT_GFX_WOMAN_4', 'OBJ_EVENT_GFX_NINJA_BOY': 'OBJ_EVENT_GFX_NINJA_BOY',
    'OBJ_EVENT_GFX_PROF_BIRCH': 'OBJ_EVENT_GFX_PROF_BIRCH', 'OBJ_EVENT_GFX_PROF_ELM': 'OBJ_EVENT_GFX_PROF_ELM',
    'OBJ_EVENT_GFX_SILVER': 'OBJ_EVENT_GFX_SILVER',
    'OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX': 'OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX',
    'OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY': 'OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY',
    # Hoenn batch 1
    'OBJ_EVENT_GFX_GIRL_3': 'OBJ_EVENT_GFX_LASS', 'OBJ_EVENT_GFX_MART_EMPLOYEE': 'OBJ_EVENT_GFX_CLERK',
    'OBJ_EVENT_GFX_MANIAC': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_MAN_3': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_BOY_1': 'OBJ_EVENT_GFX_BOY',
    'OBJ_EVENT_GFX_BUG_CATCHER': 'OBJ_EVENT_GFX_BUG_CATCHER', 'OBJ_EVENT_GFX_FISHERMAN': 'OBJ_EVENT_GFX_FISHER',
    'OBJ_EVENT_GFX_POKEFAN_M': 'OBJ_EVENT_GFX_FAT_MAN', 'OBJ_EVENT_GFX_BLACK_BELT': 'OBJ_EVENT_GFX_BLACK_BELT',
    'OBJ_EVENT_GFX_MAN_5': 'OBJ_EVENT_GFX_ROCKER', 'OBJ_EVENT_GFX_SWIMMER_F': 'OBJ_EVENT_GFX_SWIMMER_F_WATER',
    'OBJ_EVENT_GFX_SWIMMER_M': 'OBJ_EVENT_GFX_SWIMMER_M_WATER', 'OBJ_EVENT_GFX_GENTLEMAN': 'OBJ_EVENT_GFX_GENTLEMAN',
    'OBJ_EVENT_GFX_FISHER': 'OBJ_EVENT_GFX_FISHER', 'OBJ_EVENT_GFX_BATTLE_GIRL': 'OBJ_EVENT_GFX_COOLTRAINER_F',
    'OBJ_EVENT_GFX_BALDING_MAN': 'OBJ_EVENT_GFX_BALDING_MAN', 'OBJ_EVENT_GFX_PICNICKER': 'OBJ_EVENT_GFX_PICNICKER',
    'OBJ_EVENT_GFX_CAMPER': 'OBJ_EVENT_GFX_CAMPER', 'OBJ_EVENT_GFX_ROCKER': 'OBJ_EVENT_GFX_ROCKER', 'OBJ_EVENT_GFX_HIKER': 'OBJ_EVENT_GFX_HIKER', 'OBJ_EVENT_GFX_OLD_MAN_1': 'OBJ_EVENT_GFX_OLD_MAN_1',
    'OBJ_EVENT_GFX_FIREBREATHER': 'OBJ_EVENT_GFX_BIKER', 'OBJ_EVENT_GFX_OLD_WOMAN': 'OBJ_EVENT_GFX_OLD_WOMAN', 'OBJ_EVENT_GFX_BEAUTY': 'OBJ_EVENT_GFX_BEAUTY', 'OBJ_EVENT_GFX_HEX_MANIAC': 'OBJ_EVENT_GFX_CHANNELER', 'OBJ_EVENT_GFX_GAMEBOY_KID': 'OBJ_EVENT_GFX_GBA_KID', 'OBJ_EVENT_GFX_CYCLING_TRIATHLETE_M': 'OBJ_EVENT_GFX_COOLTRAINER_M', 'OBJ_EVENT_GFX_SWIMMER_F_WATER': 'OBJ_EVENT_GFX_SWIMMER_F_WATER', 'OBJ_EVENT_GFX_SWIMMER_M_WATER': 'OBJ_EVENT_GFX_SWIMMER_M_WATER', 'OBJ_EVENT_GFX_ENGINEER': 'OBJ_EVENT_GFX_WORKER_M', 'OBJ_EVENT_GFX_HOT_SPRINGS_OLD_WOMAN': 'OBJ_EVENT_GFX_OLD_WOMAN', 'OBJ_EVENT_GFX_TUBER_M': 'OBJ_EVENT_GFX_TUBER_M_LAND', 'OBJ_EVENT_GFX_TUBER_M_SWIMMING': 'OBJ_EVENT_GFX_TUBER_M_WATER', 'OBJ_EVENT_GFX_TUBER_F': 'OBJ_EVENT_GFX_TUBER_F', 'OBJ_EVENT_GFX_OLD_MAN': 'OBJ_EVENT_GFX_OLD_MAN_1', 'OBJ_EVENT_GFX_EXPERT_F': 'OBJ_EVENT_GFX_OLD_WOMAN', 'OBJ_EVENT_GFX_EXPERT_M': 'OBJ_EVENT_GFX_OLD_MAN_1', 'OBJ_EVENT_GFX_MAN_1': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_COOK': 'OBJ_EVENT_GFX_FAT_MAN', 'OBJ_EVENT_GFX_GIRL_1': 'OBJ_EVENT_GFX_LITTLE_GIRL', 'OBJ_EVENT_GFX_POKEFAN_F': 'OBJ_EVENT_GFX_WOMAN_2', 'OBJ_EVENT_GFX_CYCLING_TRIATHLETE_F': 'OBJ_EVENT_GFX_COOLTRAINER_F', 'OBJ_EVENT_GFX_RUNNING_TRIATHLETE_M': 'OBJ_EVENT_GFX_COOLTRAINER_M', 'OBJ_EVENT_GFX_RUNNING_TRIATHLETE_F': 'OBJ_EVENT_GFX_COOLTRAINER_F', 'OBJ_EVENT_GFX_SCHOOL_KID_M': 'OBJ_EVENT_GFX_BOY', 'OBJ_EVENT_GFX_SAILOR': 'OBJ_EVENT_GFX_SAILOR', 'OBJ_EVENT_GFX_SAGE': 'OBJ_EVENT_GFX_OLD_MAN_2',
    'OBJ_EVENT_GFX_GIRL_2': 'OBJ_EVENT_GFX_LITTLE_GIRL', 'OBJ_EVENT_GFX_BOY_3': 'OBJ_EVENT_GFX_BOY', 'OBJ_EVENT_GFX_MAN_2': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_MAN_4': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_WOMAN_5': 'OBJ_EVENT_GFX_WOMAN_3', 'OBJ_EVENT_GFX_RICH_BOY': 'OBJ_EVENT_GFX_BOY', 'OBJ_EVENT_GFX_DEVON_EMPLOYEE': 'OBJ_EVENT_GFX_SCIENTIST', 'OBJ_EVENT_GFX_LITTLE_GIRL': 'OBJ_EVENT_GFX_LITTLE_GIRL', 'OBJ_EVENT_GFX_JUGGLER': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_PSYCHIC_M': 'OBJ_EVENT_GFX_MAN', 'OBJ_EVENT_GFX_WOMAN_3': 'OBJ_EVENT_GFX_WOMAN_3',
    'OBJ_EVENT_GFX_COOLTRAINER_F': 'OBJ_EVENT_GFX_COOLTRAINER_F', 'OBJ_EVENT_GFX_COOLTRAINER_M': 'OBJ_EVENT_GFX_COOLTRAINER_M', 'OBJ_EVENT_GFX_SUPER_NERD': 'OBJ_EVENT_GFX_POKE_MANIAC',
    # Phase 3: Birch rescue
    'OBJ_EVENT_GFX_ZIGZAGOON_1': 'OBJ_EVENT_GFX_ZIGZAGOON', 'OBJ_EVENT_GFX_BIRCHS_BAG': 'OBJ_EVENT_GFX_BIRCHS_BAG',
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
        1: ('story', None, 'Birch in the rescue scene (pilot_story.py)'),
        2: ('story', None, "Birch's bag: starter gift and Zigzagoon battle (pilot_story.py)"),
        3: ('story', None, 'Zigzagoon in the rescue scene (pilot_story.py)'),
        5: ('npc', 'Route101_Text_WildPokemonInTallGrass', 'source hides him until Birch is rescued; always shown')},
        {0: ('sign', 'Route101_Text_RouteSign')}),
    'LittlerootTown_ProfessorBirchsLab': (EMERALD, {
        0: ('story', None, 'aide: BirchAwayOnFieldwork before the rescue, BirchIsntOneForDeskWork after (pilot_story.py)'),
        1: ('story', None, 'Birch: shown after the rescue; go see May, then CountlessPokemonAwait (pilot_story.py)')},
        'lab'),
    # Family choice (user, 08/11/2026): May is Birch's daughter. May's house = Birch family,
    # Brendan's house = Norman family, as in Emerald with a male player. Cutscene-only objects stay out.
    'LittlerootTown_BrendansHouse_1F': (EMERALD, {
        0: ('story', None, "Brendan's mother: Emerald's rest-and-heal lines, adapted for a visitor (pilot_story.py)"),
        1: ('cry', ('SPECIES_VIGOROTH', 'PlayersHouse_1F_Text_Vigoroth2'), 'mover'),
        2: ('cry', ('SPECIES_VIGOROTH', 'PlayersHouse_1F_Text_Vigoroth1'), 'mover')}, {}),
    'LittlerootTown_BrendansHouse_2F': (EMERALD, {
        0: ('npc', 'RivalsHouse_2F_Text_BrendanGettingReady', 'Brendan: getting-ready branch')},
        {1: ('sign', 'PlayersHouse_2F_Text_Notebook'), 3: ('sign', 'PlayersHouse_2F_Text_ItsAGameCube')}),
    'LittlerootTown_MaysHouse_1F': (EMERALD, {
        3: ('npc', 'RivalsHouse_1F_Text_ShouldGoHomeEverySoOften', 'Birch\'s wife: branch for a travelling trainer'),
        5: ('npc', 'RivalsHouse_1F_Text_DoYouHavePokemon', 'May\'s little brother')}, {}),
    'LittlerootTown_MaysHouse_2F': (EMERALD, {
        0: ('story', None, 'May: getting ready before the rescue, then the battle (pilot_story.py)')},
        {0: ('sign', 'PlayersHouse_2F_Text_ItsAGameCube'), 1: ('sign', 'PlayersHouse_2F_Text_Notebook')}),
    # Hoenn batch 1 (scripts/install_hoenn_batch1.py terrain). Story-only objects (rivals, Birch, Wally,
    # Scott) and berry trees (no berry system in FireRed) stay out.
    'OldaleTown': (EMERALD, {
        0: ('npc', 'OldaleTown_Text_SavingMyProgress', ''),
        1: ('npc', 'OldaleTown_Text_ThisIsAPokemonMart', 'Mart employee: explanation branch; the Potion sample is a new-trainer gift'),
        2: ('npc', 'OldaleTown_Text_FinishedSketchingFootprints', 'branch after the rival scene')},
        {0: ('sign', 'OldaleTown_Text_TownSign'), 1: ('pcsign', None), 2: ('martsign', None), 3: ('pcsign', None), 4: ('martsign', None)}),
    'Route102': (EMERALD, {
        0: ('npc', 'Route102_Text_ImNotVeryTall', ''), 4: ('npc', 'Route102_Text_CatchWholeBunchOfPokemon', ''),
        5: ('itemflag', None, 'Potion'),
        1: ('trainer', ('TRAINER_HOENN_CALVIN', 'Route102_Text_CalvinIntro', 'Route102_Text_CalvinDefeated', 'Route102_Text_CalvinPostBattle'), 'no Match Call'),
        2: ('trainer', ('TRAINER_HOENN_RICK', 'Route102_Text_RickIntro', 'Route102_Text_RickDefeated', 'Route102_Text_RickPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_TIANA', 'Route102_Text_TianaIntro', 'Route102_Text_TianaDefeated', 'Route102_Text_TianaPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_ALLEN', 'Route102_Text_AllenIntro', 'Route102_Text_AllenDefeated', 'Route102_Text_AllenPostBattle'), '')},
        {0: ('sign', 'Route102_Text_RouteSignPetalburg'), 1: ('sign', 'Route102_Text_RouteSignOldale')}),
    'Route103': (EMERALD, {
        0: ('npc', 'Route103_Text_ShortcutToOldale', ''), 9: ('npc', 'Route103_Text_ShouldHaveBroughtPotion', ''),
        12: ('itemflag', None, 'Guard Spec.'), 19: ('itemflag', None, 'PP Up'),
        13: ('cuttree', None, 'FireRed EventScript_CutTree'), 14: ('cuttree', None, 'FireRed EventScript_CutTree'),
        2: ('trainer', ('TRAINER_HOENN_DAISY', 'Route103_Text_DaisyIntro', 'Route103_Text_DaisyDefeated', 'Route103_Text_DaisyPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_AMY_AND_LIV', 'Route103_Text_LivIntro', 'Route103_Text_LivDefeated', 'Route103_Text_LivPostBattle', 'Route103_Text_LivNotEnoughPokemon'), 'double battle'),
        4: ('trainer', ('TRAINER_HOENN_AMY_AND_LIV', 'Route103_Text_AmyIntro', 'Route103_Text_AmyDefeated', 'Route103_Text_AmyPostBattle', 'Route103_Text_AmyNotEnoughPokemon'), 'double battle'),
        5: ('trainer', ('TRAINER_HOENN_ANDREW', 'Route103_Text_AndrewIntro', 'Route103_Text_AndrewDefeated', 'Route103_Text_AndrewPostBattle'), ''),
        11: ('trainer', ('TRAINER_HOENN_MIGUEL', 'Route103_Text_MiguelIntro', 'Route103_Text_MiguelDefeated', 'Route103_Text_MiguelPostBattle'), 'no Match Call'),
        15: ('trainer', ('TRAINER_HOENN_RHETT', 'Route103_Text_RhettIntro', 'Route103_Text_RhettDefeated', 'Route103_Text_RhettPostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_MARCOS', 'Route103_Text_MarcosIntro', 'Route103_Text_MarcosDefeated', 'Route103_Text_MarcosPostBattle'), ''),
        17: ('trainer', ('TRAINER_HOENN_ISABELLE', 'Route103_Text_IsabelleIntro', 'Route103_Text_IsabelleDefeated', 'Route103_Text_IsabellePostBattle'), 'on water: Surf'),
        18: ('trainer', ('TRAINER_HOENN_PETE', 'Route103_Text_PeteIntro', 'Route103_Text_PeteDefeated', 'Route103_Text_PetePostBattle'), 'on water: Surf')},
        {0: ('sign', 'Route103_Text_RouteSign')}),
    'PetalburgCity': (EMERALD, {
        0: ('npc', 'PetalburgCity_Text_WhereIsWally', "Wally's mother"),
        2: ('npc', 'PetalburgCity_Text_WaterReflection', ''), 3: ('npc', 'PetalburgCity_Text_FullPartyExplanation', ''),
        7: ('npc', 'PetalburgCity_Text_AreYouRookieTrainer', 'Gym boy'),
        5: ('itemflag', None, 'Max Revive'), 6: ('itemflag', None, 'Ether')},
        {0: ('sign', 'PetalburgCity_Text_GymSign'), 1: ('martsign', None), 2: ('pcsign', None), 3: ('sign', 'PetalburgCity_Text_CitySign'),
         4: ('pcsign', None), 5: ('martsign', None), 6: ('sign', 'PetalburgCity_Text_WallyHouseSign')}),
    # Hoenn batch 2. Left out: May/Brendan and Mr. Briney (story), Team Aqua and the Devon employee in the woods
    # and in Rustboro, the berry gifts (berry trees are left out), the White Herb florist, hidden items.
    'Route104': (EMERALD, {
        0: ('npc', 'Route104_Text_WhatsItLikeAtBottomOfSea', ''), 1: ('npc', 'Route104_Text_BrineyLivesInSeasideCottage', ''),
        3: ('npc', 'Route104_Text_ThrowBallAtWeakenedPokemon', ''), 4: ('npc', 'Route104_Text_OnlyThrowBallAtWildPokemon', ''),
        5: ('npc', 'Route104_Text_ImNotATrainer', ''),
        2: ('trainer', ('TRAINER_HOENN_HALEY', 'Route104_Text_HaleyIntro', 'Route104_Text_HaleyDefeat', 'Route104_Text_HaleyPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_IVAN', 'Route104_Text_IvanIntro', 'Route104_Text_IvanDefeat', 'Route104_Text_IvanPostBattle'), ''),
        22: ('trainer', ('TRAINER_HOENN_GINA_AND_MIA', 'Route104_Text_GinaIntro', 'Route104_Text_GinaDefeat', 'Route104_Text_GinaPostBattle', 'Route104_Text_GinaNotEnoughMons'), 'double battle'),
        23: ('trainer', ('TRAINER_HOENN_GINA_AND_MIA', 'Route104_Text_MiaIntro', 'Route104_Text_MiaDefeat', 'Route104_Text_MiaPostBattle', 'Route104_Text_MiaNotEnoughMons'), 'double battle'),
        24: ('trainer', ('TRAINER_HOENN_WINSTON', 'Route104_Text_WinstonIntro', 'Route104_Text_WinstonDefeat', 'Route104_Text_WinstonPostBattle'), ''),
        25: ('trainer', ('TRAINER_HOENN_CINDY', 'Route104_Text_CindyIntro', 'Route104_Text_CindyDefeat', 'Route104_Text_CindyPostBattle'), ''),
        27: ('trainer', ('TRAINER_HOENN_BILLY', 'Route104_Text_BillyIntro', 'Route104_Text_BillyDefeat', 'Route104_Text_BillyPostBattle'), ''),
        32: ('trainer', ('TRAINER_HOENN_DARIAN', 'Route104_Text_DarianIntro', 'Route104_Text_DarianDefeat', 'Route104_Text_DarianPostBattle'), ''),
        20: ('itemflag', None, 'PP Up'), 26: ('itemflag', None, 'Poke Ball'), 28: ('itemflag', None, 'X Accuracy'), 30: ('itemflag', None, 'Potion'),
        29: ('cuttree', None, 'FireRed EventScript_CutTree')},
        {0: ('sign', 'Route104_Text_MrBrineysCottage'), 1: ('sign', 'Route104_Text_RouteSignPetalburg'),
         2: ('sign', 'Route104_Text_RouteSignRustboro'), 3: ('sign', 'Route104_Text_PrettyPetalFlowShop'),
         4: ('sign', 'Route104_Text_TrainerTipsDoubleBattles')}),
    'PetalburgWoods': (EMERALD, {
        0: ('cuttree', None, 'FireRed EventScript_CutTree'), 1: ('cuttree', None, 'FireRed EventScript_CutTree'),
        4: ('itemflag', None, 'Great Ball'), 5: ('itemflag', None, 'X Attack'), 6: ('itemflag', None, 'Ether'), 11: ('itemflag', None, 'Paralyze Heal'),
        7: ('npc', 'PetalburgWoods_Text_StayOutOfTallGrass', ''), 10: ('npc', 'PetalburgWoods_Text_HiddenItemsExplanation', ''),
        8: ('trainer', ('TRAINER_HOENN_LYLE', 'PetalburgWoods_Text_GoBugPokemonTeam', 'PetalburgWoods_Text_ICouldntWin', 'PetalburgWoods_Text_ImOutOfPokeBalls'), ''),
        9: ('trainer', ('TRAINER_HOENN_JAMES', 'PetalburgWoods_Text_InstantlyPopularWithBugPokemon', 'PetalburgWoods_Text_CantBePopularIfILose', 'PetalburgWoods_Text_PeopleRespectYou'), 'no rematch')},
        {0: ('sign', 'PetalburgWoods_Text_TrainerTipsExperience'), 5: ('sign', 'PetalburgWoods_Text_TrainerTipsPP')}),
    'RustboroCity': (EMERALD, {
        0: ('npc', 'RustboroCity_Text_GymLeaderIsntEasyWithFire', ''), 1: ('npc', 'RustboroCity_Text_WeShortenItToDevon', ''),
        2: ('npc', 'RustboroCity_Text_CatchRarePokemonIfIGoToSchool', ''), 3: ('npc', 'RustboroCity_Text_WowYouHavePokemon', ''),
        4: ('npc', 'RustboroCity_Text_MrBrineyWalksInTheTunnel', ''), 5: ('npc', 'RustboroCity_Text_HaveYouChallengedGym', ''),
        6: ('npc', 'RustboroCity_Text_PokemonCanChangeLookFromExp', ''), 7: ('npc', 'RustboroCity_Text_PokemonChangeShape', ''),
        10: ('npc', 'RustboroCity_Text_YoureNewAroundHere', ''), 12: ('npc', 'RustboroCity_Text_TradePokemonGrowFast', ''),
        15: ('npc', 'RustboroCity_Text_YouCanHave2On2Battle', ''), 11: ('itemflag', None, 'X Defend')},
        {0: ('sign', 'RustboroCity_Text_GymSign'), 1: ('sign', 'RustboroCity_Text_TrainersSchoolSign'), 2: ('martsign', None),
         3: ('pcsign', None), 4: ('sign', 'RustboroCity_Text_CitySign'), 5: ('martsign', None), 6: ('pcsign', None),
         7: ('sign', 'RustboroCity_Text_DevonCorpSign'), 8: ('sign', 'RustboroCity_Text_TunnelNearingCompletion'),
         9: ('sign', 'RustboroCity_Text_CuttersHouse')}),
    # Hoenn batch 3. Rusturf Tunnel is open (its two Rock Smash boulders are left out, as after the story). Left out:
    # Mr. Briney, Wanda and her boyfriend, Team Aqua and Peeko, the Devon employee, the glasses man, berry trees,
    # secret bases and hidden items.
    'Route116': (EMERALD, {
        2: ('trainer', ('TRAINER_HOENN_JOEY', 'Route116_Text_JoeyIntro', 'Route116_Text_JoeyDefeat', 'Route116_Text_JoeyPostBattle'), ''),
        5: ('trainer', ('TRAINER_HOENN_JOSE', 'Route116_Text_JoseIntro', 'Route116_Text_JoseDefeat', 'Route116_Text_JosePostBattle'), ''),
        12: ('trainer', ('TRAINER_HOENN_CLARK', 'Route116_Text_ClarkIntro', 'Route116_Text_ClarkDefeat', 'Route116_Text_ClarkPostBattle'), ''),
        15: ('trainer', ('TRAINER_HOENN_JANICE', 'Route116_Text_JaniceIntro', 'Route116_Text_JaniceDefeat', 'Route116_Text_JanicePostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_KAREN', 'Route116_Text_KarenIntro', 'Route116_Text_KarenDefeat', 'Route116_Text_KarenPostBattle'), ''),
        17: ('trainer', ('TRAINER_HOENN_JERRY', 'Route116_Text_JerryIntro', 'Route116_Text_JerryDefeat', 'Route116_Text_JerryPostBattle'), ''),
        22: ('trainer', ('TRAINER_HOENN_SARAH', 'Route116_Text_SarahIntro', 'Route116_Text_SarahDefeat', 'Route116_Text_SarahPostBattle'), ''),
        23: ('trainer', ('TRAINER_HOENN_DAWSON', 'Route116_Text_DawsonIntro', 'Route116_Text_DawsonDefeat', 'Route116_Text_DawsonPostBattle'), ''),
        26: ('trainer', ('TRAINER_HOENN_JOHNSON', 'Route116_Text_JohnsonIntro', 'Route116_Text_JohnsonDefeat', 'Route116_Text_JohnsonPostBattle'), ''),
        27: ('trainer', ('TRAINER_HOENN_DEVAN', 'Route116_Text_DevanIntro', 'Route116_Text_DevanDefeat', 'Route116_Text_DevanPostBattle'), ''),
        6: ('itemflag', None, 'Ether'), 7: ('itemflag', None, 'Repel'), 14: ('itemflag', None, 'HP Up'),
        19: ('itemflag', None, 'X Special'), 24: ('itemflag', None, 'Potion'),
        3: ('cuttree', None, ''), 4: ('cuttree', None, ''), 11: ('cuttree', None, ''), 13: ('cuttree', None, ''), 25: ('cuttree', None, '')},
        {0: ('sign', 'Route116_Text_RouteSignRustboro'), 1: ('sign', 'Route116_Text_RusturfTunnelSign'),
         2: ('sign', 'Route116_Text_TunnelersRestHouse'), 5: ('sign', 'Route116_Text_TrainerTipsBToStopEvolution'),
         6: ('sign', 'Route116_Text_TrainerTipsBagHasPockets')}),
    'RusturfTunnel': (EMERALD, {
        8: ('trainer', ('TRAINER_HOENN_MIKE', 'RusturfTunnel_Text_MikeIntro', 'RusturfTunnel_Text_MikeDefeat', 'RusturfTunnel_Text_MikePostBattle'), ''),
        2: ('itemflag', None, 'Poke Ball'), 3: ('itemflag', None, 'Max Ether')}, {}),
    'VerdanturfTown': (EMERALD, {
        0: ('npc', 'VerdanturfTown_Text_AirCleanHere', ''),
        1: ('npc', 'VerdanturfTown_Text_ManDugTunnelForLove', 'branch after the tunnel opened'),
        2: ('npc', 'VerdanturfTown_Text_EasyToGetToRustboroNow', 'branch after the tunnel opened'),
        3: ('npc', 'VerdanturfTown_Text_MakeBattleTentDebut', '')},
        {0: ('martsign', None), 1: ('sign', 'VerdanturfTown_Text_TownSign'), 2: ('pcsign', None),
         3: ('sign', 'VerdanturfTown_Text_WandasHouse'), 4: ('martsign', None), 5: ('pcsign', None),
         6: ('sign', 'VerdanturfTown_Text_BattleTentSign'), 7: ('sign', 'VerdanturfTown_Text_RusturfTunnelSign')}),
    # Hoenn batch 4. Left out: the Day Care couple, the Pokemon in the Day Care yard, berry trees, the hidden item,
    # Wally, his uncle, Wattson, Scott, the Rollout tutor, and the Gym, Bike Shop, Game Corner and houses (closed).
    'Route117': (EMERALD, {
        7: ('trainer', ('TRAINER_HOENN_DYLAN', 'Route117_Text_DylanIntro', 'Route117_Text_DylanDefeat', 'Route117_Text_DylanPostBattle'), ''), 8: ('trainer', ('TRAINER_HOENN_LYDIA', 'Route117_Text_LydiaIntro', 'Route117_Text_LydiaDefeat', 'Route117_Text_LydiaPostBattle'), ''), 9: ('trainer', ('TRAINER_HOENN_ISAAC', 'Route117_Text_IsaacIntro', 'Route117_Text_IsaacDefeat', 'Route117_Text_IsaacPostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_MARIA', 'Route117_Text_MariaIntro', 'Route117_Text_MariaDefeat', 'Route117_Text_MariaPostBattle'), ''), 17: ('trainer', ('TRAINER_HOENN_DEREK', 'Route117_Text_DerekIntro', 'Route117_Text_DerekDefeat', 'Route117_Text_DerekPostBattle'), ''),
        18: ('trainer', ('TRAINER_HOENN_ANNA_AND_MEG', 'Route117_Text_MegIntro', 'Route117_Text_MegDefeat', 'Route117_Text_MegPostBattle', 'Route117_Text_MegNotEnoughMons'), 'double battle'), 19: ('trainer', ('TRAINER_HOENN_ANNA_AND_MEG', 'Route117_Text_AnnaIntro', 'Route117_Text_AnnaDefeat', 'Route117_Text_AnnaPostBattle', 'Route117_Text_AnnaNotEnoughMons'), 'double battle'),
        21: ('trainer', ('TRAINER_HOENN_BRANDI', 'Route117_Text_BrandiIntro', 'Route117_Text_BrandiDefeat', 'Route117_Text_BrandiPostBattle'), ''), 22: ('trainer', ('TRAINER_HOENN_AISHA', 'Route117_Text_AishaIntro', 'Route117_Text_AishaDefeat', 'Route117_Text_AishaPostBattle'), ''), 23: ('trainer', ('TRAINER_HOENN_MELINA', 'Route117_Text_MelinaIntro', 'Route117_Text_MelinaDefeat', 'Route117_Text_MelinaPostBattle'), ''),
        0: ('npc', 'Route117_Text_ArentTheseFlowersPretty', ''), 1: ('npc', 'Route117_Text_AirIsTastyHere', ''),
        20: ('npc', 'Route117_Text_DayCarePokemonHadNewMove', ''),
        13: ('itemflag', None, 'Great Ball'), 15: ('itemflag', None, 'Revive'), 14: ('cuttree', None, '')},
        {0: ('sign', 'Route117_Text_RouteSignVerdanturf'), 1: ('sign', 'Route117_Text_RouteSignMauville'),
         2: ('sign', 'Route117_Text_DayCareSign')}),
    'MauvilleCity': (EMERALD, {
        0: ('npc', 'MauvilleCity_Text_NurseHurtMonBackToHealth', ''), 1: ('npc', 'MauvilleCity_Text_PokemonCanJumpYouOnBike', ''),
        2: ('npc', 'MauvilleCity_Text_AllSortsOfPeopleComeThrough', ''), 3: ('npc', 'MauvilleCity_Text_RydelVeryGenerous', ''),
        4: ('npc', 'MauvilleCity_Text_ExplainTV', 'TV flag branch left out'), 8: ('itemflag', None, 'X Speed')},
        {0: ('pcsign', None), 1: ('sign', 'MauvilleCity_Text_GymSign'), 2: ('martsign', None), 3: ('martsign', None),
         4: ('pcsign', None), 5: ('sign', 'MauvilleCity_Text_CitySign'), 6: ('sign', 'MauvilleCity_Text_BikeShopSign'),
         7: ('sign', 'MauvilleCity_Text_GameCornerSign')}),
    # Hoenn batch 5. Cycling Road (the raised deck with its seven triathletes and riders) stays closed with its gates.
    # Left out: Team Aqua, the rival, Birch, the Cycling Road challenge man and results sign, berry trees, secret
    # bases, hidden items, Captain Stern, Gabby and Ty, Scott, the market clerks, the Energy Guru, the Effort Ribbon
    # woman, the harbor and shipyard signs (story branches) and the Slateport buildings (closed).
    'Route110': (EMERALD, {
        11: ('trainer', ('TRAINER_HOENN_EDWARD', 'Route110_Text_EdwardIntro', 'Route110_Text_EdwardDefeated', 'Route110_Text_EdwardPostBattle'), ''),
        12: ('trainer', ('TRAINER_HOENN_JACLYN', 'Route110_Text_JaclynIntro', 'Route110_Text_JaclynDefeated', 'Route110_Text_JaclynPostBattle'), ''),
        13: ('trainer', ('TRAINER_HOENN_EDWIN', 'Route110_Text_EdwinIntro', 'Route110_Text_EdwinDefeated', 'Route110_Text_EdwinPostBattle'), ''),
        14: ('trainer', ('TRAINER_HOENN_DALE', 'Route110_Text_DaleIntro', 'Route110_Text_DaleDefeated', 'Route110_Text_DalePostBattle'), ''),
        29: ('trainer', ('TRAINER_HOENN_TIMMY', 'Route110_Text_TimmyIntro', 'Route110_Text_TimmyDefeated', 'Route110_Text_TimmyPostBattle'), ''),
        30: ('trainer', ('TRAINER_HOENN_ISABEL', 'Route110_Text_IsabelIntro', 'Route110_Text_IsabelDefeated', 'Route110_Text_IsabelPostBattle'), ''),
        31: ('trainer', ('TRAINER_HOENN_KALEB', 'Route110_Text_KalebIntro', 'Route110_Text_KalebDefeated', 'Route110_Text_KalebPostBattle'), ''),
        32: ('trainer', ('TRAINER_HOENN_ALYSSA', 'Route110_Text_AlyssaIntro', 'Route110_Text_AlyssaDefeated', 'Route110_Text_AlyssaPostBattle'), ''),
        33: ('trainer', ('TRAINER_HOENN_JOSEPH', 'Route110_Text_JosephIntro', 'Route110_Text_JosephDefeated', 'Route110_Text_JosephPostBattle'), ''),
        0: ('npc', 'Route110_Text_WhichShouldIChoose', ''), 2: ('npc', 'Route110_Text_WalkOnTheLowRoad', ''),
        4: ('npc', 'Route110_Text_TwoRoads', ''), 6: ('npc', 'Route110_Text_RideBikeAtFullSpeed', ''),
        18: ('itemflag', None, 'Dire Hit'), 19: ('itemflag', None, 'Rare Candy'), 34: ('itemflag', None, 'Elixir')},
        {0: ('sign', 'Route110_Text_AquaWasHere'), 1: ('sign', 'Route110_Text_SeasideParkingSign'),
         2: ('sign', 'Route110_Text_CyclingRoadSign'), 3: ('sign', 'Route110_Text_SlateportCitySign'),
         4: ('sign', 'Route110_Text_Route103Sign'), 5: ('sign', 'Route110_Text_MauvilleCitySign'),
         8: ('sign', 'Route110_Text_TrainerTipsPrlzSleep'), 9: ('sign', 'Route110_Text_TrainerTipsRegisterItems'),
         10: ('sign', 'Route110_Text_TrickHouseSign'), 16: ('sign', 'Route110_Text_CyclingRoadSign')}),
    'SlateportCity': (EMERALD, {
        0: ('npc', 'SlateportCity_Text_BushedHikingFromMauville', 'Stern interview branch left out'),
        1: ('npc', 'SlateportCity_Text_EveryoneCallsHimCaptStern', 'Stern interview branch left out'),
        2: ('npc', 'SlateportCity_Text_GoingToCompeteInBattleTent', 'Stern interview branch left out'),
        3: ('npc', 'SlateportCity_Text_VisitedMuseumOften', 'text after Team Aqua left'),
        5: ('npc', 'SlateportCity_Text_SeaweedFullOfLife', 'Stern interview branch left out'),
        6: ('npc', 'SlateportCity_Text_HowTownIsBornAndGrows', 'Stern interview branch left out'),
        7: ('npc', 'SlateportCity_Text_SlateportWonderfulPlace', 'Stern interview and Secret Power branches left out'),
        11: ('npc', 'SlateportCity_Text_SeaIsSoWet', ''), 12: ('npc', 'SlateportCity_Text_SinkOldBoats', ''),
        13: ('npc', 'SlateportCity_Text_BuyTooMuch', ''), 14: ('npc', 'SlateportCity_Text_BattleTentBuiltRecently', ''),
        15: ('npc', 'SlateportCity_Text_GetNameRaterToHelpYou', 'Stern interview branch left out'),
        16: ('npc', 'SlateportCity_Text_CantChangeTradeMonName', ''),
        21: ('npc', 'SlateportCity_Text_WonderIfLighthouseStartlesPokemon', '')},
        {0: ('sign', 'SlateportCity_Text_NameRatersHouseSign'), 1: ('pcsign', None), 2: ('pcsign', None),
         3: ('martsign', None), 5: ('martsign', None), 6: ('sign', 'SlateportCity_Text_MarketSign'),
         7: ('sign', 'SlateportCity_Text_OceanicMuseumSign'), 8: ('sign', 'SlateportCity_Text_CitySign'),
         9: ('sign', 'SlateportCity_Text_PokemonFanClubSign'), 10: ('sign', 'SlateportCity_Text_BattleTentSign')}),
    # Hoenn batch 6. Route 111's two Rock Smash boulders are left out (the way north stays open, as in Rusturf).
    # Left out: Gabby and Ty, Steven, the Good Rod fisherman, the berry girl, the Winstrates, the Secret Power man,
    # the Rock Smash tip man, the desert fossil, berry trees, secret bases, hidden items, Terra Cave, Mirage Tower,
    # the Desert Ruins, the rest stop, Trainer Hill and the Winstrate house (closed).
    'Route118': (EMERALD, {
        3: ('trainer', ('TRAINER_HOENN_PERRY', 'Route118_Text_PerryIntro', 'Route118_Text_PerryDefeat', 'Route118_Text_PerryPostBattle'), ''),
        13: ('trainer', ('TRAINER_HOENN_ROSE', 'Route118_Text_RoseIntro', 'Route118_Text_RoseDefeat', 'Route118_Text_RosePostBattle'), ''),
        14: ('trainer', ('TRAINER_HOENN_WADE', 'Route118_Text_WadeIntro', 'Route118_Text_WadeDefeat', 'Route118_Text_WadePostBattle'), ''),
        15: ('trainer', ('TRAINER_HOENN_CHESTER', 'Route118_Text_ChesterIntro', 'Route118_Text_ChesterDefeat', 'Route118_Text_ChesterPostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_BARNY', 'Route118_Text_BarnyIntro', 'Route118_Text_BarnyDefeat', 'Route118_Text_BarnyPostBattle'), ''),
        17: ('trainer', ('TRAINER_HOENN_DALTON', 'Route118_Text_DaltonIntro', 'Route118_Text_DaltonDefeat', 'Route118_Text_DaltonPostBattle'), ''),
        20: ('trainer', ('TRAINER_HOENN_DEANDRE', 'Route118_Text_DeandreIntro', 'Route118_Text_DeandreDefeat', 'Route118_Text_DeandrePostBattle'), ''),
        6: ('npc', 'Route118_Text_CanCrossRiversWithSurf', ''), 19: ('itemflag', None, 'Hyper Potion'),
        11: ('cuttree', None, '')},
        {1: ('sign', 'Route118_Text_RouteSignMauville'), 2: ('sign', 'Route118_Text_RouteSign119')}),
    'Route111': (EMERALD, {
        4: ('trainer', ('TRAINER_HOENN_HEIDI', 'Route111_Text_HeidiIntro', 'Route111_Text_HeidiDefeat', 'Route111_Text_HeidiPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_DREW', 'Route111_Text_DrewIntro', 'Route111_Text_DrewDefeat', 'Route111_Text_DrewPostBattle'), ''),
        9: ('trainer', ('TRAINER_HOENN_DUSTY', 'Route111_Text_DustyIntro', 'Route111_Text_DustyDefeat', 'Route111_Text_DustyPostBattle'), ''),
        10: ('trainer', ('TRAINER_HOENN_BEAU', 'Route111_Text_BeauIntro', 'Route111_Text_BeauDefeat', 'Route111_Text_BeauPostBattle'), ''),
        11: ('trainer', ('TRAINER_HOENN_BECKY', 'Route111_Text_BeckyIntro', 'Route111_Text_BeckyDefeat', 'Route111_Text_BeckyPostBattle'), ''),
        27: ('trainer', ('TRAINER_HOENN_IRENE', 'Route111_Text_IreneIntro', 'Route111_Text_IreneDefeat', 'Route111_Text_IrenePostBattle'), ''),
        28: ('trainer', ('TRAINER_HOENN_TRAVIS', 'Route111_Text_TravisIntro', 'Route111_Text_TravisDefeat', 'Route111_Text_TravisPostBattle'), ''),
        29: ('trainer', ('TRAINER_HOENN_DAISUKE', 'Route111_Text_DaisukeIntro', 'Route111_Text_DaisukeDefeat', 'Route111_Text_DaisukePostBattle'), ''),
        30: ('trainer', ('TRAINER_HOENN_BROOKE', 'Route111_Text_BrookeIntro', 'Route111_Text_BrookeDefeat', 'Route111_Text_BrookePostBattle'), ''),
        31: ('trainer', ('TRAINER_HOENN_WILTON', 'Route111_Text_WiltonIntro', 'Route111_Text_WiltonDefeat', 'Route111_Text_WiltonPostBattle'), ''),
        34: ('trainer', ('TRAINER_HOENN_TYRON', 'Route111_Text_TyronIntro', 'Route111_Text_TyronDefeat', 'Route111_Text_TyronPostBattle'), ''),
        35: ('trainer', ('TRAINER_HOENN_CELINA', 'Route111_Text_CelinaIntro', 'Route111_Text_CelinaDefeat', 'Route111_Text_CelinaPostBattle'), ''),
        36: ('trainer', ('TRAINER_HOENN_BIANCA', 'Route111_Text_BiancaIntro', 'Route111_Text_BiancaDefeat', 'Route111_Text_BiancaPostBattle'), ''),
        37: ('trainer', ('TRAINER_HOENN_HAYDEN', 'Route111_Text_HaydenIntro', 'Route111_Text_HaydenDefeat', 'Route111_Text_HaydenPostBattle'), ''),
        38: ('trainer', ('TRAINER_HOENN_BRYAN', 'Route111_Text_BryanIntro', 'Route111_Text_BryanDefeat', 'Route111_Text_BryanPostBattle'), ''),
        39: ('trainer', ('TRAINER_HOENN_CELIA', 'Route111_Text_CeliaIntro', 'Route111_Text_CeliaDefeat', 'Route111_Text_CeliaPostBattle'), ''),
        40: ('trainer', ('TRAINER_HOENN_BRANDEN', 'Route111_Text_BrandenIntro', 'Route111_Text_BrandenDefeat', 'Route111_Text_BrandenPostBattle'), ''),
        7: ('npc', 'Route111_Text_ToughToKeepWinningUpTheRanks', ''), 33: ('npc', 'Route111_Text_WinstrateFamilyDestroyedMe', ''),
        42: ('npc', 'Route111_Text_ShouldBeMirageTowerAroundHere', 'Mirage Tower branches left out'),
        17: ('itemflag', None, 'TM37 Sandstorm'), 24: ('itemflag', None, 'Stardust'), 25: ('itemflag', None, 'HP Up'),
        41: ('itemflag', None, 'Elixir'), 26: ('cuttree', None, '')},
        {0: ('sign', 'Route111_Text_WinstrateHouseSign'), 1: ('sign', 'Route111_Text_RouteSignMauville'),
         2: ('sign', 'Route111_Text_RouteSign112'), 3: ('sign', 'Route111_Text_RouteSign113'),
         7: ('sign', 'Route111_Text_OldLadysRestStopSign'), 12: ('sign', 'Route111_Text_TrainerTipsSpAtkSpDef'),
         17: ('sign', 'Route111_Text_TrainerHillSign')}),
    # Hoenn batch 7. Left out: Mr. Briney and his boat, the Zigzagoon, hidden items and the Seashore House (closed).
    'Route109': (EMERALD, {
        2: ('trainer', ('TRAINER_HOENN_DAVID', 'Route109_Text_DavidIntro', 'Route109_Text_DavidDefeated', 'Route109_Text_DavidPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_ALICE', 'Route109_Text_AliceIntro', 'Route109_Text_AliceDefeated', 'Route109_Text_AlicePostBattle'), ''),
        4: ('trainer', ('TRAINER_HOENN_HUEY', 'Route109_Text_HueyIntro', 'Route109_Text_HueyDefeated', 'Route109_Text_HueyPostBattle'), ''),
        5: ('trainer', ('TRAINER_HOENN_EDMOND', 'Route109_Text_EdmondIntro', 'Route109_Text_EdmondDefeated', 'Route109_Text_EdmondPostBattle'), ''),
        6: ('trainer', ('TRAINER_HOENN_RICKY', 'Route109_Text_RickyIntro', 'Route109_Text_RickyDefeated', 'Route109_Text_RickyPostBattle'), ''),
        7: ('trainer', ('TRAINER_HOENN_LOLA', 'Route109_Text_LolaIntro', 'Route109_Text_LolaDefeated', 'Route109_Text_LolaPostBattle'), ''),
        10: ('trainer', ('TRAINER_HOENN_GWEN', 'Route109_Text_GwenIntro', 'Route109_Text_GwenDefeated', 'Route109_Text_GwenPostBattle'), ''),
        11: ('trainer', ('TRAINER_HOENN_AUSTINA', 'Route109_Text_AustinaIntro', 'Route109_Text_AustinaDefeated', 'Route109_Text_AustinaPostBattle'), ''),
        12: ('trainer', ('TRAINER_HOENN_CARTER', 'Route109_Text_CarterIntro', 'Route109_Text_CarterDefeated', 'Route109_Text_CarterPostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_MEL_AND_PAUL', 'Route109_Text_MelIntro', 'Route109_Text_MelDefeated', 'Route109_Text_MelPostBattle', 'Route109_Text_MelNotEnoughPokemon'), 'double battle'),
        17: ('trainer', ('TRAINER_HOENN_MEL_AND_PAUL', 'Route109_Text_PaulIntro', 'Route109_Text_PaulDefeated', 'Route109_Text_PaulPostBattle', 'Route109_Text_PaulNotEnoughPokemon'), 'double battle'),
        20: ('trainer', ('TRAINER_HOENN_HAILEY', 'Route109_Text_HaileyIntro', 'Route109_Text_HaileyDefeated', 'Route109_Text_HaileyPostBattle'), ''),
        21: ('trainer', ('TRAINER_HOENN_CHANDLER', 'Route109_Text_ChandlerIntro', 'Route109_Text_ChandlerDefeated', 'Route109_Text_ChandlerPostBattle'), ''),
        23: ('trainer', ('TRAINER_HOENN_ELIJAH', 'Route109_Text_ElijahIntro', 'Route109_Text_ElijahDefeated', 'Route109_Text_ElijahPostBattle'), ''),
        8: ('npc', 'Route109_Text_ChillAtMyPapasSpot', ''), 13: ('npc', 'Route109_Text_SandCastleTakingLongTime', ''),
        14: ('gift', ('ITEM_SOFT_SAND', 'Route109_Text_YouCanHaveThis', 'Route109_Text_WereGoingToMakeBigCastle', False), 'Soft Sand'),
        15: ('npc', 'Route109_Text_LittleKidsDartAround', ''), 18: ('npc', 'Route109_Text_ZigzagoonPicksUpLitter', ''),
        9: ('itemflag', None, 'PP Up'), 22: ('itemflag', None, 'Potion')},
        {0: ('sign', 'Route109_Text_SeashoreHouseSign'), 1: ('sign', 'Route109_Text_TrainerTipsSign')}),
    # Hoenn batch 8. Left out: the Magma grunts, berry trees, Lavaridge's rival, the Egg woman (gift with a story
    # flag), the hidden item and the closed buildings (cable car, Fiery Path, Jagged Pass, Gym, herb shop, house).
    'Route112': (EMERALD, {
        1: ('trainer', ('TRAINER_HOENN_BRICE', 'Route112_Text_BriceIntro', 'Route112_Text_BriceDefeat', 'Route112_Text_BricePostBattle'), ''),
        2: ('trainer', ('TRAINER_HOENN_LARRY', 'Route112_Text_LarryIntro', 'Route112_Text_LarryDefeat', 'Route112_Text_LarryPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_CAROL', 'Route112_Text_CarolIntro', 'Route112_Text_CarolDefeat', 'Route112_Text_CarolPostBattle'), ''),
        4: ('trainer', ('TRAINER_HOENN_TRENT', 'Route112_Text_TrentIntro', 'Route112_Text_TrentDefeat', 'Route112_Text_TrentPostBattle'), ''),
        12: ('trainer', ('TRAINER_HOENN_BRYANT', 'Route112_Text_BryantIntro', 'Route112_Text_BryantDefeat', 'Route112_Text_BryantPostBattle'), ''),
        13: ('trainer', ('TRAINER_HOENN_SHAYLA', 'Route112_Text_ShaylaIntro', 'Route112_Text_ShaylaDefeat', 'Route112_Text_ShaylaPostBattle'), ''),
        10: ('npc', 'Route112_Text_NotEasyToGetBackToLavaridge', ''), 11: ('itemflag', None, 'Nugget')},
        {0: ('sign', 'Route112_Text_MtChimneySign'), 1: ('sign', 'Route112_Text_MtChimneyCableCarSign'),
         2: ('sign', 'Route112_Text_RouteSignLavaridge')}),
    'LavaridgeTown': (EMERALD, {
        0: ('npc', 'LavaridgeTown_Text_OhYouLikeHotSprings', ''), 1: ('npc', 'LavaridgeTown_Text_HotSpringsNeverRunDry', ''),
        2: ('npc', 'LavaridgeTown_Text_PokemonNippedBackside', ''), 3: ('npc', 'LavaridgeTown_Text_BatheInHotSpringsEveryDay', ''),
        4: ('npc', 'LavaridgeTown_Text_IfPokemonInHotSprings', ''), 5: ('npc', 'LavaridgeTown_Text_HotSpringsClaims', '')},
        {0: ('sign', 'LavaridgeTown_Text_HerbShopSign'), 1: ('sign', 'LavaridgeTown_Text_GymSign'), 2: ('martsign', None),
         3: ('sign', 'LavaridgeTown_Text_TownSign'), 4: ('pcsign', None), 5: ('martsign', None), 6: ('pcsign', None)}),
    # Hoenn batch 9. Left out: the secret base, hidden items, the Azurill, and the closed buildings (glass workshop,
    # Terra Cave, Battle Tent, Cozmo's house, the Move Relearner).
    'Route113': (EMERALD, {
        2: ('trainer', ('TRAINER_HOENN_JAYLEN', 'Route113_Text_JaylenIntro', 'Route113_Text_JaylenDefeat', 'Route113_Text_JaylenPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_DILLON', 'Route113_Text_DillonIntro', 'Route113_Text_DillonDefeat', 'Route113_Text_DillonPostBattle'), ''),
        4: ('trainer', ('TRAINER_HOENN_MADELINE', 'Route113_Text_MadelineIntro', 'Route113_Text_MadelineDefeat', 'Route113_Text_MadelinePostBattle'), ''),
        7: ('trainer', ('TRAINER_HOENN_LAO', 'Route113_Text_LaoIntro', 'Route113_Text_LaoDefeat', 'Route113_Text_LaoPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_LUNG', 'Route113_Text_LungIntro', 'Route113_Text_LungDefeat', 'Route113_Text_LungPostBattle'), ''),
        9: ('trainer', ('TRAINER_HOENN_TORI_AND_TIA', 'Route113_Text_ToriIntro', 'Route113_Text_ToriDefeat', 'Route113_Text_ToriPostBattle', 'Route113_Text_ToriNotEnoughMons'), 'double battle'),
        10: ('trainer', ('TRAINER_HOENN_TORI_AND_TIA', 'Route113_Text_TiaIntro', 'Route113_Text_TiaDefeat', 'Route113_Text_TiaPostBattle', 'Route113_Text_TiaNotEnoughMons'), 'double battle'),
        12: ('trainer', ('TRAINER_HOENN_WYATT', 'Route113_Text_WyattIntro', 'Route113_Text_WyattDefeat', 'Route113_Text_WyattPostBattle'), ''),
        13: ('trainer', ('TRAINER_HOENN_LAWRENCE', 'Route113_Text_LawrenceIntro', 'Route113_Text_LawrenceDefeat', 'Route113_Text_LawrencePostBattle'), ''),
        14: ('trainer', ('TRAINER_HOENN_SOPHIE', 'Route113_Text_SophieIntro', 'Route113_Text_SophieDefeat', 'Route113_Text_SophiePostBattle'), ''),
        15: ('trainer', ('TRAINER_HOENN_COBY', 'Route113_Text_CobyIntro', 'Route113_Text_CobyDefeat', 'Route113_Text_CobyPostBattle'), ''),
        0: ('npc', 'Route113_Text_FunWalkingThroughAsh', ''), 1: ('npc', 'Route113_Text_AshCanBeFashionedIntoGlass', ''),
        5: ('itemflag', None, 'Max Ether'), 6: ('itemflag', None, 'Super Repel'), 11: ('itemflag', None, 'Hyper Potion')},
        {0: ('sign', 'Route113_Text_RouteSign111'), 1: ('sign', 'Route113_Text_RouteSignFallarbor'),
         2: ('sign', 'Route113_Text_TrainerTipsRegisterKeyItems'), 3: ('sign', 'Route113_Text_GlassWorkshopSign')}),
    'FallarborTown': (EMERALD, {
        0: ('npc', 'FallarborTown_Text_MyPreciousAzurill', ''),
        1: ('npc', 'FallarborTown_Text_RegionKnownForMeteors', 'text after Mt. Chimney'),
        2: ('npc', 'FallarborTown_Text_HaveYouChallengedFlannery', '')},
        {0: ('martsign', None), 1: ('pcsign', None), 2: ('sign', 'FallarborTown_Text_BattleTentSign'), 3: ('pcsign', None),
         4: ('sign', 'FallarborTown_Text_TownSign'), 5: ('martsign', None), 6: ('sign', 'FallarborTown_Text_MoveTutorSign')}),
    # Hoenn batch 10. The Rock Smash boulders are left out (the paths stay open). Left out: the Poochyena, the
    # Berry Crush man, berry trees, secret bases, hidden items, Meteor Falls, Terra Cave and the two houses.
    'Route114': (EMERALD, {
        2: ('trainer', ('TRAINER_HOENN_LENNY', 'Route114_Text_LennyIntro', 'Route114_Text_LennyDefeat', 'Route114_Text_LennyPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_LUCAS', 'Route114_Text_LucasIntro', 'Route114_Text_LucasDefeat', 'Route114_Text_LucasPostBattle'), ''),
        4: ('trainer', ('TRAINER_HOENN_SHANE', 'Route114_Text_ShaneIntro', 'Route114_Text_ShaneDefeat', 'Route114_Text_ShanePostBattle'), ''),
        5: ('trainer', ('TRAINER_HOENN_NANCY', 'Route114_Text_NancyIntro', 'Route114_Text_NancyDefeat', 'Route114_Text_NancyPostBattle'), ''),
        6: ('trainer', ('TRAINER_HOENN_STEVE', 'Route114_Text_SteveIntro', 'Route114_Text_SteveDefeat', 'Route114_Text_StevePostBattle'), ''),
        14: ('trainer', ('TRAINER_HOENN_NOLAN', 'Route114_Text_NolanIntro', 'Route114_Text_NolanDefeat', 'Route114_Text_NolanPostBattle'), ''),
        15: ('trainer', ('TRAINER_HOENN_CLAUDE', 'Route114_Text_ClaudeIntro', 'Route114_Text_ClaudeDefeat', 'Route114_Text_ClaudePostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_BERNIE', 'Route114_Text_BernieIntro', 'Route114_Text_BernieDefeat', 'Route114_Text_BerniePostBattle'), ''),
        21: ('trainer', ('TRAINER_HOENN_TYRA_AND_IVY', 'Route114_Text_IvyIntro', 'Route114_Text_IvyDefeat', 'Route114_Text_IvyPostBattle', 'Route114_Text_IvyNotEnoughMons'), 'double battle'),
        22: ('trainer', ('TRAINER_HOENN_TYRA_AND_IVY', 'Route114_Text_TyraIntro', 'Route114_Text_TyraDefeat', 'Route114_Text_TyraPostBattle', 'Route114_Text_TyraNotEnoughMons'), 'double battle'),
        23: ('trainer', ('TRAINER_HOENN_CHARLOTTE', 'Route114_Text_CharlotteIntro', 'Route114_Text_CharlotteDefeat', 'Route114_Text_CharlottePostBattle'), ''),
        24: ('trainer', ('TRAINER_HOENN_ANGELINA', 'Route114_Text_AngelinaIntro', 'Route114_Text_AngelinaDefeat', 'Route114_Text_AngelinaPostBattle'), ''),
        26: ('trainer', ('TRAINER_HOENN_KAI', 'Route114_Text_KaiIntro', 'Route114_Text_KaiDefeat', 'Route114_Text_KaiPostBattle'), ''),
        11: ('gift', ('ITEM_TM05', 'Route114_Text_AllMyMonDoesIsRoarTakeThis', 'Route114_Text_ExplainRoar', False), 'TM05 Roar'),
        8: ('itemflag', None, 'Rare Candy'), 9: ('itemflag', None, 'Protein'), 25: ('itemflag', None, 'Energy Powder')},
        {0: ('sign', 'Route114_Text_MeteorFallsSign'), 1: ('sign', 'Route114_Text_FossilManiacsHouseSign'),
         8: ('sign', 'Route114_Text_LanettesHouse')}),
    'Route115': (EMERALD, {
        1: ('trainer', ('TRAINER_HOENN_TIMOTHY', 'Route115_Text_TimothyIntro', 'Route115_Text_TimothyDefeat', 'Route115_Text_TimothyPostBattle'), ''),
        2: ('trainer', ('TRAINER_HOENN_NOB', 'Route115_Text_NobIntro', 'Route115_Text_NobDefeat', 'Route115_Text_NobPostBattle'), ''),
        6: ('trainer', ('TRAINER_HOENN_CYNDY', 'Route115_Text_CyndyIntro', 'Route115_Text_CyndyDefeat', 'Route115_Text_CyndyPostBattle'), ''),
        7: ('trainer', ('TRAINER_HOENN_KOICHI', 'Route115_Text_KoichiIntro', 'Route115_Text_KoichiDefeat', 'Route115_Text_KoichiPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_HECTOR', 'Route115_Text_HectorIntro', 'Route115_Text_HectorDefeat', 'Route115_Text_HectorPostBattle'), ''),
        16: ('trainer', ('TRAINER_HOENN_KYRA', 'Route115_Text_KyraIntro', 'Route115_Text_KyraDefeat', 'Route115_Text_KyraPostBattle'), ''),
        17: ('trainer', ('TRAINER_HOENN_JAIDEN', 'Route115_Text_JaidenIntro', 'Route115_Text_JaidenDefeat', 'Route115_Text_JaidenPostBattle'), ''),
        18: ('trainer', ('TRAINER_HOENN_HELENE', 'Route115_Text_HeleneIntro', 'Route115_Text_HeleneDefeat', 'Route115_Text_HelenePostBattle'), ''),
        19: ('trainer', ('TRAINER_HOENN_ALIX', 'Route115_Text_AlixIntro', 'Route115_Text_AlixDefeat', 'Route115_Text_AlixPostBattle'), ''),
        20: ('trainer', ('TRAINER_HOENN_MARLENE', 'Route115_Text_MarleneIntro', 'Route115_Text_MarleneDefeat', 'Route115_Text_MarlenePostBattle'), ''),
        0: ('npc', 'Route115_Text_NeverKnowWhenCavePokemonWillAppear', ''),
        9: ('itemflag', None, 'Super Potion'), 10: ('itemflag', None, 'TM01 Focus Punch'), 11: ('itemflag', None, 'Iron'),
        15: ('itemflag', None, 'Great Ball'), 21: ('itemflag', None, 'PP Up'), 22: ('itemflag', None, 'Heal Powder')},
        {2: ('sign', 'Route115_Text_RouteSignRustboro'), 3: ('sign', 'Route115_Text_MeteorFallsSign')}),
    # Hoenn batch 11. Left out: Team Aqua on the bridge, the rival, Scott, the Kecleon blocking Fortree's Gym
    # (the Gym itself stays closed), berry trees, secret bases, hidden items, the Weather Institute and the houses.
    'Route119': (EMERALD, {
        3: ('trainer', ('TRAINER_HOENN_GREG', 'Route119_Text_GregIntro', 'Route119_Text_GregDefeat', 'Route119_Text_GregPostBattle'), ''),
        4: ('trainer', ('TRAINER_HOENN_TAYLOR', 'Route119_Text_TaylorIntro', 'Route119_Text_TaylorDefeat', 'Route119_Text_TaylorPostBattle'), ''),
        5: ('trainer', ('TRAINER_HOENN_DONALD', 'Route119_Text_DonaldIntro', 'Route119_Text_DonaldDefeat', 'Route119_Text_DonaldPostBattle'), ''),
        6: ('trainer', ('TRAINER_HOENN_JACKSON', 'Route119_Text_JacksonIntro', 'Route119_Text_JacksonDefeat', 'Route119_Text_JacksonPostBattle'), ''),
        7: ('trainer', ('TRAINER_HOENN_BRENT', 'Route119_Text_BrentIntro', 'Route119_Text_BrentDefeat', 'Route119_Text_BrentPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_CATHERINE', 'Route119_Text_CatherineIntro', 'Route119_Text_CatherineDefeat', 'Route119_Text_CatherinePostBattle'), ''),
        9: ('trainer', ('TRAINER_HOENN_DOUG', 'Route119_Text_DougIntro', 'Route119_Text_DougDefeat', 'Route119_Text_DougPostBattle'), ''),
        10: ('trainer', ('TRAINER_HOENN_KENT', 'Route119_Text_KentIntro', 'Route119_Text_KentDefeat', 'Route119_Text_KentPostBattle'), ''),
        11: ('trainer', ('TRAINER_HOENN_YASU', 'Route119_Text_YasuIntro', 'Route119_Text_YasuDefeat', 'Route119_Text_YasuPostBattle'), ''),
        12: ('trainer', ('TRAINER_HOENN_TAKASHI', 'Route119_Text_TakashiIntro', 'Route119_Text_TakashiDefeat', 'Route119_Text_TakashiPostBattle'), ''),
        13: ('trainer', ('TRAINER_HOENN_HUGH', 'Route119_Text_HughIntro', 'Route119_Text_HughDefeat', 'Route119_Text_HughPostBattle'), ''),
        14: ('trainer', ('TRAINER_HOENN_PHIL', 'Route119_Text_PhilIntro', 'Route119_Text_PhilDefeat', 'Route119_Text_PhilPostBattle'), ''),
        33: ('trainer', ('TRAINER_HOENN_HIDEO', 'Route119_Text_HideoIntro', 'Route119_Text_HideoDefeat', 'Route119_Text_HideoPostBattle'), ''),
        36: ('trainer', ('TRAINER_HOENN_CHRIS', 'Route119_Text_ChrisIntro', 'Route119_Text_ChrisDefeat', 'Route119_Text_ChrisPostBattle'), ''),
        37: ('trainer', ('TRAINER_HOENN_RACHEL', 'Route119_Text_RachelIntro', 'Route119_Text_RachelDefeat', 'Route119_Text_RachelPostBattle'), ''),
        38: ('trainer', ('TRAINER_HOENN_DAYTON', 'Route119_Text_DaytonIntro', 'Route119_Text_DaytonDefeat', 'Route119_Text_DaytonPostBattle'), ''),
        39: ('trainer', ('TRAINER_HOENN_FABIAN', 'Route119_Text_FabianIntro', 'Route119_Text_FabianDefeat', 'Route119_Text_FabianPostBattle'), ''),
        25: ('npc', 'Route119_Text_ThoughtFlyByCatchingBirdMons', ''), 30: ('npc', 'Route119_Text_TallGrassSnaresBikeTires', ''),
        32: ('npc', 'Route119_Text_CanYourMonMakeSecretBase', ''),
        16: ('itemflag', None, 'Super Repel'), 17: ('itemflag', None, 'Zinc'), 18: ('itemflag', None, 'Elixir'),
        19: ('itemflag', None, 'Leaf Stone'), 20: ('itemflag', None, 'Rare Candy'), 21: ('itemflag', None, 'Hyper Potion'),
        31: ('itemflag', None, 'Hyper Potion'), 40: ('itemflag', None, 'Nugget'), 41: ('itemflag', None, 'Elixir')},
        {0: ('sign', 'Route119_Text_WeatherInstitute'), 1: ('sign', 'Route119_Text_RouteSignFortree'),
         19: ('sign', 'Route119_Text_TrainerTipsDecoration')}),
    'FortreeCity': (EMERALD, {
        0: ('npc', 'FortreeCity_Text_SawGiganticPokemonInSky', ''), 1: ('npc', 'FortreeCity_Text_TreesGrowByDrinkingRainwater', ''),
        2: ('npc', 'FortreeCity_Text_ThisTimeIllBeatWinona', 'text after the Kecleon left'),
        3: ('npc', 'FortreeCity_Text_BugPokemonComeThroughWindow', ''), 4: ('npc', 'FortreeCity_Text_EveryoneHealthyAndLively', ''),
        5: ('npc', 'FortreeCity_Text_PokemonThatEvolveWhenTraded', '')},
        {0: ('sign', 'FortreeCity_Text_CitySign'), 1: ('pcsign', None), 2: ('martsign', None),
         3: ('sign', 'FortreeCity_Text_GymSign'), 4: ('pcsign', None), 5: ('martsign', None)}),
    # Hoenn batch 12. Left out: Gabby and Ty, Steven and the Kecleon on Route 120's bridge (the bridge stays open),
    # the other Kecleon, the berry beauty and gentleman, Team Aqua, the rival, berry trees, secret bases, hidden
    # items, Lilycove's story-branch townsfolk and signs (harbor, Wailmer trainer, Aqua cave), and the closed
    # buildings.
    'Route120': (EMERALD, {
        10: ('trainer', ('TRAINER_HOENN_COLIN', 'Route120_Text_ColinIntro', 'Route120_Text_ColinDefeat', 'Route120_Text_ColinPostBattle'), ''),
        11: ('trainer', ('TRAINER_HOENN_ROBERT', 'Route120_Text_RobertIntro', 'Route120_Text_RobertDefeat', 'Route120_Text_RobertPostBattle'), ''),
        12: ('trainer', ('TRAINER_HOENN_LORENZO', 'Route120_Text_LorenzoIntro', 'Route120_Text_LorenzoDefeat', 'Route120_Text_LorenzoPostBattle'), ''),
        13: ('trainer', ('TRAINER_HOENN_JENNA', 'Route120_Text_JennaIntro', 'Route120_Text_JennaDefeat', 'Route120_Text_JennaPostBattle'), ''),
        14: ('trainer', ('TRAINER_HOENN_JEFFREY', 'Route120_Text_JeffreyIntro', 'Route120_Text_JeffreyDefeat', 'Route120_Text_JeffreyPostBattle'), ''),
        23: ('trainer', ('TRAINER_HOENN_JENNIFER', 'Route120_Text_JenniferIntro', 'Route120_Text_JenniferDefeat', 'Route120_Text_JenniferPostBattle'), ''),
        24: ('trainer', ('TRAINER_HOENN_CHIP', 'Route120_Text_ChipIntro', 'Route120_Text_ChipDefeat', 'Route120_Text_ChipPostBattle'), ''),
        25: ('trainer', ('TRAINER_HOENN_CLARISSA', 'Route120_Text_ClarissaIntro', 'Route120_Text_ClarissaDefeat', 'Route120_Text_ClarissaPostBattle'), ''),
        26: ('trainer', ('TRAINER_HOENN_ANGELICA', 'Route120_Text_AngelicaIntro', 'Route120_Text_AngelicaDefeat', 'Route120_Text_AngelicaPostBattle'), ''),
        33: ('trainer', ('TRAINER_HOENN_KEIGO', 'Route120_Text_KeigoIntro', 'Route120_Text_KeigoDefeat', 'Route120_Text_KeigoPostBattle'), ''),
        34: ('trainer', ('TRAINER_HOENN_RILEY', 'Route120_Text_RileyIntro', 'Route120_Text_RileyDefeat', 'Route120_Text_RileyPostBattle'), ''),
        41: ('trainer', ('TRAINER_HOENN_CALLIE', 'Route120_Text_CallieIntro', 'Route120_Text_CallieDefeat', 'Route120_Text_CalliePostBattle'), ''),
        42: ('trainer', ('TRAINER_HOENN_LEONEL', 'Route120_Text_LeonelIntro', 'Route120_Text_LeonelDefeat', 'Route120_Text_LeonelPostBattle'), ''),
        15: ('itemflag', None, 'Nugget'), 21: ('itemflag', None, 'Full Heal'), 27: ('itemflag', None, 'Nest Ball'),
        28: ('itemflag', None, 'Hyper Potion'), 43: ('itemflag', None, 'Revive'),
        20: ('cuttree', None, ''), 31: ('cuttree', None, ''), 32: ('cuttree', None, '')},
        {0: ('sign', 'Route120_Text_RouteSignFortree'), 1: ('sign', 'Route120_Text_RouteSign121')}),
    'Route121': (EMERALD, {
        9: ('trainer', ('TRAINER_HOENN_KATE_AND_JOY', 'Route121_Text_KateIntro', 'Route121_Text_KateDefeat', 'Route121_Text_KatePostBattle', 'Route121_Text_KateNotEnoughMons'), 'double battle'),
        10: ('trainer', ('TRAINER_HOENN_KATE_AND_JOY', 'Route121_Text_JoyIntro', 'Route121_Text_JoyDefeat', 'Route121_Text_JoyPostBattle', 'Route121_Text_JoyNotEnoughMons'), 'double battle'),
        16: ('trainer', ('TRAINER_HOENN_VANESSA', 'Route121_Text_VanessaIntro', 'Route121_Text_VanessaDefeat', 'Route121_Text_VanessaPostBattle'), ''),
        17: ('trainer', ('TRAINER_HOENN_WALTER', 'Route121_Text_WalterIntro', 'Route121_Text_WalterDefeat', 'Route121_Text_WalterPostBattle'), ''),
        18: ('trainer', ('TRAINER_HOENN_TAMMY', 'Route121_Text_TammyIntro', 'Route121_Text_TammyDefeat', 'Route121_Text_TammyPostBattle'), ''),
        19: ('trainer', ('TRAINER_HOENN_JESSICA', 'Route121_Text_JessicaIntro', 'Route121_Text_JessicaDefeat', 'Route121_Text_JessicaPostBattle'), ''),
        22: ('trainer', ('TRAINER_HOENN_CALE', 'Route121_Text_CaleIntro', 'Route121_Text_CaleDefeat', 'Route121_Text_CalePostBattle'), ''),
        23: ('trainer', ('TRAINER_HOENN_MYLES', 'Route121_Text_MylesIntro', 'Route121_Text_MylesDefeat', 'Route121_Text_MylesPostBattle'), ''),
        24: ('trainer', ('TRAINER_HOENN_PAT', 'Route121_Text_PatIntro', 'Route121_Text_PatDefeat', 'Route121_Text_PatPostBattle'), ''),
        25: ('trainer', ('TRAINER_HOENN_MARCEL', 'Route121_Text_MarcelIntro', 'Route121_Text_MarcelDefeat', 'Route121_Text_MarcelPostBattle'), ''),
        26: ('trainer', ('TRAINER_HOENN_CRISTIN', 'Route121_Text_CristinIntro', 'Route121_Text_CristinDefeat', 'Route121_Text_CristinPostBattle'), ''),
        0: ('npc', 'Route121_Text_AheadLoomsMtPyre', ''),
        20: ('itemflag', None, 'Carbos'), 27: ('itemflag', None, 'Revive'), 28: ('itemflag', None, 'Zinc'),
        14: ('cuttree', None, ''), 15: ('cuttree', None, ''), 21: ('cuttree', None, '')},
        {0: ('sign', 'Route121_Text_MtPyrePierSign'), 5: ('sign', 'Route121_Text_SafariZoneSign')}),
    'LilycoveCity': (EMERALD, {
        2: ('npc', 'LilycoveCity_Text_ContestHallInTown', ''), 3: ('npc', 'LilycoveCity_Text_ImFromKanto', ''),
        4: ('npc', 'LilycoveCity_Text_ImArtDealer', ''), 6: ('npc', 'LilycoveCity_Text_SeaRemainsForeverYoung', ''),
        7: ('npc', 'LilycoveCity_Text_SixtyYearsAgoHusbandProposed', ''),
        18: ('npc', 'LilycoveCity_Text_HoneymoonVowToSeeRarePokemon', ''),
        19: ('npc', 'LilycoveCity_Text_JustArrivedAndSawRarePokemon', ''),
        20: ('npc', 'LilycoveCity_Text_HeardTowerCalledSkyPillar', ''), 21: ('npc', 'LilycoveCity_Text_SawTallTowerOnRoute131', ''),
        10: ('itemflag', None, 'Max Repel')},
        {0: ('sign', 'LilycoveCity_Text_MuseumSign'), 1: ('pcsign', None), 2: ('sign', 'LilycoveCity_Text_DepartmentStoreSign'),
         3: ('pcsign', None), 4: ('sign', 'LilycoveCity_Text_CitySign'), 5: ('sign', 'LilycoveCity_Text_ContestHallSign'),
         6: ('sign', 'LilycoveCity_Text_MotelSign'), 8: ('sign', 'LilycoveCity_Text_TrainerFanClubSign'),
         9: ('sign', 'LilycoveCity_Text_MoveDeletersHouseSign')}),
    # Hoenn batch 13. Route 122 has no events. Left out on Route 123: the Giga Drain girl (gift tied to a Grass-type
    # check), berry trees, secret bases, hidden items and the Berry Master's house.
    'Route123': (EMERALD, {
        7: ('trainer', ('TRAINER_HOENN_WENDY', 'Route123_Text_WendyIntro', 'Route123_Text_WendyDefeat', 'Route123_Text_WendyPostBattle'), ''),
        8: ('trainer', ('TRAINER_HOENN_BRAXTON', 'Route123_Text_BraxtonIntro', 'Route123_Text_BraxtonDefeat', 'Route123_Text_BraxtonPostBattle'), ''),
        25: ('trainer', ('TRAINER_HOENN_VIOLET', 'Route123_Text_VioletIntro', 'Route123_Text_VioletDefeat', 'Route123_Text_VioletPostBattle'), ''),
        26: ('trainer', ('TRAINER_HOENN_MIU_AND_YUKI', 'Route123_Text_YukiIntro', 'Route123_Text_YukiDefeat', 'Route123_Text_YukiPostBattle', 'Route123_Text_YukiNotEnoughMons'), 'double battle'),
        27: ('trainer', ('TRAINER_HOENN_MIU_AND_YUKI', 'Route123_Text_MiuIntro', 'Route123_Text_MiuDefeat', 'Route123_Text_MiuPostBattle', 'Route123_Text_MiuNotEnoughMons'), 'double battle'),
        28: ('trainer', ('TRAINER_HOENN_CAMERON', 'Route123_Text_CameronIntro', 'Route123_Text_CameronDefeat', 'Route123_Text_CameronPostBattle'), ''),
        29: ('trainer', ('TRAINER_HOENN_JACKI', 'Route123_Text_JackiIntro', 'Route123_Text_JackiDefeat', 'Route123_Text_JackiPostBattle'), ''),
        30: ('trainer', ('TRAINER_HOENN_KINDRA', 'Route123_Text_KindraIntro', 'Route123_Text_KindraDefeat', 'Route123_Text_KindraPostBattle'), ''),
        33: ('trainer', ('TRAINER_HOENN_JONAS', 'Route123_Text_JonasIntro', 'Route123_Text_JonasDefeat', 'Route123_Text_JonasPostBattle'), ''),
        34: ('trainer', ('TRAINER_HOENN_KAYLEY', 'Route123_Text_KayleyIntro', 'Route123_Text_KayleyDefeat', 'Route123_Text_KayleyPostBattle'), ''),
        35: ('trainer', ('TRAINER_HOENN_ED', 'Route123_Text_EdIntro', 'Route123_Text_EdDefeat', 'Route123_Text_EdPostBattle'), ''),
        36: ('trainer', ('TRAINER_HOENN_FERNANDO', 'Route123_Text_FernandoIntro', 'Route123_Text_FernandoDefeat', 'Route123_Text_FernandoPostBattle'), ''),
        37: ('trainer', ('TRAINER_HOENN_ALBERTO', 'Route123_Text_AlbertoIntro', 'Route123_Text_AlbertoDefeat', 'Route123_Text_AlbertoPostBattle'), ''),
        38: ('trainer', ('TRAINER_HOENN_FREDRICK', 'Route123_Text_FrederickIntro', 'Route123_Text_FrederickDefeat', 'Route123_Text_FrederickPostBattle'), ''),
        40: ('trainer', ('TRAINER_HOENN_JAZMYN', 'Route123_Text_JazmynIntro', 'Route123_Text_JazmynDefeat', 'Route123_Text_JazmynPostBattle'), ''),
        41: ('trainer', ('TRAINER_HOENN_DAVIS', 'Route123_Text_DavisIntro', 'Route123_Text_DavisDefeat', 'Route123_Text_DavisPostBattle'), ''),
        20: ('itemflag', None, 'Calcium'), 31: ('itemflag', None, 'Ultra Ball'), 32: ('itemflag', None, 'Elixir'),
        39: ('itemflag', None, 'PP Up'), 42: ('itemflag', None, 'Revival Herb'),
        17: ('cuttree', None, ''), 18: ('cuttree', None, ''), 19: ('cuttree', None, '')},
        {0: ('sign', 'Route123_Text_RouteSignMtPyre'), 3: ('sign', 'Route123_Text_RouteSign'),
         5: ('sign', 'Route123_Text_BerryMastersHouse')}),
    # Hoenn batch 14. Left out: Island Cave and Granite Cave (closed), Mr. Briney and his boat, the Old Rod
    # fisherman's gift (FireRed already gives the Old Rod) and the trendy-phrase boy (needs Dewford Hall's phrase).
    'Route105': (EMERALD, {
        0: ('trainer', ('TRAINER_HOENN_LUIS', 'Route105_Text_LuisIntro', 'Route105_Text_LuisDefeated', 'Route105_Text_LuisPostBattle'), ''),
        1: ('trainer', ('TRAINER_HOENN_DOMINIK', 'Route105_Text_DominikIntro', 'Route105_Text_DominikDefeated', 'Route105_Text_DominikPostBattle'), ''),
        2: ('trainer', ('TRAINER_HOENN_BEVERLY', 'Route105_Text_BeverlyIntro', 'Route105_Text_BeverlyDefeated', 'Route105_Text_PostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_IMANI', 'Route105_Text_ImaniIntro', 'Route105_Text_ImaniDefeated', 'Route105_Text_ImaniPostBattle'), ''),
        5: ('trainer', ('TRAINER_HOENN_FOSTER', 'Route105_Text_FosterIntro', 'Route105_Text_FosterDefeated', 'Route105_Text_FosterPostBattle'), ''),
        6: ('trainer', ('TRAINER_HOENN_JOSUE', 'Route105_Text_JosueIntro', 'Route105_Text_JosueDefeated', 'Route105_Text_JosuePostBattle'), ''),
        7: ('trainer', ('TRAINER_HOENN_ANDRES', 'Route105_Text_AndresIntro', 'Route105_Text_AndresDefeated', 'Route105_Text_AndresPostBattle'), ''),
        4: ('itemflag', None, 'Iron')}, {}),
    'Route106': (EMERALD, {
        0: ('trainer', ('TRAINER_HOENN_DOUGLAS', 'Route106_Text_DouglasIntro', 'Route106_Text_DouglasDefeated', 'Route106_Text_DouglasPostBattle'), ''),
        1: ('trainer', ('TRAINER_HOENN_KYLA', 'Route106_Text_KylaIntro', 'Route106_Text_KylaDefeated', 'Route106_Text_KylaPostBattle'), ''),
        2: ('trainer', ('TRAINER_HOENN_ELLIOT', 'Route106_Text_ElliotIntro', 'Route106_Text_ElliotDefeated', 'Route106_Text_ElliotPostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_NED', 'Route106_Text_NedIntro', 'Route106_Text_NedDefeated', 'Route106_Text_NedPostBattle'), ''),
        4: ('itemflag', None, 'Protein')},
        {3: ('sign', 'Route106_Text_TrainerTips')}),
    'DewfordTown': (EMERALD, {
        0: ('npc', 'DewfordTown_Text_TinyIslandCommunity', ''),
        2: ('npc', 'DewfordTown_Text_GettingItchToFish', 'Old Rod fisherman: the gift branch is left out')},
        {0: ('sign', 'DewfordTown_Text_TownSign'), 1: ('sign', 'DewfordTown_Text_GymSign'), 2: ('pcsign', None),
         3: ('pcsign', None), 4: ('sign', 'DewfordTown_Text_HallSign')}),
    'Route107': (EMERALD, {
        0: ('trainer', ('TRAINER_HOENN_DARRIN', 'Route107_Text_DarrinIntro', 'Route107_Text_DarrinDefeated', 'Route107_Text_DarrinPostBattle'), ''),
        1: ('trainer', ('TRAINER_HOENN_TONY', 'Route107_Text_TonyIntro', 'Route107_Text_TonyDefeated', 'Route107_Text_TonyPostBattle'), ''),
        2: ('trainer', ('TRAINER_HOENN_DENISE', 'Route107_Text_DeniseIntro', 'Route107_Text_DeniseDefeated', 'Route107_Text_DenisePostBattle'), ''),
        3: ('trainer', ('TRAINER_HOENN_BETH', 'Route107_Text_BethIntro', 'Route107_Text_BethDefeated', 'Route107_Text_BethPostBattle'), ''),
        4: ('trainer', ('TRAINER_HOENN_LISA_AND_RAY', 'Route107_Text_LisaIntro', 'Route107_Text_LisaDefeated', 'Route107_Text_LisaPostBattle', 'Route107_Text_LisaNotEnoughPokemon'), 'double battle'),
        5: ('trainer', ('TRAINER_HOENN_LISA_AND_RAY', 'Route107_Text_RayIntro', 'Route107_Text_RayDefeated', 'Route107_Text_RayPostBattle', 'Route107_Text_RayNotEnoughPokemon'), 'double battle'),
        6: ('trainer', ('TRAINER_HOENN_CAMRON', 'Route107_Text_CamronIntro', 'Route107_Text_CamronDefeated', 'Route107_Text_CamronPostBattle'), ''),
        }, {}),
    'NewBarkTown': (HNS, {
        0: ('npc', 'NewBarkTown_Text_FatManState3', 'default branch'),
        1: ('npc', 'NewBarkTown_Text_LassState3', 'branch for a player who already has Pokemon'),
        2: ('story', None, 'Silver: HNS dialogue before the gift, then a post-League battle; no push movement')},
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
        0: ('story', None, 'aide: Potion after the starter, Poke Balls after Silver, then AideAlwaysBusy (johto_pilot_story.py)'),
        1: ('story', None, 'Elm research request: raise one Pokemon; no egg errand outside the pilot'),
        2: ('npc', 'NewBarkTown_Lab_Text_AideSave', 'default branch'),
        3: ('story', None, 'Chikorita gift, one starter per Johto save'),
        4: ('story', None, 'Cyndaquil gift, one starter per Johto save'),
        5: ('story', None, 'Totodile gift, one starter per Johto save')},
        'lab'),
    'NewBarkTown_PlayersHouse_1F': (HNS, {
        0: ('npc', 'NewBarkTown_PlayersHouse_1F_Text_MomLeaving2', 'mother: the farewell page, which fits any trainer'),
        1: ('npc', 'NewBarkTown_PlayersHouse_1F_Text_MomsFriend', 'source shows her only during the intro; always shown')}, {}),
    'NewBarkTown_PlayersHouse_2F': (HNS, {}, {2: ('sign', 'NewBarkTown_PlayersHouse_2F_Text_NES'),
                                             3: ('sign', 'Text_BookShelf'), 4: ('sign', 'Text_BookShelf')}),
    # Johto batch 1. Left out: the Guide Gent tour and Silver's battle (story scenes), the overworld
    # Pokemon (HNS scenery sprites), berry trees, the girl with HNS's leftover OldaleTown script, and
    # Joey's scripted fight next to his Pidgey/Rattata (FLAG_MOM_VISITED scene).
    'CherrygroveCity': (HNS, {
        1: ('gift', ('ITEM_MYSTIC_WATER', 'CherrygroveCity_Text_MysticWaterGuyBefore', 'CherrygroveCity_Text_MysticWaterGuyAfter'),
            'the question is kept; "no" goes straight to his last line like the source'),
        2: ('npc', 'CherrygroveCity_Text_YoungsterHavePokedex', 'branch after the Pokedex; a League champion has one'),
        3: ('npc', 'CherrygroveCity_Text_TeacherHaveMapCard', '')},
        {0: ('sign', 'CherrygroveCity_Text_CitySign')}),
    'Route30': (HNS, {
        0: ('npc', 'Route30_Text_YoungsterEveryoneIsBattling', 'branch after the source adventure flag'),
        1: ('trainer', ('TRAINER_JOHTO_JOEY', 'Route30_Text_YoungsterJoey1Seen', 'Route30_Text_YoungsterJoey1Beaten', 'Route30_Text_YoungsterJoey1After'),
            'no phone registration or rematch'),
        2: ('npc', 'Route30_Text_CooltrainerF', ''),
        3: ('trainer', ('TRAINER_JOHTO_DON', 'Route30_Text_BugCatcherDonSeen', 'Route30_Text_BugCatcherDonBeaten', 'Route30_Text_BugCatcherDonAfter'), ''),
        4: ('cuttree', None, 'FireRed EventScript_CutTree'),
        6: ('trainer', ('TRAINER_JOHTO_MIKEY', 'Route30_Text_YoungsterMikeySeen', 'Route30_Text_YoungsterMikeyBeaten', 'Route30_Text_YoungsterMikeyAfter'),
            'no phone registration or rematch')},
        {0: ('sign', 'Route30_Text_Sign'), 1: ('sign', 'Route30_Text_TrainerTips'), 2: ('sign', 'Route30_Text_Mailbox')}),
    'Route31': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_WADE', 'Route31_Text_BugCatcherWade1_Seen', 'Route31_Text_BugCatcherWade1_Beaten', 'Route31_Text_BugCatcherWade1_After'), ''),
        1: ('npc', 'Route31_Text_CooltrainerM', ''),
        2: ('npc', 'Route31_Text_SleepyMan', 'no Kenya mail errand (no Mr. Pokemon chain in the batch)'),
        3: ('npc', 'Route31_Text_Youngster', ''),
        4: ('itemflag', None, 'Poke Ball'), 5: ('itemflag', None, 'Antidote'),
        6: ('cuttree', None, 'FireRed EventScript_CutTree')},
        {0: ('sign', 'Route31_Text_Sign'), 1: ('sign', 'Route31_Text_DarkCaveSign')}),
    # Johto batch 2. Left out: Earl's walk into the Trainer School (story scene, school closed), the
    # Kimono Girl, Elm's call, the overworld Pokemon, the night light sprites and the berry tree.
    'VioletCity': (HNS, {
        0: ('npc', 'VioletCity_Text_Lass', ''), 1: ('npc', 'VioletCity_Text_Boy', ''),
        2: ('npc', 'VioletCity_Text_Gramps', ''), 4: ('npc', 'VioletCity_Text_Youngster', ''),
        5: ('itemflag', None, 'Pecha Berry'), 6: ('itemflag', None, 'Hyper Potion'), 27: ('itemflag', None, 'Rare Candy'),
        7: ('cuttree', None, 'FireRed EventScript_CutTree')},
        {0: ('sign', 'VioletCity_Text_GymSign')}),
    'Gate_Route31_VioletCity': (HNS, {
        0: ('npc', 'Gate_Route31_VioletCity_Text_Policeman', ''),
        1: ('npc', 'Gate_Route31_VioletCity_Text_CooltrainerF', 'HNS Battle Girl sprite; FireRed Cooltrainer F')}, {}),
    # Johto batch 3. Left out: Frieda (day-of-week gift; FireRed has no clock), the man with HNS's leftover
    # Violet script, the overworld Pokemon and the coord triggers (the seed man now gives it when talked to).
    'Route32': (HNS, {
        0: ('gift', ('ITEM_MIRACLE_SEED', 'Route32_Text_CooltrainerM_HaveThisSeed', 'Route32_Text_CooltrainerM_Experiences', False),
            'source gives it from a step trigger after Sprout Tower, the Gym and the egg errand; here when talked to'),
        1: ('gift', ('ITEM_TM05', 'Route32_Text_RoarIntro', 'Route32_Text_RoarOutro', False), 'TM05 Roar'),
        2: ('itemflag', None, 'Super Potion'), 3: ('itemflag', None, 'Great Ball'),
        4: ('itemflag', None, 'Potion'), 14: ('itemflag', None, 'Great Ball'),
        5: ('trainer', ('TRAINER_JOHTO_ALBERT', 'Route32_Text_YoungsterAlbert_Seen', 'Route32_Text_YoungsterAlbert_Beaten', 'Route32_Text_YoungsterAlbert_After'), ''),
        6: ('trainer', ('TRAINER_JOHTO_LIZ', 'Route32_Text_PicnickerLiz1_Seen', 'Route32_Text_PicnickerLiz1_Beaten', 'Route32_Text_PicnickerLiz1_After'),
            'no phone registration or rematch'),
        7: ('cuttree', None, 'FireRed EventScript_CutTree'), 31: ('cuttree', None, 'FireRed EventScript_CutTree'),
        8: ('trainer', ('TRAINER_JOHTO_ROLAND', 'Route32_Text_CamperRoland_Seen', 'Route32_Text_CamperRoland_Beaten', 'Route32_Text_CamperRoland_After'), ''),
        9: ('trainer', ('TRAINER_JOHTO_RALPH', 'Route32_Text_FisherRalph1_Seen', 'Route32_Text_FisherRalph1_Beaten', 'Route32_Text_FisherRalph1_After'), ''),
        10: ('trainer', ('TRAINER_JOHTO_HENRY', 'Route32_Text_FisherHenry_Seen', 'Route32_Text_FisherHenry_Beaten', 'Route32_Text_FisherHenry_After'),
             'no phone registration or rematch'),
        11: ('trainer', ('TRAINER_JOHTO_JUSTIN', 'Route32_Text_FisherJustin_Seen', 'Route32_Text_FisherJustin_Beaten', 'Route32_Text_FisherJustin_After'), ''),
        12: ('trainer', ('TRAINER_JOHTO_GORDON', 'Route32_Text_YoungsterGordon_Seen', 'Route32_Text_YoungsterGordon_Beaten', 'Route32_Text_YoungsterGordon_After'), ''),
        13: ('offer', ('Route32_Text_SlowpokeTailOffer', 'Route32_Text_ThoughtKidsLoaded', 'Route32_Text_RefusedSlowpokeTail'),
             'talk only; his step trigger is left out'),
        15: ('trainer', ('TRAINER_JOHTO_PETER', 'Route32_Text_BirdKeeperPeter_Seen', 'Route32_Text_BirdKeeperPeter_Beaten', 'Route32_Text_BirdKeeperPeter_After'), '')},
        {0: ('sign', 'Route32_Text_Sign1'), 1: ('sign', 'Route32_Text_Sign2'), 2: ('sign', 'Route32_Text_Sign3')}),
    # Johto batch 4. Left out: Team Rocket at the Slowpoke Well and the Gym, Silver, Kurt and the GS Ball,
    # the tailless Slowpoke (no FireRed sprite), the berry trees and Route 33's step triggers.
    'Route33': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_ANTHONY', 'Route33_Text_HikerAnthony2Seen', 'Route33_Text_HikerAnthony2Beaten', 'Route33_Text_HikerAnthony2After'),
            'no phone registration or rematch; unused slot TRAINER_AQUA_GRUNT_M'),
        1: ('npc', 'Route33_Text_Lass', '')}, {}),
    'AzaleaTown': (HNS, {
        1: ('npc', 'AzaleaTown_Text_GrampsAfter', 'branch after Team Rocket leaves the well'),
        3: ('npc', 'AzaleaTown_Text_Youngster', ''), 4: ('npc', 'AzaleaTown_Text_Teacher', '')},
        {0: ('sign', 'AzaleaTown_Text_GymSign'), 1: ('sign', 'AzaleaTown_Text_KurtsHouseSign'),
         2: ('sign', 'AzaleaTown_Text_IlexForestSign'), 3: ('sign', 'AzaleaTown_Text_CharcoalKilnSign'),
         4: ('sign', 'AzaleaTown_Text_SlowpokeWellSign'), 5: ('sign', 'AzaleaTown_Text_Sign')}),
    # Union Cave 1F; its trainers use unused FireRed/RS slots (opponents.h). Firebreathers: FireRed Burglar class.
    'UnionCave_1F': (HNS, {
        0: ('itemflag', None, 'Potion'), 3: ('itemflag', None, 'Awakening'),
        5: ('itemflag', None, 'Great Ball'), 8: ('itemflag', None, 'X Attack'),
        1: ('trainer', ('TRAINER_JOHTO_DANIEL', 'UnionCave_1F_Text_HikerDanielSeen', 'UnionCave_1F_Text_HikerDanielBeaten', 'UnionCave_1F_Text_HikerDanielAfter'), ''),
        2: ('trainer', ('TRAINER_JOHTO_RUSSELL', 'UnionCave_1F_Text_HikerRussellSeen', 'UnionCave_1F_Text_HikerRussellBeaten', 'UnionCave_1F_Text_HikerRussellAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_BILL', 'UnionCave_1F_Text_FirebreatherBillSeen', 'UnionCave_1F_Text_FirebreatherBillBeaten', 'UnionCave_1F_Text_FirebreatherBillAfter'), ''),
        6: ('trainer', ('TRAINER_JOHTO_LARRY', 'UnionCave_1F_Text_PokemaniacLarrySeen', 'UnionCave_1F_Text_PokemaniacLarryBeaten', 'UnionCave_1F_Text_PokemaniacLarryAfter'), ''),
        7: ('trainer', ('TRAINER_JOHTO_RAY', 'UnionCave_1F_Text_FirebreatherRaySeen', 'UnionCave_1F_Text_FirebreatherRayBeaten', 'UnionCave_1F_Text_FirebreatherRayAfter'), '')},
        {}),
    # Johto batch 5. Left out: the Farfetch'd chase (Charcoal Master and apprentice), Kurt, Celebi, the
    # Kimono Girl, the Headbutt tutor (FireRed has no Headbutt trees), the leftover Fury Cutter tutor,
    # the Day-Care man (FireRed's Day-Care is on Four Island) and Route 34's leftover Route 7 door sign.
    'Gate_AzaleaTown_IlexForest': (HNS, {
        0: ('npc', 'Gate_AzaleaTown_IlexForest_Text_Officer', ''), 1: ('npc', 'Gate_AzaleaTown_IlexForest_Text_Granny', '')}, {}),
    'IlexForest': (HNS, {
        3: ('cuttree', None, 'FireRed EventScript_CutTree'),
        4: ('npc', 'IlexForest_Text_Lass', ''),
        7: ('trainer', ('TRAINER_JOHTO_WAYNE', 'IlexForest_Text_BugCatcherWayneSeen', 'IlexForest_Text_BugCatcherWayneBeaten', 'IlexForest_Text_BugCatcherWayneAfter'), ''),
        8: ('itemflag', None, 'TM11 Sunny Day'), 9: ('itemflag', None, 'Full Heal'),
        10: ('itemflag', None, 'Ether'), 11: ('itemflag', None, 'Super Potion')},
        {0: ('sign', 'IlexForest_Text_Shrine'), 1: ('sign', 'IlexForest_Text_Signpost')}),
    'Gate_IlexForest_Route34': (HNS, {0: ('npc', 'Route34IlexForestGate_Text_Lass', '')}, {}),
    # Johto batch 9. Left out: the farm house and barn (Moomoo Milk), the Eon Ticket attendant, Silver, the shadow
    # on Route 39, the Gym, Lighthouse, cafe and port (closed). Jamie is a night trainer in HNS (no clock here).
    # Johto batch 10. Left out: Suicune and Eusine, the berry tree, the Modest Mint (no natures item in FireRed),
    # Mt. Mortar (closed), the Rage Candy Bar seller, the Gym, Routes 43/44 and the decorative Pokemon.
    # Johto batch 11. Left out: the Rockets at the toll gate (the policeman stands at his post, as after Mahogany's
    # hideout), the red Gyarados, Lance, Pryce, Wesley, the decorative Pokemon, berry trees and the two houses.
    'Gate_MahoganyTown_Route43': (HNS, {0: ('npc', 'Route43MahoganyGate_Text_Officer_RocketsCleared', 'text after the hideout')}, {}),
    'Route43': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_SPENCER', 'Route43_Text_CamperSpencerSeen', 'Route43_Text_CamperSpencerBeaten', 'Route43_Text_CamperSpencerAfter'), ''),
        1: ('trainer', ('TRAINER_JOHTO_TIFFANY', 'Route43_Text_PicnickerTiffanySeen', 'Route43_Text_PicnickerTiffanyBeaten', 'Route43_Text_PicnickerTiffanyWantsPicnic'), ''),
        2: ('trainer', ('TRAINER_JOHTO_BEN', 'Route43_Text_PokemaniacBenSeen', 'Route43_Text_PokemaniacBenBeaten', 'Route43_Text_PokemaniacBenAfter'), ''), 3: ('trainer', ('TRAINER_JOHTO_MARVIN', 'Route43_Text_FisherMarvinSeen', 'Route43_Text_FisherMarvinBeaten', 'Route43_Text_FisherMarvinAfter'), ''),
        16: ('trainer', ('TRAINER_JOHTO_RON', 'Route43_Text_PokemaniacRonSeen', 'Route43_Text_PokemaniacRonBeaten', 'Route43_Text_PokemaniacRonAfter'), ''), 17: ('trainer', ('TRAINER_JOHTO_BRENT', 'Route43_Text_PokemaniacBrentSeen', 'Route43_Text_PokemaniacBrentBeaten', 'Route43_Text_PokemaniacBrentAfter'), ''),
        4: ('itemflag', None, 'Max Ether')},
        {0: ('sign', 'Route43_Sign2'), 1: ('sign', 'Route43_Sign2'), 2: ('sign', 'Route43_TrainerTips')}),
    'Gate_Route43': (HNS, {
        2: ('gift', ('ITEM_TM36', 'Route43Gate_Text_OfficerFoundTM', 'Gate_Route43_Text_OfficerAvoidGrass', False),
            'TM36 Sludge Bomb; the source gives it once the Rockets leave')}, {}),
    'LakeOfRage': (HNS, {
        1: ('trainer', ('TRAINER_JOHTO_ANDRE', 'LakeOfRage_Text_FisherAndreSeen', 'LakeOfRage_Text_FisherAndreBeaten', 'LakeOfRage_Text_FisherAndreAfter'), ''), 4: ('trainer', ('TRAINER_JOHTO_RAYMOND', 'LakeOfRage_Text_FisherRaymondSeen', 'LakeOfRage_Text_FisherRaymondBeaten', 'LakeOfRage_Text_FisherRaymondAfter'), ''),
        6: ('trainer', ('TRAINER_JOHTO_LOIS', 'LakeOfRage_Text_CooltrainerfLoisSeen', 'LakeOfRage_Text_CooltrainerfLoisBeaten', 'LakeOfRage_Text_CooltrainerfLoisAfter'), ''), 8: ('trainer', ('TRAINER_JOHTO_AARON', 'LakeOfRage_Text_CooltrainermAaronSeen', 'LakeOfRage_Text_CooltrainermAaronBeaten', 'LakeOfRage_Text_CooltrainermAaronAfter'), ''),
        0: ('npc', 'LakeOfRage_Text_CooltrainerF', ''), 3: ('npc', 'LakeOfRage_Text_GrampsCleared', 'text after the hideout'),
        14: ('npc', 'LakeOfRage_Text_SuperNerd', ''),
        9: ('itemflag', None, 'Full Restore'), 10: ('itemflag', None, 'Max Revive'), 11: ('itemflag', None, 'Red Flute')},
        {0: ('sign', 'LakeOfRage_Text_Sign'), 1: ('sign', 'LakeOfRage_Text_FishingGuruSign')}),
    # Johto batch 12. Left out: the decorative Pokemon, berry trees and the two caves (Ice Path, Dark Cave).
    'Route44': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_PHIL', 'Route44_Text_PsychicPhilSeen', 'Route44_Text_PsychicPhilBeaten', 'Route44_Text_PsychicPhilAfter'), ''),
        1: ('trainer', ('TRAINER_JOHTO_EDGAR', 'Route44_Text_FisherEdgarSeen', 'Route44_Text_FisherEdgarBeaten', 'Route44_Text_FisherEdgarAfter'), ''),
        2: ('trainer', ('TRAINER_JOHTO_WILTON', 'Route44_Text_FisherWilton1Seen', 'Route44_Text_FisherWilton1Beaten', 'Route44_Text_FisherWiltonHugePoliwag'), ''),
        3: ('trainer', ('TRAINER_JOHTO_ZACH', 'Route44_Text_PokemaniacZachSeen', 'Route44_Text_PokemaniacZachBeaten', 'Route44_Text_PokemaniacZachAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_ALLEN', 'Route44_Text_CooltrainermAllenSeen', 'Route44_Text_CooltrainermAllenBeaten', 'Route44_Text_CooltrainermAllenAfter'), ''),
        5: ('trainer', ('TRAINER_JOHTO_CYBIL', 'Route44_Text_CooltrainerfCybilSeen', 'Route44_Text_CooltrainerfCybilBeaten', 'Route44_Text_CooltrainerfCybilAfter'), ''),
        26: ('trainer', ('TRAINER_JOHTO_VANCE', 'Route44_Text_BirdKeeperVance1Seen', 'Route44_Text_BirdKeeperVance1Beaten', 'Route44_Text_BirdKeeperVanceBirds'), ''),
        6: ('itemflag', None, 'Elixir'), 7: ('itemflag', None, 'Max Repel'), 8: ('itemflag', None, 'Ultra Ball')},
        {0: ('sign', 'Route44_Text_Sign1'), 1: ('sign', 'Route44_Text_Sign2')}),
    'Route46': (HNS, {
        4: ('trainer', ('TRAINER_JOHTO_TED', 'Route46_Text_CamperTedSeen', 'Route46_Text_CamperTedBeaten', 'Route46_Text_CamperTedAfterBattle'), ''),
        5: ('trainer', ('TRAINER_JOHTO_ERIN', 'Route46_Text_PicnickerErinSeen', 'Route46_Text_PicnickerErinBeaten', 'Route46_Text_PicnickerErinAfterBattle'), ''),
        6: ('trainer', ('TRAINER_JOHTO_BAILEY', 'Route46_Text_HikerBaileySeen', 'Route46_Text_HikerBaileyBeaten', 'Route46_Text_HikerBaileyAfterBattle'), ''),
        3: ('itemflag', None, 'Revive')},
        {0: ('sign', 'Route46_Text_Sign')}),
    # Johto batch 13. Left out: the Dark Cave entrance, the Ice Path, Dragon's Den, the Gym, the houses, Santos
    # (weekday sibling), the Gym boy and the Dragon's Den elder (story branches), and the decorative Pokemon.
    'Route45': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_ERIK', 'Route45_Text_HikerErikSeen', 'Route45_Text_HikerErikBeaten', 'Route45_Text_HikerErikAfter'), ''),
        1: ('trainer', ('TRAINER_JOHTO_RYAN', 'Route45_Text_CooltrainermRyanSeen', 'Route45_Text_CooltrainermRyanBeaten', 'Route45_Text_CooltrainermRyanAfter'), ''),
        3: ('trainer', ('TRAINER_JOHTO_MICHAEL', 'Route45_Text_HikerMichaelSeen', 'Route45_Text_HikerMichaelBeaten', 'Route45_Text_HikerMichaelAfter'), ''),
        5: ('trainer', ('TRAINER_JOHTO_KELLY', 'Route45_Text_CooltrainerfKellySeen', 'Route45_Text_CooltrainerfKellyBeaten', 'Route45_Text_CooltrainerfKellyAfter'), ''),
        6: ('trainer', ('TRAINER_JOHTO_KENJI', 'Route45_Text_BlackbeltKenjiSeen', 'Route45_Text_BlackbeltKenjiBeaten', 'Route45_Text_BlackbeltKenjiAfter'), ''),
        8: ('trainer', ('TRAINER_JOHTO_TIMOTHY', 'Route45_Text_HikerTimothySeen', 'Route45_Text_HikerTimothyBeaten', 'Route45_Text_HikerTimothyAfter'), ''),
        10: ('trainer', ('TRAINER_JOHTO_PARRY', 'Route45_Text_HikerParrySeen', 'Route45_Text_HikerParryBeaten', 'Route45_Text_HikerParryAfter'), ''),
        11: ('trainer', ('TRAINER_JOHTO_QUENTIN', 'Route45_Text_CamperQuentinSeen', 'Route45_Text_CamperQuentinBeaten', 'Route45_Text_CamperQuentinAfter'), ''),
        2: ('itemflag', None, 'Full Heal'), 4: ('itemflag', None, 'Nugget'), 7: ('itemflag', None, 'Elixir'),
        9: ('itemflag', None, 'Max Potion')},
        {0: ('sign', 'Route45_Text_Sign')}),
    'BlackthornCity': (HNS, {
        0: ('npc', 'BlackthornYoungsterText', ''), 3: ('npc', 'BlackthornCooltrainerF2Text', ''),
        4: ('npc', 'BlackthornCooltrainerF1Text', ''), 5: ('npc', 'BlackBeltText_VoicesInMyHead', '')},
        {0: ('sign', 'BlackthornCity_Text_DragonDenSign'), 1: ('sign', 'BlackthornGymSignText'),
         2: ('sign', 'BlackthornCity_Text_MoveDeletersHouseSign'), 3: ('sign', 'BlackthornCitySignText')}),
    # Johto batch 14. Every trainer here is a swimmer (Surf). Left out: Rock Smash boulders, the whirlpools and
    # their markers (Whirl Islands stay closed), Monica (weekday sibling), Eusine, Chuck's wife (Fly gift after
    # the Gym), the Trainer Hill gate and Cianwood's houses, Gym and cave.
    'Route40': (HNS, {
        4: ('trainer', ('TRAINER_JOHTO_ELAINE', 'Route40_Text_SwimmerfElaineSeen', 'Route40_Text_SwimmerfElaineBeaten', 'Route40_Text_SwimmerfElaineAfter'), ''),
        7: ('trainer', ('TRAINER_JOHTO_SIMON', 'Route40_Text_SwimmermSimonSeen', 'Route40_Text_SwimmermSimonBeaten', 'Route40_Text_SwimmermSimonAfter'), ''),
        8: ('trainer', ('TRAINER_JOHTO_PAULA', 'Route40_Text_SwimmerfPaulaSeen', 'Route40_Text_SwimmerfPaulaBeaten', 'Route40_Text_SwimmerfPaulaAfter'), ''),
        9: ('trainer', ('TRAINER_JOHTO_RANDALL', 'Route40_Text_SwimmermRandallSeen', 'Route40_Text_SwimmermRandallBeaten', 'Route40_Text_SwimmermRandallAfter'), ''),
        5: ('npc', 'Route40_Text_StandingYoungster', ''), 6: ('npc', 'Route40_Text_Lass1', '')}, {}),
    'Route41': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_BERKE', 'Route41_Text_SwimmermBerkeSeen', 'Route41_Text_SwimmermBerkeBeaten', 'Route41_Text_SwimmermBerkeAfter'), ''),
        1: ('trainer', ('TRAINER_JOHTO_CHARLIE', 'Route41_Text_SwimmermCharlieSeen', 'Route41_Text_SwimmermCharlieBeaten', 'Route41_Text_SwimmermCharlieAfter'), ''),
        2: ('trainer', ('TRAINER_JOHTO_DENISE', 'Route41_Text_SwimmerfDeniseSeen', 'Route41_Text_SwimmerfDeniseBeaten', 'Route41_Text_SwimmerfDeniseAfter'), ''),
        3: ('trainer', ('TRAINER_JOHTO_KARA', 'Route41_Text_SwimmerfKaraSeen', 'Route41_Text_SwimmerfKaraBeaten', 'Route41_Text_SwimmerfKaraAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_KAYLEE', 'Route41_Text_SwimmerfKayleeSeen', 'Route41_Text_SwimmerfKayleeBeaten', 'Route41_Text_SwimmerfKayleeAfter'), ''),
        5: ('trainer', ('TRAINER_JOHTO_GEORGE', 'Route41_Text_SwimmermGeorgeSeen', 'Route41_Text_SwimmermGeorgeBeaten', 'Route41_Text_SwimmermGeorgeAfter'), ''),
        6: ('trainer', ('TRAINER_JOHTO_KIRK', 'Route41_Text_SwimmermKirkSeen', 'Route41_Text_SwimmermKirkBeaten', 'Route41_Text_SwimmermKirkAfter'), ''),
        7: ('trainer', ('TRAINER_JOHTO_SUSIE', 'Route41_Text_SwimmerfSusieSeen', 'Route41_Text_SwimmerfSusieBeaten', 'Route41_Text_SwimmerfSusieAfter'), ''),
        8: ('trainer', ('TRAINER_JOHTO_MATHEW', 'Route41_Text_SwimmermMathewSeen', 'Route41_Text_SwimmermMathewBeaten', 'Route41_Text_SwimmermMathewAfter'), ''),
        9: ('trainer', ('TRAINER_JOHTO_WENDY', 'Route41_Text_SwimmerfWendySeen', 'Route41_Text_SwimmerfWendyBeaten', 'Route41_Text_SwimmerfWendyAfter'), ''),
        }, {}),
    'CianwoodCity': (HNS, {
        2: ('npc', 'CianwoodCity_Text_PokefanM', ''), 3: ('npc', 'CianwoodCity_Text_Youngster', ''),
        4: ('npc', 'CianwoodCity_Text_Lass', ''), 20: ('npc', 'CianwoodCity_Text_Engineer', '')},
        {0: ('sign', 'CianwoodCity_Text_Sign'), 1: ('sign', 'CianwoodCity_Text_PharmacySign'),
         2: ('sign', 'CianwoodCity_Text_GymSign'), 3: ('sign', 'CianwoodCity_Text_Safari')}),
    # Johto batch 15. Left out: Tohjo Falls, the Route 27 house, the Route 26 houses and gate, Route 26's north
    # half (Jake and Joyce stand there, outside this map), the berry tree and the decorative Pokemon.
    'Route27': (HNS, {
        1: ('trainer', ('TRAINER_JOHTO_MEGAN', 'Route27_Text_CooltrainerfMeganSeen', 'Route27_Text_CooltrainerfMeganBeaten', 'Route27_Text_CooltrainerfMeganAfter'), ''),
        2: ('trainer', ('TRAINER_JOHTO_BRIAN', 'Route27_Text_CooltrainermBrianSeen', 'Route27_Text_CooltrainermBrianBeaten', 'Route27_Text_CooltrainermBrianAfter'), ''),
        3: ('trainer', ('TRAINER_JOHTO_JOSE', 'Route27_Text_BirdKeeperJoseSeen', 'Route27_Text_BirdKeeperJoseBeaten', 'Route27_Text_BirdKeeperJoseAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_BLAKE', 'Route27_Text_CooltrainermBlakeSeen', 'Route27_Text_CooltrainermBlakeBeaten', 'Route27_Text_CooltrainermBlakeAfter'), ''),
        7: ('trainer', ('TRAINER_JOHTO_GILBERT', 'Route27_Text_PsychicGilbertSeen', 'Route27_Text_PsychicGilbertBeaten', 'Route27_Text_PsychicGilbertAfter'), ''),
        8: ('trainer', ('TRAINER_JOHTO_REENA', 'Route27_Text_CooltrainerfReenaSeen', 'Route27_Text_CooltrainerfReenaBeaten', 'Route27_Text_CooltrainerfReenaAfter'), ''),
        9: ('trainer', ('TRAINER_JOHTO_SCOTT', 'Route27_Text_FisherScottSeen', 'Route27_Text_FisherScottBeaten', 'Route27_Text_FisherScottAfter'), ''),
        0: ('npc', 'Route27_Text_FisherMessage', ''),
        5: ('itemflag', None, 'Rare Candy'), 6: ('itemflag', None, 'Moon Stone'), 18: ('itemflag', None, 'TM22 Solar Beam'),
        19: ('itemflag', None, 'Revive')},
        {0: ('sign', 'TohjoFalls_Text_Sign'), 1: ('sign', 'TohjoFalls_Text_Sign')}),
    'Route26': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_RICHARD', 'Route26_Text_PsychicRichardSeen', 'Route26_Text_PsychicRichardBeaten', 'Route26_Text_PsychicRichardAfter'), ''),
        14: ('trainer', ('TRAINER_JOHTO_GAVEN', 'Route26_Text_CooltrainermGavenSeen', 'Route26_Text_CooltrainermGavenBeaten', 'Route26_Text_CooltrainermGavenAfter'), ''),
        }, {}),
    # Johto batch 16. The Rock Smash boulders are left out (the paths stay open, as on Hoenn's routes).
    'DarkCave_SouthSide': (HNS, {
        0: ('itemflag', None, 'Potion'), 5: ('itemflag', None, 'Revive'), 6: ('itemflag', None, 'Max Revive'),
        7: ('itemflag', None, 'Elixir')}, {}),
    'DarkCave_NorthSide': (HNS, {
        0: ('itemflag', None, 'Black Flute'), 1: ('itemflag', None, 'Star Piece'),
        2: ('gift', ('ITEM_BLACK_GLASSES', 'DarkCave2_Text_Nerd', 'DarkCave2_Text_Nerd2', False), 'Black Glasses')}, {}),
    # Johto batch 17. Left out: the Strength boulder in Mt. Mortar 1F North, the Kimono Girl in Ice Path 1F and the
    # decorative Pokemon. Mt. Mortar's 2F and B1F and Ice Path's lower floors stay closed.
    'IcePath_1F': (HNS, {
        0: ('itemflag', None, 'PP Up'), 1: ('itemflag', None, 'HM07 Waterfall'), 2: ('itemflag', None, 'Protein')}, {}),
    'MtMortar_1F_South': (HNS, {0: ('itemflag', None, 'Elixir'), 1: ('itemflag', None, 'Revive')}, {}),
    'MtMortar_1F_North': (HNS, {
        7: ('trainer', ('TRAINER_JOHTO_MARKUS', 'MountMortar2_Text_SupernerdMarkusSeen', 'MountMortar2_Text_SupernerdMarkusBeaten', 'MountMortar2_Text_SupernerdMarkusAfter'), ''),
        8: ('trainer', ('TRAINER_JOHTO_MILLER', 'MountMortar2_Text_PokemaniacMillerSeen', 'MountMortar2_Text_PokemaniacMillerBeaten', 'MountMortar2_Text_PokemaniacMillerAfter'), ''),
        0: ('itemflag', None, 'Max Repel'), 1: ('itemflag', None, 'Escape Rope'), 2: ('itemflag', None, 'Ultra Ball'),
        3: ('itemflag', None, 'Nugget'), 4: ('itemflag', None, 'Max Potion'), 5: ('itemflag', None, 'Revive (HNS Full Incense script)'),
        6: ('itemflag', None, "King's Rock")}, {}),
    # Johto batch 18. Left out: the decorative Pokemon in Tohjo Falls, Giovanni's room, and the Victory Road
    # reception gate (closed, like Route 26's houses).
    'TohjoFalls_Cavern': (HNS, {0: ('itemflag', None, 'Heart Scale')}, {}),
    'Route26North': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_BETH', 'Route26_Text_CooltrainerfBethSeen', 'Route26_Text_CooltrainerfBethBeaten', 'Route26_Text_CooltrainerfBethAfter'), ''),
        1: ('trainer', ('TRAINER_JOHTO_JAKE', 'Route26_Text_CooltrainermJakeSeen', 'Route26_Text_CooltrainermJakeBeaten', 'Route26_Text_CooltrainermJakeAfter'), ''),
        5: ('trainer', ('TRAINER_JOHTO_JOYCE', 'Route26_Text_CooltrainerfJoyceSeen', 'Route26_Text_CooltrainerfJoyceBeaten', 'Route26_Text_CooltrainerfJoyceAfter'), ''),
        }, {}),
    'Gate_EcruteakCity_Route42': (HNS, {0: ('npc', 'Route42_Text_EcruteakGateOfficer', '')}, {}),
    'Route42': (HNS, {
        3: ('trainer', ('TRAINER_JOHTO_TULLY', 'Route42_Text_FisherTullySeen', 'Route42_Text_FisherTullyBeaten', 'Route42_Text_FisherTullyAfter'), ''), 4: ('trainer', ('TRAINER_JOHTO_SHANE', 'Route42_Text_PokemaniacShaneSeen', 'Route42_Text_PokemaniacShaneBeaten', 'Route42_Text_PokemaniacShaneAfter'), ''),
        5: ('trainer', ('TRAINER_JOHTO_BENJAMIN', 'Route42_Text_HikerBenjaminSeen', 'Route42_Text_HikerBenjaminBeaten', 'Route42_Text_HikerBenjaminAfter'), ''),
        0: ('itemflag', None, 'Super Potion'), 1: ('itemflag', None, 'Ultra Ball'), 2: ('cuttree', None, '')},
        {0: ('sign', 'Route42_Text_MtMortarSign1'), 1: ('sign', 'Route42_Text_MtMortarSign1'),
         2: ('sign', 'Route42_Text_Sign1'), 3: ('sign', 'Route42_Text_Sign1')}),
    'MahoganyTown': (HNS, {
        0: ('npc', 'MahoganyTown_Text_Gramps', 'branch after the Rocket hideout left out'),
        1: ('npc', 'MahoganyTown_Text_Lass', ''), 3: ('npc', 'MahoganyTown_Text_Fisher', '')},
        {0: ('sign', 'MahoganyTown_Text_Sign'), 1: ('sign', 'MahoganyTown_Text_RagecandybarSign'),
         2: ('sign', 'MahoganyTown_Text_GymSign')}),
    'Gate_EcruteakCity_Route38': (HNS, {0: ('npc', 'Route38_Text_EcruteakGateOfficer', '')}, {}),
    'Route38': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_HARRY', 'Route38_Text_SailorHarrySeen', 'Route38_Text_SailorHarryBeaten', 'Route38_Text_SailorHarryAfter'), ''), 1: ('trainer', ('TRAINER_JOHTO_DANA', 'Route38_Text_LassDana1Seen', 'Route38_Text_LassDana1Beaten', 'Route38_Text_LassDanaAfter'), ''),
        2: ('trainer', ('TRAINER_JOHTO_OLIVIA', 'Route38_Text_BeautyOliviaSeen', 'Route38_Text_BeautyOliviaBeaten', 'Route38_Text_BeautyOliviaAfter'), ''), 3: ('trainer', ('TRAINER_JOHTO_TOBY', 'Route38_Text_BirdKeeperTobySeen', 'Route38_Text_BirdKeeperTobyBeaten', 'Route38_Text_BirdKeeperTobyAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_CHAD', 'Route38_Text_SchoolboyChad1Seen', 'Route38_Text_SchoolboyChad1Beaten', 'Route38_Text_SchoolboyChadAfter'), ''), 5: ('trainer', ('TRAINER_JOHTO_VALERIE', 'Route38_Text_BeautyValerieSeen', 'Route38_Text_BeautyValerieBeaten', 'Route38_Text_BeautyValerieAfter'), ''),
        8: ('itemflag', None, 'Lax Incense')},
        {0: ('sign', 'Route38_Text_Sign'), 1: ('sign', 'Route38_Text_WaterSign')}),
    'Route39': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_NORMAN', 'Route39_Text_PsychicNormanSeen', 'Route39_Text_PsychicNormanBeaten', 'Route39_Text_PsychicNormanAfter'), ''), 2: ('trainer', ('TRAINER_JOHTO_RUTH', 'Route39_Text_PokefanFRuthSeen', 'Route39_Text_PokefanFRuthBeaten', 'Route39_Text_PokefanFRuthAfter'), ''),
        3: ('trainer', ('TRAINER_JOHTO_DEREK', 'Route39_Text_PokefanMDerekSeen', 'Route39_Text_PokefanMDerekBeaten', 'Route39_Text_PokefanMDerekAfter'), ''), 4: ('trainer', ('TRAINER_JOHTO_EUGENE', 'Route39_Text_SailorEugeneSeen', 'Route39_Text_SailorEugeneBeaten', 'Route39_Text_SailorEugeneAfter'), ''),
        17: ('trainer', ('TRAINER_JOHTO_JAMIE', 'Route39_Text_PokefanFJaimeSeen', 'Route39_Text_PokefanFJaimeBeaten', 'Route39_Text_PokefanFJaimeAfter'), ''),
        9: ('itemflag', None, 'TM31 Brick Break'), 18: ('itemflag', None, 'Nugget'), 19: ('npc', 'Route39_Text_Woman', '')},
        {0: ('sign', 'Route39_Text_TrainerTips'), 1: ('sign', 'Route39_Text_Sign'), 2: ('sign', 'Route39_Text_MoomooFarmSign')}),
    'OlivineCity': (HNS, {
        1: ('npc', 'OlivineCity_Text_Sailor2', ''), 2: ('npc', 'OlivineCity_Text_Sailor1', '')},
        # HNS places the TM34 ball at x=53 on a 52-wide map (unreachable): left out.
        {0: ('sign', 'OlivineCity_Text_GymSign'), 1: ('sign', 'OlivineCity_Text_LighthouseSign'),
         2: ('sign', 'OlivineCity_Text_PortSign'), 3: ('sign', 'OlivineCity_Text_Sign'), 4: ('sign', 'OlivineCity_Text_CafeSign')}),
    # Johto batch 8. Left out: Sunny (weekday gift), Silver, the Gym sage (gym closed) and the overworld Pokemon.
    'Route37': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_ANN_AND_ANNE', 'Route37_Text_TwinsAnnAndAnne1_Seen', 'Route37_Text_TwinsAnnAndAnne1_Beaten', 'Route37_Text_TwinsAnnAndAnne1_After', 'Route104_Text_GinaNotEnoughMons'), 'double battle'),
        1: ('trainer', ('TRAINER_JOHTO_ANN_AND_ANNE', 'Route37_Text_TwinsAnnAndAnne2_Seen', 'Route37_Text_TwinsAnnAndAnne2_Beaten', 'Route37_Text_TwinsAnnAndAnne2_After', 'Route104_Text_GinaNotEnoughMons'), 'double battle'),
        2: ('trainer', ('TRAINER_JOHTO_GREG', 'Route37_Text_PsychicGreg_Seen', 'Route37_Text_PsychicGreg_Beaten', 'Route37_Text_PsychicGreg_After'), '')},
        {0: ('sign', 'Route37_Text_Sign')}),
    'EcruteakCity': (HNS, {
        0: ('npc', 'EcruteakCity_Text_Lass1', ''), 2: ('npc', 'EcruteakCity_Text_Gramps2', ''),
        3: ('npc', 'EcruteakCity_Text_Gramps1', ''), 4: ('npc', 'EcruteakCity_Text_Gramps3', ''),
        5: ('npc', 'EcruteakCity_Text_Fisher_JasmineReturned', 'branch after the lighthouse Ampharos is cured'),
        6: ('npc', 'EcruteakCity_Text_Lass2', ''), 31: ('npc', 'EcruteakCity_Text_Youngster', ''),
        7: ('itemflag', None, 'Rare Candy')},
        {0: ('sign', 'EcruteakCity_Text_GymSign'), 1: ('sign', 'EcruteakCity_Text_DanceTheaterSign'),
         2: ('sign', 'EcruteakCity_Text_OfficeSign'), 3: ('sign', 'EcruteakCity_Text_BurnedTowerSign'),
         4: ('sign', 'EcruteakCity_Text_Sign')}),
    # Johto batch 7. Left out: Randy's Kenya errand, the Bug-Catching Contest attendants, Arthur (weekday gift;
    # no clock), Sudowoodo and the Ruins of Alph gate; the townspeople use their after-Sudowoodo lines.
    'Gate_GoldenrodCity_Route35': (HNS, {
        2: ('npc', 'Route35GoldenrodGate_Text_PokefanF_Lullaby', 'branch after Sudowoodo'),
        3: ('npc', 'Route35GoldenrodGate_Text_Fisher', '')}, {}),
    'Route35': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_KIM', 'Route35_Text_PicnickerKimSeen', 'Route35_Text_PicnickerKimBeaten', 'Route35_Text_PicnickerKimAfter'), ''), 1: ('trainer', ('TRAINER_JOHTO_BRYAN', 'Route35_Text_BirdKeeperBryanSeen', 'Route35_Text_BirdKeeperBryanBeaten', 'Route35_Text_BirdKeeperBryanAfter'), ''),
        2: ('trainer', ('TRAINER_JOHTO_BROOKE', 'Route35_Text_PicnickerBrookeSeen', 'Route35_Text_PicnickerBrookeBeaten', 'Route35_Text_PicnickerBrookeAfter'), ''), 3: ('trainer', ('TRAINER_JOHTO_ELLIOT', 'Route35_Text_CamperElliotSeen', 'Route35_Text_CamperElliotBeaten', 'Route35_Text_CamperElliotAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_IVAN', 'Route35_Text_CamperIvanSeen', 'Route35_Text_CamperIvanBeaten', 'Route35_Text_CamperIvanAfter'), ''), 5: ('trainer', ('TRAINER_JOHTO_WALT', 'Route35_Text_FirebreatherWaltSeen', 'Route35_Text_FirebreatherWaltBeaten', 'Route35_Text_FirebreatherWaltAfter'), ''),
        6: ('trainer', ('TRAINER_JOHTO_IRWIN', 'Route35_Text_JugglerIrwinSeen', 'Route35_Text_JugglerIrwinBeaten', 'Route35_Text_JugglerIrwinAfter'), ''), 7: ('trainer', ('TRAINER_JOHTO_DIRK', 'Route35_Text_OfficerDirkSeen', 'Route35_Text_OfficerDirkBeaten', 'Route35_Text_OfficerDirkAfter'), ''),
        8: ('trainer', ('TRAINER_JOHTO_ARNIE', 'Route35_Text_BugCatcherArnieSeen', 'Route35_Text_BugCatcherArnieBeaten', 'Route35_Text_BugCatcherArnieAfter'), ''),
        9: ('cuttree', None, 'FireRed EventScript_CutTree'),
        10: ('itemflag', None, 'TM39 Rock Tomb'), 11: ('itemflag', None, 'Nugget'), 20: ('itemflag', None, 'Paralyze Heal')},
        {0: ('sign', 'Route35_Text_Sign2'), 1: ('sign', 'Route35_Text_Sign')}),
    'Gate_NationalPark': (HNS, {
        0: ('npc', 'Route35NationalParkGate_Text_WeHoldContests', ''), 1: ('npc', 'Route35NationalParkGate_Text_WeHoldContests', ''),
        4: ('npc', 'Route35NationalParkGate_Text_Rocker', ''), 5: ('npc', 'Route35NationalParkGate_Text_YoungsterWonders', ''),
        6: ('npc', 'Route35NationalParkGate_Text_BC2', ''), 7: ('npc', 'Route35NationalParkGate_Text_Beauty', '')}, {}),
    'Route36': (HNS, {
        0: ('npc', 'Route36_Text_Lass2_ClearedSudowoodo', 'branch after Sudowoodo'),
        2: ('npc', 'Route36_Text_RockSmashGuy3', 'branch after he gave Rock Smash'),
        3: ('trainer', ('TRAINER_JOHTO_MARK', 'Route36_Text_PsychicMarkSeen', 'Route36_Text_PsychicMarkBeaten', 'Route36_Text_PsychicMarkAfter'), ''),
        4: ('trainer', ('TRAINER_JOHTO_ALAN', 'Route36_Text_SchoolboyAlan1Seen', 'Route36_Text_SchoolboyAlan1Beaten', 'Route36_Text_SchoolboyAlanAfter'), '')},
        {0: ('sign', 'Route36_Text_Sign_RuinsNorth'), 1: ('sign', 'Route36_Text_Tips1')}),
    # Johto batch 6. Left out: Team Rocket and its scout, Elm's call, the coin move tutor, the overworld
    # Snubbull/Mr. Mime and the step triggers; branches after Team Rocket leaves are used.
    'GoldenrodCity': (HNS, {
        0: ('npc', 'GoldenrodCity_Text_Lass', ''), 1: ('npc', 'GoldenrodCity_Text_HikerDay', 'day branch; FireRed has no clock'),
        3: ('npc', 'GoldenrodCity_Text_Gramps', ''), 6: ('npc', 'GoldenrodCity_Text_Youngster1', ''),
        7: ('npc', 'GoldenrodCity_Text_Youngster2', ''),
        8: ('npc', 'GoldenrodCity_Text_CooltrainerF1_Cleared', 'branch after the Rocket scout is gone'),
        11: ('npc', 'GoldenrodCity_Text_PokefanM', ''), 31: ('npc', 'GoldenrodCity_Text_Woman', ''),
        33: ('npc', 'GoldenrodCity_Text_Beauty', 'gym closed, so Whitney is away')},
        {0: ('sign', 'GoldenrodCity_Text_RadioTowerSign'), 1: ('sign', 'GoldenrodCity_Text_GameCornerSign'),
         2: ('sign', 'GoldenrodCity_Text_StationSign'), 3: ('sign', 'GoldenrodCity_Text_UndergroundSouthSign'),
         4: ('sign', 'GoldenrodCity_Text_UndergroundSouthSign'), 5: ('sign', 'GoldenrodCity_Text_BikeShopSign'),
         6: ('sign', 'GoldenrodCity_Text_GymSign'), 7: ('sign', 'GoldenrodCity_Text_FlowerShopSign'),
         8: ('sign', 'GoldenrodCity_Text_DeptStoreSign'), 9: ('sign', 'GoldenrodCity_Text_NameRaterSign'),
         10: ('sign', 'Text_EmptyTrashCan')}),
    'Route34': (HNS, {
        0: ('trainer', ('TRAINER_JOHTO_SAMUEL', 'Route34_Text_YoungsterSamuel_Seen', 'Route34_Text_YoungsterSamuel_Beaten', 'Route34_Text_YoungsterSamuel_After'), ''),
        1: ('trainer', ('TRAINER_JOHTO_BRANDON', 'Route34_Text_PokefanBrandon_Seen', 'Route34_Text_PokefanBrandon_Beaten', 'Route34_Text_PokefanBrandon_After'),
            'no phone registration or rematch'),
        2: ('trainer', ('TRAINER_JOHTO_TODD', 'Route34_Text_CamperTodd_Seen', 'Route34_Text_CamperTodd_Beaten', 'Route34_Text_CamperTodd_After'), ''),
        3: ('trainer', ('TRAINER_JOHTO_IAN', 'Route34_Text_YoungsterIan_Seen', 'Route34_Text_YoungsterIan_Beaten', 'Route34_Text_YoungsterIan_After'), ''),
        4: ('trainer', ('TRAINER_JOHTO_JENN', 'Route34_Text_CooltrainerJenn_Seen', 'Route34_Text_CooltrainerJenn_Beaten', 'Route34_Text_CooltrainerJenn_After2'), ''),
        5: ('trainer', ('TRAINER_JOHTO_IRENE', 'Route34_Text_CooltrainerIrene_Seen', 'Route34_Text_CooltrainerIrene_Beaten', 'Route34_Text_CooltrainerIrene_After2'), ''),
        6: ('trainer', ('TRAINER_JOHTO_KATE', 'Route34_Text_CooltrainerKate_Seen', 'Route34_Text_CooltrainerKate_Beaten', 'Route34_Text_CooltrainerKate_After'), ''),
        7: ('itemflag', None, 'Soft Sand'), 8: ('itemflag', None, 'Rare Candy'), 23: ('itemflag', None, 'Super Potion'),
        9: ('trainer', ('TRAINER_JOHTO_KEITH', 'Route34_Text_OfficerKeith_Seen', 'Route34_Text_OfficerKeith_Win', 'Route34_Text_OfficerKeith_After'),
            'night only in HNS; FireRed has no clock, so always there'),
        10: ('trainer', ('TRAINER_JOHTO_GINA', 'Route34_Text_PicnickerGina_Seen', 'Route34_Text_PicnickerGina_Beaten', 'Route34_Text_PicnickerGina_After'), '')},
        {0: ('sign', 'Route34_Text_TrainerTips'), 1: ('sign', 'Route34_Text_Sign_DayCare'), 2: ('sign', 'Route34_Text_Sign_Route34')}),
}
# Towns that are Fly destinations: world map flag set on entry (include/constants/flags.h).
WORLD_MAP_FLAGS = {'LittlerootTown': 'FLAG_WORLD_MAP_LITTLEROOT_TOWN', 'NewBarkTown': 'FLAG_WORLD_MAP_NEW_BARK_TOWN',
                   'OldaleTown': 'FLAG_WORLD_MAP_OLDALE_TOWN', 'PetalburgCity': 'FLAG_WORLD_MAP_PETALBURG_CITY',
                   'CherrygroveCity': 'FLAG_WORLD_MAP_CHERRYGROVE_CITY', 'VioletCity': 'FLAG_WORLD_MAP_VIOLET_CITY',
                   'AzaleaTown': 'FLAG_WORLD_MAP_AZALEA_TOWN', 'GoldenrodCity': 'FLAG_WORLD_MAP_GOLDENROD_CITY',
                   'RustboroCity': 'FLAG_WORLD_MAP_RUSTBORO_CITY', 'EcruteakCity': 'FLAG_WORLD_MAP_ECRUTEAK_CITY',
                   'VerdanturfTown': 'FLAG_WORLD_MAP_VERDANTURF_TOWN', 'OlivineCity': 'FLAG_WORLD_MAP_OLIVINE_CITY',
                   'MauvilleCity': 'FLAG_WORLD_MAP_MAUVILLE_CITY',
                   'MahoganyTown': 'FLAG_WORLD_MAP_MAHOGANY_TOWN',
                   'SlateportCity': 'FLAG_WORLD_MAP_SLATEPORT_CITY',
                   'BlackthornCity': 'FLAG_WORLD_MAP_BLACKTHORN_CITY',
                   'LavaridgeTown': 'FLAG_WORLD_MAP_LAVARIDGE_TOWN',
                   'CianwoodCity': 'FLAG_WORLD_MAP_CIANWOOD_CITY',
                   'FallarborTown': 'FLAG_WORLD_MAP_FALLARBOR_TOWN',
                   'FortreeCity': 'FLAG_WORLD_MAP_FORTREE_CITY',
                   'LilycoveCity': 'FLAG_WORLD_MAP_LILYCOVE_CITY',
                   'DewfordTown': 'FLAG_WORLD_MAP_DEWFORD_TOWN'}
# Items picked up once: map -> {source object index: (item, flag alias, free flag)}.
ITEM_FLAGS = {('Route102', 5): ('ITEM_POTION', 'FLAG_HIDE_HOENN_ROUTE102_POTION', 'FLAG_0x0AF'),
              ('Route103', 12): ('ITEM_GUARD_SPEC', 'FLAG_HIDE_HOENN_ROUTE103_GUARD_SPEC', 'FLAG_0x0B0'),
              ('Route103', 19): ('ITEM_PP_UP', 'FLAG_HIDE_HOENN_ROUTE103_PP_UP', 'FLAG_0x0B1'),
              ('PetalburgCity', 5): ('ITEM_MAX_REVIVE', 'FLAG_HIDE_HOENN_PETALBURG_MAX_REVIVE', 'FLAG_0x0B2'),
              ('PetalburgCity', 6): ('ITEM_ETHER', 'FLAG_HIDE_HOENN_PETALBURG_ETHER', 'FLAG_0x0B3'),
              ('Route31', 4): ('ITEM_POKE_BALL', 'FLAG_HIDE_JOHTO_ROUTE31_POKE_BALL', 'FLAG_0x0B4'),
              ('Route31', 5): ('ITEM_ANTIDOTE', 'FLAG_HIDE_JOHTO_ROUTE31_ANTIDOTE', 'FLAG_0x0B5'),
              ('VioletCity', 5): ('ITEM_PECHA_BERRY', 'FLAG_HIDE_JOHTO_VIOLET_PECHA_BERRY', 'FLAG_0x0B7'),
              ('VioletCity', 6): ('ITEM_HYPER_POTION', 'FLAG_HIDE_JOHTO_VIOLET_HYPER_POTION', 'FLAG_0x0B8'),
              ('VioletCity', 27): ('ITEM_RARE_CANDY', 'FLAG_HIDE_JOHTO_VIOLET_RARE_CANDY', 'FLAG_0x0B9'),
              ('Route32', 2): ('ITEM_SUPER_POTION', 'FLAG_HIDE_JOHTO_ROUTE32_SUPER_POTION', 'FLAG_0x0BA'),
              ('Route32', 3): ('ITEM_GREAT_BALL', 'FLAG_HIDE_JOHTO_ROUTE32_GREAT_BALL', 'FLAG_0x0BB'),
              ('Route32', 4): ('ITEM_POTION', 'FLAG_HIDE_JOHTO_ROUTE32_POTION', 'FLAG_0x0BC'),
              ('Route32', 14): ('ITEM_GREAT_BALL', 'FLAG_HIDE_JOHTO_ROUTE32_GREAT_BALL_2', 'FLAG_0x0BD'),
              ('Route32', 0): ('ITEM_MIRACLE_SEED', 'FLAG_GOT_JOHTO_MIRACLE_SEED', 'FLAG_0x0BE'),
              ('Route32', 1): ('ITEM_TM05', 'FLAG_GOT_JOHTO_TM_ROAR', 'FLAG_0x0BF'),
              ('UnionCave_1F', 0): ('ITEM_POTION', 'FLAG_HIDE_JOHTO_UNION_CAVE_POTION', 'FLAG_0x0C0'),
              ('UnionCave_1F', 3): ('ITEM_AWAKENING', 'FLAG_HIDE_JOHTO_UNION_CAVE_AWAKENING', 'FLAG_0x0C1'),
              ('UnionCave_1F', 5): ('ITEM_GREAT_BALL', 'FLAG_HIDE_JOHTO_UNION_CAVE_GREAT_BALL', 'FLAG_0x0C2'),
              ('UnionCave_1F', 8): ('ITEM_X_ATTACK', 'FLAG_HIDE_JOHTO_UNION_CAVE_X_ATTACK', 'FLAG_0x0C3'),
              ('IlexForest', 8): ('ITEM_TM11', 'FLAG_HIDE_JOHTO_ILEX_FOREST_TM_SUNNY_DAY', 'FLAG_0x0C4'),
              ('IlexForest', 9): ('ITEM_FULL_HEAL', 'FLAG_HIDE_JOHTO_ILEX_FOREST_FULL_HEAL', 'FLAG_0x0C5'),
              ('IlexForest', 10): ('ITEM_ETHER', 'FLAG_HIDE_JOHTO_ILEX_FOREST_ETHER', 'FLAG_0x0C6'),
              ('IlexForest', 11): ('ITEM_SUPER_POTION', 'FLAG_HIDE_JOHTO_ILEX_FOREST_SUPER_POTION', 'FLAG_0x0C7'),
              ('Route34', 7): ('ITEM_SOFT_SAND', 'FLAG_HIDE_JOHTO_ROUTE34_SOFT_SAND', 'FLAG_0x0C8'),
              ('Route34', 8): ('ITEM_RARE_CANDY', 'FLAG_HIDE_JOHTO_ROUTE34_RARE_CANDY', 'FLAG_0x0C9'),
              ('Route34', 23): ('ITEM_SUPER_POTION', 'FLAG_HIDE_JOHTO_ROUTE34_SUPER_POTION', 'FLAG_0x0CA'),
              ('Route35', 10): ('ITEM_TM39', 'FLAG_HIDE_JOHTO_ROUTE35_TM_ROCK_TOMB', 'FLAG_0x0CB'),
              ('Route35', 11): ('ITEM_NUGGET', 'FLAG_HIDE_JOHTO_ROUTE35_NUGGET', 'FLAG_0x0CC'),  # HNS Big Nugget; FireRed has Nugget
              ('Route35', 20): ('ITEM_PARALYZE_HEAL', 'FLAG_HIDE_JOHTO_ROUTE35_PARALYZE_HEAL', 'FLAG_0x0CD'),
              ('Route104', 20): ('ITEM_PP_UP', 'FLAG_HIDE_HOENN_ROUTE104_PP_UP', 'FLAG_0x0CE'),
              ('Route104', 26): ('ITEM_POKE_BALL', 'FLAG_HIDE_HOENN_ROUTE104_POKE_BALL', 'FLAG_0x0CF'),
              ('Route104', 28): ('ITEM_X_ACCURACY', 'FLAG_HIDE_HOENN_ROUTE104_X_ACCURACY', 'FLAG_0x0D0'),
              ('Route104', 30): ('ITEM_POTION', 'FLAG_HIDE_HOENN_ROUTE104_POTION', 'FLAG_0x0D1'),
              ('PetalburgWoods', 4): ('ITEM_GREAT_BALL', 'FLAG_HIDE_HOENN_PETALBURG_WOODS_GREAT_BALL', 'FLAG_0x0D2'),
              ('PetalburgWoods', 5): ('ITEM_X_ATTACK', 'FLAG_HIDE_HOENN_PETALBURG_WOODS_X_ATTACK', 'FLAG_0x0D3'),
              ('PetalburgWoods', 6): ('ITEM_ETHER', 'FLAG_HIDE_HOENN_PETALBURG_WOODS_ETHER', 'FLAG_0x0D4'),
              ('PetalburgWoods', 11): ('ITEM_PARALYZE_HEAL', 'FLAG_HIDE_HOENN_PETALBURG_WOODS_PARALYZE_HEAL', 'FLAG_0x0D5'),
              ('RustboroCity', 11): ('ITEM_X_DEFEND', 'FLAG_HIDE_HOENN_RUSTBORO_X_DEFEND', 'FLAG_0x0D6'),
              ('EcruteakCity', 7): ('ITEM_RARE_CANDY', 'FLAG_HIDE_JOHTO_ECRUTEAK_RARE_CANDY', 'FLAG_0x0D7'),
              ('Route116', 6): ('ITEM_ETHER', 'FLAG_HIDE_HOENN_ROUTE116_ETHER', 'FLAG_0x0D8'),
              ('Route116', 7): ('ITEM_REPEL', 'FLAG_HIDE_HOENN_ROUTE116_REPEL', 'FLAG_0x0D9'),
              ('Route116', 14): ('ITEM_HP_UP', 'FLAG_HIDE_HOENN_ROUTE116_HP_UP', 'FLAG_0x0DA'),
              ('Route116', 19): ('ITEM_X_SPECIAL', 'FLAG_HIDE_HOENN_ROUTE116_X_SPECIAL', 'FLAG_0x0DB'),
              ('Route116', 24): ('ITEM_POTION', 'FLAG_HIDE_HOENN_ROUTE116_POTION', 'FLAG_0x0DC'),
              ('RusturfTunnel', 2): ('ITEM_POKE_BALL', 'FLAG_HIDE_HOENN_RUSTURF_POKE_BALL', 'FLAG_0x0DD'),
              ('RusturfTunnel', 3): ('ITEM_MAX_ETHER', 'FLAG_HIDE_HOENN_RUSTURF_MAX_ETHER', 'FLAG_0x0DE'),
              ('Route38', 8): ('ITEM_LAX_INCENSE', 'FLAG_HIDE_JOHTO_ROUTE38_LAX_INCENSE', 'FLAG_0x0DF'),
              ('Route39', 9): ('ITEM_TM31', 'FLAG_HIDE_JOHTO_ROUTE39_TM_BRICK_BREAK', 'FLAG_0x0E0'),
              ('Route39', 18): ('ITEM_NUGGET', 'FLAG_HIDE_JOHTO_ROUTE39_NUGGET', 'FLAG_0x0E1'),  # HNS Big Nugget
              ('Route117', 13): ('ITEM_GREAT_BALL', 'FLAG_HIDE_HOENN_ROUTE117_GREAT_BALL', 'FLAG_0x0E2'),
              ('Route117', 15): ('ITEM_REVIVE', 'FLAG_HIDE_HOENN_ROUTE117_REVIVE', 'FLAG_0x0E3'),
              ('MauvilleCity', 8): ('ITEM_X_SPEED', 'FLAG_HIDE_HOENN_MAUVILLE_X_SPEED', 'FLAG_0x0E4'),
              ('Route42', 0): ('ITEM_SUPER_POTION', 'FLAG_HIDE_JOHTO_ROUTE42_SUPER_POTION', 'FLAG_0x0E5'),
              ('Route42', 1): ('ITEM_ULTRA_BALL', 'FLAG_HIDE_JOHTO_ROUTE42_ULTRA_BALL', 'FLAG_0x0E6'),
              ('Route110', 18): ('ITEM_DIRE_HIT', 'FLAG_HIDE_HOENN_ROUTE110_DIRE_HIT', 'FLAG_0x0E7'),
              ('Route110', 19): ('ITEM_RARE_CANDY', 'FLAG_HIDE_HOENN_ROUTE110_RARE_CANDY', 'FLAG_0x0E8'),
              ('Route110', 34): ('ITEM_ELIXIR', 'FLAG_HIDE_HOENN_ROUTE110_ELIXIR', 'FLAG_0x0E9'),
              ('Route43', 4): ('ITEM_MAX_ETHER', 'FLAG_HIDE_JOHTO_ROUTE43_MAX_ETHER', 'FLAG_0x0EB'),
              ('LakeOfRage', 9): ('ITEM_FULL_RESTORE', 'FLAG_HIDE_JOHTO_LAKE_OF_RAGE_FULL_RESTORE', 'FLAG_0x0EC'),
              ('LakeOfRage', 10): ('ITEM_MAX_REVIVE', 'FLAG_HIDE_JOHTO_LAKE_OF_RAGE_MAX_REVIVE', 'FLAG_0x0ED'),
              ('LakeOfRage', 11): ('ITEM_RED_FLUTE', 'FLAG_HIDE_JOHTO_LAKE_OF_RAGE_RED_FLUTE', 'FLAG_0x0EE'),
              ('Gate_Route43', 2): ('ITEM_TM36', 'FLAG_GOT_JOHTO_TM_SLUDGE_BOMB', 'FLAG_0x0EA'),
              ('Route118', 19): ('ITEM_HYPER_POTION', 'FLAG_HIDE_HOENN_ROUTE118_HYPER_POTION', 'FLAG_0x0EF'),
              ('Route111', 17): ('ITEM_TM37', 'FLAG_HIDE_HOENN_ROUTE111_TM_SANDSTORM', 'FLAG_0x0F0'),
              ('Route111', 24): ('ITEM_STARDUST', 'FLAG_HIDE_HOENN_ROUTE111_STARDUST', 'FLAG_0x0F1'),
              ('Route111', 25): ('ITEM_HP_UP', 'FLAG_HIDE_HOENN_ROUTE111_HP_UP', 'FLAG_0x0F2'),
              ('Route111', 41): ('ITEM_ELIXIR', 'FLAG_HIDE_HOENN_ROUTE111_ELIXIR', 'FLAG_0x0F3'),
              ('Route44', 6): ('ITEM_ELIXIR', 'FLAG_HIDE_JOHTO_ROUTE44_ELIXIR', 'FLAG_0x0F4'),
              ('Route44', 7): ('ITEM_MAX_REPEL', 'FLAG_HIDE_JOHTO_ROUTE44_MAX_REPEL', 'FLAG_0x0F5'),
              ('Route44', 8): ('ITEM_ULTRA_BALL', 'FLAG_HIDE_JOHTO_ROUTE44_ULTRA_BALL', 'FLAG_0x0F6'),
              ('Route46', 3): ('ITEM_REVIVE', 'FLAG_HIDE_JOHTO_ROUTE46_REVIVE', 'FLAG_0x0F7'),
              ('Route109', 9): ('ITEM_PP_UP', 'FLAG_HIDE_HOENN_ROUTE109_PP_UP', 'FLAG_0x0F8'),
              ('Route109', 22): ('ITEM_POTION', 'FLAG_HIDE_HOENN_ROUTE109_POTION', 'FLAG_0x0F9'),
              ('Route109', 14): ('ITEM_SOFT_SAND', 'FLAG_GOT_HOENN_SOFT_SAND', 'FLAG_0x0FA'),
              ('Route45', 2): ('ITEM_FULL_HEAL', 'FLAG_HIDE_JOHTO_ROUTE45_FULL_HEAL', 'FLAG_0x0FB'),
              ('Route45', 4): ('ITEM_NUGGET', 'FLAG_HIDE_JOHTO_ROUTE45_NUGGET', 'FLAG_0x0FC'),
              ('Route45', 7): ('ITEM_ELIXIR', 'FLAG_HIDE_JOHTO_ROUTE45_ELIXIR', 'FLAG_0x0FD'),
              ('Route45', 9): ('ITEM_MAX_POTION', 'FLAG_HIDE_JOHTO_ROUTE45_MAX_POTION', 'FLAG_0x0FE'),
              ('Route112', 11): ('ITEM_NUGGET', 'FLAG_HIDE_HOENN_ROUTE112_NUGGET', 'FLAG_0x0FF'),
              ('Route113', 5): ('ITEM_MAX_ETHER', 'FLAG_HIDE_HOENN_ROUTE113_MAX_ETHER', 'FLAG_0x100'),
              ('Route113', 6): ('ITEM_SUPER_REPEL', 'FLAG_HIDE_HOENN_ROUTE113_SUPER_REPEL', 'FLAG_0x101'),
              ('Route113', 11): ('ITEM_HYPER_POTION', 'FLAG_HIDE_HOENN_ROUTE113_HYPER_POTION', 'FLAG_0x102'),
              ('Route27', 5): ('ITEM_RARE_CANDY', 'FLAG_HIDE_JOHTO_ROUTE27_RARE_CANDY', 'FLAG_0x103'),
              ('Route27', 6): ('ITEM_MOON_STONE', 'FLAG_HIDE_JOHTO_ROUTE27_MOON_STONE', 'FLAG_0x104'),
              ('Route27', 18): ('ITEM_TM22', 'FLAG_HIDE_JOHTO_ROUTE27_TM_SOLAR_BEAM', 'FLAG_0x105'),
              ('Route27', 19): ('ITEM_REVIVE', 'FLAG_HIDE_JOHTO_ROUTE27_REVIVE', 'FLAG_0x106'),
              ('Route114', 8): ('ITEM_RARE_CANDY', 'FLAG_HIDE_HOENN_ROUTE114_RARE_CANDY', 'FLAG_0x107'),
              ('Route114', 9): ('ITEM_PROTEIN', 'FLAG_HIDE_HOENN_ROUTE114_PROTEIN', 'FLAG_0x108'),
              ('Route114', 25): ('ITEM_ENERGY_POWDER', 'FLAG_HIDE_HOENN_ROUTE114_ENERGY_POWDER', 'FLAG_0x109'),
              ('Route115', 9): ('ITEM_SUPER_POTION', 'FLAG_HIDE_HOENN_ROUTE115_SUPER_POTION', 'FLAG_0x10A'),
              ('Route115', 10): ('ITEM_TM01', 'FLAG_HIDE_HOENN_ROUTE115_TM_FOCUS_PUNCH', 'FLAG_0x10B'),
              ('Route115', 11): ('ITEM_IRON', 'FLAG_HIDE_HOENN_ROUTE115_IRON', 'FLAG_0x10C'),
              ('Route115', 15): ('ITEM_GREAT_BALL', 'FLAG_HIDE_HOENN_ROUTE115_GREAT_BALL', 'FLAG_0x10D'),
              ('Route115', 21): ('ITEM_PP_UP', 'FLAG_HIDE_HOENN_ROUTE115_PP_UP', 'FLAG_0x10E'),
              ('Route115', 22): ('ITEM_HEAL_POWDER', 'FLAG_HIDE_HOENN_ROUTE115_HEAL_POWDER', 'FLAG_0x10F'),
              ('Route114', 11): ('ITEM_TM05', 'FLAG_GOT_HOENN_TM_ROAR', 'FLAG_0x110'),
              ('DarkCave_SouthSide', 0): ('ITEM_POTION', 'FLAG_HIDE_JOHTO_DARK_CAVE_POTION', 'FLAG_0x111'),
              ('DarkCave_SouthSide', 5): ('ITEM_REVIVE', 'FLAG_HIDE_JOHTO_DARK_CAVE_REVIVE', 'FLAG_0x112'),
              ('DarkCave_SouthSide', 6): ('ITEM_MAX_REVIVE', 'FLAG_HIDE_JOHTO_DARK_CAVE_MAX_REVIVE', 'FLAG_0x113'),
              ('DarkCave_SouthSide', 7): ('ITEM_ELIXIR', 'FLAG_HIDE_JOHTO_DARK_CAVE_ELIXIR', 'FLAG_0x114'),
              ('DarkCave_NorthSide', 0): ('ITEM_BLACK_FLUTE', 'FLAG_HIDE_JOHTO_DARK_CAVE_BLACK_FLUTE', 'FLAG_0x115'),
              ('DarkCave_NorthSide', 1): ('ITEM_STAR_PIECE', 'FLAG_HIDE_JOHTO_DARK_CAVE_STAR_PIECE', 'FLAG_0x116'),
              ('DarkCave_NorthSide', 2): ('ITEM_BLACK_GLASSES', 'FLAG_GOT_JOHTO_BLACK_GLASSES', 'FLAG_0x117'),
              ('Route119', 16): ('ITEM_SUPER_REPEL', 'FLAG_HIDE_HOENN_ROUTE119_SUPER_REPEL', 'FLAG_0x118'),
              ('Route119', 17): ('ITEM_ZINC', 'FLAG_HIDE_HOENN_ROUTE119_ZINC', 'FLAG_0x119'),
              ('Route119', 18): ('ITEM_ELIXIR', 'FLAG_HIDE_HOENN_ROUTE119_ELIXIR_1', 'FLAG_0x11A'),
              ('Route119', 19): ('ITEM_LEAF_STONE', 'FLAG_HIDE_HOENN_ROUTE119_LEAF_STONE', 'FLAG_0x11B'),
              ('Route119', 20): ('ITEM_RARE_CANDY', 'FLAG_HIDE_HOENN_ROUTE119_RARE_CANDY', 'FLAG_0x11C'),
              ('Route119', 21): ('ITEM_HYPER_POTION', 'FLAG_HIDE_HOENN_ROUTE119_HYPER_POTION_1', 'FLAG_0x11D'),
              ('Route119', 31): ('ITEM_HYPER_POTION', 'FLAG_HIDE_HOENN_ROUTE119_HYPER_POTION_2', 'FLAG_0x11E'),
              ('Route119', 40): ('ITEM_NUGGET', 'FLAG_HIDE_HOENN_ROUTE119_NUGGET', 'FLAG_0x11F'),
              ('Route119', 41): ('ITEM_ELIXIR', 'FLAG_HIDE_HOENN_ROUTE119_ELIXIR_2', 'FLAG_0x120'),
              ('IcePath_1F', 0): ('ITEM_PP_UP', 'FLAG_HIDE_JOHTO_ICE_PATH_PP_UP', 'FLAG_0x121'),
              ('IcePath_1F', 1): ('ITEM_HM07', 'FLAG_HIDE_JOHTO_ICE_PATH_HM_WATERFALL', 'FLAG_0x122'),
              ('IcePath_1F', 2): ('ITEM_PROTEIN', 'FLAG_HIDE_JOHTO_ICE_PATH_PROTEIN', 'FLAG_0x123'),
              ('MtMortar_1F_South', 0): ('ITEM_ELIXIR', 'FLAG_HIDE_JOHTO_MT_MORTAR_ELIXIR', 'FLAG_0x124'),
              ('MtMortar_1F_South', 1): ('ITEM_REVIVE', 'FLAG_HIDE_JOHTO_MT_MORTAR_REVIVE', 'FLAG_0x125'),
              ('MtMortar_1F_North', 0): ('ITEM_MAX_REPEL', 'FLAG_HIDE_JOHTO_MT_MORTAR_MAX_REPEL', 'FLAG_0x126'),
              ('MtMortar_1F_North', 1): ('ITEM_ESCAPE_ROPE', 'FLAG_HIDE_JOHTO_MT_MORTAR_ESCAPE_ROPE', 'FLAG_0x127'),
              ('MtMortar_1F_North', 2): ('ITEM_ULTRA_BALL', 'FLAG_HIDE_JOHTO_MT_MORTAR_ULTRA_BALL', 'FLAG_0x128'),
              ('MtMortar_1F_North', 3): ('ITEM_NUGGET', 'FLAG_HIDE_JOHTO_MT_MORTAR_NUGGET', 'FLAG_0x129'),
              ('MtMortar_1F_North', 4): ('ITEM_MAX_POTION', 'FLAG_HIDE_JOHTO_MT_MORTAR_MAX_POTION', 'FLAG_0x12A'),
              ('MtMortar_1F_North', 5): ('ITEM_REVIVE', 'FLAG_HIDE_JOHTO_MT_MORTAR_REVIVE_2', 'FLAG_0x12B'),
              ('MtMortar_1F_North', 6): ('ITEM_KINGS_ROCK', 'FLAG_HIDE_JOHTO_MT_MORTAR_KINGS_ROCK', 'FLAG_0x12C'),
              ('Route120', 15): ('ITEM_NUGGET', 'FLAG_HIDE_HOENN_ROUTE120_NUGGET', 'FLAG_0x12D'),
              ('Route120', 21): ('ITEM_FULL_HEAL', 'FLAG_HIDE_HOENN_ROUTE120_FULL_HEAL', 'FLAG_0x12E'),
              ('Route120', 27): ('ITEM_NEST_BALL', 'FLAG_HIDE_HOENN_ROUTE120_NEST_BALL', 'FLAG_0x12F'),
              ('Route120', 28): ('ITEM_HYPER_POTION', 'FLAG_HIDE_HOENN_ROUTE120_HYPER_POTION', 'FLAG_0x130'),
              ('Route120', 43): ('ITEM_REVIVE', 'FLAG_HIDE_HOENN_ROUTE120_REVIVE', 'FLAG_0x131'),
              ('Route121', 20): ('ITEM_CARBOS', 'FLAG_HIDE_HOENN_ROUTE121_CARBOS', 'FLAG_0x132'),
              ('Route121', 27): ('ITEM_REVIVE', 'FLAG_HIDE_HOENN_ROUTE121_REVIVE', 'FLAG_0x133'),
              ('Route121', 28): ('ITEM_ZINC', 'FLAG_HIDE_HOENN_ROUTE121_ZINC', 'FLAG_0x134'),
              ('LilycoveCity', 10): ('ITEM_MAX_REPEL', 'FLAG_HIDE_HOENN_LILYCOVE_MAX_REPEL', 'FLAG_0x135'),
              ('TohjoFalls_Cavern', 0): ('ITEM_HEART_SCALE', 'FLAG_HIDE_JOHTO_TOHJO_FALLS_HEART_SCALE', 'FLAG_0x136'),
              ('Route123', 20): ('ITEM_CALCIUM', 'FLAG_HIDE_HOENN_ROUTE123_CALCIUM', 'FLAG_0x137'),
              ('Route123', 31): ('ITEM_ULTRA_BALL', 'FLAG_HIDE_HOENN_ROUTE123_ULTRA_BALL', 'FLAG_0x138'),
              ('Route123', 32): ('ITEM_ELIXIR', 'FLAG_HIDE_HOENN_ROUTE123_ELIXIR', 'FLAG_0x139'),
              ('Route123', 39): ('ITEM_PP_UP', 'FLAG_HIDE_HOENN_ROUTE123_PP_UP', 'FLAG_0x13A'),
              ('Route123', 42): ('ITEM_REVIVAL_HERB', 'FLAG_HIDE_HOENN_ROUTE123_REVIVAL_HERB', 'FLAG_0x13B'),
              ('Route105', 4): ('ITEM_IRON', 'FLAG_HIDE_HOENN_ROUTE105_IRON', 'FLAG_0x13C'),
              ('Route106', 4): ('ITEM_PROTEIN', 'FLAG_HIDE_HOENN_ROUTE106_PROTEIN', 'FLAG_0x13D'),
              # one-time gifts (kind 'gift'): the flag marks the item as given
              ('CherrygroveCity', 1): ('ITEM_MYSTIC_WATER', 'FLAG_GOT_JOHTO_MYSTIC_WATER', 'FLAG_0x0B6')}
# Objects whose source elevation differs from the cells they stand on (map, index) -> elevation. A wanderer only
# takes the cell's elevation after its first step, so until then the player could not talk to it.
ELEVATIONS = {('Route115', 0): 5}  # the woman on the Route 115 plateau (source elevation 3)
# Emerald movement types FireRed lacks -> the closest FireRed one.
MOVEMENTS = {'MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_DOWN': 'MOVEMENT_TYPE_WALK_IN_PLACE_DOWN',
             'MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_UP': 'MOVEMENT_TYPE_WALK_IN_PLACE_UP',
             'MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_LEFT': 'MOVEMENT_TYPE_WALK_IN_PLACE_LEFT',
             'MOVEMENT_TYPE_WALK_SLOWLY_IN_PLACE_RIGHT': 'MOVEMENT_TYPE_WALK_IN_PLACE_RIGHT'}  # Route 109 tubers
# Lab furniture scripts (Emerald's labels, also reused by HNS) -> text shown.
LAB_SIGNS = {
    'LittlerootTown_ProfessorBirchsLab_EventScript_Machine': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine'),
    'LittlerootTown_ProfessorBirchsLab_EventScript_Bookshelf': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon'),
    'LittlerootTown_ProfessorBirchsLab_EventScript_PC': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch'),
    'LittlerootTown_ProfessorBirchsLab_EventScript_Book': ('sign', 'LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead'),
    'NewBarkTown_Lab_EventScript_HealingMachine1': ('heal', 'LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine'),
    'EventScript_EmptyTrashCan': ('sign', 'Text_EmptyTrashCan'),
}


_SOURCES = {}


def source_text(repo, label):
    """The .string lines of a text label (single or double colon), anywhere in the repo's data folder."""
    if repo not in _SOURCES:  # read each repo's scripts once: hundreds of labels over thousands of files
        _SOURCES[repo] = [(path, path.read_text(encoding='utf-8')) for path in sorted((repo / 'data').rglob('*.inc'))]
    pattern = re.compile(r'^' + re.escape(label) + r'::?\s*\n((?:[ \t]*\.string "[^"]*"[ \t]*\n?)+)', re.M)
    for path, text in _SOURCES[repo]:
        if label not in text:
            continue
        match = pattern.search(text)
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
    pilot_story.install_constants(g)
    text = flags.read_text(encoding='utf-8')
    added = ''.join(f'#define {alias} {real} // Region item (scripts/install_pilot_events.py)\n'
                    for _item, alias, real in ITEM_FLAGS.values() if f'#define {alias} ' not in text)
    if added:
        text = text.replace(f'#define {FLAG_POTION} FLAG_0x077', added + f'#define {FLAG_POTION} FLAG_0x077', 1)
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

        order = sorted(objects)

        def story_text(label):
            local = text_label(label)
            lines, where, source = texts[local]
            texts[local] = (pilot_story.substitute(lines), where, source)
            return local

        def add_text(local, lines):
            texts[local] = (lines, 'new text (no source; pilot_story.py)', local)

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
            elif kind == 'itemflag':
                item, flag, _real = ITEM_FLAGS[(source_map, index)]
                scripts.append(f'{script}::\n\tfinditem {item}\n\tend\n')
            elif kind == 'gift':  # one item, once; with ask, "no" skips to the last line like the source
                item, before, after, *ask = label
                ask = ask[0] if ask else True
                _item, gift_flag, _real = ITEM_FLAGS[(source_map, index)]
                question = (f'\tmsgbox {text_label(before)}, MSGBOX_YESNO\n\tgoto_if_eq VAR_RESULT, NO, {script}_After\n' if ask
                            else f'\tmsgbox {text_label(before)}, MSGBOX_DEFAULT\n')
                scripts.append(f'{script}::\n\tlock\n\tfaceplayer\n\tgoto_if_set {gift_flag}, {script}_After\n' + question +
                               f'\tgiveitem {item}\n\tgoto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull\n\tsetflag {gift_flag}\n'
                               f'{script}_After::\n\tmsgbox {text_label(after)}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
            elif kind == 'offer':  # a yes/no question with one line for each answer, nothing given
                ask_text, yes_text, no_text = label
                scripts.append(f'{script}::\n\tlock\n\tfaceplayer\n\tmsgbox {text_label(ask_text)}, MSGBOX_YESNO\n'
                               f'\tgoto_if_eq VAR_RESULT, NO, {script}_No\n\tmsgbox {text_label(yes_text)}, MSGBOX_DEFAULT\n\trelease\n\tend\n'
                               f'{script}_No::\n\tmsgbox {text_label(no_text)}, MSGBOX_DEFAULT\n\trelease\n\tend\n')
            elif kind == 'trainer':
                trainer, intro, defeat, post, *not_enough = label
                battle = (f'trainerbattle_double {trainer}, {text_label(intro)}, {text_label(defeat)}, {text_label(not_enough[0])}'
                          if not_enough else f'trainerbattle_single {trainer}, {text_label(intro)}, {text_label(defeat)}')
                scripts.append(f'{script}::\n\t{battle}\n\tmsgbox {text_label(post)}, MSGBOX_AUTOCLOSE\n\tend\n')
            elif kind == 'story':
                story_script = pilot_story.object_script(source_map, index, name, story_text, lambda i: order.index(i) + 1)
                if story_script is None:
                    script = '0x0'
                else:
                    scripts.append(story_script)
                flag = pilot_story.OBJECTS[source_map][index]
            x, y, movement, placed_by = POSITIONS.get((source_map, index), (o['x'], o['y'], o['movement_type'], None))
            movement = MOVEMENTS.get(movement, movement)
            obj_events.append({'type': 'object', 'graphics_id': gfx, 'x': x, 'y': y, 'elevation': ELEVATIONS.get((source_map, index), o['elevation']),
                               'movement_type': movement, 'movement_range_x': o['movement_range_x'],
                               'movement_range_y': o['movement_range_y'],
                               'trainer_type': o['trainer_type'] if kind == 'trainer' else 'TRAINER_TYPE_NONE',
                               'trainer_sight_or_berry_tree_id': o['trainer_sight_or_berry_tree_id'] if kind == 'trainer' else '0',
                               'script': script, 'flag': flag})
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
            elif kind in ('pcsign', 'martsign'):
                script = 'EventScript_PokecenterSign' if kind == 'pcsign' else 'EventScript_PokemartSign'  # FireRed's own
            else:
                scripts.append(f'{script}::\n\tmsgbox {text_label(label)}, MSGBOX_SIGN\n\tend\n')
            bg_events.append({'type': 'sign', 'x': b['x'], 'y': b['y'], 'elevation': b['elevation'],
                              'player_facing_dir': b.get('player_facing_dir', 'BG_EVENT_PLAYER_FACING_ANY'), 'script': script})
            rows.append({'kind': kind, 'source_index': index, 'source_script': b['script'], 'text': label, 'x': b['x'], 'y': b['y']})
        map_header, story_scripts, coords = pilot_story.map_extras(source_map, name, story_text, add_text)
        if source_map in WORLD_MAP_FLAGS:
            # Fly destination: the town is marked visited on entry, like FireRed's towns.
            assert map_header is None, (name, 'merge the world map flag into the story header')
            map_header = [f'{name}_MapScripts::', f'	map_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition', '	.byte 0', '',
                          f'{name}_OnTransition:', f'	setworldmapflag {WORLD_MAP_FLAGS[source_map]}', '	end', '']
        scripts += story_scripts
        map_path = g / f'data/maps/{name}/map.json'
        current = json.loads(map_path.read_text(encoding='utf-8'))
        current['object_events'], current['bg_events'] = obj_events, bg_events
        current['coord_events'] = coords
        map_path.write_text(json.dumps(current, indent=2) + '\n', encoding='utf-8', newline='\n')
        body = [f'@ Region pilot, phase 1 (ambient). Generated by scripts/install_pilot_events.py from {repo_name}.',
                *(map_header or [f'{name}_MapScripts::', '\t.byte 0', ''])]
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
