# Equipes e preparação por desafio

**Arquivo histórico da ROM v0.4.0-dev.** Não foi recalculado para os novos times, categorias e learnsets da prévia. Veja [a migração atual](MODERNIZATION.md).

Exemplos efetivamente testados. “Vitória registrada” significa resultado do combate reconstruído, condicionado à preparação descrita; não prova captura/caminhada/economia. Nas sequências, a ordem e os níveis abaixo vêm da memória antes da luta. Pokémon com HP zero no Hardcore já estavam perdidos e não participaram. IVs/EVs: HP / Ataque / Defesa / Velocidade / At. Esp. / Def. Esp. As probabilidades de origem são pesos de slots, não chances condicionais da tentativa Nuzlocke.

## 01. Rival — laboratório de Oak

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `tutorial-normal`, tentativa 2. Contexto: isolated. Preparação: `original_Oak_tutorial_status_2`. Resultado: 1; 5 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Squirtle | 5 / 20/20 | Tackle, Tail Whip | CALM | 19/25/22/10/29/6 | 0/0/0/0/0/0 | starter |

### Hardcore — composição testada, sem vitória registrada

Inicial: Bulbasaur. Dataset: `tutorial-hardcore`, tentativa 1. Contexto: isolated. Preparação: `original_Oak_tutorial_status_0`. Resultado: 2; 3 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Bulbasaur | 5 / 19/19 | Tackle, Growl | BASHFUL | 8/26/11/0/12/4 | 0/0/0/0/0/0 | starter |

## 02. Rival — Route 22, primeira visita

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `early-normal-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Azumarill | 18 / 79/79 | Water Gun, Tackle, Defense Curl, Tail Whip | QUIET | 25/19/18/25/6/12 | 252/0/4/0/252/0 | MAP_ROUTE22; land_mons, slot 4, peso 10%, nível 5–12; evoluções: MARILL→AZUMARILL no 18 |
| Mightyena | 18 / 65/65 | Bite, Tackle, Howl, Odor Sleuth | HARDY | 3/13/13/15/22/10 | 252/0/4/0/252/0 | MAP_ROUTE1; land_mons, slot 5, peso 10%, nível 3–6; evoluções: POOCHYENA→MIGHTYENA no 18 |
| Wooper | 18 / 62/62 | Mud Shot, Slam, Water Gun, Tail Whip | RELAXED | 19/3/5/17/21/5 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 5, peso 10%, nível 5–12 |
| Rattata | 18 / 51/51 | Hyper Fang, Quick Attack, Tackle, Tail Whip | HARDY | 5/15/29/15/29/23 | 252/252/4/0/0/0 | MAP_ROUTE1; land_mons, slot 1, peso 20%, nível 3–6 |
| Makuhita | 18 / 66/66 | Vital Throw, Tackle, Arm Thrust, Focus Energy | GENTLE | 7/5/23/16/28/7 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 9, peso 4%, nível 5–12 |
| Furret | 18 / 58/58 | Quick Attack, Scratch, Fury Swipes, Defense Curl | JOLLY | 1/16/25/8/21/13 | 4/252/0/252/0/0 | MAP_ROUTE1; land_mons, slot 2, peso 10%, nível 3–6; evoluções: SENTRET→FURRET no 15 |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `early-hardcore-charmander`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 0 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Mightyena | 18 / 70/70 | Bite, Tackle, Howl, Odor Sleuth | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE1; land_mons, slot 5, peso 10%, nível 3–6; evoluções: POOCHYENA→MIGHTYENA no 18 |
| Shroomish | 18 / 66/66 | Mega Drain, Tackle, Absorb, Leech Seed | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_VIRIDIAN_FOREST; land_mons, slot 4, peso 10%, nível 8–15 |
| Mankey | 18 / 59/59 | Karate Chop, Scratch, Fury Swipes, Low Kick | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 0, peso 20%, nível 5–12 |
| Butterfree | 18 / 66/66 | Confusion, Tackle, Sleep Powder, Harden | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE2; land_mons, slot 2, peso 10%, nível 5–12; evoluções: CATERPIE→METAPOD no 7, METAPOD→BUTTERFREE no 10 |
| Charmeleon | 18 / 65/65 | Ember, Scratch, Metal Claw, Growl | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | starter; evoluções: CHARMANDER→CHARMELEON no 16 |

## 03. Brock — Pewter Gym

### Normal — vitória registrada

Inicial: Charmander. Dataset: `early-normal-charmander`, tentativa 3. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 4 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Shroomish | 18 / 63/63 | Mega Drain, Absorb, Tackle, Leech Seed | MILD | 14/15/26/18/25/24 | 252/0/4/0/252/0 | MAP_VIRIDIAN_FOREST; land_mons, slot 4, peso 10%, nível 8–15 |
| Azumarill | 18 / 75/75 | Water Gun, Tackle, Defense Curl, Tail Whip | JOLLY | 1/12/20/7/5/3 | 252/0/4/0/252/0 | MAP_ROUTE22; land_mons, slot 4, peso 10%, nível 5–12; evoluções: MARILL→AZUMARILL no 18 |
| Bellsprout | 18 / 58/58 | Vine Whip, Wrap, Sleep Powder, Growth | LAX | 9/13/3/0/19/1 | 252/0/4/0/252/0 | MAP_ROUTE2; land_mons, slot 6, peso 5%, nível 5–12 |
| Oddish | 18 / 59/59 | Absorb, Sleep Powder, Poison Powder, Stun Spore | SASSY | 23/31/25/21/9/26 | 252/0/4/0/252/0 | MAP_ROUTE2; land_mons, slot 7, peso 5%, nível 5–12 |
| Sunkern | 18 / 53/53 | Mega Drain, Absorb, Growth, Ingrain | BRAVE | 18/28/13/13/4/25 | 252/0/4/0/252/0 | MAP_ROUTE1; land_mons, slot 8, peso 4%, nível 3–6 |
| Wooper | 18 / 61/61 | Water Gun, Mud Shot, Slam, Tail Whip | MODEST | 13/27/14/6/15/18 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 5, peso 10%, nível 5–12 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `early-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 4 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Azumarill | 18 / 80/80 | Water Gun, Tackle, Defense Curl, Tail Whip | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE22; land_mons, slot 4, peso 10%, nível 5–12; evoluções: MARILL→AZUMARILL no 18 |
| Ivysaur | 18 / 66/66 | Vine Whip, Tackle, Sleep Powder, Growl | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | starter; evoluções: BULBASAUR→IVYSAUR no 16 |
| Butterfree | 18 / 66/66 | Confusion, Sleep Powder, Harden, Poison Powder | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_VIRIDIAN_FOREST; land_mons, slot 6, peso 5%, nível 8–15; evoluções: METAPOD→BUTTERFREE no 10 |
| Rattata | 18 / 55/55 | Hyper Fang, Quick Attack, Tackle, Tail Whip | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE2; land_mons, slot 1, peso 20%, nível 5–12 |
| Pidgeotto | 18 / 67/67 | Gust, Quick Attack, Tackle, Sand Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE1; land_mons, slot 0, peso 20%, nível 3–6; evoluções: PIDGEY→PIDGEOTTO no 18 |

## 04. Rival — Cerulean City

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `early-normal-squirtle`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 3 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Magnemite | 28 / 71/71 | Spark, Sonic Boom, Thunder Shock, Tackle | RELAXED | 5/18/18/0/24/4 | 252/0/4/0/252/0 | MAP_MT_MOON_1F; land_mons, slot 1, peso 20%, nível 17–25 |
| Houndoom | 28 / 85/85 | Bite, Ember, Smog, Howl | CAREFUL | 17/1/4/7/11/23 | 4/0/0/252/252/0 | MAP_MT_MOON_B2F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: HOUNDOUR→HOUNDOOM no 24 |
| Hariyama | 28 / 143/143 | Vital Throw, Fake Out, Tackle, Arm Thrust | SASSY | 26/6/25/8/1/1 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 9, peso 4%, nível 5–12; evoluções: MAKUHITA→HARIYAMA no 24 |
| Golbat | 28 / 88/88 | Wing Attack, Bite, Astonish, Leech Life | LAX | 31/10/10/23/0/1 | 4/252/0/252/0/0 | MAP_MT_MOON_B1F; land_mons, slot 7, peso 5%, nível 17–25; evoluções: ZUBAT→GOLBAT no 22 |
| Graveler | 28 / 91/91 | Rock Throw, Magnitude, Defense Curl, Mud Sport | BRAVE | 18/17/17/28/24/25 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25 |
| Donphan | 28 / 111/111 | Take Down, Tackle, Fury Attack, Growl | ADAMANT | 21/18/31/0/2/30 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `early-hardcore-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Azumarill | 28 / 120/120 | Bubble Beam, Water Gun, Tackle, Defense Curl | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE22; land_mons, slot 4, peso 10%, nível 5–12; evoluções: MARILL→AZUMARILL no 18 |
| Raticate | 28 / 77/77 | Hyper Fang, Quick Attack, Tackle, Focus Energy | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE2; land_mons, slot 1, peso 20%, nível 5–12; evoluções: RATTATA→RATICATE no 20 |
| Voltorb | 28 / 69/69 | Spark, Sonic Boom, Tackle, Charge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE4; land_mons, slot 0, peso 20%, nível 17–25 |
| Gyarados | 28 / 117/117 | Dragon Rage, Bite, Tackle, Splash | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | purchase, Route4 Pokemon Center, 500; evoluções: MAGIKARP→GYARADOS no 20 |
| Pidgeotto | 28 / 99/99 | Wing Attack, Gust, Quick Attack, Tackle | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE1; land_mons, slot 0, peso 20%, nível 3–6; evoluções: PIDGEY→PIDGEOTTO no 18 |
| Ivysaur | 28 / 97/97 | Razor Leaf, Vine Whip, Tackle, Sleep Powder | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | starter; evoluções: BULBASAUR→IVYSAUR no 16 |

## 05. Misty — Cerulean Gym

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `early-normal-squirtle`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 6 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Ivysaur | 28 / 89/89 | Razor Leaf, Vine Whip, Sleep Powder, Leech Seed | NAUGHTY | 0/3/2/24/16/11 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 6, peso 5%, nível 17–25; evoluções: BULBASAUR→IVYSAUR no 18 |
| Bayleef | 28 / 95/95 | Razor Leaf, Synthesis, Growl, Poison Powder | HARDY | 24/7/14/31/30/10 | 252/0/4/0/252/0 | MAP_ROUTE24; land_mons, slot 6, peso 5%, nível 17–25; evoluções: CHIKORITA→BAYLEEF no 18 |
| Manectric | 28 / 85/85 | Spark, Quick Attack, Howl, Leer | QUIRKY | 29/22/17/24/12/19 | 4/0/0/252/252/0 | MAP_ROUTE4; land_mons, slot 8, peso 4%, nível 17–25; evoluções: ELECTRIKE→MANECTRIC no 26 |
| Quagsire | 28 / 116/116 | Mud Shot, Slam, Water Gun, Amnesia | LONELY | 28/8/4/10/9/12 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 5, peso 10%, nível 5–12; evoluções: WOOPER→QUAGSIRE no 20 |
| Magnemite | 28 / 71/71 | Spark, Thunder Shock, Sonic Boom, Tackle | RELAXED | 5/18/18/0/24/4 | 252/0/4/0/252/0 | MAP_MT_MOON_1F; land_mons, slot 1, peso 20%, nível 17–25 |
| Hariyama | 28 / 143/143 | Vital Throw, Fake Out, Tackle, Arm Thrust | SASSY | 26/6/25/8/1/1 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 9, peso 4%, nível 5–12; evoluções: MAKUHITA→HARIYAMA no 24 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `early-hardcore-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 4 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Ivysaur | 28 / 97/97 | Razor Leaf, Vine Whip, Tackle, Sleep Powder | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | starter; evoluções: BULBASAUR→IVYSAUR no 16 |
| Bayleef | 28 / 97/97 | Razor Leaf, Synthesis, Growl, Poison Powder | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE24; land_mons, slot 6, peso 5%, nível 17–25; evoluções: CHIKORITA→BAYLEEF no 18 |
| Gyarados | 28 / 117/117 | Dragon Rage, Bite, Tackle, Splash | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | purchase, Route4 Pokemon Center, 500; evoluções: MAGIKARP→GYARADOS no 20 |
| Voltorb | 28 / 69/69 | Spark, Sonic Boom, Tackle, Charge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE4; land_mons, slot 0, peso 20%, nível 17–25 |
| Spoink | 28 / 97/97 | Psybeam, Psywave, Confuse Ray, Magic Coat | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25 |
| Raticate | 28 / 77/77 | Hyper Fang, Quick Attack, Tackle, Focus Energy | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE2; land_mons, slot 1, peso 20%, nível 5–12; evoluções: RATTATA→RATICATE no 20 |

## 06. Rival — S.S. Anne

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `early-normal-squirtle`, tentativa 3. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 4 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Lairon | 36 / 118/118 | Iron Tail, Take Down, Metal Claw, Headbutt | QUIET | 18/6/21/18/5/0 | 252/252/4/0/0/0 | MAP_MT_MOON_1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: ARON→LAIRON no 32 |
| Cacturne | 36 / 119/119 | Faint Attack, Absorb, Pin Missile, Growth | JOLLY | 0/26/24/13/2/25 | 252/0/4/0/252/0 | MAP_ROUTE24; land_mons, slot 1, peso 20%, nível 17–25; evoluções: CACNEA→CACTURNE no 32 |
| Magneton | 36 / 106/106 | Spark, Thunder Shock, Sonic Boom, Tackle | RELAXED | 5/18/18/0/24/4 | 252/0/4/0/252/0 | MAP_MT_MOON_1F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: MAGNEMITE→MAGNETON no 30 |
| Gardevoir | 36 / 125/125 | Psychic, Confusion, Growl, Calm Mind | LAX | 22/28/3/25/14/20 | 252/0/4/0/252/0 | MAP_ROUTE2; land_mons, slot 8, peso 4%, nível 5–12; evoluções: RALTS→KIRLIA no 20, KIRLIA→GARDEVOIR no 30 |
| Slaking | 36 / 156/156 | Slash, Scratch, Fury Swipes, Encore | CAREFUL | 7/3/4/22/21/9 | 4/252/0/252/0/0 | MAP_ROUTE25; land_mons, slot 10, peso 1%, nível 17–25; evoluções: SLAKOTH→VIGOROTH no 18, VIGOROTH→SLAKING no 36 |
| Hariyama | 36 / 181/181 | Vital Throw, Smelling Salt, Fake Out, Tackle | SASSY | 26/6/25/8/1/1 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 9, peso 4%, nível 5–12; evoluções: MAKUHITA→HARIYAMA no 24 |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `early-hardcore-squirtle`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 3 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Charizard | 36 / 113/113 | Flamethrower, Wing Attack, Ember, Metal Claw | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE3; land_mons, slot 6, peso 5%, nível 17–25; evoluções: CHARMANDER→CHARMELEON no 18, CHARMELEON→CHARIZARD no 36 |
| Nuzleaf | 36 / 130/130 | Faint Attack, Fake Out, Growth, Harden | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE2; land_mons, slot 5, peso 10%, nível 5–12; evoluções: SEEDOT→NUZLEAF no 14 |
| Primeape | 36 / 104/104 | Cross Chop, Seismic Toss, Karate Chop, Scratch | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE22; land_mons, slot 0, peso 20%, nível 5–12; evoluções: MANKEY→PRIMEAPE no 28 |
| Hypno | 36 / 141/141 | Psychic, Headbutt, Confusion, Meditate | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE11; land_mons, slot 11, peso 1%, nível 27–33; evoluções: DROWZEE→HYPNO no 28 |
| Mightyena | 36 / 130/130 | Bite, Tackle, Howl, Odor Sleuth | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE1; land_mons, slot 5, peso 10%, nível 3–6; evoluções: POOCHYENA→MIGHTYENA no 18 |
| Corsola | 36 / 119/119 | Bubble Beam, Rock Blast, Bubble, Recover | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 2, peso 10%, nível 27–33 |

## 07. Lt. Surge — Vermilion Gym

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `early-normal-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 2 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Geodude | 36 / 97/97 | Earthquake, Rock Throw, Rock Blast, Magnitude | LONELY | 1/5/11/6/20/10 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25 |
| Whiscash | 36 / 149/149 | Earthquake, Magnitude, Amnesia, Rest | LONELY | 5/22/3/2/15/6 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Wooper | 36 / 115/115 | Earthquake, Mud Shot, Slam, Water Gun | RELAXED | 19/3/5/17/21/5 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 5, peso 10%, nível 5–12 |
| Marowak | 36 / 117/117 | Bone Club, Bonemerang, Headbutt, Focus Energy | SERIOUS | 15/4/17/21/21/9 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: CUBONE→MAROWAK no 28 |
| Numel | 36 / 119/119 | Earthquake, Take Down, Ember, Tackle | HARDY | 22/5/30/0/1/24 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25 |
| Breloom | 36 / 121/121 | Sky Uppercut, Mach Punch, Headbutt, Mega Drain | ADAMANT | 27/14/28/20/2/21 | 252/252/4/0/0/0 | MAP_VIRIDIAN_FOREST; land_mons, slot 4, peso 10%, nível 8–15; evoluções: SHROOMISH→BRELOOM no 23 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `early-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 3 turnos contados pelo motor, 2 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Whiscash | 36 / 159/159 | Earthquake, Magnitude, Amnesia, Rest | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Venusaur | 36 / 137/137 | Razor Leaf, Vine Whip, Tackle, Sleep Powder | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | starter; evoluções: BULBASAUR→IVYSAUR no 16, IVYSAUR→VENUSAUR no 32 |
| Meganium | 36 / 137/137 | Razor Leaf, Body Slam, Synthesis, Growl | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE24; land_mons, slot 6, peso 5%, nível 17–25; evoluções: CHIKORITA→BAYLEEF no 18, BAYLEEF→MEGANIUM no 32 |
| Grumpig | 36 / 137/137 | Psybeam, Psywave, Confuse Ray, Magic Coat | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Dugtrio | 36 / 82/82 | Mud Slap, Fury Swipes, Magnitude, Dig | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 1, peso 20%, nível 27–33; evoluções: DIGLETT→DUGTRIO no 28 |
| Raticate | 36 / 97/97 | Hyper Fang, Quick Attack, Tackle, Pursuit | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE2; land_mons, slot 1, peso 20%, nível 5–12; evoluções: RATTATA→RATICATE no 20 |

## 08. Erika — Celadon Gym

### Normal — vitória registrada

Inicial: Charmander. Dataset: `early-normal-charmander`, tentativa 3. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Charizard | 44 / 126/126 | Flamethrower, Wing Attack, Ember, Slash | RELAXED | 8/26/8/27/11/21 | 4/0/0/252/252/0 | starter; evoluções: CHARMANDER→CHARMELEON no 16, CHARMELEON→CHARIZARD no 36 |
| Magcargo | 44 / 134/134 | Flamethrower, Ember, Rock Throw, Smog | CALM | 20/9/2/16/1/3 | 252/0/4/0/252/0 | MAP_ROUTE5; land_mons, slot 0, peso 20%, nível 27–33; evoluções: SLUGMA→MAGCARGO no 38 |
| Muk | 44 / 183/183 | Sludge, Pound, Acid Armor, Disable | QUIET | 22/3/5/13/8/19 | 252/252/4/0/0/0 | MAP_ROUTE3; land_mons, slot 2, peso 10%, nível 17–25; evoluções: GRIMER→MUK no 38 |
| Blaziken | 44 / 163/163 | Blaze Kick, Ember, Peck, Slash | RASH | 26/31/2/18/22/23 | 252/0/4/0/252/0 | MAP_ROUTE4; land_mons, slot 6, peso 5%, nível 17–25; evoluções: TORCHIC→COMBUSKEN no 18, COMBUSKEN→BLAZIKEN no 36 |
| Glalie | 44 / 155/155 | Ice Beam, Icy Wind, Crunch, Bite | SERIOUS | 8/1/28/4/24/31 | 252/0/4/0/252/0 | MAP_ROUTE4; land_mons, slot 9, peso 4%, nível 17–25; evoluções: SNORUNT→GLALIE no 42 |
| Metang | 44 / 135/135 | Psychic, Metal Claw, Confusion, Pursuit | BOLD | 2/15/20/8/24/2 | 252/0/4/0/252/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41 |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `early-hardcore-charmander`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 3 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Magcargo | 44 / 139/139 | Flamethrower, Ember, Rock Throw, Smog | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE5; land_mons, slot 0, peso 20%, nível 27–33; evoluções: SLUGMA→MAGCARGO no 38 |
| Charizard | 44 / 136/136 | Flamethrower, Wing Attack, Ember, Slash | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | starter; evoluções: CHARMANDER→CHARMELEON no 16, CHARMELEON→CHARIZARD no 36 |
| Swalot | 44 / 183/183 | Sludge, Body Slam, Amnesia, Encore | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE24; land_mons, slot 3, peso 10%, nível 17–25; evoluções: GULPIN→SWALOT no 26 |
| Grumpig | 44 / 165/165 | Psychic, Psybeam, Psywave, Confuse Ray | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Pelipper | 44 / 148/148 | Wing Attack, Water Gun, Growl, Mist | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE4; land_mons, slot 11, peso 1%, nível 17–25; evoluções: WINGULL→PELIPPER no 25 |
| Volbeat | 44 / 125/125 | Signal Beam, Double Edge, Helping Hand, Protect | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE9; land_mons, slot 3, peso 10%, nível 35–41 |

## 09. Giovanni — Rocket Hideout

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `early-normal-squirtle`, tentativa 3. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Swampert | 52 / 200/200 | Muddy Water, Earthquake, Water Gun, Mud Slap | QUIET | 3/9/29/9/19/0 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 7, peso 5%, nível 17–25; evoluções: MUDKIP→MARSHTOMP no 18, MARSHTOMP→SWAMPERT no 36 |
| Seadra | 52 / 128/128 | Hydro Pump, Water Gun, Twister, Agility | QUIET | 17/10/27/13/15/20 | 4/0/0/252/252/0 | MAP_CELADON_CITY; fishing_mons, slot 0, peso 70%, nível 35–41 |
| Aggron | 52 / 171/171 | Iron Tail, Take Down, Iron Defense, Metal Sound | ADAMANT | 7/10/15/23/20/3 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_B1F; land_mons, slot 6, peso 5%, nível 35–41; evoluções: LAIRON→AGGRON no 42 |
| Lanturn | 52 / 229/229 | Hydro Pump, Water Gun, Take Down, Spark | NAUGHTY | 9/20/27/15/29/25 | 252/0/4/0/252/0 | MAP_PALLET_TOWN; fishing_mons, slot 1, peso 30%, nível 27–33; evoluções: CHINCHOU→LANTURN no 28 |
| Sceptile | 52 / 142/142 | Leaf Blade, Absorb, Pursuit, Slam | SERIOUS | 14/30/2/5/23/24 | 4/0/0/252/252/0 | MAP_ROUTE24; land_mons, slot 7, peso 5%, nível 17–25; evoluções: TREECKO→GROVYLE no 18, GROVYLE→SCEPTILE no 36 |
| Octillery | 52 / 183/183 | Bubble Beam, Aurora Beam, Psybeam, Focus Energy | QUIET | 20/12/2/9/31/30 | 252/0/4/0/252/0 | MAP_CERULEAN_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: REMORAID→OCTILLERY no 28 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `early-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Seadra | 52 / 135/135 | Hydro Pump, Water Gun, Twister, Agility | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_CELADON_CITY; fishing_mons, slot 0, peso 70%, nível 35–41 |
| Octillery | 52 / 188/188 | Bubble Beam, Aurora Beam, Psybeam, Focus Energy | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_CERULEAN_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: REMORAID→OCTILLERY no 28 |
| Venusaur | 52 / 194/194 | Razor Leaf, Vine Whip, Tackle, Sleep Powder | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | starter; evoluções: BULBASAUR→IVYSAUR no 16, IVYSAUR→VENUSAUR no 32 |
| Gyarados | 52 / 209/209 | Hydro Pump, Dragon Rage, Bite, Twister | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | purchase, Route4 Pokemon Center, 500; evoluções: MAGIKARP→GYARADOS no 20 |
| Meganium | 52 / 194/194 | Razor Leaf, Body Slam, Synthesis, Growl | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE24; land_mons, slot 6, peso 5%, nível 17–25; evoluções: CHIKORITA→BAYLEEF no 18, BAYLEEF→MEGANIUM no 32 |
| Marill | 52 / 183/183 | Hydro Pump, Bubble Beam, Water Gun, Double Edge | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE22; land_mons, slot 4, peso 10%, nível 5–12 |

## 10. Rival — Pokémon Tower

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `early-normal-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 6 turnos contados pelo motor, 3 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Sudowoodo | 52 / 178/178 | Rock Slide, Slam, Faint Attack, Low Kick | GENTLE | 21/16/27/28/7/8 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_B1F; land_mons, slot 1, peso 20%, nível 35–41 |
| Houndoom | 52 / 148/148 | Flamethrower, Bite, Faint Attack, Ember | CALM | 16/14/9/6/6/7 | 4/0/0/252/252/0 | MAP_MT_MOON_B2F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: HOUNDOUR→HOUNDOOM no 24 |
| Ampharos | 52 / 198/198 | Thunder Punch, Thunder Shock, Tackle, Growl | BRAVE | 20/24/27/22/21/12 | 252/0/4/0/252/0 | MAP_ROUTE4; land_mons, slot 10, peso 1%, nível 17–25; evoluções: MAREEP→FLAAFFY no 18, FLAAFFY→AMPHAROS no 30 |
| Cacturne | 52 / 169/169 | Faint Attack, Needle Arm, Pin Missile, Absorb | BRAVE | 4/24/8/20/11/2 | 252/0/4/0/252/0 | MAP_ROUTE24; land_mons, slot 1, peso 20%, nível 17–25; evoluções: CACNEA→CACTURNE no 32 |
| Walrein | 52 / 215/215 | Blizzard, Aurora Beam, Body Slam, Hail | SASSY | 13/31/0/28/13/17 | 252/0/4/0/252/0 | MAP_ROUTE7; land_mons, slot 2, peso 10%, nível 35–41; evoluções: SEALEO→WALREIN no 44 |
| Camerupt | 52 / 179/179 | Eruption, Rock Slide, Take Down, Ember | HARDY | 22/5/30/0/1/24 | 252/0/4/0/252/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `early-hardcore-squirtle`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 7 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Walrein | 52 / 225/225 | Blizzard, Aurora Beam, Body Slam, Hail | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE7; land_mons, slot 2, peso 10%, nível 35–41; evoluções: SEALEO→WALREIN no 44 |
| Gyarados | 52 / 209/209 | Hydro Pump, Dragon Rage, Bite, Tackle | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | purchase, Route4 Pokemon Center, 500; evoluções: MAGIKARP→GYARADOS no 20 |
| Mightyena | 52 / 183/183 | Crunch, Take Down, Bite, Thief | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE1; land_mons, slot 5, peso 10%, nível 3–6; evoluções: POOCHYENA→MIGHTYENA no 18 |
| Hypno | 52 / 199/199 | Psychic, Headbutt, Confusion, Meditate | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE11; land_mons, slot 11, peso 1%, nível 27–33; evoluções: DROWZEE→HYPNO no 28 |
| Spinda | 52 / 173/173 | Double Edge, Dizzy Punch, Faint Attack, Psybeam | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE6; land_mons, slot 9, peso 4%, nível 27–33 |
| Charizard | 52 / 159/159 | Flamethrower, Wing Attack, Slash, Ember | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE3; land_mons, slot 6, peso 5%, nível 17–25; evoluções: CHARMANDER→CHARMELEON no 18, CHARMELEON→CHARIZARD no 36 |

## 11. Koga — Fuchsia Gym

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `prepared-normal-squirtle`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 3 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Metagross | 52 / 179/179 | Psychic, Confusion, Metal Claw, Pursuit | CAREFUL | 2/19/20/5/31/18 | 252/0/4/0/252/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Camerupt | 52 / 180/180 | Eruption, Earthquake, Rock Slide, Ember | GENTLE | 25/5/18/1/25/6 | 252/0/4/0/252/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |
| Quagsire | 52 / 208/208 | Earthquake, Slam, Mud Shot, Water Gun | LONELY | 28/8/4/10/9/12 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 5, peso 10%, nível 5–12; evoluções: WOOPER→QUAGSIRE no 20 |
| Golem | 52 / 187/187 | Earthquake, Rock Throw, Rock Blast, Magnitude | BRAVE | 18/17/17/28/24/25 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Magneton | 52 / 149/149 | Spark, Thunder Shock, Tri Attack, Sonic Boom | RELAXED | 5/18/18/0/24/4 | 252/0/4/0/252/0 | MAP_MT_MOON_1F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: MAGNEMITE→MAGNETON no 30 |
| Lunatone | 52 / 176/176 | Psychic, Rock Throw, Psywave | RELAXED | 18/14/11/15/22/2 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 6, peso 5%, nível 35–41 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 4. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 5 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Grumpig | 52 / 194/194 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Torkoal | 52 / 183/183 | Flamethrower, Heat Wave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 6, peso 5%, nível 44–50 |
| Whiscash | 52 / 225/225 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Solrock | 52 / 183/183 | Rock Slide, Rock Throw, Fire Spin, Psywave | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 1, peso 20%, nível 35–41 |
| Rhydon | 52 / 220/220 | Take Down, Rock Blast, Fury Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Manectric | 52 / 151/151 | Thunder, Spark, Bite | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE17; land_mons, slot 7, peso 5%, nível 44–50 |

## 12. Koichi — Fighting Dojo de Saffron

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Banette | 60 / 202/202 | Shadow Ball, Night Shade, Faint Attack, Knock Off | IMPISH | 30/15/20/3/0/17 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 2, peso 10%, nível 17–25; evoluções: SHUPPET→BANETTE no 37 |
| Altaria | 60 / 198/198 | Sky Attack, Take Down, Dragon Breath | BRAVE | 1/6/31/10/18/4 | 252/252/4/0/0/0 | MAP_ROUTE17; land_mons, slot 6, peso 5%, nível 44–50 |
| Dragonite | 60 / 230/230 | Wing Attack, Slam, Dragon Rage | RELAXED | 23/5/7/3/22/12 | 252/252/4/0/0/0 | MAP_ROUTE12; fishing_mons, slot 9, peso 1%, nível 44–50; evoluções: DRATINI→DRAGONAIR no 45, DRAGONAIR→DRAGONITE no 55 |
| Slowbro | 60 / 232/232 | Psychic, Headbutt | CALM | 18/22/24/0/6/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 7, peso 15%, nível 44–50 |
| Sableye | 60 / 176/176 | Shadow Ball, Astonish, Fake Out, Faint Attack | GENTLE | 14/29/14/3/24/24 | 252/252/4/0/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 5, peso 10%, nível 27–33 |
| Donphan | 60 / 218/218 | Earthquake, Take Down, Tackle, Rapid Spin | GENTLE | 5/8/19/0/8/7 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Dusclops | 60 / 174/174 | Shadow Punch, Pursuit | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 2, peso 10%, nível 44–50 |
| Grumpig | 60 / 222/222 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Rhydon | 60 / 252/252 | Earthquake, Take Down, Rock Blast, Fury Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Machamp | 60 / 234/234 | Cross Chop, Vital Throw, Submission, Revenge | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE16; land_mons, slot 0, peso 20%, nível 44–50; evoluções: MACHOKE→MACHAMP no 45 |
| Pidgeot | 60 / 188/188 | Wing Attack, Gust, Quick Attack, Tackle | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE1; land_mons, slot 0, peso 20%, nível 3–6; evoluções: PIDGEY→PIDGEOTTO no 18, PIDGEOTTO→PIDGEOT no 36 |
| Whiscash | 60 / 258/258 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |

## 13. Rival — Silph Co.

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 4 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 60 / 237/237 | Rock Slide, Crunch | CAREFUL | 16/24/10/23/14/5 | 252/0/4/0/252/0 | MAP_ROUTE13; land_mons, slot 8, peso 4%, nível 44–50; evoluções: LARVITAR→PUPITAR no 45, PUPITAR→TYRANITAR no 55 |
| Ampharos | 60 / 227/227 | Thunder, Thunder Punch, Thunder Shock, Tackle | BRAVE | 20/24/27/22/21/12 | 252/0/4/0/252/0 | MAP_ROUTE4; land_mons, slot 10, peso 1%, nível 17–25; evoluções: MAREEP→FLAAFFY no 18, FLAAFFY→AMPHAROS no 30 |
| Houndoom | 60 / 170/170 | Crunch, Flamethrower, Bite, Faint Attack | CALM | 16/14/9/6/6/7 | 4/0/0/252/252/0 | MAP_MT_MOON_B2F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: HOUNDOUR→HOUNDOOM no 24 |
| Metagross | 60 / 214/214 | Meteor Mash, Take Down | ADAMANT | 17/8/3/16/30/2 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46 |
| Lapras | 60 / 281/281 | Ice Beam, Hydro Pump | GENTLE | 30/5/10/31/6/26 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59 |
| Altaria | 60 / 198/198 | Sky Attack, Dragon Breath, Take Down | BRAVE | 1/6/31/10/18/4 | 252/252/4/0/0/0 | MAP_ROUTE17; land_mons, slot 6, peso 5%, nível 44–50 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 4 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Solrock | 60 / 210/210 | Rock Slide, Rock Throw, Fire Spin, Psywave | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 1, peso 20%, nível 35–41 |
| Walrein | 60 / 258/258 | Blizzard | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE19; water_mons, slot 1, peso 30%, nível 54–59; evoluções: SEALEO→WALREIN no 55 |
| Raichu | 60 / 161/161 | Thunderbolt, Thunder Shock, Quick Attack | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE18; land_mons, slot 7, peso 5%, nível 44–50 |
| Dusclops | 60 / 174/174 | Shadow Punch, Pursuit | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 2, peso 10%, nível 44–50 |
| Octillery | 60 / 216/216 | Ice Beam, Bubble Beam, Aurora Beam, Psybeam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_CERULEAN_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: REMORAID→OCTILLERY no 28 |
| Torkoal | 60 / 210/210 | Flamethrower, Heat Wave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 6, peso 5%, nível 44–50 |

## 14. Giovanni — Silph Co.

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 4 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 60 / 247/247 | Earthquake, Take Down, Horn Attack, Stomp | IMPISH | 23/16/12/7/11/21 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Wailord | 60 / 315/315 | Water Spout, Hydro Pump, Water Pulse | DOCILE | 6/4/23/23/28/14 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_WEST; fishing_mons, slot 0, peso 70%, nível 44–50; evoluções: WAILMER→WAILORD no 45 |
| Golem | 60 / 204/204 | Earthquake, Rock Throw, Rock Blast, Magnitude | LONELY | 1/5/11/6/20/10 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Milotic | 60 / 238/238 | Hydro Pump | LAX | 28/27/18/14/13/18 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B3F; water_mons, slot 3, peso 4%, nível 54–59 |
| Metagross | 60 / 211/211 | Psychic, Meteor Mash, Confusion, Metal Claw | SASSY | 12/11/17/1/12/10 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Donphan | 60 / 218/218 | Earthquake, Take Down, Tackle, Rapid Spin | GENTLE | 5/8/19/0/8/7 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 5 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 60 / 252/252 | Earthquake, Take Down, Rock Blast, Fury Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Lapras | 60 / 282/282 | Hydro Pump, Ice Beam, Body Slam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Tentacruel | 60 / 185/185 | Hydro Pump, Bubble Beam, Acid, Wrap | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_SSANNE_EXTERIOR; water_mons, slot 0, peso 60%, nível 5–10; evoluções: TENTACOOL→TENTACRUEL no 30 |
| Seadra | 60 / 155/155 | Hydro Pump, Water Gun, Twister | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_CELADON_CITY; fishing_mons, slot 0, peso 70%, nível 35–41 |
| Azumarill | 60 / 246/246 | Hydro Pump, Bubble Beam, Water Gun, Double Edge | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE22; land_mons, slot 4, peso 10%, nível 5–12; evoluções: MARILL→AZUMARILL no 18 |
| Machamp | 60 / 234/234 | Cross Chop, Vital Throw, Submission, Revenge | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE16; land_mons, slot 0, peso 20%, nível 44–50; evoluções: MACHOKE→MACHAMP no 45 |

## 15. Sabrina — Saffron Gym

### Normal — vitória registrada

Inicial: Charmander. Dataset: `prepared-normal-charmander`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 4 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 60 / 244/244 | Crunch, Rock Slide | RASH | 27/18/13/20/17/16 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_B1F; land_mons, slot 10, peso 1%, nível 35–41; evoluções: PUPITAR→TYRANITAR no 55 |
| Houndoom | 60 / 172/172 | Crunch, Bite, Faint Attack, Flamethrower | MILD | 19/24/25/18/14/4 | 4/0/0/252/252/0 | MAP_MT_MOON_B2F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: HOUNDOUR→HOUNDOOM no 24 |
| Metagross | 60 / 220/220 | Meteor Mash, Take Down | HARDY | 27/6/8/23/3/30 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46 |
| Sableye | 60 / 176/176 | Shadow Ball, Faint Attack, Astonish, Knock Off | BRAVE | 14/26/22/22/26/8 | 252/252/4/0/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 5, peso 10%, nível 27–33 |
| Crawdaunt | 60 / 190/190 | Crunch, Crabhammer, Bubble Beam, Knock Off | SASSY | 12/4/0/14/3/13 | 252/0/4/0/252/0 | MAP_ROUTE6; fishing_mons, slot 1, peso 30%, nível 27–33; evoluções: CORPHISH→CRAWDAUNT no 30 |
| Banette | 60 / 190/190 | Shadow Ball, Faint Attack | GENTLE | 9/27/29/25/27/6 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_6F; land_mons, slot 9, peso 4%, nível 44–50 |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `final-hardcore-charmander`, tentativa 4. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 6 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 60 / 246/246 | Crunch, Rock Slide | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 0, peso 20%, nível 44–50; evoluções: PUPITAR→TYRANITAR no 55 |
| Mightyena | 60 / 210/210 | Crunch, Bite, Take Down, Thief | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE1; land_mons, slot 5, peso 10%, nível 3–6; evoluções: POOCHYENA→MIGHTYENA no 18 |
| Dusclops | 60 / 174/174 | Shadow Punch, Pursuit | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 2, peso 10%, nível 44–50 |
| Slaking | 60 / 269/269 | Slash | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE14; land_mons, slot 0, peso 20%, nível 44–50; evoluções: VIGOROTH→SLAKING no 45 |
| Whiscash | 60 / 258/258 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Volbeat | 60 / 167/167 | Signal Beam, Double Edge | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE9; land_mons, slot 3, peso 10%, nível 35–41 |

## 16. Blaine — Cinnabar Gym

### Normal — vitória registrada

Inicial: Charmander. Dataset: `prepared-normal-charmander`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 6 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Wailord | 68 / 362/362 | Water Spout, Hydro Pump, Water Pulse | QUIET | 16/17/17/11/25/5 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_WEST; fishing_mons, slot 0, peso 70%, nível 44–50; evoluções: WAILMER→WAILORD no 45 |
| Tyranitar | 68 / 262/262 | Earthquake, Rock Slide, Crunch | HARDY | 9/9/1/0/31/25 | 252/252/4/0/0/0 | MAP_ROUTE13; land_mons, slot 8, peso 4%, nível 44–50; evoluções: LARVITAR→PUPITAR no 45, PUPITAR→TYRANITAR no 55 |
| Swampert | 68 / 257/257 | Earthquake, Muddy Water, Water Gun, Take Down | SASSY | 1/26/9/15/29/23 | 252/252/4/0/0/0 | MAP_ROUTE25; land_mons, slot 7, peso 5%, nível 17–25; evoluções: MUDKIP→MARSHTOMP no 18, MARSHTOMP→SWAMPERT no 36 |
| Omastar | 68 / 224/224 | Hydro Pump, Water Gun, Ancient Power, Mud Shot | BASHFUL | 12/11/18/8/14/20 | 252/0/4/0/252/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 10, peso 1%, nível 27–33; evoluções: OMANYTE→OMASTAR no 40 |
| Camerupt | 68 / 224/224 | Earthquake, Rock Slide, Take Down, Tackle | ADAMANT | 12/27/2/28/25/27 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |
| Vaporeon | 68 / 309/309 | Hydro Pump, Aurora Beam | RASH | 18/4/31/16/11/20 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B1F; land_mons, slot 5, peso 10%, nível 54–59 |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `final-hardcore-charmander`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 8 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Omastar | 68 / 237/237 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Tyranitar | 68 / 277/277 | Earthquake, Rock Slide, Crunch | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE15; land_mons, slot 0, peso 20%, nível 44–50; evoluções: PUPITAR→TYRANITAR no 55 |
| Milotic | 68 / 271/271 | Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE19; water_mons, slot 3, peso 4%, nível 54–59 |
| Whiscash | 68 / 291/291 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Lapras | 68 / 318/318 | Hydro Pump, Body Slam, Ice Beam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Gyarados | 68 / 271/271 | Hydro Pump, Dragon Rage, Tackle, Bite | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | purchase, Route4 Pokemon Center, 500; evoluções: MAGIKARP→GYARADOS no 20 |

## 17. Biker Goon — Three Island, primeira luta

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 310/310 | Earthquake, Take Down, Horn Attack, Stomp | IMPISH | 23/16/12/7/11/21 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Golem | 76 / 256/256 | Earthquake, Double Edge, Rock Throw, Rock Blast | LONELY | 1/5/11/6/20/10 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Slowbro | 76 / 291/291 | Psychic, Headbutt | CALM | 18/22/24/0/6/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 7, peso 15%, nível 44–50 |
| Lunatone | 76 / 261/261 | Psychic, Rock Throw, Psywave | RASH | 28/13/31/29/24/4 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 6, peso 5%, nível 35–41 |
| Metagross | 76 / 264/264 | Psychic, Meteor Mash, Confusion, Metal Claw | SASSY | 12/11/17/1/12/10 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Camerupt | 76 / 257/257 | Eruption, Earthquake, Take Down, Ember | HARDY | 22/5/30/0/1/24 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 317/317 | Earthquake, Take Down, Megahorn, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Omastar | 76 / 263/263 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Grumpig | 76 / 279/279 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Torkoal | 76 / 263/263 | Flamethrower, Heat Wave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 6, peso 5%, nível 44–50 |
| Venomoth | 76 / 216/216 | Psychic, Psybeam, Gust, Sleep Powder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_THREE_ISLAND_BERRY_FOREST; land_mons, slot 4, peso 10%, nível 70–75 |
| Whiscash | 76 / 324/324 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |

## 18. Biker Goon — Three Island, segunda luta

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `prepared-normal-squirtle`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Golem | 76 / 269/269 | Earthquake, Double Edge, Rock Throw, Rock Blast | BRAVE | 18/17/17/28/24/25 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Rhydon | 76 / 304/304 | Earthquake, Take Down, Horn Attack, Stomp | CAREFUL | 15/14/25/28/0/28 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Lunatone | 76 / 253/253 | Psychic, Rock Throw, Psywave | RELAXED | 18/14/11/15/22/2 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 6, peso 5%, nível 35–41 |
| Graveler | 76 / 228/228 | Earthquake, Double Edge, Rock Throw, Rock Blast | LAX | 15/3/16/16/7/13 | 252/252/4/0/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 3, peso 10%, nível 27–33 |
| Metagross | 76 / 257/257 | Psychic, Confusion, Meteor Mash, Metal Claw | CAREFUL | 2/19/20/5/31/18 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Donphan | 76 / 286/286 | Earthquake, Take Down, Tackle, Rapid Spin | ADAMANT | 21/18/31/0/2/30 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 317/317 | Earthquake, Take Down, Megahorn, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Omastar | 76 / 263/263 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Grumpig | 76 / 279/279 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Torkoal | 76 / 263/263 | Flamethrower, Heat Wave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 6, peso 5%, nível 44–50 |
| Whiscash | 76 / 324/324 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Venomoth | 76 / 216/216 | Psychic, Psybeam, Gust, Sleep Powder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_THREE_ISLAND_BERRY_FOREST; land_mons, slot 4, peso 10%, nível 70–75 |

## 19. Biker Goon — Three Island, terceira luta

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 310/310 | Earthquake, Take Down, Horn Attack, Stomp | IMPISH | 23/16/12/7/11/21 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Golem | 76 / 256/256 | Earthquake, Double Edge, Rock Throw, Rock Blast | LONELY | 1/5/11/6/20/10 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Slowbro | 76 / 291/291 | Psychic, Headbutt | CALM | 18/22/24/0/6/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 7, peso 15%, nível 44–50 |
| Lunatone | 76 / 261/261 | Psychic, Rock Throw, Psywave | RASH | 28/13/31/29/24/4 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 6, peso 5%, nível 35–41 |
| Metagross | 76 / 264/264 | Psychic, Meteor Mash, Confusion, Metal Claw | SASSY | 12/11/17/1/12/10 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Camerupt | 76 / 257/257 | Eruption, Earthquake, Take Down, Ember | HARDY | 22/5/30/0/1/24 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 317/317 | Earthquake, Take Down, Megahorn, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Omastar | 76 / 263/263 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Grumpig | 76 / 279/279 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Torkoal | 76 / 263/263 | Flamethrower, Heat Wave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 6, peso 5%, nível 44–50 |
| Venomoth | 76 / 216/216 | Psychic, Psybeam, Gust, Sleep Powder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_THREE_ISLAND_BERRY_FOREST; land_mons, slot 4, peso 10%, nível 70–75 |
| Whiscash | 76 / 324/324 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |

## 20. Paxton — líder dos Bikers de Three Island

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 3. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 310/310 | Earthquake, Take Down, Horn Attack, Stomp | IMPISH | 23/16/12/7/11/21 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Golem | 76 / 256/256 | Earthquake, Double Edge, Rock Throw, Rock Blast | LONELY | 1/5/11/6/20/10 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Slowbro | 76 / 291/291 | Psychic, Headbutt | CALM | 18/22/24/0/6/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 7, peso 15%, nível 44–50 |
| Lunatone | 76 / 261/261 | Psychic, Rock Throw, Psywave | RASH | 28/13/31/29/24/4 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 6, peso 5%, nível 35–41 |
| Metagross | 76 / 264/264 | Psychic, Meteor Mash, Confusion, Metal Claw | SASSY | 12/11/17/1/12/10 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Camerupt | 76 / 257/257 | Eruption, Earthquake, Take Down, Ember | HARDY | 22/5/30/0/1/24 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 76 / 317/317 | Earthquake, Take Down, Megahorn, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Omastar | 76 / 263/263 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Grumpig | 76 / 279/279 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Torkoal | 76 / 263/263 | Flamethrower, Heat Wave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE15; land_mons, slot 6, peso 5%, nível 44–50 |
| Venomoth | 76 / 216/216 | Psychic, Psybeam, Gust, Sleep Powder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_THREE_ISLAND_BERRY_FOREST; land_mons, slot 4, peso 10%, nível 70–75 |
| Whiscash | 76 / 324/324 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |

## 21. Giovanni — Viridian Gym

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `prepared-normal-squirtle`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 4 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Wailord | 76 / 403/403 | Water Spout, Hydro Pump | LAX | 15/31/7/15/8/20 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B4F; water_mons, slot 3, peso 4%, nível 54–59 |
| Omastar | 76 / 250/250 | Hydro Pump, Water Gun, Bite, Mud Shot | HARDY | 13/16/25/31/14/18 | 252/0/4/0/252/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 10, peso 1%, nível 27–33; evoluções: OMANYTE→OMASTAR no 40 |
| Donphan | 76 / 286/286 | Earthquake, Take Down, Tackle, Rapid Spin | ADAMANT | 21/18/31/0/2/30 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |
| Vaporeon | 76 / 332/332 | Hydro Pump, Aurora Beam | RASH | 1/20/7/31/15/12 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B1F; land_mons, slot 5, peso 10%, nível 54–59 |
| Milotic | 76 / 279/279 | Hydro Pump | RELAXED | 1/11/0/26/5/17 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B3F; water_mons, slot 3, peso 4%, nível 54–59 |
| Articuno | 76 / 230/230 | Ice Beam, Blizzard | DOCILE | 9/28/27/22/11/22 | 4/0/0/252/252/0 | Seafoam fixed encounter |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `final-hardcore-squirtle`, tentativa 4. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Blastoise | 76 / 277/277 | Hydro Pump, Water Gun, Bubble, Bite | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | starter; evoluções: SQUIRTLE→WARTORTLE no 16, WARTORTLE→BLASTOISE no 36 |
| Feraligatr | 76 / 286/286 | Hydro Pump, Slash | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_CINNABAR_ISLAND; water_mons, slot 3, peso 4%, nível 62–67 |
| Golem | 76 / 279/279 | Earthquake, Double Edge, Rock Blast, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 4, peso 10%, nível 44–50 |
| Lapras | 76 / 355/355 | Hydro Pump, Ice Beam, Body Slam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Pelipper | 76 / 248/248 | Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_FUCHSIA_CITY; fishing_mons, slot 1, peso 30%, nível 44–50 |
| Walrein | 76 / 324/324 | Blizzard, Aurora Beam, Body Slam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE7; land_mons, slot 2, peso 10%, nível 35–41; evoluções: SEALEO→WALREIN no 44 |

## 22. Rival — Route 22, antes da Liga

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `prepared-normal-bulbasaur`, tentativa 2. Contexto: isolated. Preparação: `manual_ev_farm_508`. Resultado: 1; 6 turnos contados pelo motor, 2 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 80 / 313/313 | Rock Slide, Crunch, Earthquake | CAREFUL | 16/24/10/23/14/5 | 252/252/4/0/0/0 | MAP_ROUTE13; land_mons, slot 8, peso 4%, nível 44–50; evoluções: LARVITAR→PUPITAR no 45, PUPITAR→TYRANITAR no 55 |
| Wailord | 80 / 417/417 | Water Spout, Hydro Pump, Water Pulse | DOCILE | 6/4/23/23/28/14 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_WEST; fishing_mons, slot 0, peso 70%, nível 44–50; evoluções: WAILMER→WAILORD no 45 |
| Ampharos | 80 / 300/300 | Thunder, Thunder Punch, Thunder Shock, Tackle | BRAVE | 20/24/27/22/21/12 | 252/0/4/0/252/0 | MAP_ROUTE4; land_mons, slot 10, peso 1%, nível 17–25; evoluções: MAREEP→FLAAFFY no 18, FLAAFFY→AMPHAROS no 30 |
| Metagross | 80 / 282/282 | Meteor Mash, Take Down | ADAMANT | 17/8/3/16/30/2 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46 |
| Omastar | 80 / 268/268 | Hydro Pump, Bite, Ancient Power, Water Gun | SASSY | 20/23/16/27/31/5 | 252/0/4/0/252/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 10, peso 1%, nível 27–33; evoluções: OMANYTE→OMASTAR no 40 |
| Lapras | 80 / 372/372 | Hydro Pump, Ice Beam | GENTLE | 30/5/10/31/6/26 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 4 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Omastar | 80 / 277/277 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Lapras | 80 / 373/373 | Hydro Pump, Ice Beam, Body Slam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Rhydon | 80 / 333/333 | Megahorn, Earthquake, Rock Blast, Take Down | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Raichu | 80 / 211/211 | Thunderbolt, Thunder Shock, Quick Attack | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE18; land_mons, slot 7, peso 5%, nível 44–50 |
| Solrock | 80 / 277/277 | Rock Slide, Rock Throw, Fire Spin, Psywave | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 1, peso 20%, nível 35–41 |
| Walrein | 80 / 341/341 | Blizzard | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE19; water_mons, slot 1, peso 30%, nível 54–59; evoluções: SEALEO→WALREIN no 55 |

## 23. Lorelei — primeira Liga

### Normal — vitória registrada

Inicial: Charmander. Dataset: `pickup-normal-charmander`, tentativa 2. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 8 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Jolteon | 80 / 194/194 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | RASH | 0/1/5/14/7/17 | 4/0/0/252/252/0 | MAP_ROUTE25; land_mons, slot 1, peso 20%, nível 17–25; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Ludicolo | 80 / 273/273 | Surf, Giga Drain, Ice Beam, Brick Break | MILD | 6/10/19/8/0/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Metagross | 80 / 290/290 | Meteor Mash, Shadow Ball, Brick Break, Take Down | DOCILE | 27/8/25/5/3/0 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46; TMs SHADOW_BALL, BRICK_BREAK |
| Lapras | 80 / 358/358 | Hydro Pump, Ice Beam, Thunderbolt, Surf | NAUGHTY | 13/10/9/26/14/28 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT |
| Zapdos | 80 / 244/244 | Thunderbolt, Drill Peck | MODEST | 12/27/1/15/13/11 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Snorlax | 80 / 400/400 | Body Slam, Shadow Ball, Brick Break, Surf | NAIVE | 5/17/19/2/11/22 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `pickup-hardcore-squirtle`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 8 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Jolteon | 80 / 219/219 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | Celadon gift; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Ludicolo | 80 / 293/293 | Surf, Giga Drain, Ice Beam, Brick Break | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_THREE_ISLAND_BOND_BRIDGE; land_mons, slot 0, peso 20%, nível 70–75; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Slaking | 80 / 355/355 | Shadow Ball, Brick Break, Slash, Ice Beam | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE14; land_mons, slot 0, peso 20%, nível 44–50; evoluções: VIGOROTH→SLAKING no 45; TMs SHADOW_BALL, BRICK_BREAK, ICE_BEAM |
| Lapras | 80 / 373/373 | Hydro Pump, Ice Beam, Thunderbolt, Surf | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival; equipado LEFTOVERS; TMs THUNDERBOLT |
| Zapdos | 80 / 259/259 | Thunderbolt, Drill Peck | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | fixed Power Plant Zapdos; TMs THUNDERBOLT |
| Snorlax | 80 / 421/421 | Body Slam, Shadow Ball, Brick Break, Surf | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |

## 24. Bruno — primeira Liga

### Normal — vitória registrada

Inicial: Charmander. Dataset: `pickup-normal-charmander`, tentativa 2. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 9 turnos contados pelo motor, 4 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Zapdos | 83 / 253/253 | Thunderbolt, Drill Peck | MODEST | 12/27/1/15/13/11 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Jolteon | 83 / 201/201 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | RASH | 0/1/5/14/7/17 | 4/0/0/252/252/0 | MAP_ROUTE25; land_mons, slot 1, peso 20%, nível 17–25; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Snorlax | 83 / 415/415 | Body Slam, Shadow Ball, Brick Break, Surf | NAIVE | 5/17/19/2/11/22 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |
| Metagross | 83 / 300/300 | Meteor Mash, Shadow Ball, Brick Break, Take Down | DOCILE | 27/8/25/5/3/0 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46; TMs SHADOW_BALL, BRICK_BREAK |
| Ludicolo | 83 / 283/283 | Surf, Giga Drain, Ice Beam, Brick Break | MILD | 6/10/19/8/0/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Lapras | 83 / 371/371 | Hydro Pump, Ice Beam, Thunderbolt, Surf | NAUGHTY | 13/10/9/26/14/28 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `pickup-hardcore-squirtle`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 11 turnos contados pelo motor, 4 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Zapdos | 83 / 268/268 | Thunderbolt, Drill Peck | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | fixed Power Plant Zapdos; TMs THUNDERBOLT |
| Jolteon | 83 / 227/227 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | Celadon gift; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Snorlax | 83 / 436/436 | Body Slam, Shadow Ball, Brick Break, Surf | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |
| Slaking | 83 / 368/368 | Shadow Ball, Brick Break, Slash, Ice Beam | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE14; land_mons, slot 0, peso 20%, nível 44–50; evoluções: VIGOROTH→SLAKING no 45; TMs SHADOW_BALL, BRICK_BREAK, ICE_BEAM |
| Lapras | 83 / 386/386 | Hydro Pump, Ice Beam, Thunderbolt, Surf | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival; equipado LEFTOVERS; TMs THUNDERBOLT |
| Ludicolo | 83 / 303/303 | Surf, Giga Drain, Ice Beam, Brick Break | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_THREE_ISLAND_BOND_BRIDGE; land_mons, slot 0, peso 20%, nível 70–75; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |

## 25. Agatha — primeira Liga

### Normal — vitória registrada

Inicial: Charmander. Dataset: `pickup-normal-charmander`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 6 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Metagross | 86 / 311/311 | Meteor Mash, Shadow Ball, Brick Break, Take Down | DOCILE | 27/8/25/5/3/0 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46; TMs SHADOW_BALL, BRICK_BREAK |
| Snorlax | 86 / 429/429 | Body Slam, Shadow Ball, Brick Break, Surf | NAIVE | 5/17/19/2/11/22 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |
| Zapdos | 86 / 261/261 | Thunderbolt, Drill Peck | MODEST | 12/27/1/15/13/11 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Jolteon | 86 / 208/208 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | RASH | 0/1/5/14/7/17 | 4/0/0/252/252/0 | MAP_ROUTE25; land_mons, slot 1, peso 20%, nível 17–25; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Ludicolo | 86 / 292/292 | Surf, Giga Drain, Ice Beam, Brick Break | MILD | 6/10/19/8/0/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Lapras | 86 / 384/384 | Hydro Pump, Ice Beam, Thunderbolt, Surf | NAUGHTY | 13/10/9/26/14/28 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `final-hardcore-charmander`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 9 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 86 / 348/348 | Earthquake, Crunch, Rock Slide | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE15; land_mons, slot 0, peso 20%, nível 44–50; evoluções: PUPITAR→TYRANITAR no 55 |
| Machamp | 86 / 331/331 | Cross Chop, Vital Throw, Submission, Revenge | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE16; land_mons, slot 0, peso 20%, nível 44–50; evoluções: MACHOKE→MACHAMP no 45 |
| Grumpig | 86 / 314/314 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Nidoqueen | 86 / 331/331 | Superpower, Poison Sting, Double Kick, Body Slam | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE23; land_mons, slot 1, peso 20%, nível 70–75 |
| Whiscash | 86 / 366/366 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Exploud | 86 / 355/355 | Hyper Voice, Astonish, Stomp | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE6; land_mons, slot 1, peso 20%, nível 27–33; evoluções: WHISMUR→LOUDRED no 28, LOUDRED→EXPLOUD no 40 |

## 26. Lance — primeira Liga

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `pickup-normal-squirtle`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 16 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Zapdos | 89 / 268/268 | Thunderbolt, Drill Peck | DOCILE | 9/28/27/22/11/22 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Jolteon | 89 / 240/240 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | DOCILE | 28/17/5/30/25/10 | 4/0/0/252/252/0 | MAP_ROUTE25; land_mons, slot 1, peso 20%, nível 17–25; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Metagross | 89 / 299/299 | Meteor Mash, Psychic, Shadow Ball, Brick Break | CAREFUL | 2/19/20/5/31/18 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45; TMs SHADOW_BALL, BRICK_BREAK |
| Snorlax | 89 / 466/466 | Body Slam, Shadow Ball, Brick Break, Surf | CALM | 30/29/19/4/25/18 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |
| Ludicolo | 89 / 312/312 | Surf, Giga Drain, Ice Beam, Brick Break | MILD | 17/25/5/7/4/7 | 252/0/4/0/252/0 | MAP_ROUTE2; land_mons, slot 4, peso 10%, nível 5–12; evoluções: LOTAD→LOMBRE no 14; WATER_STONE; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Lapras | 89 / 409/409 | Hydro Pump, Ice Beam, Thunderbolt, Surf | JOLLY | 26/30/13/18/26/10 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `final-hardcore-charmander`, tentativa 5. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 7 turnos contados pelo motor, 4 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Whiscash | 89 / 378/378 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Tyranitar | 89 / 360/360 | Rock Slide, Earthquake, Crunch | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE15; land_mons, slot 0, peso 20%, nível 44–50; evoluções: PUPITAR→TYRANITAR no 55 |
| Grumpig | 89 / 325/325 | Psychic, Psybeam, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 0, peso 20%, nível 17–25; evoluções: SPOINK→GRUMPIG no 32 |
| Lapras | 89 / 414/414 | Ice Beam, Body Slam, Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Dragonite | 89 / 344/344 | Wing Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_THREE_ISLAND_PORT; land_mons, slot 10, peso 1%, nível 70–75 |
| Nidoqueen | 89 / 342/342 | Superpower, Body Slam, Poison Sting, Double Kick | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE23; land_mons, slot 1, peso 20%, nível 70–75 |

## 27. Campeão — primeira Liga

### Normal — vitória registrada

Inicial: Charmander. Dataset: `pickup-normal-charmander`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 4 turnos contados pelo motor, 2 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Ludicolo | 92 / 312/312 | Surf, Giga Drain, Ice Beam, Brick Break | MILD | 6/10/19/8/0/27 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Lapras | 92 / 411/411 | Hydro Pump, Ice Beam, Thunderbolt, Surf | NAUGHTY | 13/10/9/26/14/28 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT |
| Snorlax | 92 / 458/458 | Body Slam, Shadow Ball, Brick Break, Surf | NAIVE | 5/17/19/2/11/22 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK |
| Metagross | 92 / 332/332 | Meteor Mash, Shadow Ball, Brick Break, Take Down | DOCILE | 27/8/25/5/3/0 | 252/252/4/0/0/0 | MAP_POKEMON_TOWER_7F; land_mons, slot 9, peso 4%, nível 44–50; evoluções: BELDUM→METANG no 45, METANG→METAGROSS no 46; TMs SHADOW_BALL, BRICK_BREAK |
| Jolteon | 92 / 222/222 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | RASH | 0/1/5/14/7/17 | 4/0/0/252/252/0 | MAP_ROUTE25; land_mons, slot 1, peso 20%, nível 17–25; THUNDER_STONE; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Zapdos | 92 / 279/279 | Thunderbolt, Drill Peck | MODEST | 12/27/1/15/13/11 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `final-hardcore-squirtle`, tentativa 3. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 4 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Lapras | 92 / 427/427 | Hydro Pump, Ice Beam, Body Slam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Aggron | 92 / 317/317 | Double Edge, Iron Tail | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_POWER_PLANT; land_mons, slot 0, peso 20%, nível 54–59; evoluções: LAIRON→AGGRON no 55 |
| Blastoise | 92 / 333/333 | Hydro Pump, Water Gun, Bite, Bubble | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | starter; evoluções: SQUIRTLE→WARTORTLE no 16, WARTORTLE→BLASTOISE no 36 |
| Manectric | 92 / 260/260 | Thunder, Bite, Spark | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE16; land_mons, slot 3, peso 10%, nível 44–50 |
| Feraligatr | 92 / 344/344 | Hydro Pump, Slash | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_CINNABAR_ISLAND; water_mons, slot 3, peso 4%, nível 62–67 |
| Forretress | 92 / 326/326 | Double Edge, Zap Cannon, Rapid Spin | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE13; land_mons, slot 7, peso 5%, nível 44–50 |

## 28. Administradora Rocket — Rocket Warehouse

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `final-normal-squirtle`, tentativa 4. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 3 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 100 / 414/414 | Earthquake, Take Down, Megahorn, Horn Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Golem | 100 / 364/364 | Earthquake, Double Edge, Rock Throw, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Altaria | 100 / 354/354 | Sky Attack, Dragon Breath, Take Down, Peck | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE4; land_mons, slot 5, peso 10%, nível 17–25; evoluções: SWABLU→ALTARIA no 35 |
| Graveler | 100 / 314/314 | Earthquake, Double Edge, Rock Throw, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 3, peso 10%, nível 27–33 |
| Metagross | 100 / 364/364 | Psychic, Meteor Mash, Confusion, Metal Claw | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45 |
| Donphan | 100 / 384/384 | Earthquake, Take Down, Tackle, Rapid Spin | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |

### Hardcore — vitória registrada

Inicial: Charmander. Dataset: `final-hardcore-charmander`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 2 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 100 / 404/404 | Earthquake, Rock Slide, Crunch | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE15; land_mons, slot 0, peso 20%, nível 44–50; evoluções: PUPITAR→TYRANITAR no 55 |
| Metagross | 100 / 364/364 | Meteor Mash | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_FIVE_ISLAND_LOST_CAVE_ROOM1; land_mons, slot 10, peso 1%, nível 84–94 |
| Altaria | 100 / 354/354 | Sky Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SEVEN_ISLAND_SEVAULT_CANYON; land_mons, slot 0, peso 20%, nível 84–94 |
| Quagsire | 100 / 394/394 | Earthquake | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SEVEN_ISLAND_TRAINER_TOWER; water_mons, slot 0, peso 60%, nível 84–94 |
| Whiscash | 100 / 424/424 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Slowbro | 100 / 394/394 | Psychic | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SIX_ISLAND_RUIN_VALLEY; land_mons, slot 0, peso 20%, nível 84–94 |

## 29. Administrador Rocket — Rocket Warehouse

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `final-normal-squirtle`, tentativa 4. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 0 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Tyranitar | 100 / 404/404 | Rock Slide, Crunch, Earthquake | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_B1F; land_mons, slot 10, peso 1%, nível 35–41; evoluções: PUPITAR→TYRANITAR no 55 |
| Kyogre | 100 / 342/342 | Water Spout, Double Edge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95 |
| Magneton | 100 / 304/304 | Spark, Zap Cannon, Thunder Shock, Tri Attack | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_MT_MOON_1F; land_mons, slot 1, peso 20%, nível 17–25; evoluções: MAGNEMITE→MAGNETON no 30 |
| Lunatone | 100 / 344/344 | Psychic, Rock Throw, Psywave | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROCK_TUNNEL_1F; land_mons, slot 6, peso 5%, nível 35–41 |
| Camerupt | 100 / 344/344 | Eruption, Earthquake, Rock Slide, Take Down | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: NUMEL→CAMERUPT no 33 |
| Slowbro | 100 / 394/394 | Psychic, Water Gun, Confusion, Headbutt | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_ROUTE25; land_mons, slot 2, peso 10%, nível 17–25; evoluções: SLOWPOKE→SLOWBRO no 37 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 4. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 0 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Kyogre | 100 / 342/342 | Water Spout, Double Edge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95 |
| Aggron | 100 / 344/344 | Double Edge, Iron Tail | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SIX_ISLAND_RUIN_VALLEY; land_mons, slot 4, peso 10%, nível 84–94 |
| Omastar | 100 / 344/344 | Hydro Pump, Ancient Power, Spike Cannon | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_1F; land_mons, slot 6, peso 5%, nível 54–59 |
| Latias | 100 / 302/302 | Psychic, Mist Ball, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_RESORT_GORGEOUS; water_mons, slot 4, peso 1%, nível 85–90 |
| Wailord | 100 / 544/544 | Water Spout, Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_FIVE_ISLAND; water_mons, slot 1, peso 30%, nível 84–94; evoluções: WAILMER→WAILORD no 85 |
| Lapras | 100 / 464/464 | Hydro Pump, Ice Beam, Body Slam | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |

## 30. Gideon — Rocket Warehouse

### Normal — vitória registrada

Inicial: Charmander. Dataset: `final-normal-charmander`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 5 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 100 / 414/414 | Earthquake, Megahorn, Take Down, Horn Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_MT_MOON_B1F; land_mons, slot 0, peso 20%, nível 17–25; evoluções: RHYHORN→RHYDON no 42 |
| Golem | 100 / 364/364 | Earthquake, Double Edge, Rock Throw, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_MT_MOON_B2F; land_mons, slot 4, peso 10%, nível 17–25; evoluções: GEODUDE→GRAVELER no 25, GRAVELER→GOLEM no 37 |
| Graveler | 100 / 314/314 | Earthquake, Double Edge, Rock Throw, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 3, peso 10%, nível 27–33 |
| Donphan | 100 / 384/384 | Earthquake, Take Down, Tackle, Rapid Spin | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25 |
| Swampert | 100 / 404/404 | Earthquake, Muddy Water, Mud Slap, Take Down | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE25; land_mons, slot 7, peso 5%, nível 17–25; evoluções: MUDKIP→MARSHTOMP no 18, MARSHTOMP→SWAMPERT no 36 |
| Pupitar | 100 / 344/344 | Earthquake, Rock Slide, Crunch | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROCK_TUNNEL_B1F; land_mons, slot 10, peso 1%, nível 35–41 |

### Hardcore — vitória registrada

Inicial: Bulbasaur. Dataset: `final-hardcore-bulbasaur`, tentativa 1. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 1 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Rhydon | 100 / 414/414 | Earthquake, Megahorn, Take Down, Rock Blast | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SAFARI_ZONE_CENTER; land_mons, slot 7, peso 5%, nível 44–50 |
| Quagsire | 100 / 394/394 | Earthquake | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SEVEN_ISLAND_TRAINER_TOWER; water_mons, slot 0, peso 60%, nível 84–94 |
| Whiscash | 100 / 424/424 | Earthquake, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_VERMILION_CITY; fishing_mons, slot 0, peso 70%, nível 27–33; evoluções: BARBOACH→WHISCASH no 30 |
| Machamp | 100 / 384/384 | Cross Chop, Vital Throw, Submission, Revenge | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE16; land_mons, slot 0, peso 20%, nível 44–50; evoluções: MACHOKE→MACHAMP no 45 |
| Dugtrio | 100 / 212/212 | Earthquake, Mud Slap, Slash, Fury Swipes | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_DIGLETTS_CAVE_B1F; land_mons, slot 1, peso 20%, nível 27–33; evoluções: DIGLETT→DUGTRIO no 28 |
| Kyogre | 100 / 342/342 | Water Spout, Double Edge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95 |

## 31. Lorelei — revanche

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `pickup-normal-bulbasaur`, tentativa 2. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 3 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Kyogre | 100 / 342/342 | Water Spout, Thunderbolt, Ice Beam, Surf | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95; TMs THUNDERBOLT, ICE_BEAM |
| Mewtwo | 100 / 354/354 | Psychic, Thunderbolt, Ice Beam, Brick Break | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_CERULEAN_CAVE_B1F; land_mons, slot 11, peso 1%, nível 90–95; TMs THUNDERBOLT, ICE_BEAM, BRICK_BREAK |
| Donphan | 100 / 384/384 | Earthquake, Iron Tail, Take Down, Tackle | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25; equipado LEFTOVERS; TMs IRON_TAIL |
| Slaking | 100 / 442/442 | Shadow Ball, Brick Break, Slash, Thunderbolt | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE25; land_mons, slot 10, peso 1%, nível 17–25; evoluções: SLAKOTH→VIGOROTH no 18, VIGOROTH→SLAKING no 36; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, THUNDERBOLT |
| Latios | 100 / 302/302 | Psychic, Thunderbolt, Ice Beam, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_MEMORIAL_PILLAR; land_mons, slot 11, peso 1%, nível 85–90; TMs THUNDERBOLT, ICE_BEAM |
| Metagross | 100 / 364/364 | Meteor Mash, Psychic, Shadow Ball, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45; TMs SHADOW_BALL, BRICK_BREAK |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `pickup-hardcore-squirtle`, tentativa 2. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 5 turnos contados pelo motor, 0 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Kyogre | 100 / 342/342 | Water Spout, Thunderbolt, Ice Beam, Surf | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95; TMs THUNDERBOLT, ICE_BEAM |
| Latias | 100 / 302/302 | Psychic, Thunderbolt, Surf, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_RESORT_GORGEOUS; water_mons, slot 4, peso 1%, nível 85–90; TMs THUNDERBOLT |
| Slaking | 100 / 442/442 | Shadow Ball, Brick Break, Slash, Thunderbolt | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE14; land_mons, slot 0, peso 20%, nível 44–50; evoluções: VIGOROTH→SLAKING no 45; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, THUNDERBOLT |
| Golem | 100 / 364/364 | Earthquake, Double Edge, Flamethrower, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 4, peso 10%, nível 44–50; TMs FLAMETHROWER, BRICK_BREAK |
| Altaria | 100 / 354/354 | Sky Attack, Ice Beam, Flamethrower, Iron Tail | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SEVEN_ISLAND_SEVAULT_CANYON; land_mons, slot 0, peso 20%, nível 84–94; TMs ICE_BEAM, FLAMETHROWER, IRON_TAIL |
| Shiftry | 100 / 384/384 | Shadow Ball, Giga Drain, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE2; land_mons, slot 5, peso 10%, nível 5–12; evoluções: SEEDOT→NUZLEAF no 14; LEAF_STONE; equipado LEFTOVERS; TMs SHADOW_BALL, GIGA_DRAIN, BRICK_BREAK |

## 32. Bruno — revanche

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `pickup-normal-bulbasaur`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 10 turnos contados pelo motor, 3 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Zapdos | 100 / 322/322 | Thunderbolt, Drill Peck, Thunder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Jolteon | 100 / 272/272 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_POWER_PLANT; land_mons, slot 8, peso 4%, nível 54–59; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Snorlax | 100 / 524/524 | Shadow Ball, Body Slam, Brick Break, Thunderbolt | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, THUNDERBOLT |
| Metagross | 100 / 364/364 | Meteor Mash, Psychic, Shadow Ball, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45; TMs SHADOW_BALL, BRICK_BREAK |
| Lapras | 100 / 464/464 | Ice Beam, Hydro Pump, Thunderbolt, Iron Tail | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT, IRON_TAIL |
| Ludicolo | 100 / 364/364 | Surf, Giga Drain, Ice Beam, Brick Break | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `pickup-hardcore-squirtle`, tentativa 3. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 7 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Altaria | 100 / 354/354 | Sky Attack, Ice Beam, Flamethrower, Iron Tail | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SEVEN_ISLAND_SEVAULT_CANYON; land_mons, slot 0, peso 20%, nível 84–94; TMs ICE_BEAM, FLAMETHROWER, IRON_TAIL |
| Kyogre | 100 / 342/342 | Water Spout, Thunderbolt, Ice Beam, Surf | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95; TMs THUNDERBOLT, ICE_BEAM |
| Latias | 100 / 302/302 | Psychic, Thunderbolt, Surf, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_RESORT_GORGEOUS; water_mons, slot 4, peso 1%, nível 85–90; TMs THUNDERBOLT |
| Golem | 100 / 364/364 | Earthquake, Double Edge, Flamethrower, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 4, peso 10%, nível 44–50; TMs FLAMETHROWER, BRICK_BREAK |
| Slaking | 100 / 442/442 | Shadow Ball, Brick Break, Slash, Thunderbolt | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE14; land_mons, slot 0, peso 20%, nível 44–50; evoluções: VIGOROTH→SLAKING no 45; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, THUNDERBOLT |
| Shiftry | 100 / 384/384 | Shadow Ball, Giga Drain, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE2; land_mons, slot 5, peso 10%, nível 5–12; evoluções: SEEDOT→NUZLEAF no 14; LEAF_STONE; equipado LEFTOVERS; TMs SHADOW_BALL, GIGA_DRAIN, BRICK_BREAK |

## 33. Agatha — revanche

### Normal — vitória registrada

Inicial: Charmander. Dataset: `pickup-normal-charmander`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 6 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Kyogre | 100 / 342/342 | Water Spout, Thunderbolt, Ice Beam, Surf | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95; TMs THUNDERBOLT, ICE_BEAM |
| Donphan | 100 / 384/384 | Earthquake, Iron Tail, Take Down, Tackle | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE22; land_mons, slot 6, peso 5%, nível 5–12; evoluções: PHANPY→DONPHAN no 25; equipado LEFTOVERS; TMs IRON_TAIL |
| Slaking | 100 / 442/442 | Shadow Ball, Brick Break, Slash, Ice Beam | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE25; land_mons, slot 10, peso 1%, nível 17–25; evoluções: SLAKOTH→VIGOROTH no 18, VIGOROTH→SLAKING no 36; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, ICE_BEAM |
| Mewtwo | 100 / 354/354 | Psychic, Ice Beam, Thunderbolt, Brick Break | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_CERULEAN_CAVE_B1F; land_mons, slot 11, peso 1%, nível 90–95; TMs ICE_BEAM, THUNDERBOLT, BRICK_BREAK |
| Metagross | 100 / 364/364 | Meteor Mash, Psychic, Shadow Ball, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45; TMs SHADOW_BALL, BRICK_BREAK |
| Latios | 100 / 302/302 | Psychic, Surf, Thunderbolt, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_MEMORIAL_PILLAR; land_mons, slot 11, peso 1%, nível 85–90; TMs THUNDERBOLT |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `final-hardcore-squirtle`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 6 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Kyogre | 100 / 342/342 | Water Spout, Double Edge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95 |
| Donphan | 100 / 384/384 | Earthquake, Rapid Spin, Fury Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE23; land_mons, slot 3, peso 10%, nível 70–75 |
| Latias | 100 / 302/302 | Psychic, Mist Ball, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_RESORT_GORGEOUS; water_mons, slot 4, peso 1%, nível 85–90 |
| Machamp | 100 / 384/384 | Cross Chop, Submission, Dynamic Punch | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SIX_ISLAND_WATER_PATH; land_mons, slot 4, peso 10%, nível 84–94 |
| Quagsire | 100 / 394/394 | Earthquake | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_FIVE_ISLAND; water_mons, slot 0, peso 60%, nível 84–94 |
| Wailord | 100 / 544/544 | Water Spout, Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEVEN_ISLAND_TANOBY_RUINS; water_mons, slot 1, peso 30%, nível 84–94; evoluções: WAILMER→WAILORD no 85 |

## 34. Lance — revanche

### Normal — vitória registrada

Inicial: Squirtle. Dataset: `pickup-normal-squirtle`, tentativa 1. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 10 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Zapdos | 100 / 322/322 | Thunderbolt, Drill Peck, Thunder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Jolteon | 100 / 272/272 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_POWER_PLANT; land_mons, slot 8, peso 4%, nível 54–59; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Snorlax | 100 / 524/524 | Body Slam, Shadow Ball, Brick Break, Thunderbolt | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, THUNDERBOLT |
| Metagross | 100 / 364/364 | Meteor Mash, Psychic, Shadow Ball, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45; TMs SHADOW_BALL, BRICK_BREAK |
| Ludicolo | 100 / 364/364 | Surf, Giga Drain, Ice Beam, Brick Break | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Lapras | 100 / 464/464 | Hydro Pump, Thunderbolt, Ice Beam, Iron Tail | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT, IRON_TAIL |

### Hardcore — composição testada, sem vitória registrada

Inicial: Squirtle. Dataset: `final-hardcore-squirtle`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 2; 6 turnos contados pelo motor, 6 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Donphan | 100 / 384/384 | Earthquake, Rapid Spin, Fury Attack | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE23; land_mons, slot 3, peso 10%, nível 70–75 |
| Kyogre | 100 / 342/342 | Water Spout, Double Edge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95 |
| Latias | 100 / 302/302 | Psychic, Mist Ball, Recover | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_RESORT_GORGEOUS; water_mons, slot 4, peso 1%, nível 85–90 |
| Lapras | 100 / 464/464 | Ice Beam, Body Slam, Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | Silph gift after rival |
| Golem | 100 / 364/364 | Earthquake, Double Edge, Rock Blast, Magnitude | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE12; land_mons, slot 4, peso 10%, nível 44–50 |
| Quagsire | 100 / 394/394 | Earthquake | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_FIVE_ISLAND; water_mons, slot 0, peso 60%, nível 84–94 |

## 35. Campeão — revanche

### Normal — vitória registrada

Inicial: Bulbasaur. Dataset: `pickup-normal-bulbasaur`, tentativa 2. Contexto: connected block. Preparação: `shop_TMs_gifts_stones_PP_Up_Pickup`. Resultado: 1; 2 turnos contados pelo motor, 1 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Metagross | 100 / 364/364 | Meteor Mash, Psychic, Shadow Ball, Brick Break | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE7; land_mons, slot 11, peso 1%, nível 35–41; evoluções: METANG→METAGROSS no 45; TMs SHADOW_BALL, BRICK_BREAK |
| Zapdos | 100 / 322/322 | Thunderbolt, Drill Peck, Thunder | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | Power Plant fixed encounter; TMs THUNDERBOLT |
| Snorlax | 100 / 524/524 | Shadow Ball, Body Slam, Brick Break, Thunderbolt | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | fixed Route16 Snorlax after Poke Flute; equipado LEFTOVERS; TMs SHADOW_BALL, BRICK_BREAK, THUNDERBOLT |
| Jolteon | 100 / 272/272 | Thunderbolt, Shadow Ball, Thunder, Iron Tail | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_POWER_PLANT; land_mons, slot 8, peso 4%, nível 54–59; TMs THUNDERBOLT, SHADOW_BALL, IRON_TAIL |
| Ludicolo | 100 / 364/364 | Surf, Giga Drain, Ice Beam, Brick Break | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SAFARI_ZONE_CENTER; fishing_mons, slot 4, peso 20%, nível 44–50; TMs GIGA_DRAIN, ICE_BEAM, BRICK_BREAK |
| Lapras | 100 / 464/464 | Ice Beam, Hydro Pump, Thunderbolt, Iron Tail | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEAFOAM_ISLANDS_B2F; land_mons, slot 6, peso 5%, nível 54–59; equipado LEFTOVERS; TMs THUNDERBOLT, IRON_TAIL |

### Hardcore — vitória registrada

Inicial: Squirtle. Dataset: `final-hardcore-squirtle`, tentativa 2. Contexto: isolated. Preparação: `baseline`. Resultado: 1; 6 turnos contados pelo motor, 3 desmaios.

| Pokémon | Nível / HP antes | Golpes | Natureza | IVs | EVs | Origem / requisito |
|---|---|---|---|---|---|---|
| Kyogre | 100 / 342/342 | Water Spout, Double Edge | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_FIVE_ISLAND_WATER_LABYRINTH; water_mons, slot 4, peso 1%, nível 90–95 |
| Machamp | 100 / 384/384 | Cross Chop, Submission, Dynamic Punch | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_SIX_ISLAND_WATER_PATH; land_mons, slot 4, peso 10%, nível 84–94 |
| Forretress | 100 / 354/354 | Double Edge, Zap Cannon, Rapid Spin | ADAMANT | 31/31/31/31/31/31 | 252/252/4/0/0/0 | MAP_ROUTE13; land_mons, slot 7, peso 5%, nível 44–50 |
| Charizard | 100 / 298/298 | Flamethrower, Wing Attack, Slash, Dragon Rage | JOLLY | 31/31/31/31/31/31 | 4/252/0/252/0/0 | MAP_ROUTE21_NORTH; land_mons, slot 7, peso 5%, nível 54–59 |
| Manectric | 100 / 282/282 | Thunder, Spark, Bite | TIMID | 31/31/31/31/31/31 | 4/0/0/252/252/0 | MAP_ROUTE16; land_mons, slot 3, peso 10%, nível 44–50 |
| Wailord | 100 / 544/544 | Water Spout, Hydro Pump | MODEST | 31/31/31/31/31/31 | 252/0/4/0/252/0 | MAP_SEVEN_ISLAND_TANOBY_RUINS; water_mons, slot 1, peso 30%, nível 84–94; evoluções: WAILMER→WAILORD no 85 |
