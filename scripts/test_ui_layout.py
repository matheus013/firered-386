"""Native font budgets and real battle/TM screens, with disposable test ROMs.

Fixtures provide Pokemon/items; this does not validate campaign progression.
"""
from pathlib import Path
import hashlib, json, re, shutil, subprocess, tempfile

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'output/tests/ui-layout'
OUT.mkdir(parents=True, exist_ok=True)
source = Path((ROOT / 'output/hack-build-path.txt').read_text().strip())
sha = hashlib.sha256((ROOT / 'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source / 'pokefirered.gba').read_bytes()).hexdigest() == sha
fixture = Path(tempfile.mkdtemp(prefix='ui-layout-', dir=source.parent))
shutil.copytree(source, fixture, dirs_exist_ok=True)
hook = r'''
#include "new_game.h"
#include "malloc.h"
#include "pokemon.h"
#include "data.h"
#include "battle.h"
#include "battle_main.h"
#include "game_settings.h"
#include "gflib.h"
#include "string_util.h"
#include "strings.h"
#include "item.h"
#include "tm_case.h"
#include "overworld.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
extern void CB2_InitBattle(void);
extern u8 TMCase_FormatDescription(const u8 *text,bool8 draw);
extern void TrainingUiAudit(void);
EWRAM_DATA volatile u32 gLayoutCase = 0, gLayoutDone = 0, gLayoutCount = 0;
EWRAM_DATA u16 gLayoutRows[64][5] = {0};
EWRAM_DATA u32 gLayoutOverflow = 0;
void LayoutMeasure(u16 kind,u16 id,s32 width,u16 budget)
{
    u16 *row;
    gLayoutCount++;
    if(width<=budget)return;
    if(gLayoutOverflow>=64)return;
    row=gLayoutRows[gLayoutOverflow++];
    row[0]=kind;row[1]=id;row[2]=width;row[3]=budget;row[4]=GameSettings_GetLanguage();
}
static void LayoutIdle(void) {}
static void LayoutBoot(void)
{
    static const u8 name[] = _("TEST");
    static const u8 *const controls[] = {gText_PokeSum_Controls_PageCancel,gText_PokeSum_Controls_Cancel,
        gText_PokeSum_Controls_Page,gText_PokeSum_Controls_PageDetail,gText_PokeSum_Controls_PickSwitch,
        gText_PokeSum_Controls_Pick,gText_PokeSum_Controls_PickDelete};
    u16 move,i;u8 lang,buffer[80],*end;
    if (!gLayoutCase) return;
    SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);
    StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();InitHeap(gHeap,HEAP_SIZE);
    for(lang=0;gLayoutCase==1 && lang<2;lang++)
    {
        GameSettings_SetLanguage(lang);
        for(move=1;move<MOVES_COUNT;move++)
        {
            LayoutMeasure(0,move,GetStringWidth(FONT_SMALL,gMoveNames[move],0),64);
            LayoutMeasure(1,move,GetStringWidth(FONT_NORMAL,gMoveNames[move],0),77);
            end=StringCopy(buffer,GetMoveCategoryShortName(move));*end++=CHAR_SLASH;
            StringCopy(end,gTypeNames[gBattleMoves[move].type]);
            LayoutMeasure(2,move,GetStringWidth(FONT_SMALL,buffer,0),64);
            LayoutMeasure(3,move,GetStringWidth(FONT_SMALL,GetMoveCategoryName(move),0),56);
        }
        for(i=0;i<ARRAY_COUNT(controls);i++)
            LayoutMeasure(4,i,GetStringWidth(FONT_SMALL,controls[i],0),84);
        for(i=ITEM_TM01;i<=ITEM_HM08;i++)
            LayoutMeasure(5,i,TMCase_FormatDescription(ItemId_GetDescription(i),FALSE),5);
        LayoutMeasure(6,0,GetStringWidth(FONT_SMALL,gText_TMCase,0),80);
        LayoutMeasure(6,1,GetStringWidth(FONT_SMALL,gText_PokeSum_PageName_PokemonInfo,0),104);
        LayoutMeasure(6,2,GetStringWidth(FONT_SMALL,gText_PokeSum_PageName_PokemonSkills,0),104);
        LayoutMeasure(6,3,GetStringWidth(FONT_SMALL,gText_PokeSum_PageName_KnownMoves,0),104);
        TrainingUiAudit();
    }
    GameSettings_SetLanguage((gLayoutCase-1)%2);gLayoutDone=1;
    CreateMon(&gPlayerParty[0],SPECIES_CLEFABLE,50,31,FALSE,0,OT_ID_PLAYER_ID,0);
    SetMonMoveSlot(&gPlayerParty[0],MOVE_DAZZLING_GLEAM,0);
    SetMonMoveSlot(&gPlayerParty[0],MOVE_GRASSY_TERRAIN,1);
    SetMonMoveSlot(&gPlayerParty[0],MOVE_TRICK_ROOM,2);
    SetMonMoveSlot(&gPlayerParty[0],MOVE_HEALING_WISH,3);
    CalculatePlayerPartyCount();
    if(gLayoutCase<=3)
    {
        gSaveBlock2Ptr->optionsTextSpeed=2;
        gMapHeader=*Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1));
        CreateMon(&gEnemyParty[0],SPECIES_SNORLAX,50,31,FALSE,0,OT_ID_PLAYER_ID,0);
        gBattleTypeFlags=0;gMain.savedCallback=LayoutIdle;SetMainCallback2(CB2_InitBattle);
    }
    else
    {
        for(i=ITEM_TM01;i<=ITEM_TM50;i++)AddBagItem(i,1);
        ResetTMCaseCursorPos();InitTMCase(TMCASE_FIELD,LayoutIdle,FALSE);
    }
}
'''
main = (source / 'src/main.c').read_text()
main = main.replace('static void InitMainCallbacks(void)\n{', hook + '\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(LayoutBoot);')
(fixture / 'src/main.c').write_text(main)
tm=(source/'src/tm_case.c').read_text().replace('static u8 TMCase_FormatDescription(', 'u8 TMCase_FormatDescription(')
(fixture/'src/tm_case.c').write_text(tm)
player=(source/'src/battle_controller_player.c').read_text().replace('HandleInputChooseAction','LayoutHandleInputChooseAction')
(fixture/'src/battle_controller_player.c').write_text(player)
training=(source/'src/training_screen.c').read_text()+r'''
extern void LayoutMeasure(u16 kind,u16 id,s32 width,u16 budget);
void TrainingUiAudit(void)
{
    u8 i;u16 budget;
    for(i=0;i<ARRAY_COUNT(sUi);i++)
    {
        budget=220;
        if(i==UI_FREE || i==UI_PREVIEW)budget=68;
        if(i>=UI_HP)budget=55;
        if(i==UI_NATURE || i==UI_EVS || i==UI_IVS || i==UI_BACK || i==UI_OLDNEW || i==UI_CONFIRM)budget=120;
        LayoutMeasure(7,i,GetStringWidth(FONT_SMALL,T(i),0),budget);
    }
    LayoutMeasure(7,29,GetStringWidth(FONT_SMALL,GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR?sAdvancedLockedPt:sAdvancedLockedEn,0),220);
}
'''
(fixture/'src/training_screen.c').write_text(training)
with (OUT / 'build.log').open('w') as log:
    subprocess.run(['make','-j4'],cwd=fixture,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols = {p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=fixture,text=True).splitlines() if len(p:=line.split())==3}
runner=fixture/'ui-layout'
subprocess.run(['gcc','-O2','-w',str(ROOT/'scripts/ui_layout_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
run=subprocess.run([str(runner),str(fixture/'pokefirered.gba'),str(OUT/'screen'),*['0x'+symbols[s] for s in ['gLayoutCase','gLayoutDone','gLayoutCount','gLayoutRows','gLayoutOverflow','gBattlerControllerFuncs','LayoutHandleInputChooseAction','HandleInputChooseMove','gMoveSelectionCursor']]],capture_output=True,text=True,timeout=120)
(OUT/'emulator.log').write_text(run.stdout+run.stderr)
assert run.returncode==0,run.stderr[-2000:]
failures=[list(map(int,m)) for m in re.findall(r'overflow=(\d+),(\d+),(\d+),(\d+),(\d+)',run.stdout)]
assert hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()==sha
report={'rom_sha256':sha,'status':'failed' if failures else 'passed','measured_fields':int(re.search(r'measured=(\d+)',run.stdout)[1]),'failures':failures,'failure_columns':['kind','move_or_control','width_or_lines','budget','language'],'kinds':['battle_move_name','summary_move_name','battle_category_type','summary_category','summary_controls','tm_description_lines','titles','training_labels'],'scope':'native fonts, all move names/categories/types, summary controls, training labels and 58 TM/HM descriptions; real battle/TM screenshots require inspection'}
(OUT/'results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
assert not failures,'UI text exceeds its pixel budget'
