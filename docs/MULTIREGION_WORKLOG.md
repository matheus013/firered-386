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
- Abrir as portas: não há warps, porque os interiores não foram importados.
- Encontros na grama alta da Route101: não há tabela em `wild_encounters.json`, então
  hoje não aparecem Pokémon.
- Mapa regional, Fly e nome do local para as seções Hoenn (marco M5).
- Build nativa para PC com os mapas novos.
- O piloto de Johto (New Bark/Route29) ainda não foi instalado na engine.
