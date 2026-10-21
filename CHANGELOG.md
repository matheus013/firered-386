# Histórico de versões

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
