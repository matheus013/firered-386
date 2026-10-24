"""Check generated moves against the pinned PokeAPI data and the native tables they extend."""
from pathlib import Path
import csv,json,re
ROOT=Path(__file__).resolve().parents[3];GAME=ROOT/'games/super-fire-red';SRC=ROOT/'games/super-fire-red/hack-data/upstream/pokeapi'
spec=json.loads((ROOT/'games/super-fire-red/hack-data/new_moves.json').read_text(encoding='utf-8'))['moves']
pokeapi={r['identifier'].replace('-','_').upper():r for r in csv.DictReader((SRC/'moves.csv').open(encoding='utf-8'))}
types={r['id']:'TYPE_'+r['identifier'].upper() for r in csv.DictReader((SRC/'types.csv').open(encoding='utf-8'))}
ids={n:int(i) for n,i in re.findall(r'^#define MOVE_(\w+)\s+(\d+)',(GAME/'include/constants/moves.h').read_text(),re.M)}
count=int(re.search(r'#define MOVES_COUNT (\d+)',(GAME/'include/constants/moves.h').read_text())[1])
moves=dict(re.findall(r'\[MOVE_(\w+)\] =\n    \{(.*?)\n    \}',(GAME/'src/data/battle_moves.h').read_text(encoding='utf-8'),re.S))
names=dict(re.findall(r'\[MOVE_(\w+)\]\s*= _\("([^"]*)"\)',(GAME/'src/data/text/move_names.h').read_text(encoding='utf-8')))
anim_table=(GAME/'data/battle_anim_scripts.s').read_text(encoding='utf-8')
anims=re.findall(r'^\t\.4byte (Move_\w+)',anim_table[anim_table.index('gBattleAnims_Moves::'):],re.M)
labels=set(re.findall(r'^(Move_\w+):',anim_table,re.M))
pt=json.loads((ROOT/'games/super-fire-red/hack-data/localization/move_descriptions_ptbr.json').read_text(encoding='utf-8'))
field=lambda body,name:re.search(r'\.'+name+r' = ([^,]+),',body)[1]
assert count==355+len(spec),count
checks=0
for index,(key,move) in enumerate(spec.items()):
 row=pokeapi[key];body=moves[key]
 assert ids[key]==355+index,key
 assert int(field(body,'power'))==(int(row['power'] or 0) or (0 if row['damage_class_id']=='1' else 1)),key  # 1 marks variable power
 assert int(field(body,'accuracy'))==int(row['accuracy'] or 0),key
 assert int(field(body,'pp'))==int(row['pp']) and int(field(body,'priority'))==int(row['priority']),key
 assert field(body,'type')==types[row['type_id']],key
 assert field(body,'category')=={'1':'MOVE_CATEGORY_STATUS','2':'MOVE_CATEGORY_PHYSICAL','3':'MOVE_CATEGORY_SPECIAL'}[row['damage_class_id']],key
 assert field(body,'effect')==move['effect'],key
 assert names[key]==move['name'] and len(move['name'])<=12,key
 assert anims[ids[key]]=='Move_'+move['anim'] and anims[ids[key]] in labels,key
 assert ''.join(p.capitalize() for p in key.split('_')) in pt,key
 checks+=1
assert anims[count]=='Move_COUNT' and len(anims)==count+1,'animation table must stay aligned with MOVES_COUNT'
print(f'PASS: {checks} new moves match PokeAPI numbers/type/category, names, animations and PT descriptions; MOVES_COUNT {count}.')
