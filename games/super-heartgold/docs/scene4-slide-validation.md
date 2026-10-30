# Janelas e scroll combinados — 30/10/2026

O diagnóstico nativo agora executa simultaneamente os helpers originais de
janela e scroll, usando os parâmetros de entrada/saída dos jogadores da
[cena 4 original](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/intro_movie_scene_4.c).
Essa fonte foi consultada no GitHub antes da implementação.

O harness aplica offsets iniciais +192/-192 e deslocamentos -192/+192 em dez
ticks, junto às duas janelas. Repete o trecho para capturar os dois motores em
single screen. As funções de interpolação, tarefas e commit são originais;
o agendamento das quatro fases pertence ao harness, não à máquina de estados
da cena. Sprites, partículas, outros BGs e fades não participam deste teste.

[Resultado](hgss-scene4-slide-test.json): 36 capturas intermediárias coincidem
pixel a pixel com decodificação CPU e recorte independente; quatro finais
exatos; evolução em ordem; tarefas de janela e scroll concluídas nos dois
motores. Também passaram novamente os 512 ciclos de cancelamento/reinício.
[Regressão de scroll isolado](hgss-scene4-slide-scroll-regression.json).

Durante a preparação, a referência visual assumiu incorretamente que
`PM_LCD_TOP` selecionava o motor principal. Na SDK o valor é zero, e o helper
original encaminha zero ao secundário. A referência foi corrigida; não houve
mudança no roteamento original nem correção de bug do renderer neste avanço.

O teste mantém o estado final das janelas para inspecioná-lo. Assim, a coluna
WIN1 que permanece após a abertura é incluída na referência durante a saída.
O desligamento posterior de BG1 e das janelas pela máquina de estados não é
executado. A correspondência admite deslocamento temporal da captura em relação
ao loop, mas exige a mesma etapa para a janela e o scroll em cada imagem.

Reprodução: acrescente `--scene4-slide` ao comando de
[validação do scroll](bg-scroll-validation.md), usando um novo `--output`.

Continua sendo diagnóstico Linux de componentes combinados, sem boot/menu,
abertura completa ou execução Windows. A issue #10 permanece em 10%.
