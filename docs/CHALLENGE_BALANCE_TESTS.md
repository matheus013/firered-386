# Testes dos 35 desafios — v0.4.0-dev

**Arquivo histórico da ROM v0.4.0-dev.** Não foi recalculado para os novos times, categorias e learnsets da prévia. Veja [a migração atual](MODERNIZATION.md).

ROM de referência SHA-256 `881869fbaa37abe13becbb071759ff1225984910fdb410d11818e88b56a0990d`. Randomizer **OFF**. As batalhas foram encerradas no motor nativo do mGBA, com IA adversária, dano, críticos, PP, EXP, status, itens dos treinadores e derrotas preservados. Não foram usados HP inflado, vitória forçada ou alteração de dano.

**São testes de combate com equipes reconstruídas e disponibilidade conferida nos fontes. Não são uma campanha percorrida, capturas reais ou homologação de todos os saves Hardcore.** Caminhada, sucesso das capturas, tempo de farm e economia não foram executados. As três coortes Hardcore não representam todas as possibilidades de encontro.

Cada desafio foi testado nas três escolhas de inicial/variantes correspondentes. A bateria básica usou cinco tentativas por cenário; o Normal com EVs treinados usou três. Tentativas são execuções do bot, **não probabilidades de vitória humana**. Os valores de IV/natureza/ability foram amostrados para montar cenários; não certificam capturar esses indivíduos exatos.

O Normal preparado abaixo tem EVs 508 por farm manual, preservando IVs/natureza amostrados antes da Liga; o cientista só é usado após a Liga. Hardcore tem IVs 31, EVs 508 e natureza pelo cientista, mantendo a habilidade amostrada. Sem TMs/tutores, itens equipados, mochila em batalha ou trocas voluntárias na bateria básica. O laboratório foi refeito com FIRST_BATTLE, tutorial original de Oak e golpes iniciais completos: cinco tentativas de ataque direto e cinco com dois usos iniciais de Growl/Tail Whip por inicial/modo; a tabela mostra a melhor dessas duas políticas. Normal usa a única Potion do PC; Hardcore não usa mochila. Os desafios 2–10 foram repetidos preenchendo os golpes conhecidos, sem pressupor acesso ao Move Deleter antes de Fuchsia (três tentativas por cenário).

## Classificação da bateria básica

Categorias declaradas antes da execução: baixa com todas as vitórias, zero desmaios e média até 18 turnos; moderada com pelo menos 80% de vitórias e baixo desgaste; alta com pelo menos 60% ou desgaste maior; muito alta/instável abaixo disso com alguma vitória; sem vitória = **não demonstrada**, nunca prova de impossibilidade. A categoria leva em conta a política do bot, que é limitada e conhece atributos/habilidades adversários.

Normal mostra a bateria com farm de EVs; Hardcore mostra as coortes com cotas por seção. Intervalos indicam diferenças entre os três cenários. “V/S” são vitórias / tentativas.

| # | Desafio | Normal preparado | V/S | Hardcore | V/S |
|---:|---|---|---:|---|---:|
| 01 | Rival — laboratório de Oak | Moderada → Não demonstrada | 6/15 | Não demonstrada | 0/15 |
| 02 | Rival — Route 22, primeira visita | Baixa nesta bateria | 9/9 | Baixa nesta bateria | 9/9 |
| 03 | Brock — Pewter Gym | Baixa nesta bateria → Moderada | 9/9 | Baixa nesta bateria → Alta | 9/9 |
| 04 | Rival — Cerulean City | Moderada → Alta | 9/9 | Alta | 9/9 |
| 05 | Misty — Cerulean Gym | Alta → Muito alta / instável | 7/9 | Alta | 9/9 |
| 06 | Rival — S.S. Anne | Alta | 8/9 | Alta → Muito alta / instável | 6/9 |
| 07 | Lt. Surge — Vermilion Gym | Alta | 9/9 | Alta | 8/9 |
| 08 | Erika — Celadon Gym | Baixa nesta bateria → Moderada | 9/9 | Alta | 9/9 |
| 09 | Giovanni — Rocket Hideout | Baixa nesta bateria | 9/9 | Baixa nesta bateria | 9/9 |
| 10 | Rival — Pokémon Tower | Alta → Muito alta / instável | 7/9 | Alta | 8/9 |
| 11 | Koga — Fuchsia Gym | Alta | 9/9 | Alta | 13/15 |
| 12 | Koichi — Fighting Dojo de Saffron | Baixa nesta bateria → Moderada | 9/9 | Baixa nesta bateria → Alta | 15/15 |
| 13 | Rival — Silph Co. | Alta | 9/9 | Alta → Muito alta / instável | 12/15 |
| 14 | Giovanni — Silph Co. | Moderada → Alta | 9/9 | Alta | 15/15 |
| 15 | Sabrina — Saffron Gym | Baixa nesta bateria → Alta | 9/9 | Alta → Muito alta / instável | 10/15 |
| 16 | Blaine — Cinnabar Gym | Moderada → Alta | 9/9 | Alta | 15/15 |
| 17 | Biker Goon — Three Island, primeira luta | Baixa nesta bateria | 9/9 | Baixa nesta bateria → Alta | 15/15 |
| 18 | Biker Goon — Three Island, segunda luta | Moderada → Alta | 9/9 | Baixa nesta bateria → Alta | 15/15 |
| 19 | Biker Goon — Three Island, terceira luta | Baixa nesta bateria → Moderada | 9/9 | Baixa nesta bateria → Alta | 15/15 |
| 20 | Paxton — líder dos Bikers de Three Island | Baixa nesta bateria | 9/9 | Baixa nesta bateria | 15/15 |
| 21 | Giovanni — Viridian Gym | Moderada → Alta | 9/9 | Alta | 15/15 |
| 22 | Rival — Route 22, antes da Liga | Alta | 8/9 | Alta | 15/15 |
| 23 | Lorelei — primeira Liga | Alta | 8/9 | Alta | 14/15 |
| 24 | Bruno — primeira Liga | Muito alta / instável → Não demonstrada | 1/9 | Alta → Não demonstrada | 6/15 |
| 25 | Agatha — primeira Liga | Alta → Muito alta / instável | 4/9 | Alta | 11/15 |
| 26 | Lance — primeira Liga † | Não demonstrada | 0/9 | Muito alta / instável → Não demonstrada | 2/15 |
| 27 | Campeão — primeira Liga | Alta | 7/9 | Alta | 13/15 |
| 28 | Administradora Rocket — Rocket Warehouse | Alta | 9/9 | Baixa nesta bateria → Alta | 15/15 |
| 29 | Administrador Rocket — Rocket Warehouse | Baixa nesta bateria | 9/9 | Baixa nesta bateria | 15/15 |
| 30 | Gideon — Rocket Warehouse | Baixa nesta bateria → Moderada | 9/9 | Alta | 15/15 |
| 31 | Lorelei — revanche | Moderada → Alta | 9/9 | Alta → Muito alta / instável | 11/15 |
| 32 | Bruno — revanche | Alta → Muito alta / instável | 4/9 | Muito alta / instável → Não demonstrada | 2/15 |
| 33 | Agatha — revanche | Alta → Muito alta / instável | 7/9 | Alta | 15/15 |
| 34 | Lance — revanche † | Não demonstrada | 0/9 | Não demonstrada | 0/15 |
| 35 | Campeão — revanche | Alta → Muito alta / instável | 5/9 | Alta → Muito alta / instável | 7/15 |

† Houve vitória no preparo ampliado, apesar de nenhuma na bateria básica da respectiva modalidade. O exemplo e suas condições estão no documento de equipes.

**Atenção aos níveis da Liga:** as linhas isoladas usam o teto de cada membro. Isso não significa que uma equipe alcance esses níveis apenas com a EXP das salas. As séries abaixo mantêm a equipe e o ganho real; o cenário com Candy no Normal declara o estoque obtido por Pickup.

[Equipes, golpes, EVs, IVs e origem por desafio](CHALLENGE_TEAMS_TESTED.md).

## Blocos consecutivos

Nos Bikers, HP/PP/perdas foram mantidos sem cura entre os quatro confrontos. Na Liga, não houve troca por Pokémon do PC; foram mantidos EXP, PP e perdas. Recuperação entre salas usa efeitos nativos dos itens, contados. Hardcore nunca revive perdidos.

| Série | Modo / cenário | Séries completas / tentativas |
|---|---|---:|
| Bikers | Hardcore, BULBASAUR, baseline, connected | 3/3 |
| FirstLeague | Hardcore, BULBASAUR, baseline, connected | 0/3 |
| Rematch | Hardcore, BULBASAUR, baseline, connected | 0/3 |
| Bikers | Hardcore, CHARMANDER, baseline, connected | 2/3 |
| FirstLeague | Hardcore, CHARMANDER, baseline, connected | 0/3 |
| Rematch | Hardcore, CHARMANDER, baseline, connected | 0/3 |
| Bikers | Hardcore, SQUIRTLE, baseline, connected | 3/3 |
| FirstLeague | Hardcore, SQUIRTLE, baseline, connected | 0/3 |
| Rematch | Hardcore, SQUIRTLE, baseline, connected | 0/3 |
| Bikers | Normal, BULBASAUR, baseline, connected | 3/3 |
| Bikers | Normal, BULBASAUR, farm, connected | 3/3 |
| FirstLeague | Normal, BULBASAUR, baseline, connected | 0/3 |
| FirstLeague | Normal, BULBASAUR, farm, connected | 0/3 |
| Rematch | Normal, BULBASAUR, baseline, connected | 0/3 |
| Bikers | Normal, CHARMANDER, baseline, connected | 3/3 |
| Bikers | Normal, CHARMANDER, farm, connected | 3/3 |
| FirstLeague | Normal, CHARMANDER, baseline, connected | 0/3 |
| FirstLeague | Normal, CHARMANDER, farm, connected | 0/3 |
| Rematch | Normal, CHARMANDER, baseline, connected | 0/3 |
| Bikers | Normal, SQUIRTLE, baseline, connected | 3/3 |
| Bikers | Normal, SQUIRTLE, farm, connected | 3/3 |
| FirstLeague | Normal, SQUIRTLE, baseline, connected | 0/3 |
| FirstLeague | Normal, SQUIRTLE, farm, connected | 0/3 |
| Rematch | Normal, SQUIRTLE, baseline, connected | 0/3 |
| FirstLeague | Hardcore, BULBASAUR, coverage, pickup | 0/3 |
| FirstLeague | Hardcore, BULBASAUR, water_counter, pickup | 0/3 |
| Rematch | Hardcore, BULBASAUR, coverage, pickup | 0/3 |
| Rematch | Hardcore, BULBASAUR, water_counter, pickup | 0/3 |
| FirstLeague | Hardcore, CHARMANDER, coverage, pickup | 0/3 |
| FirstLeague | Hardcore, CHARMANDER, water_counter, pickup | 0/3 |
| Rematch | Hardcore, CHARMANDER, coverage, pickup | 0/3 |
| Rematch | Hardcore, CHARMANDER, water_counter, pickup | 0/3 |
| FirstLeague | Hardcore, SQUIRTLE, coverage, pickup | 0/3 |
| FirstLeague | Hardcore, SQUIRTLE, water_counter, pickup | 0/3 |
| Rematch | Hardcore, SQUIRTLE, coverage, pickup | 0/3 |
| Rematch | Hardcore, SQUIRTLE, water_counter, pickup | 0/3 |
| FirstLeague | Normal, BULBASAUR, coverage, pickup | 0/3 |
| FirstLeague | Normal, BULBASAUR, water_counter, pickup | 0/3 |
| Rematch | Normal, BULBASAUR, coverage, pickup | 1/3 |
| Rematch | Normal, BULBASAUR, water_counter, pickup | 2/3 |
| FirstLeague | Normal, CHARMANDER, coverage, pickup | 0/3 |
| FirstLeague | Normal, CHARMANDER, water_counter, pickup | 2/3 |
| Rematch | Normal, CHARMANDER, coverage, pickup | 1/3 |
| Rematch | Normal, CHARMANDER, water_counter, pickup | 1/3 |
| FirstLeague | Normal, SQUIRTLE, coverage, pickup | 0/3 |
| FirstLeague | Normal, SQUIRTLE, water_counter, pickup | 2/3 |
| Rematch | Normal, SQUIRTLE, coverage, pickup | 1/3 |
| Rematch | Normal, SQUIRTLE, water_counter, pickup | 3/3 |

## Preparo com Pickup e lojas

Pickup permite Candy e PP Up renováveis: chance nominal de aproximadamente 0,5% de cada item por Pokémon elegível por batalha, retirando os itens achados. Seis Pickup dão cerca de um Candy a cada 33 batalhas em média; um estoque de 72 pede ordem de 2.400 batalhas, não uma garantia. Esse farm não foi executado. Logo, o Normal consegue acompanhar tetos com estoque preparado, mas o custo é muito diferente do Candy por ₽1 do Hardcore.

A série `pickup` acrescenta TMs compráveis em Celadon, TM19 limitado a uma cópia, presentes/compras/fósseis, evoluções com pedras disponíveis, dois Leftovers, seis Ethers de locais anteriores à Liga, PP Up e até 72 Candy no Normal. Mantém os mesmos Pokémon entre salas, com reordenação dos registros existentes; não zera PP. Compras, coletas e capturas são premissas de disponibilidade, não ações já realizadas no save. As cotas reservadas para Snorlax e aves estáticas pressupõem planejar a primeira visita e usar Repel para evitar encontros comuns.

## Sequências exploratórias arquivadas — não certificadas

As seis séries `final-*-chain` são exploratórias e foram superadas nas composições iniciais: usavam flags ordinárias no laboratório e omitiam golpes conhecidos antes de Fuchsia. Não entram na classificação nem comprovam uma sequência legal. Nelas, o registro de mortos exclui o indivíduo das equipes seguintes; a tentativa para em derrota total. Isso é uma sequência de confrontos preparada, sem caminhadas, treinadores de rota, capturas falhas ou economia. Não demonstra uma campanha natural.

- `final-hardcore-bulbasaur-chain`: 10 desafios tentados; último Rival — Pokémon Tower, resultado 0/1.
- `final-hardcore-charmander-chain`: 6 desafios tentados; último Rival — S.S. Anne, resultado 0/1.
- `final-hardcore-squirtle-chain`: 7 desafios tentados; último Lt. Surge — Vermilion Gym, resultado 0/1.
- `final-normal-bulbasaur-chain`: 10 desafios tentados; último Rival — Pokémon Tower, resultado 0/1.
- `final-normal-charmander-chain`: 5 desafios tentados; último Misty — Cerulean Gym, resultado 0/1.
- `final-normal-squirtle-chain`: 10 desafios tentados; último Rival — Pokémon Tower, resultado 0/1.

## Limites e reprodução

Os arquivos `final-*` e `prepared-*` conservam resultados antigos de índices 0–9 para rastreabilidade; a classificação usa `lab-tutorial` e `early-*` nesses índices. A classificação é provisória. O bot não joga como um especialista, não faz trocas voluntárias e a preparação básica ignora recursos úteis. Vitória comprova o combate naquele estado; derrota não demonstra que nenhuma equipe/estratégia vença. As coortes não certificam todos os encontros Hardcore nem randomizer ON. Os 35 grupos são os desafios nomeados do guia; não abrangem cada treinador comum, captura lendária, puzzle ou os quatro cursos completos da Torre. A campanha sem teleportes continua pendente.

Dados: [resumo e arquivos brutos comprimidos](../games/super-fire-red/hack-data/challenge_balance/summary.json). Scripts: `run_challenge_balance.py`, `run_connected_balance.py`, `run_advanced_balance.py`. Para compilar a fixture básica, use `python3 games/super-fire-red/scripts/balance_core.py --baseline`; para o preparo avançado, omita `--baseline`. Os caminhos dos builds e logs ficam em `games/super-fire-red/output/tests/balance/`. Nenhum save do jogador foi usado; a ROM pública permanece com o SHA acima.

## Leitura prática

Bruno e Lance concentram os maiores picos. No Normal houve conclusão da primeira Liga e da revanche com a mesma equipe: Jolteon, Ludicolo, Metagross, Lapras, Zapdos e Snorlax. Um exemplo da primeira Liga (inicial Charmander, estratégia water_counter) consumiu 72 Rare Candy, 21 Hyper Potion, 10 Revive, 2 Full Heal e 1 Ether entre salas, além da preparação inicial; terminou cada luta com sobreviventes, sem PC ou PP reiniciados. Isso comprova o combate condicionado ao estoque, não uma progressão confortável.

No Hardcore, vencer um chefe isolado com uma coorte intacta não comprova sobreviver a todos os anteriores. Nenhuma série completa da Liga foi demonstrada nas equipes/políticas testadas, mesmo com preparo ampliado. O rival do laboratório precede a ativação das perdas permanentes (elas começam ao receber as Poké Balls de Oak). O cientista ainda não está acessível nessa primeira luta: ambos os modos usam genes amostrados e EVs zero no teste corrigido.

Para reproduzir os resultados iniciais corrigidos, rode também `test_early_move_balance.py --build-dir CAMINHO_DA_FIXTURE_BASICA` e `test_lab_tutorial_balance.py` (fixture própria). `collect_challenge_balance.py` consolida os datasets sem promover as linhas antigas superadas.
