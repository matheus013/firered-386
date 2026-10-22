"""Audit requested level/machine learnsets against native move implementations."""
from pathlib import Path
import csv,json,re,collections
ROOT=Path(__file__).resolve().parents[1];SRC=ROOT/'data/upstream/pokeapi'
def rows(name):return list(csv.DictReader((SRC/(name+'.csv')).open(encoding='utf-8')))
versions={int(r['id']):r for r in rows('version_groups')}
moves={int(r['id']):r for r in rows('moves')}
native={n:int(i) for n,i in re.findall(r'^#define MOVE_(\w+)\s+(\d+)',(ROOT/'game/include/constants/moves.h').read_text(),re.M)}
native={n:i for n,i in native.items() if n!='UNAVAILABLE'}
by_mon=collections.defaultdict(lambda:collections.defaultdict(list))
for r in rows('pokemon_moves'):
 if int(r['pokemon_id'])<=386 and r['pokemon_move_method_id'] in ['1','4']:
  by_mon[int(r['pokemon_id'])][int(r['version_group_id'])].append(r)
result=[];missing=collections.Counter();excluded=collections.Counter()
EXCLUDED={'TERA_BLAST'} # User excludes Tera/Z/Max mechanics and dependent moves.
for species in range(1,387):
 available=by_mon[species]
 sv=[v for v in [25,26,27] if v in available and any(r['pokemon_move_method_id']=='1' for r in available[v])]
 candidates=sv or [v for v in available if int(versions[v]['generation_id'])<=9 and v!=32 and any(r['pokemon_move_method_id']=='1' for r in available[v])]
 version=max(candidates,key=lambda v:int(versions[v]['order']));learn=[]
 for row in available[version]:
  name=moves[int(row['move_id'])]['identifier'].replace('-','_').upper()
  if name in EXCLUDED:excluded[name]+=1
  else:learn.append(row)
 unsupported=[]
 for r in learn:
  move=moves[int(r['move_id'])];name=move['identifier'].replace('-','_').upper()
  implemented=name in native or (int(r['move_id'])<=354 and int(r['move_id']) in native.values())
  if not implemented:missing[name]+=1;unsupported.append(name)
 result.append({'national_dex':species,'version_group':versions[version]['identifier'],'level_moves':[{'level':int(r['level']),'move':moves[int(r['move_id'])]['identifier']} for r in learn if r['pokemon_move_method_id']=='1'],'machine_moves':sorted({moves[int(r['move_id'])]['identifier'] for r in learn if r['pokemon_move_method_id']=='4'}),'missing_native_moves':sorted(set(unsupported))})
report={'source_commit':json.loads((SRC/'manifest.json').read_text())['commit'],'species_count':len(result),'native_moves':len(native)-1,'missing_distinct_moves':len(missing),'excluded_mechanic_moves':dict(excluded),'full_gen9_learnsets_applied':False,'missing_moves':dict(sorted(missing.items())),'learnsets':result}
(ROOT/'data/gen9_coverage.json').write_text(json.dumps(report,indent=2)+'\n')
print('Species',len(result),'missing distinct native moves',len(missing),'fallback versions',dict(collections.Counter(r['version_group'] for r in result)))
