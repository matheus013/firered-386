"""Publishable battle/recipe report; retain strict separation from natural play."""
from pathlib import Path
import copy,gzip,hashlib,json,statistics
from functools import lru_cache
from challenge_planner import stages
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'output/tests/balance';DEST=ROOT/'data/challenge_balance';DEST.mkdir(parents=True,exist_ok=True)
SHA=hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()
advanced_run=json.loads((OUT/'advanced-run.json').read_text())
assert all(job['exit_code']==0 for job in advanced_run['jobs'])
assert len(list(OUT.glob('pickup-*/results.json')))==6
@lru_cache(maxsize=64)
def read(path):
 data=json.loads(path.read_text(encoding='utf-8'));assert data['rom_sha256']==SHA,path;return data
def label(value):return value.replace('_',' ').title()
def stats(rows):
 selected=[]
 for row in rows:
  tests=row['farm_attempts'] if row['preparation']=='manual_ev_farm_508' and row['farm_attempts'] else row['baseline_attempts'];selected+=tests
 wins=[t for t in selected if t['outcome']==1]
 return {'wins':len(wins),'attempts':len(selected),'zero_faint_wins':sum(t['faints']==0 for t in wins),'mean_faints_on_win':round(statistics.mean(t['faints'] for t in wins),2) if wins else None,'mean_turns_on_win':round(statistics.mean(t['turns'] for t in wins),2) if wins else None,'grades':sorted(set(r['difficulty'] for r in rows))}
datasets={};archive=[]
for prefix in ['final-normal','final-hardcore','prepared-normal']:
 rows=[]
 paths=sorted(OUT.glob(prefix+'-*/challenge-results.json'));assert len(paths)==3,(prefix,paths)
 for path in paths:
  data=read(path);assert len(data['rows'])==35,path;rows+=data['rows'];datasets[path.parent.name]=data
 assert len(rows)==105
def grade(tests,mode):
 wins=[t for t in tests if t['outcome']==1]
 if not wins:return 'Não demonstrada'
 faints=statistics.mean(t['faints'] for t in wins);turns=statistics.mean(t['turns'] for t in wins)
 if len(wins)==len(tests) and faints==0 and turns<=18:return 'Baixa nesta bateria'
 if len(wins)>=len(tests)*.8 and faints<=(0 if mode else 1):return 'Moderada'
 return 'Alta' if len(wins)>=len(tests)*.6 else 'Muito alta / instável'
# Supersede early compositions and the ordinary-flags laboratory simulation.
for name,data in datasets.items():
 data['rows']=[r for r in data['rows'] if r['index']>=10]
for path in OUT.glob('early-*/results.json'):
 data=read(path);assert len(data['rows'])==9;datasets[path.parent.name]=data
tutorial=read(OUT/'lab-tutorial/results.json')
for mode in [0,1]:
 rows=[]
 for starter in ['BULBASAUR','CHARMANDER','SQUIRTLE']:
  choices=[r for r in tutorial['rows'] if r['mode']==mode and r['starter']==starter]
  best=max(choices,key=lambda r:(r['wins'],-r['status_turns']))
  rows.append(dict(best,index=0,baseline_attempts=best['attempts'],farm_attempts=[],preparation='original_Oak_tutorial_status_'+str(best['status_turns']),difficulty=grade(best['attempts'],mode)))
 datasets['tutorial-'+('normal' if mode==0 else 'hardcore')]={'rows':rows}
archive_paths=list(OUT.glob('final-*/challenge-results.json'))+list(OUT.glob('final-*-chain/chain-results.json'))+list(OUT.glob('prepared-normal-*/challenge-results.json'))+list(OUT.glob('connected-*/results.json'))+list(OUT.glob('pickup-*/results.json'))+list(OUT.glob('advanced-*/results.json'))+list(OUT.glob('early-*/results.json'))+[OUT/'lab-tutorial/results.json']
for path in sorted(set(archive_paths)):
 data=read(path);raw=path.read_bytes();name=path.parent.name+'.json.gz';compressed=gzip.compress(raw,mtime=0);(DEST/name).write_bytes(compressed)
 archive.append({'dataset':path.parent.name,'file':'data/challenge_balance/'+name,'sha256':hashlib.sha256(compressed).hexdigest(),'uncompressed_sha256':hashlib.sha256(raw).hexdigest(),'fixture_sha256':data['fixture_sha256'],'superseded_early_rows':path.parent.name.startswith(('final-','prepared-'))})
def candidates(index,mode):
 options=[]
 for name,data in datasets.items():
  for row in data['rows']:
   if row['index']!=index or row['mode']!=mode:continue
   if name.startswith('prepared'):tests=row['baseline_attempts'];team=row['team'];prep=row['preparation']
   else:tests=row['farm_attempts'] if row['preparation']=='manual_ev_farm_508' and row['farm_attempts'] else row['baseline_attempts'];team=row['team'];prep=row['preparation']
   for trial,result in enumerate(tests):
    options.append({'dataset':name,'team':team,'result':result,'preparation':prep,'starter':row['starter'],'trainer':row['trainer'],'trial':trial,'context':'isolated'})
 for path in OUT.glob('pickup-*/results.json'):
  data=read(path)
  for row in data['rows']:
   if row['mode']!=mode:continue
   for trial,attempt in enumerate(row['trials']):
    for battle in attempt['battles']:
     if battle['index']==index:options.append({'dataset':path.parent.name,'team':row['team'],'result':battle['result'],'preparation':'shop_TMs_gifts_stones_PP_Up_Pickup','starter':row['starter'],'trainer':'see source stage variant','trial':trial,'context':'connected block','whole_block_won':attempt['won_block']})
 return options
@lru_cache(maxsize=70)
def witness(index,mode):
 options=candidates(index,mode)
 best=max(options,key=lambda o:(o['result']['outcome']==1,o.get('whole_block_won',False),o['context']=='connected block',o['preparation']!='baseline',-o['result']['faints'],-o['result']['turns']))
 best=copy.deepcopy(best);lookup={m['species']:m for m in best['team']};ordered=[]
 for actual in best['result']['before']:
  mon=copy.deepcopy(lookup[actual['species']]);mon['level']=actual['level'];mon['hp_before']=actual['hp'];mon['max_hp_before']=actual['max_hp'];ordered.append(mon)
 best['team']=ordered;return best

summary=[];main=[];recipes=[]
main += ['# Testes dos 35 desafios — v0.4.0-dev','','ROM de referência SHA-256 `'+SHA+'`. Randomizer **OFF**. As batalhas foram encerradas no motor nativo do mGBA, com IA adversária, dano, críticos, PP, EXP, status, itens dos treinadores e derrotas preservados. Não foram usados HP inflado, vitória forçada ou alteração de dano.','','**São testes de combate com equipes reconstruídas e disponibilidade conferida nos fontes. Não são uma campanha percorrida, capturas reais ou homologação de todos os saves Hardcore.** Caminhada, sucesso das capturas, tempo de farm e economia não foram executados. As três coortes Hardcore não representam todas as possibilidades de encontro.','','Cada desafio foi testado nas três escolhas de inicial/variantes correspondentes. A bateria básica usou cinco tentativas por cenário; o Normal com EVs treinados usou três. Tentativas são execuções do bot, **não probabilidades de vitória humana**. Os valores de IV/natureza/ability foram amostrados para montar cenários; não certificam capturar esses indivíduos exatos.','','O Normal preparado abaixo tem EVs 508 por farm manual, preservando IVs/natureza amostrados antes da Liga; o cientista só é usado após a Liga. Hardcore tem IVs 31, EVs 508 e natureza pelo cientista, mantendo a habilidade amostrada. Sem TMs/tutores, itens equipados, mochila em batalha ou trocas voluntárias na bateria básica. O laboratório foi refeito com FIRST_BATTLE, tutorial original de Oak e golpes iniciais completos: cinco tentativas de ataque direto e cinco com dois usos iniciais de Growl/Tail Whip por inicial/modo; a tabela mostra a melhor dessas duas políticas. Normal usa a única Potion do PC; Hardcore não usa mochila. Os desafios 2–10 foram repetidos preenchendo os golpes conhecidos, sem pressupor acesso ao Move Deleter antes de Fuchsia (três tentativas por cenário).','','## Classificação da bateria básica','','Categorias declaradas antes da execução: baixa com todas as vitórias, zero desmaios e média até 18 turnos; moderada com pelo menos 80% de vitórias e baixo desgaste; alta com pelo menos 60% ou desgaste maior; muito alta/instável abaixo disso com alguma vitória; sem vitória = **não demonstrada**, nunca prova de impossibilidade. A categoria leva em conta a política do bot, que é limitada e conhece atributos/habilidades adversários.','','Normal mostra a bateria com farm de EVs; Hardcore mostra as coortes com cotas por seção. Intervalos indicam diferenças entre os três cenários. “V/S” são vitórias / tentativas.','', '| # | Desafio | Normal preparado | V/S | Hardcore | V/S |','|---:|---|---|---:|---|---:|']
recipes += ['# Equipes e preparação por desafio','','Exemplos efetivamente testados. “Vitória registrada” significa resultado do combate reconstruído, condicionado à preparação descrita; não prova captura/caminhada/economia. Nas sequências, a ordem e os níveis abaixo vêm da memória antes da luta. Pokémon com HP zero no Hardcore já estavam perdidos e não participaram. IVs/EVs: HP / Ataque / Defesa / Velocidade / At. Esp. / Def. Esp. As probabilidades de origem são pesos de slots, não chances condicionais da tentativa Nuzlocke.','']
rank={'Baixa nesta bateria':0,'Moderada':1,'Alta':2,'Muito alta / instável':3,'Não demonstrada':4}
def grades(group):
 values=sorted(group['grades'],key=rank.get);return values[0] if len(values)==1 else values[0]+' → '+values[-1]
for step in stages():
 index=step['index'];groups=[];entry={'index':index,'title':step['title'],'modes':{}}
 for mode,prefix in [(0,'prepared-normal'),(1,'final-hardcore')]:
  selected_prefix=('tutorial-'+('normal' if mode==0 else 'hardcore')) if index==0 else ('early-'+('normal' if mode==0 else 'hardcore')) if index<=9 else prefix
  rows=[r for name,d in datasets.items() if name.startswith(selected_prefix) for r in d['rows'] if r['index']==index]
  group=stats(rows);entry['modes'][str(mode)]=group;groups.append(group)
 marker=''
 if any(groups[m]['wins']==0 and witness(index,m)['result']['outcome']==1 for m in [0,1]):marker=' †'
 main.append(f"| {index+1:02d} | {step['title']}{marker} | {grades(groups[0])} | {groups[0]['wins']}/{groups[0]['attempts']} | {grades(groups[1])} | {groups[1]['wins']}/{groups[1]['attempts']} |")
 summary.append(entry)
 recipes += [f"## {index+1:02d}. {step['title']}",'']
 for mode in [0,1]:
  best=witness(index,mode);win=best['result']['outcome']==1;title='Normal' if mode==0 else 'Hardcore'
  recipes += [f"### {title} — "+('vitória registrada' if win else 'composição testada, sem vitória registrada'),'',f"Inicial: {label(best['starter'])}. Dataset: `{best['dataset']}`, tentativa {best['trial']+1}. Contexto: {best['context']}. Preparação: `{best['preparation']}`. Resultado: {best['result']['outcome']}; {best['result']['turns']} turnos contados pelo motor, {best['result']['faints']} desmaios.",'','| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |','|---|---|---|---|---|---|---|']
  for mon in best['team']:
   origin=mon['origin'];source=origin.get('source',origin.get('map',origin.get('kind','')))
   if 'method' in origin:source+=f"; {origin['method']}, slot {origin['slot']}, peso {origin['percent']}%, nível {origin['min_level']}–{origin['max_level']}"
   if mon.get('evolution'):source+='; evoluções: '+', '.join(a+'→'+c+' no '+str(b) for a,b,c in mon['evolution'])
   if mon.get('evolution_item'):source+='; '+mon['evolution_item']
   if mon.get('item'):source+='; equipado '+mon['item']
   if mon.get('tm_moves'):source+='; TMs '+', '.join(mon['tm_moves'])
   recipes.append('| '+label(mon['species'])+f" | {mon['level']} / {mon['hp_before']}/{mon['max_hp_before']} | "+', '.join(label(m) for m in mon['moves'] if m!='NONE')+' | '+mon['nature']+' | '+'/'.join(map(str,mon.get('ivs',[mon['iv']]*6)))+' | '+'/'.join(map(str,mon['evs']))+' | '+source+' |')
  recipes.append('')
  entry['modes'][str(mode)]['demonstrated_win']=win
  entry['modes'][str(mode)]['witness']={k:v for k,v in best.items() if k!='result'}
main += ['','† Houve vitória no preparo ampliado, apesar de nenhuma na bateria básica da respectiva modalidade. O exemplo e suas condições estão no documento de equipes.','','**Atenção aos níveis da Liga:** as linhas isoladas usam o teto de cada membro. Isso não significa que uma equipe alcance esses níveis apenas com a EXP das salas. As séries abaixo mantêm a equipe e o ganho real; o cenário com Candy no Normal declara o estoque obtido por Pickup.','','[Equipes, golpes, EVs, IVs e origem por desafio](CHALLENGE_TEAMS_TESTED.md).','','## Blocos consecutivos','','Nos Bikers, HP/PP/perdas foram mantidos sem cura entre os quatro confrontos. Na Liga, não houve troca por Pokémon do PC; foram mantidos EXP, PP e perdas. Recuperação entre salas usa efeitos nativos dos itens, contados. Hardcore nunca revive perdidos.','','| Série | Modo / cenário | Séries completas / tentativas |','|---|---|---:|']
block_summary=[]
for pattern in ['connected-*/results.json','pickup-*/results.json']:
 for path in sorted(OUT.glob(pattern)):
  data=read(path)
  for row in data['rows']:
   mode='Normal' if row['mode']==0 else 'Hardcore';prep=row.get('strategy',row.get('preparation'))
   main.append(f"| {row['block']} | {mode}, {row['starter']}, {prep}, {path.parent.name.split('-')[0]} | {row['wins']}/{row['attempts']} |")
   block_summary.append({'dataset':path.parent.name,'block':row['block'],'mode':row['mode'],'starter':row['starter'],'preparation':prep,'wins':row['wins'],'attempts':row['attempts']})
main += ['','## Preparo com Pickup e lojas','','Pickup permite Candy e PP Up renováveis: chance nominal de aproximadamente 0,5% de cada item por Pokémon elegível por batalha, retirando os itens achados. Seis Pickup dão cerca de um Candy a cada 33 batalhas em média; um estoque de 72 pede ordem de 2.400 batalhas, não uma garantia. Esse farm não foi executado. Logo, o Normal consegue acompanhar tetos com estoque preparado, mas o custo é muito diferente do Candy por ₽1 do Hardcore.','','A série `pickup` acrescenta TMs compráveis em Celadon, TM19 limitado a uma cópia, presentes/compras/fósseis, evoluções com pedras disponíveis, dois Leftovers, seis Ethers de locais anteriores à Liga, PP Up e até 72 Candy no Normal. Mantém os mesmos Pokémon entre salas, com reordenação dos registros existentes; não zera PP. Compras, coletas e capturas são premissas de disponibilidade, não ações já realizadas no save. As cotas reservadas para Snorlax e aves estáticas pressupõem planejar a primeira visita e usar Repel para evitar encontros comuns.','','## Sequências exploratórias arquivadas — não certificadas','','As seis séries `final-*-chain` são exploratórias e foram superadas nas composições iniciais: usavam flags ordinárias no laboratório e omitiam golpes conhecidos antes de Fuchsia. Não entram na classificação nem comprovam uma sequência legal. Nelas, o registro de mortos exclui o indivíduo das equipes seguintes; a tentativa para em derrota total. Isso é uma sequência de confrontos preparada, sem caminhadas, treinadores de rota, capturas falhas ou economia. Não demonstra uma campanha natural.','']
chains=[]
for path in sorted(OUT.glob('final-*-chain/chain-results.json')):
 data=read(path);played=[r for r in data['rows'] if 'baseline_attempts' in r];last=played[-1]
 main.append(f"- `{path.parent.name}`: {len(played)} desafios tentados; último {last['title']}, resultado {last['wins']}/{last['attempts']}.")
 chains.append({'dataset':path.parent.name,'played':len(played),'last_index':last['index'],'last_title':last['title'],'last_won':bool(last['wins'])})
main += ['','## Limites e reprodução','','Os arquivos `final-*` e `prepared-*` conservam resultados antigos de índices 0–9 para rastreabilidade; a classificação usa `lab-tutorial` e `early-*` nesses índices. A classificação é provisória. O bot não joga como um especialista, não faz trocas voluntárias e a preparação básica ignora recursos úteis. Vitória comprova o combate naquele estado; derrota não demonstra que nenhuma equipe/estratégia vença. As coortes não certificam todos os encontros Hardcore nem randomizer ON. Os 35 grupos são os desafios nomeados do guia; não abrangem cada treinador comum, captura lendária, puzzle ou os quatro cursos completos da Torre. A campanha sem teleportes continua pendente.','','Dados: [resumo e arquivos brutos comprimidos](../data/challenge_balance/summary.json). Scripts: `run_challenge_balance.py`, `run_connected_balance.py`, `run_advanced_balance.py`. Para compilar a fixture básica, use `python3 scripts/balance_core.py --baseline`; para o preparo avançado, omita `--baseline`. Os caminhos dos builds e logs ficam em `output/tests/balance/`. Nenhum save do jogador foi usado; a ROM pública permanece com o SHA acima.']
main += ['','## Leitura prática','','Bruno e Lance concentram os maiores picos. No Normal houve conclusão da primeira Liga e da revanche com a mesma equipe: Jolteon, Ludicolo, Metagross, Lapras, Zapdos e Snorlax. Um exemplo da primeira Liga (inicial Charmander, estratégia water_counter) consumiu 72 Rare Candy, 21 Hyper Potion, 10 Revive, 2 Full Heal e 1 Ether entre salas, além da preparação inicial; terminou cada luta com sobreviventes, sem PC ou PP reiniciados. Isso comprova o combate condicionado ao estoque, não uma progressão confortável.','','No Hardcore, vencer um chefe isolado com uma coorte intacta não comprova sobreviver a todos os anteriores. Nenhuma série completa da Liga foi demonstrada nas equipes/políticas testadas, mesmo com preparo ampliado. O rival do laboratório precede a ativação das perdas permanentes (elas começam ao receber as Poké Balls de Oak). O cientista ainda não está acessível nessa primeira luta: ambos os modos usam genes amostrados e EVs zero no teste corrigido.','','Para reproduzir os resultados iniciais corrigidos, rode também `test_early_move_balance.py --build-dir CAMINHO_DA_FIXTURE_BASICA` e `test_lab_tutorial_balance.py` (fixture própria). `collect_challenge_balance.py` consolida os datasets sem promover as linhas antigas superadas.']
(ROOT/'docs/CHALLENGE_BALANCE_TESTS.md').write_text('\n'.join(main)+'\n',encoding='utf-8')
(ROOT/'docs/CHALLENGE_TEAMS_TESTED.md').write_text('\n'.join(recipes).rstrip()+'\n',encoding='utf-8')
(DEST/'summary.json').write_text(json.dumps({'version':'v0.4.0-dev','rom_sha256':SHA,'campaign_clear_certified':False,'classifications':summary,'blocks':block_summary,'chains':chains,'archives':archive},ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('Collected',len(summary),'challenge groups,',len(archive),'datasets; ROM unchanged',SHA)
