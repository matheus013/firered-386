"""Button-driven native summary move-category screenshots in both languages."""
from pathlib import Path
import argparse,json,hashlib,re,shutil,subprocess,tempfile
parser=argparse.ArgumentParser();parser.add_argument('--expanded',action='store_true');args=parser.parse_args()
ROOT=Path(__file__).resolve().parents[3];OUT=ROOT/('games/super-fire-red/output/tests/categories-ui-expanded' if args.expanded else 'games/super-fire-red/output/tests/categories-ui');OUT.mkdir(parents=True,exist_ok=True)
reference_sha=hashlib.sha256((ROOT/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()
source=Path((ROOT/'games/super-fire-red/output/hack-build-path.txt').read_text().strip());fixture=Path(tempfile.mkdtemp(prefix='category-ui-',dir=source.parent));shutil.copytree(source,fixture,dirs_exist_ok=True)
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
main=(source/'src/main.c').read_text();summary=(source/'src/pokemon_summary_screen.c').read_text()
summary=summary.replace('static void PokeSum_PrintSelectedMoveStats(void)\n{','IWRAM_DATA volatile u32 gCategoryObserved;\nstatic void PokeSum_PrintSelectedMoveStats(void)\n{')
summary=summary.replace('    if (sMoveSelectionCursorPos < 5)\n    {','    gCategoryObserved = 1 + 10*sMoveSelectionCursorPos + gBattleMoves[sMonSummaryScreen->moveIds[sMoveSelectionCursorPos]].category;\n    if (sMoveSelectionCursorPos < 5)\n    {',1)
summary=summary.replace('static void CB2_SetUpPSS(void)\n{','IWRAM_DATA volatile u32 gSummarySetupObserved;\nstatic void CB2_SetUpPSS(void)\n{\n    gSummarySetupObserved = 1 + sMonSummaryScreen->summarySetupStep;')
audit='''
IWRAM_DATA volatile u32 gDescriptionOverflow;
EWRAM_DATA volatile u16 gDescriptionTooLong[128];
static bool8 sDescriptionAudited;
'''
summary=summary.replace('IWRAM_DATA volatile u32 gCategoryObserved;',audit+'\nIWRAM_DATA volatile u32 gCategoryObserved;')
summary=summary.replace('    gCategoryObserved = 1 +', '''    if (!sDescriptionAudited)
    {
        u16 move;u8 language,saved=GameSettings_GetLanguage();
        sDescriptionAudited=TRUE;
        for(language=0;language<2;language++)
        {
            GameSettings_SetLanguage(language);
            for(move=1;move<MOVES_COUNT;move++)
                if(PokeSum_FormatMoveDescription(move,FALSE)>5)
                {
                    if(gDescriptionOverflow<128)gDescriptionTooLong[gDescriptionOverflow]=move+language*1000;
                    gDescriptionOverflow++;
                }
        }
        GameSettings_SetLanguage(saved);
    }
    gCategoryObserved = 1 +''')
summary=summary.replace('#include "global.h"','#include "global.h"\n#include "game_settings.h"',1)
(fixture/'src/pokemon_summary_screen.c').write_text(summary)
hook='''
#include "new_game.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "game_settings.h"
#include "string_util.h"
#include "constants/moves.h"
static void UiIdle(void){}
static void UiBoot(void)
{
 static const u8 name[] = _("TEST");
 // NewGameInitData copies the rival name first; zeroed EWRAM has no EOS and would overrun the stack.
 SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();GameSettings_SetLanguage(TEST_LANGUAGE);InitHeap(gHeap,HEAP_SIZE);
 CreateMon(&gPlayerParty[0],SPECIES_SNORLAX,50,31,FALSE,0,OT_ID_PLAYER_ID,0);
 SetMonMoveSlot(&gPlayerParty[0],MOVE_BITE,0);SetMonMoveSlot(&gPlayerParty[0],MOVE_SHADOW_BALL,1);
 SetMonMoveSlot(&gPlayerParty[0],MOVE_GROWL,2);SetMonMoveSlot(&gPlayerParty[0],MOVE_FIRE_PUNCH,3);
 CalculatePlayerPartyCount();ShowSelectMovePokemonSummaryScreen(gPlayerParty,0,0,UiIdle,MOVE_WATER_PULSE);
}
'''
if args.expanded:
 for old,new in [('SPECIES_SNORLAX','SPECIES_CLEFABLE'),('MOVE_BITE','MOVE_PLAY_ROUGH'),('MOVE_SHADOW_BALL','MOVE_DAZZLING_GLEAM'),('MOVE_GROWL','MOVE_GRASSY_TERRAIN'),('MOVE_FIRE_PUNCH','MOVE_SUPERCELL_SLAM'),('MOVE_WATER_PULSE','MOVE_HEALING_WISH')]:hook=hook.replace(old,new)
runner=fixture/'category-ui';subprocess.run(['gcc','-O2','-w',*(['-DEXPANDED_MOVES'] if args.expanded else []),str(ROOT/'games/super-fire-red/scripts/category_ui_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
for language in [0,1]:
 text=main.replace('static void InitMainCallbacks(void)\n{',hook.replace('TEST_LANGUAGE',str(language))+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(UiBoot);')
 (fixture/'src/main.c').write_text(text)
 with (OUT/f'build-{language}.log').open('w') as log:subprocess.run(['make','-j4'],cwd=fixture,stdout=log,stderr=subprocess.STDOUT,check=True)
 symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=fixture,text=True).splitlines() if len(p:=line.split())==3}
 with (OUT/f'emulator-{language}.log').open('w') as log:subprocess.run([str(runner),str(fixture/'pokefirered.gba'),str(OUT/('en' if language==0 else 'ptbr')),*['0x'+symbols[s] for s in ['gCategoryObserved','gSummarySetupObserved','gDescriptionOverflow','gDescriptionTooLong']]],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
 print('PASS category UI language',language,flush=True)
assert hashlib.sha256((ROOT/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha
move_count=int(re.search(r'#define MOVES_COUNT (\d+)',(ROOT/'games/super-fire-red/include/constants/moves.h').read_text())[1])-1
(OUT/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'languages':['EN','PTBR'],'slots_checked':10,'descriptions_checked':move_count*2,'expanded_moves':args.expanded,'scope':'native move selection summary UI; screenshots require visual inspection'},indent=2)+'\n')
