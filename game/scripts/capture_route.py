"""Capture the current wild encounter through the native Bag UI, with owned balls."""
from pathlib import Path
import contextlib,io,runpy,subprocess,sys
root=Path(__file__).resolve().parents[2]
def state():
 with contextlib.redirect_stdout(io.StringIO()):return runpy.run_path(str(root/'game/scripts/observe_route.py'))
def press(s):subprocess.run([sys.executable,str(root/'game/scripts/play_route.py'),s],check=True)
for attempt in range(70):
 o=state();num,read,s=o['num'],o['read'],o['symbols'];battle=bool(num(s['gMain']+0x439)&2)
 if not battle and num(s['gBattleOutcome'])==7:print('Native capture completed',flush=True);break
 message=o['text'](read(s['gDisplayedStringBattle'],500))
 if 'Give a nickname' in message:
  press('b:1 wait:180 b:1 wait:120');continue
 func=num(s['gBattlerControllerFuncs'],4)&~1
 if func==0x0802e688:
  assert not num(s['gBattleTypeFlags'],4)&8,'This is a trainer battle'
  cursor=num(s['gActionSelectionCursor']);tokens=[]
  if cursor&2:tokens+=['up:1 wait:16']
  if not cursor&1:tokens+=['right:1 wait:16']
  tokens+=['a:1 wait:180'];press(' '.join(tokens))
  o=state();pocket=o['num'](s['gBagMenuState']+6,2)
  for _ in range((2-pocket)%3):press('right:1 wait:50')
  press('a:1 wait:50 a:1 wait:900');continue
 press('a:1 wait:150')
else:raise SystemExit('Capture not completed; inspect state')
