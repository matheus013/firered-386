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
    'OBJ_EVENT_GFX_FIREBREATHER': 'OBJ_EVENT_GFX_BIKER', 'OBJ_EVENT_GFX_OLD_WOMAN': 'OBJ_EVENT_GFX_OLD_WOMAN',
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
                   'AzaleaTown': 'FLAG_WORLD_MAP_AZALEA_TOWN'}
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
              # one-time gifts (kind 'gift'): the flag marks the item as given
              ('CherrygroveCity', 1): ('ITEM_MYSTIC_WATER', 'FLAG_GOT_JOHTO_MYSTIC_WATER', 'FLAG_0x0B6')}
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
            obj_events.append({'type': 'object', 'graphics_id': gfx, 'x': x, 'y': y, 'elevation': o['elevation'],
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
