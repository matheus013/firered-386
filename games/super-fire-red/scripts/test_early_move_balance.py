"""Replay pre-Fuchsia teams without artificial empty move slots."""
from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,copy,json
from balance_core import NativeBench,OUT
from challenge_planner import possible_moves,stages,BANNED
parser=argparse.ArgumentParser();parser.add_argument('--build-dir',required=True);parser.add_argument('--workers',type=int,default=6);parser.add_argument('--trials',type=int,default=3);args=parser.parse_args()
def grade(tests,mode):
 wins=[t for t in tests if t['outcome']==1];n=len(tests)
 if not wins:return 'Não demonstrada'
 faints=sum(t['faints'] for t in wins)/len(wins);turns=sum(t['turns'] for t in wins)/len(wins)
 if len(wins)==n and faints==0 and turns<=18:return 'Baixa nesta bateria'
 if len(wins)>=n*.8 and faints<=(0 if mode else 1):return 'Moderada'
 return 'Alta' if len(wins)>=n*.6 else 'Muito alta / instável'
def fill(mon):
 mon=copy.deepcopy(mon);known=possible_moves({'original':mon['original'],'origin':mon['origin']},mon['level'],mon['evolution']);moves=[m for m in mon['moves'] if m!='NONE']
 for move in sorted(known-set(moves),key=lambda m:(m in BANNED,m)):
  if len(moves)>=4:break
  moves.append(move)
 mon['moves']=(moves+['NONE']*4)[:4];return mon
def run(job):
 mode,starter=job;label='early-'+('normal' if mode==0 else 'hardcore')+'-'+starter.lower();dest=OUT/label;core=NativeBench(args.build_dir,dest,True);rows=[]
 prefix='prepared-normal-' if mode==0 else 'final-hardcore-';data=json.loads((OUT/(prefix+starter.lower())/'challenge-results.json').read_text())
 try:
  for original in data['rows']:
   if not 1<=original['index']<=9:continue
   row=copy.deepcopy(original);step=stages()[row['index']];team=[fill(m) for m in row['team']];n=['BULBASAUR','CHARMANDER','SQUIRTLE'].index(starter)
   attempts=[core.battle(row['trainer'],mode,step['badges'],step['league'],step['post'],team,43000+row['index']*43+n*500+i) for i in range(args.trials)]
   row.update(team=team,baseline_attempts=attempts,farm_attempts=[],wins=sum(t['outcome']==1 for t in attempts),attempts=len(attempts),difficulty=grade(attempts,mode));rows.append(row)
   print(label,row['index']+1,row['wins'],'/',len(attempts),flush=True)
 finally:
  import hashlib
  (dest/'results.json').write_text(json.dumps({'rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest(),'scope':'early teams with all available slots filled; no assumed Move Deleter before Fuchsia; Normal manual EV farm and Hardcore editor; no previous battle losses applied','rows':rows},indent=2)+'\n');core.close()
 return label
with ThreadPoolExecutor(max_workers=args.workers) as pool:
 for future in as_completed([pool.submit(run,(mode,starter)) for starter in ['BULBASAUR','CHARMANDER','SQUIRTLE'] for mode in [0,1]]):print('DONE',future.result(),flush=True)
