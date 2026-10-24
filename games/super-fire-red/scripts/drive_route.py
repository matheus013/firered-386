"""Walk a local target with real buttons, resolving ordinary wild/trainer battles.

No save-state rollback, forced victory, injected scripts, item grants or RAM writes.
Stops on lack of progress so a human/agent can inspect the obstruction.
"""
from pathlib import Path
import contextlib,io,runpy,subprocess,sys
root=Path(__file__).resolve().parents[3]
def state():
 with contextlib.redirect_stdout(io.StringIO()):return runpy.run_path(str(root/'games/super-fire-red/scripts/observe_route.py'))
def buttons(s):subprocess.run([sys.executable,str(root/'games/super-fire-red/scripts/play_route.py'),s],check=True)
def position(o):return (o['g'],o['n'],o['num'](o['obj']+16,2)-7,o['num'](o['obj']+18,2)-7)
initial=position(state());target=tuple(map(int,sys.argv[1:3]));stuck=0;previous=None
for attempt in range(80):
 o=state();p=position(o);num,sym=o['num'],o['symbols']
 if p[:2]!=initial[:2]:print('Reached next map through native transition:',p,flush=True);break
 battle=bool(num(sym['gMain']+0x439)&2);locked=num(sym['sLockFieldControls'])
 if battle:
  buttons('battle:9000 wait:120');continue
 if locked:
  callback=num(sym['gMain']+4,4)&~1
  start_menu=any(num(sym['gTasks']+40*i+4) and (num(sym['gTasks']+40*i,4)&~1)==sym['Task_StartMenuHandleInput'] for i in range(16))
  if callback not in [sym['CB2_Overworld'],sym['CB2_OverworldBasic']] or start_menu:
   buttons('b:1 wait:200');continue
  buttons('wait:180 '+ ' '.join(['a:4 wait:90']*8));continue
 if p[2:]==target:print('Reached walking target:',p,flush=True);break
 if p==previous:stuck+=1
 else:stuck=0
 if stuck>=3:raise SystemExit('Walking blocked; inspect screenshot/state: '+str(p))
 previous=p
 subprocess.run([sys.executable,str(root/'games/super-fire-red/scripts/navigate_route.py'),*map(str,target)],check=True)
else:raise SystemExit('Iteration limit; route remains incomplete')
