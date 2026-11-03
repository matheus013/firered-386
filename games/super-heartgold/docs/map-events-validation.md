# Eventos do campo: parser nativo — 02/11/2026

O parser original de eventos e suas funções de consulta executaram em Linux x86-64 e em um EXE Windows x86-64. Os 491 membros do arquivo de eventos da ROM privada passaram na comparação byte a byte. Isso prepara a integração do campo; **não é um mapa jogável**.

Referência consultada antes da adaptação: [map_events.c do pokeheartgold-single-screen](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/map_events.c). O teste inclui essa unidade privada sem modificá-la, para chamar também seu parser estático. Não copia o código original nem os recursos para este repositório.

## Evidências

| Verificação | Resultado |
|---|---|
| Membros de eventos | 491/491 idênticos após leitura pelas funções originais |
| Eventos de cenário / objetos / portas / gatilhos | 1.034 / 2.667 / 1.317 / 195 |
| Layout binário nativo | 20 / 32 / 12 / 16 bytes, verificados na compilação |
| Seções vazias | Ponteiro nulo; memória preenchida previamente com `0xA5` |
| Busca por coordenadas | Retorna a primeira porta correspondente, inclusive coordenadas duplicadas |
| Consultas inválidas | Índice igual à quantidade, `UINT32_MAX` e coordenada negativa rejeitados |
| Escada do quarto inicial | Destino e retorno recíprocos confirmados nos dados dos mapas 64 e 63 |
| Linux | ASan e UBSan aprovados; instrumentação ASan de globais desativada |
| Windows | EXE MinGW executado via interoperabilidade WSL, sem sanitizadores |

Relatórios: [Linux](hgss-map-events-linux-test.json), [Windows](hgss-map-events-windows-test.json).

O leitor Python verifica os limites de todas as seções antes de entregar os dados ao parser original. Isso não demonstra que o parser original suporta arquivos corrompidos. O teste abrange o arquivo de eventos inteiro, mas só verifica a reciprocidade de destinos da escada inicial; não valida todas as transições do mundo.

## Reprodução

Execute no WSL, com as dependências privadas já preparadas:

```sh
python3 tools/verify_map_events.py \
  --hgss /root/.cache/super-heartgold/reference \
  --probe /root/.cache/super-heartgold/hgss-spl-integrated1 \
  --stack /root/.cache/super-heartgold/sdk-stack-scanline1 \
  --archive /root/.cache/super-heartgold/private-events1/a/0/3/2 \
  --output /root/.cache/super-heartgold/map-events-new-run
```

Para Windows, acrescente `--target windows --sdl-include /root/.cache/super-heartgold/aot-reference/windows-static-deps/include` e escolha outra saída. O diretório deve ser novo e externo ao Git. Os membros extraídos, resultados binários e executáveis ficam privados. O arquivo de entrada foi extraído com `import_nds_resources.py`; seus hashes estão nos relatórios.

As opções Windows que removem unwind tables permitem descartar funções não chamadas neste diagnóstico; não definem as opções do executável final.

## Carregamento original integrado ao diagnóstico — 02/11/2026

O modo `--script-archive` agora chama as funções originais `Field_AllocateMapEvents`, `Field_InitMapEvents` e `Field_FreeMapEvents` nos dois andares. São 64 alocações/liberações e 128 carregamentos alternados por plataforma, mantendo uma única alocação durante cada par de mapas. Os bancos de eventos e scripts são derivados do MapHeader original; o adaptador verifica que ambos os mapas não têm encontros selvagens.

O teste verifica que a tabela de encontros fica zerada, que a cauda do cabeçalho de scripts é limpa mesmo partindo de memória preenchida com `0xA5`, e compara os bytes carregados com os membros privados da ROM. A regressão dos 491 membros continua sendo executada no mesmo teste. Linux passou com ASan/UBSan e Windows com execução efetiva do EXE, sem sanitizadores.

Relatórios adicionais: [Linux](hgss-field-load-linux-test.json), [Windows](hgss-field-load-windows-test.json). Para reproduzir, acrescente `--script-archive /root/.cache/super-heartgold/private-field-scripts1/a/0/1/2` ao comando anterior e use outro diretório de saída. A extração usa o importador privado já existente, selecionando `a/0/1/2`.

Os serviços de heap, arquivo e consulta de MapHeader ainda são adaptadores explícitos do diagnóstico. O teste não cobre mapas com encontros selvagens e não inicializa o FieldSystem completo. Carregar o cabeçalho não significa executar o script. Os relatórios anteriores de parser permanecem como evidência do escopo anterior.

## Seleção de cenas com variáveis originais — 03/11/2026

O modo `--script-selection`, junto de `--script-archive`, compila também as unidades originais `script_manager.c` e `save_vars_flags.c`. A consulta ao GitHub confirmou as implementações de [GetMapSceneScriptId e GetMapLoadScriptId](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/script_manager.c). Cabeçalhos de índices ausentes são gerados na área privada a partir dos identificadores numerados usados pela unidade.

Passaram 640 combinações por plataforma (64 ciclos × 2 mapas × 5 estados), com leitura/escrita original da variável `0x4106`, seleção original da cena e bloqueio por `warpResetSuppressed`. O bloco `SaveVarsFlags` é inicializado pela função original; `SaveArray_Get` é um adaptador explícito para esse bloco isolado. Variáveis especiais abortam o teste caso sejam acessadas inesperadamente.

| Mapa | Estado testado | Resultado original |
|---|---|---|
| Primeiro andar | 0 | Script 1 |
| Primeiro andar | 3 | Script 7 |
| Primeiro andar | 1, 2, 4 | Nenhuma cena |
| Quarto inicial | 0, 1, 2, 3, 4 | Nenhuma cena automática: cabeçalho vazio |

A referência esperada é conferida contra os bytes dos dois cabeçalhos privados antes da execução. Também passaram a rejeição de escrita em um valor imediato, a leitura de valores imediatos e a ausência de scripts dos tipos 2–4. O teste dos 491 arquivos e os 128 carregamentos continuam incluídos.

Relatórios: [seleção Linux](hgss-script-selection-linux-test.json), [seleção Windows](hgss-script-selection-windows-test.json). Linux usa ASan/UBSan com instrumentação de globais desativada; Windows executa o EXE sem sanitizadores. Acrescente `--script-selection` ao comando de carregamento para reproduzir.

**Selecionar um script não executa seus comandos.** Não foram chamados `StartMapSceneScript`, `StartMapLoadScript` ou o loop completo do FieldSystem. Não há save completo, persistência, diálogo, movimentação ou cena demonstrados por esse teste.

## Próxima integração

Ainda falta integrar os serviços reais de arquivos e MapHeader, executar scripts, carregar encontros de mapas externos, criar objetos, integrar colisões e desenho ao FieldSystem e atravessar a escada com comandos do jogador. A seleção já foi validada; o próximo trecho é `StartMapSceneScript`/`CreateScriptContext`, a tabela de comandos e seu agendamento pelo TaskManager. O teste atual não renderiza o quarto, move o personagem, salva ou retoma uma partida. A issue #12 permanece em 0% de sua entrega jogável.

## Entrada no interpretador original — 03/11/2026

O modo `--script-dispatch` (exige `--script-selection --script-archive`) liga também o [interpretador original script.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/script.c). Ele inicializa o contexto, usa `ScriptRunByIndex` original sobre o banco privado do primeiro andar e chama `RunScriptCommand` até o primeiro comando. Um handler de diagnóstico encerra explicitamente a execução nesse ponto, sem simular sucesso do comando.

Nos scripts 1 e 7, os deslocamentos originais conferem com um cálculo independente: 30 e 220 bytes. Ambos chegam ao opcode 609, `ScrCmd_FollowMonResetMovement`. O contexto parado não executa novos comandos. Windows e Linux passaram, junto das regressões anteriores. **Zero scripts concluídos; nenhum efeito de comando foi executado.** `CreateScriptContext` e a tabela real de handlers ainda não estão integrados neste teste.

Relatórios: [entrada Linux](hgss-script-dispatch-linux-test.json), [entrada Windows](hgss-script-dispatch-windows-test.json). Os executáveis e bytecodes permanecem privados.

Durante a implementação, um filtro de nomes omitia um handler com prefixo diferente e deslocava os opcodes. O leitor agora preserva todas as posições da tabela (853 entradas nesta referência) e rejeita inicializadores que não sabe interpretar. A regressão `python3 tools/test_script_command_table.py` cobre o handler com nome diferente e a rejeição de um inicializador designado. Os relatórios publicados foram gerados depois dessa correção.

O primeiro bloqueio concreto está nas dependências dos objetos/acompanhante utilizadas por `ScrCmd_FollowMonResetMovement`, seguidas pelos comandos de bloqueio e movimento dos objetos. Integrar esses serviços ao campo é necessário para demonstrar a cena; trocar os comandos por handlers vazios não atende ao marco jogável.

## Primeiro comando com acompanhante inativo — 03/11/2026

O modo `--inactive-follower`, junto de `--script-dispatch`, executa o ramo original de `ScrCmd_FollowMonResetMovement` com `followMon.active = 0`. As funções `ScrCmd_FollowMonResetMovement` e `FollowMon_IsActive` são extraídas sem alterações para uma unidade privada; seus hashes constam nos relatórios. Isso permite testar o ramo sem afirmar que as unidades completas `scrcmd_c.c` e `follow_mon.c` já estão integradas.

Nos scripts 1 e 7, o comando retorna ao interpretador, que permanece em modo bytecode e avança exatamente dois bytes até `ScrCmd_LockAll`. São duas execuções de comando original por plataforma, com zero scripts completos. Os serviços de acesso ao time, busca do objeto acompanhante e reset de movimento abortam se forem usados: nenhum foi chamado nesse ramo inativo. O estado é uma fixture explícita; não demonstra que o boot real já constrói o FieldSystem correto.

Passaram Linux com ASan/UBSan (globais desativadas) e Windows com EXE executado, sem sanitizadores. Relatórios: [acompanhante inativo Linux](hgss-inactive-follower-linux-test.json), [acompanhante inativo Windows](hgss-inactive-follower-windows-test.json). As regressões de carregamento, seleção e parser continuam incluídas.

O próximo bloqueio é `LockAll`: requer o ambiente de scripts e o gerenciador real dos objetos, seguido pela aplicação dos movimentos. Acompanhante ativo, travessia, diálogos, renderização e save completo permanecem sem validação. Os testes anteriores de parada no primeiro comando continuam disponíveis sem `--inactive-follower`.

## LockAll com gerenciador de objetos de teste — 03/11/2026

O modo `--lock-all` (exige `--inactive-follower`) liga a unidade original `map_object.c` e executa o `ScrCmd_LockAll` original, extraído sem alterações. A consulta ao GitHub confirmou [MapObjectManager_PauseAllMovement](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/map_object.c). O teste usa as funções originais de pausa/despausa e de acesso ao ambiente de scripts; apenas `TaskManager_GetEnvironment` é adaptado para devolver o ambiente explícito do teste.

Em cada um dos dois scripts, o gerenciador tem três slots: dois ativos e um inativo. `LockAll` adiciona o flag de pausa somente aos ativos, preserva os outros flags e retorna ao interpretador. `MapObjectManager_UnpauseAllMovement` restaura os flags iniciais. O ponteiro de bytecode avança exatamente até `ApplyMovement`, onde o diagnóstico para sem simular movimento.

Passaram Linux com ASan/UBSan e execução do EXE Windows. São quatro comandos originais executados por plataforma (dois em cada script), **zero scripts completos**. O modo anterior sem `--lock-all` também passou na regressão Linux após a mudança.

Relatórios: [LockAll Linux](hgss-lock-all-linux-test.json), [LockAll Windows](hgss-lock-all-windows-test.json). As funções extraídas e a unidade `map_object.c` têm hashes registrados. Dados e código extraídos permanecem privados.

Limites: os slots são construídos pelo diagnóstico, não por `Field_InitMapObjectsFromZoneEventData`; não há escalonamento real do TaskManager, NPC de última interação ou acompanhante ativo. Esses ramos não são simulados como sucesso. O próximo bloqueio é a criação e execução do gerenciador de movimentos de `ApplyMovement`, com objetos reais do mapa. O campo continua não jogável.

## ApplyMovement: correção do deslocamento relativo em 64 bits — 03/11/2026

O modo `--movement-request` (exige `--lock-all`) entra em `ScrCmd_ApplyMovement`, resolve o personagem pela função original `Script_GetMapObjectByPerson` e verifica a chamada ao criador de tarefas. A chamada é interceptada com uma interrupção explícita antes da alocação; não devolve uma tarefa fictícia nem simula sucesso.

Foi identificada uma incompatibilidade no trecho original: o deslocamento relativo é lido em `u32` e somado a um ponteiro. O segundo script usa `-68`, codificado em 32 bits; em um host de 64 bits, tratá-lo como sem sinal adicionaria `4294967228`, em vez de retroceder 68 bytes. A preparação privada agora adapta apenas essa declaração para `s32 offset = (s32)ScriptReadWord(ctx);`, com hashes antes/depois registrados. O checkout original não foi alterado. Essa adaptação ainda precisa acompanhar a futura compilação da unidade completa no jogo.

| Script | Personagem | Deslocamento relativo | Destino no banco |
|---|---:|---:|---:|
| 1 | 255 | +122 | 164 |
| 7 | 255 | -68 | 164 |

Nos dois casos, a identidade do objeto e o endereço exato do movimento conferem com uma leitura Python independente. O contador de movimentos permanece zero, pois a interrupção ocorre antes de criar/agendar a tarefa. Passaram Linux ASan/UBSan e EXE Windows; as regressões anteriores continuam incluídas. Não se trata de movimento executado nem de cena concluída.

Relatórios: [requisição Linux](hgss-movement-request-linux-test.json), [requisição Windows](hgss-movement-request-windows-test.json). O próximo serviço é [EventObjectMovementMan_Create](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/map_object_movement_commands.c), seguido pela fila SysTask e pela máquina de movimentos. Nenhuma tarefa foi criada neste teste.
