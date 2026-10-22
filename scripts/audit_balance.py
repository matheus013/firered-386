"""Reproducible balance measurements; no battle/campaign win-rate simulation.

Run: python scripts/audit_balance.py
Requires the locally built ROM and matching published feature-test reference.
"""
from pathlib import Path
import argparse,collections,hashlib,json,re,struct,subprocess
from fractions import Fraction
from balance_trainers import Data,field

ROOT=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--verify-rom-tables',action='store_true',help='WSL: compare all 600 modeled EXP thresholds with the compiled ROM using cached ELF symbols')
args=parser.parse_args()
OUT=ROOT/'output/analysis';OUT.mkdir(parents=True,exist_ok=True)
data=Data();catalog=json.loads((ROOT/'data/trainer_balance.json').read_text(encoding='utf-8'))
teams=catalog['teams'];info={}
for name,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{\n(.*?)\n    \}',(ROOT/'game/src/data/pokemon/species_info.h').read_text(encoding='utf-8'),re.S):
 if field(body,'baseHP'):
  info[name]={k:int(field(body,k)) for k in ['baseHP','baseAttack','baseDefense','baseSpeed','baseSpAttack','baseSpDefense','expYield']}
  info[name]['growth']=field(body,'growthRate').removeprefix('GROWTH_')
  info[name]['bst']=sum(v for k,v in info[name].items() if k.startswith('base'))

def exp_at(n,g):
 if n==1:return 1  # Native tables explicitly use 1 for every growth group.
 if g=='FAST':return 4*n**3//5
 if g=='SLOW':return 5*n**3//4
 if g=='MEDIUM_FAST':return n**3
 if g=='MEDIUM_SLOW':return 6*n**3//5-15*n*n+100*n-140
 if g=='ERRATIC':
  return (100-n)*n**3//50 if n<=50 else (150-n)*n**3//100 if n<=68 else ((1911-10*n)//3)*n**3//500 if n<=98 else (160-n)*n**3//100
 if g=='FLUCTUATING':return ((n+1)//3+24)*n**3//50 if n<=15 else (n+14)*n**3//50 if n<=36 else (n//2+32)*n**3//50
 raise ValueError(g)

def level_at(exp,g):return max(n for n in range(1,101) if exp_at(n,g)<=exp)
native_tables={'status':'not_run','values_checked':0}
if args.verify_rom_tables:
 cache=Path((ROOT/'output/hack-build-path.txt').read_text().strip())
 rom=(ROOT/'output/firered-386-dev.gba').read_bytes()
 assert rom==(cache/'pokefirered.gba').read_bytes()
 symbols={p[2]:int(p[0],16) for line in subprocess.check_output(['arm-none-eabi-nm',str(cache/'pokefirered.elf')],text=True).splitlines() if len(p:=line.split())==3}
 offset=symbols['gExperienceTables']-0x08000000
 constants=(ROOT/'game/include/constants/pokemon.h').read_text(encoding='utf-8')
 for name,index in re.findall(r'^#define GROWTH_(\w+)\s+(\d+)\s*$',constants,re.M):
  for level in range(1,101):
   actual=struct.unpack_from('<I',rom,offset+(int(index)*101+level)*4)[0]
   assert actual==exp_at(level,name),(name,level,actual,exp_at(level,name))
   native_tables['values_checked']+=1
 assert native_tables['values_checked']==600
 native_tables['status']='passed'
def reward(mon,reserve=False):
 n=info[mon['species']]['expYield']*mon['level']//7
 return (n//2 if reserve else n)*150//100
def effective(mon):
 return mon.get('runtime_training',{'ivs':[mon['iv']*31//255]*6,'evs':mon.get('evs',[0]*6),'nature':mon.get('nature','unknown')})
def summary(key):
 team=teams[key]['team']
 return {'size':len(team),'min_level':min(m['level'] for m in team),'max_level':max(m['level'] for m in team),'mean_bst':round(sum(info[m['species']]['bst'] for m in team)/len(team),1),'protect_detect':sum(bool({'PROTECT','DETECT'}&set(m['moves'])) for m in team),'members':[{'species':m['species'],'level':m['level'],'moves':m['moves'],'training':effective(m)} for m in team]}

bosskeys=['sParty_Leader'+x for x in ['Brock','Misty','LtSurge','Erika','Koga','Sabrina','Blaine','Giovanni']]
league=['sParty_EliteFour'+x for x in ['Lorelei','Bruno','Agatha','Lance']]
champs=[k for k in teams if k.startswith('sParty_ChampionFirst')]
bosses={k:summary(k) for k in bosskeys+league+champs}
league_models=[]
for growth in ['FAST','MEDIUM_FAST','MEDIUM_SLOW','SLOW','ERRATIC','FLUCTUATING']:
 for position in ['lead_every_foe','reserve_every_foe']:
  for initial in ['level80_bottom','level80_max_bar']:
   xp=exp_at(80,growth) if initial.endswith('bottom') else exp_at(81,growth)-1
   rows=[]
   for key in league:
    cap=bosses[key]['max_level'];before=level_at(xp,growth)
    for mon in teams[key]['team']:xp=min(exp_at(cap+1,growth)-1,xp+reward(mon,position.startswith('reserve')))
    rows.append({'battle':key,'cap':cap,'before':before,'after':level_at(xp,growth),'xp':xp})
   league_models.append({'growth':growth,'position':position,'initial':initial,'battles':rows,'before_champion_level':level_at(xp,growth),'champion_max':92})

wild=json.loads((ROOT/'game/src/data/wild_encounters.json').read_text(encoding='utf-8'))['wild_encounter_groups'][0]
rates=next(f['encounter_rates'] for f in wild['fields'] if f['type']=='land_mons')
early={}
for row in wild['encounters']:
 if row['map'] in ['MAP_ROUTE1','MAP_ROUTE22','MAP_ROUTE2','MAP_VIRIDIAN_FOREST'] and row['base_label'].endswith('_FireRed'):
  options={}
  for weight,mon in zip(rates,row['land_mons']['mons']):
   name=mon['species'].removeprefix('SPECIES_')
   item=options.setdefault(name,{'percent':0,'min':mon['min_level'],'max':mon['max_level']})
   item['percent']+=weight;item['min']=min(item['min'],mon['min_level']);item['max']=max(item['max'],mon['max_level'])
  early[row['map']]=options
assert len(early)==4 and all(sum(v['percent'] for v in options.values())==100 for options in early.values())

# Ideal catches with exact-species duplicate clause, Route1 -> Route22 -> Route2
# -> Forest. This conditions on an eligible species eventually appearing and
# assumes every eligible catch succeeds. It is not a real run probability.
order=['MAP_ROUTE1','MAP_ROUTE22','MAP_ROUTE2','MAP_VIRIDIAN_FOREST']
dp={frozenset():Fraction(1)}
for area in order:
 nxt=collections.defaultdict(Fraction)
 for caught,p in dp.items():
  choices={s:v['percent'] for s,v in early[area].items() if s not in caught};total=sum(choices.values())
  if not total:nxt[caught]+=p
  else:
   for s,w in choices.items():nxt[caught|{s}]+=p*Fraction(w,total)
 dp=nxt
assert sum(dp.values())==1
coverage={}
for typ in ['GRASS','WATER','FIGHTING','GROUND']:
 coverage[typ]=round(100*float(sum(p for caught,p in dp.items() if any(typ in data.species[s]['types'] for s in caught))),3)

def calc(base,level,iv,ev,nature=100,hp=False):
 raw=(2*base+iv+ev//4)*level//100
 return raw+level+10 if hp else (raw+5)*nature//100
stat_examples=[]
for key,slots in [('sParty_LeaderBrock',[0,5]),('sParty_LeaderMisty',[5]),('sParty_LeaderSabrina',[5])]:
 for slot in slots:
  mon=teams[key]['team'][slot];training=effective(mon);s=info[mon['species']];n=mon['level'];ev=training['evs'];nat=training['nature']
  buffs={'ADAMANT':('baseAttack','baseSpAttack'),'MODEST':('baseSpAttack','baseAttack'),'JOLLY':('baseSpeed','baseSpAttack'),'TIMID':('baseSpeed','baseAttack'),'BOLD':('baseDefense','baseAttack'),'CALM':('baseSpDefense','baseAttack')}
  row={'species':mon['species'],'level':n,'nature':nat,'stats':{}}
  for i,k in enumerate(['baseHP','baseAttack','baseDefense','baseSpeed','baseSpAttack','baseSpDefense']):
   pair=buffs.get(nat,('',''));mult=110 if k==pair[0] else 90 if k==pair[1] else 100
   base=calc(s[k],n,15,0,hp=i==0);trained=calc(s[k],n,31,ev[i],mult,hp=i==0)
   row['stats'][k]={'neutral_iv15_ev0':base,'enemy_effective':trained,'increase_percent':round(100*(trained/base-1),1)}
  stat_examples.append(row)

population=[m for t in teams.values() for m in t['team']]
source_files=['data/trainer_balance.json','game/src/data/pokemon/species_info.h','game/src/data/wild_encounters.json','game/src/elite_four.c','game/src/nuzlocke.c','game/src/training.c','game/src/battle_script_commands.c']
rom_sha=hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert json.loads((ROOT/'data/feature_tests/features.json').read_text(encoding='utf-8'))['rom_sha256']==rom_sha, 'Audit requires matching validated ROM'
report={'version':'v0.4.0-dev','rom_sha256':rom_sha,'source_sha256':{name:hashlib.sha256((ROOT/name).read_bytes()).hexdigest() for name in source_files},'scope':'static source/data audit and explicit EXP/stat models; no campaign or battle win-rate simulation','bosses':bosses,'early_encounters':early,'hardcore_pre_brock_max_party':5,'ideal_catch_type_coverage_percent':coverage,'ideal_catch_model':{'areas':order,'assumptions':'all eligible catches succeed, exact-species dupes skipped indefinitely, starter excluded, no fishing or gifts'},'league_exp_models':league_models,'stat_examples':stat_examples,'population':{'regular_parties':len(teams),'mons':len(population),'protect_detect_mons':sum(bool({'PROTECT','DETECT'}&set(m['moves'])) for m in population),'mons_with_runtime_training':sum('runtime_training' in m for m in population)},'constraints':{'gym_caps':[bosses[k]['max_level'] for k in bosskeys],'first_league_caps':[bosses[k]['max_level'] for k in league]+[92],'nurse_total_max':19*3,'nurse_limit_covers_pc_or_beds':False,'normal_editor_before_league':False,'hardcore_editor_before_league':True,'normal_candy_1_in_shops':False,'hardcore_candy_1_in_shops':True}}
report['league_exp_budget']={key:{'fixed_participant':sum(reward(m) for m in teams[key]['team']),'fixed_reserve':sum(reward(m,True) for m in teams[key]['team'])} for key in league}
report['compiled_rom_exp_thresholds']=native_tables
(OUT/'balance-v0.4.0.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps({k:report[k] for k in ['population','constraints','early_encounters','ideal_catch_type_coverage_percent','stat_examples']},ensure_ascii=False,indent=2))
print('League before champion:',[(r['growth'],r['position'],r['initial'],r['before_champion_level']) for r in league_models])
print('Compiled EXP table check:',native_tables)
