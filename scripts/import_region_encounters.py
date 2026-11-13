"""Import the wild encounter tables of the region pilot maps into hack-data/region_pilot_encounters.json.

balance_encounters.py appends these rows after the 386-species distribution, so Kanto and Sevii slots
stay untouched. Labels end in _Pilot (neither FireRed nor LeafGreen): the build compiles them in both
versions, and the player guides, audits and planner, which only read *_FireRed rows, leave them out
while the pilot maps are not reachable in normal play.
"""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SLOTS = {'land_mons': 12, 'water_mons': 5, 'rock_smash_mons': 5, 'fishing_mons': 10}  # FireRed field sizes
# (source repo, pinned commit, source label, source map, FireRed map, FireRed label, fields, reason)
TABLES = [
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gRoute101', 'MAP_ROUTE101',
     'MAP_HOENN_ROUTE101', 'gHoennRoute101_Pilot', ['land_mons'], 'Emerald table unchanged'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute29', 'MAP_ROUTE29',
     'MAP_JOHTO_ROUTE29', 'gJohtoRoute29_Pilot', ['land_mons'],
     'day table (FireRed has no day/night); Rock Smash left out: its rocks are object events, not imported'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gNewBarkTown', 'MAP_NEW_BARK_TOWN',
     'MAP_JOHTO_NEW_BARK_TOWN', 'gJohtoNewBarkTown_Pilot', ['water_mons', 'fishing_mons'],
     'day table; HNS has 12 water slots, all Tentacool 25-29, FireRed has 5; empty land table and Rock Smash left out'),
    # Hoenn batch 1 (scripts/install_hoenn_batch1.py)
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gRoute102', 'MAP_ROUTE102',
     'MAP_HOENN_ROUTE102', 'gHoennRoute102_Pilot', ['land_mons', 'fishing_mons'], 'Emerald tables unchanged; water left out (slot layout differs)'),
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gRoute103', 'MAP_ROUTE103',
     'MAP_HOENN_ROUTE103', 'gHoennRoute103_Pilot', ['land_mons', 'fishing_mons'], 'Emerald tables unchanged; water left out (slot layout differs)'),
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gPetalburgCity', 'MAP_PETALBURG_CITY',
     'MAP_HOENN_PETALBURG_CITY', 'gHoennPetalburgCity_Pilot', ['fishing_mons'], 'Emerald fishing table; water left out (slot layout differs)'),
    # Johto batch 1 (scripts/install_johto_batch1.py); day tables, FireRed has no day/night
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute30', 'MAP_ROUTE30',
     'MAP_JOHTO_ROUTE30', 'gJohtoRoute30_Pilot', ['land_mons', 'water_mons'],
     'day table; fishing left out (HNS has 12 fishing slots, FireRed 10); Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute31', 'MAP_ROUTE31',
     'MAP_JOHTO_ROUTE31', 'gJohtoRoute31_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'day table; Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gCherrygroveCity', 'MAP_CHERRYGROVE_CITY',
     'MAP_JOHTO_CHERRYGROVE_CITY', 'gJohtoCherrygroveCity_Pilot', ['fishing_mons'],
     'day table; water left out (12 HNS slots with Tentacruel in 4, FireRed has 5); Rock Smash left out'),
    # Johto batch 2
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gVioletCity', 'MAP_VIOLET_CITY',
     'MAP_JOHTO_VIOLET_CITY', 'gJohtoVioletCity_Pilot', ['fishing_mons'],
     'day table; empty land table; water left out (12 HNS slots, FireRed has 5); Rock Smash left out'),
    # Johto batch 3
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute32', 'MAP_ROUTE32',
     'MAP_JOHTO_ROUTE32', 'gJohtoRoute32_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'day table; Rock Smash left out'),
    # Johto batch 4 (Azalea Town only has Rock Smash in HNS: left out)
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute33', 'MAP_ROUTE33',
     'MAP_JOHTO_ROUTE33', 'gJohtoRoute33_Pilot', ['land_mons'], 'day table; Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gUnionCave_1F', 'MAP_UNION_CAVE_1F',
     'MAP_JOHTO_UNION_CAVE_1F', 'gJohtoUnionCave1F_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'HNS table unchanged'),
    # Johto batch 5
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gIlexForest', 'MAP_ILEX_FOREST',
     'MAP_JOHTO_ILEX_FOREST', 'gJohtoIlexForest_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'day table; Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute34', 'MAP_ROUTE34',
     'MAP_JOHTO_ROUTE34', 'gJohtoRoute34_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'day table; Rock Smash left out'),
    # Johto batch 8
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute37', 'MAP_ROUTE37',
     'MAP_JOHTO_ROUTE37', 'gJohtoRoute37_Pilot', ['land_mons'], 'day table; Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gEcruteakCity', 'MAP_ECRUTEAK_CITY',
     'MAP_JOHTO_ECRUTEAK_CITY', 'gJohtoEcruteakCity_Pilot', ['fishing_mons'], 'day table; water left out (12 HNS slots); Rock Smash left out'),
    # Johto batch 9
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute38', 'MAP_ROUTE38',
     'MAP_JOHTO_ROUTE38', 'gJohtoRoute38_Pilot', ['land_mons'], 'day table; Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute39', 'MAP_ROUTE39',
     'MAP_JOHTO_ROUTE39', 'gJohtoRoute39_Pilot', ['land_mons'], 'day table; Rock Smash left out'),
    # Hoenn batch 2
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gRoute104', 'MAP_ROUTE104',
     'MAP_HOENN_ROUTE104', 'gHoennRoute104_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'Emerald tables unchanged'),
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gPetalburgWoods', 'MAP_PETALBURG_WOODS',
     'MAP_HOENN_PETALBURG_WOODS', 'gHoennPetalburgWoods_Pilot', ['land_mons'], 'Emerald table unchanged'),
    # Hoenn batch 3
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gRoute116', 'MAP_ROUTE116',
     'MAP_HOENN_ROUTE116', 'gHoennRoute116_Pilot', ['land_mons'], 'Emerald table unchanged'),
    ('pokeemerald', '731ad5bfd6e6f265508d0efcca0ba42f9dcf5881', 'gRusturfTunnel', 'MAP_RUSTURF_TUNNEL',
     'MAP_HOENN_RUSTURF_TUNNEL', 'gHoennRusturfTunnel_Pilot', ['land_mons'], 'Emerald table unchanged'),
    # Johto batch 7
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute35', 'MAP_ROUTE35',
     'MAP_JOHTO_ROUTE35', 'gJohtoRoute35_Pilot', ['land_mons', 'water_mons', 'fishing_mons'], 'day table; Rock Smash left out'),
    ('HNS_modern', 'bf25b6ecd23c009b899da570d62a210f35e01809', 'gRoute36', 'MAP_ROUTE36',
     'MAP_JOHTO_ROUTE36', 'gJohtoRoute36_Pilot', ['land_mons'], 'day table; Rock Smash left out'),
]


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--references', type=Path, required=True, help='folder holding the pokeemerald and HNS_modern checkouts')
    p.add_argument('--out', type=Path, default=ROOT / 'games/super-fire-red/hack-data/region_pilot_encounters.json')
    a = p.parse_args()
    rows = []
    for repo, commit, label, source_map, target_map, target_label, fields, reason in TABLES:
        group = json.loads((a.references / repo / 'src/data/wild_encounters.json').read_text(encoding='utf-8'))['wild_encounter_groups'][0]
        source = next(e for e in group['encounters'] if e['base_label'] == label and e['map'] == source_map)
        row = {'map': target_map, 'base_label': target_label}
        for field in fields:
            mons = source[field]['mons'][:SLOTS[field]]
            if len(mons) != SLOTS[field] or any(m['species'] == 'SPECIES_NONE' for m in mons):
                raise ValueError(f'{label} {field}: expected {SLOTS[field]} real species')
            if field == 'water_mons' and len(source[field]['mons']) > SLOTS[field] and len({json.dumps(m) for m in source[field]['mons']}) != 1:
                raise ValueError('water slots differ; truncating to 5 would change the table')
            row[field] = {'encounter_rate': source[field]['encounter_rate'], 'mons': mons}
        rows.append({'source': {'repository': repo, 'commit': commit, 'label': label, 'map': source_map, 'note': reason}, 'row': row})
    a.out.write_text(json.dumps({'scope': 'Region pilot maps only; appended by balance_encounters.py', 'tables': rows},
                                ensure_ascii=False, indent=2) + '\n', encoding='utf-8', newline='\n')
    print(f'{len(rows)} pilot tables -> {a.out}')


if __name__ == '__main__':
    main()
