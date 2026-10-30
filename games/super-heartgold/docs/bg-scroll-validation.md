# Deslocamento original dos fundos — 30/10/2026

O diagnóstico nativo Linux executa as tarefas originais de scroll da abertura,
com o `BgConfig` e as rotinas de atualização de GPU originais. Não é execução
da abertura completa, boot, menu ou jogo jogável.

## Referências reutilizadas

Consultados no GitHub antes da integração:

- [intro_movie_scenes_lib.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/intro_movie_scenes_lib.c):
  tarefas VBlank e principal, interpolação e conclusão.
- [bg_window.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/bg_window.c):
  posição dos fundos, escrita imediata e atualização agendada.
- [math_util.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/math_util.c):
  dependência `MTX22_2DAffine`, vinculada sem substituir sua implementação.

Os três arquivos são compilados diretamente da referência privada, sem edições.
O harness usa os recursos BG1 já validados e escolhe quatro movimentos de dez
ticks: (-48,+32), retorno, (+37,-24), retorno. Ambos os motores recebem os
movimentos. As capturas single screen mostram o motor principal nas duas
primeiras fases e o secundário nas duas últimas.

## Evidências e limites

[Resultado do scroll](hgss-bg-scroll-test.json): 36 capturas intermediárias
coincidem integralmente com posições calculadas por um decodificador CPU
independente de tiles/mapas. As posições avançam em ordem e incluem pelo menos
dois estados intermediários por fase. Os quatro finais coincidem pixel a pixel.
O harness verifica também a conclusão, liberação da tarefa, contador e posição
final nos dois motores. A fila principal usa o commit agendado no VBlank.

[Regressão de janelas](hgss-bg-scroll-pan-regression.json): o pan WIN0/WIN1
continua aprovado após a extensão do decodificador para offsets.

Este teste admite diferenças de fase entre captura e execução do loop; não
comprova temporização idêntica ao DS. Não cobre BG afim, scroll infinito,
cancelamento, combinações simultâneas de scroll/fade/sprites, nem Windows.
Os recursos continuam privados. A issue #10 permanece em 10%: foi validada
mais uma dependência, mas ainda falta executar a máquina de estados e o menu.

## Reprodução

Use `tools/verify_scene_sprites.py` com `--manager --fade-probe --bg-scroll
--scanline-windows` e os caminhos privados `--workspace`, `--stack`, `--archive`,
`--hgss`, `--probe`, `--sprite-reference`, além de um diretório novo em `--output`.
O modo exige compilação nativa Linux e o mesmo ambiente gráfico dos diagnósticos
anteriores. Para a regressão, substitua `--bg-scroll` por `--window-pan`.
