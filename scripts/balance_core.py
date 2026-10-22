"""Disposable native mGBA benchmark bridge (run in the WSL build environment)."""
from pathlib import Path
import hashlib,json,re,shutil,subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'output/tests/balance';OUT.mkdir(parents=True,exist_ok=True)

def constants(file,prefix):
 return {name:int(value,0) for name,value in re.findall(r'^#define\s+('+prefix+r'\w+)\s+(0x[\dA-Fa-f]+|\d+)\s*(?://[^\n]*)?$',(ROOT/'game/include/constants'/file).read_text(),re.M)}

class NativeBench:
 def __init__(self,build_dir=None,output_dir=None,reuse=False,baseline=False):
  self.out=Path(output_dir) if output_dir else OUT;self.out.mkdir(parents=True,exist_ok=True)
  source=Path((ROOT/'output/hack-build-path.txt').read_text().strip())
  self.sha=hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()
  assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==self.sha
  self.path=Path(build_dir) if build_dir else Path(tempfile.mkdtemp(prefix='balance-fixture-',dir=Path.home()/'.cache/firered-386'))
  provenance=self.path/'benchmark-base.json'
  if build_dir:
   if not provenance.exists() or json.loads(provenance.read_text()).get('rom_sha256')!=self.sha:
    raise ValueError('Benchmark fixture is stale or has no base-ROM provenance; create a fresh fixture from the current build.')
  else:
   shutil.copytree(source,self.path,dirs_exist_ok=True)
   provenance.write_text(json.dumps({'rom_sha256':self.sha,'source_build':str(source)},indent=2)+'\n')
  (self.out/'build-path.txt').write_text(str(self.path)+'\n')
  if not reuse:
   suffix='_baseline.c' if baseline else '.c'
   main=(source/'src/main.c').read_text();hook=(ROOT/('scripts/balance_fixture'+suffix)).read_text()
   (self.path/'src/main.c').write_text(main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(BenchBoot);'))
   player=(source/'src/battle_controller_player.c').read_text()
   player=player.replace('static void PlayerHandleGetMonData(void);',(ROOT/('scripts/balance_player'+suffix)).read_text()+'\nstatic void PlayerHandleGetMonData(void);')
   for original,replacement in [('PlayerHandleChooseAction','BenchChooseAction'),('PlayerHandleChooseMove','BenchChooseMove'),('PlayerHandleChoosePokemon','BenchChoosePokemon')]:
    start=player.index('static void '+original+'(void)\n{');opening=player.index('{',start);depth=1;end=opening+1
    while depth:depth+=(player[end]=='{')-(player[end]=='}');end+=1
    player=player[:start]+'static void '+original+'(void)\n{\n    '+replacement+'();\n}'+player[end:]
   player=player.replace('#include "constants/sound.h"','#include "constants/sound.h"\n#include "constants/abilities.h"\nstatic void PlayerBufferExecCompleted(void);')
   (self.path/'src/battle_controller_player.c').write_text(player)
   with (self.out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=self.path,stdout=log,stderr=subprocess.STDOUT,check=True)
  symbols={p[2]:int(p[0],16) for line in subprocess.check_output(['arm-none-eabi-nm',str(self.path/'pokefirered.elf')],text=True).splitlines() if len(p:=line.split())==3}
  runner=self.path/'balance-smoke'
  if not reuse:subprocess.run(['gcc','-O2','-Wall','-Wextra',str(ROOT/'scripts/balance_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
  self.stderr=(self.out/'emulator.log').open('w')
  self.command=[str(runner),str(self.path/'pokefirered.gba'),*[hex(symbols[name]) for name in ['gBenchCommand','gBenchReady','gBenchInput','gBenchOutput']],str(self.out/'battle')]
  self.process=subprocess.Popen(self.command,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=self.stderr,text=True,bufsize=1)
  self.species=constants('species.h','SPECIES_');self.moves=constants('moves.h','MOVE_');self.trainers=constants('opponents.h','TRAINER_');self.natures=constants('pokemon.h','NATURE_')
  self.items=constants('items.h','ITEM_')
  self.reverse_species={v:k.removeprefix('SPECIES_') for k,v in self.species.items()}
 def battle(self,trainer,mode,badges,league,post,party,seed=1,carry=False,heal=False,started=True,items=0,ethers=0,order=None,candies=0,menu_policy=False,status_turns=0):
  if order is not None:assert sorted(order)==list(range(6))
  values=[self.trainers[trainer],mode,badges,league,int(post),len(party),seed,int(carry),int(heal),int(started),items,ethers,sum(n<<(i*3) for i,n in enumerate(order)) if order else 0,int(order is not None),candies,int(menu_policy)]
  if menu_policy and order is None:values[12]=status_turns
  for mon in party:
   ivword=sum(value<<(5*i) for i,value in enumerate(mon['ivs'])) if 'ivs' in mon else mon['iv']
   ppword=sum(3<<(2*j) for j,move in enumerate(mon['moves']) if move!='NONE') if mon.get('pp_max') else 0
   values += [self.species['SPECIES_'+mon['species']],mon['level'],ivword,self.natures['NATURE_'+mon['nature']],mon.get('ability_num',0),self.items['ITEM_'+mon.get('item','NONE')]|(ppword<<16),*mon['evs'],*[self.moves['MOVE_'+m] for m in mon['moves']]]
  values += [0]*(112-len(values));assert len(values)==112
  self.process.stdin.write(' '.join(map(str,values))+'\n');self.process.stdin.flush()
  line=self.process.stdout.readline()
  if not line:raise RuntimeError('Benchmark stopped; inspect output/tests/balance/emulator.log')
  raw=list(map(int,line.split()));assert len(raw)==194,raw
  frames,ready,*out=raw
  result={'outcome':out[0],'turns':out[1],'faints':out[2],'cap':out[3],'party_size':out[4],'battle_healing_items':out[5],'frames':frames,'completed':ready==2,'between_items':dict(zip(['HYPER_POTION','REVIVE','FULL_HEAL','RARE_CANDY'],out[6:10])),'before':[],'after':[]}
  result['between_items']['ETHER']=out[10]
  for i in range(out[4]):
   m=out[16+i*20:36+i*20];p=out[136+i*8:144+i*8]
   result['before'].append({'species':self.reverse_species[m[0]],'level':m[1],'hp':m[2],'max_hp':m[3],'attack':m[4],'defense':m[5],'speed':m[6],'spattack':m[7],'spdefense':m[8],'exp':m[9],'ev_total':m[10],'iv_total':m[11]})
   result['after'].append({'species':self.reverse_species[p[0]],'level':p[1],'hp':p[2],'max_hp':p[3],'status':p[4],'dead':bool(p[5]),'exp':p[6],'pp':[(p[7]>>(8*j))&255 for j in range(4)]})
  if not result['completed']:
   self.process.stdin.close();self.process.wait(timeout=20)
   self.process=subprocess.Popen(self.command,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=self.stderr,text=True,bufsize=1)
  if not carry:
   for expected,actual in zip(party,result['before']):
    assert actual['species']==expected['species'] and actual['level']==expected['level'],(expected,actual)
    assert actual['hp']==actual['max_hp'] and actual['ev_total']==sum(expected['evs'])
    assert actual['iv_total']==sum(expected.get('ivs',[expected['iv']]*6))
  return result
 def close(self):
  self.process.stdin.close();self.process.wait(timeout=20);self.stderr.close()
  assert hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()==self.sha

if __name__=='__main__':
 import argparse
 parser=argparse.ArgumentParser();parser.add_argument('--build-dir');parser.add_argument('--baseline',action='store_true');args=parser.parse_args()
 core=NativeBench(args.build_dir,baseline=args.baseline)
 print('Built benchmark fixture:',core.path,'base ROM SHA-256:',core.sha,flush=True)
 core.close()
