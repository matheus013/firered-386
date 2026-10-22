"""Follow-up search with obtainable gifts/TMs, party rotation and six Ethers."""
import argparse,copy,hashlib,json
from balance_core import NativeBench,ROOT,OUT
from challenge_planner import stages,damage,stats
from advanced_challenge_planner import AdvancedRoster,compose
parser=argparse.ArgumentParser();parser.add_argument('--build-dir');parser.add_argument('--mode',choices=['normal','hardcore'],default='normal');parser.add_argument('--starter',choices=['BULBASAUR','CHARMANDER','SQUIRTLE'],default='BULBASAUR');parser.add_argument('--trials',type=int,default=3);parser.add_argument('--pickup-prep',action='store_true');args=parser.parse_args()
mode=0 if args.mode=='normal' else 1;dest=OUT/(('pickup-' if args.pickup_prep else 'advanced-')+args.mode+'-'+args.starter.lower());core=NativeBench(args.build_dir,dest,reuse=bool(args.build_dir));steps=stages();rows=[]
variant={'BULBASAUR':'Charmander','CHARMANDER':'Squirtle','SQUIRTLE':'Bulbasaur'}[args.starter]
def enemy(step):return next((v for k,v in step['variants'].items() if k.endswith(variant)),next(iter(step['variants'].values())))
def rotation(current,last,foes):
 lookup={m['species']:m for m in current};scores=[]
 for index,actual in enumerate(last['after']):
  mon=lookup[actual['species']]
  if actual['dead']:scores.append((-1,index));continue
  score=0
  for target in foes[:2]:
   attack=max((damage(mon['species'],actual['level'],move,target['species'],target['level'],mon['iv'],mon['evs'],mon['nature'],target.get('runtime_training')) for j,move in enumerate(mon['moves']) if move!='NONE' and actual['pp'][j]),default=0)
   score+=attack
  score*=max(1,sum(actual['pp']))**.2;scores.append((score,index))
 return [i for score,i in sorted(scores,reverse=True)]
def write():
 (dest/'results.json').write_text(json.dumps({'rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest(),'scope':'follow-up blocks with modeled legal captures/gifts/shops, no natural travel or actual capture/economy validation; no previous losses assumed at block entry','player_policy':'greedy legal moves with Fake Out, speed control, Water Spout threat; between League rooms reorder existing party records only; no PP reset or battle medicine','resource_budget_assumed':{'HYPER_POTION':99,'FULL_HEAL':99,'REVIVE':99 if mode==0 else 0,'RARE_CANDY':99 if mode==1 else 72 if args.pickup_prep else 0,'PP_UP':72 if args.pickup_prep else 0,'ETHER':6,'LEFTOVERS':2,'TM19':1,'MOON_STONE':2},'pickup_note':'Normal Candy/PP Up pre-farmed using native Pickup table; collection/economy not executed in this battle fixture','ether_sources_before_first_league':['Route2','MtMoon_B2F hidden','Route25 hidden','Route9 hidden','UndergroundPath_EastWestTunnel hidden','UndergroundPath_NorthSouthTunnel hidden','CeladonCity','OneIsland_KindleRoad'],'rows':rows},indent=2)+'\n')
try:
 for block,indices in [('FirstLeague',list(range(22,27))),('Rematch',list(range(30,35)))]:
  roster=AdvancedRoster(args.starter,mode,700+['BULBASAUR','CHARMANDER','SQUIRTLE'].index(args.starter))
  for step in steps[:indices[0]+1]:roster.advance(step)
  foes=[mon for i in indices for mon in enemy(steps[i])['team']];pool=roster.candidates(steps[indices[0]],foes)
  for strategy,manual in [('coverage',None),('water_counter',['JOLTEON','LUDICOLO','METAGROSS','LAPRAS','ZAPDOS','SNORLAX'])]:
   team=compose(pool,foes,manual);trials=[]
   if args.pickup_prep:
    for mon in team:mon['pp_max']=True
   for trial in range(args.trials):
    battles=[];remaining=6;remaining_candy=72 if args.pickup_prep and mode==0 else 0;current=team
    for number,index in enumerate(indices):
     step=steps[index];order=rotation(current,battles[-1]['result'],enemy(step)['team']) if number else None
     result=core.battle(enemy(step)['id'],mode,step['badges'],step['league'],step['post'],team,31000+trial*100+index,carry=number>0,heal=number>0,ethers=remaining,order=order,candies=remaining_candy)
     remaining-=result['between_items']['ETHER'];assert remaining>=0
     if mode==0:remaining_candy-=result['between_items']['RARE_CANDY'];assert remaining_candy>=0
     byspecies={m['species']:m for m in current};current=[byspecies[m['species']] for m in result['after']]
     battles.append({'index':index,'title':step['title'],'order':order,'result':result})
     if result['outcome']!=1:break
    trials.append({'won_block':len(battles)==5 and battles[-1]['result']['outcome']==1,'battles':battles})
   row={'block':block,'strategy':strategy,'mode':mode,'starter':args.starter,'team':team,'tm_coins':sum(m.get('tm_coins',0) for m in team),'gift_coins':sum(m['origin'].get('coins',0) for m in team),'trials':trials,'wins':sum(t['won_block'] for t in trials),'attempts':len(trials)};rows.append(row);write()
   print(args.mode,args.starter,block,strategy,row['wins'],'/',row['attempts'],','.join(m['species'] for m in team),flush=True)
finally:write();core.close()
