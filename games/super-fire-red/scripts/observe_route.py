"""Read-only inspection of the current production-ROM route test."""
from pathlib import Path
import json,re,struct,sys
if hasattr(sys.stdout,'reconfigure'):sys.stdout.reconfigure(encoding='utf-8')
root=Path(__file__).resolve().parents[3];out=root/'games/super-fire-red/output/tests/natural-route'
symbols={m[3]:int(m[1],16) for line in (out/'symbols.txt').read_text(encoding='utf-8-sig').splitlines() if (m:=re.match(r'([0-9a-fA-F]+) (\w) (.+)',line))}
ram={0x02000000:(out/'ewram.bin').read_bytes(),0x03000000:(out/'iwram.bin').read_bytes()}
def read(addr,n=1):
 base=addr&0xff000000;return ram[base][addr-base:addr-base+n]
def num(addr,n=1):return int.from_bytes(read(addr,n),'little')
chars={}
for a,b in re.findall(r"^'(.*)'\s*=\s*([0-9A-F]{2})\s*$",(root/'games/super-fire-red/charmap.txt').read_text(encoding='utf-8'),re.M):chars.setdefault(int(b,16),a)
def text(data):return ''.join(chars.get(c,'\n' if c in [0xfa,0xfb,0xfe] else f'<{c:02x}>') for c in data.split(b'\xff')[0])
sb=num(symbols['gSaveBlock1Ptr'],4)
if sb in range(0x2000000,0x2040000):
 groups=json.loads((root/'games/super-fire-red/data/maps/map_groups.json').read_text());g,n=read(sb+4,2)
 print('map',g,n,groups[groups['group_order'][g]][n] if g<len(groups['group_order']) and n<len(groups[groups['group_order'][g]]) else '?','saved XY',num(sb,2),num(sb+2,2))
 sb2=num(symbols['gSaveBlock2Ptr'],4);key=num(sb2+0xf20,4)
 print('money',num(sb+0x290,4)^key,'exp share',bool(num(sb+0xee0+0x20//8)&(1<<(0x20%8))))
 print('balls',[(num(sb+0x430+4*i,2),num(sb+0x432+4*i,2)^(key&0xffff)) for i in range(13) if num(sb+0x430+4*i,2)])
avatar=symbols['gPlayerAvatar'];oid=num(avatar+5);obj=symbols['gObjectEvents']+36*oid
print('player',oid,'XY',num(obj+16,2)-7,num(obj+18,2)-7,'dir',num(obj+24)&15,'movement',list(read(avatar,7)))
callback=num(symbols['gMain']+4,4)&~1
print('callback',hex(callback),next((k for k,v in symbols.items() if v==callback),'?'),'battle',bool(num(symbols['gMain']+0x439)&2))
for name in ['gStringVar4','gDisplayedStringBattle']:
 if name in symbols:print(name,text(read(symbols[name],500)).strip())
count=num(symbols['gPlayerPartyCount']);print('party count',count)
for i in range(min(count,6)):
 p=symbols['gPlayerParty']+100*i;print(i,text(read(p+8,10)),'level',num(p+84),'hp',num(p+86,2),'/',num(p+88,2))
print('tasks',[(i,next((k for k,v in symbols.items() if v==(num(symbols['gTasks']+40*i,4)&~1)),hex(num(symbols['gTasks']+40*i,4)))) for i in range(16) if num(symbols['gTasks']+40*i+4)])
print('field lock',num(symbols['sLockFieldControls']),'battle outcome',num(symbols['gBattleOutcome']))
if num(symbols['gMain']+0x439)&2:
 print('controllers',[(i,next((k for k,v in symbols.items() if v==(num(symbols['gBattlerControllerFuncs']+4*i,4)&~1)),hex(num(symbols['gBattlerControllerFuncs']+4*i,4)))) for i in range(4)])
