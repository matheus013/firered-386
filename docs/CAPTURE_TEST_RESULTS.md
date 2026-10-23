# Captura no Hardcore — v0.3.1-dev

Testado em 21/10/2026, mGBA via biblioteca nativa, em build descartável derivada dos fontes da ROM SHA-256 `d880845011b29532abf333cc082da02b20eee97cf9f7767fe5fd61acf3b55cf8`. **23/23 casos aprovados**; **124 mapas** de encontros verificados. [Resultados estruturados](../game/hack-data/capture_test_results.json).

## Regra validada

- Vale o **primeiro selvagem elegível por seção do mapa** depois das primeiras cinco Poké Balls de Oak. A cota é consumida ao começar a batalha, antes de tentar capturar.
- Fugir, derrotar ou perder o selvagem não devolve a oportunidade. Enquanto durar o mesmo encontro permitido, pode tentar várias Balls.
- A cláusula de duplicatas usa a **espécie exata já registrada como capturada**, sem gastar uma área nova. Evoluções da mesma família contam como espécies diferentes. Shiny não ignora a cota.
- Grama, Surf, pesca, Rock Smash e estáticos usam o mesmo identificador de seção. Andares de uma caverna e setores da Safari Zone com a mesma seção compartilham uma oportunidade; entrar em outro arquivo de mapa não cria uma cota.
- Treinadores, link, demonstrações e os fantasmas não capturáveis da Torre não gastam a oportunidade. A liberação do Marowak fantasma não o torna capturável.
- Presentes, ovos, compras de Pokémon e trocas por NPC continuam fora da cota de selvagens, conforme a variante documentada em [HARDCORE.md](HARDCORE.md).
- A cota persiste no save. Reabrir um save anterior continua sendo possível; não há proteção contra rollback/cheats.

## O que foi executado

O teste configura cenários com os dados de mapas reais e chama `Nuzlocke_OnBattleStart`. Simula os resultados fuga/KO e o início do próximo encontro, verifica a cota e chama os comandos nativos de lançar Ball, entregar o capturado e registrar Pokédex. Uma captura autorizada recebe Caterpie válido na equipe; a Ball seguinte, Master Ball incluída, é bloqueada. O bloqueio também passa pelo comando comum da Safari Ball.

O teste de múltiplas Balls verifica que consultas repetidas de permissão no mesmo encontro não a consomem novamente; não é uma gravação de cinco animações de lançamento. A enumeração dos 124 mapas testa a decisão de cota em cada um, sem percorrer suas portas/grama ou executar cada modalidade de encontro pela interface.

Um save Flash de **128 KiB** é transferido para uma **nova instância** do mGBA. Route 1 continua bloqueada e Route 2 continua livre, consumindo apenas ao iniciar seu primeiro encontro. Nenhum save do jogador foi usado.

| Caso automatizado | Resultado |
|---|---|
| `before_oak_balls` | Aprovado |
| `first_eligible_consumes` | Aprovado |
| `multiple_balls_same_battle` | Aprovado |
| `flee_loses_attempt` | Aprovado |
| `ko_loses_attempt` | Aprovado |
| `different_route_independent` | Aprovado |
| `return_route_still_used` | Aprovado |
| `duplicate_skips_without_consuming` | Aprovado |
| `nonduplicate_after_duplicate` | Aprovado |
| `exact_species_not_family` | Aprovado |
| `shiny_has_no_exception` | Aprovado |
| `trainer_does_not_consume` | Aprovado |
| `link_does_not_consume` | Aprovado |
| `tutorial_does_not_consume` | Aprovado |
| `cave_floors_share` | Aprovado |
| `safari_sectors_share_ball_block` | Aprovado |
| `static_same_section_blocked` | Aprovado |
| `unidentified_ghost_does_not_consume` | Aprovado |
| `revealed_marowak_does_not_consume` | Aprovado |
| `native_catch_then_next_ball_blocked` | Aprovado |
| `normal_unrestricted` | Aprovado |
| `fresh_instance_quota_persistence` | Aprovado |
| `all_compiled_wild_maps_first_only` | Aprovado |

## Falha encontrada e corrigida

Na v0.3.0-dev, o fantasma de Marowak **revelado pelo Silph Scope** consumia a oportunidade da Pokémon Tower apesar de o motor rejeitar sua captura. A condição só excluía fantasmas sem identificação. A v0.3.1-dev exclui as batalhas marcadas como fantasma nos dois estados; Gastly e outros selvagens normais da Torre continuam sujeitos à cota.

O [resultado anterior](../game/hack-data/capture_rules_v0.3.0.json) conserva a reprodução: 21/22 aprovados, com falha apenas em `revealed_marowak_does_not_consume`. Os 23 casos atuais passaram. As cotas antigas não são apagadas automaticamente.

## Repetir e completar validação manual

Após compilar no Ubuntu/WSL: `python3 game/scripts/test_capture_rules.py`. Código de saída esperado: 0. Logs e JSON em `game/output/tests/capture/`.

Ainda é necessário percorrer uma campanha real: fugir do primeiro encontro, tentar capturar outro, testar grama/Surf/pesca na mesma rota, mudar de andar, visitar Safari/Torre, salvar/reabrir e repetir no VBA Link 1.8 LAN/WiFi. O roteiro completo está no [checklist](TEST_CHECKLIST.md).
