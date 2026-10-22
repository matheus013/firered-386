"""Import modern numeric/category data for implemented move IDs only.

Does not claim to implement newer move effects or full generation-nine learnsets.
"""
from pathlib import Path
import csv,json,re
ROOT=Path(__file__).resolve().parents[1]
source=ROOT/'data/upstream/pokeapi';path=ROOT/'game/src/data/battle_moves.h'
rows={int(r['id']):r for r in csv.DictReader((source/'moves.csv').open())}
ids={n:int(i) for n,i in re.findall(r'^#define MOVE_(\w+)\s+(\d+)',(ROOT/'game/include/constants/moves.h').read_text(),re.M)}
categories={'1':'STATUS','2':'PHYSICAL','3':'SPECIAL'}
def update(match):
 name,body=match.groups();id=ids[name]
 category='STATUS' if id==0 else categories[rows[id]['damage_class_id']]
 body=re.sub(r'\n\s*\.category = [^,]+,','',body)
 body+='\n        .category = MOVE_CATEGORY_'+category+','
 if id:
  row=rows[id]
  # Preserve the native sentinel 1 for variable/fixed damage routines.
  values={key:row[column] for key,column in [('power','power'),('accuracy','accuracy'),('pp','pp'),('priority','priority')]}
  for key,value in values.items():
   if value:body=re.sub(r'\.'+key+r' = [^,]+,','.'+key+' = '+value+',',body)
   elif key=='accuracy':body=re.sub(r'\.accuracy = [^,]+,','.accuracy = 0,',body)
 return '[MOVE_'+name+'] =\n    {'+body+'\n    }'
text=path.read_text();text=re.sub(r'\[MOVE_(\w+)\] =\n    \{(.*?)\n    \}',update,text,flags=re.S)
# Crunch switched from lowering Sp.Def to lowering Defense with the split.
start=text.index('[MOVE_CRUNCH]');end=text.index('\n    },',start)
text=text[:start]+text[start:end].replace('EFFECT_SPECIAL_DEFENSE_DOWN_HIT','EFFECT_DEFENSE_DOWN_HIT')+text[end:]
path.write_text(text,encoding='utf-8')
(ROOT/'data/move_stats_import.json').write_text(json.dumps({'source_commit':json.loads((source/'manifest.json').read_text())['commit'],'implemented_move_ids':[1,354],'numeric_fields':['power','accuracy','pp','priority'],'category_source':'damage_class_id','full_modern_effects':False,'full_gen9_learnsets':False},indent=2)+'\n')
print('Imported power, accuracy, PP, priority and category for 354 implemented moves; learnsets/new effects remain separate.')
