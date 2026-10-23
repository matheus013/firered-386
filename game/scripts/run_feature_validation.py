"""Validate the current production ROM's settings/editor update with frozen provenance."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import hashlib,json,subprocess,time,argparse
parser=argparse.ArgumentParser();parser.add_argument('--reuse-regression',action='store_true');args=parser.parse_args()
root=Path(__file__).resolve().parents[2];out=root/'game/output/tests/features';out.mkdir(parents=True,exist_ok=True)
reference=hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest()
build=Path((root/'game/output/hack-build-path.txt').read_text().strip())
assert hashlib.sha256((build/'pokefirered.gba').read_bytes()).hexdigest()==reference
jobs=[('regression',['run_regression.py']),('settings',['test_settings.py']),('settings-ui',['test_settings_ui.py']),('editor-ptbr',['test_training_ui.py','--language','ptbr']),('shops-ptbr',['test_shops.py','--language','ptbr']),('capture',['test_capture_rules.py']),('community',['test_community_cases.py'])]
def run(job):
 name,command_args=job;start=time.monotonic()
 if name=='regression' and args.reuse_regression:
  previous=json.loads((root/'game/output/tests/checklist/regression_results.json').read_text())
  assert previous['rom_sha256']==reference and len(previous['suites'])==7 and all(r['status']=='passed' for r in previous['suites'])
  return {'name':name,'status':'passed','exit_code':0,'seconds':0,'reused_current_rom':True,'log':'game/output/tests/checklist/regression_results.json'}
 with (out/(name+'.log')).open('w') as log:process=subprocess.run(['python3',str(root/'scripts'/command_args[0]),*command_args[1:]],stdout=log,stderr=subprocess.STDOUT)
 return {'name':name,'status':'passed' if process.returncode==0 else 'failed','exit_code':process.returncode,'seconds':round(time.monotonic()-start,1),'log':'game/output/tests/features/'+name+'.log'}
results=[]
with ThreadPoolExecutor(max_workers=3) as pool:
 for future in as_completed([pool.submit(run,job) for job in jobs]):
  item=future.result();results.append(item);print(item['status'].upper(),item['name'],item['seconds'],'s',flush=True)
assert hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest()==reference,'ROM changed during validation'
report={'rom_sha256':reference,'status':'passed' if all(r['status']=='passed' for r in results) else 'failed','jobs':sorted(results,key=lambda r:r['name']),'normal_campaign_clear_certified':False,'hardcore_campaign_clear_certified':False}
(out/'results.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'].upper(),'feature validation',flush=True)
if report['status']!='passed':raise SystemExit(1)
