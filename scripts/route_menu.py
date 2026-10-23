"""Overworld native menu conveniences: save or swap an acquired party member to lead."""
from pathlib import Path
import contextlib,io,runpy,subprocess,sys
root=Path(__file__).resolve().parents[1]
def state():
 with contextlib.redirect_stdout(io.StringIO()):return runpy.run_path(str(root/'scripts/observe_route.py'))
def press(s):subprocess.run([sys.executable,str(root/'scripts/play_route.py'),s],check=True)
for _ in range(8):
 o=state()
 if not o['num'](o['symbols']['sLockFieldControls']):break
 press('b:1 wait:200')
else:raise SystemExit('Could not close current menu; inspect before opening overworld menu')
press('start:1 wait:80')
o=state();cursor=o['num'](o['symbols']['sStartMenuCursorPos']);target=4 if sys.argv[1]=='save' else 1
press(' '.join([('up' if cursor>target else 'down')+':1 wait:16']*abs(cursor-target)+['a:1 wait:180']))
if sys.argv[1]=='save':press('a:1 wait:120 a:1 wait:240 b:1 wait:80 b:1 wait:80')
elif sys.argv[1]=='lead':
 index=int(sys.argv[2]);o=state();current=o['num'](o['symbols']['gPartyMenu']+9)
 assert 0<=index<o['num'](o['symbols']['gPlayerPartyCount'])
 steps=[('up' if current>index else 'down')+':1 wait:16']*abs(current-index)
 press(' '.join(steps+['a:1 wait:40 down:1 wait:16 a:1 wait:40']+['up:1 wait:16']*index+['a:1 wait:100 b:1 wait:180 b:1 wait:80']))
