"""Repeat existing regression suites and retain logs/results for the checklist."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import subprocess,json,hashlib,time,argparse
root=Path(__file__).resolve().parents[2]
out=root/'game/output/tests/checklist';out.mkdir(parents=True,exist_ok=True)
reference_sha=hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest()
parser=argparse.ArgumentParser();parser.add_argument('--reuse-shops',action='store_true');args=parser.parse_args()
suites=['test_hardcore.py','test_training.py','test_training_ui.py','test_trainer_battles.py','test_encounters.py','test_shops.py','test_elite_four.py']
results=[]
def run(name):
 start=time.monotonic()
 if name=='test_shops.py' and args.reuse_shops:
  existing=json.loads((root/'game/output/tests/shops/results.json').read_text())
  assert existing['rom_sha256']==reference_sha and existing['count']==46 and all(c['passed'] for c in existing['cases'])
  return {'suite':name,'status':'passed','exit_code':0,'seconds':0,'reused_current_rom':True,'log':'game/output/tests/shops/results.json'}
 with (out/(name+'.log')).open('w') as log:
  p=subprocess.run(['python3',str(root/'game/scripts'/name)],stdout=log,stderr=subprocess.STDOUT)
 return {'suite':name,'status':'passed' if p.returncode==0 else 'failed','exit_code':p.returncode,'seconds':round(time.monotonic()-start,1),'log':'game/output/tests/checklist/'+name+'.log'}
with ThreadPoolExecutor(max_workers=3) as pool:
 for f in as_completed([pool.submit(run,name) for name in suites]):
  result=f.result();results.append(result);print(result['status'].upper(),result['suite'],result['seconds'],'s',flush=True)
assert hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha,'Production ROM changed during regression'
report={'rom_sha256':reference_sha,'suites':sorted(results,key=lambda r:r['suite'])}
(out/'regression_results.json').write_text(json.dumps(report,indent=2)+'\n')
if any(r['status']=='failed' for r in results):raise SystemExit(1)
