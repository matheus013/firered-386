"""Complete four isolated native battles with PT-BR enabled and normal HP."""
from concurrent.futures import ThreadPoolExecutor
import hashlib,json,subprocess
from balance_core import NativeBench,ROOT
from challenge_planner import stages
out=ROOT/'game/output/tests/ptbr-battles';out.mkdir(parents=True,exist_ok=True)
base=NativeBench(output_dir=out);build=base.path;sha=base.sha;base.close()
p=build/'src/main.c';text=p.read_text()
assert 'NewGameInitData();Nuzlocke_ChooseMode' in text
text=text.replace('NewGameInitData();Nuzlocke_ChooseMode','NewGameInitData();GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);Nuzlocke_ChooseMode')
text=text.replace('gBenchOutput[0]=gBattleOutcome;','gBenchOutput[0]=GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR ? gBattleOutcome : 0xDEAD;')
p.write_text(text)
with (out/'ptbr-build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=build,stdout=log,stderr=subprocess.STDOUT,check=True)
tasks=[(stage,mode) for stage in stages() if stage['key'] in ('LeaderBrock','EliteFourAgatha') for mode in (0,1)]
def run_case(pair):
 index,(stage,mode)=pair;core=NativeBench(build_dir=build,output_dir=out/str(index),reuse=True)
 enemy=next(iter(stage['variants'].values()));party=[dict(m,iv=31) for m in enemy['team']]
 try:result=core.battle(enemy['id'],mode,stage['badges'],stage['league'],stage['post'],party,seed=991+index)
 finally:core.close()
 print(enemy['id'],mode,result['completed'],result['outcome'],result['turns'],flush=True)
 assert result['completed'] and result['outcome'] in (1,2,3) and result['turns']>0
 return {'trainer':enemy['id'],'mode':mode,'result':result}
with ThreadPoolExecutor(max_workers=4) as pool:rows=list(pool.map(run_case,enumerate(tasks)))
assert len(rows)==4
report={'rom_sha256':sha,'fixture_sha256':hashlib.sha256((build/'pokefirered.gba').read_bytes()).hexdigest(),'status':'passed',
 'language':'PTBR; asserted at battle end','scope':'four isolated boss-copy player teams, actual HP and native enemy AI; no campaign/capture validation','rows':rows}
(out/'results.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('PASS four complete PT-BR battles, normal/Hardcore.',flush=True)
