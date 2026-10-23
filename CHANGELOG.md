# Histórico de versões

## v0.5.0-preview.4 — 23/10/2026

- Revisão de interface: rótulo do editor de IVs abreviado para não invadir a tabela; categoria/tipo dos golpes de batalha dentro dos 64 px; cabeçalhos PT-BR revisados; descrições do estojo de TMs reformatadas para o painel de 144 px. Novos testes de largura com fontes nativas e capturas de tela em EN/PT-BR, incluindo golpes novos e Fada. [Escopo e evidências](UI_REVIEW.md).
- Normal libera o editor de EVs desde o início; natureza e IVs continuam exigindo a primeira Liga. Hardcore mantém todos liberados desde o início. Bloqueios conferidos no menu e nas funções de edição.

- Evoluções que exigiam troca com item passam a ocorrer no nível 37, como Kadabra/Machoke/Graveler/Haunter: Steelix, Kingdra, Scizor, Porygon2, Politoed e Huntail. Slowking e Gorebyss, ramos alternativos, usam King's Rock e Deep Sea Scale direto da mochila. Pedras, amizade e beleza não mudam. A Deep Sea Scale ainda não tem fonte na campanha; as oito espécies continuam capturáveis na natureza.
- Times dos treinadores comuns regenerados com as novas evoluções (Kingdra, Politoed, Steelix, Porygon2). Corrigido o balanceador: substitutos de suporte agora evoluem como o resto do time, eliminando Oddish, Nidoran♀, Natu e Jigglypuff acima do nível de evolução.
- Tipo Fada: ícone, nome, tabela de efetividade da geração IX, tipos atuais de 18 espécies (Clefairy, Jigglypuff, Mr. Mime, Togepi, Marill, Snubbull, Ralts, Mawile e famílias) e de Charm/Sweet Kiss/Moonlight (Fada) e Curse (Ghost). Hidden Power nunca resulta em Fada.
- Atributos base da geração IX nas 386 espécies; 41 mudaram. Encontros redistribuídos pelo novo total de atributos (554 de 4.352 slots, 386/386 espécies mantidas) e guias regenerados; treinadores comuns regenerados.
- Nomes de tipo em PT-BR fixados em inglês, como os ícones: o rascunho automático mostrava DARK como "ESTÚDIO" e FLYING como "VOLTAR". O tipo Lutador ainda aparece como "LUTAR", texto compartilhado com o botão de batalha.
- 223 golpes novos no total. A expansão inclui terrenos, salas, Tailwind, Gravity, neve, perigos de campo e efeitos com memória de turno. O lote inicial tinha 167 golpes: 89 com efeitos nativos e 78 com efeitos novos no motor (alterações de atributo com dano, recuo de 50%, presas, crítico garantido, poder variável, atributo trocado no dano, Freeze-Dry, troca após o ataque, Feint, Sucker Punch, Upper Hand, curas de aliado, Meteor Beam, Steel Beam, Final Gambit, Metal Burst, Triple Axel, golpes de tipo/habilidade/status e Infestation). Números, tipo e categoria do PokeAPI; animações reaproveitadas; descrições EN/PT; aprendizado por nível. Hurricane nunca erra na chuva e tem precisão menor no sol. Metronome alcança todos os golpes. Learnsets em 32 bits e relembrador com até 30 golpes.
- Correções de motor encontradas nos testes: golpes sem precisão (Struggle, Tearful Look, Play Nice) não erram mais; Explosion/Self-Destruct não dividem mais a Defesa pela metade (o poder moderno já considera isso); queda de High Jump Kick/Jump Kick e similares passa a ser metade do HP máximo, também contra alvo imune; criação de Pokémon cerca de 2× mais rápida com os learnsets maiores.
- Documentação: categoria por golpe deixa de constar como pendente; checklist aponta a prévia atual.

## v0.5.0-preview.3 — revisão do PT-BR

- Revisadas 354 descrições de golpes, 288 de itens, descrições de habilidades, menus, mensagens principais de batalha e diálogos iniciais.
- Incluídas as falas dos Centros Pokémon e as descrições JSON de itens, ausentes do catálogo anterior.
- Corrigidas as quebras de 2.444 diálogos por largura em pixels e preservada a pontuação junto a nomes/variáveis.
- Protegidos nomes e identificadores; rótulos PODER/PRECIS./CATEG. desenhados em PT-BR no resumo de golpes.
- Encurtada uma fala do tutorial que excedia o espaço reservado para mensagens de batalha; novos testes de chaves, controles e limites de texto.
- Tradução ainda beta: diálogos secundários automáticos e glifos portugueses ausentes da fonte continuam pendentes. [Revisão completa](PTBR_REVIEW.md).

## v0.5.0-preview.2 — revisão da IA

- Corrigidos histórico por combatente, consultas ao parceiro e uso de imunidades pelos scripts de IA.
- Desempate entre golpes por dano/precisão, soma de dano em área, dano fixo e poderes baseados em amizade/HP; Thunder considera clima.
- Clima suprimido também por adversários; coordenação com Protect/Detect aliado e penalidade para dois status no mesmo alvo.
- Surge: Manectric com Static. Agatha/Lance: Gengar com Shadow Ball especial e STAB.
- Novos testes de decisões e batalhas completas isoladas contra ginásios/E4 nos dois modos. [Escopo, resultados e limites](AI_REVIEW.md).

## v0.5.0-preview.1 — 22/10/2026 (migração parcial)

- IA de duplas: desempate de alvo por dano estimado, suporte conforme o plano/limitações do parceiro, duplicação de telas/clima e uso consecutivo de Protect.
- Oito líderes com sets, habilidades, naturezas e EVs explícitos; ajustes de legalidade após a importação dos learnsets.
- Categorias modernas por golpe e dados de poder/precisão/PP/prioridade dos 354 golpes existentes; integração em dano, telas, burn, Hustle e treino por função.
- Todas as 50 TMs existentes à venda em Celadon antes da Liga.
- Categoria reposicionada para não sobrepor EFFECT; descrições ajustadas à largura do painel, com 14 traduções revisadas.
- Learnsets por nível da geração IX (Scarlet/Violet; BDSP ou Legends: Arceus quando a espécie não está em SV) para as 386 espécies, só com golpes existentes na ROM. Golpes de evolução entram no nível da evolução. Compatibilidade das TMs pela geração IX; TMs sem equivalente lá (Hail, Hidden Power, Frustration, Return, Secret Power, Snatch, Double Team, Attract, entre outras) mantêm a compatibilidade anterior; HMs só ganham compatibilidade, nunca perdem. Gerado por `scripts/import_learnsets.py`.
- A geração IX completa e o catálogo expandido de TMs ainda estão pendentes. [Estado e limites](MODERNIZATION.md).

## v0.4.0-dev — 22/10/2026

- Interface do cientista com retrato, painéis, barras, saldo de EVs e comparação de atributos antes de confirmar; edição/cancelamento e restrições por modo preservadas.
- Idioma EN/PT-BR nas opções, salvo na partida; português em beta, majoritariamente traduzido automaticamente, com 84 revisões manuais.
- Corrigido o marcador de palavra da tradução automática (`▁`) que aparecia como símbolo no lugar de espaços em cerca de 6.500 textos PT-BR; corrigidos STATUS, o título da tela de golpes e a descrição do Growl.
- Randomizer opcional após escolher Normal/Hardcore: selvagens, iniciais e treinadores; semente fixa por save, níveis e quantidades preservados.
- IVs 31 e EVs treinados em líderes, rivais, chefes, E4/Campeão e Torre; adaptação do treino às espécies aleatórias.
- Novos testes de dados, menus, captura aleatória e persistência; regressão de lojas nos dois idiomas. [Escopo e limites](FEATURE_TEST_RESULTS.md).
- Campanha completa sem assistência ainda pendente; os 114 checkpoints anteriores pertencem à v0.3.1-dev e usaram assistência.

## Validação assistida de campanha — 21/10/2026 (sem nova ROM)

- 114 checkpoints aprovados: eventos da história, líderes, Liga e revanche completas até créditos, Sevii, Mewtwo, opcionais, quatro circuitos da Torre e três confrontos do rival.
- Mapas/warps auditados e estados finais confirmados em Flash saves de teste; o save final voltou ao campo em outra instância.
- Os testes fornecem equipes, cura e deslocamentos e iniciam scripts diretamente. Não homologam uma campanha normal ou Hardcore sem assistência. [Relatório e limites](CAMPAIGN_TEST_RESULTS.md).

## v0.3.1-dev — 21/10/2026

- Corrigidos os quatro bytes dos IVs compactados, incluindo reconstrução dos lendários errantes.
- Corrigida a leitura do status dos errantes, sem incluir os três campos vizinhos.
- Creches das ilhas e Route 5 respeitam o teto de EXP/nível nos dois modos, com previsão e cobrança coerentes.
- O Marowak fantasma revelado não consome mais a oportunidade de captura da Torre; continua impossível capturá-lo.
- Validados 23 casos de captura, cotas nos 124 mapas de encontros, 28 casos de Pokémon/creche e sete suítes de regressão. Nova compilação limpa do patch reproduziu a ROM.
- [Checklist atualizado](TEST_CHECKLIST.md), [captura](CAPTURE_TEST_RESULTS.md) e [auditoria anterior preservada](TEST_AUDIT_V0_3_0.md). Campanha completa e VBA Link permanecem pendentes.

## Auditoria de testes — 21/10/2026 (sem nova ROM)

- Pesquisa de falhas documentadas por autores de hacks e pelo pret; [checklist de testes](TEST_CHECKLIST.md) com fontes, critérios, reproduções e roteiro manual.
- Sete suítes existentes repetidas e aprovadas, incluindo 46 casos de lojas e 14 variantes da Liga.
- Novos testes nativos: 13 de 18 aprovados. Cinco reprovações identificam três defeitos abertos na v0.3.0-dev: truncamento de IVs compactados, leitura larga do status dos errantes em cenário dirigido e creche acima do teto nos dois modos.
- Adicionados runners e [resumo estruturado](data/test_audit.json). O `.gba` publicado permanece o mesmo; esta entrada não anuncia correção dos defeitos encontrados.

## v0.3.0-dev — 21/10/2026

- Distribuição das 386 espécies com captura selvagem direta no modo Normal, sem depender de inicial, troca, reprodução ou ticket.
- Encontros e níveis ajustados por etapa; iniciais em slots raros e espécies fortes reservadas às etapas avançadas.
- Todos os 21 lendários/míticos com encontros aleatórios de 1% no pós-Liga, além dos eventos fixos originais. Mew e Deoxys recebem a marca interna necessária para obedecer.
- Celio exige vitória na Liga para iniciar a busca da Ruby, evitando que a Nacional antecipada libere o pós-jogo.
- Corrigido o travamento ao voltar da compra/venda e sair da loja. O Rare Candy adicional usa o estoque original da ROM sem manter referência ao heap recriado pelo mapa.
- Corrigida a apresentação do Rare Candy no balcão de TMs.
- Guias atualizados: [encontros por área](ENCOUNTERS.md), [índice das 386 espécies](SPECIES_LOCATIONS.md) e [resultados das lojas](SHOP_TEST_RESULTS.md).

Validação: 386 espécies distintas geradas a partir de 2.053 slots ativos; bloqueio dos novos lendários antes da Liga; batalha selvagem com Mew; 23 catálogos de lojas nos dois modos, totalizando 46 casos aprovados no mGBA; regras/save do Hardcore; recompilação limpa do patch idêntica à ROM entregue. A campanha completa e o VBA Link não foram validados.

## v0.2.0-dev — 21/10/2026

- Curva elevada: ginásios 18/28/36/44/52/60/68/76; Liga 80/83/86/89 e Campeão 92. Revanche inteira no 100.
- Teto de EXP nos dois modos; treinadores recalibrados e Campeão com treinamento por função.
- Cientista nos 19 Centros com edição gratuita de natureza, EVs e IVs: desde o início no Hardcore, após a Liga no Normal.
- Rare Candy por ₽1 nas lojas comuns e três curas pela enfermeira por Centro, exclusivamente no Hardcore.

O travamento de saída das lojas desta etapa foi corrigido em v0.3.0-dev.

## v0.1.1-dev — 21/10/2026

- Elite Four com adaptações de times campeões oficiais, completadas com espécies das gerações I–III.
- Naturezas e habilidades fixas, IVs 31, EVs por função, itens e golpes definidos para a primeira Liga e as revanches.
- Referências e substituições em [E4_CHAMPIONS.md](E4_CHAMPIONS.md).

## v0.1.0-dev

- Primeiras melhorias de itens, evolução, shiny, Pokédex Nacional e EXP global após o Parcel.
- Equipes e IA revistas para duplas; primeira luta do laboratório preservada em simples.
- Escolha Normal/Hardcore ao criar o save, com persistência das regras.

O histórico resume as etapas. O [README](README.md) descreve o estado atual e as funcionalidades ainda pendentes.
