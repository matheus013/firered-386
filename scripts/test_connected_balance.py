"""Bikers and League blocks with actual persistent HP/EXP/PP and deaths.

Preparation/availability is source modeled, not a walked/captured campaign.
"""
import argparse,hashlib,json
from balance_core import NativeBench,ROOT,OUT
from challenge_planner import Roster,stages,select_team
parser=argparse.ArgumentParser();parser.add_argument('--build-dir',required=True);parser.add_argument('--mode',choices=['normal','hardcore'],required=True);parser.add_argument('--starter',required=True,choices=['BULBASAUR','CHARMANDER','SQUIRTLE']);parser.add_argument('--trials',type=int,default=3);args=parser.parse_args()
mode=0 if args.mode=='normal' else 1;dest=OUT/('connected-'+args.mode+'-'+args.starter.lower());core=NativeBench(args.build_dir,dest,True);steps=stages();rows=[]
variant={'BULBASAUR':'Charmander','CHARMANDER':'Squirtle','SQUIRTLE':'Bulbasaur'}[args.starter]
def enemy(step):return next((v for k,v in step['variants'].items() if k.endswith(variant)),next(iter(step['variants'].values())))
try:
 for block,indices in [('Bikers',list(range(16,20))),('FirstLeague',list(range(22,27))),('Rematch',list(range(30,35)))]:
  roster=Roster(args.starter,mode,700+['BULBASAUR','CHARMANDER','SQUIRTLE'].index(args.starter))
  for step in steps[:indices[0]+1]:roster.advance(step)
  foes=[m for index in indices for m in enemy(steps[index])['team']]
  for preparation in (['baseline','farm'] if mode==0 and block!='Rematch' else ['baseline']):
   team=select_team(roster.candidates(steps[indices[0]],foes,preparation),foes);trials=[]
   for trial in range(args.trials):
    battles=[]
    for num,index in enumerate(indices):
     step=steps[index];result=core.battle(enemy(step)['id'],mode,step['badges'],step['league'],step['post'],team,20000+trial*100+index,carry=num>0,heal=num>0 and block!='Bikers')
     battles.append({'index':index,'title':step['title'],'result':result})
     if result['outcome']!=1:break
    trials.append({'won_block':len(battles)==len(indices) and battles[-1]['result']['outcome']==1,'battles':battles})
   row={'block':block,'mode':mode,'starter':args.starter,'preparation':preparation,'team':team,'trials':trials,'wins':sum(t['won_block'] for t in trials),'attempts':len(trials)};rows.append(row)
   print(args.mode,args.starter,block,preparation,row['wins'],'/',row['attempts'],flush=True)
   (dest/'results.json').write_text(json.dumps({'rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest(),'scope':'blocks, one fixed team; no PC swaps or PP reset; Bikers no healing; League actual Hyper Potion/Full Heal/Revive(Normal)/Candy(Hardcore) effects counted between rooms; capture success/economy/travel assumed','resource_budget_assumed':{'HYPER_POTION':99,'FULL_HEAL':99,'REVIVE':99 if mode==0 else 0,'RARE_CANDY':99 if mode==1 else 0},'rows':rows},indent=2)+'\n')
finally:core.close()
