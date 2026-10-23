"""Run assisted story/campaign/postgame integration, then audit test saves and access data.

No normal-play or Hardcore-clear claim. Generated saves remain in game/output/tests/.
"""
from pathlib import Path
import argparse,hashlib,json,shutil,subprocess,sys
root=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--collect-only',action='store_true',help='Validate and collect already completed results for the current production SHA');args=parser.parse_args()
out=root/'game/output/tests/campaign'
if not args.collect_only:
 for command in [
  [sys.executable,str(root/'game/scripts/test_campaign.py'),'--scenario','story'],
  [sys.executable,str(root/'game/scripts/test_campaign.py'),'--resume-save',str(root/'game/output/tests/campaign-story/campaign-postleague-test-only.sav')],
  [sys.executable,str(root/'game/scripts/test_campaign.py'),'--scenario','rivals'],
 ]:subprocess.run(command,check=True)
 shutil.copyfile(out/'campaign-checkpoint-12-test-only.sav',out/'first-league-test-only.sav')
 shutil.copyfile(out/'campaign-postleague-test-only.sav',out/'full-postgame-test-only.sav')
 for script in ['audit_campaign_access.py','audit_campaign_saves.py']:
  subprocess.run([sys.executable,str(root/'game/scripts'/script)],check=True)
sha=hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest()
reports={name:json.loads((root/path).read_text(encoding='utf-8')) for name,path in {
 'story':'game/output/tests/campaign-story/results.json','campaign':'game/output/tests/campaign/results.json',
 'rivals':'game/output/tests/campaign-rivals/results.json','access':'game/output/tests/campaign/access_results.json','save':'game/output/tests/campaign/save_results.json'}.items()}
for name,report in reports.items():assert report['rom_sha256']==sha,name
for name,count in [('story',30),('campaign',81),('rivals',3)]:
 assert reports[name]['exit_code']==0 and len(reports[name]['cases'])==count,name
 assert all(case['status']=='passed' for case in reports[name]['cases']),name
assert all(check['status']=='passed' for check in reports['access']['checks'])
assert reports['save']['status']=='passed'
result={'rom_sha256':sha,'status':'assisted_integration_passed','normal_play_clear_certified':False,'hardcore_campaign_clear_certified':False,
 'pokedex_386_required':False,'story_checkpoints':30,'campaign_postgame_checkpoints':81,'rival_checkpoints':3,
 'scope':'Normal mode; teleported routes, direct field-script starts, supplied level-100 teams, supplied Master Balls/Lemonade, automatic battle choices and healing; native opponents/damage/outcomes/reward game/scripts/HOF/credits/Flash saves.',
 'reports':reports}
(out/'validation_summary.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('PASS: 114 assisted checkpoints, static access audit and persisted save milestones. Normal-play/Hardcore complete runs remain uncertified.')
