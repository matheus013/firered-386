import gdb,os
from pathlib import Path
source=Path(os.environ["HG_LIBNTR_SOURCE"])/"libraries/sim/src/sim_main.cpp"
lines=source.read_text().splitlines()
anchor=next(i for i,line in enumerate(lines) if "// Convert touch coordinate" in line)
line_number=next(i+1 for i in range(anchor,len(lines)) if "SIM::GUI::NewFrame();" in lines[i])
class Probe(gdb.Breakpoint):
 def __init__(self):
  super().__init__(str(source)+':'+str(line_number));self.frames=0;self.checked=0
 def stop(self):
  self.frames+=1
  if self.frames in (20,40):
   touch=int(gdb.parse_and_eval('s_tpData.touch'))
   x=int(gdb.parse_and_eval('s_tpData.x'));y=int(gdb.parse_and_eval('s_tpData.y'))
   expected=int(os.environ['HG_EXPECT_TOUCH']) if self.frames==20 else 0
   if touch!=expected or (touch and (x,y)!=(128,96)):
    gdb.write('HG_INPUT_FAILED frame=%d touch=%d xy=%d,%d\n'%(self.frames,touch,x,y));gdb.execute('quit 1')
   self.checked+=1;gdb.write('HG_INPUT_PASS frame=%d touch=%d xy=%d,%d\n'%(self.frames,touch,x,y))
  return False
probe=Probe()
gdb.execute('run')
if probe.checked!=2:gdb.execute('quit 1')
