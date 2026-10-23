"""Run independent emulator processes against one frozen disposable fixture."""
from concurrent.futures import ThreadPoolExecutor,as_completed
from pathlib import Path
import argparse,hashlib,json,subprocess,time
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'game/output/tests/balance';OUT.mkdir(parents=True,exist_ok=True)
parser=argparse.ArgumentParser();parser.add_argument('--build-dir',required=True);parser.add_argument('--trials',type=int,default=5);parser.add_argument('--workers',type=int,default=3);parser.add_argument('--prefix',default='');args=parser.parse_args()
sha=hashlib.sha256((ROOT/'game/output/firered-386-dev.gba').read_bytes()).hexdigest();fixture=hashlib.sha256((Path(args.build_dir)/'pokefirered.gba').read_bytes()).hexdigest()
jobs=[(mode,starter,chain) for chain in [False,True] for starter in ['BULBASAUR','CHARMANDER','SQUIRTLE'] for mode in ['normal','hardcore']]
def run(job):
 mode,starter,chain=job;label=mode+'-'+starter.lower()+('-chain' if chain else '')
 if args.prefix:label=args.prefix+'-'+label
 command=['python3',str(ROOT/'game/scripts/test_challenge_balance.py'),'--build-dir',args.build_dir,'--reuse','--trials',str(args.trials),'--mode',mode,'--starter',starter,'--label',label]
 if chain:command.append('--chain')
 else:command.append('--resume')
 start=time.monotonic()
 with (OUT/(label+'.log')).open('w') as log:
  p=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,bufsize=1)
  for line in p.stdout:log.write(line);log.flush();print(label+': '+line.rstrip(),flush=True)
  code=p.wait()
 return {'label':label,'exit_code':code,'seconds':round(time.monotonic()-start,1),'log':str((OUT/(label+'.log')).relative_to(ROOT))}
results=[]
with ThreadPoolExecutor(max_workers=args.workers) as pool:
 for future in as_completed([pool.submit(run,j) for j in jobs]):
  result=future.result();results.append(result);print('JOB',result,flush=True)
assert sha==hashlib.sha256((ROOT/'game/output/firered-386-dev.gba').read_bytes()).hexdigest()
assert fixture==hashlib.sha256((Path(args.build_dir)/'pokefirered.gba').read_bytes()).hexdigest()
report={'rom_sha256':sha,'fixture_sha256':fixture,'jobs':results,'campaign_clear_certified':False}
(OUT/((args.prefix+'-' if args.prefix else '')+'run-results.json')).write_text(json.dumps(report,indent=2)+'\n')
if any(r['exit_code'] for r in results):raise SystemExit(1)
