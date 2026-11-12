# Diário de trabalho: Johto e Hoenn no Super FireRed

Registro cronológico de tudo o que foi feito na branch `codex/firered-johto-hoenn`:
o que mudou, por quê, como foi verificado e o que ficou pendente. O plano e os
marcos ficam em [MULTIREGION_ROADMAP.md](MULTIREGION_ROADMAP.md); este arquivo é o
histórico passo a passo. Cada entrada nova vai no fim.

## 07/11/2026 — Retomada da branch (Claude)

### Estado encontrado

- Branch `codex/firered-johto-hoenn`, 11 commits à frente de `main` (último: `9dd3784`,
  animações das flores do piloto Hoenn). Marcos M0 concluído; M1 e M2 iniciados.
- O Codex foi pausado pelo usuário antes desta retomada.
- Trabalho do Codex não commitado (17:21), primeira instalação do piloto Hoenn na engine:
  - Registro dos tilesets `gTileset_HoennPilotGeneral` (primário) e
    `gTileset_HoennPilotPetalburg` (secundário) em `src/data/tilesets/headers.h`,
    `graphics.h` e `metatiles.h`, com os arquivos em `data/tilesets/{primary,secondary}/hoenn_pilot/`.
  - Animação das flores (`src/data/tilesets/hoenn_pilot_anims.h`, incluída em `tileset_anims.c`).
  - Tabela de portas do par Hoenn (`src/data/hoenn_pilot_doors.h`, cadastrada em
    `src/data/door_tilesets.h`) e as imagens em `graphics/door_anims/hoenn/`.
  - Mapa `data/maps/Hoenn_LittlerootTown/` e layout `data/layouts/Hoenn_LittlerootTown/`,
    ainda sem registro em `layouts.json`/`map_groups.json`, com conexão para um
    `MAP_HOENN_ROUTE101` inexistente. Isso não compilaria a ROM.

### Divergência encontrada e corrigida

O `map.bin` de Littleroot instalado diferia do resultado auditado
(`.build/region-pilots/hoenn/LittlerootTown/map.bin`, SHA-1 `9f194a2a…`) em 3 células:
as portas (metatiles 0x2C8/0x2C9) tinham colisão 3 em vez de 1. Ambos os valores
bloqueiam a passagem no FireRed, mas a mudança não constava de nenhum relatório.
Restaurado o arquivo auditado, para manter a cadeia de origem verificável.

### Imagens em branco substituídas

As cinco imagens pequenas instaladas pelo Codex estavam vazias: todos os pixels com
índice 1, uma cor só. São as portas `graphics/door_anims/hoenn/{littleroot,birchs_lab}.png`
e as flores `data/tilesets/primary/hoenn_pilot/anim/Flower_{0,1,2}.png`. Na ROM, portas
e flores apareceriam como blocos de cor sólida. Os `.4bpp` auditados em `.build/region-pilots/`
estavam corretos: conferidos pixel a pixel contra o nibble baixo dos PNGs do pokeemerald.

Novo `scripts/export_region_png.py`: escreve o `.4bpp` auditado de volta como PNG indexado
de 16 cores, que a build converte com gbagfx. Reusa o decodificador de
`prepare_hoenn_doors.py`. A paleta vem do PNG original do pokeemerald só para visualização;
o gbagfx usa os índices. Comandos usados (na raiz, Windows, Pillow 13):

```bash
E=../map-references/pokeemerald; S=.build/region-pilots; G=games/super-fire-red
for n in littleroot birchs_lab; do python scripts/export_region_png.py --bin $S/hoenn-doors/$n.4bpp --width 16 --palette-from $E/graphics/door_anims/$n.png --out $G/graphics/door_anims/hoenn/$n.png; done
for i in 0 1 2; do python scripts/export_region_png.py --bin $S/hoenn-animations/Flower_$i.4bpp --width 16 --palette-from $E/data/tilesets/primary/general/anim/flower/$i.png --out $G/data/tilesets/primary/hoenn_pilot/anim/Flower_$i.png; done
```

Teste novo `scripts/test_region_png.py` (2 casos): ida e volta preserva os 16 índices e a
ordem dos tiles; largura incompatível é recusada.

### Conferência dos demais arquivos instalados

Iguais byte a byte aos resultados auditados: `metatiles.bin`, `metatile_attributes.bin` e
`tiles.png` dos tilesets primário e secundário, e as 16 paletas de cada um. Bordas
de Littleroot e Route101 também.

### Route101 e registro dos dois exteriores na engine

- `data/layouts/Hoenn_Route101/{map.bin,border.bin}` copiados de
  `.build/region-pilots/hoenn/Route101/` (SHA-1 do `map.bin`: `a8fe5c97…`).
- `data/maps/Hoenn_Route101/map.json`: metadados do Emerald (rota, ensolarado, sem
  eventos). Música `MUS_ROUTE1`, pois `MUS_ROUTE101` não existe no FireRed; Littleroot já
  usava `MUS_PALLET` pelo mesmo motivo. `show_map_name` desligado nos dois: as seções
  `MAPSEC_LITTLEROOT_TOWN`/`MAPSEC_ROUTE_101` existem, mas sem nome no FireRed.
- Conexões: Littleroot ↔ Route101 (norte/sul, deslocamento 0). A conexão
  Route101 → Oldale ficou de fora porque Oldale não foi importada; segue listada como
  fronteira pendente no plano.
- `layouts.json`: `LAYOUT_HOENN_LITTLEROOT_TOWN` e `LAYOUT_HOENN_ROUTE101` acrescentados
  no fim (IDs 0x180 e 0x181). Nenhum layout existente mudou de número.
- `map_groups.json`: grupo novo `gMapGroup_Hoenn` no fim, ou seja, grupo 43 como no
  plano. Littleroot = mapa 0, Route101 = mapa 1. **Mudança em relação ao plano:** ele
  previa Route101 como mapa 6, depois dos 5 interiores. Os interiores ainda não têm
  tilesets convertidos e não podem ser registrados; entrarão como mapas 2 em diante.
  Os dois mapas também foram acrescentados a `connections_include_order`.
- `data/event_scripts.s`: inclusão dos dois `scripts.inc` (vazios: só terreno; eventos
  do Emerald não foram importados).

### Teste na ROM, em build isolada

Novo `games/super-fire-red/scripts/test_hoenn_pilot_maps.py`. Cria uma build a partir dos
fontes em diretório temporário (`~/.cache/firered-386/hoenn-pilot-*`) e **nunca** grava
`output/firered-386-dev.gba`. Isso segue a política da branch: só builds locais de
validação, isoladas. O teste entra pelo carregador real do New Game em três casos:

1. Littleroot: grupo 43, mapa 0, layout 0x180, tilesets Hoenn; conexão norte para a
   Route101; a porta em (5,8) é o metatile 0x2C8 com comportamento de porta e colisão;
   o tile 508 da VRAM muda entre quadros (animação das flores).
2. Route101: grupo 43, mapa 1; conexão sul para Littleroot; animação das flores.
3. Pallet Town (controle): continua nos tilesets do FireRed.

Também confere que os `.4bpp` gerados pela build a partir dos PNGs são iguais aos
auditados, e que os `map.bin`/`border.bin` instalados são iguais aos do staging.
Capturas de tela em `output/tests/hoenn-pilot-maps/` (fora do Git): Littleroot mostra as
duas casas, caixas de correio, flores e o telhado do laboratório; Route101 mostra árvores,
o desnível, a grama alta e a placa.

Problemas do próprio teste, resolvidos durante o trabalho:

- O New Game travava em todos os mapas, inclusive Pallet: `NewGameInitData` copia o nome
  do rival, e num save em branco o nome não tem terminador (EOS = 0xFF; a RAM vem zerada),
  então a cópia invade a memória. Os outros testes de ROM definem os nomes antes; este
  agora também. Não afeta o jogo normal, que sempre passa pela tela de nomes.
- A posição salva está em coordenadas do mapa, sem o deslocamento `MAP_OFFSET` (7).

Reprodução (WSL/Linux, na raiz): `python3 games/super-fire-red/scripts/test_hoenn_pilot_maps.py`.

### Outras verificações

- 25 testes de conversão de regiões (`python -m unittest discover -s scripts -p "test_*region*.py"`): OK.
- `scripts/test_field_door_native.py` (portas, ASan/UBSan): OK.
- ROM publicada intacta: `output/firered-386-dev.gba` continua com o SHA-1 da
  v0.5.0-preview.6 (`78db88d4…`).

### Ainda não verificado / pendente

- Andar de um mapa para o outro pela conexão, em jogo. O teste confere a conexão nos
  dados carregados, mas não atravessa a fronteira.
- Abrir as portas: não há warps, porque os interiores não foram importados. (Resolvido na entrada seguinte.)
- Encontros na grama alta da Route101: não há tabela em `wild_encounters.json`, então
  hoje não aparecem Pokémon.
- Mapa regional, Fly e nome do local para as seções Hoenn (marco M5).
- Build nativa para PC com os mapas novos.
- O piloto de Johto (New Bark/Route29) ainda não foi instalado na engine.

## 07/11/2026 — Interiores de Littleroot (Claude)

Pedido do usuário: seguir com os interiores Hoenn. Os cinco mapas do plano foram
convertidos, instalados e ligados às portas de Littleroot: casa do Brendan 1F/2F,
casa da May 1F/2F e laboratório do Prof. Birch.

### Tilesets: conversão com empacotamento

Novo `scripts/convert_hoenn_interiors.py`, que reusa as funções de conversão do Codex
(`convert_hoenn_graphics.py`, `region_tilesets.py`, `normalize_region_layers.py`).
Converte três tilesets do Emerald:

| Emerald | FireRed | Situação |
|---|---|---|
| `gTileset_Building` (primário) | `gTileset_HoennPilotBuilding` | 2 metatiles usados; tiles com o mesmo número (512 cabem em 640) |
| `gTileset_BrendansMaysHouse` (secundário) | `gTileset_HoennPilotBrendansMaysHouse` | 149 metatiles; 480 tiles na fonte, **211 empacotados** |
| `gTileset_Lab` (secundário) | `gTileset_HoennPilotLab` | 67 metatiles; 512 tiles na fonte, **138 empacotados** |

**Por que empacotar:** o FireRed tem 640 tiles primários e só 384 livres para o secundário.
O deslocamento simples usado nos exteriores (512 → 640) jogaria 49 tiles da casa e 84 do
laboratório para além do tile 1023, fora da VRAM. O conversor guarda só os tiles que os
metatiles usados referenciam, na ordem original, e renumera as referências.
Paletas secundárias 6–11 → 7–12, como nos exteriores. Foram comparados 111.616 pixels
(cor e transparência, com espelhamentos) contra as folhas originais: nenhuma diferença.
Sem animações: o callback do Building anima a TV nos tiles 496–499 (conferido em
`pokeemerald/src/tileset_anims.c`), e os metatiles usados só referenciam os tiles 0–36.
Relatório com hashes: [hoenn-interior-graphics.json](map-audits/hoenn-interior-graphics.json).

### Comportamentos sem equivalente de mesmo nome

| Emerald | FireRed | Motivo |
|---|---|---|
| `MB_TELEVISION`, `MB_REGION_MAP`, `MB_SOUTH_ARROW_WARP` | mesmo nome (0x86, 0x85, 0x65) | — |
| `MB_NON_ANIMATED_DOOR` (escadas das casas) | `MB_CAVE_DOOR` (0x60) | é o valor que `MetatileBehavior_IsNonAnimDoor` do FireRed aceita: warp ao pisar, sem animação, como no Emerald |
| `MB_IMPASSABLE_SOUTH_AND_NORTH` | **novo** `0xD2` | célula do quarto (1,4) no 2F: colisão 0, só a regra impede norte/sul. Sem ela o jogador atravessaria |
| `MB_HOLDS_SMALL/LARGE_DECORATION` | `MB_NORMAL` | espaços de decoração das bases secretas, que não existem no FireRed; a passagem fica por conta da colisão |

Mudança na engine (única desta etapa): `MB_IMPASSABLE_SOUTH_AND_NORTH 0xD2` em
`include/constants/metatile_behaviors.h` e incluído em `MetatileBehavior_IsNorthBlocked`
e `MetatileBehavior_IsSouthBlocked` (`src/metatile_behavior.c`), exatamente como no Emerald.
O FireRed define comportamentos até 0xD1 e reserva até 0xF0. Conferido: nenhum dos 73
tilesets do FireRed usa 0xD2, então Kanto não muda.

A TV e o mapa na parede respondem com os textos/telas do FireRed (o mapa mostra Kanto).

### Instalação e warps

Novo `scripts/install_hoenn_interiors.py` (idempotente; rodado duas vezes sem duplicar):

- copia os tilesets de `.build/region-pilots/hoenn-interior-graphics/` e os blocos de
  `.build/region-pilots/hoenn-interiors/<mapa>/`;
- acrescenta os símbolos C em `graphics.h`, `metatiles.h` e `headers.h`;
- cria os cinco layouts e mapas (grupo 43, mapas 2 a 6, nesta ordem: Brendan 1F, Brendan 2F,
  May 1F, May 2F, laboratório) com cabeçalhos do Emerald. Música: `MUS_LITTLEROOT` →
  `MUS_PALLET` e `MUS_BIRCH_LAB` → `MUS_OAK_LAB`, que não existem no FireRed;
- copia os warps do Emerald trocando o prefixo do destino (`MAP_LITTLEROOT_TOWN…` →
  `MAP_HOENN_LITTLEROOT_TOWN…`); recusa qualquer warp que saia do piloto. Os índices de
  warp ficam iguais aos do Emerald;
- acrescenta os três warps de porta de Littleroot.

Só terreno e warps: personagens, scripts da história e eventos de coordenada do
Emerald não foram importados.

### Teste na ROM: andar pelos warps

`test_hoenn_pilot_maps.py` foi ampliado de 3 para 10 casos, com um runner novo,
`games/super-fire-red/scripts/hoenn_pilot_smoke.c`, em que o próprio jogo escolhe
as teclas e pede capturas. Os casos novos seguram o direcional como um jogador:

| Caso | Percurso | Resultado |
|---|---|---|
| 4 | Littleroot (5,9) ↑ porta do Brendan → capacho ↓ | Brendan 1F (8,8) → Littleroot (5,9) |
| 5 | Littleroot (14,9) ↑ porta da May → capacho ↓ | May 1F (2,8) → Littleroot (14,9) |
| 6 | Littleroot (7,17) ↑ laboratório → capacho ↓ | Lab (6,12) → Littleroot (7,17) |
| 7 | Brendan 1F (8,3) ↑ escada → ↓ um passo → ↑ escada | 2F (7,2) → (7,3) → 1F (8,3) |
| 8 | May 1F (2,3) ↑ escada → ↓ → ↑ | 2F (1,2) → (1,3) → 1F (2,3) |
| 9 | Brendan 2F (1,3) segurando ↓ | continua em (1,3): a célula (1,4) bloqueia entrada pelo norte |
| 10 | Brendan 2F (2,4) ← → ↑ → ↓ → → | entra em (1,4) pelo lado; ↑ e ↓ não saem; → volta a (2,4) |

Ao sair por uma porta ou pela escada, o FireRed põe o jogador um passo fora da célula
de chegada; o teste exige as posições exatas acima. Também confere que os tilesets e
blocos instalados são iguais ao staging. Capturas em `output/tests/hoenn-pilot-maps/`:
sala com mesa e cozinha, quarto com cama, PC e console, laboratório com máquinas e
plantas, quarto da May; todos desenhados corretamente.

### Outras verificações

- 27 testes de regiões, incluindo o novo `scripts/test_region_interiors.py`
  (empacotamento preserva os tiles escolhidos e a ordem; regras usam valores válidos do FireRed): OK.
- `scripts/test_field_door_native.py`: OK.
- ROM publicada intacta: `output/firered-386-dev.gba` com o SHA-1 da v0.5.0-preview.6.

### Pendente

- Atravessar a conexão Littleroot ↔ Route101 andando (só conferida nos dados).
- Encontros na Route101; mapa regional, Fly e nomes das seções Hoenn (M5).
- Personagens e eventos dos interiores (mãe, assistente, Birch); TV e mapa usam os textos do FireRed.
- Build nativa para PC com os mapas novos.
- Piloto de Johto (New Bark/Route29).

## 07/11/2026 — Piloto de Johto (Claude)

Pedido do usuário: piloto de Johto. Instalados New Bark Town, Route 29 e seis interiores
(casa do jogador 1F/2F, duas casas, laboratório do Prof. Elm e o portão Route 29/46),
como grupo 44 (`gMapGroup_Johto`, mapas 0 a 7 nesta ordem: New Bark, Route 29, casa 1F,
casa 2F, casa 1, casa 2, laboratório, portão). O plano previa grupo 43, já usado por Hoenn.

### Licença: decisão do usuário

Os tilesets de Johto no HNS_modern são arte de fãs (Kertra/Crystal Advance, Ekat99,
TheDeadHeroAlistair, Johto Redrawn Team), e o repositório de origem não tem licença
(GitHub: `null`). Este repositório é **público**. Perguntei antes de importar; o usuário
escolheu **importar com créditos**. Créditos completos e situação de licença em
[CREDITS.md](CREDITS.md), com link no README. Os arquivos ficam isolados para remoção
fácil se algum autor pedir.

### Por que não houve conversão de gráficos

O HNS_modern já usa a partição do FireRed (640 tiles e metatiles primários, 7 paletas
primárias, 1024 no total). Tiles, paletas, metatiles e blocos são copiados sem mudança; o
teste confere byte a byte contra a fonte. Só os atributos (16 bits, formato Emerald) são
convertidos para os 32 bits do FireRed, com regra explícita por comportamento; metatiles
que nenhum mapa ou borda instalada usa ficam zerados. Secundários do HNS têm 13 paletas
(00–12), e só essas são declaradas.

### Instalação

Novo `scripts/install_johto_pilot.py` (idempotente; segunda execução sem duplicar).
Relatório com hashes, comportamentos e warps descartados:
[johto-pilot-install.json](map-audits/johto-pilot-install.json).

Atributos: cada comportamento recebe os bits extras (terreno e tipo de encontro) que os
próprios tilesets do FireRed mais usam para ele. Exemplos: água `0x02000415` (terreno
água, encontros na água), grama alta `0x01000202`.

| HNS | FireRed | Motivo |
|---|---|---|
| areia, água, desníveis, pedra-escada, balcão, estante, PC, TV, bloqueios, capacho, escadas do FireRed | mesmo nome | — |
| `MB_ANIMATED_DOOR` | `MB_WARP_DOOR` (0x69) | portas animadas, tabela registrada para o par New Bark |
| `MB_NON_ANIMATED_DOOR` | `MB_CAVE_DOOR` (0x60) | entrada do portão; como nos interiores Hoenn |
| `MB_SHORT_GRASS` | normal | no HNS é só efeito visual ao andar; o FireRed não tem |
| `MB_HEADBUTT_TREE` | normal | o FireRed não tem Headbutt; a árvore mantém a colisão |
| decoração, `MB_SECRET_BASE_SPOT_TREE_RIGHT` | normal | bases secretas não existem no FireRed |
| `MB_UNUSED_81`, `MB_SLOT_MACHINE` | normal | nenhum código do HNS usa esses valores |
| `MB_SIDEWAYS_STAIRS_LEFT_SIDE_TOP` | normal | o FireRed não tem escadas laterais; anda em linha reta |

Animação: o callback `TilesetAnim_JohtoGeneral` do HNS executa quatro animações; só duas
tocam tiles usados pelo piloto e foram reproduzidas com o mesmo agendamento: borda
areia/água (18 tiles a partir do 416, 8 quadros) e flores (tiles 508–511, 5 quadros).
A cachoeira não é usada; a borda terra/água está desligada no próprio HNS (`return;`).

Portas: azul (0x2CB, que no HNS reaproveita `cerulean.png`, paleta 7) e amarela
(0x309, paleta 5), formato 1x1 igual nos dois motores; cadastradas para o par
`gTileset_JohtoPilotGeneral`/`gTileset_JohtoPilotNewBarkTown` em `src/data/door_tilesets.h`.

Mapas: cabeçalhos do HNS com músicas trocadas (`MUS_HG_NEW_BARK` → `MUS_PALLET`,
`MUS_HG_ROUTE29` → `MUS_ROUTE1`, `MUS_HG_ELM_LAB` → `MUS_OAK_LAB`); o laboratório vinha
como `MAP_TYPE_NONE` e virou `MAP_TYPE_INDOOR`. Seção de mapa: `MAPSEC_NONE`, porque o
FireRed não tem seções de Johto; seções depois de Kanto cairiam na faixa de Sevii ou
mudariam IDs salvos. O jogo trata `MAPSEC_NONE` como "sem nome", igual às seções Hoenn.

Warps e conexões que saem do piloto foram descartados, e os índices restantes
renumerados em todos os mapas que apontam para eles (o warp 6 de New Bark virou 4):

- New Bark: dois warps para `WORLD_HUB` e um para `TIN_TOWER_ROOF_DAY` (áreas do HNS);
- portão: o warp para a Route 46;
- conexões: New Bark → Route 27; Route 29 → Cherrygrove e Route 46.

Mantido por fidelidade: o warp de New Bark (12,7) fica sobre um tile normal com colisão;
no HNS ele só serve de ponto de chegada da saída dos fundos do laboratório. O 2F da casa
tem um warp (1,6) que leva ao próprio 2F, também vindo do HNS.

### Teste renomeado e ampliado

`test_hoenn_pilot_maps.py` virou `test_region_pilot_maps.py` e `hoenn_pilot_smoke.c`
virou `region_pilot_smoke.c`, pois cobrem as duas regiões. 21 casos, todos aprovados:

| Caso | O que confere | Resultado |
|---|---|---|
| 1–3 | Littleroot, Route 101 e Pallet (controle), como antes | OK |
| 4–10 | portas, capachos, escadas e célula do quarto em Hoenn, como antes | OK |
| 11 | **andar** de Littleroot (10,0) para o norte e voltar | Route 101 (10,19) → Littleroot (10,0) |
| 12 | dados de New Bark: grupo 44/mapa 0, tilesets Johto, conexão oeste para a Route 29, porta azul 0x2CB, animação | OK |
| 13 | dados da Route 29: conexão leste; a entrada do portão é warp ao pisar | OK |
| 14–17 | as quatro portas de New Bark e volta pelo capacho | laboratório (6,12), casa (9,8), casas 1 e 2 (4,8) → de volta à frente de cada porta |
| 18–19 | escadas da casa do jogador (estilo FireRed: sobe andando para a direita, desce para a esquerda) | 2F (9,2) e 1F (10,2) |
| 20 | portão da Route 46 e volta | portão (7,9) → Route 29 (34,11) |
| 21 | **andar** de New Bark (0,12) para o oeste e voltar | Route 29 (69,17) → New Bark (0,12) |

O teste também confere que os arquivos Johto instalados são idênticos aos do HNS_modern
(tiles, metatiles, paletas, blocos, quadros da animação e portas). Capturas em
`output/tests/region-pilot-maps/`: New Bark com o laboratório, flores e cercas; Route 29;
laboratório com estantes; quarto com escada; portão. Todos desenhados corretamente.
Com isso, a travessia Littleroot ↔ Route 101, pendente desde a primeira entrada, foi testada.

### Outras verificações

- 27 testes de regiões: OK. `scripts/test_field_door_native.py`: OK.
- ROM publicada intacta (SHA-1 da v0.5.0-preview.6).

### Pendente

- Seções de mapa, nomes, mapa regional e Fly de Johto e Hoenn (M5).
- Encontros selvagens (Route 29 e Route 101 não têm tabelas).
- Personagens e eventos (Elm, mãe, rival) não importados.
- Fronteiras abertas: Route 27, Cherrygrove, Route 46, Oldale.
- Build nativa para PC com os mapas novos.

## 07/11/2026 — Encontros selvagens dos pilotos (Claude)

Pedido do usuário: encontros selvagens. A Route 101, a Route 29 e a água de New Bark agora
têm tabelas; Littleroot não tem (como no Emerald).

### Por que não editar `wild_encounters.json` direto

O arquivo é gerado por `balance_encounters.py`, que parte das tabelas originais do FireRed e
distribui as 386 espécies nos slots existentes. Uma edição manual quebraria o `--check` ou
sumiria na próxima geração. As tabelas dos pilotos agora vêm de um arquivo de dados próprio e
são **acrescentadas depois** da distribuição, fora dela: nenhum slot de Kanto ou Sevii mudou
(o diff de `wild_encounters.json` só acrescenta linhas), e `encounter_distribution.json`
continua idêntico.

- Novo `scripts/import_region_encounters.py`: lê as fontes fixadas e grava
  `games/super-fire-red/hack-data/region_pilot_encounters.json`, com repositório, commit,
  rótulo de origem e motivo de cada escolha. O balanceador lê esse arquivo, então o
  `--check` não depende dos checkouts externos.
- `balance_encounters.py`: acrescenta as linhas no fim e confere mapa existente, espécies
  entre as 386 e número de slots igual ao do FireRed.
- Rótulos terminam em `_Pilot` (sem "FireRed"/"LeafGreen"): o template compila nas duas
  versões, e os guias do jogador, auditorias e o planejador, que só leem linhas `*_FireRed`,
  deixam os pilotos de fora enquanto eles não são alcançáveis no jogo normal.
  `generate_guides.py --check` e `balance_encounters.py --check`: OK, guias inalterados.

| Mapa | Fonte | Tabelas | Observação |
|---|---|---|---|
| Route 101 | pokeemerald `gRoute101` | grama (taxa 20): Wurmple, Poochyena, Zigzagoon, nv. 2–3 | igual ao Emerald |
| Route 29 | HNS_modern `gRoute29` | grama (taxa 20): Pidgey, Sentret, Hoppip, Rattata, Hoothoot, nv. 2–3 | tabela do dia; o FireRed não tem dia/noite |
| New Bark | HNS_modern `gNewBarkTown` | água (taxa 7): Tentacool 25–29; pesca (taxa 30): Magikarp, Tentacool, Chinchou, Shellder, Tentacruel, Lanturn | o HNS tem 12 slots de água, todos iguais; o FireRed tem 5 |

Ficaram de fora: Rock Smash da Route 29 e de New Bark (as pedras são objetos de evento, não
importados) e a tabela de grama vazia de New Bark. Os níveis são os das fontes; o ritmo de
progressão entre regiões fica para o marco M6.

**Limitação no Hardcore:** a regra "só o primeiro encontro de cada área" usa a seção de mapa.
A Route 101 tem a sua (`MAPSEC_ROUTE_101`), mas a Route 29 e a água de New Bark usam
`MAPSEC_NONE` e contariam como uma área só. Resolve-se com as seções de Johto no M5.

### Teste na ROM

`test_region_pilot_maps.py` passou de 21 para 23 casos:

- os casos de dados agora conferem, em `gWildMonHeaders`, quais tabelas estão ligadas a cada
  mapa, a primeira espécie e a taxa de cada uma: Littleroot sem tabela; Route 101 e Route 29
  com grama; New Bark com água e pesca;
- casos 22 e 23: o jogador anda para lá e para cá na grama alta até aparecer um Pokémon e o
  teste confere espécie e nível contra a tabela. Resultado nesta execução: **Wurmple nv. 2**
  na Route 101 e **Pidgey nv. 3** na Route 29;
- para os outros casos não serem interrompidos por batalhas, o jogador começa com um Pokémon
  nv. 100 e Repel ativo (bloqueia tudo até nv. 29); nos casos de batalha, nv. 5 sem Repel.
  Antes o New Game do teste não tinha Pokémon nenhum.

Surf e pesca não foram exercitados em jogo (precisam do golpe e da vara); só a ligação das
tabelas foi conferida.

### Outras verificações

- 27 testes de regiões: OK. Planejador de desafios (WSL): carrega 2.176 fontes e nenhuma
  linha de piloto. No Windows ele falha ao ler um arquivo por codificação, com ou sem esta
  mudança (conferido com `git stash`): problema anterior, não desta etapa.
- ROM publicada intacta (SHA-1 da v0.5.0-preview.6).

### Pendente

- Seções de mapa de Johto (M5), que também separam as áreas do Hardcore.
- Rock Smash e eventos dos pilotos; Surf e pesca testados em jogo.

## 07/11/2026 — Seções de mapa dos pilotos (M5, Claude)

Pedido do usuário: seções de mapa (marco M5). Littleroot, Route 101, New Bark e Route 29
agora têm nome no jogo: aparece o nome ao entrar no mapa, o local de encontro (met location)
de um Pokémon capturado ali fica registrado com o nome certo, e o Hardcore passa a separar as
áreas. Os interiores herdam a seção da cidade, sem nome ao entrar, como nas fontes.

### Como o FireRed organiza as seções e por que a solução ficou assim

As seções são um byte, guardado no save (local de encontro de cada Pokémon e quotas do
Hardcore). O FireRed tem três faixas:

| Faixa | IDs | Situação original |
|---|---|---|
| Hoenn (`MAPSEC_LITTLEROOT_TOWN`…) | 0–87 | sem nome nem dados; existem para Pokémon trocados com Ruby/Sapphire/Emerald |
| Kanto e Sevii | 88–196 | com nome; do `MAPSEC_ONE_ISLAND` em diante o mapa regional trata como ilha |
| `MAPSEC_NONE`, `MAPSEC_COUNT` | 197, 198 | marcadores |

0xFD–0xFF são reservados (ovo, troca, encontro especial), então só cabem 56 seções novas.

- **Hoenn:** nomeadas as seções que já existem, com os mesmos IDs do Emerald. Seções novas
  no fim não caberiam para a Hoenn inteira. Novo campo `hoenn_name` no
  `region_map_sections.json` (LITTLEROOT TOWN, ROUTE 101), nova tabela `sHoennMapNames` gerada
  pelos templates, e `GetMapName` passou a usá-la. No resumo do Pokémon, o nome Hoenn só vale
  para Pokémon encontrados no FireRed/LeafGreen: trocas de Ruby/Sapphire/Emerald continuam com
  o texto original ("trade").
- **Johto:** `MAPSEC_NEW_BARK_TOWN` (197) e `MAPSEC_ROUTE_29` (198) acrescentadas **depois** da
  última seção do FireRed; nenhum ID existente mudou (Pallet continua 88). `MAPSEC_NONE` passou
  a 199. Novo `JOHTO_MAPSEC_START`. Regra registrada no template: nunca inserir seções antes
  delas, para não mudar locais de encontro já salvos. Johto inteira cabe nos 56 IDs livres.
  Os nomes são os mesmos do HNS, então `install_johto_pilot.py` agora copia a seção e a
  opção de nome da fonte.
- `show_map_name` ligado nos exteriores (Littleroot, Route 101, New Bark, Route 29), como no
  Emerald e no HNS; interiores e portão sem nome ao entrar, também como nas fontes.

### Mapa da cidade (Town Map) e Fly: travamento corrigido

Abrir o mapa da cidade ou o Fly num mapa piloto não era seguro:

- seções Hoenn (antes de Kanto) geravam índice negativo nas tabelas do mapa regional;
- `MAPSEC_NONE` (Johto antes desta etapa) tinha largura 0 e dividia por zero;
- seções Johto no fim da lista cairiam na faixa "Sevii", e a busca em `sSeviiMapsecs`, num
  `while` sem saída, nunca terminaria.

Correção em `src/region_map.c`: novo `IsMapsecOnRegionMaps` (Kanto até antes de Johto). Fora
disso, o mapa abre em Kanto com o cursor em Pallet Town, sem tocar as tabelas fora dos limites.
A tabela de destinos do Fly ganhou entradas para as seções Johto, só para ficar nos limites:
como Johto ainda não tem célula no mapa regional, o Fly nunca as oferece. Mapa regional
próprio de Hoenn/Johto precisa de gráficos novos e fica para depois.

### Teste na ROM

`test_region_pilot_maps.py` passou de 23 para 26 casos, todos aprovados:

- os casos de dados conferem ID da seção, nome (`GetMapNameGeneric`), opção de nome ao entrar e
  o local de encontro gravado num Pokémon criado no mapa: LITTLEROOT TOWN (0), ROUTE 101 (16),
  NEW BARK TOWN (197), ROUTE 29 (198) e PALLET TOWN (88, controle);
- casos 24–26: abrem o mapa da cidade como o item faz (escurece, limpa o campo, abre) em
  Littleroot, New Bark e Pallet, e exigem que ele abra e o jogo continue rodando. Capturas:
  mapa de Kanto com o cursor em Pallet nos três;
- as capturas dos casos de dados mostram a placa "LITTLEROOT TOWN" e "NEW BARK TOWN" ao entrar.

### Outras verificações

- 27 testes de regiões, `balance_encounters.py --check` e `generate_guides.py --check`: OK.
- ROM publicada intacta (SHA-1 da v0.5.0-preview.6).

### Não verificado / pendente

- A tela de resumo não foi aberta no teste: o texto do local de encontro de um Pokémon de
  Littleroot foi conferido pela regra do código, não pela tela.
- Mapa regional próprio de Hoenn e Johto, Fly e pontos de cura (resto do M5).
- Build nativa para PC com estas mudanças.
- Limitação do Hardcore registrada na etapa de encontros (Route 29 e New Bark como uma área
  só): **resolvida**, agora são seções diferentes.

## 08/11/2026 — Eventos dos pilotos, fase 1: ambiente (Claude)

Pedido do usuário: **todos** os eventos, com sprites importados do Emerald e do HNS quando o
FireRed não tiver. Codex pausado. "Todos" foi dividido em fases para não misturar coisas de
natureza diferente:

1. **Ambiente (esta entrada):** placas, moradores com suas falas, estantes/PCs/máquinas, item,
   árvore de Cut e a máquina de cura do laboratório do Elm.
2. **Personagens principais:** mães, Birch, Elm, rivais, Silver (e seus sprites).
3. **História:** caminhão, resgate do Birch, escolha de iniciais, batalhas de rival. Mexe na
   progressão do FireRed e vai precisar de decisões do usuário.

### Sprites

Regra: se o FireRed tem o mesmo tipo de personagem (Fat Man, Boy, Youngster, Lass, Woman 1–3,
Old Man, Little Boy, Scientist, Policeman, árvore de Cut, Poké Ball), usa o do FireRed; senão,
importa. Nesta fase foram importados dois:

| Sprite | Fonte | Paleta do FireRed | Erro de cor |
|---|---|---|---|
| `OBJ_EVENT_GFX_TWIN` (152) | pokeemerald `twin.png` | `NPC_WHITE` | maior distância 92 |
| `OBJ_EVENT_GFX_SCIENTIST_F` (153) | HNS_modern `scientist_f.png` | `NPC_PINK` | maior distância 48 |

Novo `scripts/import_npc_sprite.py`. O FireRed desenha moradores com 4 paletas fixas (azul,
rosa, verde, branca) e só um espaço de paleta especial por mapa; o próprio FireRed redesenhou o
Brendan de Ruby/Sapphire na paleta branca. O importador faz o mesmo: escolhe a paleta com menor
erro e leva cada cor à mais próxima, com o erro registrado em
[pilot-sprites.json](map-audits/pilot-sprites.json). Comparação lado a lado:
[pilot-sprites.png](map-audits/pilot-sprites.png). A cientista ficou quase idêntica; a Twin
perdeu o vermelho do laço e o amarelo do vestido (ficaram puxados para o marrom), porque nenhuma
das quatro paletas tem esses tons. Folhas de 9 quadros ganham um 10º (cópia do 0), porque a
tabela de animação do FireRed usa o quadro 9 (levantar a mão). IDs de sprite: o FireRed tinha
152 e reserva até 239; foram acrescentados no fim.

### Eventos

Novo `scripts/install_pilot_events.py` (idempotente): 77 eventos em 12 mapas, com relatório em
[pilot-events.json](map-audits/pilot-events.json) (origem, sprite, texto e motivo de cada um).

- **Textos copiados das fontes**, conferidos contra o charmap do FireRed (aspas curvas e
  `{UP_ARROW}` existem). Rótulos com o prefixo do mapa (`Hoenn_…`, `Johto_…`), porque o HNS reusa
  os rótulos do laboratório do Emerald e haveria conflito.
- **Ramos fixados** onde o script original depende da história, escolhendo o que faz sentido
  para quem chega de fora: Twin "boa sorte" (pós-aventura); assistente do Birch "o PROF. está em
  trabalho de campo" (o Birch só entra na fase 2); Fat Man e Lass de New Bark em falas neutras;
  velho e garoto da Route 29 nas falas do dia; ajudantes do Elm "sempre ocupados" e "salve o jogo".
- Personagens que o original esconde durante a introdução (vizinha, menino da Route 101,
  assistente do Elm) ficam sempre visíveis.
- **Potion** da Route 29: `finditem` com a flag nova `FLAG_HIDE_JOHTO_ROUTE29_POTION`, apelido de
  `FLAG_0x077` (sem uso no FireRed nem no hack). **Árvore de Cut:** script do próprio FireRed.
  **Máquina de cura** do Elm: com Pokémon recebido, cura a equipe (fanfarra e
  `HealPlayerParty`); sem, mostra o texto da máquina, como no HNS.
- Ficaram para as fases 2 e 3: placas "casa de {PLAYER}" e caixas de correio do jogador (dependem
  de quem é o jogador), PC e relógio do quarto (gênero e relógio de tempo real), Poké Balls dos
  iniciais, presente de dia da semana da Tuscany, árvore de berry e Pokémon andando do HNS.
- `install_hoenn_interiors.py` e `install_johto_pilot.py` agora preservam eventos e
  `scripts.inc` existentes ao serem executados de novo.

### Tradução PT-BR

Novo `scripts/region_pilot_ptbr.py` gera `hack-data/localization/region_pilot_ptbr.json`:
38 traduções revisadas para os 45 textos (os outros são só nomes de lugar, iguais em PT). A
chave em inglês é lida do próprio `scripts.inc`, para nunca divergir do que o jogo mostra.
`generate_locale.py` passou a ler esse arquivo e a tratá-lo como fala de mapa (quebra de linha
automática). Termos seguem as traduções existentes: CENTRO POKéMON, SALVAR, nomes de lugar em
inglês, PROF. Resultado: 8.177 entradas, 0 recusadas.

### Teste na ROM

`test_region_pilot_maps.py` passou de 26 para **107 casos**, todos aprovados. Os 81 novos
falam de verdade com cada evento: o jogador nasce numa célula livre ao lado (do outro lado do
balcão, no caso do guarda do portão), os outros personagens ficam congelados até o A, ele vira
para o evento, aperta A e o teste lê o texto mostrado (`gStringVar4`):

- 75 textos iguais aos da fonte;
- Potion vai para a mochila e a flag é marcada;
- árvore de Cut responde com o texto do FireRed;
- as duas máquinas de cura restauram a equipe (e sem Pokémon mostram o texto da máquina);
- 2 leituras em PT-BR (placa do laboratório do Birch e a Lass de New Bark) saem em português.

Problemas do próprio teste, resolvidos no caminho: a Twin anda e escapava antes do congelamento
(agora congela logo após o carregamento e a cada quadro até o A); o guarda do portão está atrás
de um balcão (o teste agora fala por cima do balcão, como o jogo permite); o decodificador
confundia caracteres que o charmap mapeia no mesmo byte (apóstrofo reto/curvo, `{UP_ARROW}`).

### Outras verificações

- 27 testes de regiões, `balance_encounters.py --check`, `generate_guides.py --check`: OK.
- ROM publicada intacta (SHA-1 da v0.5.0-preview.6).

### Pendente

- Fases 2 (personagens principais) e 3 (história).
- A Twin com cores aproximadas; se quiser cores exatas, dá para usar o espaço de paleta especial
  enquanto não houver outro personagem com paleta própria no mesmo mapa.

## 08/11/2026 — Eventos dos pilotos, fase 2: personagens principais (Claude)

Pedido do usuário: fase 2. Duas decisões do usuário antes de começar:

- **Famílias em Littleroot:** a May é filha do Prof. Birch, como no Emerald com jogador menino.
  Casa da May = família Birch (esposa, irmãozinho, May); casa do Brendan = família Norman
  (mãe, Vigoroths, Brendan).
- **Falas:** só falas das fontes. Personagem sem fala neutra aparece, mas não conversa.

Regra adicional, tirada das próprias fontes: entram só personagens que ficam no mapa no estado
normal do jogo original. Ficam de fora os que só existem em cenas: Norman em casa, a mãe na rua e
no quarto, os caminhões, o rival no laboratório, o Birch com o Zigzagoon, o policial do Elm.

### Personagens

| Personagem | Onde | Fala (da fonte) |
|---|---|---|
| Esposa do Birch | casa da May, 1F | "I think it's wonderful for people to travel with POKéMON…" (ramo de quem viaja) |
| Irmãozinho | casa da May, 1F | "Hi, neighbor! Do you already have your own POKéMON?" |
| May | quarto da May | "POKéMON fully restored! Items ready, and…" |
| Brendan | quarto do Brendan | "POKéMON fully restored… Items all packed, and…" |
| 2 Vigoroths | casa do Brendan, 1F | "Fugiiiiih!" / "Huggoh, uggo uggo…", com o grito |
| Prof. Birch | laboratório | "Countless POKéMON await you! … the itch to get out and do fieldwork again!" |
| Silver | janela do laboratório do Elm | ". . . So this is the famous Elm Pokémon Lab…" / "…What are you staring at?" |
| Amiga da mãe | casa do jogador em New Bark, 1F | "Hi, {PLAYER}! I'm visiting!…" |
| Mãe do Brendan, mãe de New Bark, Prof. Elm | — | **em silêncio**: todas as falas falam com o próprio filho ou dependem da história (ovo, telefone, Kimono Girls) |

Ajustes, todos registrados em [pilot-events.json](map-audits/pilot-events.json):

- **Assistente do Birch:** com o Birch no laboratório, a fala da fase 1 ("o PROF. está em trabalho
  de campo") virou outro ramo original dela, "o PROF. não é de trabalho de mesa", coerente com o
  Birch dizendo que quer voltar ao campo.
- **Brendan e May** ficam onde o script do quarto os põe ao carregar (`TryUpdateBrendanPos` /
  `TryUpdateMayPos`: (0,2) e (8,2), de frente para o PC). As coordenadas do `map.json` são o ponto de
  entrada da cena, em cima da escada: o primeiro teste pegou isso, o Brendan bloqueava a descida.
- **Silver:** as duas falas, o ponto de exclamação e ele virando para o jogador. O empurrão do
  original ficou de fora: os movimentos são fixos e só funcionam vindo de uma célula específica; de
  outro lado jogariam o jogador dentro de paredes.
- Brendan e May usam os sprites que o FireRed já tem (`RS_BRENDAN`, `RS_MAY`).

### Sprites importados nesta fase

`scripts/import_npc_sprite.py` agora aceita 16x16 e 32x32, um mapa de quadros (`--frames`, para a
folha de 5 quadros do Vigoroth) e paletas com menos de 16 cores (a do Silver tem 15). Todas as
importações ficam em `scripts/import_pilot_sprites.sh` (idempotente; Twin e cientista não mudaram ao
rodar de novo).

| Sprite | Fonte | Paleta | Maior distância de cor |
|---|---|---|---|
| `EM_MOM` | pokeemerald | rosa | 42 |
| `WOMAN_4` (esposa do Birch) | pokeemerald | branca | 64 |
| `NINJA_BOY` (16x16) | pokeemerald | rosa | 50 |
| `PROF_BIRCH` | pokeemerald | branca | 99 |
| `VIGOROTH_CARRYING_BOX`, `VIGOROTH_FACING_AWAY` (32x32) | pokeemerald | branca | 55 |
| `JOHTO_MOM` | HNS_modern | verde | 67 |
| `PROF_ELM` | HNS_modern | branca | 54 |
| `SILVER` | HNS_modern | azul | 46 |

Comparação lado a lado: [pilot-sprites-phase2.png](map-audits/pilot-sprites-phase2.png). Ficaram fiéis
a mãe, a esposa, o irmãozinho, o Birch, os Vigoroths e a mãe de Johto; o cabelo do Elm ficou mais
loiro e o do Silver mais escuro. São 162 sprites agora (de 239 possíveis).

### Tradução e teste

- 46 traduções revisadas para os 55 textos (os demais são nomes de lugar e gritos do Vigoroth);
  8.185 entradas, 0 recusadas.
- `test_region_pilot_maps.py`: **119 casos**, todos aprovados. 84 textos iguais aos da fonte; os 3
  personagens em silêncio não iniciam script ao apertar A; item, Cut, cura e PT-BR como antes.
- Ajuste do teste: o Vigoroth anda o tempo todo e dava um passo no mesmo quadro em que o fade-in do
  warp descongela os personagens. Na cópia de teste, `UnfreezeObjectEvents` fica sem efeito até o A;
  depois o `release` do script funciona normalmente.
- 27 testes de regiões: OK. ROM publicada intacta.

### Pendente

- Fase 3 (história): iniciais, resgate do Birch, batalhas de rival, presente da Tuscany, Poké Balls
  dos quartos e laboratórios, placas "casa de {PLAYER}", PC e relógio do quarto.
- Personagens em silêncio (mães, Elm) ganham falas quando a história da fase 3 der contexto.

## 08/11/2026 — Fase 3, passo 1: acesso pelo SEAGALLOP (Claude)

Decisões do usuário para a fase 3 (história):

- **Acesso:** navio em Vermilion depois da primeira Liga.
- **Iniciais:** um presente por região (Birch: Treecko/Torchic/Mudkip; Elm: Chikorita/Cyndaquil/Totodile).
- **Cenas:** resgate do Birch, batalhas de rival, pedido do Elm (versão reduzida) e falas das mães e
  do Elm conforme a história.

A fase 3 é entregue em passos testados: (1) acesso, (2) história de Hoenn, (3) história de Johto,
(4) mães. Este é o passo 1.

### Como funciona

O FireRed já tem a balsa SEAGALLOP (Vermilion e Sevii), com animação e tabela de destinos. Foram
acrescentados dois destinos e usado o script comum `EventScript_SailToDest`, que mostra a mensagem
de embarque do próprio FireRed e roda a animação:

- `SEAGALLOP_LITTLEROOT` (11) chega em Littleroot (11,14); `SEAGALLOP_NEW_BARK` (12) chega na margem
  de New Bark (24,13), ao lado do mar. Littleroot não tem porto; a chegada fica no centro da cidade.
- Direção da viagem (`sTravelDirectionMatrix`): Hoenn e Johto ficam a oeste de Kanto, então a ida sai
  de Vermilion para oeste e a volta para leste. A tabela passou de 11 para 13 bits por linha.
- **Vermilion:** um marinheiro novo no píer (22,30), na borda esquerda, sem bloquear o caminho do
  S.S. Anne. Fica escondido por `FLAG_HIDE_VERMILION_CITY_REGION_SAILOR` (apelido de `FLAG_0x078`, sem
  uso), que o script de entrada de Vermilion marca enquanto `FLAG_SYS_GAME_CLEAR` não estiver ligado e
  desmarca depois. Ele pergunta Littleroot (sim/não) e depois New Bark (sim/não), sem criar menus novos.
- **Volta:** um marinheiro em Littleroot (10,13) e outro em New Bark (24,12), criados pelo
  `install_pilot_events.py`, perguntam se quer voltar a Vermilion.

### Textos novos

As perguntas dos marinheiros são **textos novos** (não existem no Emerald nem no HNS); a regra "só
falas das fontes" vale para os personagens, e este é o acesso escolhido pelo usuário. As mensagens de
embarque são as do próprio FireRed. Traduções em `region_pilot_ptbr.json` (50 entradas; 8.189 no
total, 0 recusadas); a pergunta evita gênero ("agora que você venceu a LIGA POKéMON").

### Teste

`test_region_pilot_maps.py`, 127 casos, todos aprovados. Novos:

- Vermilion antes da Liga: o marinheiro não existe (A não inicia nada);
- depois da Liga: ele aparece e pergunta por Littleroot;
- viagens completas pelo SEAGALLOP: Vermilion → Littleroot (pelo menu do marinheiro), Vermilion → New
  Bark (pela balsa diretamente; a segunda pergunta exigiria responder "não" e depois "sim"),
  Littleroot → Vermilion e New Bark → Vermilion, conferindo mapa e célula de chegada.

O teste pegou um erro de posição: o marinheiro estava em (21,32), que é água sem colisão ao lado do
píer. Foi para (22,30), em cima do píer.

### Pendente

- Passos 2 (Hoenn), 3 (Johto) e 4 (mães).
- Vermilion → New Bark pelo menu (resposta "não" + "sim") não foi exercitado; a tabela de chegada foi.

## 08/11/2026 — Fase 3, passo 2: história de Hoenn (Claude)

Cenas do Emerald adaptadas aos mapas-piloto, com as falas das fontes. Novo `scripts/pilot_story.py`,
chamado pelo `install_pilot_events.py` (tipo de evento `story`).

### Estado da história

- `VAR_HOENN_STORY` (`VAR_0x4029`, sem uso no FireRed): 0 nada, 1 resgate começou, 2 inicial escolhido
  (batalha pendente, inclusive depois de desmaiar), 3 Birch resgatado, 4 May vencida.
- `VAR_HOENN_STARTER` (`VAR_0x402A`): 0 Treecko, 1 Torchic, 2 Mudkip.
- `FLAG_HIDE_HOENN_ROUTE101_RESCUE` (`FLAG_0x022`) esconde Birch, Zigzagoon e bolsa depois do resgate;
  `FLAG_HIDE_HOENN_BIRCH_LAB_BIRCH` (`FLAG_0x023`) esconde o Birch do laboratório até o resgate (marcada
  no script de entrada do laboratório conforme o estado).

### Route 101: resgate

- Gatilhos do Emerald em (10,19)/(11,19): "H-help me!", o jogador sobe 4 passos como em
  `Route101_Movement_EnterScene` e o Birch pede ajuda (`Route101_Text_PleaseHelp`). Os gatilhos de
  "Don't leave me like this!" (sul, oeste e norte) seguram o jogador enquanto o estado é 1 ou 2.
- Birch e Zigzagoon ficam correndo no lugar onde o mapa do Emerald os coloca; a perseguição em
  círculos ficou de fora.
- **Bolsa do Birch:** o FireRed não tem o seletor gráfico do Emerald (`ChooseStarter`), então a bolsa
  pergunta sim/não para Treecko, Torchic e Mudkip, em ordem. Inicial no nível 5; com o time cheio vai
  para o PC (script do próprio FireRed). Pergunta e mensagem de recebimento são **textos novos**.
- Batalha contra Zigzagoon selvagem **nível 84** (pós-Liga). O jogador luta com o próprio time. Depois,
  `Route101_Text_YouSavedMe`, tela escurece, os três somem, o time é curado e o estado vira 3.
- Sprites novos importados do Emerald: Zigzagoon (paleta branca) e bolsa do Birch (rosa).

### Laboratório e casa da May

- Assistente: `BirchAwayOnFieldwork` antes do resgate, `BirchIsntOneForDeskWork` depois.
- Birch aparece a partir do estado 3: `MightBeGoodIdeaToGoSeeRival`, e `CountlessPokemonAwait` depois
  da batalha da May. No Emerald `{RIVAL}` é o vizinho; no FireRed é o rival de Kanto, então o texto
  diz MAY.
- May no quarto: "getting ready" antes do resgate; no estado 3, `Route103_Text_MayLetsBattle`, batalha,
  `MayDefeated` e `MayTimeToHeadBack`. As falas são da Route 103 (as que citam o presente do pai); a
  batalha acontece no quarto porque a Route 103 não faz parte do piloto.

### Times da May

`hack-data/region_pilot_trainers.json`, aplicado pelo `balance_trainers.py` nos espaços sem uso
`TRAINER_MAY`, `TRAINER_MAY_2` e `TRAINER_MAY_3` (nome "MAY", retrato RS May, IA com
TRY_TO_FAINT/CHECK_VIABILITY). Times de `sParty_MayLilycove*` do Emerald, o confronto mais forte dela,
com **+55 níveis** (86–89):

| Inicial do jogador | Treinador | Time |
|---|---|---|
| Treecko | `TRAINER_MAY_2` | Tropius, Pelipper, Ludicolo, Combusken |
| Torchic | `TRAINER_MAY_3` | Tropius, Ludicolo, Slugma, Marshtomp |
| Mudkip | `TRAINER_MAY` | Tropius, Slugma, Pelipper, Grovyle |

`balance_trainers.py --check` continua passando. Os níveis são provisórios até o rebalanceamento geral.

### Traduções

11 entradas novas em `region_pilot_ptbr.json` (61 no total; 8.200 entradas geradas, 0 recusadas),
evitando gênero para o jogador.

### Teste

`test_region_pilot_maps.py`: **141 casos, todos aprovados**. Novo modo `SCENE` (aperta a direção, A, e
continua apertando A só enquanto há script, mensagem ou batalha), com time de teste forte (Mewtwo com
Psychic, estilo SET, texto rápido). Cenas verificadas:

- o gatilho inicia o resgate e o jogador termina em (10,15); o gatilho sul o devolve;
- bolsa: Treecko nível 5 entra no time, batalha contra Zigzagoon 84, vitória, estado 3, objetos escondidos;
- depois de desmaiar (estado 2) a bolsa leva direto à batalha;
- May contra cada inicial: treinador e time certos, nível do líder 86, vitória, estado 4;
- falas por estado: assistente depois do resgate, Birch (2 estados), May depois da batalha, bolsa sumida.

O caso que atravessa de Littleroot para a Route 101 agora roda com o resgate já feito.

### Pendente

- Passo 3 (Johto) e passo 4 (mães).
- Só o caminho "sim no Treecko" foi exercitado na bolsa; os outros iniciais vêm do mesmo bloco gerado.
- A May fala como se o jogador fosse iniciante ("I'll give you a taste…"); é a fala da fonte.

## 08/11/2026 — Fase 3, passo 3: levantamento (Claude, em andamento)

Ainda não há código. O que já foi apurado no HNS_modern:

- **Laboratório do Elm** (`NewBarkTown_Lab`): três POKé BALLs nas posições (8,4), (9,4) e (10,4), com
  as perguntas `ChooseChikorita`, `ChooseCyndaquil` e `ChooseTotodile`. Elas usam `{STR_VAR_2}` para a
  categoria do Pokémon (`buffermoncategory`, que o FireRed não tem).
  - Textos disponíveis: `ElmIntro`, `ElmAccepted`, `ElmRefused`, `ElmChooseAPokemon`, `ChoseStarter`,
    `ReceivedStarter`, `ShouldntTouch`, `ElmResearchAmbitions`, `AideGivePotion`, `AideGiveBalls` e
    `AideExplainBalls`.
- **Silver:** a primeira luta (Cherrygrove) usa `RivalSeen`, `SilverWin` e `RivalAfter`. Ele leva o
  inicial em vantagem: jogador com Chikorita → Silver com Cyndaquil, Cyndaquil → Totodile,
  Totodile → Chikorita.
- **Time escolhido:** o de Victory Road (`sParty_Rival_*5`, níveis 45–48, só espécies até a geração
  III). Com +40 níveis, fica entre 85 e 88. As equipes 6 e 7 usam Ursaluna Bloodmoon, que não existe
  no jogo.
- **Treinador:** as vagas sem uso `TRAINER_BRENDAN`, `_2` e `_3` (59–61) podem receber o Silver. É
  preciso importar o retrato `TRAINER_PIC_SILVER` do HNS e registrá-lo em
  `front_pic_tables.h`/`front_pic_anims.h`. A injeção do `balance_trainers.py` também precisa passar a
  aceitar retrato e classe.
- **Variáveis e flags livres:** `VAR_0x402B`/`VAR_0x402C`; `FLAG_0x024`–`0x027` e `FLAG_0x07F`
  (bolas escondidas e presentes da assistente).

## 08/11/2026 — Revisão e continuação da história de Johto (Codex)

### Estado retomado e limites da revisão

Retomada no commit `ace3a8a`, preservando os avanços registrados acima. A instalação
parcial descrita anteriormente neste chat já havia sido resolvida por outro trabalho:
15 mapas, eventos, viagens e Hoenn estavam no Git. O topo do README ainda dizia
que nenhum mapa estava instalado; foi corrigido. Revisão concentrada nas mudanças
da expansão desde `9dd3784`, nos geradores de eventos/treinadores, seções de mapas,
balsa, armazenamento dos presentes e testes. Não é uma auditoria completa de toda
a engine nem uma validação da campanha inteira.

### Elm: pedido reduzido e inicial

- `scripts/johto_pilot_story.py`, chamado pelo gerador existente de eventos:
  Elm pede ajuda para criar um Pokémon, aceita recusa e oferece as três bolas
  originais nas posições (8,4), (9,4), (10,4).
- Estado em `VAR_JOHTO_STORY` (`0x402B`): 0 não aceitou; 1 pode escolher;
  2 inicial entregue; 3 venceu Silver. `VAR_JOHTO_STARTER` (`0x402C`) registra
  Chikorita/Cyndaquil/Totodile como 0/1/2.
- Um presente de nível 5 por região. A escolha só é gravada depois do sucesso de
  `givemon`; equipe cheia envia ao PC, armazenamento completamente cheio permite
  tentar novamente. A bola escolhida desaparece (`0x024`–`0x026`), as outras não
  podem dar outro presente.
- Nova special `BufferMonCategory` reaproveita a categoria da Pokédex em STR_VAR_2,
  sem alterar VAR_RESULT. Não cria uma tabela duplicada de categorias.
- Falas originais do HNS, com PT-BR revisado. Não foi importada a missão do ovo,
  a polícia, o telefone ou um destino ainda ausente.

### Silver e correção da May

- Silver usa retrato próprio (ID 148, acrescentado ao fim), os três espaços Brendan
  sem uso e os times de Victory Road do HNS. O inicial dele tem vantagem sobre a
  escolha do jogador. Mantidos golpes/itens; níveis provisórios 85–88 (+40).
- Antes do inicial, conserva as falas na janela. Depois, interação inicia batalha;
  vitória avança para estado 3 e passa à fala posterior. Não altera o rival de Kanto.
- A revisão encontrou May fora da passagem geral de treino: a injeção tardia
  conservava batalha simples e classe genérica. O gerador agora dá a May e Silver
  classe RIVAL_LATE (depois trocada; ver revisão abaixo), batalha em dupla e
  AI_SCRIPT_DOUBLE_BATTLE, além dos filtros de golpes já usados.
- A tabela de treinadores do piloto aceita equipes com golpes/itens explícitos e
  offset por treinador; as outras 636 equipes geradas continuam no mesmo fluxo.

### Problemas encontrados nos testes

1. A expressão regular de leitura de falas podia ficar presa em retrocesso
   combinatório em scripts longos sem `msgbox`. Foi substituída por busca limitada
   a cada rótulo; teste regressivo com 200 comandos sem mensagem.
2. O driver que só apertava A não exercitava recusas. Agora detecta a janela real
   de sim/não e envia A/B; cobre Torchic, Mudkip, recusar os três e o segundo destino
   do marinheiro de Vermilion. Após recusa terminal usa B para não reabrir o NPC.
3. Dois Mewtwo de teste perderam alguns confrontos após a correção de treino/duplas.
   Isso não prova um bug de progressão: a derrota deixou o estado pendente. Foram
   separados cenários de vitória (Latios/Soul Dew preparados, só no fixture) e
   derrota (Magikarp com 1 HP), sem enfraquecer o treinador ou mudar o dano do jogo.
4. O arquivo de resultados agora começa como `running`, evitando que um resultado
   antigo aprovado seja tomado pelo resultado de uma execução interrompida.

### Validação em andamento

- 28 testes Python de regiões aprovados; geração de treinadores e localização
  consistente (`--check`). 8.217 entradas traduzidas, nenhuma recusada.
- Na execução intermediária, passaram as três escolhas do Elm, recusa, bloqueio
  antes do pedido, bloqueio de segundo presente, envio ao PC e armazenamento cheio.
- A execução final amplia a confirmação de duplas, IVs/EVs, derrota e save/reload
  real dos setores do jogo. O resultado final será registrado abaixo quando terminar.
- Tudo em cópia temporária/saídas ignoradas. Sem substituir ROM distribuída,
  publicar release ou alterar `main`. Os casos posicionam o jogador no cenário;
  continuam sem provar um percurso contínuo desde Kanto ou a campanha completa.

Reprodução, na raiz:

```powershell
python scripts/import_silver_pilot.py --source ../map-references/HNS_modern --game games/super-fire-red
python scripts/install_pilot_events.py --references ../map-references --game games/super-fire-red --report docs/map-audits/pilot-events.json
python scripts/region_pilot_ptbr.py
python games/super-fire-red/scripts/generate_locale.py
python games/super-fire-red/scripts/balance_trainers.py
python -m unittest discover -s scripts -p 'test_*region*.py' -q
```

No Ubuntu/WSL, executar na raiz `python3 games/super-fire-red/scripts/test_region_pilot_maps.py`.
Resultados e logs ficam em `games/super-fire-red/output/tests/region-pilot-maps/`.

### Resultado final desta etapa — 08/11/2026

- **162/162 casos instrumentados aprovados** em mGBA, ROM compilada em
  `/root/.cache/firered-386/region-pilot-ny_5vx21`. Resultado `status: passed`
  em `games/super-fire-red/output/tests/region-pilot-maps/results.json`;
  log completo em `games/super-fire-red/output/tests/region-story-verified.log`.
  Essas saídas são locais e ignoradas pelo Git.
- Passaram as três equipes de May e Silver, com conferência das espécies,
  nível do primeiro Pokémon, duplas, IVs 31 e soma de EVs 508 em todos os membros.
  Os cenários de vitória usam Latios preparados; contra a terceira equipe de
  Silver, dois Kyogre nível 100 com Origin Pulse/Mystic Water. Isso é preparação
  do fixture e não comprova balanceamento para uma equipe normal da campanha.
- Derrotas de May e Silver preservam o estado para nova tentativa. As três escolhas
  do Elm, recusa, bloqueios, envio ao PC e PC completamente cheio passaram.
- Save/reload usa as rotinas reais de setores: gravou, alterou os dados em RAM e
  recarregou escolha, Pokémon, flag da bola e localização. Não testa desligamento,
  menu Continue nem migração de saves antigos de usuários.
- Categoria PT-BR `FOLHA` confirmada durante a recusa do inicial. As duas escolhas
  alternativas de Hoenn, recusa de todos os iniciais e quatro viagens de balsa
  passaram. Portas, escadas, conexões, encontros e eventos anteriores passaram.
- **28 testes Python** de regiões e **2 testes C nativos** de portas aprovados;
  geradores de localização e 636 equipes consistentes em `--check`;
  `git diff --check` sem erros de whitespace.
- Sequência, equipes, golpes e itens documentados em
  [REGION_PILOT_CHALLENGES.md](REGION_PILOT_CHALLENGES.md). README e tabela do
  roadmap corrigidos para refletir os pilotos que já existem.

Pendências principais: demais eventos de história dos pilotos (incluindo mães e
presentes de assistentes), percurso contínuo sem posicionamento de teste, Continue
com save real, compatibilidade de saves antigos, mapas regionais/Fly e expansão
geográfica além dos 15 mapas. Johto/Hoenn completas e balanceamento final continuam
pendentes. Teto de nível: a decisão de 08/11 é 255, em projeto separado na `main` (ver roadmap). Nenhuma release ou main alterada.

## 08/11/2026 — Revisão do passo 3 do Codex (Claude)

Revisado o commit `2df2c07`. A ROM de teste foi refeita do zero e confirmou os 162 casos; os 28 testes
Python também passaram. O trabalho segue o levantamento: pedido reduzido do Elm, três bolas com uma
escolha só, PC cheio, recusas, save/reload e Silver com o time de Victory Road +40 (85–88), retrato
próprio e as vagas sem uso do Brendan.

**Erro corrigido:** May e Silver tinham recebido a classe `TRAINER_CLASS_RIVAL_LATE` para ativar o
treino de IVs/EVs dos desafios. Nessa classe, o texto de batalha do FireRed (`battle_message.c`) troca
o nome do treinador pelo do rival de Kanto, e a tela mostraria "RIVAL" com o nome que o jogador deu
ao rival de Kanto. Agora:

- os dois usam `TRAINER_CLASS_PKMN_TRAINER`;
- `EliteFour_IsChallenge` reconhece os seis IDs (`TRAINER_MAY*` e `TRAINER_BRENDAN*`), então o treino
  continua igual;
- o teste confere que a classe não é de rival e que o nome do oponente é MAY/SILVER, além de duplas,
  IVs 31 e EVs 508.

Resultado: **162/162 casos aprovados** com a correção.

Também mantido da revisão do Codex: May e Silver em batalha dupla, como os demais treinadores do jogo.

A nota "teto de nível 500" foi corrigida para a decisão de 08/11: 255, em projeto separado na `main`.

## 09/11/2026 — Fase 3, passo 4: mães e presentes da assistente do Elm (Claude)

Decisões do usuário: **adaptar as falas da fonte** para as mães e **incluir** os presentes da
assistente do Elm.

### Mães

- **Mãe da casa do Brendan (Hoenn):** usa o ramo `MomHealsParty` do Emerald.
  - Diz `PlayersHouse_1F_Text_YouShouldRestABit`, a tela escurece, toca a música de cura, o time é
    curado e ela se despede com `PlayersHouse_1F_Text_TakeCareHoney`.
  - Adaptação: saiu o prefixo "MOM: " e o ", honey" ("Take care, honey!" → "Take care!"). Regra
    `ADAPTATIONS` em `pilot_story.py`, aplicada só nas falas da história.
- **Mãe de New Bark (Johto):** usa a segunda página da despedida do HNS
  (`NewBarkTown_PlayersHouse_1F_Text_MomLeaving2`, "Just be careful… I'm behind you all the way!"). O
  texto é **sem alteração**, porque não chama o jogador de filho.

### Assistente do Elm

`johto_pilot_story.py`, objeto 0 do laboratório, com as falas do HNS:

- antes do inicial: `AideAlwaysBusy`;
- depois do inicial: `AideGivePotion` e uma Poção (`FLAG_GOT_JOHTO_AIDE_POTION`, `FLAG_0x027`);
- depois de vencer o Silver: `AideGiveBalls`, 5 Poké Balls e `AideExplainBalls`
  (`FLAG_GOT_JOHTO_AIDE_BALLS`, `FLAG_0x07F`);
- com a bolsa cheia, aparece a mensagem do próprio FireRed e o presente pode ser recebido depois.

No HNS a assistente vai até o jogador na saída do laboratório. Aqui ela entrega quando o jogador fala
com ela, para não depender da célula de entrada.

### Traduções

6 entradas novas (85 no total; 8.223 geradas, 0 recusadas), evitando gênero do jogador.

### Teste

`test_region_pilot_maps.py`: **167 casos, todos aprovados**. Casos novos:

- a mãe de Hoenn cura um time com 1 HP;
- assistente antes do inicial (nada);
- Poção depois do inicial;
- espera pelo Silver quando a Poção já foi entregue;
- Poké Balls depois do Silver.

Ajustes no teste:

- os casos de cura por conversa esperam 900 quadros, por causa do texto, da fanfarra e dos fades;
- o texto esperado de um NPC com ramos vem do ramo padrão (`_Busy`).

### Estado da fase 3

Os passos 1 a 4 estão concluídos. Pendências da expansão continuam no roadmap:

- mapa regional, Fly e cura;
- percurso contínuo;
- geografia completa;
- rebalanceamento geral e level cap 255.

## 09/11/2026 — M5: mapa regional e Fly, plano (Claude)

Decisões do usuário:

- **Arte original com rolagem:** a arte do Emerald (Hoenn) e do HNS (Johto) é convertida para o
  formato do mapa do FireRed, e essas páginas rolam para os lados;
- **Menu de troca do FireRed:** Hoenn e Johto entram no menu que já troca Kanto/Sevii, aparecendo só
  depois de visitar cada região.

O que o levantamento mostrou:

- **Grade e tela:** o FireRed mostra 21 colunas de mapa mais a coluna dos botões, nas células de tela
  4 a 25. A coluna 21 da grade tem os botões "trocar" (21,11) e "cancelar" (21,13). Hoenn usa 28
  colunas e Johto 25 (colunas 0–24 do HNS).
- **Origem da arte:** as fontes guardam o mapa como fundo *affine* 8bpp (tilemap 64×64 de 1 byte). A
  célula (c,r) da grade fica no tile (c+1, r+2).
- **Tamanho da arte:** Hoenn tem 233 tiles e 27 cores; Johto, 250 tiles e 18 cores. Nenhum tile passa
  de 9 cores, então cada região cabe em duas paletas de 15 cores.
- **VRAM:** o BG0 do mapa usa os tiles 0–319 (FireRed). Os tiles 512–575 recebem a animação de
  abertura (BG1) e os tiles 512–516 o fundo do modo Fly. As páginas novas ficam a partir do tile 576,
  com 448 espaços livres.
- **Rolagem em passos de célula:** a cada coluna, o buffer do BG0 é recopiado da página larga,
  mantendo os botões fixos. Cursor, ícone do jogador e ícones de Fly usam "coluna da página =
  coluna da tela + deslocamento".
- **Fly:** no FireRed o modo Fly não tem botão de troca e mostra a página da região onde o jogador
  está. Então, em Hoenn, o Fly mostra Hoenn. Littleroot e New Bark precisam de *heal locations* e de
  flags de "visitado".
- **Menu de troca:** as opções são miniaturas das regiões (`switch_map_menu.png`), não texto. Hoenn e
  Johto precisam de miniaturas próprias.

Passos:

1. conversor da arte (tiles, paletas, tilemap largo, grade de seções);
2. páginas no motor com rolagem;
3. menu de troca;
4. Fly e locais de cura;
5. teste na ROM.

### Passo 1: conversão da arte (09/11)

Novo `scripts/convert_region_map_pages.py`. Ele lê o mapa *affine* 8bpp de cada fonte e gera:

- `graphics/region_map/<região>_page_<n>.png`/`.pal`: uma folha 4bpp por paleta, com um tile por linha
  para que a numeração continue contígua sem tiles de enchimento;
- `<região>_page.bin`: tilemap de 20 linhas, com as colunas da grade mais 4 à esquerda e 5 à direita.
  Os tiles começam em 576 e as paletas em 5 (Hoenn) e 9 (Johto);
- `src/data/region_map/region_map_layout_<região>.h`: a grade de seções, com
  `REGIONMAP_<REGIÃO>_WIDTH`.

Resultado (`docs/map-audits/region-map-pages.json`):

| Região | Colunas da grade | Colunas da página | Tiles | Paletas |
|---|---:|---:|---:|---|
| Hoenn | 28 | 37 | 232 | 4 (slots 5–8) |
| Johto | 25 | 34 | 245 | 2 (slots 9–10) |

- **Cores:** todas são opacas e exatas na precisão do GBA. O índice 0 de cada paleta, que é
  transparente no hardware, fica sem uso.
- **Tiles repetidos:** são deduplicados também por espelhamento.
- **Fora da arte original:** entra o mar aberto da própria fonte.
- **Paletas de Hoenn:** são 4, porque as 27 cores não se dividem em dois grupos de 15.
- **Seções de Johto:** só New Bark e Route 29 existem no FireRed. As outras 53 seções do HNS ficam
  `MAPSEC_NONE` (a arte aparece, mas sem nome). Em Hoenn todas as seções existem no FireRed.

Teste novo: `scripts/test_region_map_pages.py` (3 casos). Ele redesenha as páginas a partir dos
arquivos gerados e compara pixel a pixel com a fonte. Também confere o limite do BG0, as paletas
livres e a grade.

### Passo 2: páginas no motor com rolagem (09/11)

Mudanças em `src/region_map.c`:

- **Novas regiões:** `REGIONMAP_HOENN` e `REGIONMAP_JOHTO`. O mapa abre na página da região onde o
  jogador está (IDs de Hoenn abaixo de `KANTO_MAPSEC_START`; os de Johto, depois de
  `JOHTO_MAPSEC_START`).
- **Abertura:** a página já abre rolada até a seção atual.
- **Tiles e paletas:** os tiles da página vão para a VRAM só quando ela aparece, porque as duas juntas
  não cabem a partir do tile 576. As paletas (5–10) carregam ao abrir.
- **Fundo:** a arte da página preenche a janela do mapa (tiles 3–26, linhas 2–19). A moldura, as
  linhas de cima e o botão "cancelar" vêm do fundo de Kanto do FireRed, como nas outras páginas.
- **Rolagem:** é uma coluna por passo, no mesmo ritmo do cursor. Ao chegar na coluna 20 indo para a
  direita, ou na coluna 0 indo para a esquerda, a página rola em vez de o cursor andar.
  - O fundo é recopiado a cada passo.
  - O ícone do jogador e os de Fly guardam a posição na página e somem quando saem da tela.
  - Start leva o cursor ao jogador e rola a página até ele.
- **Seções:** `GetSelectedMapSection` usa coluna da tela + deslocamento, e `GetPageMapSection` usa
  coluna da página.
  - As páginas não têm camada de cavernas.
  - Seções sem nome no FireRed (fora dos pilotos) não mostram nada.
- **Posição do jogador:** vem dos novos campos `hoenn_x`/`hoenn_y`/`hoenn_width`/`hoenn_height`
  (tabelas `sHoennMapSection*`) e, em Johto, de `x`/`y`. O conversor os calcula a partir da grade de
  cada página.

O conversor passou a aceitar só as seções da própria região: o HNS usa `MAPSEC_SAFARI_ZONE`, que no
FireRed é um ID de Hoenn. As paletas são gravadas em bytes com LF. O erro do `gbagfx` vinha de um CR
solto: a primeira versão escrevia CR+CR+LF. CRLF, que o pokefirered usa nas paletas, também funciona.

**Teste:** 169 casos aprovados.

- Os casos do mapa em Littleroot e New Bark agora abrem as páginas de Hoenn e Johto.
- Dois casos novos seguram a direção com o mapa aberto e conferem o tile da tela antes e depois:
  Hoenn rola de 0 para 6 e Johto de 3 para 0.
- Capturas em `docs/map-audits/region-map-hoenn-open.png`, `region-map-hoenn-scrolled.png` e
  `region-map-johto-scrolled.png`.

Ainda falta neste marco:

- Hoenn e Johto no menu de troca;
- Fly e locais de cura.

### Passos 3 e 4: menu de troca e Fly (12/11, Claude)

**Fly (passo 4):**

- **Pontos de pouso:** `heal_locations.json` ganhou `HEAL_LOCATION_LITTLEROOT_TOWN`
  (Littleroot 5,9, em frente à casa do Brendan, como no Emerald) e `HEAL_LOCATION_NEW_BARK_TOWN`
  (New Bark 20,12, em frente à casa do jogador, como no HNS). Os campos de renascimento apontam para
  a casa e a mãe de cada cidade, mas nenhum script os usa como último ponto de cura.
- **Visitado:** as flags `FLAG_WORLD_MAP_LITTLEROOT_TOWN` (`FLAG_0x8C3`) e
  `FLAG_WORLD_MAP_NEW_BARK_TOWN` (`FLAG_0x8C4`) são marcadas ao entrar na cidade, por um
  `setworldmapflag` no script de transição que o `install_pilot_events.py` gera (`WORLD_MAP_FLAGS`).
- **Mapa:** `GetMapsecType` trata as duas cidades como destinos de Fly. Os IDs de Hoenn ficam abaixo
  de `KANTO_MAPSEC_START`, então Littleroot tem uma tabela própria (`sHoennFlyDestinations`); New Bark
  usa a tabela do FireRed.
- **Limite:** como em Kanto e Sevii, o modo Fly não tem botão de troca. O jogador só voa dentro da
  região em que está; de Kanto não dá para voar para Hoenn.

**Menu de troca (passo 3):**

- **Quando aparece:** depois de visitar Littleroot ou New Bark. A partir daí o menu tem duas colunas:
  Kanto e as Sevii desbloqueadas à esquerda, Hoenn e Johto à direita. Sem nenhuma das duas visitada,
  o menu original do FireRed continua igual. O botão de troca aparece se houver Sevii ou uma dessas
  regiões.
- **Miniaturas:** 48×32 pixels, geradas pelo conversor a partir da arte de cada região
  (`switch_map_hoenn.png`, `switch_map_johto.png`), numa paleta de 15 cores compartilhada no espaço
  11. Ficam depois dos 128 tiles do menu do FireRed, com a moldura da entrada de Kanto.
- **Navegação:** ↑/↓ andam dentro da coluna. ←/→ trocam de coluna, para a mesma linha ou para a mais
  próxima desbloqueada. O destaque e as duas metades do cursor acompanham a coluna.

**Teste:** 174 casos aprovados.

- **Novos modos** `FLY` e `SWITCH`.
- **Fly:**
  - Littleroot para Littleroot (5,9);
  - New Bark para New Bark (20,12);
  - Route 101 para Littleroot: uma linha abaixo no mapa de Fly e A, pousando em (5,9).
- **Menu:** de Pallet para Hoenn (→) e de Littleroot para Johto (↓). O teste confere o tile da página
  que aparece depois da troca.
- **Problemas corrigidos no teste:**
  - um toque de seta de 8 quadros movia o cursor duas vezes, porque ele anda uma célula a cada 4
    quadros; agora o toque tem 2 quadros;
  - o bit 1024 servia ao mesmo tempo para o Kyogre do teste do Silver e para a Poção da assistente;
    a Poção passou para 4096, Littleroot visitada é 2048 e New Bark visitada 8192.
- **Capturas:** `docs/map-audits/region-map-switch-hoenn.png`, `region-map-switch-johto.png`,
  `region-map-fly-route101.png` e `region-map-fly-landed.png`.

Ainda pendente no M5:

- save/reload entre regiões;
- as demais cidades, quando a geografia crescer;
- voar entre regiões, se for desejado; hoje se vai pelo SEAGALLOP.

### Salvar e continuar entre regiões (12/11, Claude)

O teste da ROM ganhou o modo `CONTINUE`. Ele grava o save com a rotina real (`TrySavingData`), zera a
localização na RAM, recarrega (`LoadGameSave`) e entra por `CB2_ContinueSavedGame`, o mesmo caminho do
"Continuar" do menu principal. Depois confere mapa, célula e os tilesets da região.

Passaram os três casos:

- Littleroot (10,8), com os tilesets de Hoenn;
- Route 29 (34,11), com os de Johto;
- laboratório do Birch (6,8), com o tileset de interiores de Hoenn.

Ao voltar, aparece a placa com o nome do mapa, como no FireRed (`docs/map-audits/region-continue-route29.png`).
Nenhuma mudança no motor foi necessária. **177 casos aprovados.**

O teste não cobre:

- desligar o console de verdade (o save é recarregado na mesma sessão do emulador);
- a reprodução do "quest log" que o FireRed mostra antes de continuar;
- saves de versões antigas do hack.

## 12/11/2026 — M7: percurso contínuo dos pilotos (Claude)

Novo caso `JOURNEY` no teste da ROM. A única coisa que ele posiciona é o ponto de partida: Vermilion,
ao lado do marinheiro, com a Liga vencida e um time de teste forte. Daí em diante tudo acontece só
por comandos simulados:

1. barco para Littleroot;
2. Route 101: o gatilho do resgate, a bolsa (Treecko) e a batalha contra o Zigzagoon;
3. laboratório do Birch;
4. casa da May: escada até o quarto, batalha e descida;
5. barco de volta para Vermilion;
6. o marinheiro de novo: "não" para Littleroot e "sim" para New Bark;
7. laboratório do Elm: pedido, Chikorita e a Poção da assistente;
8. Silver;
9. ida à Route 29 e volta;
10. barco para Vermilion.

**Como funciona:**

- **Rotas a pé:** são calculadas em Python por busca em largura sobre a colisão e o comportamento dos
  tiles de cada mapa. A busca evita água, desníveis, balcões, warps, objetos e os gatilhos ativos.
- **Operações:** cada passo leva a célula de destino. A ROM segura a direção até chegar nela, então um
  NPC no caminho só atrasa. Ir para uma célula fora do plano é tratado como erro. Também há
  operações para virar, falar (com as perguntas marcadas para responder "não") e registrar ponto de
  controle.
- **Diálogos, batalhas e viagens:** depois de cada operação, o teste espera o campo ficar livre e vai
  apertando A, ou B nas perguntas marcadas.

**Problemas encontrados e corrigidos no teste:**

- Fora do campo (em batalha ou no barco), a rotina do teste saía mais cedo e ninguém apertava A.
- Sequências de direções às cegas desalinhavam. Agora cada passo confere a célula de destino.
- Numa saída logo depois de outra, a checagem de "trocou de mapa" usava o mapa da operação anterior.
- Quando um Pokémon desmaiava, o menu de troca abria no desmaiado e A não saía dele. Agora o teste
  aponta o cursor para um Pokémon que ainda pode lutar.
- O limite por operação subiu para 30.000 quadros (a batalha dupla do Silver é longa). O limite do
  emulador por caso subiu para 200.000 quadros.

**Resultado:** 245 operações e 13 pontos de controle conferidos, com mapa, célula, estados das duas
histórias e time. No fim, o jogador está em Vermilion (23,32) com 4 Pokémon (os dois do teste,
Treecko e Chikorita) e a flag da Poção marcada. **178 casos aprovados.** Captura final em
`docs/map-audits/region-journey-end.png`.

**O que o teste não cobre:**

- uma campanha desde o início do jogo (o ponto de partida já tem a Liga vencida);
- times normais de campanha (o time de teste é forte);
- a geografia completa de Johto e Hoenn, que ainda não existe.

## Hoenn, lote 1: Oldale, Route 102, Route 103 e Petalburg

**Mapas novos:** Oldale Town, Route 102, Route 103 e Petalburg City, ligados a partir da Route 101.
Oldale e Petalburg têm Centro Pokémon e Mart, com os interiores do FireRed (enfermeira em 7,2 e
vendedor em 2,3). A enfermeira grava o ponto de retorno de cada cidade. O Mart vende a lista do
Emerald.

- **Instalação:** `scripts/install_hoenn_batch1.py` copia os layouts preparados por
  `scripts/prepare_region_pilot.py`, registra os mapas em `gMapGroup_Hoenn` (mapas 7 a 14) e cria as
  portas animadas do Centro e do Mart em `src/data/hoenn_pilot_doors.h`. Relatório em
  `docs/map-audits/hoenn-batch1.json`.
- **Músicas provisórias:** Oldale usa a de Pewter, as rotas usam a da Route 3 e Petalburg usa a de
  Celadon.
- **Voo e mapa:** Oldale e Petalburg entram no mapa de Hoenn como destinos de Fly, com flags de mapa
  próprias (0x8C5 e 0x8C6).
- **Eventos:** NPCs, placas e itens do chão vêm do Emerald via `scripts/install_pilot_events.py`.
  Os textos têm 59 traduções para PT-BR.
- **Treinadores:** 12 treinadores das rotas 102 e 103, com os times originais e +60 níveis
  (provisório, até o rebalanceamento). Inclui a batalha dupla das gêmeas Amy e Liv.
- **Pokémon selvagens:** grama e pesca nas rotas 102 e 103, e pesca em Petalburg.

**Fica para depois:** casas, Ginásio e casa do Wally (portas fechadas), Rare Candy escondido,
árvores de berry e encontros na água (a tabela do Emerald tem outro número de vagas).

**Teste da ROM:**

- 8 caminhadas novas entre os mapas e pelas portas do Centro e do Mart;
- conversa com cada NPC, placa e item (os treinadores começam vencidos e mostram a fala pós-batalha);
- a enfermeira cura um time com 1 HP;
- o Mart abre.

**Resultado:** 234 casos aprovados (eram 178), incluindo a jornada contínua (M7) e o salvar/continuar.

**Problemas encontrados e corrigidos:**

- **Atributos dos tilesets:** só os metatiles de Littleroot e da Route 101 tinham atributos
  convertidos; os outros estavam zerados. Por isso, as portas do Centro e do Mart não abriam, e a
  água e os desníveis para leste não funcionavam. `scripts/convert_hoenn_batch1_attributes.py`
  preenche só as entradas zeradas que os mapas do lote usam. Ele usa o comportamento do Emerald e o
  valor que o próprio FireRed usa: porta, água do mar, lago, desnível para leste e topo de
  montanha. Ficaram sem comportamento: solo de berry, parede de base secreta e desníveis diagonais.
  Relatório em `docs/map-audits/hoenn-batch1-attributes.json`.
- **Célula de conversa:** o teste escolhia uma célula de água ao lado do menino do lago e da Poké
  Ball do norte de Petalburg. Agora a célula de onde o jogador fala precisa ter a mesma elevação do
  objeto.
- **Saída do Centro e do Mart:** as portas mandavam o jogador para a ponta do tapete (warp 0), e
  apertar para baixo ali não saía. Agora vão para o meio do tapete (warp 1), como no FireRed.
- **Poção da Route 102:** a checagem da Poção só lia a flag da Poção da Route 29. Agora aceita as
  duas.

## Johto, lote 1: Cherrygrove, Route 30 e Route 31

**Mapas novos:** Cherrygrove City, Route 30 e Route 31, ligados a partir da borda oeste da Route 29.
O terreno vem do HNS sem alteração, como no piloto de Johto. O tileset secundário novo
`gTileset_JohtoPilotCherrygroveCity` é copiado tal como está. Cherrygrove tem Centro Pokémon e Mart
com os interiores do FireRed: a enfermeira grava o ponto de retorno, e o Mart vende a lista de
Cherrygrove do HGSS (Poké Ball, Potion, Antidote, Paralyze Heal e Awakening). O HNS só vende Potion e
Antidote antes da Pokédex.

- **Instalação:**
  1. `scripts/install_johto_pilot.py` agora inclui os três exteriores (mapas 8 a 10 de
     `gMapGroup_Johto`), as regras de água (lago, água rasa, água funda de interior e seta para
     oeste) e a porta deslizante do Centro e do Mart (metatile 0x062, tabela própria para o par
     Johto General + Cherrygrove).
  2. Depois dele, `scripts/install_johto_batch1.py` repõe as duas portas de Cherrygrove (no meio do
     tapete), cria os interiores (mapas 11 e 12) e registra a heal location em (47,8), a flag de mapa
     `FLAG_WORLD_MAP_CHERRYGROVE_CITY` (0x8C7) e o destino de Fly. Relatório em
     `docs/map-audits/johto-batch1.json`.
- **Mapa da região:** as seções `MAPSEC_CHERRYGROVE_CITY`, `MAPSEC_ROUTE_30` e `MAPSEC_ROUTE_31`
  entram no fim da lista. `scripts/convert_region_map_pages.py` regenerou a grade de Johto, então
  essas três áreas agora têm nome na página.
- **Músicas provisórias:** Cherrygrove usa a de Vermilion, e as Routes 30 e 31 usam a da Route 24.
- **Eventos (`scripts/install_pilot_events.py`):**
  - NPCs e placas;
  - o pescador dá a Mystic Water uma vez (com a pergunta do original);
  - Poké Ball e Antidote no chão da Route 31;
  - duas árvores de Cut;
  - 27 textos traduzidos para PT-BR.
- **Treinadores:** Joey, Mikey e Don (Route 30) e Wade (Route 31), com os times do HNS e +60 níveis
  (provisório). Ficam sem telefone e sem revanche.
- **Pokémon selvagens:** grama e água nas Routes 30 e 31, pesca na Route 31 e em Cherrygrove.

**Fica para depois:**

- o passeio do Guide Gent e a batalha do Silver em Cherrygrove;
- a luta encenada do Joey, a ligação do Elm e a entrega da carta do Kenya;
- casas, casa do Mr. Pokémon, Dark Cave e portão de Violet (portas fechadas);
- os Pokémon de cenário e as árvores de berry;
- a pesca da Route 30 (12 vagas no HNS, 10 no FireRed) e a água de Cherrygrove (12 vagas diferentes).

**Teste da ROM:**

- 5 caminhadas novas: Route 29 → Cherrygrove, Cherrygrove → Route 30, Route 30 → Route 31, e as
  portas do Centro e do Mart;
- conversas com todos os eventos novos;
- a checagem das seções de mapa cobre as três seções novas.

**Problemas encontrados e corrigidos:**

- **Tileset de Cherrygrove fora da build:** na reexecução, o `install_johto_pilot.py` não anexava
  os tilesets novos (só escrevia na primeira vez). Agora ele anexa cada tileset que ainda falta.
- **Checagem da Route 29:** a Route 29 agora tem duas conexões. A primeira registrada é a de
  Cherrygrove, a oeste. A ida até New Bark continua coberta pelo caso 21.
- **Água do HNS:** a checagem do importador recusava qualquer tabela de água que não tivesse cinco
  vagas iguais. Agora só recusa quando cortar as vagas mudaria a tabela.

**Resultado:** 263 casos aprovados (eram 234).

## Batalhas dos treinadores dos lotes 1

Até aqui o teste só conferia a fala que os treinadores novos dizem depois de vencidos. Agora há um
caso de batalha para cada treinador dos dois lotes, exceto os nadadores, que ficam na água e
precisam de Surf. São 14 casos: a batalha dupla de Amy e Liv conta uma vez.

- **Como funciona:** cada caso começa com só aquele treinador ainda não vencido
  (`gPilotFreshTrainer`). O jogador fica na mesma célula das conversas e usa o Pokémon forte do
  teste. O teste aperta A durante a apresentação, a batalha e a fala final.
- **O que é conferido:**
  - o oponente certo, com o time e o nível de origem (+60);
  - o nome e se a batalha é simples ou dupla;
  - a vitória;
  - a flag do treinador marcada depois.

**Problemas encontrados e corrigidos no teste:**

- Treinadores que veem o jogador assim que ele chega começam o script sozinhos. O campo nunca ficava
  livre, e o caso parava no texto de apresentação. Agora, se um script já está rodando, a cena passa
  direto para a fase de apertar A.
- O resultado guarda só 8 letras do nome ("AMY & LI"). A comparação agora usa os 8 primeiros
  caracteres.

**Resultado:** 277 casos aprovados (eram 263), com as 14 batalhas vencidas e as flags marcadas.
