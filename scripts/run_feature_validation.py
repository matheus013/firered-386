"""Validate the current production ROM's settings/editor update with frozen provenance."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import hashlib,json,subprocess,time
root=Path(__file__).resolve().parents[1];out=root/'output/tests/features';out.mkdir(parents=True,exist_ok=True)
reference=hashlib.sha256((root/'output/firered-386-dev.gba').read_bytes()).hexdigest()
build=Path((root/'output/hack-build-path.txt').read_text().strip())
assert hashlib.sha256((build/'pokefirered.gba').read_bytes()).hexdigest()==reference
jobs=[('regression',['run_regression.py']),('settings',['test_settings.py']),('settings-ui',['test_settings_ui.py']),('editor-ptbr',['test_training_ui.py','--language','ptbr']),('shops-ptbr',['test_shops.py','--language','ptbr']),('capture',['test_capture_rules.py']),('community',['test_community_cases.py'])]
def run(job):
 name,args=job;start=time.monotonic()
 with (out/(name+'.log')).open('w') as log:process=subprocess.run(['python3',str(root/'scripts'/args[0]),*args[1:]],stdout=log,stderr=subprocess.STDOUT)
 return {'name':name,'status':'passed' if process.returncode==0 else 'failed','exit_code':process.returncode,'seconds':round(time.monotonic()-start,1),'log':'output/tests/features/'+name+'.log'}
results=[]
with ThreadPoolExecutor(max_workers=3) as pool:
 for future in as_completed([pool.submit(run,job) for job in jobs]):
  item=future.result();results.append(item);print(item['status'].upper(),item['name'],item['seconds'],'s',flush=True)
assert hashlib.sha256((root/'output/firered-386-dev.gba').read_bytes()).hexdigest()==reference,'ROM changed during validation'
report={'rom_sha256':reference,'status':'passed' if all(r['status']=='passed' for r in results) else 'failed','jobs':sorted(results,key=lambda r:r['name']),'normal_campaign_clear_certified':False,'hardcore_campaign_clear_certified':False}
(out/'results.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'].upper(),'feature validation',flush=True)
if report['status']!='passed':raise SystemExit(1)
