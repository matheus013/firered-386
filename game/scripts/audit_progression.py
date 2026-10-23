"""Static progression audit of the current ROM sources; no gameplay simulation."""
import json
import re
from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parent))
from balance_trainers import Data, parse, field

ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'game/output/analysis'
OUT.mkdir(parents=True,exist_ok=True)
teams=json.loads((ROOT/'game/hack-data/trainer_balance.json').read_text(encoding='utf-8'))['teams']
info={}
text=(ROOT/'game/src/data/pokemon/species_info.h').read_text(encoding='utf-8')
for name,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{\n(.*?)\n    \}',text,re.S):
 if field(body,'baseHP'):
  info[name]={k:int(field(body,k)) for k in ['baseHP','baseAttack','baseDefense','baseSpeed','baseSpAttack','baseSpDefense','expYield']}
  info[name]['bst']=sum(v for k,v in info[name].items() if k.startswith('base'))

def experience(mon):
 raw=info[mon['species']]['expYield']*mon['level']//7
 return raw*150//100

def summarize(team):
 return {'size':len(team),'min_level':min(m['level'] for m in team),'max_level':max(m['level'] for m in team),'mean_level':round(sum(m['level'] for m in team)/len(team),2),'mean_bst':round(sum(info[m['species']]['bst'] for m in team)/len(team),1),'participant_exp':sum(experience(m) for m in team),'reserve_exp':sum((info[m['species']]['expYield']*m['level']//7//2)*150//100 for m in team),'members':[{'species':m['species'],'level':m['level'],'iv_actual':m['iv']*31//255,'ev_total':sum(m.get('evs',[0]))} for m in team]}

bosses={k:summarize(v['team']) for k,v in teams.items() if any(s in k for s in ['sParty_Leader','sParty_EliteFour','sParty_Champion'])}
_,_,originals,_=parse()
oldsize=sum(len(originals[k]) for k in teams)
newsize=sum(len(t['team']) for t in teams.values())
changed_sizes=sum(len(originals[k])!=len(t['team']) for k,t in teams.items())

byid={t['id']:(k,t) for k,t in teams.items()}
stages={
 'before_brock':['Route22','ViridianForest','PewterCity_Gym'],
 'brock_to_misty':['Route3','MtMoon_1F','MtMoon_B1F','MtMoon_B2F','Route4','CeruleanCity','Route24','Route25','CeruleanCity_Gym'],
 'misty_to_surge':['Route5','Route6','VermilionCity_Gym',*[p.name for p in (ROOT/'game/data/maps').glob('SSAnne_*')]],
 'surge_to_erika':['Route9','Route10','RockTunnel_1F','RockTunnel_B1F','Route8','Route7','CeladonCity_Gym']
}
# Count one starter branch. Optional trainers are included; these are illustrative
# route budgets, not mandatory-path EXP or expected player levels.
route_budgets={}
for label,maps in stages.items():
 found={}
 for name in maps:
  path=ROOT/'game/data/maps'/name/'scripts.inc'
  if not path.exists():continue
  for trainer in set(re.findall(r'trainerbattle_\w+\s+(TRAINER_\w+)',path.read_text(encoding='utf-8'))):
   if trainer not in byid:continue
   k,t=byid[trainer]
   if t['class']=='TRAINER_CLASS_LEADER':continue
   if label=='before_brock' and 'Late' in k:continue
   if 'Rival' in k and not k.endswith('Squirtle'):continue
   found[trainer]=t
 party=[m for t in found.values() for m in t['team']]
 route_budgets[label]={'maps':maps,'trainers':len(found),'enemy_mons':len(party),'full_exp_per_fixed_participant':sum(experience(m) for m in party),'half_exp_per_fixed_reserve':sum((info[m['species']]['expYield']*m['level']//7//2)*150//100 for m in party),'ids':sorted(found)}

w=json.loads((ROOT/'game/src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]
rates=next(f['encounter_rates'] for f in w['fields'] if f['type']=='land_mons')
early={}
for row in w['encounters']:
 if row['map'] in ['MAP_ROUTE1','MAP_ROUTE2','MAP_ROUTE22','MAP_VIRIDIAN_FOREST'] and row['base_label'].endswith('_FireRed'):
  species={}
  for rate,mon in zip(rates,row['land_mons']['mons']):
   s=mon['species'].removeprefix('SPECIES_')
   species[s]=species.get(s,0)+rate
  early[row['map']]=species

# Quantify opening party status-move load and Protect usage, to identify pacing.
move_counts={}
for entry in teams.values():
 for m in entry['team']:
  for move in m['moves']:move_counts[move]=move_counts.get(move,0)+1
slots=newsize

def stat(base,level,iv=26,ev=0,nature=1.0,hp=False):
 n=(2*base+iv+ev//4)*level//100
 return n+level+10 if hp else int((n+5)*nature)
stat_examples={'kyogre_lv54_untrained_vs_e4':{'hp_untrained_iv26':stat(100,54,hp=True),'hp_e4_iv31_ev252':stat(100,54,31,252,hp=True),'spatk_untrained_iv26_neutral':stat(150,54),'spatk_e4_iv31_ev252_modest':stat(150,54,31,252,1.1)}}
# Illustrative no-wild/no-loss fixed-participant route. Optional fights included;
# captures, lead rotation, missed encounters and level caps are not simulated.
def medium_slow(level):
 return max(0,6*level**3//5-15*level**2+100*level-140)
def level_for(exp):
 return max(level for level in range(1,101) if medium_slow(level)<=exp)
running=medium_slow(5)
without_global=medium_slow(5)
example=[]
for route,boss in [('before_brock','sParty_LeaderBrock'),('brock_to_misty','sParty_LeaderMisty'),('misty_to_surge','sParty_LeaderLtSurge'),('surge_to_erika','sParty_LeaderErika')]:
 running+=route_budgets[route]['full_exp_per_fixed_participant']
 without_global+=route_budgets[route]['half_exp_per_fixed_reserve']
 example.append({'boss':boss,'exp_before_boss':running,'illustrative_normal_starter_level':level_for(running),'boss_max_level':bosses[boss]['max_level'],'same_teams_two_participants_global_off_level':level_for(without_global)})
 running+=bosses[boss]['participant_exp']
 without_global+=bosses[boss]['reserve_exp']
report={'scope':'static source analysis; not a campaign playtest','bosses':bosses,'trainer_population':{'teams':len(teams),'original_mons':oldsize,'current_mons':newsize,'parties_with_size_change':changed_sizes,'protect_mons':move_counts.get('PROTECT',0),'detect_mons':move_counts.get('DETECT',0),'mons_total':slots},'route_budgets':route_budgets,'pre_brock_land_encounters_percent':early,'hardcore_pre_brock_slots_max':len(early)+1,'hardcore_caps':[bosses['sParty_Leader'+n]['max_level'] for n in ['Brock','Misty','LtSurge','Erika','Koga','Sabrina','Blaine','Giovanni']]+[bosses['sParty_EliteFourLance']['max_level'],100],'stat_examples':stat_examples,'illustrative_normal_route':example}
(OUT/'progression.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'population':report['trainer_population'],'route_budgets':{k:{f:v for f,v in x.items() if f not in ['maps','ids']} for k,x in route_budgets.items()},'pre_brock_land_encounters_percent':early,'stat_examples':stat_examples,'illustrative_normal_route':example},ensure_ascii=False,indent=2))
print('Illustrative fixed-participant route:',example)
print('Boss curves:')
for k,v in bosses.items():print(k, v['min_level'],v['max_level'],'BST',v['mean_bst'],'EXP',v['participant_exp'])
