# Encontros por rota e área — FireRed 386

Guia dos dados atuais da hack, extraído dos fontes que geraram v0.2.0-dev. As 386 espécies ainda não foram distribuídas; esta lista descreve o que existe hoje, sem antecipar encontros planejados. Válido para Normal e Hardcore; neste último, a cota por seção e a cláusula de duplicatas continuam valendo.

## Como ler as probabilidades

- **Chance** é a participação da espécie na tabela, condicionada a ocorrer um encontro pelo método indicado. Não é chance por passo, por lançamento de vara ou de captura.
- Grama/caverna, Surf, Rock Smash, Old Rod, Good Rod e Super Rod têm tabelas separadas. Cada uma soma 100%; slots repetidos da mesma espécie foram somados.
- A coluna **Níveis por slot** informa a faixa e seu peso dentro da tabela. Uma faixa como `3–5 (20%)` significa que esse slot pesa 20%; o nível é sorteado na faixa. Slots sobrepostos não significam distribuição uniforme entre todos os níveis da espécie.
- Repel, taxa da área, deslocamento, itens/habilidades e estado do jogo afetam se a batalha ocorre. As porcentagens abaixo são os pesos nominais dos slots do jogo; não incluem o pequeno viés do sorteio por módulo do gerador aleatório.
- Em Pokémon Tower, o Silph Scope é necessário para identificar/capturar os fantasmas comuns. Nas câmaras Tanoby, é necessário resolver Tanoby Key; as formas de Unown são discriminadas.
- Roamer, batalhas fixas e eventos aparecem em seções próprias. Tabelas alternativas de Altering Cave estão separadas das acessíveis normalmente. Tabelas de terra sem terreno de encontro no layout recebem aviso explícito; a presença de dados não cria grama onde ela não existe.

Os caminhos `game/...` citados abaixo são fontes locais reconstruídos por `scripts/setup.sh`, não arquivos versionados diretamente neste repositório. As alterações da hack estão em [patches/current.patch](patches/current.patch).

## Índice por rota

- [Route 1](#route1)
- [Route 2](#route2)
- [Route 3](#route3)
- [Route 4](#route4)
- [Route 5](#route5)
- [Route 6](#route6)
- [Route 7](#route7)
- [Route 8](#route8)
- [Route 9](#route9)
- [Route 10](#route10)
- [Route 11](#route11)
- [Route 12](#route12)
- [Route 13](#route13)
- [Route 14](#route14)
- [Route 15](#route15)
- [Route 16](#route16)
- [Route 17](#route17)
- [Route 18](#route18)
- [Route 19](#route19)
- [Route 20](#route20)
- [Route 21 — North](#route21-north)
- [Route 21 — South](#route21-south)
- [Route 22](#route22)
- [Route 23](#route23)
- [Route 24](#route24)
- [Route 25](#route25)

## Índice das demais áreas

- [Celadon City](#celadoncity)
- [Cerulean Cave — 1 F](#ceruleancave-1f)
- [Cerulean Cave — 2 F](#ceruleancave-2f)
- [Cerulean Cave — B1 F](#ceruleancave-b1f)
- [Cerulean City](#ceruleancity)
- [Cinnabar Island](#cinnabarisland)
- [Digletts Cave — B1 F](#diglettscave-b1f)
- [Five Island](#fiveisland)
- [Five Island — Lost Cave — Room 1](#fiveisland-lostcave-room1)
- [Five Island — Lost Cave — Room 2](#fiveisland-lostcave-room2)
- [Five Island — Lost Cave — Room 3](#fiveisland-lostcave-room3)
- [Five Island — Lost Cave — Room 4](#fiveisland-lostcave-room4)
- [Five Island — Lost Cave — Room 5](#fiveisland-lostcave-room5)
- [Five Island — Lost Cave — Room 6](#fiveisland-lostcave-room6)
- [Five Island — Lost Cave — Room 7](#fiveisland-lostcave-room7)
- [Five Island — Lost Cave — Room 8](#fiveisland-lostcave-room8)
- [Five Island — Lost Cave — Room 9](#fiveisland-lostcave-room9)
- [Five Island — Lost Cave — Room 10](#fiveisland-lostcave-room10)
- [Five Island — Lost Cave — Room 11](#fiveisland-lostcave-room11)
- [Five Island — Lost Cave — Room 12](#fiveisland-lostcave-room12)
- [Five Island — Lost Cave — Room 13](#fiveisland-lostcave-room13)
- [Five Island — Lost Cave — Room 14](#fiveisland-lostcave-room14)
- [Five Island — Meadow](#fiveisland-meadow)
- [Five Island — Memorial Pillar](#fiveisland-memorialpillar)
- [Five Island — Resort Gorgeous](#fiveisland-resortgorgeous)
- [Five Island — Water Labyrinth](#fiveisland-waterlabyrinth)
- [Four Island](#fourisland)
- [Four Island — Icefall Cave — 1 F](#fourisland-icefallcave-1f)
- [Four Island — Icefall Cave — B1 F](#fourisland-icefallcave-b1f)
- [Four Island — Icefall Cave — Back](#fourisland-icefallcave-back)
- [Four Island — Icefall Cave — Entrance](#fourisland-icefallcave-entrance)
- [Fuchsia City](#fuchsiacity)
- [Mt Ember — Exterior](#mtember-exterior)
- [Mt Ember — Ruby Path — 1 F](#mtember-rubypath-1f)
- [Mt Ember — Ruby Path — B1 F](#mtember-rubypath-b1f)
- [Mt Ember — Ruby Path — B1 F — Stairs](#mtember-rubypath-b1f-stairs)
- [Mt Ember — Ruby Path — B2 F](#mtember-rubypath-b2f)
- [Mt Ember — Ruby Path — B2 F — Stairs](#mtember-rubypath-b2f-stairs)
- [Mt Ember — Ruby Path — B3 F](#mtember-rubypath-b3f)
- [Mt Ember — Summit Path — 1 F](#mtember-summitpath-1f)
- [Mt Ember — Summit Path — 2 F](#mtember-summitpath-2f)
- [Mt Ember — Summit Path — 3 F](#mtember-summitpath-3f)
- [Mt Moon — 1 F](#mtmoon-1f)
- [Mt Moon — B1 F](#mtmoon-b1f)
- [Mt Moon — B2 F](#mtmoon-b2f)
- [One Island](#oneisland)
- [One Island — Kindle Road](#oneisland-kindleroad)
- [One Island — Treasure Beach](#oneisland-treasurebeach)
- [Pallet Town](#pallettown)
- [Pokemon Mansion — 1 F](#pokemonmansion-1f)
- [Pokemon Mansion — 2 F](#pokemonmansion-2f)
- [Pokemon Mansion — 3 F](#pokemonmansion-3f)
- [Pokemon Mansion — B1 F](#pokemonmansion-b1f)
- [Pokemon Tower — 3 F](#pokemontower-3f)
- [Pokemon Tower — 4 F](#pokemontower-4f)
- [Pokemon Tower — 5 F](#pokemontower-5f)
- [Pokemon Tower — 6 F](#pokemontower-6f)
- [Pokemon Tower — 7 F](#pokemontower-7f)
- [Power Plant](#powerplant)
- [Rock Tunnel — 1 F](#rocktunnel-1f)
- [Rock Tunnel — B1 F](#rocktunnel-b1f)
- [SSAnne — Exterior](#ssanne-exterior)
- [Safari Zone — Center](#safarizone-center)
- [Safari Zone — East](#safarizone-east)
- [Safari Zone — North](#safarizone-north)
- [Safari Zone — West](#safarizone-west)
- [Seafoam Islands — 1 F](#seafoamislands-1f)
- [Seafoam Islands — B1 F](#seafoamislands-b1f)
- [Seafoam Islands — B2 F](#seafoamislands-b2f)
- [Seafoam Islands — B3 F](#seafoamislands-b3f)
- [Seafoam Islands — B4 F](#seafoamislands-b4f)
- [Seven Island — Sevault Canyon](#sevenisland-sevaultcanyon)
- [Seven Island — Sevault Canyon — Entrance](#sevenisland-sevaultcanyon-entrance)
- [Seven Island — Tanoby Ruins](#sevenisland-tanobyruins)
- [Seven Island — Tanoby Ruins — Dilford Chamber](#sevenisland-tanobyruins-dilfordchamber)
- [Seven Island — Tanoby Ruins — Liptoo Chamber](#sevenisland-tanobyruins-liptoochamber)
- [Seven Island — Tanoby Ruins — Monean Chamber](#sevenisland-tanobyruins-moneanchamber)
- [Seven Island — Tanoby Ruins — Rixy Chamber](#sevenisland-tanobyruins-rixychamber)
- [Seven Island — Tanoby Ruins — Scufib Chamber](#sevenisland-tanobyruins-scufibchamber)
- [Seven Island — Tanoby Ruins — Viapois Chamber](#sevenisland-tanobyruins-viapoischamber)
- [Seven Island — Tanoby Ruins — Weepth Chamber](#sevenisland-tanobyruins-weepthchamber)
- [Seven Island — Trainer Tower](#sevenisland-trainertower)
- [Six Island — Altering Cave](#sixisland-alteringcave)
- [Six Island — Green Path](#sixisland-greenpath)
- [Six Island — Outcast Island](#sixisland-outcastisland)
- [Six Island — Pattern Bush](#sixisland-patternbush)
- [Six Island — Ruin Valley](#sixisland-ruinvalley)
- [Six Island — Water Path](#sixisland-waterpath)
- [Three Island — Berry Forest](#threeisland-berryforest)
- [Three Island — Bond Bridge](#threeisland-bondbridge)
- [Three Island — Port](#threeisland-port)
- [Two Island — Cape Brink](#twoisland-capebrink)
- [Vermilion City](#vermilioncity)
- [Victory Road — 1 F](#victoryroad-1f)
- [Victory Road — 2 F](#victoryroad-2f)
- [Victory Road — 3 F](#victoryroad-3f)
- [Viridian City](#viridiancity)
- [Viridian Forest](#viridianforest)

<a id="route1"></a>

## Route 1

Fonte: tabela `sRoute1_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route1`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pidgey | 2–5 | 50% | 2 (10%); 3 (35%); 4 (4%); 5 (1%) |
| Rattata | 2–4 | 50% | 2 (10%); 3 (35%); 4 (5%) |


<a id="route2"></a>

## Route 2

Fonte: tabela `sRoute2_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route2`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pidgey | 2–5 | 45% | 2 (10%); 3 (20%); 4 (10%); 5 (5%) |
| Rattata | 2–5 | 45% | 2 (10%); 3 (20%); 4 (10%); 5 (5%) |
| Caterpie | 4–5 | 5% | 4 (4%); 5 (1%) |
| Weedle | 4–5 | 5% | 4 (4%); 5 (1%) |


<a id="route3"></a>

## Route 3

Fonte: tabela `sRoute3_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route3`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Spearow | 6–8 | 35% | 6 (20%); 7 (10%); 8 (5%) |
| Pidgey | 6–7 | 30% | 6 (20%); 7 (10%) |
| Nidoran♂ | 6–7 | 14% | 6 (10%); 7 (4%) |
| Jigglypuff | 3, 5, 7 | 10% | 3 (5%); 5 (4%); 7 (1%) |
| Mankey | 7 | 10% | 7 (10%) |
| Nidoran♀ | 6 | 1% | 6 (1%) |


<a id="route4"></a>

## Route 4

Fonte: tabela `sRoute4_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route4`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Rattata | 8, 10, 12 | 35% | 8 (10%); 10 (20%); 12 (5%) |
| Spearow | 8, 10, 12 | 35% | 8 (10%); 10 (20%); 12 (5%) |
| Ekans | 6, 8, 10, 12 | 25% | 6 (10%); 8 (4%); 10 (10%); 12 (1%) |
| Mankey | 10, 12 | 5% | 10 (4%); 12 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route5"></a>

## Route 5

Fonte: tabela `sRoute5_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route5`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pidgey | 13, 15–16 | 40% | 13 (20%); 15 (15%); 16 (5%) |
| Meowth | 10, 12, 14, 16 | 35% | 10 (20%); 12 (10%); 14 (4%); 16 (1%) |
| Oddish | 13, 15–16 | 25% | 13 (10%); 15 (10%); 16 (5%) |


<a id="route6"></a>

## Route 6

Fonte: tabela `sRoute6_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route6`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pidgey | 13, 15–16 | 40% | 13 (20%); 15 (15%); 16 (5%) |
| Meowth | 10, 12, 14, 16 | 35% | 10 (20%); 12 (10%); 14 (4%); 16 (1%) |
| Oddish | 13, 15–16 | 25% | 13 (10%); 15 (10%); 16 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="route7"></a>

## Route 7

Fonte: tabela `sRoute7_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route7`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Meowth | 17–20 | 40% | 17 (25%); 18 (10%); 19 (4%); 20 (1%) |
| Pidgey | 19, 22 | 30% | 19 (20%); 22 (10%) |
| Oddish | 19, 22 | 20% | 19 (10%); 22 (10%) |
| Growlithe | 18, 20 | 10% | 18 (5%); 20 (5%) |


<a id="route8"></a>

## Route 8

Fonte: tabela `sRoute8_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route8`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Meowth | 18, 20 | 30% | 18 (20%); 20 (10%) |
| Pidgey | 18, 20 | 30% | 18 (20%); 20 (10%) |
| Ekans | 17, 19 | 20% | 17 (15%); 19 (5%) |
| Growlithe | 15–18 | 20% | 15 (4%); 16 (10%); 17 (5%); 18 (1%) |


<a id="route9"></a>

## Route 9

Fonte: tabela `sRoute9_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route9`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Rattata | 14, 16–17 | 40% | 14 (15%); 16 (20%); 17 (5%) |
| Spearow | 13, 16–17 | 35% | 13 (10%); 16 (20%); 17 (5%) |
| Ekans | 11, 13, 15, 17 | 25% | 11 (10%); 13 (4%); 15 (10%); 17 (1%) |


<a id="route10"></a>

## Route 10

Fonte: tabela `sRoute10_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route10`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Voltorb | 14, 16–17 | 40% | 14 (15%); 16 (20%); 17 (5%) |
| Spearow | 13, 16–17 | 35% | 13 (10%); 16 (20%); 17 (5%) |
| Ekans | 11, 13, 15, 17 | 25% | 11 (10%); 13 (4%); 15 (10%); 17 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route11"></a>

## Route 11

Fonte: tabela `sRoute11_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route11`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Ekans | 12, 14–15 | 40% | 12 (15%); 14 (20%); 15 (5%) |
| Spearow | 13, 15, 17 | 35% | 13 (10%); 15 (20%); 17 (5%) |
| Drowzee | 11, 13, 15 | 25% | 11 (10%); 13 (10%); 15 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route12"></a>

## Route 12

Fonte: tabela `sRoute12_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route12`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 22, 24, 26 | 35% | 22 (10%); 24 (20%); 26 (5%) |
| Pidgey | 23, 25, 27 | 30% | 23 (15%); 25 (10%); 27 (5%) |
| Venonat | 24, 26 | 30% | 24 (20%); 26 (10%) |
| Gloom | 28, 30 | 5% | 28 (4%); 30 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route13"></a>

## Route 13

Fonte: tabela `sRoute13_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route13`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 22, 24, 26 | 35% | 22 (10%); 24 (20%); 26 (5%) |
| Venonat | 24, 26 | 30% | 24 (20%); 26 (10%) |
| Pidgey | 25, 27 | 20% | 25 (10%); 27 (10%) |
| Ditto | 25 | 5% | 25 (5%) |
| Gloom | 28, 30 | 5% | 28 (4%); 30 (1%) |
| Pidgeotto | 29 | 5% | 29 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route14"></a>

## Route 14

Fonte: tabela `sRoute14_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route14`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 22, 24, 26 | 35% | 22 (10%); 24 (20%); 26 (5%) |
| Venonat | 24, 26 | 30% | 24 (20%); 26 (10%) |
| Ditto | 23 | 15% | 23 (15%) |
| Pidgey | 27 | 10% | 27 (10%) |
| Gloom | 30 | 5% | 30 (5%) |
| Pidgeotto | 29 | 5% | 29 (5%) |


<a id="route15"></a>

## Route 15

Fonte: tabela `sRoute15_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route15`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 22, 24, 26 | 35% | 22 (10%); 24 (20%); 26 (5%) |
| Venonat | 24, 26 | 30% | 24 (20%); 26 (10%) |
| Pidgey | 25, 27 | 20% | 25 (10%); 27 (10%) |
| Ditto | 25 | 5% | 25 (5%) |
| Gloom | 28, 30 | 5% | 28 (4%); 30 (1%) |
| Pidgeotto | 29 | 5% | 29 (5%) |


<a id="route16"></a>

## Route 16

Fonte: tabela `sRoute16_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route16`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Doduo | 18, 20, 22 | 35% | 18 (20%); 20 (10%); 22 (5%) |
| Rattata | 18, 20, 22 | 30% | 18 (15%); 20 (10%); 22 (5%) |
| Spearow | 20, 22 | 30% | 20 (20%); 22 (10%) |
| Raticate | 23, 25 | 5% | 23 (4%); 25 (1%) |


<a id="route17"></a>

## Route 17

Fonte: tabela `sRoute17_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route17`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Doduo | 24, 26, 28 | 35% | 24 (20%); 26 (10%); 28 (5%) |
| Spearow | 20, 22 | 30% | 20 (20%); 22 (10%) |
| Raticate | 25, 27, 29 | 25% | 25 (10%); 27 (10%); 29 (5%) |
| Fearow | 25, 27 | 5% | 25 (4%); 27 (1%) |
| Rattata | 22 | 5% | 22 (5%) |


<a id="route18"></a>

## Route 18

Fonte: tabela `sRoute18_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route18`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Doduo | 24, 26, 28 | 35% | 24 (20%); 26 (10%); 28 (5%) |
| Spearow | 20, 22 | 30% | 20 (20%); 22 (10%) |
| Fearow | 25, 27, 29 | 15% | 25 (10%); 27 (4%); 29 (1%) |
| Raticate | 25, 29 | 15% | 25 (10%); 29 (5%) |
| Rattata | 22 | 5% | 22 (5%) |


<a id="route19"></a>

## Route 19

Fonte: tabela `sRoute19_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route19`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route20"></a>

## Route 20

Fonte: tabela `sRoute20_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route20`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route21-north"></a>

## Route 21 — North

Fonte: tabela `sRoute21North_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route21_North`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tangela | 17–28 | 100% | 17 (1%); 18 (4%); 19 (5%); 20 (10%); 21 (10%); 22 (20%); 23 (20%); 24 (10%); 25 (10%); 26 (5%); 27 (4%); 28 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route21-south"></a>

## Route 21 — South

Fonte: tabela `sRoute21South_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route21_South`.

**Tabela sem terreno de encontro terrestre no layout atual:** estes slots constam nos dados, mas não representam captura por grama/caverna neste mapa.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tangela | 17–28 | 100% | 17 (1%); 18 (4%); 19 (5%); 20 (10%); 21 (10%); 22 (20%); 23 (20%); 24 (10%); 25 (10%); 26 (5%); 27 (4%); 28 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route22"></a>

## Route 22

Fonte: tabela `sRoute22_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route22`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mankey | 2–5 | 45% | 2 (10%); 3 (20%); 4 (10%); 5 (5%) |
| Rattata | 2–5 | 45% | 2 (10%); 3 (20%); 4 (10%); 5 (5%) |
| Spearow | 3, 5 | 10% | 3 (5%); 5 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="route23"></a>

## Route 23

Fonte: tabela `sRoute23_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route23`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mankey | 32, 34 | 30% | 32 (20%); 34 (10%) |
| Fearow | 40, 42, 44 | 25% | 40 (20%); 42 (4%); 44 (1%) |
| Ekans | 32, 34 | 20% | 32 (10%); 34 (10%) |
| Spearow | 32, 34 | 15% | 32 (5%); 34 (10%) |
| Arbok | 44 | 5% | 44 (5%) |
| Primeape | 42 | 5% | 42 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="route24"></a>

## Route 24

Fonte: tabela `sRoute24_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route24`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 12–14 | 25% | 12 (10%); 13 (10%); 14 (5%) |
| Caterpie | 7 | 20% | 7 (20%) |
| Weedle | 7 | 20% | 7 (20%) |
| Abra | 8, 10, 12 | 15% | 8 (4%); 10 (10%); 12 (1%) |
| Pidgey | 11, 13 | 15% | 11 (10%); 13 (5%) |
| Kakuna | 8 | 4% | 8 (4%) |
| Metapod | 8 | 1% | 8 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="route25"></a>

## Route 25

Fonte: tabela `sRoute25_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route25`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 12–14 | 25% | 12 (5%); 13 (10%); 14 (10%) |
| Caterpie | 8 | 20% | 8 (20%) |
| Weedle | 8 | 20% | 8 (20%) |
| Abra | 9, 11, 13 | 15% | 9 (4%); 11 (10%); 13 (1%) |
| Pidgey | 11, 13 | 15% | 11 (5%); 13 (10%) |
| Kakuna | 9 | 4% | 9 (4%) |
| Metapod | 9 | 1% | 9 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="celadoncity"></a>

## Celadon City

Fonte: tabela `sCeladonCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeladonCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 5–40 | 99% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–40 (4%) |
| Koffing | 30–40 | 1% | 30–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5–15 | 100% | 5–15 (100%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 15–35 | 99% | 15–25 (95%); 25–35 (4%) |
| Grimer | 30–40 | 1% | 30–40 (1%) |


<a id="ceruleancave-1f"></a>

## Cerulean Cave — 1 F

Fonte: tabela `sCeruleanCave1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCave_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Parasect | 49, 58 | 25% | 49 (20%); 58 (5%) |
| Magneton | 49 | 20% | 49 (20%) |
| Golbat | 46, 55 | 14% | 46 (10%); 55 (4%) |
| Ditto | 52, 61 | 11% | 52 (10%); 61 (1%) |
| Primeape | 52, 61 | 11% | 52 (10%); 61 (1%) |
| Machoke | 46 | 10% | 46 (10%) |
| Electrode | 58 | 5% | 58 (5%) |
| Wobbuffet | 55 | 4% | 55 (4%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 30–50 | 65% | 30–40 (60%); 40–50 (5%) |
| Golduck | 40–55 | 35% | 40–50 (30%); 45–55 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 30–50 | 65% | 30–40 (60%); 40–50 (5%) |
| Graveler | 40–55 | 35% | 40–50 (30%); 45–55 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="ceruleancave-2f"></a>

## Cerulean Cave — 2 F

Fonte: tabela `sCeruleanCave2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCave_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Golbat | 49, 58 | 25% | 49 (20%); 58 (5%) |
| Machoke | 49 | 20% | 49 (20%) |
| Parasect | 52, 61 | 14% | 52 (10%); 61 (4%) |
| Ditto | 55, 64 | 11% | 55 (10%); 64 (1%) |
| Kadabra | 55, 64 | 11% | 55 (10%); 64 (1%) |
| Magneton | 52 | 10% | 52 (10%) |
| Wobbuffet | 58 | 5% | 58 (5%) |
| Electrode | 61 | 4% | 61 (4%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 35–55 | 65% | 35–45 (60%); 45–55 (5%) |
| Graveler | 45–60 | 35% | 45–55 (30%); 50–60 (5%) |


<a id="ceruleancave-b1f"></a>

## Cerulean Cave — B1 F

Fonte: tabela `sCeruleanCaveB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCave_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Ditto | 58, 67 | 25% | 58 (20%); 67 (5%) |
| Kadabra | 58, 67 | 25% | 58 (20%); 67 (5%) |
| Parasect | 55, 64 | 14% | 55 (10%); 64 (4%) |
| Golbat | 52, 61 | 11% | 52 (10%); 61 (1%) |
| Machoke | 52 | 10% | 52 (10%) |
| Magneton | 55 | 10% | 55 (10%) |
| Electrode | 64 | 4% | 64 (4%) |
| Wobbuffet | 61 | 1% | 61 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 40–60 | 65% | 40–50 (60%); 50–60 (5%) |
| Golduck | 50–65 | 35% | 50–60 (30%); 55–65 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 40–60 | 65% | 40–50 (60%); 50–60 (5%) |
| Graveler | 50–65 | 35% | 50–60 (30%); 55–65 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–35 | 16% | 15–25 (15%); 25–35 (1%) |
| Psyduck | 15–25 | 4% | 15–25 (4%) |


<a id="ceruleancity"></a>

## Cerulean City

Fonte: tabela `sCeruleanCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 84% | 15–25 (80%); 25–35 (4%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="cinnabarisland"></a>

## Cinnabar Island

Fonte: tabela `sCinnabarIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `CinnabarIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="diglettscave-b1f"></a>

## Digletts Cave — B1 F

Fonte: tabela `sDiglettsCaveB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `DiglettsCave_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Diglett | 15–22 | 95% | 15 (10%); 16 (10%); 17 (15%); 18 (20%); 19 (20%); 20 (10%); 21 (5%); 22 (5%) |
| Dugtrio | 29, 31 | 5% | 29 (4%); 31 (1%) |


<a id="fiveisland"></a>

## Five Island

Fonte: tabela `sFiveIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 65% | 5–35 (60%); 35–40 (5%) |
| Hoppip | 5–15 | 30% | 5–15 (30%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="fiveisland-lostcave-room1"></a>

## Five Island — Lost Cave — Room 1

Fonte: tabela `sFiveIslandLostCaveRoom1_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room1`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room2"></a>

## Five Island — Lost Cave — Room 2

Fonte: tabela `sFiveIslandLostCaveRoom2_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room2`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room3"></a>

## Five Island — Lost Cave — Room 3

Fonte: tabela `sFiveIslandLostCaveRoom3_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room3`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room4"></a>

## Five Island — Lost Cave — Room 4

Fonte: tabela `sFiveIslandLostCaveRoom4_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room4`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room5"></a>

## Five Island — Lost Cave — Room 5

Fonte: tabela `sFiveIslandLostCaveRoom5_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room5`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room6"></a>

## Five Island — Lost Cave — Room 6

Fonte: tabela `sFiveIslandLostCaveRoom6_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room6`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room7"></a>

## Five Island — Lost Cave — Room 7

Fonte: tabela `sFiveIslandLostCaveRoom7_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room7`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room8"></a>

## Five Island — Lost Cave — Room 8

Fonte: tabela `sFiveIslandLostCaveRoom8_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room8`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room9"></a>

## Five Island — Lost Cave — Room 9

Fonte: tabela `sFiveIslandLostCaveRoom9_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room9`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room10"></a>

## Five Island — Lost Cave — Room 10

Fonte: tabela `sFiveIslandLostCaveRoom10_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room10`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 38, 40 | 25% | 38 (5%); 40 (20%) |
| Golbat | 41, 43 | 20% | 41 (10%); 43 (10%) |
| Zubat | 37 | 20% | 37 (20%) |
| Murkrow | 22 | 5% | 22 (5%) |


<a id="fiveisland-lostcave-room11"></a>

## Five Island — Lost Cave — Room 11

Fonte: tabela `sFiveIslandLostCaveRoom11_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room11`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 40 | 20% | 40 (20%) |
| Murkrow | 15, 20, 22 | 20% | 15 (10%); 20 (5%); 22 (5%) |
| Zubat | 37 | 20% | 37 (20%) |
| Golbat | 41 | 10% | 41 (10%) |


<a id="fiveisland-lostcave-room12"></a>

## Five Island — Lost Cave — Room 12

Fonte: tabela `sFiveIslandLostCaveRoom12_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room12`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 40 | 20% | 40 (20%) |
| Murkrow | 15, 20, 22 | 20% | 15 (10%); 20 (5%); 22 (5%) |
| Zubat | 37 | 20% | 37 (20%) |
| Golbat | 41 | 10% | 41 (10%) |


<a id="fiveisland-lostcave-room13"></a>

## Five Island — Lost Cave — Room 13

Fonte: tabela `sFiveIslandLostCaveRoom13_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room13`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 40 | 20% | 40 (20%) |
| Murkrow | 15, 20, 22 | 20% | 15 (10%); 20 (5%); 22 (5%) |
| Zubat | 37 | 20% | 37 (20%) |
| Golbat | 41 | 10% | 41 (10%) |


<a id="fiveisland-lostcave-room14"></a>

## Five Island — Lost Cave — Room 14

Fonte: tabela `sFiveIslandLostCaveRoom14_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room14`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Haunter | 44, 46, 48, 50, 52 | 30% | 44 (10%); 46 (10%); 48 (5%); 50 (4%); 52 (1%) |
| Gastly | 40 | 20% | 40 (20%) |
| Murkrow | 15, 20, 22 | 20% | 15 (10%); 20 (5%); 22 (5%) |
| Zubat | 37 | 20% | 37 (20%) |
| Golbat | 41 | 10% | 41 (10%) |


<a id="fiveisland-meadow"></a>

## Five Island — Meadow

Fonte: tabela `sFiveIslandMeadow_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_Meadow`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sentret | 10, 15 | 30% | 10 (20%); 15 (10%) |
| Pidgey | 44 | 20% | 44 (20%) |
| Hoppip | 10, 15 | 15% | 10 (10%); 15 (5%) |
| Pidgeotto | 48, 50 | 15% | 48 (10%); 50 (5%) |
| Meowth | 41 | 10% | 41 (10%) |
| Persian | 47, 50 | 5% | 47 (4%); 50 (1%) |
| Psyduck | 41 | 5% | 41 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 65% | 5–35 (60%); 35–40 (5%) |
| Hoppip | 5–15 | 30% | 5–15 (30%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="fiveisland-memorialpillar"></a>

## Five Island — Memorial Pillar

Fonte: tabela `sFiveIslandMemorialPillar_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_MemorialPillar`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Hoppip | 6, 8, 10, 12, 14, 16 | 100% | 6 (5%); 8 (15%); 10 (30%); 12 (30%); 14 (15%); 16 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 65% | 5–35 (60%); 35–40 (5%) |
| Hoppip | 5–15 | 30% | 5–15 (30%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="fiveisland-resortgorgeous"></a>

## Five Island — Resort Gorgeous

Fonte: tabela `sFiveIslandResortGorgeous_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_ResortGorgeous`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 65% | 5–35 (60%); 35–40 (5%) |
| Hoppip | 5–15 | 30% | 5–15 (30%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="fiveisland-waterlabyrinth"></a>

## Five Island — Water Labyrinth

Fonte: tabela `sFiveIslandWaterLabyrinth_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_WaterLabyrinth`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–20, 35–40 | 65% | 5–20 (60%); 35–40 (5%) |
| Hoppip | 5–15 | 30% | 5–15 (30%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="fourisland"></a>

## Four Island

Fonte: tabela `sFourIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wooper | 5–25 | 70% | 5–15 (60%); 15–25 (10%) |
| Psyduck | 5–35 | 30% | 5–35 (30%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="fourisland-icefallcave-1f"></a>

## Four Island — Icefall Cave — 1 F

Fonte: tabela `sFourIslandIcefallCave1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Swinub | 23, 25, 27, 29, 31 | 50% | 23 (5%); 25 (20%); 27 (10%); 29 (10%); 31 (5%) |
| Golbat | 45, 48 | 25% | 45 (20%); 48 (5%) |
| Seel | 45 | 10% | 45 (10%) |
| Zubat | 40 | 10% | 40 (10%) |
| Delibird | 30 | 5% | 30 (5%) |


<a id="fourisland-icefallcave-b1f"></a>

## Four Island — Icefall Cave — B1 F

Fonte: tabela `sFourIslandIcefallCaveB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Swinub | 23, 25, 27, 29, 31 | 50% | 23 (5%); 25 (20%); 27 (10%); 29 (10%); 31 (5%) |
| Golbat | 45, 48 | 25% | 45 (20%); 48 (5%) |
| Seel | 45 | 10% | 45 (10%) |
| Zubat | 40 | 10% | 40 (10%) |
| Delibird | 30 | 5% | 30 (5%) |


<a id="fourisland-icefallcave-back"></a>

## Four Island — Icefall Cave — Back

Fonte: tabela `sFourIslandIcefallCaveBack_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_Back`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 43, 45, 47 | 40% | 43 (20%); 45 (10%); 47 (10%) |
| Golbat | 45, 48 | 25% | 45 (20%); 48 (5%) |
| Dewgong | 49, 51, 53 | 20% | 49 (10%); 51 (5%); 53 (5%) |
| Zubat | 40 | 10% | 40 (10%) |
| Psyduck | 41 | 5% | 41 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–45 | 95% | 5–20 (60%); 20–35 (30%); 35–45 (5%) |
| Tentacruel | 35–45 | 4% | 35–45 (4%) |
| Lapras | 30–45 | 1% | 30–45 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="fourisland-icefallcave-entrance"></a>

## Four Island — Icefall Cave — Entrance

Fonte: tabela `sFourIslandIcefallCaveEntrance_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_Entrance`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 43, 45, 47 | 40% | 43 (20%); 45 (10%); 47 (10%) |
| Golbat | 45, 48 | 25% | 45 (20%); 48 (5%) |
| Dewgong | 49, 51, 53 | 20% | 49 (10%); 51 (5%); 53 (5%) |
| Zubat | 40 | 10% | 40 (10%) |
| Psyduck | 41 | 5% | 41 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 5–35 | 60% | 5–35 (60%) |
| Psyduck | 5–35 | 30% | 5–35 (30%) |
| Dewgong | 35–40 | 5% | 35–40 (5%) |
| Wooper | 5–15 | 5% | 5–15 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="fuchsiacity"></a>

## Fuchsia City

Fonte: tabela `sFuchsiaCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `FuchsiaCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 5–15 | 60% | 5–15 (60%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |
| Poliwag | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 15–25 | 40% | 15–25 (40%) |
| Seaking | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="mtember-exterior"></a>

## Mt Ember — Exterior

Fonte: tabela `sMtEmberExterior_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_Exterior`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Ponyta | 30, 33, 36 | 35% | 30 (20%); 33 (10%); 36 (5%) |
| Fearow | 38, 40 | 25% | 38 (20%); 40 (5%) |
| Spearow | 30, 32 | 15% | 30 (5%); 32 (10%) |
| Geodude | 33 | 10% | 33 (10%) |
| Machop | 35 | 10% | 35 (10%) |
| Rapidash | 39, 42 | 5% | 39 (4%); 42 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 5–30 | 95% | 5–20 (60%); 10–20 (30%); 15–30 (5%) |
| Graveler | 25–40 | 5% | 25–40 (4%); 30–40 (1%) |


<a id="mtember-rubypath-1f"></a>

## Mt Ember — Ruby Path — 1 F

Fonte: tabela `sMtEmberRubyPath1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 32, 34, 36, 38, 40 | 50% | 32 (10%); 34 (10%); 36 (20%); 38 (5%); 40 (5%) |
| Machop | 34, 36, 38 | 40% | 34 (10%); 36 (10%); 38 (20%) |
| Machoke | 40, 42 | 10% | 40 (5%); 42 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 25–40 | 65% | 25–35 (60%); 30–40 (5%) |
| Graveler | 30–50 | 35% | 30–45 (30%); 35–50 (5%) |


<a id="mtember-rubypath-b1f"></a>

## Mt Ember — Ruby Path — B1 F

Fonte: tabela `sMtEmberRubyPathB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 34, 36, 38, 40, 42 | 70% | 34 (10%); 36 (20%); 38 (20%); 40 (10%); 42 (10%) |
| Slugma | 24, 26, 28, 30 | 30% | 24 (10%); 26 (10%); 28 (5%); 30 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 25–40 | 65% | 25–35 (60%); 30–40 (5%) |
| Graveler | 30–50 | 35% | 30–45 (30%); 35–50 (5%) |


<a id="mtember-rubypath-b1f-stairs"></a>

## Mt Ember — Ruby Path — B1 F — Stairs

Fonte: tabela `sMtEmberRubyPathB1FStairs_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B1F_Stairs`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slugma | 22, 24, 26, 28, 30, 32 | 60% | 22 (5%); 24 (10%); 26 (20%); 28 (10%); 30 (10%); 32 (5%) |
| Geodude | 40, 42, 44 | 40% | 40 (20%); 42 (10%); 44 (10%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 25–40 | 65% | 25–35 (60%); 30–40 (5%) |
| Graveler | 30–50 | 35% | 30–45 (30%); 35–50 (5%) |


<a id="mtember-rubypath-b2f"></a>

## Mt Ember — Ruby Path — B2 F

Fonte: tabela `sMtEmberRubyPathB2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slugma | 22, 24, 26, 28, 30, 32 | 60% | 22 (5%); 24 (10%); 26 (20%); 28 (10%); 30 (10%); 32 (5%) |
| Geodude | 40, 42, 44 | 40% | 40 (20%); 42 (10%); 44 (10%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 25–40 | 65% | 25–35 (60%); 30–40 (5%) |
| Graveler | 30–50 | 35% | 30–45 (30%); 35–50 (5%) |


<a id="mtember-rubypath-b2f-stairs"></a>

## Mt Ember — Ruby Path — B2 F — Stairs

Fonte: tabela `sMtEmberRubyPathB2FStairs_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B2F_Stairs`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 34, 36, 38, 40, 42 | 70% | 34 (10%); 36 (20%); 38 (20%); 40 (10%); 42 (10%) |
| Slugma | 24, 26, 28, 30 | 30% | 24 (10%); 26 (10%); 28 (5%); 30 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 25–40 | 65% | 25–35 (60%); 30–40 (5%) |
| Graveler | 30–50 | 35% | 30–45 (30%); 35–50 (5%) |


<a id="mtember-rubypath-b3f"></a>

## Mt Ember — Ruby Path — B3 F

Fonte: tabela `sMtEmberRubyPathB3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slugma | 18, 20, 22, 24, 26, 28, 30, 32, 34, 36 | 100% | 18 (5%); 20 (5%); 22 (10%); 24 (10%); 26 (20%); 28 (20%); 30 (10%); 32 (10%); 34 (5%); 36 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slugma | 15–35 | 90% | 15–25 (60%); 25–35 (30%) |
| Magcargo | 25–45 | 10% | 25–35 (1%); 35–45 (4%); 40–45 (5%) |


<a id="mtember-summitpath-1f"></a>

## Mt Ember — Summit Path — 1 F

Fonte: tabela `sMtEmberSummitPath1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_SummitPath_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 29, 31, 33, 35, 37 | 50% | 29 (10%); 31 (10%); 33 (20%); 35 (5%); 37 (5%) |
| Machop | 31, 33, 35, 37, 39 | 50% | 31 (10%); 33 (10%); 35 (20%); 37 (5%); 39 (5%) |


<a id="mtember-summitpath-2f"></a>

## Mt Ember — Summit Path — 2 F

Fonte: tabela `sMtEmberSummitPath2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_SummitPath_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 30, 32, 34 | 40% | 30 (10%); 32 (10%); 34 (20%) |
| Machop | 32, 34, 36 | 40% | 32 (10%); 34 (10%); 36 (20%) |
| Machoke | 38, 40 | 20% | 38 (10%); 40 (10%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 5–30 | 95% | 5–20 (60%); 10–20 (30%); 15–30 (5%) |
| Graveler | 25–40 | 5% | 25–40 (4%); 30–40 (1%) |


<a id="mtember-summitpath-3f"></a>

## Mt Ember — Summit Path — 3 F

Fonte: tabela `sMtEmberSummitPath3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_SummitPath_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 29, 31, 33, 35, 37 | 50% | 29 (10%); 31 (10%); 33 (20%); 35 (5%); 37 (5%) |
| Machop | 31, 33, 35, 37, 39 | 50% | 31 (10%); 33 (10%); 35 (20%); 37 (5%); 39 (5%) |


<a id="mtmoon-1f"></a>

## Mt Moon — 1 F

Fonte: tabela `sMtMoon1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtMoon_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Zubat | 7–10 | 69% | 7 (29%); 8 (20%); 9 (10%); 10 (10%) |
| Geodude | 7–9 | 25% | 7 (10%); 8 (10%); 9 (5%) |
| Paras | 8 | 5% | 8 (5%) |
| Clefairy | 8 | 1% | 8 (1%) |


<a id="mtmoon-b1f"></a>

## Mt Moon — B1 F

Fonte: tabela `sMtMoonB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtMoon_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Paras | 5–10 | 100% | 5 (14%); 6 (14%); 7 (25%); 8 (25%); 9 (11%); 10 (11%) |


<a id="mtmoon-b2f"></a>

## Mt Moon — B2 F

Fonte: tabela `sMtMoonB2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtMoon_B2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Zubat | 8–11 | 49% | 8 (20%); 9 (10%); 10 (10%); 11 (9%) |
| Geodude | 9–10 | 30% | 9 (20%); 10 (10%) |
| Paras | 10, 12 | 15% | 10 (10%); 12 (5%) |
| Clefairy | 10, 12 | 6% | 10 (5%); 12 (1%) |


<a id="oneisland"></a>

## One Island

Fonte: tabela `sOneIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `OneIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="oneisland-kindleroad"></a>

## One Island — Kindle Road

Fonte: tabela `sOneIslandKindleRoad_FireRed` de `game/src/data/wild_encounters.json`; mapa `OneIsland_KindleRoad`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Ponyta | 31, 34 | 30% | 31 (10%); 34 (20%) |
| Spearow | 30, 32 | 25% | 30 (5%); 32 (20%) |
| Fearow | 36 | 10% | 36 (10%) |
| Geodude | 31 | 10% | 31 (10%) |
| Meowth | 31 | 10% | 31 (10%) |
| Persian | 37, 40 | 5% | 37 (4%); 40 (1%) |
| Psyduck | 34 | 5% | 34 (5%) |
| Rapidash | 37, 40 | 5% | 37 (4%); 40 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 5–30 | 95% | 5–20 (60%); 10–20 (30%); 15–30 (5%) |
| Graveler | 25–40 | 5% | 25–40 (4%); 30–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="oneisland-treasurebeach"></a>

## One Island — Treasure Beach

Fonte: tabela `sOneIslandTreasureBeach_FireRed` de `game/src/data/wild_encounters.json`; mapa `OneIsland_TreasureBeach`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Spearow | 31–32 | 30% | 31 (10%); 32 (20%) |
| Tangela | 33, 35 | 30% | 33 (20%); 35 (10%) |
| Fearow | 36, 38, 40 | 20% | 36 (10%); 38 (5%); 40 (5%) |
| Meowth | 31 | 10% | 31 (10%) |
| Persian | 37, 40 | 5% | 37 (4%); 40 (1%) |
| Psyduck | 31 | 5% | 31 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="pallettown"></a>

## Pallet Town

Fonte: tabela `sPalletTown_FireRed` de `game/src/data/wild_encounters.json`; mapa `PalletTown`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5–10 | 100% | 5–10 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="pokemonmansion-1f"></a>

## Pokemon Mansion — 1 F

Fonte: tabela `sPokemonMansion1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Koffing | 28, 30 | 30% | 28 (20%); 30 (10%) |
| Raticate | 32, 36 | 30% | 32 (20%); 36 (10%) |
| Growlithe | 30, 32 | 15% | 30 (10%); 32 (5%) |
| Rattata | 26, 28 | 15% | 26 (5%); 28 (10%) |
| Grimer | 28 | 5% | 28 (5%) |
| Weezing | 32 | 5% | 32 (5%) |


<a id="pokemonmansion-2f"></a>

## Pokemon Mansion — 2 F

Fonte: tabela `sPokemonMansion2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Koffing | 28, 30 | 30% | 28 (20%); 30 (10%) |
| Raticate | 32, 36 | 30% | 32 (20%); 36 (10%) |
| Growlithe | 30, 32 | 15% | 30 (10%); 32 (5%) |
| Rattata | 26, 28 | 15% | 26 (5%); 28 (10%) |
| Grimer | 28 | 5% | 28 (5%) |
| Weezing | 32 | 5% | 32 (5%) |


<a id="pokemonmansion-3f"></a>

## Pokemon Mansion — 3 F

Fonte: tabela `sPokemonMansion3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Koffing | 28, 30 | 30% | 28 (20%); 30 (10%) |
| Raticate | 32, 36 | 30% | 32 (20%); 36 (10%) |
| Growlithe | 30, 32 | 15% | 30 (10%); 32 (5%) |
| Rattata | 26, 28 | 15% | 26 (5%); 28 (10%) |
| Grimer | 28 | 5% | 28 (5%) |
| Weezing | 32 | 5% | 32 (5%) |


<a id="pokemonmansion-b1f"></a>

## Pokemon Mansion — B1 F

Fonte: tabela `sPokemonMansionB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Koffing | 28, 30 | 30% | 28 (20%); 30 (10%) |
| Raticate | 34, 38 | 30% | 34 (20%); 38 (10%) |
| Growlithe | 30, 32 | 15% | 30 (10%); 32 (5%) |
| Ditto | 30 | 10% | 30 (10%) |
| Grimer | 28 | 5% | 28 (5%) |
| Rattata | 26 | 5% | 26 (5%) |
| Weezing | 34 | 5% | 34 (5%) |


<a id="pokemontower-3f"></a>

## Pokemon Tower — 3 F

Fonte: tabela `sPokemonTower3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gastly | 13–19 | 90% | 13 (10%); 14 (10%); 15 (20%); 16 (20%); 17 (15%); 18 (10%); 19 (5%) |
| Cubone | 15, 17 | 9% | 15 (5%); 17 (4%) |
| Haunter | 20 | 1% | 20 (1%) |


<a id="pokemontower-4f"></a>

## Pokemon Tower — 4 F

Fonte: tabela `sPokemonTower4F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_4F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gastly | 13–19 | 86% | 13 (10%); 14 (10%); 15 (20%); 16 (20%); 17 (15%); 18 (10%); 19 (1%) |
| Cubone | 15, 17 | 9% | 15 (5%); 17 (4%) |
| Haunter | 20 | 5% | 20 (5%) |


<a id="pokemontower-5f"></a>

## Pokemon Tower — 5 F

Fonte: tabela `sPokemonTower5F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_5F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gastly | 13–19 | 86% | 13 (10%); 14 (10%); 15 (20%); 16 (20%); 17 (15%); 18 (10%); 19 (1%) |
| Cubone | 15, 17 | 9% | 15 (5%); 17 (4%) |
| Haunter | 20 | 5% | 20 (5%) |


<a id="pokemontower-6f"></a>

## Pokemon Tower — 6 F

Fonte: tabela `sPokemonTower6F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_6F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gastly | 14–19 | 85% | 14 (10%); 15 (10%); 16 (20%); 17 (20%); 18 (15%); 19 (10%) |
| Cubone | 17, 19 | 9% | 17 (5%); 19 (4%) |
| Haunter | 21, 23 | 6% | 21 (5%); 23 (1%) |


<a id="pokemontower-7f"></a>

## Pokemon Tower — 7 F

Fonte: tabela `sPokemonTower7F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_7F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gastly | 15–19 | 75% | 15 (10%); 16 (20%); 17 (20%); 18 (15%); 19 (10%) |
| Haunter | 23, 25 | 15% | 23 (14%); 25 (1%) |
| Cubone | 17, 19 | 10% | 17 (5%); 19 (5%) |


<a id="powerplant"></a>

## Power Plant

Fonte: tabela `sPowerPlant_FireRed` de `game/src/data/wild_encounters.json`; mapa `PowerPlant`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magnemite | 22, 25 | 30% | 22 (20%); 25 (10%) |
| Voltorb | 22, 25 | 30% | 22 (20%); 25 (10%) |
| Pikachu | 22, 24, 26 | 25% | 22 (10%); 24 (10%); 26 (5%) |
| Magneton | 31, 34 | 10% | 31 (5%); 34 (5%) |
| Electabuzz | 32, 35 | 5% | 32 (4%); 35 (1%) |


<a id="rocktunnel-1f"></a>

## Rock Tunnel — 1 F

Fonte: tabela `sRockTunnel1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `RockTunnel_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 15–17 | 35% | 15 (5%); 16 (20%); 17 (10%) |
| Zubat | 15–16 | 30% | 15 (20%); 16 (10%) |
| Machop | 16–17 | 15% | 16 (10%); 17 (5%) |
| Mankey | 16–17 | 15% | 16 (10%); 17 (5%) |
| Onix | 13, 15 | 5% | 13 (4%); 15 (1%) |


<a id="rocktunnel-b1f"></a>

## Rock Tunnel — B1 F

Fonte: tabela `sRockTunnelB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `RockTunnel_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 15–17 | 35% | 15 (5%); 16 (10%); 17 (20%) |
| Zubat | 15–16 | 30% | 15 (10%); 16 (20%) |
| Mankey | 16–17 | 15% | 16 (5%); 17 (10%) |
| Machop | 17 | 10% | 17 (10%) |
| Onix | 13, 15, 17 | 10% | 13 (5%); 15 (4%); 17 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 5–30 | 95% | 5–20 (60%); 10–20 (30%); 15–30 (5%) |
| Graveler | 25–40 | 5% | 25–40 (4%); 30–40 (1%) |


<a id="ssanne-exterior"></a>

## SSAnne — Exterior

Fonte: tabela `sSSAnneExterior_FireRed` de `game/src/data/wild_encounters.json`; mapa `SSAnne_Exterior`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 44% | 15–25 (40%); 25–35 (4%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="safarizone-center"></a>

## Safari Zone — Center

Fonte: tabela `sSafariZoneCenter_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_Center`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Exeggcute | 24–25 | 20% | 24 (10%); 25 (10%) |
| Nidoran♂ | 22 | 20% | 22 (20%) |
| Rhyhorn | 25 | 20% | 25 (20%) |
| Venonat | 22 | 15% | 22 (15%) |
| Nidorino | 31 | 10% | 31 (10%) |
| Nidorina | 31 | 5% | 31 (5%) |
| Parasect | 30 | 5% | 30 (5%) |
| Scyther | 23 | 4% | 23 (4%) |
| Chansey | 23 | 1% | 23 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 5–15 | 60% | 5–15 (60%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |
| Poliwag | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 15–25 | 40% | 15–25 (40%) |
| Seaking | 20–30 | 40% | 20–30 (40%) |
| Dratini | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 4% | 15–35 (4%) |
| Dragonair | 25–35 | 1% | 25–35 (1%) |


<a id="safarizone-east"></a>

## Safari Zone — East

Fonte: tabela `sSafariZoneEast_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_East`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Doduo | 26 | 20% | 26 (20%) |
| Exeggcute | 23, 25 | 20% | 23 (10%); 25 (10%) |
| Nidoran♂ | 24 | 20% | 24 (20%) |
| Paras | 22 | 15% | 22 (15%) |
| Nidorino | 33 | 10% | 33 (10%) |
| Nidoran♀ | 24 | 5% | 24 (5%) |
| Parasect | 25 | 5% | 25 (5%) |
| Kangaskhan | 25 | 4% | 25 (4%) |
| Scyther | 28 | 1% | 28 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 5–15 | 60% | 5–15 (60%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |
| Poliwag | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 15–25 | 40% | 15–25 (40%) |
| Seaking | 20–30 | 40% | 20–30 (40%) |
| Dratini | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 4% | 15–35 (4%) |
| Dragonair | 25–35 | 1% | 25–35 (1%) |


<a id="safarizone-north"></a>

## Safari Zone — North

Fonte: tabela `sSafariZoneNorth_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_North`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Exeggcute | 25, 27 | 20% | 25 (10%); 27 (10%) |
| Nidoran♂ | 30 | 20% | 30 (20%) |
| Rhyhorn | 26 | 20% | 26 (20%) |
| Paras | 23 | 15% | 23 (15%) |
| Nidorino | 30 | 10% | 30 (10%) |
| Nidorina | 30 | 5% | 30 (5%) |
| Venomoth | 32 | 5% | 32 (5%) |
| Chansey | 26 | 4% | 26 (4%) |
| Tauros | 28 | 1% | 28 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 5–15 | 60% | 5–15 (60%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |
| Poliwag | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 15–25 | 40% | 15–25 (40%) |
| Seaking | 20–30 | 40% | 20–30 (40%) |
| Dratini | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 4% | 15–35 (4%) |
| Dragonair | 25–35 | 1% | 25–35 (1%) |


<a id="safarizone-west"></a>

## Safari Zone — West

Fonte: tabela `sSafariZoneWest_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_West`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Doduo | 26 | 20% | 26 (20%) |
| Exeggcute | 25, 27 | 20% | 25 (10%); 27 (10%) |
| Nidoran♂ | 22 | 20% | 22 (20%) |
| Venonat | 23 | 15% | 23 (15%) |
| Nidorino | 30 | 10% | 30 (10%) |
| Nidoran♀ | 30 | 5% | 30 (5%) |
| Venomoth | 32 | 5% | 32 (5%) |
| Tauros | 25 | 4% | 25 (4%) |
| Kangaskhan | 28 | 1% | 28 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 5–15 | 60% | 5–15 (60%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |
| Poliwag | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 15–25 | 40% | 15–25 (40%) |
| Seaking | 20–30 | 40% | 20–30 (40%) |
| Dratini | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 4% | 15–35 (4%) |
| Dragonair | 25–35 | 1% | 25–35 (1%) |


<a id="seafoamislands-1f"></a>

## Seafoam Islands — 1 F

Fonte: tabela `sSeafoamIslands1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 26–27, 29, 31, 33 | 55% | 26 (1%); 27 (20%); 29 (20%); 31 (10%); 33 (4%) |
| Zubat | 22, 24, 26 | 34% | 22 (20%); 24 (10%); 26 (4%) |
| Golbat | 26, 28, 30 | 11% | 26 (5%); 28 (5%); 30 (1%) |


<a id="seafoamislands-b1f"></a>

## Seafoam Islands — B1 F

Fonte: tabela `sSeafoamIslandsB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 29, 31 | 40% | 29 (20%); 31 (20%) |
| Zubat | 22, 24, 26 | 34% | 22 (20%); 24 (10%); 26 (4%) |
| Golbat | 26, 28, 30 | 11% | 26 (5%); 28 (5%); 30 (1%) |
| Seel | 28 | 10% | 28 (10%) |
| Golduck | 33, 35 | 5% | 33 (4%); 35 (1%) |


<a id="seafoamislands-b2f"></a>

## Seafoam Islands — B2 F

Fonte: tabela `sSeafoamIslandsB2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 30, 32 | 40% | 30 (20%); 32 (20%) |
| Seel | 30, 32 | 20% | 30 (10%); 32 (10%) |
| Zubat | 22, 24 | 20% | 22 (10%); 24 (10%) |
| Golbat | 26, 28, 30 | 10% | 26 (5%); 28 (4%); 30 (1%) |
| Golduck | 32, 34 | 10% | 32 (5%); 34 (5%) |


<a id="seafoamislands-b3f"></a>

## Seafoam Islands — B3 F

Fonte: tabela `sSeafoamIslandsB3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 30, 32 | 40% | 30 (20%); 32 (20%) |
| Psyduck | 30, 32 | 20% | 30 (10%); 32 (10%) |
| Golduck | 32, 34 | 15% | 32 (10%); 34 (5%) |
| Golbat | 26, 28, 30 | 10% | 26 (5%); 28 (4%); 30 (1%) |
| Zubat | 24 | 10% | 24 (10%) |
| Dewgong | 32, 34 | 5% | 32 (4%); 34 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 25–35 | 60% | 25–35 (60%) |
| Horsea | 25–30 | 30% | 25–30 (30%) |
| Dewgong | 35–40 | 5% | 35–40 (5%) |
| Psyduck | 30–40 | 4% | 30–40 (4%) |
| Golduck | 35–40 | 1% | 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–30 | 80% | 15–25 (40%); 20–30 (40%) |
| Gyarados | 15–35 | 16% | 15–25 (15%); 25–35 (1%) |
| Psyduck | 15–25 | 4% | 15–25 (4%) |


<a id="seafoamislands-b4f"></a>

## Seafoam Islands — B4 F

Fonte: tabela `sSeafoamIslandsB4F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B4F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 30, 32, 34 | 50% | 30 (20%); 32 (20%); 34 (10%) |
| Golbat | 26, 28, 30 | 15% | 26 (10%); 28 (4%); 30 (1%) |
| Golduck | 32, 34 | 15% | 32 (10%); 34 (5%) |
| Dewgong | 34, 36 | 10% | 34 (5%); 36 (5%) |
| Psyduck | 32 | 10% | 32 (10%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seel | 25–35 | 60% | 25–35 (60%) |
| Horsea | 25–30 | 30% | 25–30 (30%) |
| Dewgong | 35–40 | 5% | 35–40 (5%) |
| Psyduck | 30–40 | 4% | 30–40 (4%) |
| Golduck | 35–40 | 1% | 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–30 | 80% | 15–25 (40%); 20–30 (40%) |
| Gyarados | 15–35 | 16% | 15–25 (15%); 25–35 (1%) |
| Psyduck | 15–25 | 4% | 15–25 (4%) |


<a id="sevenisland-sevaultcanyon"></a>

## Seven Island — Sevault Canyon

Fonte: tabela `sSevenIslandSevaultCanyon_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_SevaultCanyon`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 46 | 20% | 46 (20%) |
| Phanpy | 15 | 20% | 15 (20%) |
| Cubone | 46 | 10% | 46 (10%) |
| Fearow | 50 | 10% | 50 (10%) |
| Marowak | 52 | 10% | 52 (10%) |
| Meowth | 43 | 10% | 43 (10%) |
| Larvitar | 15, 20 | 5% | 15 (4%); 20 (1%) |
| Onix | 54 | 5% | 54 (5%) |
| Persian | 49, 52 | 5% | 49 (4%); 52 (1%) |
| Skarmory | 30 | 5% | 30 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 25–40 | 65% | 25–35 (60%); 30–40 (5%) |
| Graveler | 30–50 | 35% | 30–45 (30%); 35–50 (5%) |


<a id="sevenisland-sevaultcanyon-entrance"></a>

## Seven Island — Sevault Canyon — Entrance

Fonte: tabela `sSevenIslandSevaultCanyonEntrance_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_SevaultCanyon_Entrance`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sentret | 10, 15 | 30% | 10 (20%); 15 (10%) |
| Spearow | 44 | 20% | 44 (20%) |
| Fearow | 48, 50 | 15% | 48 (10%); 50 (5%) |
| Phanpy | 10, 15 | 15% | 10 (10%); 15 (5%) |
| Meowth | 41 | 10% | 41 (10%) |
| Persian | 47, 50 | 5% | 47 (4%); 50 (1%) |
| Psyduck | 41 | 5% | 41 (5%) |


<a id="sevenisland-tanobyruins"></a>

## Seven Island — Tanoby Ruins

Fonte: tabela `sSevenIslandTanobyRuins_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="sevenisland-tanobyruins-dilfordchamber"></a>

## Seven Island — Tanoby Ruins — Dilford Chamber

Fonte: tabela `sSevenIslandTanobyRuinsDilfordChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_DilfordChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown P | 25 | 40% | 25 (40%) |
| Unown J | 25 | 20% | 25 (20%) |
| Unown L | 25 | 20% | 25 (20%) |
| Unown R | 25 | 14% | 25 (14%) |
| Unown Q | 25 | 6% | 25 (6%) |


<a id="sevenisland-tanobyruins-liptoochamber"></a>

## Seven Island — Tanoby Ruins — Liptoo Chamber

Fonte: tabela `sSevenIslandTanobyRuinsLiptooChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_LiptooChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown C | 25 | 50% | 25 (50%) |
| Unown D | 25 | 30% | 25 (30%) |
| Unown H | 25 | 14% | 25 (14%) |
| Unown U | 25 | 5% | 25 (5%) |
| Unown O | 25 | 1% | 25 (1%) |


<a id="sevenisland-tanobyruins-moneanchamber"></a>

## Seven Island — Tanoby Ruins — Monean Chamber

Fonte: tabela `sSevenIslandTanobyRuinsMoneanChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_MoneanChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown A | 25 | 99% | 25 (99%) |
| Unown ? | 25 | 1% | 25 (1%) |


<a id="sevenisland-tanobyruins-rixychamber"></a>

## Seven Island — Tanoby Ruins — Rixy Chamber

Fonte: tabela `sSevenIslandTanobyRuinsRixyChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_RixyChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown V | 25 | 50% | 25 (50%) |
| Unown W | 25 | 30% | 25 (30%) |
| Unown X | 25 | 10% | 25 (10%) |
| Unown M | 25 | 8% | 25 (8%) |
| Unown B | 25 | 2% | 25 (2%) |


<a id="sevenisland-tanobyruins-scufibchamber"></a>

## Seven Island — Tanoby Ruins — Scufib Chamber

Fonte: tabela `sSevenIslandTanobyRuinsScufibChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_ScufibChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown Y | 25 | 40% | 25 (40%) |
| Unown G | 25 | 25% | 25 (25%) |
| Unown T | 25 | 20% | 25 (20%) |
| Unown F | 25 | 13% | 25 (13%) |
| Unown K | 25 | 2% | 25 (2%) |


<a id="sevenisland-tanobyruins-viapoischamber"></a>

## Seven Island — Tanoby Ruins — Viapois Chamber

Fonte: tabela `sSevenIslandTanobyRuinsViapoisChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_ViapoisChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown Z | 25 | 99% | 25 (99%) |
| Unown ! | 25 | 1% | 25 (1%) |


<a id="sevenisland-tanobyruins-weepthchamber"></a>

## Seven Island — Tanoby Ruins — Weepth Chamber

Fonte: tabela `sSevenIslandTanobyRuinsWeepthChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_WeepthChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown N | 25 | 60% | 25 (60%) |
| Unown S | 25 | 30% | 25 (30%) |
| Unown I | 25 | 8% | 25 (8%) |
| Unown E | 25 | 2% | 25 (2%) |


<a id="sevenisland-trainertower"></a>

## Seven Island — Trainer Tower

Fonte: tabela `sSevenIslandTrainerTower_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TrainerTower`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="sixisland-alteringcave"></a>

## Six Island — Altering Cave

Fonte: tabela `sSixIslandAlteringCave_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

Conjunto 0, disponível normalmente: Zubat. Outros conjuntos constam somente no apêndice de tabelas alternativas.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Zubat | 6, 8, 10, 12, 14, 16 | 100% | 6 (5%); 8 (15%); 10 (30%); 12 (30%); 14 (15%); 16 (5%) |


<a id="sixisland-greenpath"></a>

## Six Island — Green Path

Fonte: tabela `sSixIslandGreenPath_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_GreenPath`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="sixisland-outcastisland"></a>

## Six Island — Outcast Island

Fonte: tabela `sSixIslandOutcastIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_OutcastIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="sixisland-patternbush"></a>

## Six Island — Pattern Bush

Fonte: tabela `sSixIslandPatternBush_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_PatternBush`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Spinarak | 9, 14 | 30% | 9 (20%); 14 (10%) |
| Heracross | 15, 20, 25, 30 | 20% | 15 (10%); 20 (5%); 25 (4%); 30 (1%) |
| Kakuna | 9 | 20% | 9 (20%) |
| Caterpie | 6 | 10% | 6 (10%) |
| Weedle | 6 | 10% | 6 (10%) |
| Ledyba | 9, 14 | 5% | 9 (4%); 14 (1%) |
| Metapod | 9 | 5% | 9 (5%) |


<a id="sixisland-ruinvalley"></a>

## Six Island — Ruin Valley

Fonte: tabela `sSixIslandRuinValley_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_RuinValley`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Natu | 15, 20 | 25% | 15 (20%); 20 (5%) |
| Spearow | 44 | 20% | 44 (20%) |
| Fearow | 49 | 10% | 49 (10%) |
| Meowth | 43 | 10% | 43 (10%) |
| Wooper | 15 | 10% | 15 (10%) |
| Yanma | 18 | 10% | 18 (10%) |
| Persian | 49, 52 | 5% | 49 (4%); 52 (1%) |
| Psyduck | 41 | 5% | 41 (5%) |
| Wobbuffet | 25 | 5% | 25 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wooper | 5–25 | 100% | 5–20 (60%); 10–20 (30%); 15–25 (5%); 20–25 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="sixisland-waterpath"></a>

## Six Island — Water Path

Fonte: tabela `sSixIslandWaterPath_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_WaterPath`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sentret | 10, 15 | 30% | 10 (20%); 15 (10%) |
| Spearow | 44 | 20% | 44 (20%) |
| Fearow | 48, 50 | 15% | 48 (10%); 50 (5%) |
| Meowth | 41 | 10% | 41 (10%) |
| Oddish | 44 | 10% | 44 (10%) |
| Gloom | 48 | 5% | 48 (5%) |
| Persian | 47, 50 | 5% | 47 (4%); 50 (1%) |
| Psyduck | 41 | 5% | 41 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 40% | 15–25 (40%) |
| Qwilfish | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="threeisland-berryforest"></a>

## Three Island — Berry Forest

Fonte: tabela `sThreeIslandBerryForest_FireRed` de `game/src/data/wild_encounters.json`; mapa `ThreeIsland_BerryForest`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gloom | 35 | 20% | 35 (20%) |
| Pidgeotto | 37 | 20% | 37 (20%) |
| Drowzee | 34 | 10% | 34 (10%) |
| Oddish | 30 | 10% | 30 (10%) |
| Pidgey | 32 | 10% | 32 (10%) |
| Venonat | 34 | 10% | 34 (10%) |
| Exeggcute | 35 | 5% | 35 (5%) |
| Hypno | 37, 40 | 5% | 37 (4%); 40 (1%) |
| Psyduck | 31 | 5% | 31 (5%) |
| Venomoth | 37, 40 | 5% | 37 (4%); 40 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Golduck | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 5–15 | 60% | 5–15 (60%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |
| Poliwag | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Goldeen | 15–25 | 40% | 15–25 (40%) |
| Seaking | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="threeisland-bondbridge"></a>

## Three Island — Bond Bridge

Fonte: tabela `sThreeIslandBondBridge_FireRed` de `game/src/data/wild_encounters.json`; mapa `ThreeIsland_BondBridge`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pidgey | 29, 32 | 30% | 29 (10%); 32 (20%) |
| Oddish | 31 | 20% | 31 (20%) |
| Pidgeotto | 34, 37, 40 | 15% | 34 (10%); 37 (4%); 40 (1%) |
| Gloom | 36 | 10% | 36 (10%) |
| Meowth | 31 | 10% | 31 (10%) |
| Persian | 37, 40 | 5% | 37 (4%); 40 (1%) |
| Psyduck | 31 | 5% | 31 (5%) |
| Venonat | 34 | 5% | 34 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Tentacruel | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 80% | 5–15 (80%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–25 | 80% | 15–25 (80%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Seadra | 25–35 | 4% | 25–35 (4%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="threeisland-port"></a>

## Three Island — Port

Fonte: tabela `sThreeIslandPort_FireRed` de `game/src/data/wild_encounters.json`; mapa `ThreeIsland_Port`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Dunsparce | 5, 10, 15, 20, 25, 30, 35 | 100% | 5 (1%); 10 (20%); 15 (40%); 20 (20%); 25 (9%); 30 (9%); 35 (1%) |


<a id="twoisland-capebrink"></a>

## Two Island — Cape Brink

Fonte: tabela `sTwoIslandCapeBrink_FireRed` de `game/src/data/wild_encounters.json`; mapa `TwoIsland_CapeBrink`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Oddish | 30, 32 | 30% | 30 (20%); 32 (10%) |
| Spearow | 31 | 20% | 31 (20%) |
| Gloom | 36, 38 | 15% | 36 (10%); 38 (5%) |
| Fearow | 36 | 10% | 36 (10%) |
| Meowth | 31 | 10% | 31 (10%) |
| Golduck | 37, 40 | 5% | 37 (4%); 40 (1%) |
| Persian | 37, 40 | 5% | 37 (4%); 40 (1%) |
| Psyduck | 31 | 5% | 31 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 5–40 | 95% | 5–20 (60%); 20–35 (30%); 35–40 (5%) |
| Golduck | 35–40 | 5% | 35–40 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="vermilioncity"></a>

## Vermilion City

Fonte: tabela `sVermilionCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `VermilionCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tentacool | 5–40 | 100% | 5–10 (60%); 10–20 (30%); 20–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 5–15 | 60% | 5–15 (60%) |
| Krabby | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 15–35 | 44% | 15–25 (40%); 25–35 (4%) |
| Shellder | 15–25 | 40% | 15–25 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 25–35 | 1% | 25–35 (1%) |


<a id="victoryroad-1f"></a>

## Victory Road — 1 F

Fonte: tabela `sVictoryRoad1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `VictoryRoad_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Onix | 40, 43, 46 | 30% | 40 (10%); 43 (10%); 46 (10%) |
| Geodude | 32 | 20% | 32 (20%) |
| Machop | 32 | 20% | 32 (20%) |
| Zubat | 32 | 10% | 32 (10%) |
| Arbok | 44 | 5% | 44 (5%) |
| Golbat | 44 | 5% | 44 (5%) |
| Machoke | 44, 46 | 5% | 44 (4%); 46 (1%) |
| Marowak | 44, 46 | 5% | 44 (4%); 46 (1%) |


<a id="victoryroad-2f"></a>

## Victory Road — 2 F

Fonte: tabela `sVictoryRoad2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `VictoryRoad_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Geodude | 34 | 20% | 34 (20%) |
| Machop | 34 | 20% | 34 (20%) |
| Onix | 45, 48 | 20% | 45 (10%); 48 (10%) |
| Primeape | 42 | 10% | 42 (10%) |
| Zubat | 34 | 10% | 34 (10%) |
| Arbok | 46 | 5% | 46 (5%) |
| Golbat | 46 | 5% | 46 (5%) |
| Machoke | 46, 48 | 5% | 46 (4%); 48 (1%) |
| Marowak | 46, 48 | 5% | 46 (4%); 48 (1%) |


<a id="victoryroad-3f"></a>

## Victory Road — 3 F

Fonte: tabela `sVictoryRoad3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `VictoryRoad_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Onix | 40, 43, 46 | 30% | 40 (10%); 43 (10%); 46 (10%) |
| Geodude | 32 | 20% | 32 (20%) |
| Machop | 32 | 20% | 32 (20%) |
| Zubat | 32 | 10% | 32 (10%) |
| Arbok | 44 | 5% | 44 (5%) |
| Golbat | 44 | 5% | 44 (5%) |
| Machoke | 44, 46 | 5% | 44 (4%); 46 (1%) |
| Marowak | 44, 46 | 5% | 44 (4%); 46 (1%) |


<a id="viridiancity"></a>

## Viridian City

Fonte: tabela `sViridianCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `ViridianCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Psyduck | 20–40 | 100% | 20–25 (90%); 25–30 (5%); 30–35 (4%); 35–40 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 5 | 100% | 5 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 5–15 | 60% | 5–15 (60%) |
| Goldeen | 5–15 | 20% | 5–15 (20%) |
| Magikarp | 5–15 | 20% | 5–15 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwag | 15–25 | 40% | 15–25 (40%) |
| Poliwhirl | 20–30 | 40% | 20–30 (40%) |
| Gyarados | 15–25 | 15% | 15–25 (15%) |
| Psyduck | 15–35 | 5% | 15–25 (4%); 25–35 (1%) |


<a id="viridianforest"></a>

## Viridian Forest

Fonte: tabela `sViridianForest_FireRed` de `game/src/data/wild_encounters.json`; mapa `ViridianForest`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Caterpie | 3–5 | 40% | 3 (10%); 4 (20%); 5 (10%) |
| Weedle | 3–5 | 40% | 3 (10%); 4 (20%); 5 (10%) |
| Kakuna | 4–6 | 10% | 4 (4%); 5 (5%); 6 (1%) |
| Metapod | 5 | 5% | 5 (5%) |
| Pikachu | 3, 5 | 5% | 3 (4%); 5 (1%) |

## Encontros fixos e eventos

A espécie é fixa (100% ao acionar o evento disponível); isso não garante captura. As flags de história, itens e derrotas anteriores continuam necessárias. A nova chance para lendários derrotados ainda está pendente.

| Local | Pokémon | Nível | Condição / disponibilidade |
|---|---|---:|---|
| Birth Island — Exterior | Deoxys | 30 | **Área de evento**: requer acesso por ticket/flags correspondentes; esta hack ainda não fornece novo acesso normal a esses eventos. |
| Cerulean Cave — B1 F | Mewtwo | 70 | Pós-jogo; concluir a rede de Celio libera Cerulean Cave. |
| Mt Ember — Summit | Moltres | 50 | Encontro fixo de história; uma oportunidade por objeto/evento. |
| Navel Rock — Base | Lugia | 70 | **Área de evento**: requer acesso por ticket/flags correspondentes; esta hack ainda não fornece novo acesso normal a esses eventos. |
| Navel Rock — Summit | Ho-Oh | 70 | **Área de evento**: requer acesso por ticket/flags correspondentes; esta hack ainda não fornece novo acesso normal a esses eventos. |
| Pokemon Tower — 6 F | Marowak | 30 | **Não capturável**: fantasma de história, exige Silph Scope. |
| Power Plant | Zapdos | 50 | Encontro fixo de história; uma oportunidade por objeto/evento. |
| Power Plant | Electrode | 34 | Poké Ball falsa; duas ocorrências na Power Plant, listadas separadamente. |
| Power Plant | Electrode | 34 | Poké Ball falsa; duas ocorrências na Power Plant, listadas separadamente. |
| Route 12 | Snorlax | 30 | Exige Poké Flute; um encontro nesta rota. |
| Route 16 | Snorlax | 30 | Exige Poké Flute; um encontro nesta rota. |
| Seafoam Islands — B4 F | Articuno | 50 | Encontro fixo de história; uma oportunidade por objeto/evento. |
| Three Island — Berry Forest | Hypno | 30 | Encontro fixo de história; uma oportunidade por objeto/evento. |

## Lendário errante

Após concluir a rede de Celio (entrega da Sapphire), a base ativa um único errante de nível 50: **Entei** se você escolheu Bulbasaur, **Suicune** se escolheu Charmander, ou **Raikou** se escolheu Squirtle. Ele se desloca pelas rotas de Kanto (1–25; Route 21 North, não South).

Quando ele está ativo e no mesmo mapa do jogador, o teste de substituição tem **25%** de chance nominal, após a verificação de encontro e antes de gerar o selvagem comum. Não é 25% em cada rota em todos os momentos: sua localização muda e Repel ainda pode impedir a batalha. Pesca e Rock Smash não usam essa substituição. Fonte: `game/src/roamer.c` e `game/src/wild_encounter.c`.

## Presentes, compras, fósseis e trocas

Recebimentos não são sorteios de grama: a espécie é garantida quando você escolhe/paga/cumpre os requisitos (não uma probabilidade de encontro). Escolhas exclusivas e pré-requisitos originais continuam vigentes. No Hardcore, presentes e trocas por NPC ficam fora da cota de selvagens.

| Local | Recebimento | Nível | Requisito |
|---|---|---:|---|
| Laboratório de Oak | Bulbasaur / Charmander / Squirtle | 5 | Escolher um inicial |
| Route 4 — Centro Pokémon | Magikarp | 5 | Comprar por ₽500 |
| Celadon — Condominiums, Roof Room | Eevee | 25 | Presente único |
| Saffron — Fighting Dojo | Hitmonlee / Hitmonchan | 25 | Vencer Koichi; escolher um |
| Silph Co. — 7F | Lapras | 25 | Presente do funcionário |
| Cinnabar — laboratório, Experiment Room | Omanyte / Kabuto / Aerodactyl | 5 | Entregar Helix Fossil / Dome Fossil / Old Amber correspondente |
| Five Island — Water Labyrinth | Ovo de Togepi | 5 ao nascer | Mostrar ao idoso um líder com amizade alta e ter espaço |
| Celadon — Game Corner | Abra | 9 | 180 moedas |
| Celadon — Game Corner | Clefairy | 8 | 500 moedas |
| Celadon — Game Corner | Dratini | 18 | 2.800 moedas |
| Celadon — Game Corner | Scyther | 25 | 5.500 moedas |
| Celadon — Game Corner | Porygon | 26 | 9.999 moedas |

Trocas por NPC, com nível igual ao Pokémon entregue: Route 2 House, Abra → Mr. Mime; Cerulean House 3, Poliwhirl → Jynx; Underground Path North Entrance, Nidoran♂ → Nidoran♀; Vermilion House 2, Spearow → Farfetch’d; Route 11 East Entrance 2F, Nidorino → Nidorina; Route 18 East Entrance 2F, Golduck → Lickitung; Cinnabar Lab Lounge, Raichu → Electrode e Venonat → Tangela; Cinnabar Lab Experiment Room, Ponyta → Seel. Fonte: `game/src/data/ingame_trades.h` e scripts dos mapas.

## Altering Cave — tabelas alternativas inativas

O jogo usa o conjunto 0 (Zubat) por padrão. Os oito conjuntos abaixo existem na base e dependem de `VAR_ALTERING_CAVE_WILD_SET`; não foi implementado evento normal para selecioná-los. Sua presença nos fontes **não significa que essas espécies estejam obtíveis aqui nesta release**. Estas tabelas não se somam à tabela de Zubat.


### sSixIslandAlteringCave_2_FireRed

Fonte: tabela `sSixIslandAlteringCave_2_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mareep | 3, 5, 7, 9, 11, 13 | 100% | 3 (5%); 5 (15%); 7 (30%); 9 (30%); 11 (15%); 13 (5%) |


### sSixIslandAlteringCave_3_FireRed

Fonte: tabela `sSixIslandAlteringCave_3_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pineco | 19, 21–23, 25, 27, 29 | 100% | 19 (5%); 21 (5%); 22 (10%); 23 (30%); 25 (30%); 27 (15%); 29 (5%) |


### sSixIslandAlteringCave_4_FireRed

Fonte: tabela `sSixIslandAlteringCave_4_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Houndour | 12, 14, 16, 18, 20, 22 | 100% | 12 (5%); 14 (15%); 16 (30%); 18 (30%); 20 (15%); 22 (5%) |


### sSixIslandAlteringCave_5_FireRed

Fonte: tabela `sSixIslandAlteringCave_5_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Teddiursa | 18, 20, 22, 24, 26, 28 | 100% | 18 (5%); 20 (15%); 22 (30%); 24 (30%); 26 (15%); 28 (5%) |


### sSixIslandAlteringCave_6_FireRed

Fonte: tabela `sSixIslandAlteringCave_6_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Aipom | 18, 20, 22, 24, 26, 28 | 100% | 18 (5%); 20 (15%); 22 (30%); 24 (30%); 26 (15%); 28 (5%) |


### sSixIslandAlteringCave_7_FireRed

Fonte: tabela `sSixIslandAlteringCave_7_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Shuckle | 18, 20, 22, 24, 26, 28 | 100% | 18 (5%); 20 (15%); 22 (30%); 24 (30%); 26 (15%); 28 (5%) |


### sSixIslandAlteringCave_8_FireRed

Fonte: tabela `sSixIslandAlteringCave_8_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Stantler | 18, 20, 22, 24, 26, 28 | 100% | 18 (5%); 20 (15%); 22 (30%); 24 (30%); 26 (15%); 28 (5%) |


### sSixIslandAlteringCave_9_FireRed

Fonte: tabela `sSixIslandAlteringCave_9_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Smeargle | 18, 20, 22, 24, 26, 28 | 100% | 18 (5%); 20 (15%); 22 (30%); 24 (30%); 26 (15%); 28 (5%) |

## Cobertura e atualização

124 mapas com tabelas comuns de FireRed, 26 entradas de rotas (Route 21 separada em Norte/Sul), 8 tabelas alternativas de Altering Cave e 13 ocorrências fixas/eventos catalogadas. As tabelas de LeafGreen não entram neste guia. Áreas sem tabela aleatória não recebem encontros inventados.

Gerar novamente após mudar treinadores ou encontros: `python3 scripts/generate_guides.py`. Conferir sem escrever: `python3 scripts/generate_guides.py --check`. O gerador valida soma de 100% por método, número de slots, separação das varas, formas de Unown e duplicação somente em Altering Cave. Não altera a ROM.
