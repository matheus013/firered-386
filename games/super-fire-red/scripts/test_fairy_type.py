"""Native Fairy/type-chart checks through the engine's own type calculation, plus summary screenshots."""
from pathlib import Path
import json,hashlib,shutil,subprocess,tempfile
ROOT=Path(__file__).resolve().parents[3];OUT=ROOT/'games/super-fire-red/output/tests/fairy-type';OUT.mkdir(parents=True,exist_ok=True)
source=Path((ROOT/'games/super-fire-red/output/hack-build-path.txt').read_text().strip());fixture=Path(tempfile.mkdtemp(prefix='fairy-type-',dir=source.parent));shutil.copytree(source,fixture,dirs_exist_ok=True)
main=(source/'src/main.c').read_text()
# (move, target species, expected damage multiplier x100)
CHECKS=[('DRAGON_CLAW','CLEFAIRY',0),('SLUDGE_BOMB','CLEFAIRY',200),('IRON_TAIL','CLEFAIRY',200),('BRICK_BREAK','CLEFAIRY',50),
        ('MEGAHORN','CLEFAIRY',50),('BITE','GARDEVOIR',100),('DRAGON_CLAW','AZUMARILL',0),('SHADOW_BALL','STEELIX',100),
        ('CRUNCH','STEELIX',100),('FLAMETHROWER','MAWILE',200),('EARTHQUAKE','MAWILE',200),('SLUDGE_BOMB','MAWILE',0)]
calls='\n'.join(f' gBattleMoveDamage=100;flags=AI_TypeCalc(MOVE_{m},SPECIES_{s},0);Check(!(flags&MOVE_RESULT_DOESNT_AFFECT_FOE)?gBattleMoveDamage=={e}:{e}==0);' for m,s,e in CHECKS)
hook='''
#include "new_game.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "game_settings.h"
#include "string_util.h"
#include "battle.h"
#include "battle_main.h"
#include "constants/moves.h"
#include "constants/battle.h"
extern u8 AI_TypeCalc(u16 move, u16 targetSpecies, u8 targetAbility);
EWRAM_DATA volatile u32 gFairyDone=0,gFairyPassed=0,gFairyCount=0;
static void Check(bool32 ok){if(ok)gFairyPassed|=1u<<gFairyCount;gFairyCount++;}
static void UiIdle(void){}
static void UiBoot(void)
{
 static const u8 name[] = _("TEST");
 u8 flags;
 SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();GameSettings_SetLanguage(TEST_LANGUAGE);InitHeap(gHeap,HEAP_SIZE);
CALLS
 Check(gSpeciesInfo[SPECIES_CLEFAIRY].types[0]==TYPE_FAIRY&&gSpeciesInfo[SPECIES_CLEFAIRY].types[1]==TYPE_FAIRY);
 Check(gSpeciesInfo[SPECIES_MAWILE].types[0]==TYPE_STEEL&&gSpeciesInfo[SPECIES_MAWILE].types[1]==TYPE_FAIRY);
 Check(gSpeciesInfo[SPECIES_PIKACHU].baseSpeed==90&&gSpeciesInfo[SPECIES_PIKACHU].baseDefense==40);
 Check(gBattleMoves[MOVE_CHARM].type==TYPE_FAIRY&&gBattleMoves[MOVE_CURSE].type==TYPE_GHOST);
 gFairyDone=1;
 CreateMon(&gPlayerParty[0],SPECIES_CLEFAIRY,50,31,FALSE,0,OT_ID_PLAYER_ID,0);
 SetMonMoveSlot(&gPlayerParty[0],MOVE_CHARM,0);SetMonMoveSlot(&gPlayerParty[0],MOVE_SWEET_KISS,1);
 SetMonMoveSlot(&gPlayerParty[0],MOVE_MOONLIGHT,2);SetMonMoveSlot(&gPlayerParty[0],MOVE_METRONOME,3);
 CalculatePlayerPartyCount();ShowPokemonSummaryScreen(gPlayerParty,0,0,UiIdle,PSS_MODE_NORMAL);
}
'''.replace('CALLS',calls)
runner=fixture/'fairy-type';subprocess.run(['gcc','-O2','-w','-I',str(ROOT/'games/super-fire-red/scripts'),str(ROOT/'games/super-fire-red/scripts/fairy_type_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
results={}
for language in [0,1]:
 text=main.replace('static void InitMainCallbacks(void)\n{',hook.replace('TEST_LANGUAGE',str(language))+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(UiBoot);')
 (fixture/'src/main.c').write_text(text)
 with (OUT/f'build-{language}.log').open('w') as log:subprocess.run(['make','-j4'],cwd=fixture,stdout=log,stderr=subprocess.STDOUT,check=True)
 symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=fixture,text=True).splitlines() if len(p:=line.split())==3}
 prefix=str(OUT/('en' if language==0 else 'ptbr'))
 run=subprocess.run([str(runner),str(fixture/'pokefirered.gba'),prefix,*['0x'+symbols[s] for s in ('gFairyDone','gFairyPassed','gFairyCount')]],capture_output=True,text=True,timeout=90)
 (OUT/f'emulator-{language}.log').write_text(run.stdout+run.stderr)
 results['en' if language==0 else 'ptbr']=json.loads(run.stdout.splitlines()[0]) if run.stdout else None
 assert run.returncode==0,(run.stdout,run.stderr[-1500:])
 print('PASS fairy/type checks language',language,results['en' if language==0 else 'ptbr'],flush=True)
(OUT/'results.json').write_text(json.dumps({'rom_sha256':hashlib.sha256((ROOT/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest(),'checks':[{'move':m,'target':s,'multiplier_x100':e} for m,s,e in CHECKS]+['species types Clefairy/Mawile','Pikachu gen9 stats','Charm Fairy / Curse Ghost'],'results':results,'scope':'engine type calculation and summary screenshots; screenshots require visual inspection'},indent=2)+'\n')
