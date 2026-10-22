from concurrent.futures import ThreadPoolExecutor,as_completed
from pathlib import Path
import argparse,json,subprocess
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'output/tests/balance'
parser=argparse.ArgumentParser();parser.add_argument('--build-dir',required=True);parser.add_argument('--workers',type=int,default=3);parser.add_argument('--trials',type=int,default=3);parser.add_argument('--prepared-only',action='store_true');args=parser.parse_args()
jobs=[]
for starter in ['BULBASAUR','CHARMANDER','SQUIRTLE']:
 if args.prepared_only:
  label='prepared-normal-'+starter.lower();command=['python3',str(ROOT/'scripts/test_challenge_balance.py'),'--build-dir',args.build_dir,'--reuse','--trials',str(args.trials),'--mode','normal','--starter',starter,'--label',label,'--farm-all','--resume']
  jobs.append((label,command))
 else:
  for mode in ['normal','hardcore']:
   label='connected-'+mode+'-'+starter.lower();jobs.append((label,['python3',str(ROOT/'scripts/test_connected_balance.py'),'--build-dir',args.build_dir,'--mode',mode,'--starter',starter,'--trials',str(args.trials)]))
def run(job):
 label,command=job
 with (OUT/(label+'.log')).open('w') as log:
  p=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,bufsize=1)
  for line in p.stdout:log.write(line);log.flush();print(label+': '+line.rstrip(),flush=True)
  return {'label':label,'exit_code':p.wait()}
with ThreadPoolExecutor(max_workers=args.workers) as pool:results=[f.result() for f in as_completed([pool.submit(run,j) for j in jobs])]
(OUT/('prepared-run.json' if args.prepared_only else 'connected-run.json')).write_text(json.dumps(results,indent=2)+'\n')
if any(r['exit_code'] for r in results):raise SystemExit(1)
