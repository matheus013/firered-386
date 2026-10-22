"""Native battle benchmarks with source-checked team construction.

Isolated results do NOT prove a no-warp campaign or a surviving Hardcore run.
"""
from pathlib import Path
import argparse,hashlib,json,time
from balance_core import NativeBench,ROOT,OUT
from challenge_planner import stages,Roster,select_team,DATA,possible_moves

parser=argparse.ArgumentParser();parser.add_argument('--build-dir');parser.add_argument('--last-step',type=int,default=34);parser.add_argument('--trials',type=int,default=5);parser.add_argument('--starter',choices=['all','BULBASAUR','CHARMANDER','SQUIRTLE'],default='all');parser.add_argument('--mode',choices=['both','normal','hardcore'],default='both');parser.add_argument('--chain',action='store_true');parser.add_argument('--label');parser.add_argument('--reuse',action='store_true');parser.add_argument('--resume',action='store_true');parser.add_argument('--farm-all',action='store_true');args=parser.parse_args()
if args.label:OUT=OUT/args.label;OUT.mkdir(parents=True,exist_ok=True)
core=NativeBench(args.build_dir,OUT,args.reuse);steps=stages();results=[]
starters=['BULBASAUR','CHARMANDER','SQUIRTLE'] if args.starter=='all' else [args.starter]
modes=[0,1] if args.mode=='both' else [0 if args.mode=='normal' else 1]
rival={'BULBASAUR':'Charmander','CHARMANDER':'Squirtle','SQUIRTLE':'Bulbasaur'}
path=OUT/('chain-results.json' if args.chain else 'challenge-results.json')
if args.resume and path.exists() and not args.chain:
 previous=json.loads(path.read_text(encoding='utf-8'))
 assert previous['rom_sha256']==core.sha and previous['trials_requested']==args.trials
 assert previous['fixture_sha256']==hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest()
 results=previous['rows']

def write():
 report={'version':'v0.4.0-dev','rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest(),'suite':'coherent challenge sequence, simulated capture roster' if args.chain else 'isolated native battles, source-checked teams','trials_requested':args.trials,'player_policy':'deterministic greedy damage/status selection; reads native enemy stats/abilities; no voluntary switches or battle items; replacement heuristic; not an optimal human policy','preparation':'Normal sampled IVs/nature/ability and EV0 before League, optional 508EV farm retries; Hardcore/postgame IV31 role EV508/nature, original sampled ability; level-up moves retained from catch level/evolution timeline only; no TMs/tutors/egg/stone/friendship/trades/held items; capture success, travel and economy assumed, not executed','hardcore_captures':'one simulated eligible capture per section; seeded weighted land-first table; exact-species duplicates skipped; free gifts per current rules; loss registry only in chain suite','campaign_clear_certified':False,'rows':results}
 path.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

def grade(attempts,mode):
 wins=[a for a in attempts if a['outcome']==1];n=len(attempts)
 if not wins:return 'Não demonstrada'
 deaths=sum(a['faints'] for a in wins)/len(wins);turns=sum(a['turns'] for a in wins)/len(wins)
 if len(wins)==n and deaths==0 and turns<=18:return 'Baixa nesta bateria'
 if len(wins)>=n*.8 and deaths<=(0 if mode else 1):return 'Moderada'
 if len(wins)>=n*.6:return 'Alta'
 return 'Muito alta / instável'

try:
 for mode in modes:
  for startnum,starter in enumerate(starters):
   startnum=['BULBASAUR','CHARMANDER','SQUIRTLE'].index(starter)
   roster=Roster(starter,mode,700+startnum);stopped=False
   for step in steps[:args.last_step+1]:
    roster.advance(step)
    if not args.chain and any(r['index']==step['index'] and r['mode']==mode and r['starter']==starter for r in results):
     print('RESUME kept',step['index']+1,mode,starter,flush=True);continue
    variants=step['variants'];key=next((k for k in variants if k.endswith(rival[starter])),next(iter(variants)))
    enemy=variants[key];seed_base=9000+step['index']*43+startnum*500
    if args.chain and stopped:
     results.append({'index':step['index'],'title':step['title'],'mode':mode,'starter':starter,'trainer':enemy['id'],'status':'not_reached_after_wipe','difficulty':'Não alcançado nesta tentativa'});write();continue
    league_carry=step['league']>0 and (22<=step['index']<=26 or 30<=step['index']<=34)
    biker_carry=17<=step['index']<=19
    carry=args.chain and (league_carry or biker_carry)
    if carry:team=last_sequence_team
    else:team=select_team(roster.candidates(step,enemy['team'],'farm' if args.farm_all and mode==0 and step['index']>0 else 'baseline'),enemy['team'])
    assert team
    attempts=[];best_team=team;prep='manual_ev_farm_508' if args.farm_all and mode==0 and 0<step['index']<27 else 'baseline';start=time.monotonic()
    for trial in range(1 if args.chain else args.trials):
     result=core.battle(enemy['id'],mode,step['badges'],step['league'],step['post'],team,seed_base+trial,carry=carry,heal=carry and league_carry,started=step['index']>0)
     attempts.append(result)
    # Normal can manually earn EVs before the cap; this is a distinct, costly
    # preparation scenario, never mislabeled as scientist access.
    farm_attempts=[]
    if not args.chain and not args.farm_all and mode==0 and step['index']>0 and not step['post'] and not any(a['outcome']==1 for a in attempts):
     farm_team=select_team(roster.candidates(step,enemy['team'],'farm'),enemy['team'])
     for trial in range(args.trials):farm_attempts.append(core.battle(enemy['id'],mode,step['badges'],step['league'],step['post'],farm_team,seed_base+trial,started=True))
     if any(a['outcome']==1 for a in farm_attempts):best_team=farm_team;prep='manual_ev_farm_508'
    selected_attempts=farm_attempts if farm_attempts and prep!='baseline' else attempts
    wins=sum(a['outcome']==1 for a in selected_attempts)
    row={'index':step['index'],'title':step['title'],'mode':mode,'starter':starter,'trainer':enemy['id'],'capture_pool_size':len(roster.caught),'preparation':prep,'team':best_team,'baseline_attempts':attempts,'farm_attempts':farm_attempts,'wins':wins,'attempts':len(selected_attempts),'difficulty':grade(selected_attempts,mode),'seconds':round(time.monotonic()-start,2),'league_isolated_warning':not args.chain and 23<=step['index']<=26}
    results.append(row);write()
    print(f"{step['index']+1:02d} mode={mode} starter={starter} {step['key']}: {wins}/{len(selected_attempts)} {row['difficulty']} prep={prep} team="+','.join(m['species'] for m in best_team),flush=True)
    if args.chain:
     actual=attempts[0]
     for mon,after in zip(team,actual['after']):
      roster.seen_species.add(after['species'])
      if after['dead']:roster.dead.add(mon['uid'])
     if actual['outcome']!=1 and step['index']>0:stopped=True
     if step['index'] in [16,22,30]:last_sequence_team=team
     if actual['outcome']!=1 and (22<=step['index']<=26 or 30<=step['index']<=34):stopped=True
finally:
 write();core.close()
print('Recorded native benchmark:',path,flush=True)
