"""Assisted campaign integration: real field scripts, completed battles, HOF/credits and save reload.

Travel is teleported and an artificial level-100 team is provided. This is NOT a
normal-play clear or a balance/route-accessibility certification.
"""
from pathlib import Path
import argparse,hashlib,json,re,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=Path((root/'game/output/hack-build-path.txt').read_text().strip())
parser=argparse.ArgumentParser();parser.add_argument('--build-dir');parser.add_argument('--resume-save');parser.add_argument('--start-case',type=int,default=0);parser.add_argument('--end-case',type=int);parser.add_argument('--reseed-team',action='store_true');parser.add_argument('--scenario',choices=['campaign','story','rivals'],default='campaign');args=parser.parse_args()
out=root/'game/output/tests'/('campaign' if args.scenario=='campaign' else 'campaign-'+args.scenario);out.mkdir(parents=True,exist_ok=True)
test=Path(args.build_dir) if args.build_dir else Path(tempfile.mkdtemp(prefix='campaign-fixture-',dir=Path.home()/'.cache/firered-386'))
if not args.build_dir:shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
cases=[]
for index,(folder,npc) in enumerate([('PewterCity_Gym','Brock'),('CeruleanCity_Gym','Misty'),('VermilionCity_Gym','LtSurge'),('CeladonCity_Gym','Erika'),('FuchsiaCity_Gym','Koga'),('SaffronCity_Gym','Sabrina'),('CinnabarIsland_Gym','Blaine'),('ViridianCity_Gym','Giovanni')]):
 m=json.loads((root/'game/data/maps'/folder/'map.json').read_text())
 obj=next((i,o) for i,o in enumerate(m['object_events']) if o.get('script','').endswith('_EventScript_'+npc))
 i,o=obj
 cases.append({'name':npc,'map':m['id'],'x':o['x'],'y':o['y']+1,'local_id':i+1,'script':o['script'],'flag':f'FLAG_BADGE0{index+1}_GET'})
for npc in ['Lorelei','Bruno','Agatha','Lance']:
 folder='PokemonLeague_'+npc+'sRoom';m=json.loads((root/'game/data/maps'/folder/'map.json').read_text())
 i,o=next((i,o) for i,o in enumerate(m['object_events']) if o.get('script','').endswith('_EventScript_'+npc))
 cases.append({'name':npc,'map':m['id'],'x':o['x'],'y':o['y']+1,'local_id':i+1,'script':o['script'],'flag':'FLAG_DEFEATED_'+npc.upper()})
cases.append({'name':'Champion-HallOfFame-Credits','map':'MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM','x':6,'y':19,'local_id':1,'script':None,'flag':'FLAG_SYS_GAME_CLEAR'})
cases[-1]['credits']=1

def event(folder,suffix,flag='0',**checks):
 m=json.loads((root/'game/data/maps'/folder/'map.json').read_text())
 script=folder+'_EventScript_'+suffix
 found=next(((i,o) for i,o in enumerate(m['object_events']) if o.get('script')==script),None)
 if found:i,o=found;x,y,local_id=o['x'],o['y']+1,i+1
 else:x,y,local_id=checks.pop('x',6),checks.pop('y',6),checks.pop('local_id',1)
 cases.append({'name':suffix,'map':m['id'],'x':x,'y':y,'local_id':local_id,'script':script,'flag':flag,**checks})

cases.append({'name':'Sevii-introduction','map':'MAP_ONE_ISLAND_POKEMON_CENTER_1F','x':8,'y':9,'local_id':1,'script':None,'flag':'FLAG_SYS_SEVII_MAP_123','item':'ITEM_TRI_PASS','var':'VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F','value':1})
event('ThreeIsland_BerryForest','Lostelle','FLAG_RESCUED_LOSTELLE',supply='ITEM_MASTER_BALL')
event('TwoIsland_JoyfulGameCorner','LostellesDaddy','FLAG_GOT_MOON_STONE_FROM_JOYFUL_GAME_CORNER')
event('OneIsland_PokemonCenter_1F','LeaveOneIslandTriggerBottom',var='VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F',value=3,x=13,y=9,local_id=1)
event('OneIsland_PokemonCenter_1F','Celio',var='VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F',value=4)
event('MtEmber_RubyPath_B5F','Ruby','FLAG_GOT_RUBY',item='ITEM_RUBY')
event('OneIsland_PokemonCenter_1F','Celio','FLAG_SYS_SEVII_MAP_4567',item='ITEM_RAINBOW_PASS')
event('FourIsland_IcefallCave_Back','LoreleiRocketsScene','FLAG_HIDE_RUIN_VALLEY_SCIENTIST',x=7,y=13,local_id=1)
event('SixIsland_DottedHole_SapphireRoom','Sapphire','FLAG_LEARNED_YES_NAH_CHANSEY')
event('FiveIsland_RocketWarehouse','Admin1',trainer='TRAINER_TEAM_ROCKET_ADMIN')
event('FiveIsland_RocketWarehouse','Admin2',trainer='TRAINER_TEAM_ROCKET_ADMIN_2')
event('FiveIsland_RocketWarehouse','Gideon','FLAG_RECOVERED_SAPPHIRE',item='ITEM_SAPPHIRE')
event('OneIsland_PokemonCenter_1F','Celio','FLAG_SYS_CAN_LINK_WITH_RS')
for npc in ['Lorelei','Bruno','Agatha','Lance']:
 event('PokemonLeague_'+npc+'sRoom',npc,'FLAG_DEFEATED_'+npc.upper())
cases.append({'name':'Rematch-Champion-HallOfFame-Credits','map':'MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM','x':6,'y':19,'local_id':1,'script':None,'flag':'FLAG_SYS_GAME_CLEAR','credits':1})
event('CeruleanCave_B1F','Mewtwo','FLAG_FOUGHT_MEWTWO',supply='ITEM_MASTER_BALL')
cases.append({'name':'Selphy-rescue','map':'MAP_FIVE_ISLAND_LOST_CAVE_ROOM10','x':5,'y':7,'local_id':1,'script':None,'flag':'FLAG_HIDE_LOST_CAVE_SELPHY','trainer':'TRAINER_LADY_SELPHY'})
event('FiveIsland_MemorialPillar','Memorial','FLAG_GOT_TM42_AT_MEMORIAL_PILLAR',x=4,y=46,local_id=1,supply='ITEM_LEMONADE')
for switch in range(1,8):
 event('SevenIsland_SevaultCanyon_TanobyKey','Switch'+str(switch),'FLAG_SYS_UNLOCKED_TANOBY_RUINS' if switch==7 else '0',var='VAR_TEMP_'+str(switch),value=100,x=7,y=10,local_id=1,reuseMap=int(switch>1))
for challenge in range(4):
 event('TrainerTower_Lobby','BeginChallenge',towerStart=challenge+1,towerExpected=challenge+1,var='VAR_MAP_SCENE_TRAINER_TOWER',value=1)
 for floor in range(1,9):
  cases.append({'name':f'Tower-{challenge}-floor-{floor}','map':f'MAP_TRAINER_TOWER_{floor}F','x':9,'y':13,'local_id':1,'script':'TrainerTower_EventScript_TriggerBattle','flag':'0','towerFloor':floor,'towerExpected':challenge+1})
 event('TrainerTower_Roof','Owner',x=9,y=8,local_id=1,towerPrize=1,towerExpected=challenge+1)
if args.scenario=='story':
 cases.clear()
 cases.append({'name':'Oak-Parcel-pickup','map':'MAP_VIRIDIAN_CITY_MART','x':5,'y':8,'local_id':1,'script':None,'flag':'0','item':'ITEM_OAKS_PARCEL'})
 event('PalletTown_ProfessorOaksLab','ProfOak','FLAG_EXP_SHARE_UNLOCKED',var='VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB',value=6,item='ITEM_POKE_BALL')
 event('Route25_SeaCottage','Bill','FLAG_TEMP_2',x=10,y=7,local_id=2)
 event('Route25_SeaCottage','Computer','FLAG_HELPED_BILL_IN_SEA_COTTAGE',x=5,y=5,local_id=1,reuseMap=1)
 event('Route25_SeaCottage','Bill','FLAG_GOT_SS_TICKET',item='ITEM_SS_TICKET',local_id=1,reuseMap=1)
 event('SSAnne_CaptainsOffice','Captain','FLAG_GOT_HM01',item='ITEM_HM01')
 event('Route16_House','Woman','FLAG_GOT_HM02',item='ITEM_HM02')
 event('CeladonCity_Condominiums_1F','TeaWoman','FLAG_GOT_TEA',item='ITEM_TEA')
 event('Route7_EastEntrance','GuardTriggerMid',var='VAR_MAP_SCENE_ROUTE5_ROUTE6_ROUTE7_ROUTE8_GATES',value=1,x=9,y=3)
 event('RocketHideout_B4F','Grunt1',trainer='TRAINER_TEAM_ROCKET_GRUNT_18')
 event('RocketHideout_B4F','LiftKey','FLAG_CAN_USE_ROCKET_HIDEOUT_LIFT',item='ITEM_LIFT_KEY')
 event('RocketHideout_B4F','Grunt2',trainer='TRAINER_TEAM_ROCKET_GRUNT_16')
 event('RocketHideout_B4F','Grunt3',trainer='TRAINER_TEAM_ROCKET_GRUNT_17')
 event('RocketHideout_B4F','Giovanni','FLAG_HIDE_CELADON_ROCKETS')
 event('RocketHideout_B4F','SilphScope',item='ITEM_SILPH_SCOPE')
 event('PokemonTower_6F','MarowakGhost',var='VAR_MAP_SCENE_POKEMON_TOWER_6F',value=1,x=10,y=15)
 for n in range(1,4):event('PokemonTower_7F','Grunt'+str(n))
 event('PokemonTower_7F','MrFuji','FLAG_RESCUED_MR_FUJI')
 event('LavenderTown_VolunteerPokemonHouse','MrFuji','FLAG_GOT_POKE_FLUTE',item='ITEM_POKE_FLUTE')
 event('SafariZone_SecretHouse','Attendant','FLAG_GOT_HM03',item='ITEM_HM03')
 def item_case(item):
  text=(root/'game/data/scripts/item_ball_scripts.inc').read_text()
  label=re.search(r'^(\w+)::\n\s*finditem '+item+r'\b',text,re.M)[1]
  for path in (root/'game/data/maps').glob('*/map.json'):
   m=json.loads(path.read_text())
   for i,o in enumerate(m['object_events']):
    if o.get('script')==label:
     cases.append({'name':item,'map':m['id'],'x':o['x'],'y':o['y']+1,'local_id':i+1,'script':label,'flag':'0','item':item});return
  raise AssertionError(item)
 item_case('ITEM_GOLD_TEETH')
 event('FuchsiaCity_WardensHouse','Warden','FLAG_GOT_HM04',item='ITEM_HM04')
 item_case('ITEM_HM07')
 event('OneIsland_KindleRoad_EmberSpa','RockSmashMan','FLAG_GOT_HM06',item='ITEM_HM06')
 item_case('ITEM_CARD_KEY')
 event('SilphCo_11F','BattleGiovanni','FLAG_HIDE_SAFFRON_ROCKETS',x=6,y=9)
 event('SilphCo_11F','President',item='ITEM_MASTER_BALL')
 item_case('ITEM_SECRET_KEY')
if args.scenario=='rivals':
 cases.clear()
 event('SSAnne_2F_Corridor','RivalTriggerMid',var='VAR_MAP_SCENE_S_S_ANNE_2F_CORRIDOR',value=1,x=36,y=5)
 event('SilphCo_7F','RivalTriggerBottom',var='VAR_MAP_SCENE_SILPH_CO_7F',value=1,x=5,y=6)
 event('Route22','LateRivalTriggerMid',var='VAR_MAP_SCENE_ROUTE22',value=4,x=21,y=8)
(out/'cases.json').write_text(json.dumps(cases,indent=2)+'\n')
header='#include "constants/opponents.h"\n#include "constants/items.h"\nstruct CampaignCase {u16 map;u8 x,y,localId;const u8 *script;u16 flag,var,value,item,trainer;u8 credits,towerStart,towerFloor,towerPrize;u16 supply;u8 reuseMap,towerExpected;};\n'
header+=''.join('extern const u8 '+c['script']+'[];\n' for c in cases if c['script'])
header+='static const struct CampaignCase sCampaignCases[]={\n'+''.join('{'+','.join([c['map'],str(c['x']),str(c['y']),str(c['local_id']),c['script'] or 'NULL',c['flag'],*[str(c.get(k,0)) for k in ['var','value','item','trainer','credits','towerStart','towerFloor','towerPrize','supply','reuseMap','towerExpected']]])+'},\n' for c in cases)+'};\n'
(test/'include/campaign_cases.h').write_text(header)
main=(root/'game/src/main.c').read_text().replace('static void InitMainCallbacks(void)\n{',(root/'game/scripts/campaign_fixture.c').read_text()+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(CampaignBoot);').replace('    ReadKeys();','    CampaignTick();\n    ReadKeys();').replace('#include "global.h"','#include "global.h"\nstatic void CampaignTick(void);')
if args.reseed_team:main='#define CAMPAIGN_RESEED_TEAM\n'+main
if args.scenario=='story':main='#define CAMPAIGN_STORY_TEST\n'+main
(test/'src/main.c').write_text(main)
overworld=(root/'game/src/overworld.c').read_text().replace('    NewGameInitData();\n    ResetInitialPlayerAvatarState();','    NewGameInitData();\n    CampaignInit();\n    ResetInitialPlayerAvatarState();').replace('#include "global.h"','#include "global.h"\nextern void CampaignInit(void);')
(test/'src/overworld.c').write_text(overworld+'\nbool8 CampaignIsField(void){return gMain.callback2==CB2_Overworld;}\n')
player=(root/'game/src/battle_controller_player.c').read_text()
marker='static void PlayerHandleGetMonData(void);'
player=player.replace(marker,(root/'game/scripts/campaign_player.c').read_text()+'\n'+marker)
for original,replacement in [('PlayerHandleChooseAction','CampaignChooseAction'),('PlayerHandleChooseMove','CampaignChooseMove'),('PlayerHandleChoosePokemon','CampaignChoosePokemon'),('PlayerHandleChooseItem','CampaignChooseItem')]:
 start=player.index('static void '+original+'(void)\n{');opening=player.index('{',start);depth=1;end=opening+1
 while depth:
  depth+=(player[end]=='{')-(player[end]=='}');end+=1
 player=player[:start]+'static void '+original+'(void)\n{\n    '+replacement+'();\n}'+player[end:]
player=player.replace('#include "constants/sound.h"','#include "constants/sound.h"\n#include "constants/abilities.h"\nstatic void PlayerBufferExecCompleted(void);')
(test/'src/battle_controller_player.c').write_text(player)
credits=(root/'game/src/credits.c').read_text().replace('#include "global.h"','#include "global.h"\nextern void CampaignCreditsFinished(void);').replace('SoftReset(RESET_ALL);','CampaignCreditsFinished();')
(test/'src/credits.c').write_text(credits)
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'campaign-smoke';subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'game/scripts/campaign_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
names=['gCampaignCommand','gCampaignCase','gCampaignReady','gCampaignResult','gCampaignStage','gCampaignCredits','gCampaignLoaded','gMain','gBattleOutcome','gCampaignCaseType','gCampaignAudit']
with (out/'emulator.log').open('w') as log:
 result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'campaign'),str(args.end_case or len(cases)),*['0x'+symbols[s] for s in names],str(args.start_case),args.resume_save or ''],stdout=log,stderr=subprocess.STDOUT,text=True,timeout=3600)
log=(out/'emulator.log').read_text();print(log,flush=True)
passed={int(n) for n in re.findall(r'PASS checkpoint=(\d+) ',log)}
for i,c in enumerate(cases):c['status']='passed' if i in passed else 'not_completed'
report={'rom_sha256':hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest(),'fixture_sha256':hashlib.sha256((test/'pokefirered.gba').read_bytes()).hexdigest(),'scenario':args.scenario,'scope':'Normal mode assisted integration, teleported travel, directly invoked native scripts and supplied artificial level-100 teams; not a normal-play or Hardcore campaign clear','exit_code':result.returncode,'log':str((out/'emulator.log').relative_to(root)),'cases':cases}
(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
assert result.returncode==0,(result.returncode,log[-3000:])
