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
