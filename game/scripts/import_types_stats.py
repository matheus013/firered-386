"""Import generation-nine species types/base stats, move types and the type chart, including Fairy.

Only the 386 native species and 354 native moves are touched. The chart keeps the native
Foresight separator: Normal/Fighting -> Ghost immunities stay after it.
"""
from pathlib import Path
import csv,json,re
ROOT=Path(__file__).resolve().parents[2];SRC=ROOT/'game/hack-data/upstream/pokeapi';GAME=ROOT/'game'
def rows(name):return list(csv.DictReader((SRC/(name+'.csv')).open(encoding='utf-8')))
TYPES={int(r['id']):r['identifier'].upper() for r in rows('types') if int(r['id'])<=18}
STATS={1:'baseHP',2:'baseAttack',3:'baseDefense',4:'baseSpAttack',5:'baseSpDefense',6:'baseSpeed'}
species_ids={n:int(i) for n,i in re.findall(r'^#define SPECIES_(\w+)\s+(\d+)',(GAME/'include/constants/species.h').read_text(),re.M)}
move_ids={n:int(i) for n,i in re.findall(r'^#define MOVE_(\w+)\s+(\d+)',(GAME/'include/constants/moves.h').read_text(),re.M)}
dex_name={int(r['id']):r['identifier'].upper().replace('-','_') for r in rows('pokemon_species') if int(r['id'])<=386}
assert all(n in species_ids for n in dex_name.values())
types={};stats={}
for r in rows('pokemon_types'):
 if int(r['pokemon_id'])<=386:types.setdefault(int(r['pokemon_id']),{})[int(r['slot'])]=TYPES[int(r['type_id'])]
for r in rows('pokemon_stats'):
 if int(r['pokemon_id'])<=386:stats.setdefault(int(r['pokemon_id']),{})[STATS[int(r['stat_id'])]]=int(r['base_stat'])
changed={'types':[],'stats':[]}
path=GAME/'src/data/pokemon/species_info.h';text=path.read_text(encoding='utf-8')
for dex,name in dex_name.items():
 start=text.index('    [SPECIES_%s] =\n'%name);end=text.index('\n    },',start);body=text[start:end]
 t=types[dex];new=body
 pair='{TYPE_%s, TYPE_%s}'%(t[1],t.get(2,t[1]))
 new=re.sub(r'\.types = \{[^}]*\}','.types = '+pair,new)
 for field,value in stats[dex].items():new=re.sub(r'\.'+field+r' = \d+',f'.{field} = {value}',new)
 if re.search(r'\.types = \{[^}]*\}',body)[0]!='.types = '+pair:changed['types'].append(name)
 if any(re.search(r'\.'+f+r' = (\d+)',body)[1]!=str(v) for f,v in stats[dex].items()):changed['stats'].append(name)
 text=text[:start]+new+text[end:]
path.write_text(text,encoding='utf-8')
# Move types; Curse keeps its Ghost-user logic regardless of type.
moves={int(r['id']):r for r in rows('moves')}
path=GAME/'src/data/battle_moves.h';text=path.read_text(encoding='utf-8');move_changes=[]
def move_type(match):
 name,body=match.groups();id=move_ids[name]
 if not 1<=id<=354:return match.group(0)
 wanted='TYPE_'+TYPES[int(moves[id]['type_id'])]
 old=re.search(r'\.type = (\w+),',body)[1]
 if old!=wanted:move_changes.append((name,old,wanted))
 return '[MOVE_'+name+'] =\n    {'+re.sub(r'\.type = \w+,','.type = '+wanted+',',body)+'\n    }'
text=re.sub(r'\[MOVE_(\w+)\] =\n    \{(.*?)\n    \}',move_type,text,flags=re.S)
path.write_text(text,encoding='utf-8')
# Type chart: only non-neutral pairs, Ghost immunities of Normal/Fighting after the Foresight marker.
mul={0:'TYPE_MUL_NO_EFFECT',50:'TYPE_MUL_NOT_EFFECTIVE',200:'TYPE_MUL_SUPER_EFFECTIVE'}
entries=[];foresight=[]
for r in rows('type_efficacy'):
 atk,dfn,factor=int(r['damage_type_id']),int(r['target_type_id']),int(r['damage_factor'])
 if atk>18 or dfn>18 or factor==100:continue
 line='    TYPE_%s, TYPE_%s, %s,'%(TYPES[atk],TYPES[dfn],mul[factor])
 (foresight if TYPES[dfn]=='GHOST' and TYPES[atk] in('NORMAL','FIGHTING') else entries).append(line)
lines=entries+['    TYPE_FORESIGHT, TYPE_FORESIGHT, TYPE_MUL_NO_EFFECT,']+foresight+['    TYPE_ENDTABLE, TYPE_ENDTABLE, TYPE_MUL_NO_EFFECT']
assert len(lines)<=128,'Conversion 2 samples entries with Random() % 128'
path=GAME/'src/battle_main.c';text=path.read_text(encoding='utf-8')
text=re.sub(r'const u8 gTypeEffectiveness\[\d*\] =\n\{\n.*?\n\};','const u8 gTypeEffectiveness[%d] =\n{\n%s\n};'%(len(lines)*3,'\n'.join(lines)),text,count=1,flags=re.S)
path.write_text(text,encoding='utf-8')
header=GAME/'include/battle_main.h';h=header.read_text(encoding='utf-8')
header.write_text(re.sub(r'extern const u8 gTypeEffectiveness\[\d+\];','extern const u8 gTypeEffectiveness[%d];'%(len(lines)*3),h),encoding='utf-8')
(ROOT/'game/hack-data/types_stats_import.json').write_text(json.dumps({'source_commit':json.loads((SRC/'manifest.json').read_text())['commit'],'species_type_changes':changed['types'],'species_stat_changes':changed['stats'],'move_type_changes':move_changes,'chart_entries':len(lines)-2},indent=2)+'\n')
print('Types changed:',len(changed['types']),'stats changed:',len(changed['stats']),'move types:',move_changes,'chart entries:',len(lines)-2)
