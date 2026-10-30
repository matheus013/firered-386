# Pan de janelas da abertura — 30/10/2026

O helper original `IntroMovie_StartWindowPanEffect` agora passa no diagnóstico
nativo Linux com single screen. Não é execução da cena completa nem boot do jogo.

## Referências consultadas antes da adaptação

- [Helper original](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/intro_movie_scenes_lib.c):
  a largura completa depende da união de WIN1 (0..1) com WIN0 (1..0, circular).
- [Renderer upstream](https://github.com/cybervisi0n/libntr/blob/f6b3a690c72b1484fda86c47021dbda2b54ffb69/libraries/sim/src/sim_g2.cpp):
  os caminhos de BG de texto verificavam somente WIN0 com intervalo simples.

## Correção e evidências

O adaptador usa uma seleção compartilhada de máscara para BG de texto e OBJ
colorido: WIN0 tem prioridade sobre WIN1; fora das duas vale WINOUT. Intervalos
circulares e bordas exclusivas são tratados nos dois eixos.

- [Baseline](hgss-window-pan-baseline.json): ambos os finais abertos ficaram
  incorretamente pretos. O baseline também contém um flash inicial do próprio
  harness; sua inicialização foi corrigida antes do teste final.
- [Pan original](hgss-window-pan-test.json): quatro fases, 36 capturas
  intermediárias, progressão monotônica com pelo menos dois estados parciais
  por fase, quatro finais exatos e conclusão das tarefas originais VBlank.
  Os fundos abertos são comparados a uma referência CPU independente.
  Neste relatório o campo legado `invalid_rows` conta pixels inconsistentes.
- [Fixtures dos registradores](hgss-window-masks-test.json): 540 verificações
  com ASan/UBSan, cobrindo WIN1 isolado, prioridade na sobreposição, máscara
  de OBJ, largura completa, intervalos circulares e bordas nos dois motores.
- [Regressão dos fades](hgss-window-pan-fade-regression.json): efeitos originais
  9/8 com BG1 + OBJ continuam aprovados nas quatro fases.

O harness chama o código original de pan, mas escolhe seus próprios parâmetros
e quadros. Não valida janela OBJ, efeitos de cor por janela, BG afim, 3D,
temporização de hardware, máquina de estados completa ou Windows. A coluna
adicional de WIN1 é coberta explicitamente pelas fixtures; o fundo real pode
ter pixels pretos nessa coluna. Nenhum recurso privado foi incluído no Git.

## Reprodução

`tools/verify_scene_sprites.py` aceita `--manager --fade-probe --window-pan
--scanline-windows`, junto aos mesmos caminhos privados `--workspace`, `--stack`,
`--archive`, `--hgss`, `--probe`, `--sprite-reference` e um `--output` novo.
Use `--fade-sprites` no lugar de `--window-pan` para a regressão dos fades.
As fixtures independentes usam `python3 tools/verify_window_masks.py --output
resultado.json`, com compilador C++ e sanitizadores disponíveis.
