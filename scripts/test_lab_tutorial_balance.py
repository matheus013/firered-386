"""Correct first-battle flags and original Oak controller; real button input."""
import argparse,hashlib,json,subprocess
from balance_core import NativeBench,ROOT,OUT
from challenge_planner import Roster,stages,DATA
parser=argparse.ArgumentParser();parser.add_argument('--build-dir');parser.add_argument('--hardcore-only',action='store_true');args=parser.parse_args()
dest=OUT/'lab-tutorial';prepare=NativeBench(args.build_dir,output_dir=dest);build=prepare.path;prepare.close()
source=__import__('pathlib').Path((ROOT/'output/hack-build-path.txt').read_text().strip())
main=(build/'src/main.c').read_text()
main=main.replace('#include "global.h"','#include "global.h"\nextern void BenchSetLabFlags(void);\nextern u32 BenchLabMenuState(void);\nextern volatile u32 gBenchOutput[192];',1)
main=main.replace('gBenchReady=0;gBattleTypeFlags=BATTLE_TYPE_TRAINER;','gBenchReady=0;BenchSetLabFlags();gBattleTypeFlags=BATTLE_TYPE_TRAINER|BATTLE_TYPE_FIRST_BATTLE;')
main=main.replace('    ReadKeys();','    gBenchOutput[11]=gMain.inBattle?BenchLabMenuState():0;\n    ReadKeys();')
(build/'src/main.c').write_text(main)
(build/'src/battle_setup.c').write_text((source/'src/battle_setup.c').read_text()+'\nvoid BenchSetLabFlags(void){sRivalBattleFlags=RIVAL_BATTLE_TUTORIAL;}\n')
(build/'src/battle_controller_oak_old_man.c').write_text((source/'src/battle_controller_oak_old_man.c').read_text()+'''
extern volatile u32 gBenchInput[112];
extern u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT];
u32 BenchLabMenuState(void)
{
    if(gBattlerControllerFuncs[0]==OakOldManHandleInputChooseMove)
    {
        u8 desired=gBattleResults.battleTurnCounter<gBenchInput[12]?1:0;
        if(gMoveSelectionCursor[0]!=desired)return desired?6:7;
        return 0;
    }
    if(gBattlerControllerFuncs[0]!=HandleInputChooseAction)return 0;
    if(gBenchInput[1]==0 && gBattleMons[0].hp*2<=gBattleMons[0].maxHP && CheckBagHasItem(ITEM_POTION,1))return 3;
    if(gActionSelectionCursor[0]&1)return 4;
    return 1;
}
''')
with (dest/'tutorial-build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=build,stdout=log,stderr=subprocess.STDOUT,check=True)
core=NativeBench(build,dest,True);rows=[r for r in json.loads((dest/'results.json').read_text())['rows'] if r['mode']==0] if args.hardcore_only else [];step=stages()[0];rival={'BULBASAUR':'Charmander','CHARMANDER':'Squirtle','SQUIRTLE':'Bulbasaur'}
try:
 for mode in ([1] if args.hardcore_only else [0,1]):
  for number,starter in enumerate(rival):
   # The scientist is inaccessible before the first rival, even in Hardcore.
   roster=Roster(starter,0,700+number);target=step['variants']['sParty_RivalOaksLab'+rival[starter]];team=roster.candidates(step,target['team'])
   initial=[m for level,m in DATA.learn[starter] if level<=5][-4:];team[0]['moves']=(initial+['NONE']*4)[:4]
   for status_turns in [0,2]:
    attempts=[core.battle(target['id'],mode,0,0,False,team,9000+number*500+i,started=False,items=1 if mode==0 else 0,menu_policy=True,status_turns=status_turns) for i in range(5)]
    row={'mode':mode,'starter':starter,'trainer':target['id'],'team':team,'status_turns':status_turns,'attempts':attempts,'wins':sum(a['outcome']==1 for a in attempts)};rows.append(row)
    print('tutorial',mode,starter,'status',status_turns,row['wins'],'/5 healing',[a['battle_healing_items'] for a in attempts],flush=True)
finally:
 (dest/'results.json').write_text(json.dumps({'rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((build/'pokefirered.gba').read_bytes()).hexdigest(),'scope':'BATTLE_TYPE_FIRST_BATTLE and RIVAL_BATTLE_TUTORIAL; unmodified Oak/opponent controllers; original accuracy/crit/tutorial messages; native buttons, Normal one PC Potion, Hardcore no battle items','rows':rows},indent=2)+'\n');core.close()
