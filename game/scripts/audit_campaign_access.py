"""Static map/gate audit. Never reports this as a played campaign or solved puzzle."""
from pathlib import Path
import hashlib,io,json,re,struct,subprocess,tarfile
from collections import deque
from source_base import base_tar
root=Path(__file__).resolve().parents[2];game=root/'game';out=root/'game/output/tests/campaign';out.mkdir(parents=True,exist_ok=True)
with base_tar() as tar:
 original={m.name:tar.extractfile(m).read() for m in tar if m.isfile() and m.name.startswith(('data/maps/','data/layouts/'))}
maps={p.parent.name:json.loads(p.read_text(encoding='utf-8')) for p in (game/'data/maps').glob('*/map.json')}
byid={m['id']:m for m in maps.values()};layouts=json.loads((game/'data/layouts/layouts.json').read_text(encoding='utf-8'))['layouts'];bylayout={m['id']:m for m in layouts if 'id' in m}
checks=[]
def check(name,condition,detail):
 checks.append({'id':name,'status':'passed' if condition else 'failed','detail':detail})
changed_geometry=[];added=[];warp_errors=[];warp_count=0
for folder,m in maps.items():
 baseline=json.loads(original['data/maps/'+folder+'/map.json'])
 for field in ['layout','connections','warp_events','coord_events','bg_events']:
  if m.get(field)!=baseline.get(field):changed_geometry.append(folder+':'+field)
 layout=bylayout[m['layout']]
 for warp in m['warp_events']:
  warp_count+=1
  if not (0<=warp['x']<layout['width'] and 0<=warp['y']<layout['height']):warp_errors.append([folder,'out_of_bounds',warp])
  dest=warp['dest_map'];number=str(warp['dest_warp_id'])
  if dest in ['MAP_DYNAMIC','MAP_UNDEFINED']:continue
  if dest not in byid:warp_errors.append([folder,'missing_destination',dest]);continue
  if number.lstrip('-').isdigit() and int(number)>=0 and int(number)>=len(byid[dest]['warp_events']):warp_errors.append([folder,'missing_destination_warp',warp])
 original_objects=baseline['object_events'];new_objects=m['object_events']
 if new_objects[:len(original_objects)]!=original_objects:changed_geometry.append(folder+':existing_objects')
 for obj in new_objects[len(original_objects):]:
  x,y=obj['x'],obj['y'];blocks=(game/layout['blockdata_filepath']).read_bytes()
  tile=struct.unpack_from('<H',blocks,2*(y*layout['width']+x))[0]
  added.append({'map':m['id'],'x':x,'y':y,'script':obj['script'],'on_warp':any(w['x']==x and w['y']==y for w in m['warp_events']),'overlaps_object':any(o.get('x')==x and o.get('y')==y for o in original_objects),'collision':(tile>>10)&3})
changed_layouts=[]
for name,data in original.items():
 if name.startswith('data/layouts/') and (game/name).read_bytes()!=data:changed_layouts.append(name)
check('map_geometry_connections_events_preserved',not changed_geometry,{'maps':len(maps),'changes':changed_geometry})
check('layout_blocks_preserved',not changed_layouts,{'files':sum(n.startswith('data/layouts/') for n in original),'changes':changed_layouts})
check('warp_targets_and_bounds',not warp_errors,{'warps':warp_count,'errors':warp_errors})
check('added_npcs_do_not_occupy_warps_or_existing_npcs',all(not a['on_warp'] and not a['overlaps_object'] and a['collision']==0 for a in added),{'added_npcs':added})
connectivity=[]
for folder,m in maps.items():
 new_positions={(a['x'],a['y']) for a in added if a['map']==m['id']}
 if not new_positions:continue
 layout=bylayout[m['layout']];width,height=layout['width'],layout['height'];blocks=(game/layout['blockdata_filepath']).read_bytes()
 original_objects=json.loads(original['data/maps/'+folder+'/map.json'])['object_events']
 walls={(o['x'],o['y']) for o in original_objects}
 floor={(x,y) for y in range(height) for x in range(width) if (struct.unpack_from('<H',blocks,2*(y*width+x))[0]>>10)&3==0}
 def flood(extra):
  walk=floor-walls-extra;queue=deque();seen=set()
  for warp in m['warp_events'][:1]:
   x,y=warp['x'],warp['y']
   for p in [(x,y),(x-1,y),(x+1,y),(x,y-1),(x,y+1)]:
    if p in walk:seen.add(p);queue.append(p)
  while queue:
   x,y=queue.popleft()
   for p in [(x-1,y),(x+1,y),(x,y-1),(x,y+1)]:
    if p in walk and p not in seen:seen.add(p);queue.append(p)
  return seen
 before,after=flood(set()),flood(new_positions)
 connectivity.append({'map':m['id'],'reachable_before':len(before),'reachable_after':len(after),'lost_tiles_beyond_new_npc':sorted(before-after-new_positions)})
check('center_floor_connectivity_after_added_npcs',all(not row['lost_tiles_beyond_new_npc'] for row in connectivity),connectivity)
celio=(game/'data/maps/OneIsland_PokemonCenter_1F/scripts.inc').read_text()
check('celio_league_gate_precedes_national_dex_check',celio.index('goto_if_unset FLAG_SYS_GAME_CLEAR')<celio.index('specialvar VAR_RESULT, IsNationalPokedexEnabled',celio.index('OneIsland_PokemonCenter_1F_EventScript_Celio::')), 'Static ordering, not a UI traversal.')
check('celio_sapphire_unlocks_rematch_and_cave',all(s in celio for s in ['setflag FLAG_SYS_CAN_LINK_WITH_RS','setflag FLAG_HIDE_CERULEAN_CAVE_GUARD','special SetPostgameFlags']), 'Reward script contains both unlocks; native scenario checks final state separately.')
league=(game/'data/maps/PokemonLeague_ChampionsRoom/scripts.inc').read_text()
check('rematch_uses_celio_completion_flag','call_if_set FLAG_SYS_CAN_LINK_WITH_RS, PokemonLeague_ChampionsRoom_EventScript_Rematch' in league, 'First HOF alone does not select the competitive rematch.')
gate_inventory=[]
for token in ['ITEM_HM01','ITEM_HM02','ITEM_HM03','ITEM_HM04','ITEM_HM05','ITEM_HM06','ITEM_HM07','ITEM_SS_TICKET','ITEM_POKE_FLUTE','ITEM_SILPH_SCOPE','ITEM_CARD_KEY','ITEM_LIFT_KEY','ITEM_SECRET_KEY','ITEM_GOLD_TEETH','ITEM_TEA','ITEM_RAINBOW_PASS']:
 locations=[]
 for p in [*(game/'data/maps').glob('*/scripts.inc'),*(game/'data/scripts').glob('*.inc')]:
  for line in p.read_text(encoding='utf-8').splitlines():
   if re.search(r'^\s*(?:giveitem|giveitem_msg|additem|finditem)\s+',line) and re.search(r'\b'+token+r'\b',line):locations.append({'map':p.parent.name,'command':line.strip()})
 gate_inventory.append({'item':token,'award_sources':locations})
check('critical_items_have_award_scripts',all(row['award_sources'] for row in gate_inventory),gate_inventory)
report={'rom_sha256':hashlib.sha256((root/'game/output/firered-386-dev.gba').read_bytes()).hexdigest(),'scope':'static geometry/warp/item/gate checks; conditional reachability and puzzle solutions are not proved','checks':checks}
(out/'access_results.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
for row in checks:print(row['status'].upper(),row['id'])
if any(row['status']=='failed' for row in checks):raise SystemExit(1)
