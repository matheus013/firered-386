# Sprites, fundos e fades da cena 4 — pesquisa de 2026-10-27

Objetivo: reaproveitar implementações existentes, verificando o caminho nativo
de 64 bits antes de ligar a cena completa. Resultados de busca não equivalem a
uma validação de execução.

## Referências encontradas

| Parte | Código existente | Decisão |
| --- | --- | --- |
| Sprites | [Cena 4 single screen](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/intro_movie_scene_4.c), `sprite.c`, `sprite_resource.c`, `sprite_transfer.c`; loaders G2D do libntrsystem | Reutilizar a cadeia original de recursos, células e animação. Compilação prévia não comprova carregamento ou desenho. |
| Fundos | Mesma cena, `bg_window.c` e `gf_gfx_loader.c` | Reutilizar tiles, mapas e paletas. Os índices já foram conferidos no NARC privado; falta o teste gráfico completo desses fundos. |
| Fades | [screen_fade.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/screen_fade.c), `screen_fade_effects.c`, `screen_window_wipe.c` | Preferir os módulos HGSS da base. A cena usa efeitos 9 na entrada e 8 na saída, com janelas/HBlank; um simples ajuste de brilho não os substitui. |
| Referência PSP de sprites | [pspoke graphics.c](https://github.com/IbrahimIrfan/pspoke/blob/7477648fbe904a27eef8ba56bcc11c1799475c1a/port/soulsilver-native-menu-sprites/graphics.c) | Útil para a ligação de gerenciadores, templates e transferências. O teste `main.c` encontrado substitui operações de sprite por callbacks controlados; não prova renderização Windows. |
| Referência PSP de fades | [pspoke names.h](https://github.com/IbrahimIrfan/pspoke/blob/7477648fbe904a27eef8ba56bcc11c1799475c1a/port/soulsilver-native-assets/fade-port/names.h) | Adaptação de nomes Platinum/SS, com dependências de tarefas e HBlank. Não importada: nossa base já possui os efeitos HGSS. |

Consultas: `IntroMovie_Scene4_Load`, `sprite_resource`, `Sprite`, `bg_window`,
`BeginNormalPaletteFade`, `NNS_G2dGetUnpackedCellBank`, `NNS_G2dUnpackNCL SDK_PORT`.
A busca pelo último termo não retornou uma correção pronta. Também foi
inspecionado o checkout privado de `cybervisi0n/libntrsystem`, revisão
`97fa995ed33fe2ab8c83ec7068aa3aa5b4c3e12f`, que já contém ramos SDK_PORT para os
formatos 2D. Nenhum fonte PSP foi incorporado nesta rodada.

## Recursos identificados no NARC da abertura

| Uso | Paleta NCLR | Tiles NCGR | Células NCER | Animação NANR / mapas NSCR |
| --- | --- | --- | --- | --- |
| Fundos | 53 | 54 | — | mapas 55, 56, 57, 58 |
| Mãos/efeitos de sprites | 81 | 82 | 84 | animação 83 |
| Chikorita | 85 | 86 | 88 | animação 87 |
| Cyndaquil | 93 | 94 | 96 | animação 95 |
| Totodile | 89 | 90 | 92 | animação 91 |

Tiles, mapas, células e animações indicados como `_lz` na cena usam LZ10.
Os arquivos extraídos permanecem privados.

## Bloqueio confirmado e corrigido: paletas nativas

O loader NCLR do SDK copiava o tamanho do bloco inteiro a partir do início dos
dados de cor. Na paleta 53, ASan detectou leitura de 536 bytes que ultrapassava
o arquivo alocado de 552 bytes. Além disso, o caminho nativo colocava um ponteiro
de 64 bits no descritor de disco do DS, cujo campo de offset tem 32 bits.

`prepare_sdk_palette_loader.py` verifica o hash da revisão e adapta a cópia
privada: usa descritores nativos separados e referencia os dados originais sem
copiar o bloco nem escrever ponteiros no arquivo. O ramo DS permanece intacto.
Mantém o cache circular de descritores do SDK; o buffer de origem deve permanecer
vivo enquanto seus dados são usados. Não resolve a capacidade/longevidade desse
cache para todos os recursos do jogo.

`verify_sdk_palette_loader.py` compilou os loaders completos NCL e Load com
ASan/UBSan, reproduziu a falha original e validou as cinco paletas 53/81/85/89/93
na versão adaptada, comparando tamanho, formato, bytes das cores e preservação
integral do arquivo. Relatório: `hgss-sdk-palette-loader-test.json`. Não é um teste
de arquivos malformados, renderização ou de todos os formatos Nitro 2D.

Próxima integração: validar tiles/células/animações, desenhar os sprites e fundos
com os gerenciadores originais e então executar os fades 8/9 com tarefas e HBlank.
