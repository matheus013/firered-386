# Créditos de mapas e gráficos regionais

Recursos de terceiros usados nos pilotos de Johto e Hoenn (branch `codex/firered-johto-hoenn`).
Pokémon e seus mapas originais pertencem aos respectivos titulares; este é um projeto de fãs,
sem afiliação oficial.

## Hoenn: Littleroot, Route 101 e interiores

- Mapas, tilesets, portas e animações: [pret/pokeemerald](https://github.com/pret/pokeemerald),
  commit `731ad5bfd6e6f265508d0efcca0ba42f9dcf5881`. Convertidos para o FireRed por
  `scripts/convert_hoenn_*.py`, `scripts/prepare_hoenn_*.py` e `scripts/install_hoenn_interiors.py`.

## Johto: New Bark Town, Route 29 e interiores

Fonte: [resetes12/HNS_modern](https://github.com/resetes12/HNS_modern), commit
`bf25b6ecd23c009b899da570d62a210f35e01809` (Heart & Soul "Modern"). Mapas, tilesets, portas e
animações instalados por `scripts/install_johto_pilot.py`. Créditos conforme o README dessa fonte:

- **Heart & Soul**: Lil Dill (direção); TixoRebel, InfiniteBacon42, Exclsior, Phantonomy, DaniRainbow
  (desenvolvimento). Versão "Modern": resetes12.
- **Tilesets**: Crystal Advance — Kertra; Ekat99; TheDeadHeroAlistair; Johto Redrawn Team.
- **Mapas e referências de mapeamento**: Crystal Advance — Kertra; Fire Gold — blackfragrant; SkidMarc25.
- **Base de código da fonte**: pokeemerald (pret), Modern Emerald (resetes12), pokeemerald-expansion.

**Licença:** o repositório HNS_modern não traz arquivo de licença (o GitHub não informa licença),
e o README só lista créditos. A importação com créditos foi decisão do mantenedor deste projeto em
07/11/2026. Se algum autor pedir remoção ou condições diferentes, os arquivos ficam isolados em
`data/tilesets/*/johto_pilot_*`, `data/layouts/Johto_*`, `data/maps/Johto_*` e
`graphics/door_anims/johto/`, e o instalador pode ser revertido.

## Sprites de personagens

Importados por `scripts/import_npc_sprite.py` e redesenhados numa paleta de moradores do FireRed;
detalhes em `docs/map-audits/pilot-sprites.json`.

- pret/pokeemerald: Twin, mãe (`EM_MOM`), `WOMAN_4`, `NINJA_BOY`, Prof. Birch e Vigoroth
  (`graphics/object_events/pics/people/` e `pics/pokemon/vigoroth.png`).
- resetes12/HNS_modern: cientista (`SCIENTIST_F`), mãe de Johto (`JOHTO_MOM`), Prof. Elm e Silver
  (`graphics/object_events/pics/people/`), mesma situação de licença dos tilesets de Johto acima.

Todas as importações estão em `scripts/import_pilot_sprites.sh`.
