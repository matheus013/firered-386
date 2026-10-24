"""Plan collision-free button steps within the current map (no memory writes)."""
from pathlib import Path
from collections import deque
import contextlib,io,json,runpy,struct,subprocess,sys
root=Path(__file__).resolve().parents[3]
with contextlib.redirect_stdout(io.StringIO()): o=runpy.run_path(str(root/'games/super-fire-red/scripts/observe_route.py'))
num,read,symbols=o['num'],o['read'],o['symbols'];sb=o['sb'];g,n=read(sb+4,2)
groups=o['groups'];name=groups[groups['group_order'][g]][n]
m=json.loads((root/'games/super-fire-red/data/maps'/name/'map.json').read_text())
layout=next(x for x in json.loads((root/'games/super-fire-red/data/layouts/layouts.json').read_text())['layouts'] if x.get('id')==m['layout'])
w,h=layout['width'],layout['height'];blocks=(root/'games/super-fire-red'/layout['blockdata_filepath']).read_bytes()
grid=[[struct.unpack_from('<H',blocks,2*(y*w+x))[0] for x in range(w)] for y in range(h)]
rom=(root/'games/super-fire-red/output/tests/natural-route/rom.gba').read_bytes()
def behavior(tile):
 t=tile&0x3ff;tileset=layout['primary_tileset'] if t<640 else layout['secondary_tileset']
 address=symbols['gMetatileAttributes_'+tileset.removeprefix('gTileset_')]+4*(t if t<640 else t-640)-0x08000000
 return struct.unpack_from('<I',rom,address)[0]&0x1ff
obj=o['obj'];start=(num(obj+16,2)-7,num(obj+18,2)-7)
blocked=set()
for i in range(16):
 p=symbols['gObjectEvents']+36*i
 if i!=o['oid'] and num(p)&1 and not num(p+1)&32:blocked.add((num(p+16,2)-7,num(p+18,2)-7))
if len(sys.argv)==1:
 print(name,'player',start,'size',(w,h),'warps',[(a['x'],a['y'],a['dest_map']) for a in m['warp_events']])
 for y in range(h):print(f'{y:2} '+''.join('@' if (x,y)==start else 'N' if (x,y) in blocked else 'W' if any(a['x']==x and a['y']==y for a in m['warp_events']) else '#' if (grid[y][x]>>10)&3 else ',' if behavior(grid[y][x])==2 else '.' for x in range(w)))
 sys.exit()
target=tuple(map(int,sys.argv[1:3]));warps={(a['x'],a['y']) for a in m['warp_events']} - {target}
queue=deque([start]);prev={start:None}
while queue and target not in prev:
 x,y=queue.popleft()
 for dx,dy,key,facing in [(0,-1,'up',2),(0,1,'down',1),(-1,0,'left',3),(1,0,'right',4)]:
  p=(x+dx,y+dy)
  if 0<=p[0]<w and 0<=p[1]<h and p not in prev and p not in blocked and p not in warps and (p==target or not (grid[p[1]][p[0]]>>10)&3):
   prev[p]=((x,y),key,facing);queue.append(p)
if target not in prev:raise SystemExit('No floor path to '+str(target))
path=[];p=target
while prev[p]:p,key,facing=prev[p];path.append((key,facing))
path.reverse();direction=num(obj+24)&15;tokens=[]
for key,facing in path[:24]:
 if facing!=direction:tokens.extend([key+':1','wait:24']);direction=facing
 tokens.extend([key+':1','wait:24'])
print(name,start,'->',target,'planned steps',len(path),'batch',min(24,len(path)),flush=True)
if tokens:subprocess.run([sys.executable,str(root/'games/super-fire-red/scripts/play_route.py'),'--walk',' '.join(tokens)],check=True)
