"""Load and walk the Hoenn and Johto pilot maps through the real New Game loader, in an isolated build.

Never touches output/firered-386-dev.gba: the region branch policy forbids replacing distributed builds.
Data cases check what loads (layout, tilesets, connection, door cell, tile animation; Pallet Town is the
Kanto control). Walk cases hold the D-pad like a player: through every door and back out, up and down the
stairs, into the Route 29 gate, across the Littleroot/Route 101 and New Bark/Route 29 connections, and
around the Hoenn bedroom cell that blocks north/south only.
"""
from pathlib import Path
import json,os,re,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/region-pilot-maps';out.mkdir(parents=True,exist_ok=True)
(out/'results.json').write_text(json.dumps({'status':'running','note':'No passing result for this run until every assertion finishes.'})+'\n')
for old in out.glob('*.png'):old.unlink()
cache=Path(os.environ.get('XDG_CACHE_HOME',Path.home()/'.cache'))/'firered-386'
test=Path(tempfile.mkdtemp(prefix='region-pilot-',dir=cache));(out/'build-path.txt').write_text(str(test)+'\n')
files=subprocess.check_output(['python3',str(game/'scripts/source_base.py'),'--files'])
subprocess.run(['tar','-C',str(game),'-cf','-','-T','-'],input=files,stdout=open(test/'src.tar','wb'),check=True)
subprocess.run(['tar','-xf','src.tar'],cwd=test,check=True);(test/'src.tar').unlink()
subprocess.run(['bash','install.sh',str(test)],cwd=cache/'agbcc',check=True,stdout=subprocess.DEVNULL)

LITTLEROOT,ROUTE101,B1F,B2F,M1F,M2F,BIRCH=range(7)           # gMapGroup_Hoenn
NEWBARK,ROUTE29,P1F,P2F,HOUSE1,HOUSE2,ELM,GATE=range(8)       # gMapGroup_Johto
HOENN,JOHTO='MAP_HOENN_','MAP_JOHTO_'
# (map, spawn x, spawn y, probe x, probe y, steps). No steps = data case.
# Step modes: MAP = hold until the map changes, POS = hold until the player steps, HOLD = hold 48 frames,
# BATTLE = walk back and forth until a wild Pokemon appears. Battle cases lead with a level-5 Pokemon; all
# other cases lead with level 100 under Repel, so the grass on the pilot routes cannot interrupt them.
CASES=[
 (HOENN+'LITTLEROOT_TOWN',10,8,5,8,[]),
 (HOENN+'ROUTE101',10,8,5,8,[]),
 ('MAP_PALLET_TOWN',10,8,5,8,[]),
 (HOENN+'LITTLEROOT_TOWN',5,9,0,0,[('UP','MAP'),('DOWN','MAP')]),      # Brendan's door, doormat out
 (HOENN+'LITTLEROOT_TOWN',14,9,0,0,[('UP','MAP'),('DOWN','MAP')]),     # May's door
 (HOENN+'LITTLEROOT_TOWN',7,17,0,0,[('UP','MAP'),('DOWN','MAP')]),     # Birch's lab
 (HOENN+'LITTLEROOT_TOWN_BRENDANS_HOUSE_1F',8,3,0,0,[('UP','MAP'),('DOWN','POS'),('UP','MAP')]),
 (HOENN+'LITTLEROOT_TOWN_MAYS_HOUSE_1F',2,3,0,0,[('UP','MAP'),('DOWN','POS'),('UP','MAP')]),
 (HOENN+'LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',1,3,0,0,[('DOWN','HOLD')]),  # (1,4) blocks entry from the north
 (HOENN+'LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',2,4,0,0,[('LEFT','POS'),('UP','HOLD'),('DOWN','HOLD'),('RIGHT','POS')]),
 (HOENN+'LITTLEROOT_TOWN',10,0,0,0,[('UP','MAP'),('DOWN','MAP')]),     # walk north into Route 101 and back (story state 3)
 (JOHTO+'NEW_BARK_TOWN',10,10,10,9,[]),
 (JOHTO+'ROUTE29',34,11,34,10,[]),
 (JOHTO+'NEW_BARK_TOWN',10,10,0,0,[('UP','MAP'),('DOWN','MAP')]),      # Elm's lab
 (JOHTO+'NEW_BARK_TOWN',20,12,0,0,[('UP','MAP'),('DOWN','MAP')]),      # player's house
 (JOHTO+'NEW_BARK_TOWN',4,22,0,0,[('UP','MAP'),('DOWN','MAP')]),       # house 1
 (JOHTO+'NEW_BARK_TOWN',15,22,0,0,[('UP','MAP'),('DOWN','MAP')]),      # house 2, yellow door
 (JOHTO+'NEW_BARK_TOWN_PLAYERS_HOUSE_1F',9,2,0,0,[('RIGHT','MAP')]),   # up-right stairs fire moving east
 (JOHTO+'NEW_BARK_TOWN_PLAYERS_HOUSE_2F',10,2,0,0,[('LEFT','MAP')]),   # down-left stairs fire moving west
 (JOHTO+'ROUTE29',34,11,0,0,[('UP','MAP'),('DOWN','MAP')]),            # Route 46 gate and back
 (JOHTO+'NEW_BARK_TOWN',0,12,0,0,[('LEFT','MAP'),('RIGHT','MAP')]),    # walk west into Route 29 and back
 (HOENN+'ROUTE101',3,3,0,0,[('RIGHT','BATTLE')]),                    # zig-zag in tall grass until a wild battle
 (JOHTO+'ROUTE29',3,3,0,0,[('RIGHT','BATTLE')]),
 (HOENN+'LITTLEROOT_TOWN',10,8,0,0,[('NONE','TOWNMAP')]),             # open the Town Map like the item does
 (JOHTO+'NEW_BARK_TOWN',10,10,0,0,[('NONE','TOWNMAP')]),
 ('MAP_PALLET_TOWN',10,8,0,0,[('NONE','TOWNMAP')]),                   # control: Kanto section
]
# Hoenn batch 1 (Oldale, Routes 102/103, Petalburg): connections both ways and FireRed Pokemon Center /
# Mart doors in and out. Expected (map, cells) per step; map numbers resolved after the build.
BATCH_WALKS={}
for _m,_x,_y,_steps,_want in (
  (HOENN+'ROUTE101',10,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_OLDALE_TOWN',{(10,19)}),('MAP_HOENN_ROUTE101',{(10,0)})]),
  (HOENN+'OLDALE_TOWN',0,10,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_HOENN_ROUTE102',{(49,10)}),('MAP_HOENN_OLDALE_TOWN',{(0,10)})]),
  (HOENN+'OLDALE_TOWN',10,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_ROUTE103',{(10,21)}),('MAP_HOENN_OLDALE_TOWN',{(10,0)})]),
  (HOENN+'ROUTE102',0,7,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_HOENN_PETALBURG_CITY',None),('MAP_HOENN_ROUTE102',{(0,7)})]),
  (HOENN+'OLDALE_TOWN',6,17,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_OLDALE_TOWN_POKEMON_CENTER_1F',{(7,8)}),('MAP_HOENN_OLDALE_TOWN',{(6,17)})]),
  (HOENN+'OLDALE_TOWN',14,7,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_OLDALE_TOWN_MART',{(4,7)}),('MAP_HOENN_OLDALE_TOWN',{(14,7)})]),
  (HOENN+'PETALBURG_CITY',20,17,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_PETALBURG_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_HOENN_PETALBURG_CITY',{(20,17)})]),
  (HOENN+'PETALBURG_CITY',25,13,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_PETALBURG_CITY_MART',{(4,7)}),('MAP_HOENN_PETALBURG_CITY',{(25,13)})]),
  # Hoenn batch 2: Petalburg west into Route 104, north to Rustboro, Petalburg Woods and Rustboro's doors.
  (HOENN+'PETALBURG_CITY',0,12,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_HOENN_ROUTE104',{(39,62)}),('MAP_HOENN_PETALBURG_CITY',{(0,12)})]),
  (HOENN+'ROUTE104',14,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_RUSTBORO_CITY',{(14,59)}),('MAP_HOENN_ROUTE104',{(14,0)})]),
  (HOENN+'ROUTE104',10,39,[('UP','MAP')],[('MAP_HOENN_PETALBURG_WOODS',None)]),
  (HOENN+'ROUTE104',10,29,[('DOWN','MAP')],[('MAP_HOENN_PETALBURG_WOODS',None)]),
  (HOENN+'RUSTBORO_CITY',16,39,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_RUSTBORO_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_HOENN_RUSTBORO_CITY',{(16,39)})]),
  (HOENN+'RUSTBORO_CITY',16,46,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_RUSTBORO_CITY_MART',{(4,7)}),('MAP_HOENN_RUSTBORO_CITY',{(16,46)})]),
  # Hoenn batch 3: Rustboro east into Route 116, both ends of Rusturf Tunnel, Verdanturf's doors.
  (HOENN+'RUSTBORO_CITY',39,10,[('RIGHT','MAP'),('LEFT','MAP')],[('MAP_HOENN_ROUTE116',{(0,10)}),('MAP_HOENN_RUSTBORO_CITY',{(39,10)})]),
  (HOENN+'ROUTE116',47,9,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_RUSTURF_TUNNEL',None),('MAP_HOENN_ROUTE116',None)]),
  (HOENN+'VERDANTURF_TOWN',8,2,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_RUSTURF_TUNNEL',None),('MAP_HOENN_VERDANTURF_TOWN',None)]),
  (HOENN+'VERDANTURF_TOWN',16,4,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_VERDANTURF_TOWN_POKEMON_CENTER_1F',{(7,8)}),('MAP_HOENN_VERDANTURF_TOWN',{(16,4)})]),
  (HOENN+'VERDANTURF_TOWN',12,4,[('UP','MAP'),('DOWN','MAP')],[('MAP_HOENN_VERDANTURF_TOWN_MART',{(4,7)}),('MAP_HOENN_VERDANTURF_TOWN',{(12,4)})]),
  # Johto batch 1: Route 29 west into Cherrygrove, north to Routes 30 and 31, and the Pokemon Center/Mart doors.
  (JOHTO+'ROUTE29',0,16,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_CHERRYGROVE_CITY',{(64,10)}),('MAP_JOHTO_ROUTE29',{(0,16)})]),
  (JOHTO+'CHERRYGROVE_CITY',34,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ROUTE30',{(25,57)}),('MAP_JOHTO_CHERRYGROVE_CITY',{(34,0)})]),
  (JOHTO+'ROUTE30',22,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ROUTE31',{(32,22)}),('MAP_JOHTO_ROUTE30',{(22,0)})]),
  (JOHTO+'CHERRYGROVE_CITY',47,8,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_CHERRYGROVE_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_CHERRYGROVE_CITY',{(47,8)})]),
  (JOHTO+'CHERRYGROVE_CITY',41,8,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_CHERRYGROVE_CITY_MART',{(4,7)}),('MAP_JOHTO_CHERRYGROVE_CITY',{(41,8)})]),
  # Johto batch 2: Route 31 -> gate -> Violet City (the map edge is scenery only, like HGSS), and Violet's doors.
  (JOHTO+'ROUTE31',5,10,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_GATE_ROUTE31_VIOLET_CITY',None),('MAP_JOHTO_ROUTE31',None)]),
  (JOHTO+'GATE_ROUTE31_VIOLET_CITY',2,5,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_VIOLET_CITY',None),('MAP_JOHTO_GATE_ROUTE31_VIOLET_CITY',None)]),
  (JOHTO+'VIOLET_CITY',39,46,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_VIOLET_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_VIOLET_CITY',{(39,46)})]),
  (JOHTO+'VIOLET_CITY',16,36,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_VIOLET_CITY_MART',{(4,7)}),('MAP_JOHTO_VIOLET_CITY',{(16,36)})]),
  # Johto batch 3: Violet south into Route 32 and its Pokemon Center by Union Cave.
  (JOHTO+'VIOLET_CITY',17,54,[('DOWN','MAP'),('UP','MAP')],[('MAP_JOHTO_ROUTE32',{(21,0)}),('MAP_JOHTO_VIOLET_CITY',{(17,54)})]),
  (JOHTO+'ROUTE32',25,85,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ROUTE32_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_ROUTE32',{(25,85)})]),
  # Johto batch 4: Route 32 -> Union Cave 1F -> Route 33 -> Azalea Town, and Azalea's doors.
  (JOHTO+'ROUTE32',17,92,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_UNION_CAVE_1F',None),('MAP_JOHTO_ROUTE32',None)]),
  (JOHTO+'ROUTE33',27,18,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_UNION_CAVE_1F',None),('MAP_JOHTO_ROUTE33',None)]),
  (JOHTO+'ROUTE33',0,22,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_AZALEA_TOWN',{(61,23)}),('MAP_JOHTO_ROUTE33',{(0,22)})]),
  (JOHTO+'AZALEA_TOWN',31,16,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_AZALEA_TOWN_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_AZALEA_TOWN',{(31,16)})]),
  (JOHTO+'AZALEA_TOWN',40,12,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_AZALEA_TOWN_MART',{(4,7)}),('MAP_JOHTO_AZALEA_TOWN',{(40,12)})]),
  # Johto batch 5: Azalea -> gate -> Ilex Forest -> gate <- Route 34.
  (JOHTO+'AZALEA_TOWN',9,16,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_GATE_AZALEA_TOWN_ILEX_FOREST',None),('MAP_JOHTO_AZALEA_TOWN',None)]),
  (JOHTO+'GATE_AZALEA_TOWN_ILEX_FOREST',2,5,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_ILEX_FOREST',None),('MAP_JOHTO_GATE_AZALEA_TOWN_ILEX_FOREST',None)]),
  (JOHTO+'ILEX_FOREST',19,14,[('UP','MAP')],[('MAP_JOHTO_GATE_ILEX_FOREST_ROUTE34',None)]),
  (JOHTO+'ROUTE34',31,66,[('DOWN','MAP')],[('MAP_JOHTO_GATE_ILEX_FOREST_ROUTE34',None)]),
  # Johto batch 6: Route 34 north into Goldenrod, its Pokemon Center and Dept. Store.
  (JOHTO+'ROUTE34',26,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_GOLDENROD_CITY',{(33,45)}),('MAP_JOHTO_ROUTE34',{(26,0)})]),
  (JOHTO+'GOLDENROD_CITY',28,37,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_GOLDENROD_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_GOLDENROD_CITY',{(28,37)})]),
  (JOHTO+'GOLDENROD_CITY',39,37,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_GOLDENROD_CITY_MART',{(4,7)}),('MAP_JOHTO_GOLDENROD_CITY',{(39,37)})]),
  # Johto batch 7: Goldenrod -> gate -> Route 35 -> National Park gate <- Route 36 <-> Violet.
  (JOHTO+'GOLDENROD_CITY',33,8,[('UP','MAP')],[('MAP_JOHTO_GATE_GOLDENROD_CITY_ROUTE35',None)]),
  (JOHTO+'ROUTE35',13,47,[('DOWN','MAP')],[('MAP_JOHTO_GATE_GOLDENROD_CITY_ROUTE35',None)]),
  (JOHTO+'ROUTE35',17,6,[('UP','MAP')],[('MAP_JOHTO_GATE_NATIONAL_PARK',None)]),
  (JOHTO+'ROUTE36',17,20,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_GATE_NATIONAL_PARK',None),('MAP_JOHTO_ROUTE36',None)]),
  (JOHTO+'VIOLET_CITY',0,18,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_ROUTE36',{(65,20)}),('MAP_JOHTO_VIOLET_CITY',{(0,18)})]),
  # Johto batch 8: Route 36 north to Route 37, the arrow warps into Ecruteak, Ecruteak's doors.
  (JOHTO+'ROUTE36',35,0,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ROUTE37',{(13,40)}),('MAP_JOHTO_ROUTE36',{(35,0)})]),
  (JOHTO+'ROUTE37',17,11,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ECRUTEAK_CITY',None),('MAP_JOHTO_ROUTE37',None)]),
  (JOHTO+'ECRUTEAK_CITY',39,48,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ECRUTEAK_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_ECRUTEAK_CITY',{(39,48)})]),
  (JOHTO+'ECRUTEAK_CITY',48,41,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_ECRUTEAK_CITY_MART',{(4,7)}),('MAP_JOHTO_ECRUTEAK_CITY',{(48,41)})]),
  # Johto batch 9: Ecruteak west gate, Route 38 -> 39 -> Olivine, Olivine's doors.
  (JOHTO+'ECRUTEAK_CITY',9,35,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_GATE_ECRUTEAK_CITY_ROUTE38',None),('MAP_JOHTO_ECRUTEAK_CITY',None)]),
  (JOHTO+'ROUTE38',42,16,[('RIGHT','MAP'),('LEFT','MAP')],[('MAP_JOHTO_GATE_ECRUTEAK_CITY_ROUTE38',None),('MAP_JOHTO_ROUTE38',None)]),
  (JOHTO+'ROUTE38',0,21,[('LEFT','MAP'),('RIGHT','MAP')],[('MAP_JOHTO_ROUTE39',{(39,21)}),('MAP_JOHTO_ROUTE38',{(0,21)})]),
  (JOHTO+'ROUTE39',25,53,[('DOWN','MAP'),('UP','MAP')],[('MAP_JOHTO_OLIVINE_CITY',{(19,0)}),('MAP_JOHTO_ROUTE39',{(25,53)})]),
  (JOHTO+'OLIVINE_CITY',15,44,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_OLIVINE_CITY_POKEMON_CENTER_1F',{(7,8)}),('MAP_JOHTO_OLIVINE_CITY',{(15,44)})]),
  (JOHTO+'OLIVINE_CITY',26,41,[('UP','MAP'),('DOWN','MAP')],[('MAP_JOHTO_OLIVINE_CITY_MART',{(4,7)}),('MAP_JOHTO_OLIVINE_CITY',{(26,41)})])):
 CASES.append((_m,_x,_y,0,0,_steps));BATCH_WALKS[len(CASES)]=_want
# Region map pages scroll: hold RIGHT on Hoenn (opens at column 0) and LEFT on Johto (opens at its last).
PAGE_SCROLL={}
# Switch menu (M5 step 3): Start puts the cursor on the switch button, A opens the menu, the key picks
# an entry, A switches. Screen tile (10,12) must then show that page (scroll 0 when not the player's).
SWITCHES={}
for _m,_x,_y,_key,_setup,_page in (('MAP_PALLET_TOWN',10,8,'RIGHT',2048|8192,'hoenn'),(HOENN+'LITTLEROOT_TOWN',10,8,'DOWN',8192,'johto')):
 CASES.append((_m,_x,_y,0,0,[(_key,'SWITCH')],_setup));SWITCHES[len(CASES)]=(_m,_page)
# Fly (M5 step 4): open the Fly map like the party menu does, optionally move, press A; record the landing.
# Save and Continue (M5): save on a pilot map, wipe the location in RAM, load the save and go through
# CB2_ContinueSavedGame like the main menu's Continue; the player must be back on the same map and cell,
# with the region's tilesets.
CONTINUES={}
for _m,_x,_y,_tiles in ((HOENN+'LITTLEROOT_TOWN',10,8,'hoenn'),(JOHTO+'ROUTE29',34,11,'johto'),(HOENN+'LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB',6,8,'hoenn_interior')):
 CASES.append((_m,_x,_y,0,0,[('NONE','CONTINUE')],0));CONTINUES[len(CASES)]=(_m,_x,_y,_tiles)
FLIGHTS={}
for _m,_x,_y,_key,_setup,_land in ((HOENN+'LITTLEROOT_TOWN',10,8,'NONE',0,(5,9)),(JOHTO+'NEW_BARK_TOWN',10,10,'NONE',0,(20,12)),
                                   (HOENN+'ROUTE101',10,10,'DOWN',2048,(5,9))):
 CASES.append((_m,_x,_y,0,0,[(_key,'FLY')],_setup));FLIGHTS[len(CASES)]=(_m,_land)
for _m,_x,_y,_key,_page,_start,_end in ((HOENN+'LITTLEROOT_TOWN',10,8,'RIGHT','hoenn',0,6),(JOHTO+'NEW_BARK_TOWN',10,10,'LEFT','johto',3,0)):
 CASES.append((_m,_x,_y,0,0,[(_key,'TOWNMAP')]));PAGE_SCROLL[len(CASES)]=(_page,_start,_end)
# TALK: stand next to every pilot event (object or sign), turn to it, press A and read the text shown.
# Setup bits: 1 = has received a Pokemon (FLAG_SYS_POKEMON_GET) with the lead at 1 HP, 2 = PT-BR language.
import struct as _struct
LAYOUTS={l['id']:l for l in json.loads((game/'data/layouts/layouts.json').read_text())['layouts'] if 'id' in l}
TALKS={}
# Story state per case: (VAR_HOENN_STORY, VAR_HOENN_STARTER); default (0, 0). See scripts/pilot_story.py.
STORY={11:(3,0)}
JOHTO_STORY={}
NO_ANSWERS={}
STOP_AFTER_PROMPT={}
SPOTS={}
def _pilot_texts_all(folder):
 text=(folder/'scripts.inc').read_text(encoding='utf-8')
 return {l:''.join(re.findall(r'\.string "([^"]*)"',b)) for l,b in re.findall(r'^(\w+_Text_\w+)::\n((?:\t\.string "[^"]*"\n?)+)',text,re.M)}
def _pilot_texts(folder):
 text=(folder/'scripts.inc').read_text(encoding='utf-8')
 bodies={l:''.join(re.findall(r'\.string "([^"]*)"',b)) for l,b in re.findall(r'^(\w+_Text_\w+)::\n((?:\t\.string "[^"]*"\n?)+)',text,re.M)}
 # Bound each lookup to one label. The old nested regex backtracked heavily
 # over story branches that contain no msgbox, stalling test discovery.
 shown={}
 for block in re.finditer(r'^(\w+)::\n(.*?)(?=^\w+::?\n|\Z)',text,re.M|re.S):
  message=re.search(r'^\tmsgbox (\w+_Text_\w+)',block[2],re.M)
  if message and message[1] in bodies:shown[block[1]]=bodies[message[1]]
 return shown
_ATTRS={name:[v for (v,) in _struct.iter_unpack('<I',(game/path).read_bytes())] for name,path in re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)',(game/'src/data/tilesets/metatiles.h').read_text())}
_TILESETS={name:_ATTRS[re.search(r'\.metatileAttributes = (\w+)',body)[1]] for name,body in re.findall(r'const struct Tileset (\w+) =\s*\{(.*?)\};',(game/'src/data/tilesets/headers.h').read_text(),re.S)}
MB_COUNTER=0x80
def _spot(events,collision,behavior,width,height,x,y,north_only,elev=None,want=0):
 """A free cell facing the event; across a counter (MB_COUNTER) the player talks from the far side."""
 taken={(o['x'],o['y']) for o in events}
 options=[(0,1,'UP'),(0,-1,'DOWN'),(-1,0,'RIGHT'),(1,0,'LEFT')]
 for dx,dy,key in options[:1] if north_only else options:
  for reach in (1,2):
   sx,sy=x+dx*reach,y+dy*reach
   if not (0<=sx<width and 0<=sy<height):break
   if reach==2 and behavior[(y+dy)*width+x+dx]!=MB_COUNTER:break
   if elev and want and elev[sy*width+sx] not in (0,want):continue  # e.g. water (1) beside a land NPC: unreachable without Surf
   if collision[sy*width+sx]==0 and (sx,sy) not in taken:return sx,sy,key
 return None
for folder in sorted(list((game/'data/maps').glob('Hoenn_*'))+list((game/'data/maps').glob('Johto_*'))):
 header=json.loads((folder/'map.json').read_text());layout=LAYOUTS[header['layout']]
 objs=header.get('object_events') or [];bgs=header.get('bg_events') or []
 if not objs and not bgs:continue
 blocks=[v for (v,) in _struct.iter_unpack('<H',(game/layout['blockdata_filepath']).read_bytes())]
 cells=[(v>>10)&3 for v in blocks];elevs=[v>>12 for v in blocks]
 behavior=[(_TILESETS[layout['primary_tileset']][v&0x3ff] if v&0x3ff<640 else _TILESETS[layout['secondary_tileset']][(v&0x3ff)-640])&0x1ff for v in blocks]
 shown=_pilot_texts(folder)
 targets=[('object',i,o) for i,o in enumerate(objs)]+[('sign',i,b) for i,b in enumerate(bgs)]
 for kind,i,e in targets:
  spot=_spot(objs,cells,behavior,layout['width'],layout['height'],e['x'],e['y'],kind=='sign' and e.get('player_facing_dir')=='BG_EVENT_PLAYER_FACING_NORTH',elevs,(int(e.get('elevation',0)) or elevs[e['y']*layout['width']+e['x']]) if kind=='object' else 0)  # elevation 0 = the tile's
  assert spot,(folder.name,kind,i,'no free cell next to this event')
  sx,sy,key=spot
  SPOTS[(folder.name,kind,i)]=(header['id'],sx,sy,key)
  script=e['script']
  if (folder.name,kind,i)==('Hoenn_Route101','object',2):continue  # Birch's bag: story scene cases below
  if folder.name=='Johto_NewBarkTown_Lab' and kind=='object' and i in (1,3,4,5):continue  # story cases below
  inc=(folder/'scripts.inc').read_text(encoding='utf-8')
  body_=inc.split(script+'::')[-1].split('\n\n')[0] if script+'::' in inc else ''
  if 'SWIMMER' in e.get('graphics_id',''):continue  # on water: needs Surf to reach
  if 'trainerbattle' in body_:
   args=re.search(r'(trainerbattle_\w+) \w+, ([^\n]*)',body_)
   # Talk cases start with the region trainers beaten (PilotSetup), so A shows the post-battle line;
   # otherwise a trainer would spot the player on load. Battles are tested on their own.
   intro=re.search(r'msgbox (\w+)',body_)[1]
   CASES.append((header['id'],sx,sy,0,0,[(key,'TALK')],0))
   TALKS[len(CASES)]={'map':folder.name,'kind':kind,'index':i,'script':script,'what':'text','setup':0,'expected':_pilot_texts_all(folder).get(intro)}
   continue
  if 'EventScript_PkmnCenterNurse' in body_:  # FireRed nurse in the region Pokemon Centers: heals a 1 HP lead
   CASES.append((header['id'],sx,sy,0,0,[(key,'TALK')],1))
   TALKS[len(CASES)]={'map':folder.name,'kind':kind,'index':i,'script':script,'what':'heal','setup':1,'expected':None}
   continue
  if 'pokemart ' in body_:  # FireRed clerk: the shop opens
   CASES.append((header['id'],sx,sy,0,0,[(key,'TALK')],0))
   TALKS[len(CASES)]={'map':folder.name,'kind':kind,'index':i,'script':script,'what':'started','setup':0,'expected':None}
   continue
  if script.startswith('EventScript_Poke'):  # FireRed's own Pokemon Center / Mart signs
   CASES.append((header['id'],sx,sy,0,0,[(key,'TALK')],0))
   TALKS[len(CASES)]={'map':folder.name,'kind':kind,'index':i,'script':script,'what':'started','setup':0,'expected':None}
   continue
  if 'finditem' in body_ and e.get('flag')!='FLAG_HIDE_JOHTO_ROUTE29_POTION':  # item balls: the script starts; Route 29's Potion is checked in full below
   CASES.append((header['id'],sx,sy,0,0,[(key,'TALK')],0))
   TALKS[len(CASES)]={'map':folder.name,'kind':kind,'index':i,'script':script,'what':'started','setup':0,'expected':None}
   continue
  what='silent' if script=='0x0' or (folder.name,kind,i)==('Hoenn_LittlerootTown_ProfessorBirchsLab','object',1) else 'cuttree' if script=='EventScript_CutTree' else 'item' if 'finditem' in inc.split(script+'::')[-1].split('\n\n')[0] else 'heal' if f'{script}_Heal' in inc else 'text'
  setups=[0]+([1] if what=='heal' else [])
  for setup in setups:
   CASES.append((header['id'],sx,sy,0,0,[(key,'TALK')],setup))
   TALKS[len(CASES)]={'map':folder.name,'kind':kind,'index':i,'script':script,'what':what,'setup':setup,'expected':shown.get(script) or shown.get(script+'_Busy')}  # story NPCs branch to their default line
# Brendan's mother rests the party (step 4), like Emerald's MomHealsParty branch.
for case,info in list(TALKS.items()):
 if (info['map'],info['kind'],info['index'],info['setup'])==('Hoenn_LittlerootTown_BrendansHouse_1F','object',0,0):
  m,sx,sy,_,_,steps,_=CASES[case-1];CASES.append((m,sx,sy,0,0,steps,1));TALKS[len(CASES)]=dict(info,setup=1,what='heal')
# Two PT-BR readings: a Hoenn sign and a Johto townsperson.
for case,info in list(TALKS.items()):
 if (info['map'],info['kind'],info['index']) in {('Hoenn_LittlerootTown','sign',1),('Johto_NewBarkTown','object',1)} and info['setup']==0:
  m,sx,sy,_,_,steps,_=CASES[case-1];CASES.append((m,sx,sy,0,0,steps,2));TALKS[len(CASES)]=dict(info,setup=2)
# SEAGALLOP access (setup bit 4 = first Hall of Fame done, bit 8 = sail Vermilion -> New Bark directly).
GAME_CLEAR,DIRECT_NEW_BARK=4,8
CASES.append(('MAP_VERMILION_CITY',23,30,0,0,[('LEFT','TALK')],0))
TALKS[len(CASES)]={'map':'VermilionCity','kind':'object','index':'region sailor','script':'VermilionCity_EventScript_RegionSailor','what':'silent','setup':0,'expected':None}
CASES.append(('MAP_VERMILION_CITY',23,30,0,0,[('LEFT','TALK')],GAME_CLEAR))
_vermilion=(game/'data/maps/VermilionCity/text.inc').read_text(encoding='utf-8')
TALKS[len(CASES)]={'map':'VermilionCity','kind':'object','index':'region sailor','script':'VermilionCity_EventScript_RegionSailor','what':'text','setup':GAME_CLEAR,
 'expected':''.join(re.findall(r'\.string "([^"]*)"',_vermilion.split('VermilionCity_Text_RegionSailorLittleroot::')[1].split('\n\n')[0]))}
TRIPS={}
for start,sx,sy,key,setup,dest in [('MAP_VERMILION_CITY',23,30,'LEFT',GAME_CLEAR,'littleroot'),('MAP_VERMILION_CITY',23,30,'LEFT',GAME_CLEAR|DIRECT_NEW_BARK,'newbark'),
                                   (HOENN+'LITTLEROOT_TOWN',10,14,'UP',GAME_CLEAR,'vermilion'),(JOHTO+'NEW_BARK_TOWN',24,13,'UP',GAME_CLEAR,'vermilion')]:
 CASES.append((start,sx,sy,0,0,[(key,'TRIP')],setup));TRIPS[len(CASES)]=(start,dest)
# Hoenn story (pilot_story.py). Setup bit 16 = strong lead (Mewtwo with Psychic), SET battle style, fast
# text, no animations and no Exp. Share, so A-pressing wins the battles quickly.
STRONG=16
def _text(folder,label):
 body=(game/'data/maps'/folder/'scripts.inc').read_text(encoding='utf-8').split(f'\n{label}::\n')[1].split('\n\n')[0]
 return ''.join(re.findall(r'\.string "([^"]*)"',body))
SCENES={}
def _scene(map_,x,y,key,story,starter,expect):
 CASES.append((map_,x,y,0,0,[(key,'SCENE')],STRONG|(128 if 'trainer' in expect else 0)));STORY[len(CASES)]=(story,starter);SCENES[len(CASES)]=expect
_scene(HOENN+'ROUTE101',10,18,'DOWN',0,0,{'name':'rescue starts','story':1,'pos':(10,15),'party':1})
_scene(HOENN+'ROUTE101',10,17,'DOWN',1,0,{'name':'Birch keeps the player close','story':1,'pos':(10,17),'party':1})
_bag=SPOTS[('Hoenn_Route101','object',2)]
_scene(_bag[0],_bag[1],_bag[2],_bag[3],1,0,{'name':'bag: Treecko, then Zigzagoon','story':3,'starter':0,'party':2,'gift':'SPECIES_TREECKO','wild':('SPECIES_ZIGZAGOON',84),'hidden':1})
_scene(_bag[0],_bag[1],_bag[2],_bag[3],2,0,{'name':'bag after a blackout: battle again','story':3,'party':1,'wild':('SPECIES_ZIGZAGOON',84),'hidden':1})
_may=SPOTS[('Hoenn_LittlerootTown_MaysHouse_2F','object',0)]
for _starter,_trainer in ((0,'TRAINER_MAY_2'),(1,'TRAINER_MAY_3'),(2,'TRAINER_MAY')):
 _scene(_may[0],_may[1],_may[2],_may[3],3,_starter,{'name':f'May vs starter {_starter}','story':4,'trainer':_trainer})
def _talk_story(where,story,expected,what='text'):
 m,sx,sy,key=SPOTS[where];CASES.append((m,sx,sy,0,0,[(key,'TALK')],0));STORY[len(CASES)]=(story,0)
 TALKS[len(CASES)]={'map':where[0],'kind':where[1],'index':where[2],'script':'story','what':what,'setup':0,'expected':expected,'story':story}
LAB='Hoenn_LittlerootTown_ProfessorBirchsLab'
_talk_story((LAB,'object',0),3,_text(LAB,LAB+'_Text_BirchIsntOneForDeskWork'))
_talk_story((LAB,'object',1),3,_text(LAB,LAB+'_Text_MightBeGoodIdeaToGoSeeRival'))
_talk_story((LAB,'object',1),4,_text(LAB,LAB+'_Text_CountlessPokemonAwait'))
_talk_story(('Hoenn_LittlerootTown_MaysHouse_2F','object',0),4,_text('Hoenn_LittlerootTown_MaysHouse_2F','Hoenn_LittlerootTown_MaysHouse_2F_Text_MayTimeToHeadBack'))
_talk_story(('Hoenn_Route101','object',2),3,None,'silent')  # the bag is gone after the rescue
# Exercise real yes/no branches, including all gifts and full storage.
FULL_PARTY,FULL_PC=32,64
def _johto_scene(index,state,starter,expect,setup=0,no_answers=0):
 m,x,y,key=SPOTS[('Johto_NewBarkTown_Lab','object',index)]
 CASES.append((m,x,y,0,0,[(key,'SCENE')],STRONG|setup|(128 if 'trainer' in expect else 0)))
 JOHTO_STORY[len(CASES)]=(state,starter)
 SCENES[len(CASES)]=dict(expect,johto=True)
 NO_ANSWERS[len(CASES)]=no_answers
 if no_answers:STOP_AFTER_PROMPT[len(CASES)]=1
_johto_scene(1,0,0,{'name':'Elm request accepted','story':1,'party':1})
_johto_scene(1,0,0,{'name':'Elm request refused','story':0,'party':1},no_answers=1)
_johto_scene(3,0,0,{'name':'ball before Elm request','story':0,'party':1})
for choice,species in enumerate(('CHIKORITA','CYNDAQUIL','TOTODILE')):
 _johto_scene(3+choice,1,0,{'name':f'Elm gift {species}','story':2,'starter':choice,'party':2,'gift':'SPECIES_'+species,'ballflags':1<<choice})
_johto_scene(3,1,0,{'name':'Elm gift declined','story':1,'party':1,'ballflags':0},no_answers=1)
_johto_scene(3,1,0,{'name':'Elm PT-BR category','story':1,'party':1,'category':'FOLHA'},setup=2,no_answers=1)
_johto_scene(4,2,0,{'name':'second Johto starter refused','story':2,'starter':0,'party':1})
_johto_scene(5,1,0,{'name':'Elm gift sent to PC','story':2,'starter':2,'party':6,'boxed':'SPECIES_TOTODILE','ballflags':4},setup=FULL_PARTY)
_johto_scene(4,1,0,{'name':'Elm full storage preserves choice','story':1,'starter':0,'party':6,'ballflags':0},setup=FULL_PARTY|FULL_PC)
_johto_scene(1,2,0,{'name':'Elm research after gift','story':2,'party':1})
# Elm's aide (step 4): Potion once the starter is given, Poke Balls once Silver is beaten.
AIDE_POTION_GIVEN=4096  # 1024 = Kyogre fixture, 2048 = Littleroot visited
_johto_scene(0,1,0,{'name':'aide before the starter','story':1,'gifts':(0,0,0,0)})
_johto_scene(0,2,0,{'name':'aide gives the Potion','story':2,'gifts':(1,0,1,0)})
_johto_scene(0,2,0,{'name':'aide waits for Silver','story':2,'gifts':(0,0,1,0)},setup=AIDE_POTION_GIVEN)
_johto_scene(0,3,0,{'name':'aide gives the Poke Balls','story':3,'gifts':(0,1,1,1)},setup=AIDE_POTION_GIVEN)
_johto_scene(3,1,0,{'name':'Johto gift save reload','story':2,'starter':0,'party':2,'gift':'SPECIES_CHIKORITA','persist':True},setup=512)
for choice,trainer in enumerate(('TRAINER_BRENDAN','TRAINER_BRENDAN_2','TRAINER_BRENDAN_3')):
 m,x,y,key=SPOTS[('Johto_NewBarkTown','object',2)]
 CASES.append((m,x,y,0,0,[(key,'SCENE')],STRONG|128|(1024 if choice==2 else 0)))
 JOHTO_STORY[len(CASES)]=(2,choice)
 SCENES[len(CASES)]={'name':f'Silver vs starter {choice}','johto':True,'story':3,'trainer':trainer}
# Losing does not advance the rival story; this is distinct from a victory fixture.
_scene(_may[0],_may[1],_may[2],_may[3],3,0,{'name':'May defeat keeps retry state','story':3,'trainer':'TRAINER_MAY_2','outcome':2})
CASES[-1]=(*CASES[-1][:-1],CASES[-1][-1]|256)
m,x,y,key=SPOTS[('Johto_NewBarkTown','object',2)]
CASES.append((m,x,y,0,0,[(key,'SCENE')],STRONG|128|256));JOHTO_STORY[len(CASES)]=(2,0)
SCENES[len(CASES)]={'name':'Silver defeat keeps retry state','johto':True,'story':2,'trainer':'TRAINER_BRENDAN','outcome':2}
# Hoenn's previously untested alternatives use the same actual input driver.
for choice,species in ((1,'TORCHIC'),(2,'MUDKIP')):
 _scene(_bag[0],_bag[1],_bag[2],_bag[3],1,0,{'name':f'bag: {species}','story':3,'starter':choice,'party':2,'gift':'SPECIES_'+species})
 NO_ANSWERS[len(CASES)]=(1<<choice)-1
_scene(_bag[0],_bag[1],_bag[2],_bag[3],1,0,{'name':'bag: refuse all starters','story':1,'party':1})
NO_ANSWERS[len(CASES)]=7
STOP_AFTER_PROMPT[len(CASES)]=3
# Geography batch trainers: one battle per trainer, from the talk cell, with only that trainer unbeaten
# (FRESH_TRAINER). The scene presses A through intro, battle and post-battle line; the strong lead wins.
# Swimmers stand on water (Surf) and are left out, like their talk cases.
FRESH_TRAINER={}
# Fixed-damage move for the strong lead (level 100 = 100 HP a hit, no type chart): one move cannot be
# blocked by every route team otherwise (Dark vs Psychic, Fairy vs Dragon, Water Absorb, Water/Ice vs Ice).
# Ghosts are immune to Seismic Toss, so a team with one gets Night Shade.
FRESH_MOVE={}
_types={sp:tuple(re.findall(r'TYPE_\w+',t)) for sp,t in re.findall(r'\[(SPECIES_\w+)\] =\s*\{.*?\.types = \{([^}]*)\}',(game/'src/data/pokemon/species_info.h').read_text(),re.S)}
_pilot_teams=json.loads((game/'hack-data/region_pilot_trainers.json').read_text(encoding='utf-8'))['trainers']
def _trainer_ids(text):
 """TRAINER_ name -> number, with the aliases of geography trainers in reused slots resolved."""
 ids=dict(re.findall(r'#define (TRAINER_\w+)\s+(\d+)\b',text))
 for alias,target in re.findall(r'#define (TRAINER_\w+)\s+(TRAINER_\w+)\b',text):ids[alias]=ids[target]
 return ids
_opponent_ids=_trainer_ids((game/'include/constants/opponents.h').read_text())
_fought=set()
for _folder in sorted(list((game/'data/maps').glob('Hoenn_*'))+list((game/'data/maps').glob('Johto_*'))):
 _objs=json.loads((_folder/'map.json').read_text()).get('object_events') or []
 _inc=(_folder/'scripts.inc').read_text(encoding='utf-8')
 for _i,_o in enumerate(_objs):
  _body=_inc.split(_o['script']+'::')[-1].split('\n\n')[0] if _o['script']+'::' in _inc else ''
  _m=re.search(r'trainerbattle_\w+ (TRAINER_(?:HOENN|JOHTO)_\w+)',_body)
  if not _m or 'SWIMMER' in _o['graphics_id'] or _m[1] in _fought:continue
  _fought.add(_m[1])
  _sm,_sx,_sy,_key=SPOTS[(_folder.name,'object',_i)]
  _johto=_folder.name.startswith('Johto_')
  # Strong Mewtwo; PilotSetup swaps Psychic for Ice Beam in these cases (see gPilotFreshTrainer): every
  # other single move hits an immunity in some route team (Dark Poochyena, Fairy Marill, Water Absorb Poliwag).
  CASES.append((_sm,_sx,_sy,0,0,[(_key,'SCENE')],STRONG))
  (JOHTO_STORY if _johto else STORY)[len(CASES)]=(0,0)
  SCENES[len(CASES)]={'name':f'battle {_m[1]}','johto':_johto,'story':0,'trainer':_m[1],'outcome':1,'route_trainer':True}
  FRESH_TRAINER[len(CASES)]=int(_opponent_ids[_m[1]])
  _team=[m[0] if isinstance(m,list) else m['species'] for m in _pilot_teams[_m[1]]['team']]
  FRESH_MOVE[len(CASES)]='MOVE_NIGHT_SHADE' if any('TYPE_GHOST' in _types.get(sp,()) for sp in _team) else 'MOVE_SEISMIC_TOSS'
# Sail to New Bark through the sailor's two questions, not a direct script call.
CASES.append(('MAP_VERMILION_CITY',23,30,0,0,[('LEFT','TRIP')],GAME_CLEAR))
TRIPS[len(CASES)]=('MAP_VERMILION_CITY','newbark');NO_ANSWERS[len(CASES)]=1
# Continuous journey (M7): one case starts in Vermilion after the League and plays the whole pilot story
# with simulated input only: the ferries, both towns, the routes, every scene and battle, and back.
# Walks are planned here with a BFS over each map's collision and metatile behaviours; the ROM only
# replays the keys (holding through NPCs that wander into the way) and presses A/B in dialogue.
from collections import deque
_BLOCK_BEHAVIORS=set(range(0x10,0x1C))-{0x17}|set(range(0x30,0x3C))|set(range(0x50,0x54))|set(range(0x60,0x70))|{0x80,0xD2}
def _walk_grid(folder,coords=True):
 header=json.loads((game/'data/maps'/folder/'map.json').read_text());layout=LAYOUTS[header['layout']]
 blocks=[v for (v,) in _struct.iter_unpack('<H',(game/layout['blockdata_filepath']).read_bytes())]
 w,h=layout['width'],layout['height']
 def behavior(v):
  t=v&0x3ff
  return (_TILESETS[layout['primary_tileset']][t] if t<640 else _TILESETS[layout['secondary_tileset']][t-640])&0x1ff
 blocked={(i%w,i//w) for i,v in enumerate(blocks) if (v>>10)&3 or behavior(v) in _BLOCK_BEHAVIORS}
 blocked|={(o['x'],o['y']) for o in header.get('object_events') or []}
 blocked|={(e['x'],e['y']) for e in header.get('warp_events') or []}
 if coords:blocked|={(e['x'],e['y']) for e in header.get('coord_events') or []}  # triggers, unless inactive
 return w,h,blocked
JDIRS={'UP':(0,-1),'DOWN':(0,1),'LEFT':(-1,0),'RIGHT':(1,0)}
def _path(folder,start,goal,coords=True):
 w,h,blocked=_walk_grid(folder,coords);blocked-={start,goal}
 prev={start:None};q=deque([start])
 while q:
  c=q.popleft()
  if c==goal:break
  for d,(dx,dy) in JDIRS.items():
   n=(c[0]+dx,c[1]+dy)
   if 0<=n[0]<w and 0<=n[1]<h and n not in blocked and n not in prev:prev[n]=(c,d);q.append(n)
 assert goal in prev,(folder,'no path',start,goal)
 steps=[];c=goal
 while prev[c]:n=c;c,d=prev[c];steps.append((d,n))
 return steps[::-1]  # (direction, cell reached)
J_WALK,J_FACE,J_TALK,J_CHECK=1,2,3,4
JOURNEY=[];JOURNEY_CHECKS=[]
# Ops are (op, arg, x, y): a walk carries the cell it must reach; 255 = done on leaving the map.
def j_walk(folder,start,goal,coords=True):JOURNEY.extend((J_WALK,d,x,y) for d,(x,y) in _path(folder,start,goal,coords))
def j_step(d):JOURNEY.append((J_WALK,d,255,255))      # a step that leaves the map (door, edge, stairs)
def j_talk(face,no_mask=0):JOURNEY.extend([(J_FACE,face,0,0),(J_TALK,no_mask,0,0)])
def j_check(name):JOURNEY.append((J_CHECK,len(JOURNEY_CHECKS),0,0));JOURNEY_CHECKS.append(name)
H,J='Hoenn_','Johto_'
# Vermilion: the region sailor, YES to Littleroot.
j_talk('LEFT');j_check('ferry to Littleroot')
# Littleroot -> Route 101: the rescue starts on the first step into the route.
j_walk(H+'LittlerootTown',(11,14),(10,0));j_step('UP');j_check('rescue started')
j_walk(H+'Route101',(10,15),(7,15));j_talk('UP');j_check('Birch rescued')
j_walk(H+'Route101',(7,15),(10,19),coords=False);j_step('DOWN')  # rescue triggers are off after it
# Birch's lab, then May's room (battle), then home to the ferry.
j_walk(H+'LittlerootTown',(10,0),(7,17));j_step('UP')
j_walk(H+'LittlerootTown_ProfessorBirchsLab',(6,12),(6,5));j_talk('UP');j_check('Birch in his lab')
j_walk(H+'LittlerootTown_ProfessorBirchsLab',(6,5),(6,12));j_step('DOWN')
j_walk(H+'LittlerootTown',(7,17),(14,9));j_step('UP')
j_walk(H+'LittlerootTown_MaysHouse_1F',(2,8),(2,3));j_step('UP')
_may=SPOTS[('Hoenn_LittlerootTown_MaysHouse_2F','object',0)]
j_walk(H+'LittlerootTown_MaysHouse_2F',(1,2),_may[1:3]);j_talk(_may[3]);j_check('May beaten')
j_walk(H+'LittlerootTown_MaysHouse_2F',_may[1:3],(1,3));j_step('UP')
j_walk(H+'LittlerootTown_MaysHouse_1F',(2,3),(2,8));j_step('DOWN')
j_walk(H+'LittlerootTown',(14,9),(10,14));j_talk('UP');j_check('back in Vermilion')
# Vermilion: NO to Littleroot, YES to New Bark.
j_walk('VermilionCity',(23,32),(23,30));j_talk('LEFT',1);j_check('ferry to New Bark')
# Elm's lab: request, Chikorita, the aide's Potion.
j_walk(J+'NewBarkTown',(24,13),(10,10));j_step('UP')
j_walk(J+'NewBarkTown_Lab',(6,12),(6,4));j_talk('UP');j_check('Elm request')
j_walk(J+'NewBarkTown_Lab',(6,4),(8,5));j_talk('UP');j_check('Chikorita received')
_aide=SPOTS[('Johto_NewBarkTown_Lab','object',0)]
j_walk(J+'NewBarkTown_Lab',(8,5),_aide[1:3]);j_talk(_aide[3]);j_check('aide Potion')
j_walk(J+'NewBarkTown_Lab',_aide[1:3],(6,12));j_step('DOWN')
# Silver, Route 29 and back, the ferry home.
_silver=SPOTS[('Johto_NewBarkTown','object',2)]
j_walk(J+'NewBarkTown',(10,10),_silver[1:3]);j_talk(_silver[3]);j_check('Silver beaten')
j_walk(J+'NewBarkTown',_silver[1:3],(0,12));j_step('LEFT');j_step('RIGHT');j_check('Route 29 and back')
j_walk(J+'NewBarkTown',(0,12),(24,13));j_talk('UP');j_check('home in Vermilion')
assert len(JOURNEY_CHECKS)<=14 and len(JOURNEY)<1000,(len(JOURNEY_CHECKS),len(JOURNEY))
JOURNEY_SETUP=16384  # region sailor shown (bit 4) + strong team (16|128) are added below; 16384 = long Repel
CASES.append(('MAP_VERMILION_CITY',23,30,0,0,[('NONE','JOURNEY')],4|16|128|JOURNEY_SETUP));JOURNEY_CASE=len(CASES)
CASES=[c if len(c)==7 else (*c,0) for c in CASES]
KEYS={'UP':'DPAD_UP','DOWN':'DPAD_DOWN','LEFT':'DPAD_LEFT','RIGHT':'DPAD_RIGHT','NONE':'0'}
MODES={'MAP':0,'POS':1,'HOLD':2,'BATTLE':3,'TOWNMAP':4,'TALK':5,'TRIP':6,'SCENE':7,'FLY':8,'SWITCH':9,'CONTINUE':10,'JOURNEY':11}
spawns=',\n    '.join(f'{{MAP_GROUP({m}), MAP_NUM({m}), {x}, {y}, {px}, {py}, {5 if any(mode=="BATTLE" for _,mode in s) else 100}, {setup}, {STORY.get(n,(0,0))[0]}, {STORY.get(n,(0,0))[1]}, {JOHTO_STORY.get(n,(0,0))[0]}, {JOHTO_STORY.get(n,(0,0))[1]}}}' for n,(m,x,y,px,py,s,setup) in enumerate(CASES,1))
answers=', '.join(str(NO_ANSWERS.get(n,0)) for n in range(1,len(CASES)+1))
stop_prompts=', '.join(str(STOP_AFTER_PROMPT.get(n,0)) for n in range(1,len(CASES)+1))
fresh_trainers=', '.join(str(FRESH_TRAINER.get(n,0)) for n in range(1,len(CASES)+1))
_move_ids=dict(re.findall(r'#define (MOVE_\w+) (\d+)',(game/'include/constants/moves.h').read_text()))  # main.c has no move constants
fresh_moves=', '.join(_move_ids[FRESH_MOVE[n]] if n in FRESH_MOVE else '0' for n in range(1,len(CASES)+1))
_pilot=json.loads((game/'hack-data/region_pilot_trainers.json').read_text(encoding='utf-8'))
slot_trainers=', '.join(sorted({str(_opponent_ids[t]) for t,e in _pilot['trainers'].items() if e.get('slot')})+['0'])
steps=',\n    '.join('{'+', '.join(f'{{{KEYS[k]}, {MODES[mode]}}}' for k,mode in s)+'}' if s else '{{0, 0}}' for *_,s,_setup in CASES)
counts=', '.join(str(len(s)) for *_,s,_setup in CASES)
journey=', '.join(f'{{{op}, {KEYS[a] if isinstance(a,str) else a}, {x}, {y}}}' for op,a,x,y in JOURNEY)
hook='''
#include "overworld.h"
#include "fieldmap.h"
#include "palette.h"
#include "script.h"
#include "field_player_avatar.h"
#include "constants/maps.h"
#include "string_util.h"
#include "wild_encounter.h"
#include "region_map.h"
#include "field_weather.h"
#include "constants/region_map_sections.h"
#include "pokemon.h"
#include "event_object_movement.h"
#include "event_data.h"
#include "item.h"
#include "constants/items.h"
#include "battle.h"
#include "battle_main.h"
#include "party_menu.h"
#include "data.h"
#include "constants/trainers.h"
#include "pokemon_storage_system.h"
#include "save.h"
bool8 PilotYesNoActive(void);
extern const struct Tileset gTileset_General, gTileset_HoennPilotGeneral, gTileset_HoennPilotPetalburg;
extern const struct Tileset gTileset_JohtoPilotGeneral, gTileset_JohtoPilotNewBarkTown, gTileset_HoennPilotBuilding;
void CB2_NewGame(void);
void CB1_Overworld(void);
void CleanupOverworldWindowsAndTilemaps(void);
void CB2_ReturnToField(void);
extern const u8 EventScript_SailToDest[];
#include "constants/seagallop.h"
struct PilotStep { u16 keys; u8 mode; };
const s16 gPilotSpawn[][12] = {
    SPAWNS
};
static const struct PilotStep sPilotSteps[][4] = {
    STEPS
};
static const u8 sPilotStepCount[] = {COUNTS};
static const u8 sPilotNoAnswers[] = {ANSWERS};
static const u8 sPilotStopPrompts[] = {STOP_PROMPTS};
u16 GetTrainerFlagId(u16 trainerId);
const u16 gPilotFreshTrainer[] = {FRESH_TRAINERS};  // route trainer left unbeaten in this case (0: none)
const u16 gPilotFreshMove[] = {FRESH_MOVES};  // the strong lead's move in those battle cases
const u16 gPilotSlotTrainers[] = {SLOT_TRAINERS};  // geography trainers in reused FireRed slots, 0-terminated
extern volatile u32 gPilotCase;
EWRAM_DATA static u8 sPilotPrompt = 0;
EWRAM_DATA static bool8 sPilotWasYesNo = FALSE;
static u16 PilotDialogueKey(void)
{
    bool8 active = PilotYesNoActive();
    if (active && !sPilotWasYesNo) sPilotPrompt++;
    sPilotWasYesNo = active;
    // Once a terminal refusal is sent, B advances closing text without talking
    // to the object again on the frame field controls become available.
    if (!active && sPilotStopPrompts[gPilotCase - 1] && sPilotPrompt >= sPilotStopPrompts[gPilotCase - 1])
        return B_BUTTON;
    if (active && sPilotPrompt <= 8 && (sPilotNoAnswers[gPilotCase - 1] & (1 << (sPilotPrompt - 1))))
        return B_BUTTON;
    return A_BUTTON;
}
EWRAM_DATA volatile u32 gPilotCase = 0, gPilotStage = 0, gPilotShot = 0;
// Journey ops: {op, arg}. 1 walk one tile (arg: key), 2 face (key), 3 talk (arg: prompts answered NO,
// bit n = n-th yes/no), 4 checkpoint (arg: slot).
static const u16 sJourney[][4] = {JOURNEY_OPS};
EWRAM_DATA static u16 sJourneyOp = 0, sJourneyFrames = 0;
EWRAM_DATA static u8 sJourneyPhase = 0, sJourneyPrompt = 0, sJourneyGroup = 0, sJourneyMap = 0;
EWRAM_DATA static bool8 sJourneyWasYesNo = FALSE;
EWRAM_DATA static s16 sJourneyX = 0, sJourneyY = 0;
static u16 JourneyDialogueKey(u16 noMask)
{
    bool8 active = PilotYesNoActive();
    if (active && !sJourneyWasYesNo) sJourneyPrompt++;
    sJourneyWasYesNo = active;
    if (active && sJourneyPrompt <= 8 && (noMask & (1 << (sJourneyPrompt - 1))))
        return B_BUTTON;
    return A_BUTTON;
}
EWRAM_DATA volatile u16 gPilotKeys = 0;
EWRAM_DATA u8 gPilotResult[128] = {0};
EWRAM_DATA static u32 sPilotFrames = 0, sPilotFlower = 0, sPilotIdle = 0, sPilotStepFrames = 0;
EWRAM_DATA static u8 sPilotStep = 0, sPilotPhase = 0, sPilotStartGroup = 0, sPilotStartMap = 0;
EWRAM_DATA static MainCallback sPilotFieldCallback = NULL;
EWRAM_DATA static s16 sPilotX = 0, sPilotY = 0, sPilotStartX = 0, sPilotStartY = 0;
static bool8 PilotIsTalkCase(void)
{
    return sPilotStepCount[gPilotCase - 1] && sPilotSteps[gPilotCase - 1][0].mode == 5;
}
static void PilotPut16(u8 *t, u16 v) { t[0] = v; t[1] = v >> 8; }
// Called right after New Game loads the map, before any object moves.
void PilotAfterLoad(void)
{
    if (PilotIsTalkCase())
        FreezeObjectEventsExceptOne(gPlayerAvatar.objectEventId);
}
// The warp fade-in unfreezes objects, and a walker can start a step in that same frame. Until A is
// pressed, talk cases ignore that unfreeze (test copy only); the event script's release then works.
bool8 PilotKeepFrozen(void)
{
    return gPilotCase && PilotIsTalkCase() && sPilotPhase < 6;
}
static u32 PilotFlowerSum(void)
{
    const u32 *tile = (const u32 *)(BG_VRAM + 508 * 32);
    u32 i, sum = 0;
    for (i = 0; i < 32; i++) sum = sum * 31 + tile[i];
    return sum;
}
static void PilotDataCase(void)
{
    u8 *r = gPilotResult;
    const s16 *spawn = gPilotSpawn[gPilotCase - 1];
    s16 px = spawn[4] + MAP_OFFSET, py = spawn[5] + MAP_OFFSET;
    const struct MapLayout *layout = gMapHeader.mapLayout;
    sPilotFrames++;
    if (sPilotFrames == 60) { sPilotFlower = PilotFlowerSum(); gPilotShot = 1; }
    if (sPilotFrames == 76)
    {
        r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
        r[2] = gMapHeader.mapLayoutId; r[3] = gMapHeader.mapLayoutId >> 8;
        r[4] = (layout->primaryTileset == &gTileset_HoennPilotGeneral) | (layout->secondaryTileset == &gTileset_HoennPilotPetalburg) << 1
             | (layout->primaryTileset == &gTileset_General) << 2
             | (layout->primaryTileset == &gTileset_JohtoPilotGeneral) << 3 | (layout->secondaryTileset == &gTileset_JohtoPilotNewBarkTown) << 4;
        r[7] = gMapHeader.connections ? gMapHeader.connections->count : 0;
        if (r[7]) { r[8] = gMapHeader.connections->connections[0].direction; r[9] = gMapHeader.connections->connections[0].mapGroup; r[10] = gMapHeader.connections->connections[0].mapNum; }
        r[11] = MapGridGetMetatileIdAt(px, py); r[12] = MapGridGetMetatileIdAt(px, py) >> 8;
        r[13] = MapGridGetMetatileBehaviorAt(px, py);
        r[14] = MapGridGetCollisionAt(px, py);
        r[15] = gSaveBlock1Ptr->pos.x; r[16] = gSaveBlock1Ptr->pos.y;
        r[17] = PilotFlowerSum() != sPilotFlower;
        r[24] = MapGridGetCollisionAt(spawn[2] + MAP_OFFSET, spawn[3] + MAP_OFFSET);
        r[25] = gMapHeader.regionMapSectionId;
        r[37] = gMapHeader.showMapName; r[38] = gMapHeader.regionMapSectionId;
        r[39] = GetMonData(&gPlayerParty[0], MON_DATA_MET_LOCATION);
        GetMapNameGeneric(r + 40, gMapHeader.regionMapSectionId); r[63] = EOS;
        {
            const struct WildPokemonHeader *h = gWildMonHeaders;
            while (h->mapGroup != MAP_GROUP(MAP_UNDEFINED) && !(h->mapGroup == r[0] && h->mapNum == r[1])) h++;
            if (h->mapGroup != MAP_GROUP(MAP_UNDEFINED))
            {
                r[26] = (h->landMonsInfo != NULL) | (h->waterMonsInfo != NULL) << 1 | (h->rockSmashMonsInfo != NULL) << 2 | (h->fishingMonsInfo != NULL) << 3;
                if (h->landMonsInfo) { r[27] = h->landMonsInfo->wildPokemon[0].species; r[28] = h->landMonsInfo->wildPokemon[0].species >> 8; r[33] = h->landMonsInfo->encounterRate; }
                if (h->waterMonsInfo) { r[29] = h->waterMonsInfo->wildPokemon[0].species; r[30] = h->waterMonsInfo->wildPokemon[0].species >> 8; r[34] = h->waterMonsInfo->encounterRate; }
                if (h->fishingMonsInfo) { r[31] = h->fishingMonsInfo->wildPokemon[0].species; r[32] = h->fishingMonsInfo->wildPokemon[0].species >> 8; r[35] = h->fishingMonsInfo->encounterRate; }
            }
        }
        gPilotStage = 3;
    }
}
// Hold one D-pad key per step, then wait until the player is idle and record map and position.
static void PilotWalkCase(void)
{
    const struct PilotStep *step = &sPilotSteps[gPilotCase - 1][sPilotStep];
    u8 *r = gPilotResult + sPilotStep * 4;
    s16 x, y;
    bool8 field = gMain.callback1 == CB1_Overworld && !ArePlayerFieldControlsLocked() && !gPaletteFade.active;
    bool8 moved;
    PlayerGetDestCoords(&x, &y);
    sPilotIdle = field && x == sPilotX && y == sPilotY ? sPilotIdle + 1 : 0;
    sPilotX = x; sPilotY = y;
    sPilotStepFrames++;
    moved = gSaveBlock1Ptr->location.mapGroup != sPilotStartGroup || gSaveBlock1Ptr->location.mapNum != sPilotStartMap;
    // A wild encounter fills gEnemyParty (zeroed by New Game) before the battle transition.
    if (step->mode == 3 && sPilotPhase == 1 && GetMonData(&gEnemyParty[0], MON_DATA_SPECIES))
    {
        u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES);
        r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
        r[2] = species; r[3] = species >> 8; r[4] = GetMonData(&gEnemyParty[0], MON_DATA_LEVEL);
        gPilotKeys = 0; gPilotShot = 1; gPilotStage = 3;
        return;
    }
    // Talk: tap toward the event, press A, read the text, then keep pressing A until the script ends.
    if (step->mode == 5 && sPilotPhase >= 5)
    {
        u8 *t = gPilotResult;
        if (sPilotPhase == 5 && sPilotStepFrames >= 4) { gPilotKeys = A_BUTTON; sPilotPhase = 6; sPilotStepFrames = 0; }
        else if (sPilotPhase == 6 && sPilotStepFrames >= 4) { gPilotKeys = 0; sPilotPhase = 7; sPilotStepFrames = 0; }
        else if (sPilotPhase == 7 && sPilotStepFrames == 60)
        {
            t[0] = gSaveBlock1Ptr->location.mapGroup; t[1] = gSaveBlock1Ptr->location.mapNum;
            t[2] = x - MAP_OFFSET; t[3] = y - MAP_OFFSET; t[4] = ArePlayerFieldControlsLocked();
            memcpy(t + 8, gStringVar4, 120);
            gPilotShot = 1;
        }
        else if (sPilotPhase == 7 && sPilotStepFrames > 60)
        {
            gPilotKeys = (sPilotStepFrames % 20) < 3 ? A_BUTTON : 0;
            if (sPilotStepFrames >= ((gPilotSpawn[gPilotCase - 1][7] & 1) ? 900 : 420))  // heal cases: text, fanfare and fades first
            {
                gPilotKeys = 0;
                t[5] = GetMonData(&gPlayerParty[0], MON_DATA_HP) == GetMonData(&gPlayerParty[0], MON_DATA_MAX_HP);
                t[6] = CheckBagHasItem(ITEM_POTION, 1);
                t[7] = FlagGet(FLAG_HIDE_JOHTO_ROUTE29_POTION) | FlagGet(FLAG_HIDE_HOENN_ROUTE102_POTION) << 1 | FlagGet(FLAG_HIDE_JOHTO_ROUTE32_POTION) << 2;  // any region Potion ball
                gPilotStage = 3;
            }
        }
        return;
    }
    // Scene: hold the key (a step or a turn), press A, then press A only while a script, message or
    // battle runs; done once the player is free and still for 60 frames.
    if (step->mode == 7 && sPilotPhase >= 5)
    {
        u8 *t = gPilotResult;
        bool8 busy = gMain.callback1 != CB1_Overworld || ArePlayerFieldControlsLocked() || gPaletteFade.active || gMain.inBattle;
        if (!t[21] && GetMonData(&gEnemyParty[0], MON_DATA_SPECIES))
        {
            u8 i;
            t[21] = 1; PilotPut16(t + 8, GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)); t[10] = GetMonData(&gEnemyParty[0], MON_DATA_LEVEL);
            PilotPut16(t + 11, gTrainerBattleOpponent_A);
            for (i = 0; i < 4; i++) PilotPut16(t + 13 + i * 2, GetMonData(&gEnemyParty[i], MON_DATA_SPECIES));
            t[44] = (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) != 0;
            t[45] = t[46] = 1;
            // Battle text prints the Kanto rival's name for the RIVAL/CHAMPION classes; pilot rivals keep their own.
            t[47] = gTrainers[gTrainerBattleOpponent_A].trainerClass != TRAINER_CLASS_RIVAL_EARLY
                 && gTrainers[gTrainerBattleOpponent_A].trainerClass != TRAINER_CLASS_RIVAL_LATE
                 && gTrainers[gTrainerBattleOpponent_A].trainerClass != TRAINER_CLASS_CHAMPION;
            memcpy(t + 96, gTrainers[gTrainerBattleOpponent_A].trainerName, 8);
            for (i = 0; i < PARTY_SIZE; i++)
            {
                u16 total = 0; u8 stat;
                PilotPut16(t + 32 + i * 2, GetMonData(&gEnemyParty[i], MON_DATA_SPECIES));
                if (!GetMonData(&gEnemyParty[i], MON_DATA_SPECIES)) continue;
                for (stat = 0; stat < NUM_STATS; stat++)
                {
                    if (GetMonData(&gEnemyParty[i], MON_DATA_HP_IV + stat) != 31) t[45] = 0;
                    total += GetMonData(&gEnemyParty[i], MON_DATA_HP_EV + stat);
                }
                if (total != 508) t[46] = 0;
            }
        }
        if (sPilotPhase == 5 && sPilotStepFrames >= 12) { gPilotKeys = A_BUTTON; sPilotPhase = 6; sPilotStepFrames = 0; }
        else if (sPilotPhase == 6 && sPilotStepFrames >= 4) { gPilotKeys = 0; sPilotPhase = 7; sPilotStepFrames = 0; }
        else if (sPilotPhase == 7)
        {
            u16 key = PilotDialogueKey();
            // A fainted lead opens the party menu on its own slot; point it at a healthy Pokemon (as the journey does).
            if (gMain.inBattle && gMain.callback2 != BattleMainCB2)
            {
                s8 i;
                for (i = gPlayerPartyCount - 1; i >= 0; i--)
                    if (GetMonData(&gPlayerParty[i], MON_DATA_HP) != 0 && i != gBattlerPartyIndexes[0] && i != gBattlerPartyIndexes[2])
                    {
                        gPartyMenu.slotId = i;
                        break;
                    }
            }
            gPilotKeys = busy && (sPilotStepFrames % 8) < 3 ? key : 0;
            if ((sPilotStepFrames > 200 && sPilotIdle >= 60) || sPilotStepFrames >= 38000)
            {
                gPilotKeys = 0;
                t[0] = gSaveBlock1Ptr->location.mapGroup; t[1] = gSaveBlock1Ptr->location.mapNum;
                t[2] = x - MAP_OFFSET; t[3] = y - MAP_OFFSET;
                t[4] = VarGet(VAR_HOENN_STORY); t[5] = VarGet(VAR_HOENN_STARTER); t[6] = gPlayerPartyCount;
                t[7] = FlagGet(FLAG_HIDE_HOENN_ROUTE101_RESCUE);
                PilotPut16(t + 22, GetMonData(&gPlayerParty[1], MON_DATA_SPECIES)); t[24] = GetMonData(&gPlayerParty[1], MON_DATA_LEVEL);
                t[25] = gBattleOutcome; t[26] = sPilotStepFrames >= 38000;
                t[27] = VarGet(VAR_JOHTO_STORY); t[28] = VarGet(VAR_JOHTO_STARTER);
                t[29] = FlagGet(FLAG_HIDE_JOHTO_CHIKORITA_BALL)
                      | FlagGet(FLAG_HIDE_JOHTO_CYNDAQUIL_BALL) << 1 | FlagGet(FLAG_HIDE_JOHTO_TOTODILE_BALL) << 2;
                PilotPut16(t + 30, GetBoxMonDataAt(0, 0, MON_DATA_SPECIES));
                memcpy(t + 64, gStringVar2, 32); t[95] = EOS;
                t[104] = CheckBagHasItem(ITEM_POTION, 1); t[105] = CheckBagHasItem(ITEM_POKE_BALL, 5);
                t[106] = FlagGet(FLAG_GOT_JOHTO_AIDE_POTION); t[107] = FlagGet(FLAG_GOT_JOHTO_AIDE_BALLS);
                t[108] = gPilotFreshTrainer[gPilotCase - 1] && FlagGet(GetTrainerFlagId(gPilotFreshTrainer[gPilotCase - 1]));
                if (gPilotSpawn[gPilotCase - 1][7] & 512)
                {
                    t[48] = TrySavingData(SAVE_NORMAL);
                    VarSet(VAR_JOHTO_STORY, 0); VarSet(VAR_JOHTO_STARTER, 2);
                    FlagClear(FLAG_HIDE_JOHTO_CHIKORITA_BALL);
                    memset(gPlayerParty, 0, sizeof(gPlayerParty)); gPlayerPartyCount = 0;
                    gSaveBlock1Ptr->location.mapGroup = 0; gSaveBlock1Ptr->location.mapNum = 0;
                    t[49] = LoadGameSave(SAVE_NORMAL);
                    t[50] = VarGet(VAR_JOHTO_STORY); t[51] = VarGet(VAR_JOHTO_STARTER);
                    t[52] = FlagGet(FLAG_HIDE_JOHTO_CHIKORITA_BALL); t[53] = gPlayerPartyCount;
                    t[54] = gSaveBlock1Ptr->location.mapGroup; t[55] = gSaveBlock1Ptr->location.mapNum;
                    PilotPut16(t + 56, GetMonData(&gPlayerParty[1], MON_DATA_SPECIES));
                }
                gPilotShot = 1; gPilotStage = 3;
            }
        }
        return;
    }
    // Trip: talk to the SEAGALLOP sailor and keep pressing A (YES is the default) until a new map is up.
    if (step->mode == 6 && sPilotPhase >= 5)
    {
        u8 *t = gPilotResult;
        if (sPilotPhase == 5 && sPilotStepFrames >= 4) { gPilotKeys = A_BUTTON; sPilotPhase = 6; sPilotStepFrames = 0; }
        else if (sPilotPhase == 6 && sPilotStepFrames >= 4) { gPilotKeys = 0; sPilotPhase = 7; sPilotStepFrames = 0; }
        else if (sPilotPhase == 7)
        {
            u16 key = PilotDialogueKey();
            gPilotKeys = (sPilotStepFrames % 20) < 3 ? key : 0;
            if ((moved && sPilotIdle >= 30) || sPilotStepFrames >= 4000)
            {
                gPilotKeys = 0;
                t[0] = gSaveBlock1Ptr->location.mapGroup; t[1] = gSaveBlock1Ptr->location.mapNum;
                t[2] = x - MAP_OFFSET; t[3] = y - MAP_OFFSET; t[4] = moved;
                gPilotShot = 1; gPilotStage = 3;
            }
        }
        return;
    }
    // Town Map: fade, clean up the field and open the region map as the item does, then keep running.
    // Journey: replay the planned ops; after each, let scripts, warps and battles finish (A/B pulses).
    if (step->mode == 11)
    {
        u8 *t = gPilotResult;
        bool8 busy = gMain.callback1 != CB1_Overworld || ArePlayerFieldControlsLocked() || gPaletteFade.active || gMain.inBattle;
        const u16 *op = sJourney[sJourneyOp];
        bool8 left = gSaveBlock1Ptr->location.mapGroup != sJourneyGroup || gSaveBlock1Ptr->location.mapNum != sJourneyMap;
        sJourneyFrames++;
        // Leave phase 0 at once: PilotCheck skips phase-0 cases while the field is not running
        // (battles, ferry scenes), and the journey must keep pressing keys there.
        sPilotPhase = 5;
        if (sJourneyOp >= JOURNEY_LEN)
        {
            gPilotKeys = 0; t[0] = sJourneyOp; t[1] = sJourneyOp >> 8; t[2] = 0;
            gPilotShot = 1; gPilotStage = 3;
            return;
        }
        if (sJourneyFrames >= 30000)
        {
            // Stuck: report the op and where the player is.
            gPilotKeys = 0; t[0] = sJourneyOp; t[1] = sJourneyOp >> 8; t[2] = 1 + sJourneyPhase;
            t[3] = gSaveBlock1Ptr->location.mapGroup; t[4] = gSaveBlock1Ptr->location.mapNum; t[5] = x - MAP_OFFSET; t[6] = y - MAP_OFFSET;
            t[7] = busy;
            gPilotShot = 1; gPilotStage = 3;
            return;
        }
        switch (sJourneyPhase)
        {
        case 0: // wait for a free, still player, then start the op
            if (busy)
            {
                gPilotKeys = (sJourneyFrames % 8) < 3 ? A_BUTTON : 0;
                break;
            }
            gPilotKeys = 0;
            if (sPilotIdle < 8)
                break;
            sJourneyX = x; sJourneyY = y;
            sJourneyGroup = gSaveBlock1Ptr->location.mapGroup; sJourneyMap = gSaveBlock1Ptr->location.mapNum;
            sJourneyFrames = 0; sJourneyPrompt = 0; sJourneyWasYesNo = FALSE;
            if (op[0] == 4)
            {
                u8 *c = t + 8 + 8 * op[1];
                c[0] = gSaveBlock1Ptr->location.mapGroup; c[1] = gSaveBlock1Ptr->location.mapNum; c[2] = x - MAP_OFFSET; c[3] = y - MAP_OFFSET;
                c[4] = VarGet(VAR_HOENN_STORY); c[5] = VarGet(VAR_JOHTO_STORY); c[6] = gPlayerPartyCount;
                c[7] = CheckBagHasItem(ITEM_POTION, 1) | FlagGet(FLAG_GOT_JOHTO_AIDE_POTION) << 1;
                sJourneyOp++;
                break;
            }
            sJourneyPhase = 1; // act from the next frame: `left` above was computed against the previous op's map
            break;
        case 1: // the action
            if (op[0] == 1)
            {
                // Hold until the player is headed for the planned cell (or, for exits, left the map);
                // NPCs in the way just delay it. Ending up anywhere else is reported as stuck.
                // Exits count on the map changing or a warp fade starting, not on any short field lock.
                gPilotKeys = op[1];
                if ((op[2] == 255 && (left || gPaletteFade.active || gMain.callback1 != CB1_Overworld)) || (op[2] != 255 && x - MAP_OFFSET == op[2] && y - MAP_OFFSET == op[3]))
                {
                    gPilotKeys = 0; sJourneyPhase = 2; sJourneyFrames = 0;
                }
                else if (op[2] != 255 && (x != sJourneyX || y != sJourneyY))
                {
                    sJourneyFrames = 30000; // moved somewhere unplanned
                }
            }
            else
            {
                // Face: a 2-frame tap turns in place. Talk: A.
                gPilotKeys = op[0] == 2 ? op[1] : A_BUTTON;
                if (sJourneyFrames >= 2)
                {
                    gPilotKeys = 0; sJourneyPhase = 2; sJourneyFrames = 0;
                }
            }
            break;
        case 2: // settle: scripts, warps, battles, ferries
            // A battle asking for a replacement opens the party menu on the fainted slot; point it at a
            // healthy Pokemon that is not already battling (the gifts join the lead pair here).
            if (gMain.inBattle && gMain.callback2 != BattleMainCB2)
            {
                s8 i;
                for (i = gPlayerPartyCount - 1; i >= 0; i--)
                {
                    if (GetMonData(&gPlayerParty[i], MON_DATA_HP) != 0
                     && i != gBattlerPartyIndexes[0] && i != gBattlerPartyIndexes[2])
                    {
                        gPartyMenu.slotId = i;
                        break;
                    }
                }
            }
            if (busy)
                gPilotKeys = (sJourneyFrames % 8) < 3 ? JourneyDialogueKey(op[0] == 3 ? op[1] : 0) : 0;
            else
                gPilotKeys = 0;
            if (!busy && sPilotIdle >= 20 && sJourneyFrames >= 12)
            {
                sJourneyOp++; sJourneyPhase = 0; sJourneyFrames = 0;
            }
            break;
        }
        return;
    }
    // Continue: save, wipe the location, load the save, continue; record where the player is.
    if (step->mode == 10 && sPilotPhase >= 5)
    {
        const struct MapLayout *layout = gMapHeader.mapLayout;
        if (sPilotPhase == 5)
        {
            r[8] = TrySavingData(SAVE_NORMAL);
            gSaveBlock1Ptr->location.mapGroup = 0; gSaveBlock1Ptr->location.mapNum = 0;
            gSaveBlock1Ptr->pos.x = gSaveBlock1Ptr->pos.y = 0;
            r[9] = LoadGameSave(SAVE_NORMAL);
            SetMainCallback1(NULL);
            SetMainCallback2(CB2_ContinueSavedGame);
            sPilotPhase = 6; sPilotStepFrames = 0;
        }
        else if (sPilotPhase == 6 && ((field && sPilotIdle >= 60) || sPilotStepFrames >= 2000))
        {
            r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
            r[2] = x - MAP_OFFSET; r[3] = y - MAP_OFFSET; r[5] = sPilotStepFrames >= 2000;
            r[4] = (layout->primaryTileset == &gTileset_HoennPilotGeneral) | (layout->secondaryTileset == &gTileset_HoennPilotPetalburg) << 1
                 | (layout->primaryTileset == &gTileset_General) << 2
                 | (layout->primaryTileset == &gTileset_JohtoPilotGeneral) << 3 | (layout->secondaryTileset == &gTileset_JohtoPilotNewBarkTown) << 4
                 | (layout->primaryTileset == &gTileset_HoennPilotBuilding) << 5;
            gPilotShot = 1; gPilotStage = 3;
        }
        return;
    }
    // Switch menu: open the Town Map, Start (switch button), A, the step key, A; then read BG0.
    if (step->mode == 9 && sPilotPhase >= 5)
    {
        const u16 *bg0 = (const u16 *)(BG_VRAM + 30 * 0x800);
        if (sPilotPhase == 5 && !gPaletteFade.active)
        {
            CleanupOverworldWindowsAndTilemaps();
            InitRegionMapWithExitCB(REGIONMAP_TYPE_NORMAL, CB2_ReturnToField);
            sPilotPhase = 6; sPilotStepFrames = 0;
        }
        else if (sPilotPhase == 6)
        {
            u32 f = sPilotStepFrames;
            gPilotKeys = (f >= 240 && f < 244) ? START_BUTTON : (f >= 280 && f < 284) ? A_BUTTON
                       : (f >= 460 && f < 464) ? step->keys : (f >= 520 && f < 524) ? A_BUTTON : 0;
            if (f == 500)
                gPilotShot = 1; // the menu with the new entry selected
            if (f >= 800)
            {
                r[0] = !gPaletteFade.active; r[2] = bg0[12 * 32 + 10]; r[3] = bg0[12 * 32 + 10] >> 8;
                gPilotShot = 2; gPilotStage = 3;
            }
        }
        return;
    }
    // Fly: open the Fly map, tap the key, then A; wait for the landing and record map and cell.
    if (step->mode == 8 && sPilotPhase >= 5)
    {
        if (sPilotPhase == 5 && !gPaletteFade.active)
        {
            CleanupOverworldWindowsAndTilemaps();
            SetMainCallback2(CB2_OpenFlyMap);
            sPilotPhase = 6; sPilotStepFrames = 0;
        }
        else if (sPilotPhase == 6)
        {
            if (sPilotStepFrames >= 240 && sPilotStepFrames < 242)
                gPilotKeys = step->keys; // a tap: the cursor moves one cell per 4 frames while held
            else if (sPilotStepFrames >= 300 && sPilotStepFrames < 304)
                gPilotKeys = A_BUTTON;
            else
                gPilotKeys = 0;
            if (sPilotStepFrames == 120)
                gPilotShot = 1; // the Fly map as it opens
            if (sPilotStepFrames == 280)
                gPilotShot = 2; // after the move
            if (sPilotStepFrames >= 320) { sPilotPhase = 7; sPilotStepFrames = 0; }
        }
        else if (sPilotPhase == 7)
        {
            if ((field && (x != sPilotStartX || y != sPilotStartY || moved) && sPilotIdle >= 30) || sPilotStepFrames >= 3000)
            {
                r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
                r[2] = x - MAP_OFFSET; r[3] = y - MAP_OFFSET; r[4] = sPilotStepFrames >= 3000;
                gPilotShot = 3; gPilotStage = 3;
            }
        }
        return;
    }
    if (step->mode == 4 && sPilotPhase >= 5)
    {
        if (sPilotPhase == 5 && !gPaletteFade.active)
        {
            sPilotFieldCallback = gMain.callback2;
            CleanupOverworldWindowsAndTilemaps();
            InitRegionMapWithExitCB(REGIONMAP_TYPE_NORMAL, CB2_ReturnToField);
            sPilotPhase = 6; sPilotStepFrames = 0;
        }
        else if (sPilotPhase == 6 && sPilotStepFrames >= 240)
        {
            // BG0 tilemap (screen block 30): screen tile (10, 12), before and after holding the key.
            const u16 *bg0 = (const u16 *)(BG_VRAM + 30 * 0x800);
            if (sPilotStepFrames == 240)
            {
                r[0] = gMain.callback2 != sPilotFieldCallback; r[1] = !gPaletteFade.active;
                r[2] = bg0[12 * 32 + 10]; r[3] = bg0[12 * 32 + 10] >> 8;
            }
            if (step->keys == 0)
            {
                gPilotShot = 1; gPilotStage = 3;
            }
            else if (sPilotStepFrames < 700)
            {
                gPilotKeys = step->keys; // hold: the cursor walks to the edge, then the page scrolls
            }
            else if (sPilotStepFrames >= 760)
            {
                gPilotKeys = 0;
                r[4] = bg0[12 * 32 + 10]; r[5] = bg0[12 * 32 + 10] >> 8;
                gPilotShot = 1; gPilotStage = 3;
            }
            else
            {
                gPilotKeys = 0;
            }
        }
        return;
    }
    if (sPilotPhase == 0)
    {
        // Scene whose script starts on its own (a trainer who sees the player on arrival): the field never
        // goes idle, so press through it like phase 7 does.
        if (step->mode == 7 && sPilotIdle < 30 && sPilotStepFrames >= 240 && (ArePlayerFieldControlsLocked() || gMain.inBattle))
        {
            sPilotStartGroup = gSaveBlock1Ptr->location.mapGroup; sPilotStartMap = gSaveBlock1Ptr->location.mapNum;
            sPilotPhase = 7; sPilotStepFrames = 0;
            return;
        }
        if (sPilotIdle < 30) return;
        if (step->mode == 5 || step->mode == 6 || step->mode == 7)
        {
            sPilotStartGroup = gSaveBlock1Ptr->location.mapGroup; sPilotStartMap = gSaveBlock1Ptr->location.mapNum;
            if (step->mode == 6 && (gPilotSpawn[gPilotCase - 1][7] & 8))
            {
                gSpecialVar_0x8004 = SEAGALLOP_VERMILION_CITY;
                gSpecialVar_0x8006 = SEAGALLOP_NEW_BARK;
                ScriptContext_SetupScript(EventScript_SailToDest);
                sPilotPhase = 7; sPilotStepFrames = 0;
                return;
            }
            gPilotKeys = step->keys; sPilotPhase = 5; sPilotStepFrames = 0;
            return;
        }
        if (step->mode == 10)
        {
            sPilotPhase = 5; sPilotStepFrames = 0;
            return;
        }
        if (step->mode == 11)
        {
            sPilotPhase = 5; sPilotStepFrames = 0;
            return;
        }
        if (step->mode == 4 || step->mode == 8 || step->mode == 9)
        {
            sPilotStartGroup = gSaveBlock1Ptr->location.mapGroup; sPilotStartMap = gSaveBlock1Ptr->location.mapNum;
            sPilotStartX = x; sPilotStartY = y;
            FadeScreen(FADE_TO_BLACK, 0);
            sPilotPhase = 5;
            return;
        }
        sPilotStartGroup = gSaveBlock1Ptr->location.mapGroup; sPilotStartMap = gSaveBlock1Ptr->location.mapNum;
        sPilotStartX = x; sPilotStartY = y;
        gPilotKeys = step->keys; sPilotPhase = 1; sPilotStepFrames = 0;
    }
    else if (sPilotPhase == 1)
    {
        if (step->mode == 3 && (x != sPilotStartX || y != sPilotStartY))
        {
            gPilotKeys = gPilotKeys == DPAD_RIGHT ? DPAD_LEFT : DPAD_RIGHT;
            sPilotStartX = x; sPilotStartY = y;
        }
        if ((step->mode == 0 && (!field || moved))
         || (step->mode == 1 && (x != sPilotStartX || y != sPilotStartY))
         || (step->mode == 2 && sPilotStepFrames >= 48)
         || (step->mode == 3 ? sPilotStepFrames >= 3000 : sPilotStepFrames >= 600))
        {
            gPilotKeys = 0; sPilotPhase = 2; sPilotIdle = 0; sPilotStepFrames = 0;
        }
    }
    else if (sPilotIdle >= 30 || sPilotStepFrames >= 900)
    {
        r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
        r[2] = x - MAP_OFFSET; r[3] = y - MAP_OFFSET;
        gPilotShot = sPilotStep + 1;
        sPilotPhase = 0; sPilotIdle = 0;
        if (++sPilotStep >= sPilotStepCount[gPilotCase - 1])
            gPilotStage = 3;
    }
}
void PilotCheck(void)
{
    if (gPilotStage == 0 || gPilotStage == 3) return;
    if (gMain.callback1 != CB1_Overworld && sPilotFrames == 0 && sPilotPhase == 0 && sPilotStep == 0) return;
    // Talk cases keep everyone but the player frozen until A is pressed, so wanderers stay on their cell.
    if (PilotIsTalkCase() && sPilotPhase < 6)
        FreezeObjectEventsExceptOne(gPlayerAvatar.objectEventId);
    if (sPilotStepCount[gPilotCase - 1] == 0) PilotDataCase();
    else PilotWalkCase();
}
static void PilotBoot(void)
{
    static const u8 name[] = _("TEST");
    if (!gPilotCase) return;
    gPilotStage = 1; sPilotFrames = 0;
    SetSaveBlocksPointers();
    // NewGameInitData copies the rival name: a blank save has no EOS and the copy overruns.
    StringCopy(gSaveBlock2Ptr->playerName, name); StringCopy(gSaveBlock1Ptr->rivalName, name);
    SetMainCallback2(CB2_NewGame);
}
'''.replace('SPAWNS',spawns).replace('STEPS',steps).replace('COUNTS',counts).replace('ANSWERS',answers).replace('JOURNEY_OPS',journey).replace('JOURNEY_LEN',str(len(JOURNEY))).replace('STOP_PROMPTS',stop_prompts).replace('FRESH_TRAINERS',fresh_trainers).replace('SLOT_TRAINERS',slot_trainers).replace('FRESH_MOVES',fresh_moves)
main=(game/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(PilotBoot);')
main=main.replace('#include "global.h"','#include "global.h"\nvoid PilotCheck(void);',1).replace('    ReadKeys();','    PilotCheck();\n    ReadKeys();')
assert 'PilotCheck();' in main and 'PilotBoot);' in main
(test/'src/main.c').write_text(main)
# Same entry as test_shops.py: right after NewGameInitData, warp straight onto the map under test.
setup='''
#include "pokemon.h"
#include "constants/heal_locations.h"
#include "event_data.h"
#include "game_settings.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "pokemon_storage_system.h"
#include "elite_four.h"
extern volatile u32 gPilotCase;
extern const s16 gPilotSpawn[][12];
u16 GetTrainerFlagId(u16 trainerId);
extern const u16 gPilotFreshTrainer[];
extern const u16 gPilotSlotTrainers[];
extern const u16 gPilotFreshMove[];
void PilotSetup(void)
{
    const s16 *spawn = gPilotSpawn[gPilotCase - 1];
    SetWarpDestination(spawn[0], spawn[1], -1, spawn[2], spawn[3]);
    SetLastHealLocationWarp(HEAL_LOCATION_PALLET_TOWN);  // New Game's respawn; a blackout read past the heal table without it
    WarpIntoMap();
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, spawn[6], 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
    CalculatePlayerPartyCount();
    if (spawn[6] >= 100)
        VarSet(VAR_REPEL_STEP_COUNT, 250);
    if (spawn[7] & 1)
    {
        u16 hp = 1;
        FlagSet(FLAG_SYS_POKEMON_GET);
        SetMonData(&gPlayerParty[0], MON_DATA_HP, &hp);
    }
    if (spawn[7] & 2)
        GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);
    if (spawn[7] & 4)
        FlagSet(FLAG_SYS_GAME_CLEAR);
    if (spawn[7] & 16)
    {
        u16 move = MOVE_PSYCHIC; u8 pp = 16, bonus = 0xFF;
        CreateMon(&gPlayerParty[0], SPECIES_MEWTWO, 100, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
        SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &move); SetMonData(&gPlayerParty[0], MON_DATA_PP_BONUSES, &bonus);
        SetMonData(&gPlayerParty[0], MON_DATA_PP1, &pp);
        gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SET;
        gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
        gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FAST;
        FlagSet(FLAG_SYS_POKEMON_GET);
        FlagSet(FLAG_EXP_SHARE_DISABLED);
        if (gPilotFreshTrainer[gPilotCase - 1])
        {
            u16 move = gPilotFreshMove[gPilotCase - 1];  // fixed damage (Seismic Toss / Night Shade)
            u8 fullPp = 32;
            // Snorlax with full facility training: bulky enough for the +60 route teams (an untrained
            // Mewtwo fell to Arnie's bug moves), and only Fighting hits it super effectively.
            CreateMon(&gPlayerParty[0], SPECIES_SNORLAX, 100, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
            EliteFour_ApplyFacilityTraining(&gPlayerParty[0]);
            SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &move); SetMonData(&gPlayerParty[0], MON_DATA_PP1, &fullPp);
            gPlayerParty[1] = gPlayerParty[0];  // two Pokemon for Amy and Liv's double battle
            CalculatePlayerPartyCount();
        }
    }
    VarSet(VAR_HOENN_STORY, spawn[8]);
    VarSet(VAR_HOENN_STARTER, spawn[9]);
    if (spawn[8] >= 3)
        FlagSet(FLAG_HIDE_HOENN_ROUTE101_RESCUE);
    VarSet(VAR_JOHTO_STORY, spawn[10]);
    VarSet(VAR_JOHTO_STARTER, spawn[11]);
    if (spawn[7] & 128)
    {
        u16 move = MOVE_DRAGON_PULSE, item = ITEM_SOUL_DEW;
        u16 species = SPECIES_LATIOS;
        u8 pp = 32;
        // Silver's Meganium/Octillery/Houndoom/Tyranitar team counters the Latios
        // fixture. Use a rain pair for that victory path; defeat is tested apart.
        if (spawn[7] & 1024) { species = SPECIES_KYOGRE; move = MOVE_ORIGIN_PULSE; item = ITEM_MYSTIC_WATER; }
        CreateMon(&gPlayerParty[0], species, 100, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
        SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &move);
        SetMonData(&gPlayerParty[0], MON_DATA_PP1, &pp);
        SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &item);
        EliteFour_ApplyFacilityTraining(&gPlayerParty[0]);
        gPlayerParty[1] = gPlayerParty[0];
        CalculatePlayerPartyCount();
    }
    if (spawn[7] & 4096)
        FlagSet(FLAG_GOT_JOHTO_AIDE_POTION);
    {
        u16 t;
        for (t = TRAINER_HOENN_CALVIN; t < NUM_TRAINERS; t++)
            FlagSet(GetTrainerFlagId(t)); // region route trainers start beaten (see the talk cases)
        for (t = 0; gPilotSlotTrainers[t]; t++)
            FlagSet(GetTrainerFlagId(gPilotSlotTrainers[t]));
        if (gPilotFreshTrainer[gPilotCase - 1])
            FlagClear(GetTrainerFlagId(gPilotFreshTrainer[gPilotCase - 1]));  // battle cases
    }
    if (spawn[7] & 2048)
        FlagSet(FLAG_WORLD_MAP_LITTLEROOT_TOWN);
    if (spawn[7] & 16384)
        VarSet(VAR_REPEL_STEP_COUNT, 60000); // the journey walks far more than one Repel
    if (spawn[7] & 8192)
        FlagSet(FLAG_WORLD_MAP_NEW_BARK_TOWN);
    if (spawn[7] & 256)
    {
        u16 hp = 1;
        CreateMon(&gPlayerParty[0], SPECIES_MAGIKARP, 5, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
        SetMonData(&gPlayerParty[0], MON_DATA_HP, &hp);
        gPlayerParty[1] = gPlayerParty[0];
    }
    if (spawn[7] & 32)
    {
        u8 i;
        for (i = 1; i < PARTY_SIZE; i++) gPlayerParty[i] = gPlayerParty[0];
        CalculatePlayerPartyCount();
    }
    if (spawn[7] & 64)
    {
        u8 box, slot;
        for (box = 0; box < TOTAL_BOXES_COUNT; box++)
            for (slot = 0; slot < IN_BOX_COUNT; slot++) *GetBoxedMonPtr(box, slot) = gPlayerParty[0].box;
    }
}
'''
world=(game/'src/overworld.c').read_text()
anchor='    NewGameInitData();\n    ResetInitialPlayerAvatarState();'
assert anchor in world
world=world.replace(anchor,'    NewGameInitData();\n    PilotSetup();\n    ResetInitialPlayerAvatarState();')
load='    gFieldCallback2 = NULL;\n    DoMapLoadLoop(&gMain.state);\n'
assert world.count(load)==1
world=world.replace(load,load+'    PilotAfterLoad();\n').replace('#include "global.h"','#include "global.h"\nvoid PilotAfterLoad(void);',1).replace('#include "global.h"','#include "global.h"\nvoid PilotSetup(void);',1)
(test/'src/overworld.c').write_text(world+setup)
movement=(test/'src/event_object_movement.c').read_text()
unfreeze='void UnfreezeObjectEvents(void)\n{\n    u8 i;\n'
assert movement.count(unfreeze)==1
movement=movement.replace(unfreeze,unfreeze+'    if (PilotKeepFrozen())\n        return;\n').replace('#include "global.h"','#include "global.h"\nbool8 PilotKeepFrozen(void);',1)
(test/'src/event_object_movement.c').write_text(movement)
menu=test/'src/script_menu.c'
menu.write_text(menu.read_text()+'\nbool8 PilotYesNoActive(void) { return FuncIsActiveTask(Task_YesNoMenu_HandleInput); }\n')
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
constants=dict(re.findall(r'#define (MAP_\w+|LAYOUT_\w+) +\(?(\d+[^)\n]*)',(test/'include/constants/maps.h').read_text()+(test/'include/constants/layouts.h').read_text()))
runner=test/'region-pilot-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/region_pilot_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'map'),str(len(CASES)),
                       *['0x'+symbols[s] for s in ('gPilotCase','gPilotStage','gPilotResult','gPilotKeys','gPilotShot')]],
                      capture_output=True,text=True,timeout=1800)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
rows={}
for m in re.finditer(r'case=(\d+) stage=(\d+) text=([0-9a-f]+)',result.stdout):
 assert int(m[2])==3,(m[1],'case did not finish',m[2])
 rows[int(m[1])]=bytes.fromhex(m[3])
assert len(rows)==len(CASES),rows.keys()

def same(installed,source,what):
 assert installed.read_bytes()==source.read_bytes(),('installed file differs from its source',what)
# Hoenn: built and installed assets must equal the audited staging output.
stage=root/'.build/region-pilots'
for built,staged in [('graphics/door_anims/hoenn/littleroot.4bpp','hoenn-doors/littleroot.4bpp'),('graphics/door_anims/hoenn/birchs_lab.4bpp','hoenn-doors/birchs_lab.4bpp'),
                     *[(f'data/tilesets/primary/hoenn_pilot/anim/Flower_{i}.4bpp',f'hoenn-animations/Flower_{i}.4bpp') for i in range(3)]]:
 if (stage/staged).exists():same(test/built,stage/staged,built)
for name in ['LittlerootTown','Route101']:
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn'/name/f).exists():same(game/'data/layouts'/('Hoenn_'+name)/f,stage/'hoenn'/name/f,name)
for name in ['LittlerootTown_BrendansHouse_1F','LittlerootTown_BrendansHouse_2F','LittlerootTown_MaysHouse_1F','LittlerootTown_MaysHouse_2F','LittlerootTown_ProfessorBirchsLab']:
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn-interiors'/name/f).exists():same(game/'data/layouts'/('Hoenn_'+name)/f,stage/'hoenn-interiors'/name/f,name)
for folder in ['primary/hoenn_pilot_building','secondary/hoenn_pilot_brendans_mays_house','secondary/hoenn_pilot_lab']:
 for f in ('tiles.png','metatiles.bin','metatile_attributes.bin'):
  if (stage/'hoenn-interior-graphics'/folder/f).exists():same(game/'data/tilesets'/folder/f,stage/'hoenn-interior-graphics'/folder/f,folder)
# Johto: tiles, metatiles, palettes, blocks, animation frames and doors are unchanged HNS_modern files.
hns=root.parent/'map-references/HNS_modern'
if hns.exists():
 for src,dst in [('primary/johto_general','primary/johto_pilot_general'),('secondary/new_bark_town','secondary/johto_pilot_new_bark_town'),
                 ('primary/johto_building','primary/johto_pilot_building'),('secondary/players_house','secondary/johto_pilot_players_house'),
                 ('secondary/house_lab','secondary/johto_pilot_house_lab'),('secondary/gate_standard','secondary/johto_pilot_gate_standard')]:
  for f in ['tiles.png','metatiles.bin',*[f'palettes/{p.name}' for p in (hns/'data/tilesets'/src/'palettes').glob('*.pal')]]:
   same(game/'data/tilesets'/dst/f,hns/'data/tilesets'/src/f,dst+'/'+f)
 for name in ['NewBarkTown','Route29','NewBarkTown_PlayersHouse_1F','NewBarkTown_PlayersHouse_2F','NewBarkTown_House1','NewBarkTown_House2','NewBarkTown_Lab','Gate_Route29_Route46']:
  for f in ('map.bin','border.bin'):same(game/'data/layouts'/('Johto_'+name)/f,hns/'data/layouts'/name/f,name)
 for f in [*[f'flower/{i}.png' for i in range(5)],*[f'water_current_landwatersedge/{i}.png' for i in range(8)]]:
  same(game/'data/tilesets/primary/johto_pilot_general/anim'/f,hns/'data/tilesets/primary/johto_general/anim'/f,f)
 same(game/'graphics/door_anims/johto/new_bark_blue.png',hns/'graphics/door_anims/cerulean.png','blue door')
 same(game/'graphics/door_anims/johto/new_bark_yellow.png',hns/'graphics/door_anims/NewBarkTown_Door_Yellow.png','yellow door')

HG,JG=int(constants['MAP_GROUP_HOENN_LITTLEROOT_TOWN'] if 'MAP_GROUP_HOENN_LITTLEROOT_TOWN' in constants else rows[1][0]),rows[12][0]
UP,DOWN,LEFT,RIGHT=2,1,3,4  # CONNECTION_*
HOENN_TILES,KANTO_TILES,JOHTO_TILES=0b00011,0b00100,0b11000
WARP_DOOR,CAVE_DOOR=0x69,0x60
little,route,pallet,newbark,route29=rows[1],rows[2],rows[3],rows[12],rows[13]
assert (little[0],little[1])==(HG,LITTLEROOT) and (route[0],route[1])==(HG,ROUTE101),'Hoenn maps loaded in the wrong slots'
assert (newbark[0],newbark[1])==(JG,NEWBARK) and (route29[0],route29[1])==(JG,ROUTE29) and JG==HG+1,'Johto maps loaded in the wrong slots'
for name,row,tiles in (('Littleroot',little,HOENN_TILES),('Route101',route,HOENN_TILES),('New Bark',newbark,JOHTO_TILES),('Route29',route29,JOHTO_TILES)):
 assert row[4]==tiles,(name,'wrong tilesets',bin(row[4]))
 assert row[24]==0,(name,'spawn cell is blocked')
 assert row[17],(name,'tile animation did not update VRAM tile 508')
assert pallet[4]==KANTO_TILES,'Pallet Town no longer uses the FireRed tilesets'
assert little[7]==1 and tuple(little[8:11])==(UP,HG,ROUTE101),('Littleroot must connect north to Route101',list(little[7:11]))
# Route 101: north to Oldale and south to Littleroot; the first one recorded follows the Emerald header order.
assert route[7]==2 and tuple(route[8:11]) in ((DOWN,HG,LITTLEROOT),(UP,HG,7)),('Route101 must connect south to Littleroot and north to Oldale',list(route[7:11]))
assert newbark[7]==1 and tuple(newbark[8:11])==(LEFT,JG,ROUTE29),('New Bark must connect west to Route29',list(newbark[7:11]))
# Route 29: west to Cherrygrove (Johto batch 1, listed first) and east to New Bark (walked in case 21).
assert route29[7]==2 and tuple(route29[8:11])==(LEFT,JG,8),('Route29 must connect west to Cherrygrove and east to New Bark',list(route29[7:11]))
assert little[11]|little[12]<<8==0x2C8 and little[13]==WARP_DOOR and little[14],('Littleroot door cell',little[11:15].hex())
assert newbark[11]|newbark[12]<<8==0x2CB and newbark[13]==WARP_DOOR and newbark[14],('New Bark blue door cell',newbark[11:15].hex())
assert route29[13]==CAVE_DOOR,('Route 46 gate entrance should be a step-on warp',route29[11:15].hex())

def walked(case):
 row=rows[case];return [tuple(row[i*4:i*4+4]) for i in range(len(CASES[case-1][5]))]
def at(case,expected):
 got=walked(case)
 for i,(g,want) in enumerate(zip(got,expected)):
  group,num,cells=want
  assert g[0]==group and g[1]==num and (g[2],g[3]) in cells,(CASES[case-1][0],'step',i,'got',g,'expected group',group,'map',num,'in',cells)
# Doors and doormats land on the arrival cell; leaving through a door or step-on stairs steps one cell off it.
at(4,[(HG,B1F,{(8,8)}),(HG,LITTLEROOT,{(5,9)})])
at(5,[(HG,M1F,{(2,8)}),(HG,LITTLEROOT,{(14,9)})])
at(6,[(HG,BIRCH,{(6,12)}),(HG,LITTLEROOT,{(7,17)})])
at(7,[(HG,B2F,{(7,2)}),(HG,B2F,{(7,3)}),(HG,B1F,{(8,3)})])
at(8,[(HG,M2F,{(1,2)}),(HG,M2F,{(1,3)}),(HG,M1F,{(2,3)})])
at(9,[(HG,B2F,{(1,3)})])
at(10,[(HG,B2F,{(1,4)}),(HG,B2F,{(1,4)}),(HG,B2F,{(1,4)}),(HG,B2F,{(2,4)})])
at(11,[(HG,ROUTE101,{(10,19)}),(HG,LITTLEROOT,{(10,0)})])
for case,(num,arrive,back) in {14:(ELM,(6,12),(10,10)),15:(P1F,(9,8),(20,12)),16:(HOUSE1,(4,8),(4,22)),17:(HOUSE2,(4,8),(15,22))}.items():
 at(case,[(JG,num,{arrive}),(JG,NEWBARK,{back})])
at(18,[(JG,P2F,{(9,2)})])   # FireRed stairs: arrive on the stair cell
at(19,[(JG,P1F,{(10,2)})])
at(20,[(JG,GATE,{(7,9)}),(JG,ROUTE29,{(34,11)})])
at(21,[(JG,ROUTE29,{(69,17)}),(JG,NEWBARK,{(0,12)})])
pilot={t['row']['map']:t['row'] for t in json.loads((game/'hack-data/region_pilot_encounters.json').read_text())['tables']}
species_ids={name:int(v,0) for name,v in re.findall(r'#define (SPECIES_\w+)\s+(\d+|0x[0-9A-Fa-f]+)\b',(test/'include/constants/species.h').read_text())}
def u16(row,i):return row[i]|row[i+1]<<8
LAND,WATER,ROCK,FISH=1,2,4,8
assert little[26]==0,'Littleroot has no wild table'
for name,row,mapid in (('Route101',route,'MAP_HOENN_ROUTE101'),('Route29',route29,'MAP_JOHTO_ROUTE29'),('New Bark',newbark,'MAP_JOHTO_NEW_BARK_TOWN')):
 t=pilot[mapid];want=(LAND if 'land_mons' in t else 0)|(WATER if 'water_mons' in t else 0)|(FISH if 'fishing_mons' in t else 0)
 assert row[26]==want,(name,'tables attached',bin(row[26]),bin(want))
 for field,(i,rate) in {'land_mons':(27,33),'water_mons':(29,34),'fishing_mons':(31,35)}.items():
  if field in t:assert (u16(row,i),row[rate])==(species_ids[t[field]['mons'][0]['species']],t[field]['encounter_rate']),(name,field)
battles={}
for case,mapid,where in ((22,'MAP_HOENN_ROUTE101',(HG,ROUTE101)),(23,'MAP_JOHTO_ROUTE29',(JG,ROUTE29))):
 row=rows[case];species=u16(row,2);level=row[4]
 table=[(species_ids[m['species']],m['min_level'],m['max_level']) for m in pilot[mapid]['land_mons']['mons']]
 assert any(species==s and lo<=level<=hi for s,lo,hi in table),(mapid,'wild battle species/level not in its table',species,level)
 assert tuple(row[:2])==where,(mapid,'battle on the wrong map',row[:2])
 battles[mapid]={'species':next(n for n,v in species_ids.items() if v==species),'level':level}
print('Wild battles:',battles)
chars={}
for line in (game/'charmap.txt').read_text(encoding='utf-8').splitlines():
 m=re.match(r"^'((?:\\.|[^'])*)'\s*=\s*([0-9A-F]{2})\s*$",line)
 if m:chars.setdefault(int(m[2],16),m[1].replace("\\'","'"))
def decode(data):
 text=''
 for byte in data:
  if byte==0xFF:break
  text+=chars.get(byte,f'<{byte:02x}>')
 return text
order=[s['id'] for s in json.loads((game/'src/data/region_map/region_map_sections.json').read_text(encoding='utf-8'))['map_sections']]
sections={}
for name,row,section,label in (('Littleroot',little,'MAPSEC_LITTLEROOT_TOWN','LITTLEROOT TOWN'),('Route101',route,'MAPSEC_ROUTE_101','ROUTE 101'),
                               ('New Bark',newbark,'MAPSEC_NEW_BARK_TOWN','NEW BARK TOWN'),('Route29',route29,'MAPSEC_ROUTE_29','ROUTE 29'),('Pallet',pallet,'MAPSEC_PALLET_TOWN','PALLET TOWN')):
 shown=decode(row[40:64])
 assert row[38]==order.index(section),(name,'section id',row[38],order.index(section))
 assert shown==label,(name,'map name',shown)
 assert row[39]==row[38],(name,'Pokemon created here should record this section as met location',row[39])
 if name!='Pallet':assert row[37]==1,(name,'pilot exteriors show the map name popup')
 sections[name]={'id':row[38],'name':shown}
# Existing IDs keep their place: the Johto sections come after the last FireRed one.
assert order.index('MAPSEC_NEW_BARK_TOWN')==order.index('MAPSEC_SPECIAL_AREA')+1 and order[-19:]==['MAPSEC_ROUTE_29','MAPSEC_CHERRYGROVE_CITY','MAPSEC_ROUTE_30','MAPSEC_ROUTE_31','MAPSEC_VIOLET_CITY','MAPSEC_ROUTE_32','MAPSEC_ROUTE_33','MAPSEC_AZALEA_TOWN','MAPSEC_UNION_CAVE','MAPSEC_ILEX_FOREST','MAPSEC_ROUTE_34','MAPSEC_GOLDENROD_CITY','MAPSEC_ROUTE_35','MAPSEC_ROUTE_36','MAPSEC_ROUTE_37','MAPSEC_ECRUTEAK_CITY','MAPSEC_ROUTE_38','MAPSEC_ROUTE_39','MAPSEC_OLIVINE_CITY']
for case in (24,25,26,*PAGE_SCROLL):
 assert rows[case][0]==1 and rows[case][1]==1,(CASES[case-1][0],'Town Map did not open and settle',rows[case][:2].hex())
# Page scroll: screen tile (10,12) shows page tile (scroll + 10, 12) of the page tilemap.
import struct as _st
scrolls={}
for case,(page,start,end) in PAGE_SCROLL.items():
 data=(game/f'graphics/region_map/{page}_page.bin').read_bytes();words=_st.unpack(f'<{len(data)//2}H',data);cols=len(words)//20
 before,after=u16(rows[case],2),u16(rows[case],4)
 assert before==words[12*cols+start+10],(page,'page did not open at scroll',start,hex(before),hex(words[12*cols+start+10]))
 assert after==words[12*cols+end+10],(page,'page did not scroll to',end,hex(after),hex(words[12*cols+end+10]))
 scrolls[page]=f'{start}->{end}'
print('Region map page scroll:',scrolls)
switched={}
for case,(start,page) in SWITCHES.items():
 data=(game/f'graphics/region_map/{page}_page.bin').read_bytes();words=_st.unpack(f'<{len(data)//2}H',data);cols=len(words)//20
 got=u16(rows[case],2)
 assert rows[case][0]==1 and got==words[12*cols+10],(start,'switch menu did not show the',page,'page',hex(got),hex(words[12*cols+10]))
 switched[start.replace('MAP_','')]=page
print('Switch menu:',switched)
flown={}
for case,(start,land) in FLIGHTS.items():
 row=rows[case];dest=(HG,LITTLEROOT) if land==(5,9) else (JG,NEWBARK)
 assert not row[4] and tuple(row[:2])==dest and tuple(row[2:4])==land,(start,'Fly landing',list(row[:5]),dest,land)
 flown[start.replace('MAP_','')]=f'{land}'
print('Fly:',flown)
print('Map sections:',sections)
CODES={0xFE:'\n',0xFB:'\\p',0xFA:'\\l'}
TOKENS={m[1]:int(m[2],16) for m in re.finditer(r'^([A-Z_0-9]+)\s*=\s*([0-9A-F]{2})\s*$',(game/'charmap.txt').read_text(encoding='utf-8'),re.M)}
for line in (game/'charmap.txt').read_text(encoding='utf-8').splitlines():
 m=re.match(r'^([A-Z_0-9]+)\s*=\s*([0-9A-F]{2})\s*$',line)
 if m and int(m[2],16) not in chars:chars[int(m[2],16)]='{'+m[1]+'}'
def shown_text(data):
 out=''
 for byte in data:
  if byte==0xFF:break
  out+=CODES.get(byte) or chars.get(byte,f'<{byte:02x}>')
 return out
talked={'text':0,'item':0,'cuttree':0,'heal':0,'ptbr':0}
for case,info in TALKS.items():
 row=rows[case];got=shown_text(row[8:128]).replace('’',"'")  # the apostrophe byte has two charmap names
 where=(info['map'],info['kind'],info['index'],info['setup'])
 if info['what']=='silent':
  # Characters with no neutral source line stand there; A starts nothing.
  assert row[4]==0,(where,'a silent character started a script');talked['silent']=talked.get('silent',0)+1;continue
 assert row[4]==1,(where,'pressing A did not start the event script')
 if info['setup']&2:
  assert got and got!=shown_text(rows[next(c for c,i in TALKS.items() if i['map']==info['map'] and i['kind']==info['kind'] and i['index']==info['index'] and i['setup']==0)][8:128]),(where,'PT-BR shows the English text',got)
  assert got.startswith(('LABORATÓRIO','Ah! Seu')),(where,got);talked['ptbr']+=1;continue
 if info['what']=='started':
  talked['started']=talked.get('started',0)+1;continue  # row[4] (script started) was checked above
 if info['what']=='item':
  assert row[6] and row[7],(where,'Potion not in the bag or flag not set',row[6],row[7]);talked['item']+=1;continue
 if info['what']=='cuttree':
  assert got.startswith('This tree'),(where,got);talked['cuttree']+=1;continue
 if info['what']=='heal' and info['setup']&1:
  assert row[5],(where,'healing machine did not restore the party');talked['heal']+=1;continue
 want=info['expected'].replace('{PLAYER}','TEST').replace('\\n','\n')
 # One-byte tokens such as {UP_ARROW} decode as whatever character the charmap lists first for that byte.
 want=re.sub(r'\{(\w+)\}',lambda m:chars.get(TOKENS[m[1]],m[0]) if m[1] in TOKENS else m[0],want)
 want=want[:-1] if want.endswith('$') else want
 assert got and want.startswith(got.rstrip('\\')) or got.startswith(want),(where,'text differs',got,want)
 talked['text']+=1
print('Talked to pilot events:',talked)
trainer_ids=_trainer_ids((test/'include/constants/opponents.h').read_text())
pilot_trainers=json.loads((game/'hack-data/region_pilot_trainers.json').read_text(encoding='utf-8'))
story_seen={}
for case,expect in SCENES.items():
 row=rows[case];name=expect['name']
 assert not row[26],(name,'scene did not settle in time',row[:32].hex())
 state_index,starter_index=(27,28) if expect.get('johto') else (4,5)
 assert row[state_index]==expect['story'],(name,'story state',row[state_index],expect['story'])
 if 'pos' in expect:assert tuple(row[2:4])==expect['pos'],(name,'player position',tuple(row[2:4]))
 if 'party' in expect:assert row[6]==expect['party'],(name,'party size',row[6])
 if 'starter' in expect:assert row[starter_index]==expect['starter'],(name,'starter choice',row[starter_index])
 if 'ballflags' in expect:assert row[29]==expect['ballflags'],(name,'ball flags',row[29])
 if 'gifts' in expect:assert tuple(row[104:108])==expect['gifts'],(name,'aide gifts: Potion, 5 Poke Balls, Potion flag, Balls flag',tuple(row[104:108]))
 if 'boxed' in expect:assert u16(row,30)==species_ids[expect['boxed']],(name,'boxed starter',u16(row,30))
 if 'outcome' in expect:assert row[25]==expect['outcome'],(name,'battle outcome',row[25])
 if 'category' in expect:assert decode(row[64:96])==expect['category'],(name,'category',decode(row[64:96]))
 if expect.get('persist'):
  assert list(row[48:56])==[1,1,2,0,1,2,JG,ELM],(name,'save roundtrip',list(row[48:56]))
  assert u16(row,56)==species_ids['SPECIES_CHIKORITA'],(name,'saved gift missing')
 if 'gift' in expect:assert (u16(row,22),row[24])==(species_ids[expect['gift']],5),(name,'starter not given at level 5',u16(row,22),row[24])
 if 'hidden' in expect:assert row[7]==1,(name,'rescue objects not hidden afterwards')
 if 'wild' in expect:
  sp,lv=expect['wild'];assert row[21] and (u16(row,8),row[10])==(species_ids[sp],lv),(name,'wild battle',u16(row,8),row[10])
 if 'trainer' in expect:
  t=pilot_trainers['trainers'][expect['trainer']]
  assert row[21] and u16(row,11)==int(trainer_ids[expect['trainer']]),(name,'trainer',u16(row,11))
  team=[species_ids[m['species'] if isinstance(m,dict) else m[0]] for m in t['team']]
  assert [u16(row,32+i*2) for i in range(len(team))]==team,(name,'team',[u16(row,32+i*2) for i in range(len(team))])
  leader=t['team'][0];level=leader['level'] if isinstance(leader,dict) else leader[1]
  assert row[10]==level+t.get('level_offset',pilot_trainers['level_offset']),(name,'lead level',row[10])
  if expect.get('route_trainer'):
   # Geography trainers: source teams (no IV/EV spread), single or double as in the source; beaten afterwards.
   assert row[44]==int(bool(t.get('double'))),(name,'double battle',row[44])
   assert decode(row[96:104])==t['name'][:8],(name,'trainer name (first 8 characters are recorded)',decode(row[96:104]))
   assert row[108]==1,(name,'trainer flag not set after the win')
  else:
   assert row[44:47]==bytes([1,1,1]),(name,'double battle / perfect IVs / 508 EVs',list(row[44:47]))
   assert row[47]==1 and decode(row[96:104])==t['name'],(name,'battle shows the Kanto rival name instead of',t['name'],row[47],decode(row[96:104]))
 story_seen[name]='ok'
print('Region story scenes:',story_seen)
# Generated at build time: #define MAP_X (num | (group << 8))
_maps={m[1]:(int(m[3]),int(m[2])) for m in re.finditer(r'#define (MAP_\w+)\s+\((\d+) \| \((\d+) << 8\)\)',(test/'include/constants/map_groups.h').read_text())}
def _group_num(name):
 return _maps[name]
ARRIVAL={'littleroot':((HG,LITTLEROOT),(11,14)),'newbark':((JG,NEWBARK),(24,13)),'vermilion':(_group_num('MAP_VERMILION_CITY'),(23,32))}
for case,(start,dest) in TRIPS.items():
 row=rows[case];where_,cell=ARRIVAL[dest]
 assert row[4]==1 and tuple(row[:2])==where_ and tuple(row[2:4])==cell,(start,'->',dest,'ferry arrival',list(row[:4]),where_,cell)
print('SEAGALLOP trips:',{f"{s.replace('MAP_','')}->{d}":'ok' for s,d in TRIPS.values()})
batch_walked={}
for case,want in BATCH_WALKS.items():
 for i,(g,(mapname,cells)) in enumerate(zip(walked(case),want)):
  assert (g[0],g[1])==_group_num(mapname) and (cells is None or (g[2],g[3]) in cells),(CASES[case-1][0],'step',i,'got',g,'expected',mapname,cells)
 batch_walked[CASES[case-1][0].replace('MAP_HOENN_','').replace('MAP_JOHTO_','')+'->'+want[0][0].replace('MAP_HOENN_','').replace('MAP_JOHTO_','')]='ok'
print('Geography batch walks:',batch_walked)
jrow=rows[JOURNEY_CASE];jdone=u16(jrow,0)
assert jrow[2]==0 and jdone==len(JOURNEY),('journey stopped at op',jdone,'of',len(JOURNEY),'phase',jrow[2]-1,'map',list(jrow[3:5]),'cell',list(jrow[5:7]),'busy',jrow[7],
  'next',JOURNEY[jdone] if jdone<len(JOURNEY) else None,'previous',JOURNEY[jdone-1] if jdone else None,'last check',JOURNEY_CHECKS[max([a for o,a,*_ in JOURNEY[:jdone] if o==J_CHECK] or [0])])
_vm=_group_num('MAP_VERMILION_CITY')
JWANT={'ferry to Littleroot':((HG,LITTLEROOT),(11,14),0,0),'rescue started':((HG,ROUTE101),(10,15),1,0),'Birch rescued':((HG,ROUTE101),(7,15),3,0),
 'Birch in his lab':((HG,BIRCH),(6,5),3,0),'May beaten':((HG,M2F),None,4,0),'back in Vermilion':(_vm,(23,32),4,0),'ferry to New Bark':((JG,NEWBARK),(24,13),4,0),
 'Elm request':((JG,ELM),(6,4),4,1),'Chikorita received':((JG,ELM),(8,5),4,2),'aide Potion':((JG,ELM),None,4,2),'Silver beaten':((JG,NEWBARK),None,4,3),
 'Route 29 and back':((JG,NEWBARK),(0,12),4,3),'home in Vermilion':(_vm,(23,32),4,3)}
journey_seen={}
for k,name in enumerate(JOURNEY_CHECKS):
 c=jrow[8+8*k:16+8*k];where,cell,hs,js=JWANT[name]
 assert tuple(c[:2])==tuple(where) and (cell is None or tuple(c[2:4])==cell) and (c[4],c[5])==(hs,js),(name,'checkpoint',list(c))
 journey_seen[name]='ok'
last=jrow[8+8*(len(JOURNEY_CHECKS)-1):16+8*(len(JOURNEY_CHECKS)-1)]
assert last[6]==4 and last[7]&2,('journey end: party should hold the lead pair plus Treecko and Chikorita, Potion flag set',list(last))
print('Journey (M7):',len(JOURNEY),'ops,',journey_seen)
continued={}
_continue_tiles={'hoenn':0b00011,'johto':0b01000,'hoenn_interior':0b100000}
for case,(start,x0,y0,tiles) in CONTINUES.items():
 row=rows[case];want=_group_num(start) if start in _maps else None
 assert row[8]==1 and row[9]==1,(start,'save or load failed',row[8],row[9])
 assert not row[5],(start,'Continue never reached a free field',list(row[:6]))
 assert want is None or tuple(row[:2])==want,(start,'Continue loaded another map',list(row[:2]),want)
 assert tuple(row[2:4])==(x0,y0),(start,'Continue moved the player',tuple(row[2:4]))
 assert row[4]&_continue_tiles[tiles]==_continue_tiles[tiles],(start,'region tilesets missing after Continue',bin(row[4]))
 continued[start.replace('MAP_','')]=f'{x0},{y0}'
print('Save and Continue:',continued)
print('PASS: Hoenn and Johto pilots load on their own tilesets with doors, animation and connections; every door, '
      'doormat, staircase and the Route 29 gate warp both ways; both connections cross on foot; the Hoenn bedroom cell '
      'blocks north/south only; wild tables are attached and both pilot routes start real wild battles from them; '
      'pilot maps carry named sections (popup, met location) and the Town Map opens on them; '
      'every ambient event answers A with its source text, the Potion is picked up, the lab machine heals, PT-BR shows; '
      'the SEAGALLOP sails Vermilion <-> Littleroot/New Bark only after the League; '
      'the region map shows scrolling Hoenn/Johto pages, the switch menu reaches them and Fly lands in Littleroot/New Bark; '
      'a save made in Hoenn or Johto continues on the same map and cell; '
      'a continuous journey from Vermilion plays both pilot stories by input alone and sails home; '
      'Birch is rescued on Route 101 (starter gift, Zigzagoon battle), his lab follows the story and May battles with the counter team; '
      'assets match their sources; Pallet unchanged.')
(out/'results.json').write_text(json.dumps({'status':'passed','build':str(test),'cases':{str(k):v.hex() for k,v in rows.items()},
                                            'walks':{str(k):walked(k) for k in rows if CASES[k-1][5] and CASES[k-1][5][0][1] not in ('BATTLE','TOWNMAP')},'sections':sections,'battles':battles},indent=2)+'\n')
