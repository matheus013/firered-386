"""Compare implemented move data with the pinned upstream snapshot and shop stock."""
from pathlib import Path
import csv,re,json
from balance_trainers import Data
ROOT=Path(__file__).resolve().parents[3];data=Data()
ids={n:int(i) for n,i in re.findall(r'^#define MOVE_(\w+)\s+(\d+)',(ROOT/'games/super-fire-red/include/constants/moves.h').read_text(),re.M) if 0<int(i)<=354}
upstream={int(r['id']):r for r in csv.DictReader((ROOT/'games/super-fire-red/hack-data/upstream/pokeapi/moves.csv').open(encoding='utf-8'))}
source=(ROOT/'games/super-fire-red/src/data/battle_moves.h').read_text();checks=0
for name,id in ids.items():
 row=upstream[id];mon=data.moves[name]
 assert mon['category']=={'1':'STATUS','2':'PHYSICAL','3':'SPECIAL'}[row['damage_class_id']],name
 assert mon['accuracy']==int(row['accuracy'] or 0),name
 if row['power']:assert mon['power']==int(row['power']),name
 checks+=1
assert checks==354
assert data.learn['CATERPIE'] and (1,'TACKLE') in data.learn['CATERPIE'], 'Padded low levels must be parsed.'
assert data.moves['FIRE_PUNCH']['category']=='PHYSICAL'
assert data.moves['SHADOW_BALL']['category']=='SPECIAL'
shop=(ROOT/'games/super-fire-red/data/maps/CeladonCity_DepartmentStore_2F/scripts.inc').read_text().split('CeladonCity_DepartmentStore_2F_TMs::')[1].split('ITEM_NONE')[0]
assert list(map(int,re.findall(r'ITEM_TM(\d+)',shop)))==list(range(1,51))
imported=json.loads((ROOT/'games/super-fire-red/hack-data/learnset_import.json').read_text())
assert len(imported['species'])==386
print('PASS: 354 move categories/power/accuracy, padded level-1 parser, 50 Celadon TMs and 386 imported species; full new-move coverage remains pending.')
