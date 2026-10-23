# Campanha e pós-jogo — testes assistidos da v0.3.1-dev

**Resultado: os marcos testados passaram em cenários assistidos. Uma campanha normal completa e uma tentativa Hardcore completa ainda não estão homologadas.**

Auditoria de 21/10/2026, ROM de referência SHA-256 `d880845011b29532abf333cc082da02b20eee97cf9f7767fe5fd61acf3b55cf8`. Foram aprovados **114 checkpoints**: 30 de eventos da história, 81 na sequência de líderes/Liga/Sevii/opcionais/Torre e três confrontos adicionais do rival. Não foi exigida a captura dos 386, conforme a escolha do usuário.

Os resultados estruturados estão em [game/hack-data/campaign_tests.json](../game/hack-data/campaign_tests.json); logs nativos: [história](../game/hack-data/campaign_logs/story.log), [campanha/pós-jogo](../game/hack-data/campaign_logs/campaign.log), [rival](../game/hack-data/campaign_logs/rivals.log).

## O que o teste demonstra

Os scripts conseguem entregar recompensas, iniciar e encerrar batalhas, gravar insígnias e flags, chegar ao Hall da Fama, terminar os créditos, liberar os principais marcos do pós-jogo e continuar a partir do save. As batalhas desta suíte foram concluídas; não ficaram apenas na seleção de golpes ou em alguns turnos.

| Grupo | Execução e resultado |
|---|---|
| Eventos de história | Parcel/Oak, Nacional e EXP global; Bill e SS Ticket; Cut/Fly/Surf/Strength/Rock Smash/Waterfall; Tea e guarda; Rocket Hideout, Lift Key e Silph Scope; Marowak, grunts e Fuji/Poké Flute; Card Key, Giovanni da Silph/Master Ball e Secret Key. 30 checkpoints aprovados. Flash tem fonte de obtenção verificada estaticamente, sem testar a entrega condicional por dez capturas nesta suíte. |
| Oito líderes | Batalhas encerradas e insígnias/recompensas recebidas pelos scripts. O save com oito insígnias foi recarregado em outra instância. |
| Primeira Liga | Lorelei, Bruno, Agatha, Lance e Campeão vencidos; cena do Hall da Fama, gravação e créditos até seu encerramento. |
| Sevii | Introdução, Lostelle/Meteorite, retorno com Bill, pedido e entrega da Ruby, Rainbow Pass, Icefall Cave, Sapphire roubada, dois Admins/Gideon e entrega a Celio. |
| Liberação do pós-jogo | `FLAG_SYS_CAN_LINK_WITH_RS` e remoção do guarda de Cerulean Cave confirmadas após Celio; permaneciam desativadas no primeiro Hall da Fama. |
| Revanche | Cinco lutas concluídas, segundo Hall da Fama, créditos e recarga em outra instância. |
| Opcionais | Mewtwo capturado e registrado no PC, Selphy resgatada, recompensa do Memorial e ativação dos sete interruptores de Tanoby. Não foi percorrida a solução física do labirinto ou dos blocos. |
| Torre | Simples, duplas, Knockout e misto: oito andares registrados em cada modalidade, prêmio recebido e tempo registrado. A modalidade selecionada também é conferida, evitando repetir o mesmo curso por engano. |
| Rival | S.S. Anne, Silph e Route 22 tardio: batalhas e cenas posteriores encerradas em cenários separados, com a variante do inicial padrão da fixture. |

A etapa de líderes/pós-jogo carregou o save produzido pelos 30 eventos da história. Na passagem final não foram montados saves independentes com apenas uma insígnia ou missão já concluída para aprovar cada chefe. Os confrontos adicionais do rival são cenários separados.

## Assistência usada e limites

As fixtures rodam na biblioteca nativa do mGBA, em cópias descartáveis dos fontes da build. A ROM pública não foi alterada.

- O início é preparado após o recebimento do inicial e o primeiro duelo; título, escolha e caminhada inicial não são refeitos.
- O teste usa o **modo Normal**, posiciona o jogador por warp e inicia diretamente scripts reais. Isso **pula barreiras de acesso, trajetos e parte da ordem natural**. Por exemplo, não certifica os caminhos com HMs, acesso por senhas, labirintos, placas de gelo ou movimentação dos blocos de Tanoby.
- A equipe inicial fornecida tem seis Mewtwo no nível 100; sua disponibilidade antes da Liga não é legítima. Antes dos cursos da Torre, a fixture fornece Mewtwo, Rayquaza, Metagross, Kyogre, Tyranitar e Snorlax no 100, com IVs/EVs e itens preparados. Há cura nos deslocamentos. Portanto, **não testa treinamento, economia, dificuldade ou sobrevivência com recursos reais**.
- Master Balls para os cenários selvagens e Lemonade do Memorial são fornecidas. As demais recompensas listadas na etapa de história são obtidas pelos scripts e conferidas no save.
- As escolhas do jogador são respostas automáticas ao controlador de batalha. Dano, adversários, IA inimiga, resultados e recompensas permanecem no motor nativo. Não há vitória ou insígnia forçada no fim de uma batalha.
- Animações de golpes são desligadas pela opção do jogo. O reset ao fim dos créditos é interceptado para coletar evidência; o save é recarregado em uma nova instância usando as funções nativas de continuar a partida.
- A ativação de switches e a invocação de scripts não provam que uma pessoa consiga alcançá-los na ordem correta sem a assistência.

Uma tentativa exploratória com a equipe automática perdeu no oitavo andar da Torre de duplas (`B_OUTCOME_LOST`). Foi uma derrota da automação, não um travamento confirmado. A estratégia foi ajustada e a execução final encadeada foi refeita. Essa aprovação não mede chance de vitória de um jogador.

**Conclusão de alcance:** o fluxo dos eventos e das gravações testadas funciona sob essas condições; a possibilidade de zerar uma partida normal ou Hardcore sem assistência permanece uma validação diferente e pendente. Pedidos repetíveis de Selphy, minigames, ilhas de evento por ticket e todas as variações de iniciais não foram integralmente percorridos.

## Conferência estática de acesso

Comparação com a revisão fixa do pret usada como base:

- **425 mapas:** layouts atribuídos, conexões, warps, eventos de coordenadas/background e objetos antigos preservados.
- **732 arquivos de layouts** preservados, incluindo blocos dos mapas.
- **1.294 warps:** destinos, índices e limites conferidos.
- Cientistas adicionados: sem ocupar warps/objetos antigos, em piso livre; a conectividade do piso dos Centros desde a entrada não perdeu tiles além da posição do próprio NPC.
- Fontes de obtenção dos itens críticos identificadas; guarda de Liga antes da consulta à Nacional em Celio; Sapphire ligada à liberação da rede, caverna e seleção da revanche.

Isso reduz o risco de regressão estrutural, mas não executa todas as condições de portas ou resolve puzzles. Detalhes dos nove checks em [game/hack-data/campaign_tests.json](../game/hack-data/campaign_tests.json).

## Estado final persistido

Além dos retornos dos scripts, foram lidos os Flash saves gerados pelo teste, validando setores e checksums. **111 snapshots** foram conferidos; nenhum save do jogador foi aberto ou alterado.

| Marco | Estado no save final |
|---|---|
| Insígnias | 8 |
| Hall da Fama | 2 entradas |
| Rede de Celio e guarda da caverna | Concluída / removido |
| Mewtwo | Capturado; Pokémon no PC com checksum válido |
| Tanoby | Liberado |
| Torre simples | 8 andares; prêmio e tempo registrados |
| Torre duplas | 8 andares; prêmio e tempo registrados |
| Torre Knockout | 8 andares; prêmio e tempo registrados |
| Torre misto | 8 andares; prêmio e tempo registrados |
| Treinadores Rocket verificados na história | Flags de derrota persistidas |

Os itens de história preservados incluem SS Ticket, Poké Flute, Silph Scope, Lift Key, Card Key, Secret Key, Master Ball e HMs 01/02/03/04/06/07. Parcel e Gold Teeth foram consumidos pelas entregas. O save ao final do pós-jogo voltou ao campo em uma nova instância.

## Repetir

No Ubuntu/WSL, depois do setup e da compilação, com GCC, libmGBA e libpng:

```sh
python3 game/scripts/run_campaign_validation.py
```

O runner executa eventos da história, carrega seu save na sequência assistida, testa os rivais e audita os mapas/saves. Pode levar vários minutos. Os logs e ROMs/saves descartáveis ficam em `game/output/tests/`; não devem ser usados como progresso legítimo do jogador. Qualquer falha retorna código diferente de zero. `--collect-only` apenas confere e reúne resultados já completos para o SHA atual.

O [checklist](TEST_CHECKLIST.md) mantém abertos os percursos e condições ainda não homologados. Esta auditoria adicionou testes e documentação; não gerou uma nova versão do jogo.
