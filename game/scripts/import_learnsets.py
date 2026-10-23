"""Import generation-nine level-up learnsets and TM compatibility, limited to native moves.

Uses the version group chosen by audit_gen9_coverage.py (Scarlet/Violet, or the most recent
fallback for species absent there). Moves newer than generation III are skipped.
Evolution moves (level 0) are placed at the evolution level when the species evolves by level,
otherwise at level 1. A TM whose move does not exist in the source version group keeps its
original compatibility (a move is in the source when it is a machine there); HMs never lose compatibility so story progression is preserved.
"""
from pathlib import Path
import csv,json,re,collections
ROOT=Path(__file__).resolve().parents[2];SRC=ROOT/'game/hack-data/upstream/pokeapi';GAME=ROOT/'game'
def rows(name):return list(csv.DictReader((SRC/(name+'.csv')).open(encoding='utf-8')))
MAX_LEVEL_UP_MOVES=int(re.search(r'#define MAX_LEVEL_UP_MOVES\s+(\d+)',(GAME/'include/constants/pokemon.h').read_text()).group(1))
NATIVE_MAX=max(int(i) for i in re.findall(r'^#define MOVE_\w+\s+(\d+)',(GAME/'include/constants/moves.h').read_text(),re.M) if int(i)<1000)
move_ids={n:int(i) for n,i in re.findall(r'^#define MOVE_(\w+)\s+(\d+)',(GAME/'include/constants/moves.h').read_text(),re.M)}
move_names={i:n for n,i in move_ids.items()}
# PokeAPI and native IDs match up to Psycho Boost; later moves are matched by name.
native={int(r['id']):(int(r['id']) if int(r['id'])<=354 else move_ids.get(r['identifier'].replace('-','_').upper())) for r in csv.DictReader((ROOT/'game/hack-data/upstream/pokeapi/moves.csv').open(encoding='utf-8'))}
native={k:v for k,v in native.items() if v}
species_ids={n:int(i) for n,i in re.findall(r'^#define SPECIES_(\w+)\s+(\d+)',(GAME/'include/constants/species.h').read_text(),re.M)}
versions={r['identifier']:int(r['id']) for r in rows('version_groups')}
coverage=json.loads((ROOT/'game/hack-data/gen9_coverage.json').read_text())
dex_name={int(r['id']):r['identifier'] for r in rows('pokemon_species')}
def species_constant(dex):
 name=dex_name[dex].upper().replace('-','_')
 name={'NIDORAN_F':'NIDORAN_F','NIDORAN_M':'NIDORAN_M'}.get(name,name)
 assert name in species_ids,name
 return name
# Pokemon IDs 1-386 match native move IDs 1-354 and the default form of each species.
learn=collections.defaultdict(list);vg_moves=collections.defaultdict(set)
chosen={c['national_dex']:versions[c['version_group']] for c in coverage['learnsets']}
for r in rows('pokemon_moves'):
 pid,vg,move=int(r['pokemon_id']),int(r['version_group_id']),int(r['move_id'])
 if r['pokemon_move_method_id']=='4':vg_moves[vg].add(move)
 if pid<=386 and chosen.get(pid)==vg:learn[pid].append(r)
# Level at which each species is reached by a level-based evolution.
evo_level={}
for prevo,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{(.*?)\}\s*,\n',(GAME/'src/data/pokemon/evolution.h').read_text(),re.S):
 for method,param,target in re.findall(r'\{(EVO_\w+),\s*(\d+),\s*SPECIES_(\w+)\}',body):
  if method.startswith('EVO_LEVEL'):evo_level.setdefault(target,int(param))
# Level-up learnsets.
pointers=dict(re.findall(r'\[SPECIES_(\w+)\] = (s\w+LevelUpLearnset),',(GAME/'src/data/pokemon/level_up_learnset_pointers.h').read_text()))
path=GAME/'src/data/pokemon/level_up_learnsets.h';text=path.read_text()
report=[]
for dex in range(1,387):
 name=species_constant(dex);entries=[];seen=set()
 level_rows=[r for r in learn[dex] if r['pokemon_move_method_id']=='1']
 for r in sorted(level_rows,key=lambda r:(int(r['level']) or evo_level.get(name,1),int(r['order'] or 0))):
  move=int(r['move_id'])
  move=native.get(move)
  if move is None or move in seen:continue
  seen.add(move);entries.append((int(r['level']) or evo_level.get(name,1),move))
 entries.sort(key=lambda e:e[0])
 assert entries and len(entries)<=MAX_LEVEL_UP_MOVES,(name,len(entries))
 array=pointers[name]
 body=''.join('    LEVEL_UP_MOVE(%2d, MOVE_%s),\n'%(level,move_names[move]) for level,move in entries)
 text,count=re.subn(r'(static const u32 '+array+r'\[\] = \{\n).*?(    LEVEL_UP_END\n\};)',lambda m:m.group(1)+body+m.group(2),text,flags=re.S)
 assert count==(2 if name=='DEOXYS' else 1),array
 report.append({'species':name,'version_group':[k for k,v in versions.items() if v==chosen[dex]][0],'level_moves':len(entries)})
path.write_text(text,encoding='utf-8')
# TM/HM compatibility.
machines=re.findall(r'^#define ITEM_((?:TM|HM)\d\d_(\w+)) ITEM_(?:TM|HM)\d\d$',(GAME/'include/constants/items.h').read_text(),re.M)
assert len(machines)==58
path=GAME/'src/data/pokemon/tmhm_learnsets.h';text=path.read_text()
kept=collections.Counter()
def tmhm(match):
 prefix,name,body=match.groups()
 if name not in species_ids or not 1<=species_ids[name]<=411 or name.startswith('OLD_UNOWN'):return match.group(0)
 dex=next(d for d in range(1,387) if species_constant(d)==name)
 old=set(re.findall(r'TMHM\((\w+)\)',body));learned={int(r['move_id']) for r in learn[dex] if r['pokemon_move_method_id'] in ['1','3','4']}
 new=[]
 for token,move in machines:
  id=move_ids[move];present=id in vg_moves[chosen[dex]]
  if token.startswith('HM'):ok=token in old or id in learned
  elif present:ok=id in learned
  else:ok=token in old;kept[token]+=1
  if ok:new.append(token)
 if not new:return prefix+'0),'
 indent=' '*(len(prefix)-2)
 return prefix+('\n'+indent+'| ').join('TMHM(%s)'%t for t in new)+'),'
text=re.sub(r'(    \[SPECIES_(\w+)\]\s*= TMHM_LEARNSET\()(.*?)\),',tmhm,text,flags=re.S)
path.write_text(text,encoding='utf-8')
absent=sorted(t for t,move in machines if t.startswith('TM') and move_ids[move] not in vg_moves[versions['scarlet-violet']])
(ROOT/'game/hack-data/learnset_import.json').write_text(json.dumps({'source_commit':coverage['source_commit'],'max_level_up_moves':MAX_LEVEL_UP_MOVES,'native_move_limit':NATIVE_MAX,'tms_absent_from_scarlet_violet':absent,'species':report},indent=2)+'\n')
print('Imported level-up learnsets for',len(report),'species; TMs kept from generation III where absent in source:',', '.join(absent))
