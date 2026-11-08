# Desafios dos pilotos regionais

Acesso após a primeira Liga pelo SEAGALLOP de Vermilion. Apenas os pilotos,
não as campanhas completas de Johto/Hoenn. Níveis provisórios; teto continua 100.

Hoenn: escolha do inicial na bolsa → Zigzagoon nível 84 → May no quarto dela.
Johto: pedido do Elm → escolha de um inicial → Silver em New Bark.

May e Silver usam duplas, IVs 31 e EVs por função (508 no total).
Os times preservam as espécies da fonte; não representam uma revisão competitiva
completa da expansão. Os presentes de inicial são de nível 5, um por região.

| Rival | Inicial escolhido pelo jogador | Equipe (nível) |
|---|---|---|
| MAY | Mudkip | Tropius 86, Slugma 87, Pelipper 87, Grovyle 89 |
| MAY | Treecko | Tropius 86, Pelipper 87, Ludicolo 87, Combusken 89 |
| MAY | Torchic | Tropius 86, Ludicolo 87, Slugma 87, Marshtomp 89 |
| SILVER | Chikorita | Ursaring 85, Golbat 85, Octillery 86, Victreebel 86, Typhlosion 87, Tyranitar 88 |
| SILVER | Cyndaquil | Ursaring 85, Golbat 85, Victreebel 86, Houndoom 86, Feraligatr 87, Tyranitar 88 |
| SILVER | Totodile | Ursaring 85, Golbat 85, Octillery 86, Houndoom 86, Meganium 87, Tyranitar 88 |

## Golpes e itens do Silver

Importados de HNS_modern `sParty_Rival_*5`; +40 níveis. As variantes compartilham
a origem, mas cada linha abaixo é gerada dos dados da respectiva equipe.

### Contra Chikorita

| Pokémon | Item | Golpes |
|---|---|---|
| Ursaring | CHESTO_BERRY | SLASH, FAINT_ATTACK, ROCK_SMASH, REST |
| Golbat | NONE | TOXIC, DOUBLE_TEAM, CONFUSE_RAY, WING_ATTACK |
| Octillery | QUICK_CLAW | SURF, ICE_BEAM, PSYBEAM, HYPER_BEAM |
| Victreebel | NONE | SLUDGE_BOMB, GIGA_DRAIN, SLEEP_POWDER, GROWTH |
| Typhlosion | CHARCOAL | FLAMETHROWER, QUICK_ATTACK, IRON_TAIL, THUNDER_PUNCH |
| Tyranitar | NONE | CRUNCH, EARTHQUAKE, ROCK_SLIDE, FIRE_PUNCH |

### Contra Cyndaquil

| Pokémon | Item | Golpes |
|---|---|---|
| Ursaring | CHESTO_BERRY | SLASH, FAINT_ATTACK, ROCK_SMASH, REST |
| Golbat | NONE | TOXIC, DOUBLE_TEAM, CONFUSE_RAY, WING_ATTACK |
| Victreebel | QUICK_CLAW | SLUDGE_BOMB, GIGA_DRAIN, SLEEP_POWDER, GROWTH |
| Houndoom | NONE | FLAMETHROWER, IRON_TAIL, CRUNCH, DOUBLE_TEAM |
| Feraligatr | MYSTIC_WATER | ICE_PUNCH, SURF, SLASH, BITE |
| Tyranitar | NONE | CRUNCH, EARTHQUAKE, ROCK_SLIDE, FIRE_PUNCH |

### Contra Totodile

| Pokémon | Item | Golpes |
|---|---|---|
| Ursaring | CHESTO_BERRY | SLASH, FAINT_ATTACK, ROCK_SMASH, REST |
| Golbat | NONE | TOXIC, DOUBLE_TEAM, CONFUSE_RAY, WING_ATTACK |
| Octillery | QUICK_CLAW | SURF, ICE_BEAM, PSYBEAM, HYPER_BEAM |
| Houndoom | NONE | FLAMETHROWER, IRON_TAIL, CRUNCH, SUNNY_DAY |
| Meganium | MIRACLE_SEED | SUNNY_DAY, GIGA_DRAIN, BODY_SLAM, SYNTHESIS |
| Tyranitar | NONE | CRUNCH, EARTHQUAKE, ROCK_SLIDE, FIRE_PUNCH |

May usa os golpes por nível da engine atual (não um moveset importado fixo).

Gerado por `scripts/document_region_challenges.py`; dados em
`games/super-fire-red/hack-data/region_pilot_trainers.json`.
