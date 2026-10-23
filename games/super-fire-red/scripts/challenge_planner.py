"""Conservative source-based availability and legal level-up-move planner.

Does not certify walking, capture success, encounter RNG or economy. No trading,
TM/tutor/egg moves, stone or friendship evolutions, held items or battle items.
"""
from pathlib import Path
import ast,collections,hashlib,json,random,re
from balance_trainers import Data,field,PHYSICAL_TYPES
from balance_encounters import method_stage,LEGENDS
ROOT=Path(__file__).resolve().parents[3]
NATURES='HARDY LONELY BRAVE ADAMANT NAUGHTY BOLD DOCILE RELAXED IMPISH LAX TIMID HASTY SERIOUS JOLLY NAIVE MODEST MILD QUIET BASHFUL RASH CALM GENTLE SASSY CAREFUL QUIRKY'.split()
DATA=Data();TEAMS=json.loads((ROOT/'games/super-fire-red/hack-data/trainer_balance.json').read_text(encoding='utf-8'))['teams']
INFO={}
for name,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{\n(.*?)\n    \}',(ROOT/'games/super-fire-red/src/data/pokemon/species_info.h').read_text(),re.S):
 if field(body,'baseHP'):
  INFO[name]={k:int(field(body,k)) for k in ['baseHP','baseAttack','baseDefense','baseSpeed','baseSpAttack','baseSpDefense']}
EVOL={}
for species,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{(.*?)(?=\n\s*\[SPECIES_|\n\};)',(ROOT/'games/super-fire-red/src/data/pokemon/evolution.h').read_text(),re.S):
 EVOL[species]=[(int(l),s) for l,s in re.findall(r'\{EVO_LEVEL,\s*(\d+),\s*SPECIES_(\w+)\}',body)]
CHART={}
text=(ROOT/'games/super-fire-red/src/battle_main.c').read_text();body=text[text.index('const u8 gTypeEffectiveness[336]'):];body=body[:body.index('};')]
for a,b,mult in re.findall(r'TYPE_(\w+),\s*TYPE_(\w+),\s*TYPE_MUL_(\w+)',body):CHART[a,b]={'SUPER_EFFECTIVE':2,'NOT_EFFECTIVE':0.5,'NO_EFFECT':0}[mult]

def effect(typ,species,ability=None):
 value=1
 for target in DATA.species[species]['types']:value*=CHART.get((typ,target),1)
 abilities={ability} if ability else DATA.species[species]['abilities']
 # Without scouting ability slot, use a conservative immunity if either slot has it.
 if (typ=='GROUND' and 'LEVITATE' in abilities) or (typ=='WATER' and 'WATER_ABSORB' in abilities) or (typ=='ELECTRIC' and 'VOLT_ABSORB' in abilities) or (typ=='FIRE' and 'FLASH_FIRE' in abilities):return 0
 if 'WONDER_GUARD' in abilities and value<=1:return 0
 return value

def stats(species,level,iv=15,evs=None,nature='HARDY'):
 evs=evs or [0]*6;s=INFO[species];out=[]
 ni=NATURES.index(nature);boost=ni//5+1;drop=ni%5+1
 for i,k in enumerate(['baseHP','baseAttack','baseDefense','baseSpeed','baseSpAttack','baseSpDefense']):
  n=(2*s[k]+iv+evs[i]//4)*level//100+(level+10 if i==0 else 5)
  if i and boost!=drop:n=n*(110 if i==boost else 90 if i==drop else 100)//100
  out.append(n)
 if species=='SHEDINJA':out[0]=1
 return out

def damage(species,level,move,target,targetlevel,iv=15,evs=None,nature='HARDY',target_training=None):
 m=DATA.moves[move];power=m['power'];e=effect(m['type'],target)
 if not e or power<=0:return 0
 if power==1:return {'DRAGON_RAGE':40,'SONIC_BOOM':20,'SEISMIC_TOSS':level,'NIGHT_SHADE':level}.get(move,0)
 a=stats(species,level,iv,evs,nature);t=target_training or {};d=stats(target,targetlevel,t.get('ivs',[20])[0],t.get('evs'),t.get('nature','HARDY'))
 attack,defense=(a[1],d[2]) if m['category'] == 'PHYSICAL' else (a[4],d[5])
 return (((2*level//5+2)*power*attack/max(1,defense)/50)+2)*e*(1.5 if m['type'] in DATA.species[species]['types'] else 1)*(m['accuracy'] or 100)/100

def stages():
 tree=ast.parse((ROOT/'games/super-fire-red/scripts/generate_guides.py').read_text())
 sequence=next(ast.literal_eval(n.value) for n in ast.walk(tree) if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='sequence' for t in n.targets))
 configs=[];badges=0
 stage_map={'RivalOaksLab':0,'RivalRoute22Early':1,'LeaderBrock':1,'RivalCerulean':2,'LeaderMisty':2,'RivalSsAnne':3,'LeaderLtSurge':3,'LeaderErika':4,'BossGiovanni':4,'RivalPokemonTower':4,'LeaderKoga':5,'BlackBeltKoichi':6,'RivalSilph':6,'BossGiovanni2':6,'LeaderSabrina':6,'LeaderBlaine':7,'BikerGoon':8,'BikerGoon2':8,'BikerGoon3':8,'CueBallPaxton':8,'LeaderGiovanni':8,'RivalRoute22Late':8}
 caps=[18,28,36,44,52,60,68,76]
 for index,(title,key,note) in enumerate(sequence):
  post=index>=27;league=(['EliteFourLorelei','EliteFourBruno','EliteFourAgatha','EliteFourLance','ChampionFirst'].index(key) if key in ['EliteFourLorelei','EliteFourBruno','EliteFourAgatha','EliteFourLance','ChampionFirst'] else ['EliteFourLorelei2','EliteFourBruno2','EliteFourAgatha2','EliteFourLance2','ChampionRematch'].index(key) if key in ['EliteFourLorelei2','EliteFourBruno2','EliteFourAgatha2','EliteFourLance2','ChampionRematch'] else 0)
  variants={k:v for k,v in TEAMS.items() if k.removeprefix('sParty_')==key or k.removeprefix('sParty_') in [key+s for s in ['Squirtle','Bulbasaur','Charmander']]}
  if key=='RivalOaksLab':
   variants={f'sParty_{key}{s}':{'id':f'TRAINER_RIVAL_OAKS_LAB_{s.upper()}','team':[{'species':s.upper(),'level':5}]} for s in ['Squirtle','Bulbasaur','Charmander']}
  assert variants,(key,title)
  cap=100 if post else caps[badges] if badges<8 else [80,83,86,89,92][league]
  config={'index':index,'title':title,'key':key,'stage':stage_map.get(key,9 if post else 8),'badges':badges,'cap':cap,'post':post,'league':league,'variants':variants}
  configs.append(config)
  if key.startswith('Leader'):badges+=1
 return configs

WILD=json.loads((ROOT/'games/super-fire-red/src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]
RATES={f['type']:f['encounter_rates'] for f in WILD['fields']}
MAPS={m['id']:m for p in (ROOT/'games/super-fire-red/data/maps').glob('*/map.json') if (m:=json.loads(p.read_text()))}
SOURCES=[]
for row in WILD['encounters']:
 if not row['base_label'].endswith('_FireRed'):continue
 for method in RATES:
  if method not in row:continue
  for slot,mon in enumerate(row[method]['mons']):
   SOURCES.append({'species':mon['species'].removeprefix('SPECIES_'),'map':row['map'],'section':MAPS[row['map']]['region_map_section'],'method':method,'slot':slot,'percent':RATES[method][slot],'stage':method_stage(row['map'],method,slot),'min_level':mon['min_level'],'max_level':mon['max_level']})

def eligible(source,step):
 if source['stage']>step['stage'] or source['min_level']>step['cap']:return False
 if source['species'] in LEGENDS and not step['post']:return False
 name=source['map']
 if step['key']=='RivalCerulean' and name in ['MAP_ROUTE24','MAP_ROUTE25']:return False
 if step['index']<=21 and ('VICTORY_ROAD' in name or name=='MAP_ROUTE23'):return False
 if step['index']<=29 and 'CERULEAN_CAVE' in name:return False
 # Surf before Sabrina is available, but sea routes behind Cinnabar require no
 # additional boss; supplied stage bands remain a conservative subset.
 return True

def evolved_forms(species,level,caught_level=1):
 yield species,[]
 for threshold,target in EVOL.get(species,[]):
  at=max(threshold,caught_level+1)
  if level>=at:
   for form,path in evolved_forms(target,level,at):yield form,[(species,at,target)]+path

def possible_moves(record,level,path):
 species=record['original'];start=record['origin'].get('min_level',record['origin'].get('level',5))
 initial=[move for learnt,move in DATA.learn.get(species,[]) if learnt<=start]
 # A wild/gift starts with the last four level-up moves, not every past move.
 known=set(initial[-4:])
 for old,at,new in path:
  known.update(move for learnt,move in DATA.learn.get(old,[]) if start<learnt<=at)
  known.update(move for learnt,move in DATA.learn.get(new,[]) if learnt==at)
  species=new;start=at
 known.update(move for learnt,move in DATA.learn.get(species,[]) if start<learnt<=level)
 return known

class Roster:
 def __init__(self,starter,mode,seed):
  self.starter=starter;self.mode=mode;self.rng=random.Random(seed);self.dna=random.Random(seed^0xDADA);self.caught=[];self.sections=set();self.dead=set();self.seen_species={starter};self.uid=0
  self.add(starter,{'kind':'starter','level':5})
 def add(self,species,origin):
  mon={'uid':self.uid,'original':species,'origin':origin,'ivs':[self.dna.randrange(32) for _ in range(6)],'nature':self.dna.choice(NATURES),'ability_num':self.dna.randrange(len(DATA.species[species]['ability_slots']))};self.uid+=1;self.caught.append(mon);self.seen_species.add(species)
 def advance(self,step):
  if step['stage']==0:return
  options=[s for s in SOURCES if eligible(s,step)]
  if self.mode==0:
   owned={c['original'] for c in self.caught}
   for s in sorted(options,key=lambda s:(s['stage'],s['min_level'],-s['percent'])):
    if s['species'] not in owned:self.add(s['species'],s);owned.add(s['species'])
  else:
   bysection=collections.defaultdict(list)
   for s in options:
    # One declared land-first choice per section; water/fishing only if no land
    # is available. Different rods are not mixed into one probability table.
    if s['section'] not in self.sections:bysection[s['section']].append(s)
   for section,entries in sorted(bysection.items()):
    pool=next((p for kind in ['land_mons','water_mons','fishing_mons','rock_smash_mons'] if (p:=[s for s in entries if s['method']==kind])),[])
    map_id=sorted({s['map'] for s in pool})[0];pool=[s for s in pool if s['map']==map_id]
    if pool and pool[0]['method']=='fishing_mons':pool=[s for s in pool if s['slot']<2] or [s for s in pool if s['slot'] in [2,3,4]]
    pool=[s for s in pool if s['species'] not in self.seen_species]
    if not pool:continue
    chosen=self.rng.choices(pool,weights=[s['percent'] for s in pool])[0];self.add(chosen['species'],chosen);self.sections.add(section)
  # Optional fixed gifts are outside wild quotas in the implemented rules.
  gifts=[(2,'MAGIKARP','purchase, Route4 Pokemon Center, 500'),(4,'EEVEE','Celadon gift'),(6,'HITMONLEE','Koichi reward'),(6,'LAPRAS','Silph gift after rival')]
  for stage,species,origin in gifts:
   allowed=step['stage']>=stage and (species!='HITMONLEE' or step['index']>11) and (species!='LAPRAS' or step['index']>12)
   if allowed and species not in {c['original'] for c in self.caught}:self.add(species,{'kind':'gift','source':origin,'level':5 if species=='MAGIKARP' else 25})
  if self.mode==0:
   for stage,species,location in [(6,'ZAPDOS','Power Plant fixed encounter'),(6,'ARTICUNO','Seafoam fixed encounter'),(8,'MOLTRES','Mt Ember fixed encounter')]:
    if step['stage']>=stage and species not in {c['original'] for c in self.caught}:self.add(species,{'kind':'static','source':location,'level':50})
 def candidates(self,step,enemy,training='baseline'):
  out=[];level=5 if step['stage']==0 else step['cap']
  for record in self.caught:
   if record['uid'] in self.dead:continue
   caught_level=record['origin'].get('min_level',record['origin'].get('level',5))
   for species,path in evolved_forms(record['original'],level,caught_level):
    iv=31 if (self.mode==1 and step['stage']>0) or step['post'] else 15
    trained=(self.mode==1 and step['stage']>0) or step['post'] or training=='farm'
    known=possible_moves(record,level,path);moves=choose_moves(species,level,enemy,known)
    assert all(m=='NONE' or m in known for m in moves)
    special=sum(DATA.moves[m]['power'] for m in moves if DATA.moves[m]['category'] != 'PHYSICAL')>sum(DATA.moves[m]['power'] for m in moves if DATA.moves[m]['category'] == 'PHYSICAL')
    evs=[0]*6;nature=record['nature']
    if trained:
     fast=INFO[species]['baseSpeed']>=85;offense=4 if special else 1;evs[offense]=252;evs[3 if fast else 0]=252;evs[0 if fast else 2]=4
     if self.mode==1 or step['post']:nature=('TIMID' if fast else 'MODEST') if special else ('JOLLY' if fast else 'ADAMANT')
    mon={'uid':record['uid'],'species':species,'level':level,'iv':iv,'ivs':([31]*6 if iv==31 else record['ivs']),'evs':evs,'nature':nature,'ability_num':0,'moves':(moves+['NONE']*4)[:4],'origin':record['origin'],'original':record['original'],'evolution':path,'manual_ev_farm':training=='farm' and not (self.mode==1 or step['post'])}
    # Keep the sampled capture's ability slot; the current editor cannot change it.
    abilities=DATA.species[species]['ability_slots']
    mon['ability_num']=min(record['ability_num'],len(abilities)-1)
    out.append(mon)
  return out

BANNED={'EXPLOSION','SELF_DESTRUCT','FISSURE','GUILLOTINE','HORN_DRILL','SHEER_COLD','HYPER_BEAM','BOUNCE','FLY','DIG','DIVE','SOLAR_BEAM','FOCUS_PUNCH','SKULL_BASH','RAZOR_WIND','BIDE','COUNTER','MIRROR_COAT','FLAIL','REVERSAL','ENDEAVOR','PRESENT','FUTURE_SIGHT','DOOM_DESIRE','METRONOME','ASSIST','MIMIC','MIRROR_MOVE','SKETCH','SLEEP_TALK','SNORE','DREAM_EATER','NIGHTMARE','RETURN','FRUSTRATION','THRASH','PETAL_DANCE','OUTRAGE','ROLLOUT','ICE_BALL','RAGE','FURY_CUTTER','SPIT_UP'}
def choose_moves(species,level,enemies,known=None):
 choices=({m for l,m in DATA.learn.get(species,[]) if l<=level} if known is None else known)-BANNED
 attacks=[m for m in choices if DATA.moves[m]['power']]
 selected=[];covered=[0]*len(enemies)
 while attacks and len(selected)<4:
  def score(move):
   values=[damage(species,level,move,e['species'],e['level'],target_training=e.get('runtime_training')) for e in enemies]
   return sum(max(0,v-covered[i]) for i,v in enumerate(values))+sum(values)*.08
  best=max(sorted(attacks),key=score);selected.append(best);attacks.remove(best)
  for i,e in enumerate(enemies):covered[i]=max(covered[i],damage(species,level,best,e['species'],e['level'],target_training=e.get('runtime_training')))
 support=next((m for m in ['SPORE','SLEEP_POWDER','RECOVER','MILK_DRINK','SOFT_BOILED','SYNTHESIS'] if m in choices),None)
 if support:
  if len(selected)==4:selected[-1]=support
  else:selected.append(support)
 if len(selected)<4 and 'GROWL' in choices:selected.append('GROWL')
 if not selected:selected=[next(iter(sorted(choices)),'NONE')]
 return selected

def select_team(candidates,enemies,variant=0):
 if len(candidates)==1:return candidates
 def strengths(mon):
  own=stats(mon['species'],mon['level'],mon['iv'],mon['evs'],mon['nature']);rows=[]
  for e in enemies:
   attack=max((damage(mon['species'],mon['level'],m,e['species'],e['level'],mon['iv'],mon['evs'],mon['nature'],e.get('runtime_training')) for m in mon['moves'] if m!='NONE'),default=0)
   training=e.get('runtime_training',{});incoming=max((damage(e['species'],e['level'],m,mon['species'],mon['level'],31,training.get('evs'),training.get('nature','HARDY'),{'ivs':[mon['iv']]*6,'evs':mon['evs'],'nature':mon['nature']}) for m in e.get('moves',[]) if m!='NONE'),default=20)
   ehp=stats(e['species'],e['level'],31,training.get('evs'),training.get('nature','HARDY'))[0]
   rows.append((attack/max(1,ehp))*(own[0]/max(10,incoming))**.65)
  return rows
 scored=[(m,strengths(m)) for m in candidates];team=[];covered=[0]*len(enemies)
 for slot in range(min(6,len(candidates))):
  available=[(m,s) for m,s in scored if m['uid'] not in {x['uid'] for x in team} and m['species'] not in {x['species'] for x in team}]
  if not available:break
  def utility(entry):
   m,s=entry
   opening=sum(s[:min(2,len(s))])*(.6 if slot<2 else .1)
   coverage=sum(max(0,v-covered[i]) for i,v in enumerate(s))
   diverse=1/(1+.13*sum(bool(DATA.species[m['species']]['types']&DATA.species[x['species']]['types']) for x in team))
   jitter=1+(.12*variant)*((int(hashlib.sha256((m['species']+str(slot)).encode()).hexdigest()[:4],16)%100)/100-.5)
   return (coverage+sum(s)*.12+opening)*diverse*jitter
  best=max(available,key=utility);team.append(best[0]);covered=[max(a,b) for a,b in zip(covered,best[1])]
 return team
