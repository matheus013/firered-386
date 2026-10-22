"""Optional preparation with verified shops, gifts, stones and limited TM19."""
import copy,re
from challenge_planner import Roster,ROOT,DATA,INFO,EVOL,evolved_forms,possible_moves,choose_moves,stats,damage
TM_COINS={'ICE_BEAM':4000,'IRON_TAIL':3500,'THUNDERBOLT':4000,'SHADOW_BALL':4500,'FLAMETHROWER':4000}
REPEAT=set(TM_COINS)|{'BRICK_BREAK'}
STONES={}
text=(ROOT/'game/src/data/pokemon/evolution.h').read_text()
for species,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{(.*?)(?=\n\s*\[SPECIES_|\n\};)',text,re.S):
 STONES[species]=[(stone,target) for stone,target in re.findall(r'\{EVO_ITEM, ITEM_(\w+), SPECIES_(\w+)\}',body) if stone in ['FIRE_STONE','THUNDER_STONE','WATER_STONE','LEAF_STONE','MOON_STONE']]

class AdvancedRoster(Roster):
 def __init__(self,*args):
  super().__init__(*args)
  self.static_sections={'MAPSEC_ROUTE_16','MAPSEC_POWER_PLANT','MAPSEC_SEAFOAM_ISLANDS','MAPSEC_MT_EMBER'}
  if self.mode:self.sections.update(self.static_sections)
 def advance(self,step):
  super().advance(step)
  available={c['original'] for c in self.caught}
  gifts=[(4,'ABRA',9,180,'Game Corner purchase'),(4,'CLEFAIRY',8,500,'Game Corner purchase'),(4,'DRATINI',18,2800,'Game Corner purchase'),(4,'SCYTHER',25,5500,'Game Corner purchase'),(4,'PORYGON',26,6500,'Game Corner purchase'),(7,'AERODACTYL',5,0,'Old Amber revived in Cinnabar; Cut needed to collect Amber'),(7,'OMANYTE',5,0,'Helix Fossil chosen in Mt Moon, revived in Cinnabar')]
  for stage,species,level,coins,source in gifts:
   if step['stage']>=stage and species not in available:
    self.add(species,{'kind':'gift_or_purchase','level':level,'source':source,'coins':coins});available.add(species)
  for stage,species,section,level,source in [(5,'SNORLAX','MAPSEC_ROUTE_16',30,'fixed Route16 Snorlax after Poke Flute'),(6,'ZAPDOS','MAPSEC_POWER_PLANT',50,'fixed Power Plant Zapdos'),(6,'ARTICUNO','MAPSEC_SEAFOAM_ISLANDS',50,'fixed Seafoam Articuno'),(8,'MOLTRES','MAPSEC_MT_EMBER',50,'fixed Mt Ember Moltres')]:
   if step['stage']>=stage and species not in available:
    self.add(species,{'kind':'planned_static','section':section,'level':level,'source':source,'quota_strategy':'reserve this section; Repel with lead above all ordinary wild levels; capture success assumed'});available.add(species)
 def candidates(self,step,enemy,training='farm'):
  pool=super().candidates(step,enemy,training)
  level=step['cap'];byuid={r['uid']:r for r in self.caught}
  extra=[]
  for mon in pool:
   record=byuid[mon['uid']]
   if step['stage']>=4:
    for stone,target in STONES.get(mon['species'],[]):
     if stone=='MOON_STONE' and step['stage']<2:continue
     clone=copy.deepcopy(mon);clone['species']=target;clone['evolution_item']=stone
     at=mon['evolution'][-1][1] if mon['evolution'] else record['origin'].get('min_level',record['origin'].get('level',5))
     known=possible_moves(record,at,mon['evolution']);known.update(m for l,m in DATA.learn.get(target,[]) if at<=l<=level)
     clone['known_moves']=known;extra.append(clone)
  pool+=extra
  for mon in pool:
   record=byuid[mon['uid']];known=mon.pop('known_moves',possible_moves(record,level,mon['evolution']))
   shop=(DATA.tm.get(mon['species'],set())&REPEAT) if step['stage']>=4 else set()
   gifts=({'GIGA_DRAIN'}&DATA.tm.get(mon['species'],set())) if step['index']>=8 else set()
   hm=({'SURF'}&DATA.tm.get(mon['species'],set())) if step['badges']>=5 else set()
   moves=choose_moves(mon['species'],level,enemy,known|shop|gifts|hm)
   mon['moves']=(moves+['NONE']*4)[:4];mon['tm_moves']=[m for m in moves if m not in known and m in shop|gifts];mon['hm_moves']=[m for m in moves if m not in known and m in hm]
   mon['tm_coins']=sum(TM_COINS.get(m,0) for m in mon['tm_moves']);mon['single_tm19']='GIGA_DRAIN' in mon['tm_moves']
   special=sum(DATA.moves[m]['power'] for m in moves if DATA.moves[m]['type'] not in {'NORMAL','FIGHTING','FLYING','POISON','GROUND','ROCK','BUG','GHOST','STEEL'})>sum(DATA.moves[m]['power'] for m in moves if DATA.moves[m]['type'] in {'NORMAL','FIGHTING','FLYING','POISON','GROUND','ROCK','BUG','GHOST','STEEL'})
   fast=INFO[mon['species']]['baseSpeed']>=85;off=4 if special else 1
   mon['evs']=[0]*6;mon['evs'][off]=252;mon['evs'][3 if fast else 0]=252;mon['evs'][0 if fast else 2]=4
   if self.mode or step['post']:mon['nature']=('TIMID' if fast else 'MODEST') if special else ('JOLLY' if fast else 'ADAMANT')
  return pool

def compose(pool,foes,manual=None):
 def strength(mon):
  own=stats(mon['species'],mon['level'],mon['iv'],mon['evs'],mon['nature']);values=[]
  for foe in foes:
   training=foe.get('runtime_training',{});enemy=stats(foe['species'],foe['level'],31,training.get('evs'),training.get('nature','HARDY'))
   dealt=max((damage(mon['species'],mon['level'],m,foe['species'],foe['level'],mon['iv'],mon['evs'],mon['nature'],training) for m in mon['moves'] if m!='NONE'),default=0)
   taken=max((damage(foe['species'],foe['level'],m,mon['species'],mon['level'],31,training.get('evs'),training.get('nature','HARDY'),{'ivs':[mon['iv']]*6,'evs':mon['evs'],'nature':mon['nature']}) for m in foe.get('moves',[]) if m!='NONE'),default=20)
   speed=1.3 if own[3]>=enemy[3] else .8
   values.append(dealt/max(1,enemy[0])*(own[0]/max(10,taken))**.6*speed)
  return values
 ranked=[(m,strength(m)) for m in pool];selected=[];covered=[0]*len(foes)
 for index in range(6):
  options=[(m,v) for m,v in ranked if m['uid'] not in {x['uid'] for x in selected} and m['species'] not in {x['species'] for x in selected} and not(m.get('single_tm19') and any(x.get('single_tm19') for x in selected)) and not(m.get('evolution_item')=='MOON_STONE' and sum(x.get('evolution_item')=='MOON_STONE' for x in selected)>=2)]
  if not options:break
  if manual and index<len(manual) and any(m['species']==manual[index] for m,v in options):options=[(m,v) for m,v in options if m['species']==manual[index]]
  best=max(options,key=lambda pair:sum(max(0,v-covered[i]) for i,v in enumerate(pair[1]))+sum(pair[1])*.18+sum(pair[1][:2])*(.5 if index<2 else .05));selected.append(copy.deepcopy(best[0]));covered=[max(a,b) for a,b in zip(covered,best[1])]
 assert len(selected)==6
 for mon in sorted(selected,key=lambda m:stats(m['species'],m['level'],m['iv'],m['evs'],m['nature'])[0],reverse=True)[:2]:mon['item']='LEFTOVERS'
 assert sum(m.get('single_tm19',False) for m in selected)<=1
 return selected
