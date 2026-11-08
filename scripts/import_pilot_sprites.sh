#!/usr/bin/env bash
# Import every pilot NPC sprite FireRed lacks (idempotent). Run from the repository root with the
# pokeemerald and HNS_modern checkouts in ../map-references. See scripts/import_npc_sprite.py.
set -euo pipefail
R=${REFERENCES:-../map-references}
E=$R/pokeemerald/graphics/object_events
H=$R/HNS_modern/graphics/object_events
run() { python scripts/import_npc_sprite.py --game games/super-fire-red --report docs/map-audits/pilot-sprites.json "$@"; }
EM="pokeemerald 731ad5b"
HN="HNS_modern bf25b6e"
# Phase 1
run --png $E/pics/people/twin.png --pal $E/palettes/npc_2.pal --name twin --const TWIN --symbol Twin --source "$EM graphics/object_events/pics/people/twin.png"
run --png $H/pics/people/scientist_f.png --pal $H/palettes/scientist_f.pal --name scientist_f --const SCIENTIST_F --symbol ScientistF --source "$HN graphics/object_events/pics/people/scientist_f.png"
# Phase 2: main characters
run --png $E/pics/people/mom.png --pal $E/palettes/npc_4.pal --name em_mom --const EM_MOM --symbol EmMom --source "$EM graphics/object_events/pics/people/mom.png"
run --png $E/pics/people/woman_4.png --pal $E/palettes/npc_1.pal --name woman_4 --const WOMAN_4 --symbol Woman4 --source "$EM graphics/object_events/pics/people/woman_4.png"
run --png $E/pics/people/ninja_boy.png --pal $E/palettes/npc_1.pal --name ninja_boy --const NINJA_BOY --symbol NinjaBoy --size 16x16 --source "$EM graphics/object_events/pics/people/ninja_boy.png"
run --png $E/pics/people/prof_birch.png --pal $E/palettes/npc_3.pal --name prof_birch --const PROF_BIRCH --symbol ProfBirch --source "$EM graphics/object_events/pics/people/prof_birch.png"
run --png $E/pics/pokemon/vigoroth.png --pal $E/palettes/vigoroth.pal --name vigoroth_carrying_box --const VIGOROTH_CARRYING_BOX --symbol VigorothCarryingBox --size 32x32 --frames 0,0,0,1,2,1,2,1,2 --source "$EM graphics/object_events/pics/pokemon/vigoroth.png (frames 0-2)"
run --png $E/pics/pokemon/vigoroth.png --pal $E/palettes/vigoroth.pal --name vigoroth_facing_away --const VIGOROTH_FACING_AWAY --symbol VigorothFacingAway --size 32x32 --frames 3,3,3,4,4,4,4,4,4 --source "$EM graphics/object_events/pics/pokemon/vigoroth.png (frames 3-4)"
run --png $H/pics/people/mom.png --pal $H/palettes/npc_3.pal --name johto_mom --const JOHTO_MOM --symbol JohtoMom --source "$HN graphics/object_events/pics/people/mom.png"
run --png $H/pics/people/special/elm.png --pal $H/palettes/elm.pal --name prof_elm --const PROF_ELM --symbol ProfElm --source "$HN graphics/object_events/pics/people/special/elm.png"
run --png $H/pics/people/special/silver.png --pal $H/palettes/silver.pal --name silver --const SILVER --symbol Silver --source "$HN graphics/object_events/pics/people/special/silver.png"
# Phase 3: Birch rescue on Route 101
run --png $E/pics/pokemon/zigzagoon.png --pal $E/palettes/npc_1.pal --name zigzagoon --const ZIGZAGOON --symbol Zigzagoon --size 16x16 --frames 0,1,2,0,0,1,1,2,2 --source "$EM graphics/object_events/pics/pokemon/zigzagoon.png"
run --png $E/pics/misc/birchs_bag.png --pal $E/palettes/npc_2.pal --name birchs_bag --const BIRCHS_BAG --symbol BirchsBag --size 16x16 --frames 0,0,0,0,0,0,0,0,0 --source "$EM graphics/object_events/pics/misc/birchs_bag.png"
