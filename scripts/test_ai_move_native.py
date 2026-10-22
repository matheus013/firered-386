"""Run actual AI/damage routines in an initialized native battle, disposable copy."""
from pathlib import Path
import shutil,subprocess,tempfile,json,re,hashlib
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'output/tests/ai-modern';OUT.mkdir(parents=True,exist_ok=True)
source=Path((ROOT/'output/hack-build-path.txt').read_text().strip())
fixture=Path(tempfile.mkdtemp(prefix='ai-modern-',dir=source.parent));shutil.copytree(source,fixture,dirs_exist_ok=True)
(OUT/'build-path.txt').write_text(str(fixture)+'\n')
main=(fixture/'src/main.c').read_text()
hook='''
#include "new_game.h"
#include "pokemon.h"
#include "battle.h"
#include "overworld.h"
#include "string_util.h"
#include "constants/maps.h"
#include "constants/opponents.h"
extern void CB2_InitBattle(void);
extern void HandleTurnActionSelectionState(void);
extern void RunAIDecisionChecks(void);
extern volatile u32 gDecisionDone;
static void CheckIdle(void){}
static void CheckBoot(void)
{
 static const u8 name[] = _("TEST");
 SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();
 gSaveBlock2Ptr->optionsTextSpeed=2;gSaveBlock2Ptr->optionsBattleSceneOff=TRUE;
 gMapHeader=*Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1));
 CreateMon(&gPlayerParty[0],SPECIES_BULBASAUR,18,31,FALSE,0,OT_ID_PLAYER_ID,0);
 CreateMon(&gPlayerParty[1],SPECIES_SQUIRTLE,18,31,FALSE,0,OT_ID_PLAYER_ID,0);CalculatePlayerPartyCount();
 gTrainerBattleOpponent_A=TRAINER_LEADER_BROCK;gBattleTypeFlags=BATTLE_TYPE_TRAINER;
 gMain.savedCallback=CheckIdle;SetMainCallback2(CB2_InitBattle);
}
'''
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(CheckBoot);')
main=main.replace('#include "global.h"','#include "global.h"\n#include "battle.h"\nextern void HandleTurnActionSelectionState(void);\nextern void RunAIDecisionChecks(void);\nextern volatile u32 gDecisionDone;',1)
main=main.replace('    ReadKeys();','    if (!gDecisionDone && gMain.inBattle && gBattleMainFunc==HandleTurnActionSelectionState) RunAIDecisionChecks();\n    ReadKeys();')
(fixture/'src/main.c').write_text(main)
checks=['extern void BenchCreateGymParty(u16 trainer);','static void BenchGymPartyChecks(void) {']
teams=json.loads((ROOT/'data/trainer_balance.json').read_text())['teams']
for name,config in teams.items():
 if not config.get('curated_gym'):continue
 checks += ['gBattleTypeFlags=BATTLE_TYPE_TRAINER|BATTLE_TYPE_DOUBLE;',f'BenchCreateGymParty({config["id"]});']
 for slot,mon in enumerate(config['team']):
  base=f'&gEnemyParty[{slot}]'
  for field,value in [('SPECIES','SPECIES_'+mon['species']),('LEVEL',str(mon['level'])),('HELD_ITEM','ITEM_'+mon['item']),('ABILITY_NUM',str(mon['ability_num']))]:
   checks.append(f'BenchExpect(GetMonData({base},MON_DATA_{field})=={value});')
  checks.append(f'BenchExpect(GetNature({base})==NATURE_{mon["nature"]});')
  for stat in range(6):
   checks += [f'BenchExpect(GetMonData({base},MON_DATA_HP_IV+{stat})==31);',f'BenchExpect(GetMonData({base},MON_DATA_HP_EV+{stat})=={mon["evs"][stat]});']
  for position,move in enumerate(mon['moves']):checks.append(f'BenchExpect(GetMonData({base},MON_DATA_MOVE1+{position})==MOVE_{move});')
checks.append('}')
p=fixture/'src/battle_ai_script_commands.c';p.write_text(p.read_text()+'\n#include "constants/opponents.h"\n#include "constants/items.h"\n'+(ROOT/'scripts/ai_move_native_checks.c').read_text()+'\n'+'\n'.join(checks)+'\n')
p=fixture/'src/battle_main.c';p.write_text(p.read_text().replace('static void HandleTurnActionSelectionState(void)','void HandleTurnActionSelectionState(void)')+'\nvoid BenchCreateGymParty(u16 trainer){CreateNPCTrainerParty(gEnemyParty,trainer);}\n')
with (OUT/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=fixture,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={parts[2]:parts[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=fixture,text=True).splitlines() if len(parts:=line.split())==3}
runner=fixture/'ai-checks';subprocess.run(['gcc','-O2','-w',str(ROOT/'scripts/ai_checks_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
command=[str(runner),str(fixture/'pokefirered.gba')]+['0x'+symbols[s] for s in ['gDecisionDone','gDecisionChecks','gDecisionFailures','gDecisionFailureBits']]
result=subprocess.run(command,capture_output=True,text=True,timeout=90);(OUT/'emulator.log').write_text(result.stdout+result.stderr)
match=re.search(r'done=(\d+) checks=(\d+) failures=(\d+) bits=(\d+) frames=(\d+)',result.stdout);assert match,result.stdout[-1000:]
record=dict(zip(['done','checks','failures','failure_bits','frames'],map(int,match.groups())));record['rom_sha256']=hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()
(OUT/'results.json').write_text(json.dumps(record,indent=2)+'\n');print(record,flush=True)
assert result.returncode==0 and record['checks']>=35
