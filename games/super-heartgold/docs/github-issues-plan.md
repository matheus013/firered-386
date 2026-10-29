# Issues planejadas — Super HeartGold nativo

Preparado em 28/10/2026 e publicado como issues em 29/10/2026 após reconectar o
plugin GitHub. Acompanhar pela [issue principal #5](https://github.com/matheus013/firered-386/issues/5).
As entregas abaixo correspondem, na ordem, às issues #6–#13. Carregadores (#6)
e fundos estáticos (#7) estão encerrados; as demais permanecem abertas.

## Issue principal: acompanhar o port nativo Windows single screen

Objetivo: executar a engine nativamente no Windows, preservando single screen, até validar campanha e pós-jogo.

Estado geral: ainda não jogável. Partículas, alternância de motor e componentes isolados têm testes aprovados. Abertura completa, menu, mapas e campanha ainda não foram validados. Não há percentual global confiável; acompanhar pelos critérios abaixo.

Marco anterior: [8c919fc](https://github.com/matheus013/firered-386/commit/8c919fc).
As correções de animações, testes e relatórios seguintes acompanham este documento
no Git. As tentativas de 28/10/2026 retornavam HTTP 403; a conexão do plugin
permitiu publicar as nove issues em 29/10/2026.

Ordem de dependência: carregadores → fundos/sprites → fades e composição → boot/menu → primeiro trecho jogável → campanha/pós-jogo. A preparação da toolchain Windows pode ocorrer em paralelo, mas a validação Windows completa depende da integração do jogo.

A issue principal contém os links das oito entregas. A cada entrega, registrar commit, plataforma, comando, resultado e limites do teste. Não usar compilação ou screenshots isoladas como comprovação de campanha jogável.

## 1. [Super HeartGold] Corrigir e consolidar os carregadores Nitro 2D nativos

O loader NANR deixava pStringBank e pExtendedData sem inicialização quando os recursos opcionais estavam ausentes. A falha foi reproduzida; a adaptação local inicializa os campos. Os 17 recursos da cena 4 passaram por 512 ciclos de carga/liberação cada com ASan/UBSan após a correção. A correção anterior de paletas já está publicada.

- [x] Reproduzir a falha dos ponteiros opcionais.
- [x] Validar a adaptação nos 17 recursos, preservando os bytes de entrada.
- [x] Recompilar a pilha SDK com a adaptação integrada.
- [x] Registrar código, relatório e instruções reproduzíveis no Git.
- [x] Documentar limites de vida útil de descritores e buffers.

Não valida GPU nem o jogo completo.

## 2. [Super HeartGold] Validar os seis fundos da cena 4 no renderizador nativo

O diagnóstico compilou e gerou capturas dos BG1/BG2/BG3 dos motores principal e secundário em 512×384. Após corrigir a expansão RGB555 no comparador para a faixa completa, os seis casos passaram com zero pixels divergentes. Não houve alteração no renderizador para fazer o teste passar.

- [x] Conferir a conversão de cores e corrigir o comparador quando necessário.
- [x] Comparar todos os pixels das seis combinações camada/motor.
- [x] Inspecionar os quatro mapas 55–58.
- [x] Confirmar alternância do motor visível em single screen.
- [x] Registrar teste e relatório no Git; capturas com recursos do jogo permanecem privadas.

Ainda não executa a rolagem nem a máquina de estados da cena.

## 3. [Super HeartGold] Integrar sprites e animações da abertura ao desenho nativo

Reutilizar os módulos originais de recursos, células, animação e transferência. Carregamento aprovado não comprova desenho.

Avanço parcial em 28/10/2026: diagnóstico de células iniciais passou para as
duas mãos, brilho e três iniciais. Usa controlador e conversão OAM do SDK,
com imagens iguais nos dois motores e controles sem desenho inteiramente
pretos. Relatório: [hgss-scene-sprites-test.json](hgss-scene-sprites-test.json).
Não conclui a integração do gerenciador original nem a reprodução temporal.

Em 29/10/2026, o controlador passou por 7.224 ticks com seis sequências, após
corrigir uma leitura fora do buffer no término. O diagnóstico gráfico passou
também em playback de 160 frames com deslocamentos, mudanças visíveis e
controles sem desenho pretos. Ver [relatório temporal](hgss-animation-controller-test.json)
e [relatório gráfico](hgss-scene-playback-test.json). A integração do gerenciador
completo e a composição continuam pendentes na issue #8.

Também passaram 12 casos de composição estática BG1/OBJ (seis sprites nos
dois motores), com fundo exato no controle e pixels coloridos dos sprites
preservados. [Relatório](hgss-scene-composition-test.json). Ainda não é a
composição completa da cena nem a integração de SpriteList.

Na preparação de SpriteList foi corrigido o armazenamento interno de animações
para os tipos nativos: 116 bytes originais eram insuficientes para variantes de
136/144/200 bytes. O teste de layout/alinhamento passou e `sprite.c` completo
compilou com o header adaptado. [Relatório](hgss-sprite-storage-test.json).
Ainda não valida execução do gerenciador; seus consumidores precisam ser
recompilados juntos, pois o layout de Sprite mudou no ramo nativo.

- [ ] Integrar gerenciadores e transferências originais.
- [ ] Desenhar mãos/efeitos e Chikorita, Cyndaquil e Totodile.
- [ ] Validar frames, sequências, posições, transparência e paletas.
- [ ] Validar OAM, prioridades e composição com os fundos.
- [ ] Validar recursos nos dois motores e seleção da tela visível.
- [ ] Testar descarte/recriação e publicar resultados.

Depende de carregadores e fundos. Consultar referências GitHub antes de novas adaptações.

## 4. [Super HeartGold] Integrar fades 8/9, janelas e HBlank da cena 4

A base contém screen_fade.c, screen_fade_effects.c e screen_window_wipe.c. Os efeitos 9 de entrada e 8 de saída dependem de tarefas, janelas e HBlank.

- [ ] Conferir implementações existentes antes de adaptar.
- [ ] Integrar tarefas e atualizações de janela/HBlank.
- [ ] Validar progressão, término e repetição dos efeitos 8/9.
- [ ] Validar ambos os motores em single screen.
- [ ] Publicar capturas e resultados.

Ajustar brilho não comprova os efeitos originais. Depende da composição e das tarefas.

## 5. [Super HeartGold] Executar a abertura original e chegar ao menu pelo fluxo normal

O jogo completo ainda não inicia; os diagnósticos não executam seu boot.

- [ ] Atualizar levantamento de dependências de link/runtime do executável real.
- [ ] Integrar gráficos e tarefas à máquina de estados original.
- [ ] Executar abertura e saída normal sem travamentos.
- [ ] Validar pular a abertura pelos controles previstos.
- [ ] Chegar ao menu inicial com entrada funcional.
- [ ] Repetir início/saída e verificar liberação de recursos.
- [ ] Publicar relatório de execução.

Depende das issues 1–4. Compilar módulos ou eliminar símbolos não basta.

## 6. [Super HeartGold] Consolidar build e execução nativos no Windows com single screen

Há testes Windows de componentes e testes gráficos Linux/WSLg. Não há build Windows jogável confirmado.

- [ ] Reproduzir toolchain e dependências em ambiente limpo.
- [ ] Compilar e ligar o executável completo Windows.
- [ ] Validar janela, gráficos, controles e coordenadas de toque.
- [ ] Validar single screen, áudio e caminhos de recursos.
- [ ] Validar gravação, encerramento e reabertura.
- [ ] Publicar instruções e artefatos permitidos.

Port real, sem substituir por emulação interna. ROM, recursos extraídos e fontes externos privados não entram no Git nem nos artefatos públicos.

## 7. [Super HeartGold] Validar o primeiro trecho jogável, save e retomada

- [ ] Criar novo jogo pelo menu, sem injeção de estado ou teleporte.
- [ ] Concluir introdução e obter controle do personagem.
- [ ] Caminhar, trocar de mapa e conversar com NPCs.
- [ ] Receber o inicial e concluir uma batalha.
- [ ] Usar menus básicos com single screen.
- [ ] Salvar pelo jogo, encerrar, reabrir e retomar o progresso.
- [ ] Repetir no Windows e publicar evidências.

Depende de boot/menu e runtime Windows. Estado atual: não validado.

## 8. [Super HeartGold] Criar matriz de regressão e validar campanha e pós-jogo do port

- [ ] Criar matriz de testes por plataforma, build/commit e cenário.
- [ ] Cobrir mapas, colisões, diálogos, menus, inventário e batalhas.
- [ ] Cobrir captura, evolução, áudio, transições, entrada e save/load.
- [ ] Executar campanha por caminhos normais até a Liga.
- [ ] Executar pós-jogo e registrar objetivos efetivamente testados.
- [ ] Abrir issues específicas para bloqueios reproduzidos.
- [ ] Publicar itens aprovados, falhos e não testados.

Depende do primeiro trecho jogável. Não exige Pokédex completa. Não presume que as modificações anteriores de FireRed estejam implementadas nesta base HeartGold/SoulSilver.

### Evidência adicional da issue #8 — ciclo de vida de SpriteList

64 ciclos, 448 criações e 61.440 ticks aprovados no Linux com ASan/UBSan nas
unidades exercitadas (ASan de globais desativado). Gerenciador original, com
adaptadores de heap/IO/assert. Ordenação, capacidade e reutilização verificadas.
[Relatório](hgss-sprite-manager-test.json). Renderização pelo gerenciador,
multicell, transferência e integração da cena continuam pendentes.

### Issue #8 — primeiro teste gráfico do gerenciador original

Renderização estática via SpriteList e GF_InitG2dRenderer aprovada em seis
casos nos dois motores, com controles desativados e referência cell/OAM.
Três casos são visíveis; três começam fora da tela e validam emissão OAM e
tela vazia. [Relatório](hgss-sprite-manager-render-test.json).
Ainda faltam playback pelo gerenciador e composição da abertura completa.
