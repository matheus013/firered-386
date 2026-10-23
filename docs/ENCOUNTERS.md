# Encontros por rota e área — FireRed 386

Guia dos encontros válido para v0.4.0-dev com randomizer desligado. A distribuição foi introduzida na v0.3.0-dev: as 386 espécies estão distribuídas e podem ser capturadas diretamente no modo Normal, sem troca, reprodução ou tickets de evento. Veja também o [índice das 386 espécies](SPECIES_LOCATIONS.md). No Hardcore, os mesmos encontros existem, mas a cota por seção limita as capturas da tentativa; não é possível prometer a coleção completa sob essa regra.

## Como ler as probabilidades

- **Chance** é a participação da espécie na tabela, condicionada a ocorrer um encontro pelo método indicado. Não é chance por passo, por lançamento de vara ou de captura.
- Grama/caverna, Surf, Rock Smash, Old Rod, Good Rod e Super Rod têm tabelas separadas. Cada uma soma 100%; slots repetidos da mesma espécie foram somados.
- A coluna **Níveis por slot** informa a faixa e seu peso dentro da tabela. Uma faixa como `3–5 (20%)` significa que esse slot pesa 20%; o nível é sorteado na faixa. Slots sobrepostos não significam distribuição uniforme entre todos os níveis da espécie.
- Repel, taxa da área, deslocamento, itens/habilidades e estado do jogo afetam se a batalha ocorre. As porcentagens abaixo são os pesos nominais dos slots do jogo; não incluem o pequeno viés do sorteio por módulo do gerador aleatório.
- Em Pokémon Tower, o Silph Scope é necessário para identificar/capturar os fantasmas comuns. Nas câmaras Tanoby, é necessário resolver Tanoby Key; as formas de Unown são discriminadas.
- Os 21 lendários e míticos têm novos encontros aleatórios de 1% em áreas de pós-jogo, com trava pela primeira vitória na Liga. Encontros fixos originais continuam existindo; lendários derrotados em eventos também podem ser procurados nos novos encontros aleatórios.
- Roamer, batalhas fixas e eventos aparecem em seções próprias. Tabelas alternativas de Altering Cave estão separadas das acessíveis normalmente. Tabelas de terra sem terreno de encontro no layout recebem aviso explícito; a presença de dados não cria grama onde ela não existe.

Os caminhos `game/...` citados abaixo apontam para os fontes completos incluídos neste repositório. O patch cumulativo alternativo está em [patches/current.patch](../patches/current.patch).

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
| Pidgey | 3–6 | 20% | 3–6 (20%) |
| Rattata | 3–6 | 20% | 3–6 (20%) |
| Poochyena | 3–6 | 10% | 3–6 (10%) |
| Sentret | 3–6 | 10% | 3–6 (10%) |
| Taillow | 3–6 | 10% | 3–6 (10%) |
| Zigzagoon | 3–6 | 10% | 3–6 (10%) |
| Hoothoot | 3–6 | 5% | 3–6 (5%) |
| Spearow | 3–6 | 5% | 3–6 (5%) |
| Sunkern | 3–6 | 4% | 3–6 (4%) |
| Wurmple | 3–6 | 4% | 3–6 (4%) |
| Hoppip | 3–6 | 1% | 3–6 (1%) |
| Pichu | 3–6 | 1% | 3–6 (1%) |


<a id="route2"></a>

## Route 2

Fonte: tabela `sRoute2_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route2`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pidgey | 5–12 | 20% | 5–12 (20%) |
| Rattata | 5–12 | 20% | 5–12 (20%) |
| Caterpie | 5–12 | 10% | 5–12 (10%) |
| Lotad | 5–12 | 10% | 5–12 (10%) |
| Seedot | 5–12 | 10% | 5–12 (10%) |
| Weedle | 5–12 | 10% | 5–12 (10%) |
| Bellsprout | 5–12 | 5% | 5–12 (5%) |
| Oddish | 5–12 | 5% | 5–12 (5%) |
| Ralts | 5–12 | 4% | 5–12 (4%) |
| Surskit | 5–12 | 4% | 5–12 (4%) |
| Nidoran♀ | 5–12 | 1% | 5–12 (1%) |
| Nidoran♂ | 5–12 | 1% | 5–12 (1%) |


<a id="route3"></a>

## Route 3

Fonte: tabela `sRoute3_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route3`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Delibird | 17–25 | 20% | 17–25 (20%) |
| Koffing | 17–25 | 20% | 17–25 (20%) |
| Grimer | 17–25 | 10% | 17–25 (10%) |
| Lombre | 17–25 | 10% | 17–25 (10%) |
| Pidgeotto | 18–25 | 10% | 18–25 (10%) |
| Skiploom | 18–25 | 10% | 18–25 (10%) |
| Charmander | 17–25 | 5% | 17–25 (5%) |
| Totodile | 17–25 | 5% | 17–25 (5%) |
| Machop | 17–25 | 4% | 17–25 (4%) |
| Poliwag | 17–25 | 4% | 17–25 (4%) |
| Snubbull | 17–25 | 1% | 17–25 (1%) |
| Spheal | 17–25 | 1% | 17–25 (1%) |


<a id="route4"></a>

## Route 4

Fonte: tabela `sRoute4_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route4`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Drowzee | 17–25 | 20% | 17–25 (20%) |
| Voltorb | 17–25 | 20% | 17–25 (20%) |
| Kirlia | 20–25 | 10% | 20–25 (10%) |
| Nuzleaf | 17–25 | 10% | 17–25 (10%) |
| Psyduck | 17–25 | 10% | 17–25 (10%) |
| Seel | 17–25 | 10% | 17–25 (10%) |
| Squirtle | 17–25 | 5% | 17–25 (5%) |
| Torchic | 17–25 | 5% | 17–25 (5%) |
| Electrike | 17–25 | 4% | 17–25 (4%) |
| Snorunt | 17–25 | 4% | 17–25 (4%) |
| Mareep | 17–25 | 1% | 17–25 (1%) |
| Wingull | 17–25 | 1% | 17–25 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 54–59 | 60% | 54–59 (60%) |
| Wailmer | 54–59 | 30% | 54–59 (30%) |
| Vaporeon | 54–59 | 9% | 54–59 (9%) |
| Mantine | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 27–33 | 70% | 27–33 (70%) |
| Carvanha | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 60% | 44–50 (60%) |
| Kabutops | 44–50 | 20% | 44–50 (20%) |
| Lanturn | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 44–50 | 40% | 44–50 (40%) |
| Sealeo | 44–50 | 40% | 44–50 (40%) |
| Gorebyss | 44–50 | 15% | 44–50 (15%) |
| Ludicolo | 44–50 | 4% | 44–50 (4%) |
| Qwilfish | 44–50 | 1% | 44–50 (1%) |


<a id="route5"></a>

## Route 5

Fonte: tabela `sRoute5_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route5`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slugma | 27–33 | 20% | 27–33 (20%) |
| Togepi | 27–33 | 20% | 27–33 (20%) |
| Beautifly | 27–33 | 10% | 27–33 (10%) |
| Beedrill | 27–33 | 10% | 27–33 (10%) |
| Gloom | 27–33 | 10% | 27–33 (10%) |
| Weepinbell | 27–33 | 10% | 27–33 (10%) |
| Flaaffy | 27–33 | 5% | 27–33 (5%) |
| Magby | 27–33 | 5% | 27–33 (5%) |
| Loudred | 27–33 | 4% | 27–33 (4%) |
| Nidorino | 27–33 | 4% | 27–33 (4%) |
| Meowth | 27–33 | 1% | 27–33 (1%) |
| Pidgey | 27–33 | 1% | 27–33 (1%) |


<a id="route6"></a>

## Route 6

Fonte: tabela `sRoute6_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route6`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Meowth | 27–33 | 25% | 27–33 (25%) |
| Whismur | 27–33 | 20% | 27–33 (20%) |
| Butterfree | 27–33 | 10% | 27–33 (10%) |
| Dustox | 27–33 | 10% | 27–33 (10%) |
| Ledian | 27–33 | 10% | 27–33 (10%) |
| Mawile | 27–33 | 10% | 27–33 (10%) |
| Elekid | 27–33 | 5% | 27–33 (5%) |
| Nidorina | 27–33 | 5% | 27–33 (5%) |
| Growlithe | 27–33 | 4% | 27–33 (4%) |
| Pidgey | 27–33 | 1% | 27–33 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 60% | 54–59 (60%) |
| Pelipper | 54–59 | 30% | 54–59 (30%) |
| Crawdaunt | 54–59 | 5% | 54–59 (5%) |
| Swampert | 54–59 | 4% | 54–59 (4%) |
| Starmie | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 27–33 | 70% | 27–33 (70%) |
| Goldeen | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 44–50 | 60% | 44–50 (60%) |
| Azumarill | 44–50 | 20% | 44–50 (20%) |
| Whiscash | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 44–50 | 40% | 44–50 (40%) |
| Poliwhirl | 44–50 | 40% | 44–50 (40%) |
| Mantine | 44–50 | 19% | 44–50 (19%) |
| Sharpedo | 44–50 | 1% | 44–50 (1%) |


<a id="route7"></a>

## Route 7

Fonte: tabela `sRoute7_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route7`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sunflora | 35–41 | 20% | 35–41 (20%) |
| Togetic | 35–41 | 20% | 35–41 (20%) |
| Ariados | 35–41 | 10% | 35–41 (10%) |
| Minun | 35–41 | 10% | 35–41 (10%) |
| Sealeo | 35–41 | 10% | 35–41 (10%) |
| Yanma | 35–41 | 10% | 35–41 (10%) |
| Arbok | 35–41 | 5% | 35–41 (5%) |
| Persian | 35–41 | 5% | 35–41 (5%) |
| Azumarill | 35–41 | 4% | 35–41 (4%) |
| Illumise | 35–41 | 4% | 35–41 (4%) |
| Furret | 35–41 | 1% | 35–41 (1%) |
| Metang | 35–41 | 1% | 35–41 (1%) |


<a id="route8"></a>

## Route 8

Fonte: tabela `sRoute8_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route8`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Castform | 35–41 | 20% | 35–41 (20%) |
| Wynaut | 35–41 | 20% | 35–41 (20%) |
| Ledian | 35–41 | 10% | 35–41 (10%) |
| Machoke | 35–41 | 10% | 35–41 (10%) |
| Parasect | 35–41 | 10% | 35–41 (10%) |
| Plusle | 35–41 | 10% | 35–41 (10%) |
| Fearow | 35–41 | 5% | 35–41 (5%) |
| Pelipper | 35–41 | 5% | 35–41 (5%) |
| Sunflora | 35–41 | 4% | 35–41 (4%) |
| Tangela | 35–41 | 4% | 35–41 (4%) |
| Dunsparce | 35–41 | 1% | 35–41 (1%) |
| Shelgon | 35–41 | 1% | 35–41 (1%) |


<a id="route9"></a>

## Route 9

Fonte: tabela `sRoute9_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route9`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Arbok | 35–41 | 20% | 35–41 (20%) |
| Porygon | 35–41 | 20% | 35–41 (20%) |
| Delcatty | 35–41 | 10% | 35–41 (10%) |
| Haunter | 35–41 | 10% | 35–41 (10%) |
| Murkrow | 35–41 | 10% | 35–41 (10%) |
| Roselia | 35–41 | 10% | 35–41 (10%) |
| Kecleon | 35–41 | 5% | 35–41 (5%) |
| Vigoroth | 35–41 | 5% | 35–41 (5%) |
| Volbeat | 35–41 | 4% | 35–41 (4%) |
| Wigglytuff | 35–41 | 4% | 35–41 (4%) |
| Mightyena | 35–41 | 1% | 35–41 (1%) |
| Raticate | 35–41 | 1% | 35–41 (1%) |


<a id="route10"></a>

## Route 10

Fonte: tabela `sRoute10_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route10`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Dunsparce | 35–41 | 20% | 35–41 (20%) |
| Vibrava | 35–41 | 20% | 35–41 (20%) |
| Minun | 35–41 | 10% | 35–41 (10%) |
| Murkrow | 35–41 | 10% | 35–41 (10%) |
| Togetic | 35–41 | 10% | 35–41 (10%) |
| Wynaut | 35–41 | 10% | 35–41 (10%) |
| Misdreavus | 35–41 | 5% | 35–41 (5%) |
| Sneasel | 35–41 | 5% | 35–41 (5%) |
| Castform | 35–41 | 4% | 35–41 (4%) |
| Linoone | 35–41 | 4% | 35–41 (4%) |
| Bayleef | 35–41 | 1% | 35–41 (1%) |
| Medicham | 37–41 | 1% | 37–41 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 60% | 54–59 (60%) |
| Seadra | 54–59 | 30% | 54–59 (30%) |
| Kingdra | 54–59 | 6% | 54–59 (6%) |
| Ludicolo | 54–59 | 4% | 54–59 (4%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 35–41 | 70% | 35–41 (70%) |
| Qwilfish | 35–41 | 30% | 35–41 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 44–50 | 60% | 44–50 (60%) |
| Ludicolo | 44–50 | 20% | 44–50 (20%) |
| Relicanth | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 44–50 | 40% | 44–50 (40%) |
| Wailmer | 44–50 | 40% | 44–50 (40%) |
| Dewgong | 44–50 | 15% | 44–50 (15%) |
| Gorebyss | 44–50 | 4% | 44–50 (4%) |
| Ludicolo | 44–50 | 1% | 44–50 (1%) |


<a id="route11"></a>

## Route 11

Fonte: tabela `sRoute11_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route11`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Ekans | 27–33 | 25% | 27–33 (25%) |
| Igglybuff | 27–33 | 20% | 27–33 (20%) |
| Farfetch’d | 27–33 | 10% | 27–33 (10%) |
| Lickitung | 27–33 | 10% | 27–33 (10%) |
| Porygon | 27–33 | 10% | 27–33 (10%) |
| Yanma | 27–33 | 10% | 27–33 (10%) |
| Aipom | 27–33 | 5% | 27–33 (5%) |
| Drowzee | 27–33 | 5% | 27–33 (5%) |
| Spinda | 27–33 | 5% | 27–33 (5%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 60% | 54–59 (60%) |
| Qwilfish | 54–59 | 30% | 54–59 (30%) |
| Starmie | 54–59 | 5% | 54–59 (5%) |
| Blastoise | 54–59 | 4% | 54–59 (4%) |
| Crawdaunt | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 27–33 | 70% | 27–33 (70%) |
| Corphish | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 44–50 | 80% | 44–50 (80%) |
| Seaking | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 40% | 44–50 (40%) |
| Wailmer | 44–50 | 40% | 44–50 (40%) |
| Quagsire | 44–50 | 15% | 44–50 (15%) |
| Kingler | 44–50 | 4% | 44–50 (4%) |
| Wartortle | 44–50 | 1% | 44–50 (1%) |


<a id="route12"></a>

## Route 12

Fonte: tabela `sRoute12_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route12`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cacturne | 44–50 | 20% | 44–50 (20%) |
| Rhydon | 44–50 | 20% | 44–50 (20%) |
| Dusclops | 44–50 | 10% | 44–50 (10%) |
| Linoone | 44–50 | 10% | 44–50 (10%) |
| Manectric | 44–50 | 10% | 44–50 (10%) |
| Tauros | 44–50 | 10% | 44–50 (10%) |
| Clefable | 44–50 | 5% | 44–50 (5%) |
| Swalot | 44–50 | 5% | 44–50 (5%) |
| Hitmontop | 44–50 | 4% | 44–50 (4%) |
| Quilava | 44–50 | 4% | 44–50 (4%) |
| Armaldo | 44–50 | 1% | 44–50 (1%) |
| Gloom | 44–50 | 1% | 44–50 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 90% | 54–59 (90%) |
| Huntail | 54–59 | 5% | 54–59 (5%) |
| Golduck | 54–59 | 4% | 54–59 (4%) |
| Poliwrath | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 70% | 44–50 (70%) |
| Corsola | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 44–50 | 60% | 44–50 (60%) |
| Kingler | 44–50 | 20% | 44–50 (20%) |
| Sharpedo | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 40% | 44–50 (40%) |
| Corsola | 44–50 | 40% | 44–50 (40%) |
| Gorebyss | 44–50 | 15% | 44–50 (15%) |
| Sharpedo | 44–50 | 4% | 44–50 (4%) |
| Kabutops | 44–50 | 1% | 44–50 (1%) |


<a id="route13"></a>

## Route 13

Fonte: tabela `sRoute13_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route13`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Misdreavus | 44–50 | 20% | 44–50 (20%) |
| Tropius | 44–50 | 20% | 44–50 (20%) |
| Electabuzz | 44–50 | 10% | 44–50 (10%) |
| Jumpluff | 44–50 | 10% | 44–50 (10%) |
| Tangela | 44–50 | 10% | 44–50 (10%) |
| Torkoal | 44–50 | 10% | 44–50 (10%) |
| Forretress | 44–50 | 5% | 44–50 (5%) |
| Hariyama | 44–50 | 5% | 44–50 (5%) |
| Ivysaur | 44–50 | 4% | 44–50 (4%) |
| Primeape | 44–50 | 4% | 44–50 (4%) |
| Arbok | 44–50 | 1% | 44–50 (1%) |
| Gloom | 44–50 | 1% | 44–50 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 54–59 | 90% | 54–59 (90%) |
| Lanturn | 54–59 | 5% | 54–59 (5%) |
| Poliwrath | 54–59 | 4% | 54–59 (4%) |
| Relicanth | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 44–50 | 70% | 44–50 (70%) |
| Corsola | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 44–50 | 60% | 44–50 (60%) |
| Crawdaunt | 44–50 | 20% | 44–50 (20%) |
| Qwilfish | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 40% | 44–50 (40%) |
| Seadra | 44–50 | 40% | 44–50 (40%) |
| Kingler | 44–50 | 15% | 44–50 (15%) |
| Seaking | 44–50 | 4% | 44–50 (4%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="route14"></a>

## Route 14

Fonte: tabela `sRoute14_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route14`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Raticate | 44–50 | 20% | 44–50 (20%) |
| Vigoroth | 44–50 | 20% | 44–50 (20%) |
| Dodrio | 44–50 | 10% | 44–50 (10%) |
| Girafarig | 44–50 | 10% | 44–50 (10%) |
| Hypno | 44–50 | 10% | 44–50 (10%) |
| Sunflora | 44–50 | 10% | 44–50 (10%) |
| Armaldo | 44–50 | 5% | 44–50 (5%) |
| Manectric | 44–50 | 5% | 44–50 (5%) |
| Granbull | 44–50 | 4% | 44–50 (4%) |
| Seviper | 44–50 | 4% | 44–50 (4%) |
| Bagon | 44–50 | 1% | 44–50 (1%) |
| Ditto | 44–50 | 1% | 44–50 (1%) |


<a id="route15"></a>

## Route 15

Fonte: tabela `sRoute15_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route15`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Illumise | 44–50 | 20% | 44–50 (20%) |
| Pupitar | 44–50 | 20% | 44–50 (20%) |
| Camerupt | 44–50 | 10% | 44–50 (10%) |
| Castform | 44–50 | 10% | 44–50 (10%) |
| Kabutops | 44–50 | 10% | 44–50 (10%) |
| Linoone | 44–50 | 10% | 44–50 (10%) |
| Electabuzz | 44–50 | 5% | 44–50 (5%) |
| Torkoal | 44–50 | 5% | 44–50 (5%) |
| Golbat | 44–50 | 4% | 44–50 (4%) |
| Venomoth | 44–50 | 4% | 44–50 (4%) |
| Gloom | 44–50 | 1% | 44–50 (1%) |
| Larvitar | 44–50 | 1% | 44–50 (1%) |


<a id="route16"></a>

## Route 16

Fonte: tabela `sRoute16_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route16`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Machoke | 44–50 | 20% | 44–50 (20%) |
| Manectric | 44–50 | 20% | 44–50 (20%) |
| Cacturne | 44–50 | 10% | 44–50 (10%) |
| Dugtrio | 44–50 | 10% | 44–50 (10%) |
| Miltank | 44–50 | 10% | 44–50 (10%) |
| Wobbuffet | 44–50 | 10% | 44–50 (10%) |
| Glalie | 44–50 | 5% | 44–50 (5%) |
| Magmar | 44–50 | 5% | 44–50 (5%) |
| Hitmonlee | 44–50 | 4% | 44–50 (4%) |
| Ninjask | 44–50 | 4% | 44–50 (4%) |
| Smeargle | 44–50 | 1% | 44–50 (1%) |
| Solrock | 44–50 | 1% | 44–50 (1%) |


<a id="route17"></a>

## Route 17

Fonte: tabela `sRoute17_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route17`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Girafarig | 44–50 | 20% | 44–50 (20%) |
| Haunter | 44–50 | 20% | 44–50 (20%) |
| Raticate | 44–50 | 20% | 44–50 (20%) |
| Illumise | 44–50 | 10% | 44–50 (10%) |
| Togetic | 44–50 | 10% | 44–50 (10%) |
| Altaria | 44–50 | 5% | 44–50 (5%) |
| Raichu | 44–50 | 5% | 44–50 (5%) |
| Hitmonchan | 44–50 | 4% | 44–50 (4%) |
| Skarmory | 44–50 | 4% | 44–50 (4%) |
| Fearow | 44–50 | 1% | 44–50 (1%) |
| Wynaut | 44–50 | 1% | 44–50 (1%) |


<a id="route18"></a>

## Route 18

Fonte: tabela `sRoute18_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route18`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Forretress | 44–50 | 20% | 44–50 (20%) |
| Raticate | 44–50 | 20% | 44–50 (20%) |
| Seviper | 44–50 | 20% | 44–50 (20%) |
| Fearow | 44–50 | 10% | 44–50 (10%) |
| Lairon | 44–50 | 10% | 44–50 (10%) |
| Electrode | 44–50 | 5% | 44–50 (5%) |
| Weezing | 44–50 | 5% | 44–50 (5%) |
| Magneton | 44–50 | 4% | 44–50 (4%) |
| Masquerain | 44–50 | 4% | 44–50 (4%) |
| Ditto | 44–50 | 1% | 44–50 (1%) |
| Sneasel | 44–50 | 1% | 44–50 (1%) |


<a id="route19"></a>

## Route 19

Fonte: tabela `sRoute19_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route19`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 90% | 54–59 (90%) |
| Relicanth | 54–59 | 5% | 54–59 (5%) |
| Milotic | 54–59 | 4% | 54–59 (4%) |
| Swampert | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 70% | 54–59 (70%) |
| Qwilfish | 54–59 | 30% | 54–59 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 54–59 | 60% | 54–59 (60%) |
| Relicanth | 54–59 | 20% | 54–59 (20%) |
| Sharpedo | 54–59 | 20% | 54–59 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 40% | 54–59 (40%) |
| Quagsire | 54–59 | 40% | 54–59 (40%) |
| Mantine | 54–59 | 15% | 54–59 (15%) |
| Seadra | 54–59 | 4% | 54–59 (4%) |
| Cloyster | 54–59 | 1% | 54–59 (1%) |


<a id="route20"></a>

## Route 20

Fonte: tabela `sRoute20_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route20`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 90% | 54–59 (90%) |
| Gorebyss | 54–59 | 5% | 54–59 (5%) |
| Relicanth | 54–59 | 4% | 54–59 (4%) |
| Cloyster | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 70% | 54–59 (70%) |
| Seadra | 54–59 | 30% | 54–59 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 60% | 54–59 (60%) |
| Kabutops | 54–59 | 20% | 54–59 (20%) |
| Kingler | 54–59 | 20% | 54–59 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 40% | 54–59 (40%) |
| Sealeo | 54–59 | 40% | 54–59 (40%) |
| Whiscash | 54–59 | 15% | 54–59 (15%) |
| Starmie | 54–59 | 4% | 54–59 (4%) |
| Poliwrath | 54–59 | 1% | 54–59 (1%) |


<a id="route21-north"></a>

## Route 21 — North

Fonte: tabela `sRoute21North_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route21_North`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Tangela | 54–59 | 40% | 54–59 (40%) |
| Espeon | 54–59 | 10% | 54–59 (10%) |
| Exeggutor | 54–59 | 10% | 54–59 (10%) |
| Flareon | 54–59 | 10% | 54–59 (10%) |
| Umbreon | 54–59 | 10% | 54–59 (10%) |
| Charizard | 54–59 | 5% | 54–59 (5%) |
| Typhlosion | 54–59 | 5% | 54–59 (5%) |
| Crobat | 54–59 | 4% | 54–59 (4%) |
| Snorlax | 54–59 | 4% | 54–59 (4%) |
| Blaziken | 54–59 | 1% | 54–59 (1%) |
| Blissey | 54–59 | 1% | 54–59 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 60% | 54–59 (60%) |
| Seadra | 54–59 | 30% | 54–59 (30%) |
| Gorebyss | 54–59 | 5% | 54–59 (5%) |
| Blastoise | 54–59 | 4% | 54–59 (4%) |
| Slowbro | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 70% | 54–59 (70%) |
| Azumarill | 54–59 | 30% | 54–59 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 60% | 54–59 (60%) |
| Kingler | 54–59 | 20% | 54–59 (20%) |
| Whiscash | 54–59 | 20% | 54–59 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 40% | 54–59 (40%) |
| Sealeo | 54–59 | 40% | 54–59 (40%) |
| Sharpedo | 54–59 | 15% | 54–59 (15%) |
| Seadra | 54–59 | 4% | 54–59 (4%) |
| Mantine | 54–59 | 1% | 54–59 (1%) |


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
| Quagsire | 54–59 | 60% | 54–59 (60%) |
| Qwilfish | 54–59 | 30% | 54–59 (30%) |
| Politoed | 54–59 | 5% | 54–59 (5%) |
| Ludicolo | 54–59 | 4% | 54–59 (4%) |
| Milotic | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 70% | 54–59 (70%) |
| Wailmer | 54–59 | 30% | 54–59 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 54–59 | 60% | 54–59 (60%) |
| Dewgong | 54–59 | 20% | 54–59 (20%) |
| Relicanth | 54–59 | 20% | 54–59 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 40% | 54–59 (40%) |
| Quagsire | 54–59 | 40% | 54–59 (40%) |
| Huntail | 54–59 | 15% | 54–59 (15%) |
| Politoed | 54–59 | 4% | 54–59 (4%) |
| Walrein | 54–59 | 1% | 54–59 (1%) |


<a id="route22"></a>

## Route 22

Fonte: tabela `sRoute22_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route22`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mankey | 5–12 | 20% | 5–12 (20%) |
| Spearow | 5–12 | 20% | 5–12 (20%) |
| Marill | 5–12 | 10% | 5–12 (10%) |
| Nidoran♀ | 5–12 | 10% | 5–12 (10%) |
| Nidoran♂ | 5–12 | 10% | 5–12 (10%) |
| Wooper | 5–12 | 10% | 5–12 (10%) |
| Azurill | 5–12 | 5% | 5–12 (5%) |
| Phanpy | 5–12 | 5% | 5–12 (5%) |
| Makuhita | 5–12 | 4% | 5–12 (4%) |
| Tyrogue | 5–12 | 4% | 5–12 (4%) |
| Ralts | 5–12 | 1% | 5–12 (1%) |
| Trapinch | 5–12 | 1% | 5–12 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 60% | 54–59 (60%) |
| Qwilfish | 54–59 | 30% | 54–59 (30%) |
| Seaking | 54–59 | 5% | 54–59 (5%) |
| Feraligatr | 54–59 | 4% | 54–59 (4%) |
| Slowbro | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Horsea | 27–33 | 70% | 27–33 (70%) |
| Tentacool | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 44–50 | 60% | 44–50 (60%) |
| Omastar | 44–50 | 20% | 44–50 (20%) |
| Slowking | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwhirl | 44–50 | 40% | 44–50 (40%) |
| Seadra | 44–50 | 40% | 44–50 (40%) |
| Quagsire | 44–50 | 15% | 44–50 (15%) |
| Dewgong | 44–50 | 4% | 44–50 (4%) |
| Mantine | 44–50 | 1% | 44–50 (1%) |


<a id="route23"></a>

## Route 23

Fonte: tabela `sRoute23_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route23`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mantine | 70–75 | 20% | 70–75 (20%) |
| Swalot | 70–75 | 20% | 70–75 (20%) |
| Donphan | 70–75 | 15% | 70–75 (15%) |
| Machamp | 70–75 | 10% | 70–75 (10%) |
| Steelix | 70–75 | 10% | 70–75 (10%) |
| Ursaring | 70–75 | 10% | 70–75 (10%) |
| Porygon2 | 70–75 | 5% | 70–75 (5%) |
| Arcanine | 70–75 | 4% | 70–75 (4%) |
| Scizor | 70–75 | 4% | 70–75 (4%) |
| Salamence | 70–75 | 1% | 70–75 (1%) |
| Slaking | 70–75 | 1% | 70–75 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 70–75 | 60% | 70–75 (60%) |
| Wailmer | 70–75 | 30% | 70–75 (30%) |
| Swampert | 70–75 | 5% | 70–75 (5%) |
| Walrein | 70–75 | 4% | 70–75 (4%) |
| Vaporeon | 70–75 | 1% | 70–75 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 70–75 | 100% | 70–75 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 70–75 | 60% | 70–75 (60%) |
| Dewgong | 70–75 | 20% | 70–75 (20%) |
| Relicanth | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 70–75 | 40% | 70–75 (40%) |
| Sealeo | 70–75 | 40% | 70–75 (40%) |
| Lanturn | 70–75 | 15% | 70–75 (15%) |
| Starmie | 70–75 | 4% | 70–75 (4%) |
| Kingdra | 70–75 | 1% | 70–75 (1%) |


<a id="route24"></a>

## Route 24

Fonte: tabela `sRoute24_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route24`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cacnea | 17–25 | 20% | 17–25 (20%) |
| Teddiursa | 17–25 | 20% | 17–25 (20%) |
| Abra | 17–25 | 10% | 17–25 (10%) |
| Exeggcute | 17–25 | 10% | 17–25 (10%) |
| Gulpin | 17–25 | 10% | 17–25 (10%) |
| Swablu | 17–25 | 10% | 17–25 (10%) |
| Chikorita | 17–25 | 5% | 17–25 (5%) |
| Treecko | 17–25 | 5% | 17–25 (5%) |
| Ekans | 17–25 | 4% | 17–25 (4%) |
| Vulpix | 17–25 | 4% | 17–25 (4%) |
| Jigglypuff | 17–25 | 1% | 17–25 (1%) |
| Skitty | 17–25 | 1% | 17–25 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 60% | 54–59 (60%) |
| Seadra | 54–59 | 30% | 54–59 (30%) |
| Gyarados | 54–59 | 5% | 54–59 (5%) |
| Walrein | 54–59 | 4% | 54–59 (4%) |
| Ludicolo | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 27–33 | 70% | 27–33 (70%) |
| Krabby | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 60% | 44–50 (60%) |
| Azumarill | 44–50 | 20% | 44–50 (20%) |
| Huntail | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 40% | 44–50 (40%) |
| Wailmer | 44–50 | 40% | 44–50 (40%) |
| Huntail | 44–50 | 15% | 44–50 (15%) |
| Octillery | 44–50 | 4% | 44–50 (4%) |
| Dratini | 44–50 | 1% | 44–50 (1%) |


<a id="route25"></a>

## Route 25

Fonte: tabela `sRoute25_FireRed` de `game/src/data/wild_encounters.json`; mapa `Route25`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Eevee | 17–25 | 20% | 17–25 (20%) |
| Spoink | 17–25 | 20% | 17–25 (20%) |
| Doduo | 17–25 | 10% | 17–25 (10%) |
| Natu | 17–25 | 10% | 17–25 (10%) |
| Slowpoke | 17–25 | 10% | 17–25 (10%) |
| Smoochum | 17–25 | 10% | 17–25 (10%) |
| Bulbasaur | 17–25 | 5% | 17–25 (5%) |
| Mudkip | 17–25 | 5% | 17–25 (5%) |
| Cyndaquil | 17–25 | 4% | 17–25 (4%) |
| Meowth | 17–25 | 4% | 17–25 (4%) |
| Meditite | 17–25 | 1% | 17–25 (1%) |
| Slakoth | 17–25 | 1% | 17–25 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 54–59 | 60% | 54–59 (60%) |
| Wailmer | 54–59 | 30% | 54–59 (30%) |
| Politoed | 54–59 | 9% | 54–59 (9%) |
| Tentacruel | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magikarp | 27–33 | 70% | 27–33 (70%) |
| Poliwhirl | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 44–50 | 80% | 44–50 (80%) |
| Huntail | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Poliwhirl | 44–50 | 40% | 44–50 (40%) |
| Quagsire | 44–50 | 40% | 44–50 (40%) |
| Whiscash | 44–50 | 15% | 44–50 (15%) |
| Omastar | 44–50 | 4% | 44–50 (4%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="celadoncity"></a>

## Celadon City

Fonte: tabela `sCeladonCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeladonCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 54–59 | 60% | 54–59 (60%) |
| Seadra | 54–59 | 30% | 54–59 (30%) |
| Octillery | 54–59 | 5% | 54–59 (5%) |
| Tentacruel | 54–59 | 4% | 54–59 (4%) |
| Slowking | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 35–41 | 100% | 35–41 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 60% | 44–50 (60%) |
| Gorebyss | 44–50 | 20% | 44–50 (20%) |
| Omastar | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 40% | 44–50 (40%) |
| Qwilfish | 44–50 | 40% | 44–50 (40%) |
| Huntail | 44–50 | 15% | 44–50 (15%) |
| Lanturn | 44–50 | 4% | 44–50 (4%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="ceruleancave-1f"></a>

## Cerulean Cave — 1 F

Fonte: tabela `sCeruleanCave1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCave_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Crobat | 84–94 | 21% | 84–94 (21%) |
| Absol | 84–94 | 20% | 84–94 (20%) |
| Claydol | 84–94 | 20% | 84–94 (20%) |
| Piloswine | 84–94 | 20% | 84–94 (20%) |
| Golbat | 84–94 | 10% | 84–94 (10%) |
| Nidoking | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 4% | 84–94 (4%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Sealeo | 84–94 | 30% | 84–94 (30%) |
| Walrein | 84–94 | 5% | 84–94 (5%) |
| Wailord | 84–94 | 4% | 84–94 (4%) |
| Poliwrath | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Shedinja | 84–94 | 90% | 84–94 (90%) |
| Nidoking | 84–94 | 5% | 84–94 (5%) |
| Scizor | 84–94 | 4% | 84–94 (4%) |
| Aggron | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 70% | 84–94 (70%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 60% | 84–94 (60%) |
| Kabutops | 84–94 | 20% | 84–94 (20%) |
| Octillery | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 80% | 84–94 (80%) |
| Gorebyss | 84–94 | 15% | 84–94 (15%) |
| Blastoise | 84–94 | 4% | 84–94 (4%) |
| Milotic | 84–94 | 1% | 84–94 (1%) |


<a id="ceruleancave-2f"></a>

## Cerulean Cave — 2 F

Fonte: tabela `sCeruleanCave2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCave_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cacturne | 84–94 | 20% | 84–94 (20%) |
| Lunatone | 84–94 | 20% | 84–94 (20%) |
| Aggron | 84–94 | 10% | 84–94 (10%) |
| Crobat | 84–94 | 10% | 84–94 (10%) |
| Golbat | 84–94 | 10% | 84–94 (10%) |
| Nidoking | 84–94 | 10% | 84–94 (10%) |
| Flygon | 84–94 | 9% | 84–94 (9%) |
| Aerodactyl | 84–94 | 6% | 84–94 (6%) |
| Nidoqueen | 84–94 | 4% | 84–94 (4%) |
| Rayquaza | 90–95 | 1% | 90–95 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Vibrava | 84–94 | 60% | 84–94 (60%) |
| Quagsire | 84–94 | 30% | 84–94 (30%) |
| Shuckle | 84–94 | 6% | 84–94 (6%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |


<a id="ceruleancave-b1f"></a>

## Cerulean Cave — B1 F

Fonte: tabela `sCeruleanCaveB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCave_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Piloswine | 84–94 | 20% | 84–94 (20%) |
| Sandslash | 84–94 | 20% | 84–94 (20%) |
| Aerodactyl | 84–94 | 18% | 84–94 (18%) |
| Nidoqueen | 84–94 | 15% | 84–94 (15%) |
| Aggron | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Flygon | 84–94 | 5% | 84–94 (5%) |
| Crobat | 84–94 | 1% | 84–94 (1%) |
| Mewtwo | 90–95 | 1% | 90–95 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Azumarill | 84–94 | 30% | 84–94 (30%) |
| Blastoise | 84–94 | 5% | 84–94 (5%) |
| Cloyster | 84–94 | 4% | 84–94 (4%) |
| Feraligatr | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Parasect | 84–94 | 60% | 84–94 (60%) |
| Shedinja | 84–94 | 30% | 84–94 (30%) |
| Scizor | 84–94 | 5% | 84–94 (5%) |
| Heracross | 84–94 | 4% | 84–94 (4%) |
| Steelix | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 70% | 84–94 (70%) |
| Qwilfish | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 60% | 84–94 (60%) |
| Lanturn | 84–94 | 20% | 84–94 (20%) |
| Ludicolo | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 80% | 84–94 (80%) |
| Crawdaunt | 84–94 | 15% | 84–94 (15%) |
| Tentacruel | 84–94 | 4% | 84–94 (4%) |
| Starmie | 84–94 | 1% | 84–94 (1%) |


<a id="ceruleancity"></a>

## Cerulean City

Fonte: tabela `sCeruleanCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `CeruleanCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 60% | 54–59 (60%) |
| Quagsire | 54–59 | 30% | 54–59 (30%) |
| Crawdaunt | 54–59 | 5% | 54–59 (5%) |
| Poliwrath | 54–59 | 4% | 54–59 (4%) |
| Cloyster | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Remoraid | 27–33 | 70% | 27–33 (70%) |
| Clamperl | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 44–50 | 60% | 44–50 (60%) |
| Dewgong | 44–50 | 20% | 44–50 (20%) |
| Huntail | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 44–50 | 80% | 44–50 (80%) |
| Wailmer | 44–50 | 15% | 44–50 (15%) |
| Kabutops | 44–50 | 4% | 44–50 (4%) |
| Seaking | 44–50 | 1% | 44–50 (1%) |


<a id="cinnabarisland"></a>

## Cinnabar Island

Fonte: tabela `sCinnabarIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `CinnabarIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 62–67 | 60% | 62–67 (60%) |
| Qwilfish | 62–67 | 30% | 62–67 (30%) |
| Walrein | 62–67 | 5% | 62–67 (5%) |
| Feraligatr | 62–67 | 4% | 62–67 (4%) |
| Lapras | 62–67 | 1% | 62–67 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 62–67 | 70% | 62–67 (70%) |
| Seadra | 62–67 | 30% | 62–67 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 62–67 | 60% | 62–67 (60%) |
| Sharpedo | 62–67 | 20% | 62–67 (20%) |
| Slowking | 62–67 | 20% | 62–67 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 62–67 | 40% | 62–67 (40%) |
| Seadra | 62–67 | 40% | 62–67 (40%) |
| Ludicolo | 62–67 | 15% | 62–67 (15%) |
| Milotic | 62–67 | 4% | 62–67 (4%) |
| Lapras | 62–67 | 1% | 62–67 (1%) |


<a id="diglettscave-b1f"></a>

## Digletts Cave — B1 F

Fonte: tabela `sDiglettsCaveB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `DiglettsCave_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Diglett | 27–33 | 41% | 27–33 (41%) |
| Graveler | 27–33 | 10% | 27–33 (10%) |
| Nosepass | 27–33 | 10% | 27–33 (10%) |
| Onix | 27–33 | 10% | 27–33 (10%) |
| Sableye | 27–33 | 10% | 27–33 (10%) |
| Anorith | 27–33 | 5% | 27–33 (5%) |
| Kabuto | 27–33 | 5% | 27–33 (5%) |
| Lileep | 27–33 | 5% | 27–33 (5%) |
| Omanyte | 27–33 | 4% | 27–33 (4%) |


<a id="fiveisland"></a>

## Five Island

Fonte: tabela `sFiveIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 60% | 84–94 (60%) |
| Wailmer | 84–94 | 30% | 84–94 (30%) |
| Feraligatr | 84–94 | 5% | 84–94 (5%) |
| Vaporeon | 84–94 | 4% | 84–94 (4%) |
| Poliwrath | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 70% | 84–94 (70%) |
| Corsola | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Seaking | 84–94 | 20% | 84–94 (20%) |
| Sharpedo | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 84–94 | 40% | 84–94 (40%) |
| Sealeo | 84–94 | 40% | 84–94 (40%) |
| Slowking | 84–94 | 15% | 84–94 (15%) |
| Politoed | 84–94 | 4% | 84–94 (4%) |
| Vaporeon | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room1"></a>

## Five Island — Lost Cave — Room 1

Fonte: tabela `sFiveIslandLostCaveRoom1_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room1`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Chimecho | 84–94 | 20% | 84–94 (20%) |
| Cradily | 84–94 | 20% | 84–94 (20%) |
| Crobat | 84–94 | 19% | 84–94 (19%) |
| Flygon | 84–94 | 10% | 84–94 (10%) |
| Nidoqueen | 84–94 | 10% | 84–94 (10%) |
| Shuckle | 84–94 | 10% | 84–94 (10%) |
| Umbreon | 84–94 | 5% | 84–94 (5%) |
| Donphan | 84–94 | 4% | 84–94 (4%) |
| Aggron | 84–94 | 1% | 84–94 (1%) |
| Metagross | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room2"></a>

## Five Island — Lost Cave — Room 2

Fonte: tabela `sFiveIslandLostCaveRoom2_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room2`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Girafarig | 84–94 | 20% | 84–94 (20%) |
| Rhydon | 84–94 | 20% | 84–94 (20%) |
| Crobat | 84–94 | 19% | 84–94 (19%) |
| Gardevoir | 84–94 | 10% | 84–94 (10%) |
| Golbat | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Exeggutor | 84–94 | 5% | 84–94 (5%) |
| Flygon | 84–94 | 4% | 84–94 (4%) |
| Nidoqueen | 84–94 | 1% | 84–94 (1%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room3"></a>

## Five Island — Lost Cave — Room 3

Fonte: tabela `sFiveIslandLostCaveRoom3_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room3`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mr. Mime | 84–94 | 20% | 84–94 (20%) |
| Xatu | 84–94 | 20% | 84–94 (20%) |
| Aggron | 84–94 | 10% | 84–94 (10%) |
| Alakazam | 84–94 | 10% | 84–94 (10%) |
| Espeon | 84–94 | 10% | 84–94 (10%) |
| Flygon | 84–94 | 10% | 84–94 (10%) |
| Nidoqueen | 84–94 | 10% | 84–94 (10%) |
| Exeggutor | 84–94 | 4% | 84–94 (4%) |
| Umbreon | 84–94 | 4% | 84–94 (4%) |
| Nidoking | 84–94 | 2% | 84–94 (2%) |


<a id="fiveisland-lostcave-room4"></a>

## Five Island — Lost Cave — Room 4

Fonte: tabela `sFiveIslandLostCaveRoom4_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room4`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Jynx | 84–94 | 20% | 84–94 (20%) |
| Omastar | 84–94 | 20% | 84–94 (20%) |
| Shuckle | 84–94 | 20% | 84–94 (20%) |
| Donphan | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Alakazam | 84–94 | 5% | 84–94 (5%) |
| Exeggutor | 84–94 | 5% | 84–94 (5%) |
| Gardevoir | 84–94 | 4% | 84–94 (4%) |
| Umbreon | 84–94 | 4% | 84–94 (4%) |
| Claydol | 84–94 | 1% | 84–94 (1%) |
| Golbat | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room5"></a>

## Five Island — Lost Cave — Room 5

Fonte: tabela `sFiveIslandLostCaveRoom5_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room5`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Crobat | 84–94 | 24% | 84–94 (24%) |
| Golbat | 84–94 | 20% | 84–94 (20%) |
| Jynx | 84–94 | 20% | 84–94 (20%) |
| Gengar | 84–94 | 14% | 84–94 (14%) |
| Alakazam | 84–94 | 10% | 84–94 (10%) |
| Umbreon | 84–94 | 10% | 84–94 (10%) |
| Flygon | 84–94 | 1% | 84–94 (1%) |
| Houndoom | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room6"></a>

## Five Island — Lost Cave — Room 6

Fonte: tabela `sFiveIslandLostCaveRoom6_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room6`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Absol | 84–94 | 20% | 84–94 (20%) |
| Sandslash | 84–94 | 20% | 84–94 (20%) |
| Swampert | 84–94 | 14% | 84–94 (14%) |
| Aerodactyl | 84–94 | 10% | 84–94 (10%) |
| Gardevoir | 84–94 | 10% | 84–94 (10%) |
| Houndoom | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Shuckle | 84–94 | 4% | 84–94 (4%) |
| Crobat | 84–94 | 1% | 84–94 (1%) |
| Exeggutor | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room7"></a>

## Five Island — Lost Cave — Room 7

Fonte: tabela `sFiveIslandLostCaveRoom7_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room7`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Absol | 84–94 | 20% | 84–94 (20%) |
| Cradily | 84–94 | 20% | 84–94 (20%) |
| Crobat | 84–94 | 11% | 84–94 (11%) |
| Flygon | 84–94 | 10% | 84–94 (10%) |
| Houndoom | 84–94 | 10% | 84–94 (10%) |
| Nidoking | 84–94 | 10% | 84–94 (10%) |
| Alakazam | 84–94 | 5% | 84–94 (5%) |
| Exeggutor | 84–94 | 5% | 84–94 (5%) |
| Swampert | 84–94 | 5% | 84–94 (5%) |
| Gengar | 84–94 | 4% | 84–94 (4%) |


<a id="fiveisland-lostcave-room8"></a>

## Five Island — Lost Cave — Room 8

Fonte: tabela `sFiveIslandLostCaveRoom8_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room8`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cradily | 84–94 | 20% | 84–94 (20%) |
| Rhydon | 84–94 | 20% | 84–94 (20%) |
| Golbat | 84–94 | 19% | 84–94 (19%) |
| Gardevoir | 84–94 | 10% | 84–94 (10%) |
| Shuckle | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Espeon | 84–94 | 5% | 84–94 (5%) |
| Aggron | 84–94 | 4% | 84–94 (4%) |
| Crobat | 84–94 | 2% | 84–94 (2%) |


<a id="fiveisland-lostcave-room9"></a>

## Five Island — Lost Cave — Room 9

Fonte: tabela `sFiveIslandLostCaveRoom9_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room9`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Absol | 84–94 | 20% | 84–94 (20%) |
| Dusclops | 84–94 | 20% | 84–94 (20%) |
| Golbat | 84–94 | 15% | 84–94 (15%) |
| Alakazam | 84–94 | 10% | 84–94 (10%) |
| Claydol | 84–94 | 10% | 84–94 (10%) |
| Espeon | 84–94 | 10% | 84–94 (10%) |
| Crobat | 84–94 | 5% | 84–94 (5%) |
| Gengar | 84–94 | 4% | 84–94 (4%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |
| Nidoking | 84–94 | 1% | 84–94 (1%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room10"></a>

## Five Island — Lost Cave — Room 10

Fonte: tabela `sFiveIslandLostCaveRoom10_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room10`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Crobat | 84–94 | 30% | 84–94 (30%) |
| Rhydon | 84–94 | 20% | 84–94 (20%) |
| Wobbuffet | 84–94 | 20% | 84–94 (20%) |
| Claydol | 84–94 | 14% | 84–94 (14%) |
| Aggron | 84–94 | 5% | 84–94 (5%) |
| Espeon | 84–94 | 5% | 84–94 (5%) |
| Donphan | 84–94 | 4% | 84–94 (4%) |
| Mew | 85–90 | 1% | 85–90 (1%) |
| Nidoking | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room11"></a>

## Five Island — Lost Cave — Room 11

Fonte: tabela `sFiveIslandLostCaveRoom11_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room11`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Absol | 84–94 | 20% | 84–94 (20%) |
| Golbat | 84–94 | 20% | 84–94 (20%) |
| Houndoom | 84–94 | 20% | 84–94 (20%) |
| Crobat | 84–94 | 16% | 84–94 (16%) |
| Shuckle | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 5% | 84–94 (5%) |
| Alakazam | 84–94 | 4% | 84–94 (4%) |
| Donphan | 84–94 | 4% | 84–94 (4%) |
| Aerodactyl | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room12"></a>

## Five Island — Lost Cave — Room 12

Fonte: tabela `sFiveIslandLostCaveRoom12_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room12`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Girafarig | 84–94 | 20% | 84–94 (20%) |
| Nidoking | 84–94 | 20% | 84–94 (20%) |
| Xatu | 84–94 | 20% | 84–94 (20%) |
| Houndoom | 84–94 | 15% | 84–94 (15%) |
| Nidoqueen | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 5% | 84–94 (5%) |
| Flygon | 84–94 | 4% | 84–94 (4%) |
| Golbat | 84–94 | 4% | 84–94 (4%) |
| Crobat | 84–94 | 1% | 84–94 (1%) |
| Metagross | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room13"></a>

## Five Island — Lost Cave — Room 13

Fonte: tabela `sFiveIslandLostCaveRoom13_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room13`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Golbat | 84–94 | 35% | 84–94 (35%) |
| Wobbuffet | 84–94 | 20% | 84–94 (20%) |
| Gengar | 84–94 | 14% | 84–94 (14%) |
| Alakazam | 84–94 | 10% | 84–94 (10%) |
| Donphan | 84–94 | 10% | 84–94 (10%) |
| Shuckle | 84–94 | 5% | 84–94 (5%) |
| Crobat | 84–94 | 4% | 84–94 (4%) |
| Espeon | 84–94 | 1% | 84–94 (1%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-lostcave-room14"></a>

## Five Island — Lost Cave — Room 14

Fonte: tabela `sFiveIslandLostCaveRoom14_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_LostCave_Room14`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Banette | 84–94 | 20% | 84–94 (20%) |
| Dusclops | 84–94 | 20% | 84–94 (20%) |
| Claydol | 84–94 | 15% | 84–94 (15%) |
| Aerodactyl | 84–94 | 10% | 84–94 (10%) |
| Donphan | 84–94 | 10% | 84–94 (10%) |
| Gengar | 84–94 | 10% | 84–94 (10%) |
| Golbat | 84–94 | 5% | 84–94 (5%) |
| Aggron | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 4% | 84–94 (4%) |
| Jirachi | 85–90 | 1% | 85–90 (1%) |
| Swampert | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-meadow"></a>

## Five Island — Meadow

Fonte: tabela `sFiveIslandMeadow_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_Meadow`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Breloom | 84–94 | 20% | 84–94 (20%) |
| Kabutops | 84–94 | 20% | 84–94 (20%) |
| Ampharos | 84–94 | 10% | 84–94 (10%) |
| Golduck | 84–94 | 10% | 84–94 (10%) |
| Poliwrath | 84–94 | 10% | 84–94 (10%) |
| Umbreon | 84–94 | 10% | 84–94 (10%) |
| Swampert | 84–94 | 5% | 84–94 (5%) |
| Vaporeon | 84–94 | 5% | 84–94 (5%) |
| Flareon | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 4% | 84–94 (4%) |
| Arcanine | 84–94 | 1% | 84–94 (1%) |
| Raikou | 85–90 | 1% | 85–90 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 60% | 84–94 (60%) |
| Azumarill | 84–94 | 30% | 84–94 (30%) |
| Milotic | 84–94 | 5% | 84–94 (5%) |
| Blastoise | 84–94 | 4% | 84–94 (4%) |
| Walrein | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 70% | 84–94 (70%) |
| Sealeo | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 60% | 84–94 (60%) |
| Crawdaunt | 84–94 | 20% | 84–94 (20%) |
| Seaking | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 40% | 84–94 (40%) |
| Qwilfish | 84–94 | 40% | 84–94 (40%) |
| Kabutops | 84–94 | 15% | 84–94 (15%) |
| Walrein | 84–94 | 5% | 84–94 (5%) |


<a id="fiveisland-memorialpillar"></a>

## Five Island — Memorial Pillar

Fonte: tabela `sFiveIslandMemorialPillar_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_MemorialPillar`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Arcanine | 84–94 | 20% | 84–94 (20%) |
| Breloom | 84–94 | 20% | 84–94 (20%) |
| Shedinja | 84–94 | 20% | 84–94 (20%) |
| Ninetales | 84–94 | 10% | 84–94 (10%) |
| Politoed | 84–94 | 10% | 84–94 (10%) |
| Vaporeon | 84–94 | 10% | 84–94 (10%) |
| Typhlosion | 84–94 | 5% | 84–94 (5%) |
| Feraligatr | 84–94 | 4% | 84–94 (4%) |
| Latios | 85–90 | 1% | 85–90 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 60% | 84–94 (60%) |
| Pelipper | 84–94 | 30% | 84–94 (30%) |
| Poliwrath | 84–94 | 6% | 84–94 (6%) |
| Walrein | 84–94 | 4% | 84–94 (4%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 70% | 84–94 (70%) |
| Wailmer | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Mantine | 84–94 | 20% | 84–94 (20%) |
| Omastar | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 84–94 | 40% | 84–94 (40%) |
| Sealeo | 84–94 | 40% | 84–94 (40%) |
| Ludicolo | 84–94 | 15% | 84–94 (15%) |
| Kingdra | 84–94 | 4% | 84–94 (4%) |
| Starmie | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-resortgorgeous"></a>

## Five Island — Resort Gorgeous

Fonte: tabela `sFiveIslandResortGorgeous_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_ResortGorgeous`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Azumarill | 84–94 | 30% | 84–94 (30%) |
| Vaporeon | 84–94 | 9% | 84–94 (9%) |
| Latias | 85–90 | 1% | 85–90 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 70% | 84–94 (70%) |
| Qwilfish | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 60% | 84–94 (60%) |
| Relicanth | 84–94 | 20% | 84–94 (20%) |
| Slowking | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 40% | 84–94 (40%) |
| Wailmer | 84–94 | 40% | 84–94 (40%) |
| Gorebyss | 84–94 | 15% | 84–94 (15%) |
| Vaporeon | 84–94 | 4% | 84–94 (4%) |
| Walrein | 84–94 | 1% | 84–94 (1%) |


<a id="fiveisland-waterlabyrinth"></a>

## Five Island — Water Labyrinth

Fonte: tabela `sFiveIslandWaterLabyrinth_FireRed` de `game/src/data/wild_encounters.json`; mapa `FiveIsland_WaterLabyrinth`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 60% | 84–94 (60%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |
| Feraligatr | 84–94 | 5% | 84–94 (5%) |
| Politoed | 84–94 | 4% | 84–94 (4%) |
| Kyogre | 90–95 | 1% | 90–95 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 70% | 84–94 (70%) |
| Corsola | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 60% | 84–94 (60%) |
| Lanturn | 84–94 | 20% | 84–94 (20%) |
| Sharpedo | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 80% | 84–94 (80%) |
| Slowbro | 84–94 | 15% | 84–94 (15%) |
| Tentacruel | 84–94 | 4% | 84–94 (4%) |
| Swampert | 84–94 | 1% | 84–94 (1%) |


<a id="fourisland"></a>

## Four Island

Fonte: tabela `sFourIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 60% | 84–94 (60%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |
| Feraligatr | 84–94 | 5% | 84–94 (5%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |
| Gyarados | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 70% | 84–94 (70%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 60% | 84–94 (60%) |
| Slowking | 84–94 | 20% | 84–94 (20%) |
| Whiscash | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 40% | 84–94 (40%) |
| Pelipper | 84–94 | 40% | 84–94 (40%) |
| Omastar | 84–94 | 15% | 84–94 (15%) |
| Politoed | 84–94 | 4% | 84–94 (4%) |
| Poliwrath | 84–94 | 1% | 84–94 (1%) |


<a id="fourisland-icefallcave-1f"></a>

## Four Island — Icefall Cave — 1 F

Fonte: tabela `sFourIslandIcefallCave1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cradily | 84–94 | 20% | 84–94 (20%) |
| Lunatone | 84–94 | 20% | 84–94 (20%) |
| Poliwrath | 84–94 | 14% | 84–94 (14%) |
| Vaporeon | 84–94 | 14% | 84–94 (14%) |
| Aerodactyl | 84–94 | 10% | 84–94 (10%) |
| Crobat | 84–94 | 10% | 84–94 (10%) |
| Blastoise | 84–94 | 5% | 84–94 (5%) |
| Golduck | 84–94 | 5% | 84–94 (5%) |
| Articuno | 85–90 | 1% | 85–90 (1%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |


<a id="fourisland-icefallcave-b1f"></a>

## Four Island — Icefall Cave — B1 F

Fonte: tabela `sFourIslandIcefallCaveB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Shuckle | 84–94 | 24% | 84–94 (24%) |
| Cradily | 84–94 | 20% | 84–94 (20%) |
| Kabutops | 84–94 | 20% | 84–94 (20%) |
| Golbat | 84–94 | 10% | 84–94 (10%) |
| Politoed | 84–94 | 10% | 84–94 (10%) |
| Vaporeon | 84–94 | 10% | 84–94 (10%) |
| Aggron | 84–94 | 4% | 84–94 (4%) |
| Poliwrath | 84–94 | 1% | 84–94 (1%) |
| Regice | 85–90 | 1% | 85–90 (1%) |


<a id="fourisland-icefallcave-back"></a>

## Four Island — Icefall Cave — Back

Fonte: tabela `sFourIslandIcefallCaveBack_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_Back`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Omastar | 84–94 | 20% | 84–94 (20%) |
| Piloswine | 84–94 | 20% | 84–94 (20%) |
| Aggron | 84–94 | 15% | 84–94 (15%) |
| Aerodactyl | 84–94 | 10% | 84–94 (10%) |
| Crobat | 84–94 | 10% | 84–94 (10%) |
| Walrein | 84–94 | 10% | 84–94 (10%) |
| Golduck | 84–94 | 9% | 84–94 (9%) |
| Vaporeon | 84–94 | 5% | 84–94 (5%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 60% | 84–94 (60%) |
| Pelipper | 84–94 | 30% | 84–94 (30%) |
| Vaporeon | 84–94 | 5% | 84–94 (5%) |
| Milotic | 84–94 | 4% | 84–94 (4%) |
| Suicune | 85–90 | 1% | 85–90 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 70% | 84–94 (70%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 60% | 84–94 (60%) |
| Omastar | 84–94 | 20% | 84–94 (20%) |
| Relicanth | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 40% | 84–94 (40%) |
| Pelipper | 84–94 | 40% | 84–94 (40%) |
| Whiscash | 84–94 | 15% | 84–94 (15%) |
| Kingdra | 84–94 | 4% | 84–94 (4%) |
| Vaporeon | 84–94 | 1% | 84–94 (1%) |


<a id="fourisland-icefallcave-entrance"></a>

## Four Island — Icefall Cave — Entrance

Fonte: tabela `sFourIslandIcefallCaveEntrance_FireRed` de `game/src/data/wild_encounters.json`; mapa `FourIsland_IcefallCave_Entrance`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Lunatone | 84–94 | 20% | 84–94 (20%) |
| Omastar | 84–94 | 20% | 84–94 (20%) |
| Crobat | 84–94 | 11% | 84–94 (11%) |
| Golbat | 84–94 | 11% | 84–94 (11%) |
| Aggron | 84–94 | 10% | 84–94 (10%) |
| Politoed | 84–94 | 10% | 84–94 (10%) |
| Poliwrath | 84–94 | 10% | 84–94 (10%) |
| Golduck | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 4% | 84–94 (4%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 60% | 84–94 (60%) |
| Qwilfish | 84–94 | 30% | 84–94 (30%) |
| Golduck | 84–94 | 5% | 84–94 (5%) |
| Lapras | 84–94 | 5% | 84–94 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 70% | 84–94 (70%) |
| Azumarill | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 60% | 84–94 (60%) |
| Ludicolo | 84–94 | 20% | 84–94 (20%) |
| Slowking | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 40% | 84–94 (40%) |
| Qwilfish | 84–94 | 40% | 84–94 (40%) |
| Mantine | 84–94 | 15% | 84–94 (15%) |
| Lapras | 84–94 | 4% | 84–94 (4%) |
| Walrein | 84–94 | 1% | 84–94 (1%) |


<a id="fuchsiacity"></a>

## Fuchsia City

Fonte: tabela `sFuchsiaCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `FuchsiaCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 60% | 54–59 (60%) |
| Seadra | 54–59 | 30% | 54–59 (30%) |
| Feraligatr | 54–59 | 5% | 54–59 (5%) |
| Lapras | 54–59 | 4% | 54–59 (4%) |
| Crawdaunt | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 44–50 | 70% | 44–50 (70%) |
| Pelipper | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 60% | 44–50 (60%) |
| Dewgong | 44–50 | 20% | 44–50 (20%) |
| Kabutops | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 40% | 44–50 (40%) |
| Wailmer | 44–50 | 40% | 44–50 (40%) |
| Lanturn | 44–50 | 15% | 44–50 (15%) |
| Huntail | 44–50 | 4% | 44–50 (4%) |
| Croconaw | 44–50 | 1% | 44–50 (1%) |


<a id="mtember-exterior"></a>

## Mt Ember — Exterior

Fonte: tabela `sMtEmberExterior_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_Exterior`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Claydol | 70–75 | 20% | 70–75 (20%) |
| Piloswine | 70–75 | 20% | 70–75 (20%) |
| Torkoal | 70–75 | 20% | 70–75 (20%) |
| Rapidash | 70–75 | 15% | 70–75 (15%) |
| Flareon | 70–75 | 10% | 70–75 (10%) |
| Arcanine | 70–75 | 5% | 70–75 (5%) |
| Nidoking | 70–75 | 5% | 70–75 (5%) |
| Swampert | 70–75 | 5% | 70–75 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Lairon | 70–75 | 60% | 70–75 (60%) |
| Marowak | 70–75 | 30% | 70–75 (30%) |
| Pinsir | 70–75 | 5% | 70–75 (5%) |
| Claydol | 70–75 | 4% | 70–75 (4%) |
| Heracross | 70–75 | 1% | 70–75 (1%) |


<a id="mtember-rubypath-1f"></a>

## Mt Ember — Ruby Path — 1 F

Fonte: tabela `sMtEmberRubyPath1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Camerupt | 84–94 | 20% | 84–94 (20%) |
| Rapidash | 84–94 | 20% | 84–94 (20%) |
| Vibrava | 84–94 | 20% | 84–94 (20%) |
| Claydol | 84–94 | 11% | 84–94 (11%) |
| Arcanine | 84–94 | 10% | 84–94 (10%) |
| Charizard | 84–94 | 5% | 84–94 (5%) |
| Flygon | 84–94 | 5% | 84–94 (5%) |
| Flareon | 84–94 | 4% | 84–94 (4%) |
| Steelix | 84–94 | 4% | 84–94 (4%) |
| Donphan | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pupitar | 84–94 | 60% | 84–94 (60%) |
| Illumise | 84–94 | 30% | 84–94 (30%) |
| Scyther | 84–94 | 5% | 84–94 (5%) |
| Claydol | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 1% | 84–94 (1%) |


<a id="mtember-rubypath-b1f"></a>

## Mt Ember — Ruby Path — B1 F

Fonte: tabela `sMtEmberRubyPathB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Piloswine | 84–94 | 40% | 84–94 (40%) |
| Arcanine | 84–94 | 20% | 84–94 (20%) |
| Nidoking | 84–94 | 10% | 84–94 (10%) |
| Nidoqueen | 84–94 | 10% | 84–94 (10%) |
| Claydol | 84–94 | 5% | 84–94 (5%) |
| Houndoom | 84–94 | 5% | 84–94 (5%) |
| Swampert | 84–94 | 5% | 84–94 (5%) |
| Donphan | 84–94 | 4% | 84–94 (4%) |
| Moltres | 85–90 | 1% | 85–90 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Volbeat | 84–94 | 60% | 84–94 (60%) |
| Parasect | 84–94 | 30% | 84–94 (30%) |
| Claydol | 84–94 | 5% | 84–94 (5%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 1% | 84–94 (1%) |


<a id="mtember-rubypath-b1f-stairs"></a>

## Mt Ember — Ruby Path — B1 F — Stairs

Fonte: tabela `sMtEmberRubyPathB1FStairs_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B1F_Stairs`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Donphan | 84–94 | 25% | 84–94 (25%) |
| Piloswine | 84–94 | 20% | 84–94 (20%) |
| Sandslash | 84–94 | 20% | 84–94 (20%) |
| Claydol | 84–94 | 10% | 84–94 (10%) |
| Nidoqueen | 84–94 | 10% | 84–94 (10%) |
| Ninetales | 84–94 | 5% | 84–94 (5%) |
| Arcanine | 84–94 | 4% | 84–94 (4%) |
| Flygon | 84–94 | 4% | 84–94 (4%) |
| Charizard | 84–94 | 1% | 84–94 (1%) |
| Swampert | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Ariados | 84–94 | 60% | 84–94 (60%) |
| Parasect | 84–94 | 30% | 84–94 (30%) |
| Aerodactyl | 84–94 | 5% | 84–94 (5%) |
| Steelix | 84–94 | 4% | 84–94 (4%) |
| Tyranitar | 84–94 | 1% | 84–94 (1%) |


<a id="mtember-rubypath-b2f"></a>

## Mt Ember — Ruby Path — B2 F

Fonte: tabela `sMtEmberRubyPathB2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Arcanine | 84–94 | 20% | 84–94 (20%) |
| Magmar | 84–94 | 20% | 84–94 (20%) |
| Torkoal | 84–94 | 20% | 84–94 (20%) |
| Flareon | 84–94 | 15% | 84–94 (15%) |
| Rapidash | 84–94 | 10% | 84–94 (10%) |
| Houndoom | 84–94 | 5% | 84–94 (5%) |
| Blaziken | 84–94 | 4% | 84–94 (4%) |
| Steelix | 84–94 | 4% | 84–94 (4%) |
| Entei | 85–90 | 1% | 85–90 (1%) |
| Swampert | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 60% | 84–94 (60%) |
| Marowak | 84–94 | 30% | 84–94 (30%) |
| Nidoking | 84–94 | 5% | 84–94 (5%) |
| Aerodactyl | 84–94 | 4% | 84–94 (4%) |
| Scyther | 84–94 | 1% | 84–94 (1%) |


<a id="mtember-rubypath-b2f-stairs"></a>

## Mt Ember — Ruby Path — B2 F — Stairs

Fonte: tabela `sMtEmberRubyPathB2FStairs_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B2F_Stairs`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Camerupt | 84–94 | 20% | 84–94 (20%) |
| Houndoom | 84–94 | 20% | 84–94 (20%) |
| Sandslash | 84–94 | 20% | 84–94 (20%) |
| Arcanine | 84–94 | 10% | 84–94 (10%) |
| Flareon | 84–94 | 10% | 84–94 (10%) |
| Nidoking | 84–94 | 10% | 84–94 (10%) |
| Rapidash | 84–94 | 4% | 84–94 (4%) |
| Steelix | 84–94 | 4% | 84–94 (4%) |
| Swampert | 84–94 | 1% | 84–94 (1%) |
| Typhlosion | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Lairon | 84–94 | 60% | 84–94 (60%) |
| Parasect | 84–94 | 30% | 84–94 (30%) |
| Scyther | 84–94 | 5% | 84–94 (5%) |
| Pinsir | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 1% | 84–94 (1%) |


<a id="mtember-rubypath-b3f"></a>

## Mt Ember — Ruby Path — B3 F

Fonte: tabela `sMtEmberRubyPathB3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_RubyPath_B3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magmar | 84–94 | 40% | 84–94 (40%) |
| Donphan | 84–94 | 11% | 84–94 (11%) |
| Flareon | 84–94 | 10% | 84–94 (10%) |
| Flygon | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Typhlosion | 84–94 | 9% | 84–94 (9%) |
| Rapidash | 84–94 | 5% | 84–94 (5%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |
| Groudon | 90–95 | 1% | 90–95 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Magcargo | 84–94 | 60% | 84–94 (60%) |
| Ariados | 84–94 | 30% | 84–94 (30%) |
| Shuckle | 84–94 | 5% | 84–94 (5%) |
| Flygon | 84–94 | 4% | 84–94 (4%) |
| Heracross | 84–94 | 1% | 84–94 (1%) |


<a id="mtember-summitpath-1f"></a>

## Mt Ember — Summit Path — 1 F

Fonte: tabela `sMtEmberSummitPath1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_SummitPath_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Steelix | 70–75 | 30% | 70–75 (30%) |
| Golem | 70–75 | 20% | 70–75 (20%) |
| Piloswine | 70–75 | 20% | 70–75 (20%) |
| Flygon | 70–75 | 10% | 70–75 (10%) |
| Nidoking | 70–75 | 10% | 70–75 (10%) |
| Nidoqueen | 70–75 | 4% | 70–75 (4%) |
| Ninetales | 70–75 | 4% | 70–75 (4%) |
| Charizard | 70–75 | 1% | 70–75 (1%) |
| Rapidash | 70–75 | 1% | 70–75 (1%) |


<a id="mtember-summitpath-2f"></a>

## Mt Ember — Summit Path — 2 F

Fonte: tabela `sMtEmberSummitPath2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_SummitPath_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Houndoom | 70–75 | 25% | 70–75 (25%) |
| Rhydon | 70–75 | 20% | 70–75 (20%) |
| Sandslash | 70–75 | 20% | 70–75 (20%) |
| Rapidash | 70–75 | 11% | 70–75 (11%) |
| Flareon | 70–75 | 10% | 70–75 (10%) |
| Arcanine | 70–75 | 6% | 70–75 (6%) |
| Nidoking | 70–75 | 4% | 70–75 (4%) |
| Nidoqueen | 70–75 | 4% | 70–75 (4%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sudowoodo | 70–75 | 60% | 70–75 (60%) |
| Magcargo | 70–75 | 30% | 70–75 (30%) |
| Nidoking | 70–75 | 5% | 70–75 (5%) |
| Scizor | 70–75 | 4% | 70–75 (4%) |
| Tyranitar | 70–75 | 1% | 70–75 (1%) |


<a id="mtember-summitpath-3f"></a>

## Mt Ember — Summit Path — 3 F

Fonte: tabela `sMtEmberSummitPath3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtEmber_SummitPath_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Golem | 70–75 | 20% | 70–75 (20%) |
| Rhydon | 70–75 | 20% | 70–75 (20%) |
| Arcanine | 70–75 | 15% | 70–75 (15%) |
| Claydol | 70–75 | 11% | 70–75 (11%) |
| Donphan | 70–75 | 10% | 70–75 (10%) |
| Steelix | 70–75 | 10% | 70–75 (10%) |
| Swampert | 70–75 | 5% | 70–75 (5%) |
| Flareon | 70–75 | 4% | 70–75 (4%) |
| Rapidash | 70–75 | 4% | 70–75 (4%) |
| Typhlosion | 70–75 | 1% | 70–75 (1%) |


<a id="mtmoon-1f"></a>

## Mt Moon — 1 F

Fonte: tabela `sMtMoon1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtMoon_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Aron | 17–25 | 20% | 17–25 (20%) |
| Magnemite | 17–25 | 20% | 17–25 (20%) |
| Baltoy | 17–25 | 10% | 17–25 (10%) |
| Gastly | 17–25 | 10% | 17–25 (10%) |
| Ledyba | 17–25 | 10% | 17–25 (10%) |
| Nincada | 17–25 | 10% | 17–25 (10%) |
| Zubat | 17–25 | 9% | 17–25 (9%) |
| Cleffa | 17–25 | 5% | 17–25 (5%) |
| Paras | 17–25 | 5% | 17–25 (5%) |
| Clefairy | 17–25 | 1% | 17–25 (1%) |


<a id="mtmoon-b1f"></a>

## Mt Moon — B1 F

Fonte: tabela `sMtMoonB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtMoon_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cubone | 17–25 | 20% | 17–25 (20%) |
| Rhyhorn | 17–25 | 20% | 17–25 (20%) |
| Paras | 17–25 | 15% | 17–25 (15%) |
| Diglett | 17–25 | 10% | 17–25 (10%) |
| Numel | 17–25 | 10% | 17–25 (10%) |
| Sandshrew | 17–25 | 10% | 17–25 (10%) |
| Shuppet | 17–25 | 10% | 17–25 (10%) |
| Zubat | 17–25 | 5% | 17–25 (5%) |


<a id="mtmoon-b2f"></a>

## Mt Moon — B2 F

Fonte: tabela `sMtMoonB2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `MtMoon_B2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Clefairy | 17–25 | 26% | 17–25 (26%) |
| Houndour | 17–25 | 20% | 17–25 (20%) |
| Duskull | 17–25 | 10% | 17–25 (10%) |
| Geodude | 17–25 | 10% | 17–25 (10%) |
| Swinub | 17–25 | 10% | 17–25 (10%) |
| Venonat | 17–25 | 10% | 17–25 (10%) |
| Zubat | 17–25 | 9% | 17–25 (9%) |
| Paras | 17–25 | 5% | 17–25 (5%) |


<a id="oneisland"></a>

## One Island

Fonte: tabela `sOneIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `OneIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 70–75 | 60% | 70–75 (60%) |
| Quagsire | 70–75 | 30% | 70–75 (30%) |
| Poliwrath | 70–75 | 5% | 70–75 (5%) |
| Tentacruel | 70–75 | 5% | 70–75 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 70–75 | 70% | 70–75 (70%) |
| Corsola | 70–75 | 30% | 70–75 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 70–75 | 60% | 70–75 (60%) |
| Kabutops | 70–75 | 20% | 70–75 (20%) |
| Ludicolo | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 70–75 | 40% | 70–75 (40%) |
| Qwilfish | 70–75 | 40% | 70–75 (40%) |
| Kabutops | 70–75 | 15% | 70–75 (15%) |
| Kingdra | 70–75 | 4% | 70–75 (4%) |
| Walrein | 70–75 | 1% | 70–75 (1%) |


<a id="oneisland-kindleroad"></a>

## One Island — Kindle Road

Fonte: tabela `sOneIslandKindleRoad_FireRed` de `game/src/data/wild_encounters.json`; mapa `OneIsland_KindleRoad`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mantine | 70–75 | 40% | 70–75 (40%) |
| Crobat | 70–75 | 20% | 70–75 (20%) |
| Exeggutor | 70–75 | 10% | 70–75 (10%) |
| Golduck | 70–75 | 10% | 70–75 (10%) |
| Lapras | 70–75 | 5% | 70–75 (5%) |
| Muk | 70–75 | 5% | 70–75 (5%) |
| Porygon2 | 70–75 | 4% | 70–75 (4%) |
| Rapidash | 70–75 | 4% | 70–75 (4%) |
| Salamence | 70–75 | 1% | 70–75 (1%) |
| Slaking | 70–75 | 1% | 70–75 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 70–75 | 90% | 70–75 (90%) |
| Golduck | 70–75 | 5% | 70–75 (5%) |
| Tentacruel | 70–75 | 5% | 70–75 (5%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Gligar | 70–75 | 60% | 70–75 (60%) |
| Pupitar | 70–75 | 30% | 70–75 (30%) |
| Aggron | 70–75 | 5% | 70–75 (5%) |
| Nidoking | 70–75 | 4% | 70–75 (4%) |
| Swampert | 70–75 | 1% | 70–75 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 70–75 | 70% | 70–75 (70%) |
| Seadra | 70–75 | 30% | 70–75 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 70–75 | 60% | 70–75 (60%) |
| Mantine | 70–75 | 20% | 70–75 (20%) |
| Omastar | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 70–75 | 40% | 70–75 (40%) |
| Quagsire | 70–75 | 40% | 70–75 (40%) |
| Octillery | 70–75 | 15% | 70–75 (15%) |
| Swampert | 70–75 | 4% | 70–75 (4%) |
| Milotic | 70–75 | 1% | 70–75 (1%) |


<a id="oneisland-treasurebeach"></a>

## One Island — Treasure Beach

Fonte: tabela `sOneIslandTreasureBeach_FireRed` de `game/src/data/wild_encounters.json`; mapa `OneIsland_TreasureBeach`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cradily | 70–75 | 20% | 70–75 (20%) |
| Vileplume | 70–75 | 20% | 70–75 (20%) |
| Exeggutor | 70–75 | 11% | 70–75 (11%) |
| Golduck | 70–75 | 10% | 70–75 (10%) |
| Politoed | 70–75 | 10% | 70–75 (10%) |
| Walrein | 70–75 | 10% | 70–75 (10%) |
| Feraligatr | 70–75 | 5% | 70–75 (5%) |
| Meganium | 70–75 | 5% | 70–75 (5%) |
| Sceptile | 70–75 | 4% | 70–75 (4%) |
| Vaporeon | 70–75 | 4% | 70–75 (4%) |
| Poliwrath | 70–75 | 1% | 70–75 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 70–75 | 60% | 70–75 (60%) |
| Pelipper | 70–75 | 30% | 70–75 (30%) |
| Lapras | 70–75 | 5% | 70–75 (5%) |
| Tentacruel | 70–75 | 5% | 70–75 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 70–75 | 70% | 70–75 (70%) |
| Wailmer | 70–75 | 30% | 70–75 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 70–75 | 60% | 70–75 (60%) |
| Lanturn | 70–75 | 20% | 70–75 (20%) |
| Octillery | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 70–75 | 40% | 70–75 (40%) |
| Sealeo | 70–75 | 40% | 70–75 (40%) |
| Octillery | 70–75 | 15% | 70–75 (15%) |
| Politoed | 70–75 | 4% | 70–75 (4%) |
| Kingdra | 70–75 | 1% | 70–75 (1%) |


<a id="pallettown"></a>

## Pallet Town

Fonte: tabela `sPalletTown_FireRed` de `game/src/data/wild_encounters.json`; mapa `PalletTown`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 60% | 54–59 (60%) |
| Azumarill | 54–59 | 30% | 54–59 (30%) |
| Walrein | 54–59 | 5% | 54–59 (5%) |
| Lapras | 54–59 | 4% | 54–59 (4%) |
| Huntail | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Feebas | 27–33 | 70% | 27–33 (70%) |
| Chinchou | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 80% | 44–50 (80%) |
| Quagsire | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 44–50 | 40% | 44–50 (40%) |
| Wailmer | 44–50 | 40% | 44–50 (40%) |
| Huntail | 44–50 | 15% | 44–50 (15%) |
| Crawdaunt | 44–50 | 4% | 44–50 (4%) |
| Marshtomp | 44–50 | 1% | 44–50 (1%) |


<a id="pokemonmansion-1f"></a>

## Pokemon Mansion — 1 F

Fonte: tabela `sPokemonMansion1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Nidoqueen | 62–67 | 30% | 62–67 (30%) |
| Magmar | 62–67 | 20% | 62–67 (20%) |
| Vileplume | 62–67 | 20% | 62–67 (20%) |
| Rapidash | 62–67 | 10% | 62–67 (10%) |
| Claydol | 62–67 | 5% | 62–67 (5%) |
| Pinsir | 62–67 | 5% | 62–67 (5%) |
| Flygon | 62–67 | 4% | 62–67 (4%) |
| Ninetales | 62–67 | 4% | 62–67 (4%) |
| Gengar | 62–67 | 1% | 62–67 (1%) |
| Umbreon | 62–67 | 1% | 62–67 (1%) |


<a id="pokemonmansion-2f"></a>

## Pokemon Mansion — 2 F

Fonte: tabela `sPokemonMansion2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Absol | 62–67 | 20% | 62–67 (20%) |
| Shiftry | 62–67 | 20% | 62–67 (20%) |
| Crobat | 62–67 | 10% | 62–67 (10%) |
| Gengar | 62–67 | 10% | 62–67 (10%) |
| Houndoom | 62–67 | 10% | 62–67 (10%) |
| Umbreon | 62–67 | 10% | 62–67 (10%) |
| Heracross | 62–67 | 5% | 62–67 (5%) |
| Scyther | 62–67 | 5% | 62–67 (5%) |
| Machamp | 62–67 | 4% | 62–67 (4%) |
| Sceptile | 62–67 | 4% | 62–67 (4%) |
| Ursaring | 62–67 | 1% | 62–67 (1%) |
| Venusaur | 62–67 | 1% | 62–67 (1%) |


<a id="pokemonmansion-3f"></a>

## Pokemon Mansion — 3 F

Fonte: tabela `sPokemonMansion3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Shiftry | 62–67 | 20% | 62–67 (20%) |
| Swalot | 62–67 | 20% | 62–67 (20%) |
| Rapidash | 62–67 | 15% | 62–67 (15%) |
| Arcanine | 62–67 | 14% | 62–67 (14%) |
| Crobat | 62–67 | 10% | 62–67 (10%) |
| Umbreon | 62–67 | 10% | 62–67 (10%) |
| Donphan | 62–67 | 5% | 62–67 (5%) |
| Gardevoir | 62–67 | 4% | 62–67 (4%) |
| Gengar | 62–67 | 1% | 62–67 (1%) |
| Nidoking | 62–67 | 1% | 62–67 (1%) |


<a id="pokemonmansion-b1f"></a>

## Pokemon Mansion — B1 F

Fonte: tabela `sPokemonMansionB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonMansion_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Absol | 62–67 | 20% | 62–67 (20%) |
| Magmar | 62–67 | 20% | 62–67 (20%) |
| Nidoking | 62–67 | 20% | 62–67 (20%) |
| Houndoom | 62–67 | 15% | 62–67 (15%) |
| Crobat | 62–67 | 11% | 62–67 (11%) |
| Alakazam | 62–67 | 5% | 62–67 (5%) |
| Meganium | 62–67 | 4% | 62–67 (4%) |
| Porygon2 | 62–67 | 4% | 62–67 (4%) |
| Arcanine | 62–67 | 1% | 62–67 (1%) |


<a id="pokemontower-3f"></a>

## Pokemon Tower — 3 F

Fonte: tabela `sPokemonTower3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Grumpig | 44–50 | 25% | 44–50 (25%) |
| Shedinja | 44–50 | 20% | 44–50 (20%) |
| Haunter | 44–50 | 11% | 44–50 (11%) |
| Medicham | 44–50 | 10% | 44–50 (10%) |
| Murkrow | 44–50 | 10% | 44–50 (10%) |
| Shiftry | 44–50 | 10% | 44–50 (10%) |
| Slowking | 44–50 | 5% | 44–50 (5%) |
| Dusclops | 44–50 | 4% | 44–50 (4%) |
| Wobbuffet | 44–50 | 4% | 44–50 (4%) |
| Beldum | 44–50 | 1% | 44–50 (1%) |


<a id="pokemontower-4f"></a>

## Pokemon Tower — 4 F

Fonte: tabela `sPokemonTower4F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_4F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Chimecho | 44–50 | 24% | 44–50 (24%) |
| Shedinja | 44–50 | 21% | 44–50 (21%) |
| Wynaut | 44–50 | 11% | 44–50 (11%) |
| Metang | 44–50 | 10% | 44–50 (10%) |
| Shiftry | 44–50 | 10% | 44–50 (10%) |
| Sneasel | 44–50 | 10% | 44–50 (10%) |
| Absol | 44–50 | 5% | 44–50 (5%) |
| Slowbro | 44–50 | 5% | 44–50 (5%) |
| Kadabra | 44–50 | 4% | 44–50 (4%) |


<a id="pokemontower-5f"></a>

## Pokemon Tower — 5 F

Fonte: tabela `sPokemonTower5F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_5F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Grumpig | 44–50 | 20% | 44–50 (20%) |
| Murkrow | 44–50 | 20% | 44–50 (20%) |
| Shedinja | 44–50 | 15% | 44–50 (15%) |
| Lunatone | 44–50 | 11% | 44–50 (11%) |
| Cacturne | 44–50 | 10% | 44–50 (10%) |
| Xatu | 44–50 | 10% | 44–50 (10%) |
| Hypno | 44–50 | 5% | 44–50 (5%) |
| Mr. Mime | 44–50 | 5% | 44–50 (5%) |
| Girafarig | 44–50 | 4% | 44–50 (4%) |


<a id="pokemontower-6f"></a>

## Pokemon Tower — 6 F

Fonte: tabela `sPokemonTower6F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_6F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cacturne | 44–50 | 25% | 44–50 (25%) |
| Solrock | 44–50 | 20% | 44–50 (20%) |
| Lunatone | 44–50 | 15% | 44–50 (15%) |
| Haunter | 44–50 | 11% | 44–50 (11%) |
| Chimecho | 44–50 | 10% | 44–50 (10%) |
| Slowking | 44–50 | 10% | 44–50 (10%) |
| Banette | 44–50 | 4% | 44–50 (4%) |
| Jynx | 44–50 | 4% | 44–50 (4%) |
| Metang | 44–50 | 1% | 44–50 (1%) |


<a id="pokemontower-7f"></a>

## Pokemon Tower — 7 F

Fonte: tabela `sPokemonTower7F_FireRed` de `game/src/data/wild_encounters.json`; mapa `PokemonTower_7F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Grumpig | 44–50 | 30% | 44–50 (30%) |
| Mr. Mime | 44–50 | 20% | 44–50 (20%) |
| Haunter | 44–50 | 11% | 44–50 (11%) |
| Medicham | 44–50 | 10% | 44–50 (10%) |
| Metang | 44–50 | 10% | 44–50 (10%) |
| Shiftry | 44–50 | 5% | 44–50 (5%) |
| Xatu | 44–50 | 5% | 44–50 (5%) |
| Beldum | 44–50 | 4% | 44–50 (4%) |
| Solrock | 44–50 | 4% | 44–50 (4%) |
| Slowking | 44–50 | 1% | 44–50 (1%) |


<a id="powerplant"></a>

## Power Plant

Fonte: tabela `sPowerPlant_FireRed` de `game/src/data/wild_encounters.json`; mapa `PowerPlant`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Lairon | 54–59 | 20% | 54–59 (20%) |
| Minun | 54–59 | 20% | 54–59 (20%) |
| Gengar | 54–59 | 10% | 54–59 (10%) |
| Muk | 54–59 | 10% | 54–59 (10%) |
| Nidoqueen | 54–59 | 10% | 54–59 (10%) |
| Scizor | 54–59 | 10% | 54–59 (10%) |
| Ampharos | 54–59 | 5% | 54–59 (5%) |
| Venusaur | 54–59 | 5% | 54–59 (5%) |
| Aggron | 54–59 | 4% | 54–59 (4%) |
| Jolteon | 54–59 | 4% | 54–59 (4%) |
| Nidoking | 54–59 | 1% | 54–59 (1%) |
| Steelix | 54–59 | 1% | 54–59 (1%) |


<a id="rocktunnel-1f"></a>

## Rock Tunnel — 1 F

Fonte: tabela `sRockTunnel1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `RockTunnel_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Marowak | 35–41 | 40% | 35–41 (40%) |
| Onix | 35–41 | 20% | 35–41 (20%) |
| Gligar | 35–41 | 15% | 35–41 (15%) |
| Pupitar | 35–41 | 10% | 35–41 (10%) |
| Magcargo | 38–41 | 5% | 38–41 (5%) |
| Corsola | 35–41 | 4% | 35–41 (4%) |
| Dugtrio | 35–41 | 4% | 35–41 (4%) |
| Combusken | 35–41 | 1% | 35–41 (1%) |
| Sudowoodo | 35–41 | 1% | 35–41 (1%) |


<a id="rocktunnel-b1f"></a>

## Rock Tunnel — B1 F

Fonte: tabela `sRockTunnelB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `RockTunnel_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sudowoodo | 35–41 | 30% | 35–41 (30%) |
| Corsola | 35–41 | 20% | 35–41 (20%) |
| Dugtrio | 35–41 | 20% | 35–41 (20%) |
| Vibrava | 35–41 | 10% | 35–41 (10%) |
| Lairon | 35–41 | 5% | 35–41 (5%) |
| Quagsire | 35–41 | 5% | 35–41 (5%) |
| Marowak | 35–41 | 4% | 35–41 (4%) |
| Ponyta | 35–41 | 4% | 35–41 (4%) |
| Charmeleon | 35–41 | 1% | 35–41 (1%) |
| Pupitar | 35–41 | 1% | 35–41 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Vibrava | 70–75 | 60% | 70–75 (60%) |
| Parasect | 70–75 | 30% | 70–75 (30%) |
| Swampert | 70–75 | 5% | 70–75 (5%) |
| Scizor | 70–75 | 4% | 70–75 (4%) |
| Heracross | 70–75 | 1% | 70–75 (1%) |


<a id="ssanne-exterior"></a>

## SSAnne — Exterior

Fonte: tabela `sSSAnneExterior_FireRed` de `game/src/data/wild_encounters.json`; mapa `SSAnne_Exterior`.

**Tabelas originais de acesso limitado:** a S.S. Anne parte antes da obtenção normal de Surf e das varas avançadas. Estes dados foram preservados, mas não contam como prova de captura das 386 espécies. Todas elas têm opções em áreas permanentes.

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
| Exploud | 44–50 | 20% | 44–50 (20%) |
| Golem | 44–50 | 20% | 44–50 (20%) |
| Nidorino | 44–50 | 10% | 44–50 (10%) |
| Parasect | 44–50 | 10% | 44–50 (10%) |
| Raticate | 44–50 | 10% | 44–50 (10%) |
| Tangela | 44–50 | 10% | 44–50 (10%) |
| Kangaskhan | 44–50 | 5% | 44–50 (5%) |
| Victreebel | 44–50 | 5% | 44–50 (5%) |
| Camerupt | 44–50 | 4% | 44–50 (4%) |
| Jumpluff | 44–50 | 4% | 44–50 (4%) |
| Grovyle | 44–50 | 1% | 44–50 (1%) |
| Noctowl | 44–50 | 1% | 44–50 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 60% | 54–59 (60%) |
| Pelipper | 54–59 | 30% | 54–59 (30%) |
| Omastar | 54–59 | 5% | 54–59 (5%) |
| Kingdra | 54–59 | 4% | 54–59 (4%) |
| Blastoise | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 70% | 44–50 (70%) |
| Pelipper | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 44–50 | 60% | 44–50 (60%) |
| Corsola | 44–50 | 20% | 44–50 (20%) |
| Slowking | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 44–50 | 44% | 44–50 (44%) |
| Qwilfish | 44–50 | 40% | 44–50 (40%) |
| Slowbro | 44–50 | 15% | 44–50 (15%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="safarizone-east"></a>

## Safari Zone — East

Fonte: tabela `sSafariZoneEast_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_East`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Kangaskhan | 44–50 | 20% | 44–50 (20%) |
| Marowak | 44–50 | 20% | 44–50 (20%) |
| Delcatty | 44–50 | 10% | 44–50 (10%) |
| Nidorino | 44–50 | 10% | 44–50 (10%) |
| Rhydon | 44–50 | 10% | 44–50 (10%) |
| Wigglytuff | 44–50 | 10% | 44–50 (10%) |
| Golem | 44–50 | 5% | 44–50 (5%) |
| Tauros | 44–50 | 5% | 44–50 (5%) |
| Dodrio | 44–50 | 4% | 44–50 (4%) |
| Pidgeot | 44–50 | 4% | 44–50 (4%) |
| Piloswine | 44–50 | 1% | 44–50 (1%) |
| Zangoose | 44–50 | 1% | 44–50 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 54–59 | 60% | 54–59 (60%) |
| Sealeo | 54–59 | 30% | 54–59 (30%) |
| Lanturn | 54–59 | 5% | 54–59 (5%) |
| Swampert | 54–59 | 4% | 54–59 (4%) |
| Milotic | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 44–50 | 70% | 44–50 (70%) |
| Azumarill | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 44–50 | 60% | 44–50 (60%) |
| Omastar | 44–50 | 20% | 44–50 (20%) |
| Seaking | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 40% | 44–50 (40%) |
| Corsola | 44–50 | 40% | 44–50 (40%) |
| Ludicolo | 44–50 | 15% | 44–50 (15%) |
| Relicanth | 44–50 | 4% | 44–50 (4%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="safarizone-north"></a>

## Safari Zone — North

Fonte: tabela `sSafariZoneNorth_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_North`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Miltank | 44–50 | 20% | 44–50 (20%) |
| Tauros | 44–50 | 20% | 44–50 (20%) |
| Delcatty | 44–50 | 10% | 44–50 (10%) |
| Linoone | 44–50 | 10% | 44–50 (10%) |
| Nidorino | 44–50 | 10% | 44–50 (10%) |
| Shiftry | 44–50 | 10% | 44–50 (10%) |
| Cradily | 44–50 | 5% | 44–50 (5%) |
| Exploud | 44–50 | 5% | 44–50 (5%) |
| Breloom | 44–50 | 4% | 44–50 (4%) |
| Vileplume | 44–50 | 4% | 44–50 (4%) |
| Chansey | 44–50 | 1% | 44–50 (1%) |
| Tropius | 44–50 | 1% | 44–50 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 60% | 54–59 (60%) |
| Azumarill | 54–59 | 30% | 54–59 (30%) |
| Omastar | 54–59 | 5% | 54–59 (5%) |
| Gorebyss | 54–59 | 4% | 54–59 (4%) |
| Whiscash | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 70% | 44–50 (70%) |
| Seadra | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 44–50 | 60% | 44–50 (60%) |
| Huntail | 44–50 | 20% | 44–50 (20%) |
| Mantine | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 44% | 44–50 (44%) |
| Quagsire | 44–50 | 40% | 44–50 (40%) |
| Lanturn | 44–50 | 15% | 44–50 (15%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="safarizone-west"></a>

## Safari Zone — West

Fonte: tabela `sSafariZoneWest_FireRed` de `game/src/data/wild_encounters.json`; mapa `SafariZone_West`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Golem | 44–50 | 20% | 44–50 (20%) |
| Linoone | 44–50 | 20% | 44–50 (20%) |
| Nidorino | 44–50 | 10% | 44–50 (10%) |
| Raticate | 44–50 | 10% | 44–50 (10%) |
| Sunflora | 44–50 | 10% | 44–50 (10%) |
| Tropius | 44–50 | 10% | 44–50 (10%) |
| Bellossom | 44–50 | 5% | 44–50 (5%) |
| Miltank | 44–50 | 5% | 44–50 (5%) |
| Rhydon | 44–50 | 4% | 44–50 (4%) |
| Stantler | 44–50 | 4% | 44–50 (4%) |
| Sandslash | 44–50 | 1% | 44–50 (1%) |
| Swellow | 44–50 | 1% | 44–50 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 60% | 54–59 (60%) |
| Pelipper | 54–59 | 30% | 54–59 (30%) |
| Golduck | 54–59 | 5% | 54–59 (5%) |
| Kabutops | 54–59 | 4% | 54–59 (4%) |
| Gorebyss | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 44–50 | 70% | 44–50 (70%) |
| Corsola | 44–50 | 30% | 44–50 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 44–50 | 60% | 44–50 (60%) |
| Azumarill | 44–50 | 20% | 44–50 (20%) |
| Qwilfish | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 44–50 | 40% | 44–50 (40%) |
| Wailmer | 44–50 | 40% | 44–50 (40%) |
| Sealeo | 44–50 | 15% | 44–50 (15%) |
| Pelipper | 44–50 | 4% | 44–50 (4%) |
| Dragonair | 44–50 | 1% | 44–50 (1%) |


<a id="seafoamislands-1f"></a>

## Seafoam Islands — 1 F

Fonte: tabela `sSeafoamIslands1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Kabutops | 54–59 | 35% | 54–59 (35%) |
| Glalie | 54–59 | 25% | 54–59 (25%) |
| Jynx | 54–59 | 20% | 54–59 (20%) |
| Walrein | 54–59 | 10% | 54–59 (10%) |
| Omastar | 54–59 | 5% | 54–59 (5%) |
| Politoed | 54–59 | 4% | 54–59 (4%) |
| Slowking | 54–59 | 1% | 54–59 (1%) |


<a id="seafoamislands-b1f"></a>

## Seafoam Islands — B1 F

Fonte: tabela `sSeafoamIslandsB1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Kabutops | 54–59 | 20% | 54–59 (20%) |
| Slowking | 54–59 | 20% | 54–59 (20%) |
| Golduck | 54–59 | 15% | 54–59 (15%) |
| Glalie | 54–59 | 10% | 54–59 (10%) |
| Jynx | 54–59 | 10% | 54–59 (10%) |
| Vaporeon | 54–59 | 10% | 54–59 (10%) |
| Feraligatr | 54–59 | 6% | 54–59 (6%) |
| Ludicolo | 54–59 | 5% | 54–59 (5%) |
| Shuckle | 54–59 | 4% | 54–59 (4%) |


<a id="seafoamislands-b2f"></a>

## Seafoam Islands — B2 F

Fonte: tabela `sSeafoamIslandsB2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slowbro | 54–59 | 21% | 54–59 (21%) |
| Ludicolo | 54–59 | 20% | 54–59 (20%) |
| Omastar | 54–59 | 20% | 54–59 (20%) |
| Golduck | 54–59 | 10% | 54–59 (10%) |
| Jynx | 54–59 | 10% | 54–59 (10%) |
| Kabutops | 54–59 | 10% | 54–59 (10%) |
| Lapras | 54–59 | 5% | 54–59 (5%) |
| Aerodactyl | 54–59 | 4% | 54–59 (4%) |


<a id="seafoamislands-b3f"></a>

## Seafoam Islands — B3 F

Fonte: tabela `sSeafoamIslandsB3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Mantine | 54–59 | 30% | 54–59 (30%) |
| Golduck | 54–59 | 25% | 54–59 (25%) |
| Glalie | 54–59 | 20% | 54–59 (20%) |
| Ludicolo | 54–59 | 10% | 54–59 (10%) |
| Dewgong | 54–59 | 5% | 54–59 (5%) |
| Slowbro | 54–59 | 5% | 54–59 (5%) |
| Jynx | 54–59 | 4% | 54–59 (4%) |
| Feraligatr | 54–59 | 1% | 54–59 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 54–59 | 60% | 54–59 (60%) |
| Corsola | 54–59 | 30% | 54–59 (30%) |
| Dewgong | 54–59 | 5% | 54–59 (5%) |
| Milotic | 54–59 | 4% | 54–59 (4%) |
| Golduck | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 54–59 | 70% | 54–59 (70%) |
| Qwilfish | 54–59 | 30% | 54–59 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 60% | 54–59 (60%) |
| Relicanth | 54–59 | 20% | 54–59 (20%) |
| Seaking | 54–59 | 20% | 54–59 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 40% | 54–59 (40%) |
| Qwilfish | 54–59 | 40% | 54–59 (40%) |
| Octillery | 54–59 | 15% | 54–59 (15%) |
| Swampert | 54–59 | 4% | 54–59 (4%) |
| Gyarados | 54–59 | 1% | 54–59 (1%) |


<a id="seafoamislands-b4f"></a>

## Seafoam Islands — B4 F

Fonte: tabela `sSeafoamIslandsB4F_FireRed` de `game/src/data/wild_encounters.json`; mapa `SeafoamIslands_B4F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Slowking | 54–59 | 24% | 54–59 (24%) |
| Dewgong | 54–59 | 20% | 54–59 (20%) |
| Jynx | 54–59 | 20% | 54–59 (20%) |
| Golduck | 54–59 | 15% | 54–59 (15%) |
| Glalie | 54–59 | 11% | 54–59 (11%) |
| Ludicolo | 54–59 | 10% | 54–59 (10%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 54–59 | 60% | 54–59 (60%) |
| Azumarill | 54–59 | 30% | 54–59 (30%) |
| Dewgong | 54–59 | 5% | 54–59 (5%) |
| Wailord | 54–59 | 4% | 54–59 (4%) |
| Golduck | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 70% | 54–59 (70%) |
| Corsola | 54–59 | 30% | 54–59 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 54–59 | 60% | 54–59 (60%) |
| Crawdaunt | 54–59 | 20% | 54–59 (20%) |
| Huntail | 54–59 | 20% | 54–59 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 54–59 | 40% | 54–59 (40%) |
| Qwilfish | 54–59 | 40% | 54–59 (40%) |
| Lanturn | 54–59 | 19% | 54–59 (19%) |
| Gyarados | 54–59 | 1% | 54–59 (1%) |


<a id="sevenisland-sevaultcanyon"></a>

## Seven Island — Sevault Canyon

Fonte: tabela `sSevenIslandSevaultCanyon_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_SevaultCanyon`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Altaria | 84–94 | 20% | 84–94 (20%) |
| Rhydon | 84–94 | 20% | 84–94 (20%) |
| Shuckle | 84–94 | 20% | 84–94 (20%) |
| Flygon | 84–94 | 10% | 84–94 (10%) |
| Nidoking | 84–94 | 10% | 84–94 (10%) |
| Steelix | 84–94 | 10% | 84–94 (10%) |
| Donphan | 84–94 | 4% | 84–94 (4%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |
| Regirock | 85–90 | 1% | 85–90 (1%) |
| Salamence | 84–94 | 1% | 84–94 (1%) |

### Rock Smash

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Shedinja | 84–94 | 60% | 84–94 (60%) |
| Corsola | 84–94 | 30% | 84–94 (30%) |
| Flygon | 84–94 | 5% | 84–94 (5%) |
| Claydol | 84–94 | 4% | 84–94 (4%) |
| Shuckle | 84–94 | 1% | 84–94 (1%) |


<a id="sevenisland-sevaultcanyon-entrance"></a>

## Seven Island — Sevault Canyon — Entrance

Fonte: tabela `sSevenIslandSevaultCanyonEntrance_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_SevaultCanyon_Entrance`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Aggron | 84–94 | 25% | 84–94 (25%) |
| Nidoqueen | 84–94 | 25% | 84–94 (25%) |
| Lunatone | 84–94 | 20% | 84–94 (20%) |
| Piloswine | 84–94 | 20% | 84–94 (20%) |
| Donphan | 84–94 | 4% | 84–94 (4%) |
| Swampert | 84–94 | 4% | 84–94 (4%) |
| Ho-Oh | 90–95 | 1% | 90–95 (1%) |
| Zapdos | 85–90 | 1% | 85–90 (1%) |


<a id="sevenisland-tanobyruins"></a>

## Seven Island — Tanoby Ruins

Fonte: tabela `sSevenIslandTanobyRuins_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 84–94 | 60% | 84–94 (60%) |
| Quagsire | 84–94 | 30% | 84–94 (30%) |
| Blastoise | 84–94 | 5% | 84–94 (5%) |
| Golduck | 84–94 | 4% | 84–94 (4%) |
| Starmie | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 70% | 84–94 (70%) |
| Sealeo | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 60% | 84–94 (60%) |
| Omastar | 84–94 | 20% | 84–94 (20%) |
| Whiscash | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 40% | 84–94 (40%) |
| Sealeo | 84–94 | 40% | 84–94 (40%) |
| Crawdaunt | 84–94 | 15% | 84–94 (15%) |
| Wailord | 84–94 | 4% | 84–94 (4%) |
| Lapras | 84–94 | 1% | 84–94 (1%) |


<a id="sevenisland-tanobyruins-dilfordchamber"></a>

## Seven Island — Tanoby Ruins — Dilford Chamber

Fonte: tabela `sSevenIslandTanobyRuinsDilfordChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_DilfordChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown P | 50 | 40% | 50 (40%) |
| Unown J | 50 | 20% | 50 (20%) |
| Unown L | 50 | 20% | 50 (20%) |
| Unown R | 50 | 14% | 50 (14%) |
| Unown Q | 50 | 6% | 50 (6%) |


<a id="sevenisland-tanobyruins-liptoochamber"></a>

## Seven Island — Tanoby Ruins — Liptoo Chamber

Fonte: tabela `sSevenIslandTanobyRuinsLiptooChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_LiptooChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown C | 50 | 50% | 50 (50%) |
| Unown D | 50 | 30% | 50 (30%) |
| Unown H | 50 | 14% | 50 (14%) |
| Unown U | 50 | 5% | 50 (5%) |
| Unown O | 50 | 1% | 50 (1%) |


<a id="sevenisland-tanobyruins-moneanchamber"></a>

## Seven Island — Tanoby Ruins — Monean Chamber

Fonte: tabela `sSevenIslandTanobyRuinsMoneanChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_MoneanChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown A | 50 | 99% | 50 (99%) |
| Unown ? | 50 | 1% | 50 (1%) |


<a id="sevenisland-tanobyruins-rixychamber"></a>

## Seven Island — Tanoby Ruins — Rixy Chamber

Fonte: tabela `sSevenIslandTanobyRuinsRixyChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_RixyChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown V | 50 | 50% | 50 (50%) |
| Unown W | 50 | 30% | 50 (30%) |
| Unown X | 50 | 10% | 50 (10%) |
| Unown M | 50 | 8% | 50 (8%) |
| Unown B | 50 | 2% | 50 (2%) |


<a id="sevenisland-tanobyruins-scufibchamber"></a>

## Seven Island — Tanoby Ruins — Scufib Chamber

Fonte: tabela `sSevenIslandTanobyRuinsScufibChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_ScufibChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown Y | 50 | 40% | 50 (40%) |
| Unown G | 50 | 25% | 50 (25%) |
| Unown T | 50 | 20% | 50 (20%) |
| Unown F | 50 | 13% | 50 (13%) |
| Unown K | 50 | 2% | 50 (2%) |


<a id="sevenisland-tanobyruins-viapoischamber"></a>

## Seven Island — Tanoby Ruins — Viapois Chamber

Fonte: tabela `sSevenIslandTanobyRuinsViapoisChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_ViapoisChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown Z | 50 | 99% | 50 (99%) |
| Unown ! | 50 | 1% | 50 (1%) |


<a id="sevenisland-tanobyruins-weepthchamber"></a>

## Seven Island — Tanoby Ruins — Weepth Chamber

Fonte: tabela `sSevenIslandTanobyRuinsWeepthChamber_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TanobyRuins_WeepthChamber`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Unown N | 50 | 60% | 50 (60%) |
| Unown S | 50 | 30% | 50 (30%) |
| Unown I | 50 | 8% | 50 (8%) |
| Unown E | 50 | 2% | 50 (2%) |


<a id="sevenisland-trainertower"></a>

## Seven Island — Trainer Tower

Fonte: tabela `sSevenIslandTrainerTower_FireRed` de `game/src/data/wild_encounters.json`; mapa `SevenIsland_TrainerTower`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 60% | 84–94 (60%) |
| Corsola | 84–94 | 30% | 84–94 (30%) |
| Golduck | 84–94 | 5% | 84–94 (5%) |
| Wailord | 84–94 | 4% | 84–94 (4%) |
| Milotic | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 70% | 84–94 (70%) |
| Qwilfish | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 60% | 84–94 (60%) |
| Dewgong | 84–94 | 20% | 84–94 (20%) |
| Kabutops | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 84–94 | 40% | 84–94 (40%) |
| Wailmer | 84–94 | 40% | 84–94 (40%) |
| Kingler | 84–94 | 15% | 84–94 (15%) |
| Wailord | 84–94 | 4% | 84–94 (4%) |
| Milotic | 84–94 | 1% | 84–94 (1%) |


<a id="sixisland-alteringcave"></a>

## Six Island — Altering Cave

Fonte: tabela `sSixIslandAlteringCave_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_AlteringCave`.

Conjunto 0 redesenhado para esta hack, com Deoxys raro após a Liga. Os conjuntos alternativos originais constam somente no apêndice.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Flygon | 84–94 | 30% | 84–94 (30%) |
| Cradily | 84–94 | 20% | 84–94 (20%) |
| Kabutops | 84–94 | 20% | 84–94 (20%) |
| Crobat | 84–94 | 10% | 84–94 (10%) |
| Aggron | 84–94 | 5% | 84–94 (5%) |
| Houndoom | 84–94 | 5% | 84–94 (5%) |
| Claydol | 84–94 | 4% | 84–94 (4%) |
| Umbreon | 84–94 | 4% | 84–94 (4%) |
| Deoxys | 85–90 | 1% | 85–90 (1%) |
| Swampert | 84–94 | 1% | 84–94 (1%) |


<a id="sixisland-greenpath"></a>

## Six Island — Green Path

Fonte: tabela `sSixIslandGreenPath_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_GreenPath`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 60% | 84–94 (60%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |
| Vaporeon | 84–94 | 5% | 84–94 (5%) |
| Walrein | 84–94 | 4% | 84–94 (4%) |
| Lapras | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 100% | 84–94 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 84–94 | 60% | 84–94 (60%) |
| Dewgong | 84–94 | 20% | 84–94 (20%) |
| Ludicolo | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 40% | 84–94 (40%) |
| Wailmer | 84–94 | 40% | 84–94 (40%) |
| Ludicolo | 84–94 | 15% | 84–94 (15%) |
| Blastoise | 84–94 | 4% | 84–94 (4%) |
| Kingdra | 84–94 | 1% | 84–94 (1%) |


<a id="sixisland-outcastisland"></a>

## Six Island — Outcast Island

Fonte: tabela `sSixIslandOutcastIsland_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_OutcastIsland`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 84–94 | 60% | 84–94 (60%) |
| Pelipper | 84–94 | 30% | 84–94 (30%) |
| Vaporeon | 84–94 | 5% | 84–94 (5%) |
| Politoed | 84–94 | 4% | 84–94 (4%) |
| Lugia | 90–95 | 1% | 90–95 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 70% | 84–94 (70%) |
| Wailmer | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Dewgong | 84–94 | 20% | 84–94 (20%) |
| Huntail | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 84–94 | 40% | 84–94 (40%) |
| Wailmer | 84–94 | 40% | 84–94 (40%) |
| Gorebyss | 84–94 | 15% | 84–94 (15%) |
| Tentacruel | 84–94 | 4% | 84–94 (4%) |
| Lapras | 84–94 | 1% | 84–94 (1%) |


<a id="sixisland-patternbush"></a>

## Six Island — Pattern Bush

Fonte: tabela `sSixIslandPatternBush_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_PatternBush`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Cradily | 84–94 | 30% | 84–94 (30%) |
| Shiftry | 84–94 | 20% | 84–94 (20%) |
| Exeggutor | 84–94 | 18% | 84–94 (18%) |
| Shuckle | 84–94 | 10% | 84–94 (10%) |
| Vileplume | 84–94 | 10% | 84–94 (10%) |
| Sceptile | 84–94 | 6% | 84–94 (6%) |
| Pinsir | 84–94 | 5% | 84–94 (5%) |
| Celebi | 85–90 | 1% | 85–90 (1%) |


<a id="sixisland-ruinvalley"></a>

## Six Island — Ruin Valley

Fonte: tabela `sSixIslandRuinValley_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_RuinValley`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Girafarig | 84–94 | 20% | 84–94 (20%) |
| Solrock | 84–94 | 20% | 84–94 (20%) |
| Claydol | 84–94 | 15% | 84–94 (15%) |
| Espeon | 84–94 | 14% | 84–94 (14%) |
| Aggron | 84–94 | 10% | 84–94 (10%) |
| Scizor | 84–94 | 10% | 84–94 (10%) |
| Exeggutor | 84–94 | 5% | 84–94 (5%) |
| Steelix | 84–94 | 5% | 84–94 (5%) |
| Registeel | 85–90 | 1% | 85–90 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 60% | 84–94 (60%) |
| Corsola | 84–94 | 30% | 84–94 (30%) |
| Blastoise | 84–94 | 5% | 84–94 (5%) |
| Cloyster | 84–94 | 4% | 84–94 (4%) |
| Kingdra | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 84–94 | 70% | 84–94 (70%) |
| Qwilfish | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 60% | 84–94 (60%) |
| Octillery | 84–94 | 20% | 84–94 (20%) |
| Relicanth | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 40% | 84–94 (40%) |
| Sealeo | 84–94 | 40% | 84–94 (40%) |
| Relicanth | 84–94 | 15% | 84–94 (15%) |
| Vaporeon | 84–94 | 4% | 84–94 (4%) |
| Gyarados | 84–94 | 1% | 84–94 (1%) |


<a id="sixisland-waterpath"></a>

## Six Island — Water Path

Fonte: tabela `sSixIslandWaterPath_FireRed` de `game/src/data/wild_encounters.json`; mapa `SixIsland_WaterPath`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Dewgong | 84–94 | 20% | 84–94 (20%) |
| Mr. Mime | 84–94 | 20% | 84–94 (20%) |
| Golduck | 84–94 | 10% | 84–94 (10%) |
| Houndoom | 84–94 | 10% | 84–94 (10%) |
| Machamp | 84–94 | 10% | 84–94 (10%) |
| Politoed | 84–94 | 10% | 84–94 (10%) |
| Flareon | 84–94 | 5% | 84–94 (5%) |
| Nidoqueen | 84–94 | 5% | 84–94 (5%) |
| Ampharos | 84–94 | 4% | 84–94 (4%) |
| Umbreon | 84–94 | 4% | 84–94 (4%) |
| Metagross | 84–94 | 1% | 84–94 (1%) |
| Sceptile | 84–94 | 1% | 84–94 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 84–94 | 60% | 84–94 (60%) |
| Seadra | 84–94 | 30% | 84–94 (30%) |
| Vaporeon | 84–94 | 5% | 84–94 (5%) |
| Blastoise | 84–94 | 4% | 84–94 (4%) |
| Kingdra | 84–94 | 1% | 84–94 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 84–94 | 70% | 84–94 (70%) |
| Quagsire | 84–94 | 30% | 84–94 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 84–94 | 60% | 84–94 (60%) |
| Huntail | 84–94 | 20% | 84–94 (20%) |
| Relicanth | 84–94 | 20% | 84–94 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 84–94 | 80% | 84–94 (80%) |
| Sharpedo | 84–94 | 15% | 84–94 (15%) |
| Wailord | 84–94 | 4% | 84–94 (4%) |
| Milotic | 84–94 | 1% | 84–94 (1%) |


<a id="threeisland-berryforest"></a>

## Three Island — Berry Forest

Fonte: tabela `sThreeIslandBerryForest_FireRed` de `game/src/data/wild_encounters.json`; mapa `ThreeIsland_BerryForest`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Jumpluff | 70–75 | 50% | 70–75 (50%) |
| Forretress | 70–75 | 10% | 70–75 (10%) |
| Shuckle | 70–75 | 10% | 70–75 (10%) |
| Venomoth | 70–75 | 10% | 70–75 (10%) |
| Scizor | 70–75 | 9% | 70–75 (9%) |
| Meganium | 70–75 | 5% | 70–75 (5%) |
| Pinsir | 70–75 | 5% | 70–75 (5%) |
| Sceptile | 70–75 | 1% | 70–75 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 70–75 | 60% | 70–75 (60%) |
| Qwilfish | 70–75 | 30% | 70–75 (30%) |
| Blastoise | 70–75 | 5% | 70–75 (5%) |
| Golduck | 70–75 | 5% | 70–75 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 70–75 | 70% | 70–75 (70%) |
| Seadra | 70–75 | 30% | 70–75 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 70–75 | 60% | 70–75 (60%) |
| Ludicolo | 70–75 | 20% | 70–75 (20%) |
| Slowbro | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Corsola | 70–75 | 40% | 70–75 (40%) |
| Wailmer | 70–75 | 40% | 70–75 (40%) |
| Crawdaunt | 70–75 | 15% | 70–75 (15%) |
| Feraligatr | 70–75 | 5% | 70–75 (5%) |


<a id="threeisland-bondbridge"></a>

## Three Island — Bond Bridge

Fonte: tabela `sThreeIslandBondBridge_FireRed` de `game/src/data/wild_encounters.json`; mapa `ThreeIsland_BondBridge`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Bellossom | 70–75 | 20% | 70–75 (20%) |
| Ludicolo | 70–75 | 20% | 70–75 (20%) |
| Exeggutor | 70–75 | 16% | 70–75 (16%) |
| Cradily | 70–75 | 10% | 70–75 (10%) |
| Shiftry | 70–75 | 10% | 70–75 (10%) |
| Venomoth | 70–75 | 10% | 70–75 (10%) |
| Heracross | 70–75 | 5% | 70–75 (5%) |
| Scyther | 70–75 | 4% | 70–75 (4%) |
| Venusaur | 70–75 | 4% | 70–75 (4%) |
| Shuckle | 70–75 | 1% | 70–75 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 70–75 | 60% | 70–75 (60%) |
| Seadra | 70–75 | 30% | 70–75 (30%) |
| Gyarados | 70–75 | 5% | 70–75 (5%) |
| Tentacruel | 70–75 | 5% | 70–75 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Qwilfish | 70–75 | 70% | 70–75 (70%) |
| Wailmer | 70–75 | 30% | 70–75 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 70–75 | 60% | 70–75 (60%) |
| Dewgong | 70–75 | 20% | 70–75 (20%) |
| Whiscash | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 70–75 | 80% | 70–75 (80%) |
| Lanturn | 70–75 | 15% | 70–75 (15%) |
| Poliwrath | 70–75 | 4% | 70–75 (4%) |
| Kingdra | 70–75 | 1% | 70–75 (1%) |


<a id="threeisland-port"></a>

## Three Island — Port

Fonte: tabela `sThreeIslandPort_FireRed` de `game/src/data/wild_encounters.json`; mapa `ThreeIsland_Port`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Electabuzz | 70–75 | 20% | 70–75 (20%) |
| Hariyama | 70–75 | 20% | 70–75 (20%) |
| Arcanine | 70–75 | 15% | 70–75 (15%) |
| Crobat | 70–75 | 15% | 70–75 (15%) |
| Porygon2 | 70–75 | 10% | 70–75 (10%) |
| Ursaring | 70–75 | 10% | 70–75 (10%) |
| Politoed | 70–75 | 4% | 70–75 (4%) |
| Scyther | 70–75 | 4% | 70–75 (4%) |
| Dragonite | 70–75 | 1% | 70–75 (1%) |
| Salamence | 70–75 | 1% | 70–75 (1%) |


<a id="twoisland-capebrink"></a>

## Two Island — Cape Brink

Fonte: tabela `sTwoIslandCapeBrink_FireRed` de `game/src/data/wild_encounters.json`; mapa `TwoIsland_CapeBrink`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Camerupt | 70–75 | 20% | 70–75 (20%) |
| Shiftry | 70–75 | 20% | 70–75 (20%) |
| Claydol | 70–75 | 10% | 70–75 (10%) |
| Gengar | 70–75 | 10% | 70–75 (10%) |
| Houndoom | 70–75 | 10% | 70–75 (10%) |
| Nidoqueen | 70–75 | 10% | 70–75 (10%) |
| Golduck | 70–75 | 5% | 70–75 (5%) |
| Machamp | 70–75 | 5% | 70–75 (5%) |
| Snorlax | 70–75 | 5% | 70–75 (5%) |
| Arcanine | 70–75 | 4% | 70–75 (4%) |
| Dragonite | 70–75 | 1% | 70–75 (1%) |

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 70–75 | 60% | 70–75 (60%) |
| Quagsire | 70–75 | 30% | 70–75 (30%) |
| Golduck | 70–75 | 5% | 70–75 (5%) |
| Milotic | 70–75 | 5% | 70–75 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 70–75 | 100% | 70–75 (100%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Sealeo | 70–75 | 60% | 70–75 (60%) |
| Crawdaunt | 70–75 | 20% | 70–75 (20%) |
| Omastar | 70–75 | 20% | 70–75 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Azumarill | 70–75 | 40% | 70–75 (40%) |
| Pelipper | 70–75 | 40% | 70–75 (40%) |
| Relicanth | 70–75 | 15% | 70–75 (15%) |
| Milotic | 70–75 | 4% | 70–75 (4%) |
| Blastoise | 70–75 | 1% | 70–75 (1%) |


<a id="vermilioncity"></a>

## Vermilion City

Fonte: tabela `sVermilionCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `VermilionCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 54–59 | 60% | 54–59 (60%) |
| Azumarill | 54–59 | 30% | 54–59 (30%) |
| Relicanth | 54–59 | 5% | 54–59 (5%) |
| Cloyster | 54–59 | 4% | 54–59 (4%) |
| Gyarados | 54–59 | 1% | 54–59 (1%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Barboach | 27–33 | 70% | 27–33 (70%) |
| Luvdisc | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Seadra | 44–50 | 80% | 44–50 (80%) |
| Huntail | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Quagsire | 44–50 | 40% | 44–50 (40%) |
| Seadra | 44–50 | 40% | 44–50 (40%) |
| Dewgong | 44–50 | 15% | 44–50 (15%) |
| Whiscash | 44–50 | 4% | 44–50 (4%) |
| Wailmer | 44–50 | 1% | 44–50 (1%) |


<a id="victoryroad-1f"></a>

## Victory Road — 1 F

Fonte: tabela `sVictoryRoad1F_FireRed` de `game/src/data/wild_encounters.json`; mapa `VictoryRoad_1F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Xatu | 70–75 | 20% | 70–75 (20%) |
| Zangoose | 70–75 | 20% | 70–75 (20%) |
| Espeon | 70–75 | 10% | 70–75 (10%) |
| Gengar | 70–75 | 10% | 70–75 (10%) |
| Shuckle | 70–75 | 10% | 70–75 (10%) |
| Walrein | 70–75 | 10% | 70–75 (10%) |
| Golduck | 70–75 | 5% | 70–75 (5%) |
| Houndoom | 70–75 | 5% | 70–75 (5%) |
| Aerodactyl | 70–75 | 4% | 70–75 (4%) |
| Meganium | 70–75 | 4% | 70–75 (4%) |
| Metagross | 70–75 | 2% | 70–75 (2%) |


<a id="victoryroad-2f"></a>

## Victory Road — 2 F

Fonte: tabela `sVictoryRoad2F_FireRed` de `game/src/data/wild_encounters.json`; mapa `VictoryRoad_2F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Hitmontop | 70–75 | 20% | 70–75 (20%) |
| Kangaskhan | 70–75 | 20% | 70–75 (20%) |
| Donphan | 70–75 | 10% | 70–75 (10%) |
| Exeggutor | 70–75 | 10% | 70–75 (10%) |
| Nidoqueen | 70–75 | 10% | 70–75 (10%) |
| Ninetales | 70–75 | 10% | 70–75 (10%) |
| Flygon | 70–75 | 5% | 70–75 (5%) |
| Poliwrath | 70–75 | 5% | 70–75 (5%) |
| Aerodactyl | 70–75 | 4% | 70–75 (4%) |
| Politoed | 70–75 | 4% | 70–75 (4%) |
| Rapidash | 70–75 | 1% | 70–75 (1%) |
| Tyranitar | 70–75 | 1% | 70–75 (1%) |


<a id="victoryroad-3f"></a>

## Victory Road — 3 F

Fonte: tabela `sVictoryRoad3F_FireRed` de `game/src/data/wild_encounters.json`; mapa `VictoryRoad_3F`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Hitmonchan | 70–75 | 20% | 70–75 (20%) |
| Tropius | 70–75 | 20% | 70–75 (20%) |
| Arcanine | 70–75 | 15% | 70–75 (15%) |
| Crobat | 70–75 | 10% | 70–75 (10%) |
| Golduck | 70–75 | 10% | 70–75 (10%) |
| Umbreon | 70–75 | 10% | 70–75 (10%) |
| Scizor | 70–75 | 9% | 70–75 (9%) |
| Porygon2 | 70–75 | 5% | 70–75 (5%) |
| Dragonite | 70–75 | 1% | 70–75 (1%) |


<a id="viridiancity"></a>

## Viridian City

Fonte: tabela `sViridianCity_FireRed` de `game/src/data/wild_encounters.json`; mapa `ViridianCity`.

### Surf

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 54–59 | 60% | 54–59 (60%) |
| Sealeo | 54–59 | 30% | 54–59 (30%) |
| Gyarados | 54–59 | 5% | 54–59 (5%) |
| Octillery | 54–59 | 5% | 54–59 (5%) |

### Old Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Shellder | 27–33 | 70% | 27–33 (70%) |
| Staryu | 27–33 | 30% | 27–33 (30%) |

### Good Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Wailmer | 44–50 | 80% | 44–50 (80%) |
| Slowking | 44–50 | 20% | 44–50 (20%) |

### Super Rod

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Pelipper | 44–50 | 40% | 44–50 (40%) |
| Poliwhirl | 44–50 | 40% | 44–50 (40%) |
| Kingler | 44–50 | 15% | 44–50 (15%) |
| Relicanth | 44–50 | 4% | 44–50 (4%) |
| Seadra | 44–50 | 1% | 44–50 (1%) |


<a id="viridianforest"></a>

## Viridian Forest

Fonte: tabela `sViridianForest_FireRed` de `game/src/data/wild_encounters.json`; mapa `ViridianForest`.

### Grama / caverna

| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |
|---|---|---:|---|
| Caterpie | 8–15 | 20% | 8–15 (20%) |
| Weedle | 8–15 | 20% | 8–15 (20%) |
| Paras | 8–15 | 10% | 8–15 (10%) |
| Pikachu | 8–15 | 10% | 8–15 (10%) |
| Shroomish | 8–15 | 10% | 8–15 (10%) |
| Wurmple | 8–15 | 10% | 8–15 (10%) |
| Kakuna | 8–15 | 5% | 8–15 (5%) |
| Metapod | 8–15 | 5% | 8–15 (5%) |
| Cascoon | 8–15 | 4% | 8–15 (4%) |
| Silcoon | 8–15 | 4% | 8–15 (4%) |
| Pineco | 8–15 | 1% | 8–15 (1%) |
| Spinarak | 8–15 | 1% | 8–15 (1%) |

## Encontros fixos e eventos

A espécie é fixa (100% ao acionar o evento disponível); isso não garante captura. As flags de história, itens e derrotas anteriores continuam necessárias. Eventos fixos derrotados não foram resetados; os novos encontros aleatórios oferecem outra forma de capturar todos os lendários, repetível no Normal e sujeita à cota de área no Hardcore.

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

O jogo usa o conjunto 0 redesenhado por padrão. Os oito conjuntos abaixo foram preservados da base e dependem de `VAR_ALTERING_CAVE_WILD_SET`; não foi implementado evento normal para selecioná-los. Essas espécies também foram distribuídas em áreas acessíveis, sem depender destas tabelas. Os conjuntos alternativos não se somam à tabela principal.


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

Distribuição reproduzível: `python3 scripts/balance_encounters.py --check`, com cobertura por espécie em `data/encounter_distribution.json`. Gerar novamente os guias: `python3 scripts/generate_guides.py`. Conferir sem escrever: `python3 scripts/generate_guides.py --check`. O gerador valida soma de 100% por método, número de slots, separação das varas, formas de Unown e duplicação somente em Altering Cave. Não altera a ROM.
