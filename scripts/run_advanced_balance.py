from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,hashlib,json,subprocess
from balance_core import NativeBench,ROOT,OUT
parser=argparse.ArgumentParser();parser.add_argument('--build-dir');parser.add_argument('--workers',type=int,default=3);parser.add_argument('--trials',type=int,default=3);args=parser.parse_args()
if args.build_dir:build=args.build_dir
else:
 core=NativeBench(output_dir=OUT/'advanced-build');build=str(core.path);core.close()
sha=hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest();fixture=hashlib.sha256((__import__('pathlib').Path(build)/'pokefirered.gba').read_bytes()).hexdigest()
jobs=[(mode,starter) for starter in ['BULBASAUR','CHARMANDER','SQUIRTLE'] for mode in ['normal','hardcore']]
def run(job):
 mode,starter=job;label='pickup-'+mode+'-'+starter.lower()
 command=['python3',str(ROOT/'scripts/test_advanced_blocks.py'),'--build-dir',build,'--mode',mode,'--starter',starter,'--trials',str(args.trials),'--pickup-prep']
 with (OUT/(label+'.log')).open('w') as log:
  p=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,bufsize=1)
  for line in p.stdout:log.write(line);log.flush();print(label+': '+line.rstrip(),flush=True)
  return {'label':label,'exit_code':p.wait()}
with ThreadPoolExecutor(max_workers=args.workers) as pool:results=[f.result() for f in as_completed([pool.submit(run,j) for j in jobs])]
assert sha==hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()
(OUT/'advanced-run.json').write_text(json.dumps({'rom_sha256':sha,'fixture_sha256':fixture,'jobs':results},indent=2)+'\n')
if any(r['exit_code'] for r in results):raise SystemExit(1)
