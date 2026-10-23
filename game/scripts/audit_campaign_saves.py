"""Read only test-generated Flash snapshots. Validate sectors and persisted milestones."""
from pathlib import Path
import hashlib,json,re,struct
root=Path(__file__).resolve().parents[2];out=root/'game/output/tests/campaign'
def checksum(data):
 total=sum(struct.unpack('<'+'I'*(len(data)//4),data[:len(data)//4*4]))&0xffffffff
 return ((total>>16)+total)&0xffff
sizes=[0xf24]+[min(0xf80,0x3d68-i*0xf80) for i in range(4)]+[min(0xf80,0x83d0-i*0xf80) for i in range(9)]
def load(path):
 raw=path.read_bytes();assert len(raw)==131072
 valid=[]
 for bank in range(2):
  sections={};counters=[]
  for i in range(14):
   sector=raw[(bank*14+i)*4096:(bank*14+i+1)*4096]
   index,expected,magic,counter=struct.unpack_from('<HHII',sector,0xff4)
   if magic!=0x08012025 or index>=14 or checksum(sector[:sizes[index]])!=expected:break
   sections[index]=sector[:sizes[index]];counters.append(counter)
  if len(sections)==14 and len(set(counters))==1:valid.append((counters[0],sections))
 assert valid,path
 counter,sections=max(valid,key=lambda row:row[0]);sb2=sections[0];sb1=b''.join(sections[i] for i in range(1,5));pc=b''.join(sections[i] for i in range(5,14))
 key=struct.unpack_from('<I',sb2,0xf20)[0]
 flag=lambda index:bool(sb1[0xee0+index//8]&(1<<(index%8)))
 stats=lambda index:struct.unpack_from('<I',sb1,0x1200+4*index)[0]^key
 inventory={}
 for start,end in [(0x310,0x3b8),(0x3b8,0x430),(0x430,0x464),(0x464,0x54c),(0x54c,0x5f8)]:
  for offset in range(start,end,4):
   item,amount=struct.unpack_from('<HH',sb1,offset)
   if item:inventory[item]=amount^(key&0xffff)
 pc_count=0
 for slot in range(420):
  mon=pc[4+slot*80:4+(slot+1)*80]
  if not mon[19]&2:continue
  assert not mon[19]&1,('bad egg',path,slot)
  personality,otid=struct.unpack_from('<II',mon)
  decoded=[v^personality^otid for v in struct.unpack_from('<12I',mon,32)]
  words=struct.unpack('<24H',struct.pack('<12I',*decoded))
  assert sum(words)&0xffff==struct.unpack_from('<H',mon,28)[0],('PC checksum',path,slot)
  pc_count+=1
 return {'save_counter':counter,'valid_banks':len(valid),'badges':sum(flag(0x820+i)<<i for i in range(8)),
         'game_clear':flag(0x82c),'network_complete':flag(0x844),'cerulean_cave_unlocked':flag(0x05c),
         'tanoby_unlocked':flag(0x849),'hof_entries':stats(10),'trainer_battles':stats(9),'captures':stats(11),
         'mewtwo_caught':bool(sb2[0x28+(150-1)//8]&(1<<((150-1)%8))),
         'tower':[{'challenge':i,'floors':sb1[0x3d38+12*i+8],'prize':bool(sb1[0x3d38+12*i+10]&1),'time_checked':bool(sb1[0x3d38+12*i+10]&2)} for i in range(4)],
         'pc_mons_with_valid_checksums':pc_count,'inventory':inventory,
         'rocket_story_trainers_defeated':{str(i):flag(0x500+i) for i in [366,367,368,369,370,371]}}
first=load(out/'first-league-test-only.sav')
final=load(out/('full-postgame-test-only.sav' if (out/'full-postgame-test-only.sav').exists() else 'campaign-postleague-test-only.sav'))
story_path=root/'game/output/tests/campaign-story/campaign-postleague-test-only.sav'
story=load(story_path)
assert first['badges']==255 and first['game_clear'] and first['hof_entries']==1
assert not first['network_complete'] and not first['cerulean_cave_unlocked']
assert not first['mewtwo_caught']
assert final['badges']==255 and final['game_clear'] and final['hof_entries']==2
assert final['network_complete'] and final['cerulean_cave_unlocked'] and final['tanoby_unlocked'] and final['mewtwo_caught']
assert all(row['floors']==8 and row['prize'] and row['time_checked'] for row in final['tower'])
assert final['pc_mons_with_valid_checksums']>=1
item_definitions=(root/'game/include/constants/items.h').read_text(encoding='utf-8')
item_ids={name:int(value,0) for name,value in re.findall(r'^#define\s+(ITEM_\w+)\s+(0x[0-9A-Fa-f]+|\d+)\b',item_definitions,re.M)}
required=['ITEM_SS_TICKET','ITEM_POKE_FLUTE','ITEM_SILPH_SCOPE','ITEM_LIFT_KEY','ITEM_CARD_KEY','ITEM_SECRET_KEY','ITEM_MASTER_BALL','ITEM_HM01','ITEM_HM02','ITEM_HM03','ITEM_HM04','ITEM_HM06','ITEM_HM07']
assert all(story['inventory'].get(item_ids[name],0)>0 for name in required)
assert all(story['rocket_story_trainers_defeated'].values())
assert item_ids['ITEM_OAKS_PARCEL'] not in story['inventory'] and item_ids['ITEM_GOLD_TEETH'] not in story['inventory']
snapshots=[*out.glob('campaign-checkpoint-*-test-only.sav'),*(root/'game/output/tests/campaign-story').glob('campaign-checkpoint-*-test-only.sav')]
for path in snapshots:load(path)
for state in [first,final,story]:state.pop('inventory')
report={'rom_sha256':hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest(),'status':'passed','first_league':first,'final_postgame':final,'story':story,'story_items_verified':required,'test_snapshots_checked':len(snapshots),'scope':'only test-created saves; sector checksums and selected persisted milestones; no user save migration claim'}
(out/'save_results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
