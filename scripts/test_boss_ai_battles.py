"""Finish isolated mirror battles against all gyms/E4 in both modes.

This is an execution/stall regression, not a difficulty or campaign verdict.
Player records are injected copies of each boss's team, with actual HP/stats,
no healing items and a declared greedy bot. No capture/travel is executed.
"""
from pathlib import Path
import json,hashlib
from concurrent.futures import ThreadPoolExecutor
from balance_core import NativeBench,ROOT
from challenge_planner import stages

out=ROOT/'output/tests/boss-ai-battles';out.mkdir(parents=True,exist_ok=True)
base=NativeBench(output_dir=out);build=base.path;sha=base.sha;base.close()
tasks=[(stage,mode) for stage in stages() if stage['key'].startswith(('Leader','EliteFour')) for mode in (0,1)]
def run_lane(lane):
 core=NativeBench(build_dir=build,output_dir=out/str(lane),reuse=True)
 rows=[]
 try:
  for stage,mode in tasks[lane::4]:
   enemy=next(iter(stage['variants'].values()))
   party=[dict(m,iv=31) for m in enemy['team']]
   result=core.battle(enemy['id'],mode,stage['badges'],stage['league'],stage['post'],party,seed=20261022+stage['index'])
   row={'trainer':enemy['id'],'mode':mode,'result':result}
   rows.append(row)
   report={'rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest(),
    'suite':'isolated mirror battles, greedy player bot; injected boss-copy player teams; native HP/damage/enemy AI',
    'campaign_clear_certified':False,'difficulty_calibrated':False,'rows':rows}
   (core.out/'results.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
   print(enemy['id'],mode,'completed',result['completed'],'outcome',result['outcome'],'turns',result['turns'],flush=True)
   assert result['completed'] and result['outcome'] in (1,2,3) and result['turns']>0,row
 finally:
  core.close()
 return rows
with ThreadPoolExecutor(max_workers=4) as pool:
 rows=[row for lane_rows in pool.map(run_lane,range(4)) for row in lane_rows]
report={'rom_sha256':sha,'fixture_sha256':hashlib.sha256((build/'pokefirered.gba').read_bytes()).hexdigest(),
 'suite':'isolated mirror battles, greedy player bot; injected boss-copy player teams; native HP/damage/enemy AI',
 'campaign_clear_certified':False,'difficulty_calibrated':False,'rows':rows}
(out/'results.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
assert len(rows)==32,len(rows)
print('PASS: 32 complete native boss battles; no campaign/difficulty claim.',flush=True)
