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

## Carregadores e fundos validados em 28/10/2026

A consulta GitHub por `NNS_G2dGetUnpackedCellBank` confirmou os pontos de ligação
em `gf_gfx_loader.c` e `sprite_resource.c` da revisão HGSS acima. As consultas
por `NNS_G2dUnpackNCG` e `pStringBank pExtendedData` no libntrsystem não trouxeram
uma correção pronta. A busca por `GXS_LoadBG1Scr` no libntr confirmou a API de
transferência; os testes mantêm a revisão local fixada `380a3d7`, sem atualizar
automaticamente para a revisão mais recente da busca.

`verify_sdk_scene_loaders.py` executa os loaders completos de NCGR/NSCR/NCER/NANR
contra 17 recursos privados da cena, por 512 ciclos de carga/liberação cada.
Confere dimensões, offsets, atributos OAM, retângulos das células, sequências,
duração e conteúdo dos frames, além da preservação integral da entrada.
O teste ampliado reproduziu dois ponteiros opcionais não inicializados no
descritor NANR. `prepare_sdk_animation_loader.py` inicializa `pStringBank` e
`pExtendedData` no caminho nativo, mantendo a reconstrução posterior das
extensões presentes. Os quatro NANR da cena não contêm esses dados opcionais;
o teste não cobre conteúdo de extensões presentes nem arquivos malformados.

Os 8.704 ciclos passaram com ASan/UBSan após a adaptação. Resultado:
[hgss-scene-loaders-test.json](hgss-scene-loaders-test.json). O comando requer
um `--stack` anterior à adaptação NANR, para reproduzir a falha original antes
de testar a correção. A compilação completa do SDK com a adaptação integrada
também passou: [hgss-sdk-stack-build.json](hgss-sdk-stack-build.json).

`verify_scene_backgrounds.py` compila um diagnóstico gráfico novo usando os
loaders do SDK e as APIs GX. Reproduz os parâmetros estáticos dos BG1/2/3 da cena,
carrega os tiles 54, paleta 53 e mapas 55–58 e alterna o motor visível com o
código single screen original. Cada camada é desenhada isoladamente nos dois
motores. As seis capturas de 512×384 tiveram **zero pixels divergentes** contra
um decodificador independente de mapa 4bpp, incluindo flip, banco de paleta e
transparência. As capturas também foram inspecionadas visualmente.

A primeira comparação reprovava por expandir RGB555 apenas com deslocamento;
o framebuffer usa componentes normalizados de 0 a 255. O comparador corrigido
usa arredondamento de `canal * 255 / 31`, sem tolerância ou máscara de pixels.
O renderizador não foi alterado para fazer essa comparação passar.
Resultado: [hgss-scene-backgrounds-test.json](hgss-scene-backgrounds-test.json).

Exemplo de reprodução, com caminhos privados substituídos pelos seus:

```sh
python3 tools/verify_sdk_scene_loaders.py --stack /private/sdk-before-nanr-fix --archive /private/opening.narc --output /private/loaders-new
python3 tools/verify_scene_backgrounds.py --hgss /private/hgss --libntr /private/libntr --stack /private/sdk-with-palette-fix --archive /private/opening.narc --output /private/backgrounds-new
```

Os testes gráficos ocorreram em Linux/WSLg, com áudio dummy. Não executam os
gerenciadores HGSS de sprites/BG, rolagem, composição completa ou máquina de
estados da abertura. Próxima integração: desenhar os sprites com os gerenciadores
originais, compor a cena e executar os fades 8/9 com tarefas e HBlank. O jogo
completo e a execução Windows continuam pendentes.

## Primeiras células dos sprites — 28/10/2026

A pesquisa por `NNS_G2dDrawCell` na base single screen confirmou que `sprite.c`
usa `NNS_G2dDrawCellAnimation`. Para validar a etapa de células/OAM antes do
gerenciador completo, `verify_scene_sprites.py` liga os loaders e os módulos
originais `g2d_Animation`, `g2d_CellAnimation`, `g2d_SRTControl` e
`g2d_CellTransferManager` ao diagnóstico nativo. A busca por
`NNS_G2dMakeCellToOams` no libntrsystem não retornou resultados indexados, mas
a implementação e a declaração existem no checkout privado fixado.

O harness inicializa cada sequência pelo controlador original, obtém sua célula
inicial, converte-a com `NNS_G2dMakeCellToOams` e envia tiles, paletas e OAM pelas
APIs GX. As tabelas de dimensões OBJ são instanciadas pelas macros existentes
no header do SDK; não foi introduzido um renderer substituto.

Os seis casos são sequências 0/1/2 do recurso 83 (duas mãos e brilho) e sequência
0 dos recursos 87/95/91 (Chikorita/Cyndaquil/Totodile). Cada caso executa em dois
processos: desenho habilitado e conversão para OAM desabilitada. As capturas
dos motores principal/secundário são idênticas, as seis imagens habilitadas
contêm sprites visíveis e os seis controles ficam inteiramente pretos.
As capturas foram inspecionadas visualmente. Resultado:
[hgss-scene-sprites-test.json](hgss-scene-sprites-test.json).

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-backgrounds --stack /private/sdk-with-animation-fix --archive /private/opening.narc --output /private/sprites-new
```

Esse comando substitui o harness e recompila o binário do workspace privado
fornecido. O relatório e as capturas anteriores dos fundos permanecem como
evidência histórica, mas o binário passa a executar sprites. O relatório novo
registra o hash do harness usado. O `--stack` deve conter as correções de NCLR e
NANR. Arquivos privados ficam vivos durante o diagnóstico; não é um teste de
descarte do gerenciador completo.

Limites: Linux/WSLg, áudio dummy, células iniciais estáticas, sem comparação
pixel a pixel com um DS. Ainda não integra `SpriteList`, reprodução temporal,
transformações SRT animadas, composição com BG/partículas ou fades. A issue de
integração completa dos sprites permanece pendente.

## Controlador de animações — 29/10/2026

A pesquisa GitHub por `NNS_G2dTickCellAnimation` confirmou o caminho de
`Sprite_UpdateAnim` no `sprite.c` original. A consulta por
`SequenceEdgeHandleCommon_` no libntrsystem não encontrou correção indexada.
O teste com as sequências reais reproduziu uma leitura fora do buffer nessa
função: o ramo SDK_PORT acessava `pCurrent->frames` depois de avançar o ponteiro
para além do último frame, antes da normalização do índice.

`prepare_sdk_animation_controller.py` remove essa leitura adicional do caminho
nativo, deixando a lógica comum de parada/reinício e normalização de limites
tratar o fim da sequência. Verifica o hash do fonte antes de adaptar a cópia
privada. O build SDK aplica a adaptação; a recompilação completa passou.

`verify_sdk_animation_playback.py` compila o controlador completo com o loader
NANR corrigido e a multiplicação de ponto fixo original do SDK. Primeiro exige
o estouro de buffer com ASan no controlador original; em seguida confere cada
índice, elemento e estado ativo na versão adaptada contra uma linha do tempo
calculada pelas durações dos frames. Passaram seis sequências e 7.224 ticks,
com ASan/UBSan. As duas mãos usam loop de 1.076 ticks; brilho termina após 24;
cada inicial termina após 76. O teste atravessa três durações completas mais
dois ticks para cada sequência. Não cobre reprodução reversa, callbacks,
velocidade fracionária ou sequências com duração zero.

Relatório: [hgss-animation-controller-test.json](hgss-animation-controller-test.json).

```sh
python3 tools/verify_sdk_animation_playback.py --stack /private/sdk-with-nanr-fix-before-controller-fix --archive /private/opening.narc --output /private/controller-new
```

O modo `verify_scene_sprites.py --playback` executa 160 frames por processo,
avança o controlador original e aplica os deslocamentos INDEX_T de cada frame
antes da conversão OAM. Os seis casos tiveram capturas distintas e pixels
visíveis, enquanto os seis controles sem desenho permaneceram pretos. Foram
capturados os frames 2/4/6/8/10 e depois a cada 20 até 160, para não perder o
brilho curto. As capturas iniciais foram inspecionadas visualmente. Relatório:
[hgss-scene-playback-test.json](hgss-scene-playback-test.json).

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-backgrounds --stack /private/sdk-with-controller-fix --archive /private/opening.narc --output /private/playback-new --playback
```

O teste visual confirma mudança e visibilidade, sem comparar cada pixel com um
DS. A igualdade entre motores foi validada no modo estático anterior; durante
playback, as capturas de cada motor ocorrem em instantes distintos e não são
comparadas entre si. O loop completo das mãos é coberto pelo teste do controlador,
pois sua duração excede os 160 frames do teste gráfico. Ainda faltam SpriteList,
composição com fundos/partículas, transformações SRT completas e fades.

## Composição estática BG1/OBJ — 29/10/2026

A pesquisa por `IntroMovie_Scene4_EnableBgLayers` confirmou o controle das
camadas na cena original. O modo `--compose` de `verify_scene_sprites.py` testa
BG1 e OBJ simultâneos: BG principal usa banco B, BG secundário C, OBJ principal
A e OBJ secundário D. BG1 mantém charbase 0x4000, screenbase 0 e prioridade 1;
os sprites mantêm os atributos OAM dos recursos. Carrega paleta 53, tiles 54 e
mapas 58/57, além dos recursos de cada sprite.

Os 12 casos passaram (seis sprites × dois motores). Sem sprites, cada captura
é idêntica à decodificação independente do fundo. Com sprites, todos os pixels
coloridos da referência estática isolada permanecem iguais; os demais pixels
devem ser fundo ou preto. Essa última regra não distingue preto opaco de
transparência na referência de fundo preto, portanto não comprova a máscara
completa de transparência. Capturas principais inspecionadas visualmente.
Relatório: [hgss-scene-composition-test.json](hgss-scene-composition-test.json).

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-backgrounds --stack /private/sdk-with-controller-fix --archive /private/opening.narc --output /private/composition-new --compose --sprite-reference /private/passed-static-sprites
```

O modo exclui `--playback` e reutiliza uma referência estática aprovada. As
posições diagnósticas provocam sobreposição deliberada dos iniciais com o
retrato para conferir a composição; não representam a disposição final da
abertura. Não testa inversão de prioridade, SpriteList, múltiplos sprites
simultâneos, BG2/BG3 compostos, partículas ou fades. A issue #8 segue aberta.

## Armazenamento do gerenciador original — 29/10/2026

A busca GitHub por `BUGFIX_SPRITE_ANIM_SIZE` encontrou a correção opcional no
[sprite.h da base](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/include/sprite.h).
O mesmo padrão foi encontrado em `pret/pokeheartgold`, `ctrl-cheeb-del/pokeheartgold`
e `PikalaxALT/pokeheartgold-speedchoice`. A consulta por
`SPRITE_ANIMATION_DATA_WORD_COUNT` em pspoke não retornou resultado indexado.
O comentário da base já identifica que o bloco fixo é pequeno; a opção existente
dimensiona por `sizeof`, mas não explicita o alinhamento nativo e as três variantes.

O teste com os tipos reais HGSS/SDK em Linux x86_64 mediu 116 bytes reservados
contra 136 para animação simples, 144 com transferência e 200 para multicélulas.
`prepare_hgss_sprite_storage.py` adapta somente o ramo SDK_PORT do header privado:
uma união das três estruturas garante capacidade e alinhamento, preservando o
acesso existente pelo nome `animationData`. O ramo DS permanece como estava.
O módulo de preparação `probe_hgss_modules.py` passa a usar o header adaptado.
Todos os módulos que compartilham Sprite devem ser recompilados com o mesmo
header; objetos anteriores não são compatíveis com esse layout.

`verify_hgss_sprite_storage.py` reproduziu a insuficiência original, validou
capacidade/alinhamento e escrita dos três tamanhos sem alterar bytes adjacentes,
com ASan/UBSan, e compilou a unidade original completa `sprite.c` com o header
adaptado. O teste não executa criação, renderização ou descarte de SpriteList.
Resultado: [hgss-sprite-storage-test.json](hgss-sprite-storage-test.json).

```sh
python3 tools/verify_hgss_sprite_storage.py --hgss /private/hgss --probe /private/compatible-hgss-module-probe --stack /private/sdk --output /private/sprite-storage-new
```

Essa correção remove um bloqueio de layout para a integração do gerenciador;
a ligação e execução completa de SpriteList permanecem pendentes na issue #8.

## Ciclo de vida do gerenciador original (29/10/2026)

Após consultar `SpriteList_Create` no GitHub da base ClickingMaster, o teste
reutiliza `src/sprite.c` integral e controladores/carregadores nativos do SDK.
Não há substituição das funções de SpriteList. Somente heap, leitura de arquivo
e falhas de assert têm adaptadores explícitos no executável de teste.

`verify_hgss_sprite_manager.py` passou em Linux x86_64: 64 listas criadas e
liberadas, 448 sprites criados e 61.440 ticks. Verifica lista duplamente ligada,
ordem por prioridade, recusa acima da capacidade, reutilização do slot excluído,
preservação dos proxies e retorno ao número inicial de alocações.
Usa NCER 88/NANR 87 da abertura e o NANR padrão da base; fontes e dados ficam privados.

ASan/UBSan instrumentam as unidades compiladas. A instrumentação ASan de globais
fica desativada para permitir descartar a tabela de callbacks de renderização
não usada; heap e pilha continuam instrumentados. O arquivo precompilado de
fallback matemático não é instrumentado. O renderer é zerado apenas para os
campos de configuração acessados: não é inicializado nem usado para desenhar.

Cobertura: variante CELL, animação inteira e ciclo de vida. Não cobre draw,
transferência de células, multicell, cena completa, heap original ou Windows.
Isso não constitui boot ou jogo jogável.
[Resultado](hgss-sprite-manager-test.json).

```sh
python3 tools/verify_hgss_sprite_manager.py --hgss /private/hgss --probe /private/compatible-hgss-module-probe --stack /private/sdk --archive /private/a/2/6/2 --output /private/sprite-manager-new
```

## Renderização estática pelo SpriteList original (29/10/2026)

Consulta prévia no GitHub confirmou `GF_InitG2dRenderer` e
`GF_InitG2dRenderSurface` na base ClickingMaster. O novo modo `--manager` usa
essas funções e `src/sprite.c` originais, o header nativo adaptado e as unidades
reais de renderização do SDK. Adaptadores de heap/arquivo e registro de OAM
permanecem explícitos; o gerenciador OAM original do jogo não é usado.

Seis casos, cada um com desenho ligado/desligado, passaram no Linux/WSLg.
Capturas de ambos os motores coincidem pixel a pixel com a conversão direta
cell/OAM do SDK, agora incluindo a translação inicial presente no NANR.
As três mãos aparecem (6.464, 6.864 e 228 pixels); brilho e iniciais começam
fora da tela nesta posição diagnóstica. Esses três casos verificam emissão
OAM e igualdade de tela vazia, não comprovam sua aparência. Controles sem
spritem são pretos. Uma captura do gerenciador foi inspecionada visualmente.

A referência anterior ignorava a translação inicial: o primeiro confronto
falhou por posição, não por pixels faltantes. `--initial-transform` fornece
uma referência explícita com a translação para esse teste, preservando os
modos antigos. Ambos os motores passam pela inicialização de superfície e
renderização original antes do upload dos OAMs capturados.

[Resultado](hgss-sprite-manager-render-test.json). Sem sanitizadores neste
executável gráfico. Não cobre playback pelo SpriteList, affine, multicell,
transferência, composição completa da abertura, Windows ou boot do jogo.

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-bg --stack /private/sdk --archive /private/a/2/6/2 --output /private/initial-reference --initial-transform
python3 tools/verify_scene_sprites.py --workspace /private/validated-bg --stack /private/sdk --archive /private/a/2/6/2 --output /private/manager-render --manager --hgss /private/hgss --probe /private/compatible-hgss-module-probe --sprite-reference /private/initial-reference
```



## Playback pelo SpriteList original (29/10/2026)

Consulta ao GitHub da base confirmou que SpriteList_RenderAndAnimateSprites
primeiro desenha e depois atualiza a animação. O diagnóstico agora conserva
listas, renderizadores e superfícies durante 160 frames por processo, em ambos
os motores. Define velocidade 1 explicitamente (o padrão de Sprite_CreateAffine
é 2) e avança um tick antes do loop para igualar a referência, que atualiza antes
de desenhar. A referência passou a aplicar a translação NANR também na imagem
inicial. Nenhuma dessas normalizações altera a engine original.

Seis casos com desenho ligado/desligado, 13 capturas por processo (frames
2/4/6/8/10 e 20–160 de 20 em 20): **156 capturas comparadas pixel a pixel,
sem divergências**. Todos os seis casos têm pixels visíveis e múltiplas imagens
distintas; os controles sem desenho são pretos. A seleção single screen alterna
os motores nos frames 40 e 80. Não se exige igualdade entre motores capturados
em tempos diferentes; cada captura é comparada ao mesmo frame da referência.

[Resultado](hgss-sprite-manager-playback-test.json). Este teste gráfico não usa
sanitizadores. Ainda usa heap/IO e registro OAM de diagnóstico; não valida o
gerenciador OAM/transferências original, affine/multicell, composição completa,
Windows ou boot. A finalização é limitada pela captura que encerra o processo;
a prova de liberação permanece no teste separado de ciclo de vida.

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-bg --stack /private/sdk --archive /private/a/2/6/2 --output /private/playback-reference --playback --initial-transform
python3 tools/verify_scene_sprites.py --workspace /private/validated-bg --stack /private/sdk --archive /private/a/2/6/2 --output /private/manager-playback --manager --playback --hgss /private/hgss --probe /private/compatible-hgss-module-probe --sprite-reference /private/playback-reference
```


## Composição animada BG1 + SpriteList (29/10/2026)

O modo `--animated-compose --manager --playback` reutiliza o gerenciador original
já validado e o BG1 da cena 4. Seis casos de 160 frames, com desenho ligado e
sem desenho, passaram em Linux/WSLg. As 78 capturas sem sprites coincidem
integralmente com um dos dois mapas calculados pelo comparador CPU; ambos os
motores são identificados em cada caso. Nas 78 capturas compostas, 170.756
ocorrências de pixels coloridos dos sprites coincidem com o playback isolado
no mesmo frame. Uma captura foi inspecionada visualmente.

[Resultado](hgss-sprite-manager-composition-test.json). O comparador exige
igualdade dos pixels coloridos e aceita fundo ou preto no restante. Portanto,
não distingue preto opaco de transparência e não comprova a máscara alfa
completa. Usa OBJ à frente de BG1; inversão de prioridades não foi testada.
Posições são diagnósticas, não a composição final da máquina de estados.
Continua sem gerenciador OAM/transferências original, fades, boot ou teste
Windows. Sem sanitizadores neste executável gráfico.

Antes da adaptação, consultados `SpriteList_RenderAndAnimateSprites` e
`OamManager_ApplyAndResetBuffers` na base ClickingMaster no GitHub. O segundo
já existe e será reutilizado na próxima integração, substituindo o registro
OAM do diagnóstico.

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-bg --stack /private/sdk --archive /private/a/2/6/2 --output /private/manager-composition --manager --playback --animated-compose --hgss /private/hgss --probe /private/compatible-hgss-module-probe --sprite-reference /private/validated-manager-playback
```


## Gerenciador OAM original (29/10/2026)

Após consultar OamManager_Create e OamManager_ApplyAndResetBuffers no GitHub
da base, o modo `--original-oam` liga `oam_manager.c`, `g2d_cell_culling.c`
e o módulo `g2d_OAM.c` real do SDK. Reutiliza a adaptação existente que substitui
a estrutura opaca OamInstance pela declaração nativa do SDK; a lógica original
é preservada. As tabelas de tamanho de objetos passam a ser fornecidas pelo
módulo OAM, evitando sua definição duplicada no diagnóstico.

Criação, callbacks de registro, seleção de células visíveis, envio e reset dos
buffers agora seguem o gerenciador original. São reservados OAMs 4–127 no motor
principal e 0–127 no secundário, com faixas de matrizes 1–31 e 0–31. O diagnóstico
não utiliza transformações affine nestes casos.

**Seis casos por 160 frames, desenho ligado/desligado: 156 capturas idênticas**
à referência anterior com registro OAM de diagnóstico. Todos os casos ficam
visíveis e mudam durante a reprodução; controles sem desenho são pretos.
Ambos os motores são selecionados em single screen. [Resultado](hgss-original-oam-test.json).

Limites: Linux/WSLg, sem sanitizadores no executável gráfico; heap/IO ainda
host. Não valida esgotamento de OAM, affine, descarte/recriação do gerenciador
OAM, todas as geometrias de culling ou composição de fundos neste modo. Faltam
transferências originais de recursos, fades, máquina de estados da abertura,
Windows e boot do jogo completo.

```sh
python3 tools/verify_scene_sprites.py --workspace /private/validated-bg --stack /private/sdk --archive /private/a/2/6/2 --output /private/original-oam --manager --playback --original-oam --hgss /private/hgss --probe /private/compatible-hgss-module-probe --sprite-reference /private/validated-manager-playback
```
