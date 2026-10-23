"""Normal lab retries using the obtainable PC Potion through native Bag/party UI."""
import hashlib,json
from balance_core import NativeBench,ROOT,OUT
from challenge_planner import Roster,stages
dest=OUT/'lab-potion';core=NativeBench(output_dir=dest);rows=[];step=stages()[0]
rival={'BULBASAUR':'Charmander','CHARMANDER':'Squirtle','SQUIRTLE':'Bulbasaur'}
try:
 for n,starter in enumerate(rival):
  roster=Roster(starter,0,700+n);key='sParty_RivalOaksLab'+rival[starter];enemy=step['variants'][key]
  party=roster.candidates(step,enemy['team'])
  attempts=[core.battle(enemy['id'],0,0,0,False,party,9000+n*500+i,started=False,items=1) for i in range(5)]
  rows.append({'starter':starter,'trainer':enemy['id'],'team':party,'potion_source':'one Potion withdrawn from bedroom PC before choosing starter','attempts':attempts,'wins':sum(a['outcome']==1 for a in attempts)})
  print(starter,'with one native Potion:',rows[-1]['wins'],'/5',flush=True)
finally:
 (dest/'results.json').write_text(json.dumps({'rom_sha256':core.sha,'fixture_sha256':hashlib.sha256((core.path/'pokefirered.gba').read_bytes()).hexdigest(),'rows':rows},indent=2)+'\n')
 core.close()
