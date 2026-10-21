# Checklist de testes — FireRed 386

> **Registro histórico da v0.3.0-dev.** Os três defeitos abaixo foram corrigidos na v0.3.1-dev. Resultados atuais e pendências estão em [TEST_CHECKLIST.md](TEST_CHECKLIST.md). As reprovações foram preservadas para documentar a descoberta.

Auditoria de **21/10/2026**, versão **v0.3.0-dev**. Resultado: **há falhas abertas; versão não homologada para campanha completa**.

ROM de referência, SHA-256: `d7a31095b588898a05ee3912a9285ac441b054536e459ffc26a1ad677c3c82c1`.

As sete suítes existentes passaram novamente. Nos 18 novos casos inspirados pela pesquisa, **13 passaram e cinco falharam**, correspondendo a **três causas distintas**. Os resultados estruturados estão em [data/test_audit_v0.3.0.json](data/test_audit_v0.3.0.json). Esta auditoria adiciona testes e documentação; não corrige esses defeitos nem altera o `.gba` publicado.

## Pesquisa e seleção dos casos

Foram consultados relatos dos próprios autores e fontes do projeto pret. São exemplos de categorias conhecidas na comunidade, sem estimativa de frequência. Um problema relatado em outra hack não é automaticamente um defeito desta ROM.

| Fonte primária | Categoria documentada | Aplicação ao checklist |
|---|---|---|
| [Rocket Edition, tópico do autor](https://www.pokecommunity.com/threads/pok%C3%A9mon-firered-rocket-edition-completed.360725/) | Base exata/emuladores, apresentação de Surf e travamento numa combinação específica de Fly com adversários | Integridade do patch, matriz de emuladores, HMs e interações de golpes. O travamento citado pertence àquela hack; ainda não foi reproduzido aqui. |
| [FireRed & LeafGreen+, tópico do autor](https://www.pokecommunity.com/threads/complete-pokefirered-pok%C3%A9mon-firered-leafgreen.454382/) | Cheats antigos e editores externos podem ser incompatíveis com alterações de dados da hack | Saves de teste isolados e validação de persistência. Não significa que nosso layout de bolsa tenha a mesma alteração. |
| [Dragon Ball Z Team Training, tópico do autor](https://www.pokecommunity.com/threads/dragon-ball-z-team-training.356222/) | Falha ao salvar no VBA por configuração de memória Flash | Gravação de save de 128 KiB e recarga em outra instância; VBA Link ainda pendente. |
| [pret, pokemon.c da revisão usada](https://github.com/pret/pokefirered/blob/ce6406d8c2eaf46d543c8a2606de0c3cb1951982/src/pokemon.c) | Caminho original de escrita dos IVs compactados lê apenas um byte; correção opcional sob `BUGFIX` | Casos `packed_ivs_32bit` e `roamer_ivs`, ambos reprovados. |
| [pret, roamer.c da revisão usada](https://github.com/pret/pokefirered/blob/ce6406d8c2eaf46d543c8a2606de0c3cb1951982/src/roamer.c) | Status de um byte é passado a uma função que lê quatro; correção opcional sob `BUGFIX` | Caso dirigido `roamer_status_width`, reprovado. |

O teto da creche, as regras Hardcore, lojas e editor de treinamento foram incluídos por serem riscos específicos das nossas mudanças. A pesquisa não foi usada como prova de que esses defeitos existiam: a classificação abaixo depende dos testes executados.

## Ambiente e limites da evidência

- Ubuntu/WSL, compilador e base fixados pelo [setup](scripts/setup.sh); mGBA via biblioteca nativa, sem interface gráfica.
- O boot de 900 frames executa a ROM de produção. As demais suítes de emulação compilam cópias descartáveis dos mesmos fontes, acrescentando pontos de entrada, cenários e verificações. Não são execuções completas da campanha na ROM intacta.
- Testes de save usam arquivos gerados para teste, gravam Flash e recarregam em uma nova instância. Não carregam nem modificam o save do jogador.
- Aprovação de dados ou de uma função não prova que todos os caminhos de menus, scripts e história estejam corretos. Os casos manuais abaixo continuam abertos.
- As suítes da Liga usam um Pokémon artificialmente resistente para avançar turnos; verificam equipes e execução, não a chance de vitória de um jogador ou o equilíbrio da campanha.

Legenda: **A** = aprovado no escopo indicado; **R** = reprovado; **P** = parcial; **M** = pendente de execução manual. Uma linha marcada A não amplia a cobertura além de sua descrição.

## Falhas confirmadas e como reproduzir

### BUG-001 — IVs compactados truncados (prioridade alta)

Dois casos falharam, com uma causa comum. Chamar `CreateMonWithIVsPersonality` com os seis IVs 31 (`0x3fffffff`) deveria preservar os 30 bits. A leitura posterior retorna **255 (`0x000000ff`)**, em vez de **1073741823**. O mesmo acontece na reconstrução do lendário errante por `CreateRoamerMonInstance`.

Isso comprova a falha desses caminhos de criação/restauração; não significa que todos os Pokémon selvagens ou o editor de IVs estejam defeituosos. O editor altera campos individuais e passou nos próprios testes. O caminho original com esse defeito é mantido quando `BUGFIX` está desativado.

Reprodução: executar `test_community_cases.py`; observar `packed_ivs_32bit` e `roamer_ivs`. Aceitação da correção: ambos retornarem `0x3fffffff`; acrescentar também IVs variados para impedir aprovação por um valor constante.

### BUG-002 — leitura larga do status de errante (prioridade média)

Preparar um errante com `status=0`, `cool=1`, `beauty=2`, `cute=4` e chamar `CreateRoamerMonInstance`. Esperado: status **0**. Observado: **67240192 (`0x04020100`)**, incluindo os três bytes vizinhos.

É um **cenário dirigido com bytes vizinhos não zero**. Não foi demonstrado que uma partida comum sem edição produza esses valores; os campos normalmente zerados podem esconder a falha. A fonte pret documenta a incompatibilidade de largura.

Reprodução: caso `roamer_status_width`. Aceitação: copiar apenas o status e preservar corretamente cada condição válida, independentemente dos campos vizinhos.

### BUG-003 — creche ultrapassa teto de nível (prioridade alta para progressão)

Sem insígnias, teto **18**, colocar Pidgey nível 5 na creche e simular **50.000 passos** antes de chamar a retirada nativa. Ele sai no **nível 38**, tanto no Normal quanto no Hardcore. O teste espera que o treinamento respeite o teto vigente.

A retirada calcula EXP em um Pokémon temporário, fora de `gPlayerParty`; a limitação de EXP reconhece apenas ponteiros da equipe. No Normal isso permite treinamento acima do teto. No Hardcore o Pokémon acima do teto fica indisponível para lutar até a progressão permitir seu nível; isso não equivale a morte permanente.

Reprodução: casos `daycare_cap_normal` e `daycare_cap_hardcore`. Aceitação: retirada respeitar o teto e evolução/aprendizado não contornarem esse limite. Testar também retirada após aumentar o teto para definir o tratamento da EXP excedente.

## Checklist automatizado executado

### Build, conteúdo e distribuição

| ID | Estado | Verificação e resultado | Evidência |
|---|---|---|---|
| B01 | A | Base USA 1.0 tem SHA-1 esperado; IPS reconstrói exatamente a ROM de produção | `audit_release.py` |
| B02 | A | Todos os hashes do manifesto local da release conferem; ROM da release = ROM de produção | `audit_release.py` |
| B03 | A | Patch de fontes da release = patch atual; aplicação sobre índice da base limpa aceita | `audit_release.py` |
| B04 | A | Boot por 900 frames, imagem não uniforme, sem erro de opcode detectado | `audit_release.py`; não valida campanha |
| B05 | A | Dados gerados de equipes correspondem ao gerador: 636 equipes regulares, laboratório simples | `balance_trainers.py --check` |
| B06 | A | Distribuição gerada cobre 386 espécies, com raridade e etapa configuradas | `balance_encounters.py --check` |
| B07 | A | Guias de equipes/encontros correspondem aos dados | `generate_guides.py --check` |
| B08 | A | 2.053 slots compilados geram as 386 espécies; novos lendários bloqueados antes da Liga | `test_encounters.py` |
| B09 | A | Mew inicia batalha e Mew/Deoxys gerados recebem marca de obediência | `test_encounters.py`; não captura todas as espécies em uma campanha |
| B10 | P | Curva/raridade conferidas por dados; dificuldade real exige jogar cada etapa | [Revisão de dificuldade](DIFFICULTY_REVIEW.md) é histórica; não é nova campanha de teste |

### Lojas, menus e treinamento

| ID | Estado | Verificação e resultado | Evidência |
|---|---|---|---|
| L01 | A | 23 catálogos × Normal/Hardcore = 46 casos, cancelamento de compra e saída | `test_shops.py` |
| L02 | A | Compra e venda, retorno ao balcão, saída por B e SEE YA, três ciclos e reabertura | `test_shops.py` |
| L03 | A | Estoque mantém referência válida após recriação do mapa/heap | `test_shops.py`; [detalhes](SHOP_TEST_RESULTS.md) |
| L04 | A | Rare Candy extra e preço 1 no Hardcore; sem inclusão indevida no Normal | `test_shops.py`, `test_training.py` |
| L05 | A | Editor bloqueado no Normal antes da Liga e liberado após; liberado no Hardcore | `test_training.py`, funções nativas |
| L06 | A | Natureza/EVs/IVs aceitos e limites inválidos rejeitados; HP e identidade preservados | `test_training.py` |
| L07 | A | Interação real com cientista em Viridian: abrir, cancelar, confirmar e voltar ao campo | `test_training_ui.py`; os 19 mapas não foram percorridos pela interface |
| L08 | A | Enfermeira cura três vezes e recusa a quarta sem curar | `test_training_ui.py` |
| L09 | A | Cotas independentes por Centro e persistidas após salvar/reabrir | `test_training.py` |
| L10 | M | Máquinas de bebidas, prêmios e vendedores com scripts próprios | Não fazem parte dos 23 catálogos Pokémart |

### Regras, save e combate

| ID | Estado | Verificação e resultado | Evidência |
|---|---|---|---|
| R01 | A | Seleção Normal/Hardcore nos menus de início | `test_hardcore.py` |
| R02 | A | Modo e cota de encontro persistem em Flash e recarga em nova instância | `test_hardcore.py` |
| R03 | A | Morto permanece sem HP após cura, revive e conversão de Box; treinamento o rejeita | `test_hardcore.py`, `test_training.py` |
| R04 | A | Duplicata não gasta tentativa; encontro já gasto bloqueia captura, inclusive shiny | `test_hardcore.py`; regra atual não tem exceção shiny |
| R05 | A | Itens de cura em batalha bloqueados no Hardcore; bolas seguem cota; Normal permite cura | `test_hardcore.py`, API |
| R06 | A | Teto aplica-se à EXP da equipe e muda com insígnias/Liga; pós-Liga chega a 100 | `test_hardcore.py` |
| R07 | R | Creche obedece teto nos dois modos | BUG-003 |
| R08 | A | Derrota real e fluxo de fim da tentativa executados | `test_hardcore.py` |
| R09 | A | Laboratório simples, treinador em dupla e jogador com apenas um Pokémon | `test_trainer_battles.py`; início/seleção de ação |
| R10 | A | 14 variantes de E4/Campeão, primeiras e revanche, seis oponentes auditados; 4–6 turnos | `test_elite_four.py`; não conclui a Liga inteira |
| R11 | A | Ajustes da IA para parceiro, imunidade, suporte, clima e pontuação zero | `test_doubles_ai.c`, execução nativa; não prova todas as decisões em batalha |
| R12 | A | Layout estrutural do save e persistência de natureza/EV/IV/shiny/cotas | `test_training.py`; sem save antigo real importado |
| R13 | M | Migrar cópias de saves reais de cada versão anterior e salvar novamente | Pendente; preservar originais e testar Normal/Hardcore separadamente |
| R14 | M | EXP global após Parcel: múltiplos participantes, KO duplo, morto, teto e aprendizado | Validar numa batalha completa, não apenas flags/funções |
| R15 | M | TM: ensinar, cancelar troca de golpe, reabrir e ensinar a outro Pokémon sem consumir | Pendente de interface real |

### Novos casos de Pokémon e armazenamento

| ID | Estado | Caso | Resultado/limite |
|---|---|---|---|
| C01 | A | `national_dex_api` | Ativação nativa habilita Nacional; entrega do Parcel/Oak na história ainda requer percurso |
| C02 | A | `kadabra_37` | Não evolui no 36; Alakazam no 37 |
| C03 | A | `machoke_37` | Não evolui no 36; Machamp no 37 |
| C04 | A | `graveler_37` | Não evolui no 36; Golem no 37 |
| C05 | A | `haunter_37` | Não evolui no 36; Gengar no 37 |
| C06 | A | `everstone` | Everstone bloqueia a evolução consultada |
| C07 | A | `sentret_furret_national` | Evolução fora de Kanto disponível com Nacional habilitada |
| C08 | A | `thunder_stone` | Consulta de evolução Pikachu → Raichu com pedra |
| C09 | A | `shiny_boundaries` | Para OT 0, PID 0/255 shiny e 256 comum |
| C10 | A | `shiny_256_of_65536` | Função real retorna 256 shinies em 65.536 PIDs baixos, OT e parte alta fixos em 0; não é amostragem de encontros/RNG |
| C11 | R | `packed_ivs_32bit` | BUG-001: 255 em vez de 1073741823 |
| C12 | R | `roamer_ivs` | BUG-001 no errante |
| C13 | R | `roamer_status_width` | BUG-002, bytes vizinhos dirigidos |
| C14 | R | `daycare_cap_normal` | BUG-003, nível 38 com teto 18 |
| C15 | R | `daycare_cap_hardcore` | BUG-003, nível 38 com teto 18 |
| C16 | A | `nature_pc_identity` | Natureza treinada sobrevive à conversão Box/equipe; PID/shiny preservados, sem Bad Egg |
| C17 | A | `full_pc_no_overwrite` | 420 posições + equipe cheia: entrega recusada e última posição preservada |
| C18 | A | `last_pc_slot` | Apenas última posição livre: entrega vai para ela, espécie/checksum válidos |

C16–C18 usam funções nativas; arrastar, depositar, retirar e liberar pela interface do PC continuam pendentes. C02–C08 consultam o alvo de evolução; não observam todas as animações ou trocas de golpes durante evolução.

## Roteiro manual obrigatório antes de considerar uma versão estável

Usar cópias de saves e registrar versão, emulador, modo, local, passos, resultado esperado/observado e screenshot/log quando houver falha. Cada tarefa deve ser executada nos dois modos quando aplicável. Marcar somente após execução.

- [ ] **M01 — início:** NEW GAME, nome, inicial, laboratório, entrega do Parcel, Nacional e EXP global; salvar, fechar e continuar.
- [ ] **M02 — campanha:** atravessar cada ginásio, rival e chefe na ordem possível; conferir teto antes/depois, time de seis dos líderes e desbloqueios de história.
- [ ] **M03 — Liga:** concluir as cinco lutas em sequência, créditos/Hall da Fama, salvar/reabrir e concluir revanche com adversários no 100.
- [ ] **M04 — pós-jogo:** Celio/Ruby somente após Liga, ilhas e retornos, lendários fixos e aleatórios; evitar bloqueio de história com Nacional antecipada.
- [ ] **M05 — deslocamento:** Cut, Surf, Strength, Flash, Rock Smash, Waterfall, Fly, Dig, Escape Rope, bicicletas, escadas e portas; retorno após blackout e saída de lojas/Centros.
- [ ] **M06 — todas as lojas no campo:** chegar por cada mapa e NPC reais, comprar, vender, sair, entrar e salvar; suíte de catálogos compartilha atendimento e não percorre todos os acessos da campanha.
- [ ] **M07 — limites de bolsa/dinheiro:** sem dinheiro, bolsa cheia, quantidade máxima, venda do último item, TM/Candy em balcão especial, máquinas e prêmios.
- [ ] **M08 — golpes/duplas:** Fly contra Koffing/Weezing (motivação da pesquisa), Dig/Surf/Earthquake com parceiro, Protect, Substitute, clima, sono, veneno, recuo e KO simultâneo. Sem afirmar previamente que falham aqui.
- [ ] **M09 — equipe reduzida:** só um apto, um morto, um acima do teto, último apto desmaia, captura com equipe cheia; conferir alvo/turno e ausência de softlock.
- [ ] **M10 — PC pela interface:** caixas cheias, última vaga, troca de caixa, retirar/depositar morto e treinado, liberar, salvar e reabrir; checksum/identidade preservados.
- [ ] **M11 — creche/reprodução:** depósito/retirada com teto, aumento de teto, novo golpe, Everstone, ovo com equipe/PC cheios, eclosão e save/reload. BUG-003 precisa ser corrigido antes de aprovar teto.
- [ ] **M12 — errantes:** gerar cada variante, fugir, reencontrar com HP/status preservados, capturar e conferir IVs. Bloqueado parcialmente por BUG-001/002.
- [ ] **M13 — apresentação:** sprites, ícones, cries e animações das 386 espécies; telas de resumo/Pokédex, nomes/textos longos, Surf/Fly e paletas.
- [ ] **M14 — Centros:** visitar os 19 cientistas, confirmar bloqueio/liberação por modo, cota independente e recarga; verificar efeitos intencionais de PC/camas que não usam a enfermeira.
- [ ] **M15 — Torre:** concluir circuitos, transições, recompensas e retornos; auditoria de elencos não equivale a circuito jogado.
- [ ] **M16 — emuladores:** repetir início/save/loja/dupla/PC no mGBA desktop e **VBA Link 1.8 LAN/WiFi**; registrar versões e Flash 128 KiB. Outros emuladores/hardware exigem aprovação própria.
- [ ] **M17 — conexão:** troca e batalha local/LAN entre duas instâncias, salvar/reabrir ambos; desmaio em link não deve marcar morte Hardcore. A função de exceção foi testada, a conexão real ainda não.
- [ ] **M18 — migração:** cópias de saves antigos Normal/Hardcore, locais diferentes, Bolsa/PC/flags/cotas; nenhum teste desta rodada comprova toda migração. Savestate entre versões não substitui teste de `.sav`.
- [ ] **M19 — duração:** sessões longas, muitas transições/lojas/caixas, salvar repetidamente e retornar ao título; observar travamentos e memória.
- [ ] **M20 — dificuldade:** jogar cada etapa com recursos realmente disponíveis, sem equipe artificial; registrar níveis, tentativas, estratégias e gargalos. Não aprovar balanceamento só porque a batalha executa.

Separação físico/especial por golpe, Fada, engine/golpes modernos, Alphas/raids e outras funcionalidades ainda pendentes no [README](README.md) não foram consideradas implementadas ou aprovadas. Testes correspondentes devem ser adicionados quando essas mudanças existirem.

## Repetir os testes

No Ubuntu/WSL, após `bash scripts/setup.sh`, `bash scripts/build.sh baseline` e `bash scripts/build.sh hack`:

```sh
python3 scripts/audit_release.py
python3 scripts/run_regression.py
python3 scripts/test_community_cases.py
```

`audit_release.py` é específico ao pacote local **v0.3.0-dev** e precisa dos assets em `output/release/v0.3.0-dev/`, obtidos da release. Ele não baixa ou reempacota assets. `run_regression.py` executa sete suítes em até três processos simultâneos. Os runners usam Python 3, GCC, libmGBA/libpng e o toolchain configurado no projeto.

**O último comando retorna código 1 enquanto os cinco casos conhecidos falharem.** Isso é resultado de teste reprovado, não aprovação parcial silenciosa. Não remova as verificações para tornar o comando verde. As fixtures C e os logs/JSON ficam nos caminhos indicados pelos scripts, dentro de `output/tests/`; somente o resumo sem saves/ROMs é versionado em [data/test_audit_v0.3.0.json](data/test_audit_v0.3.0.json).

Antes da próxima release: corrigir BUG-001/002/003, repetir os testes que os reproduzem e as regressões afetadas; registrar novo SHA, resultados e tarefas manuais concluídas. Os resultados desta página continuam sendo o histórico da v0.3.0-dev.
