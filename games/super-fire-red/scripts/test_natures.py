"""Check every nature against the reference table: stat effects, names, flavors and confusion berries.

Static source checks; the same reference also drives the native checks in test_ai_move_native.py.
"""
from pathlib import Path
import json, re

ROOT = Path(__file__).resolve().parents[3]
GAME = ROOT / 'games/super-fire-red'
ref = json.loads((GAME / 'hack-data/nature_reference.json').read_text(encoding='utf-8'))
STAT_INDEX = {'attack': 0, 'defense': 1, 'speed': 2, 'spatk': 3, 'spdef': 4}  # sNatureStatTable column order
failures = []
checks = 0


def expect(ok, message):
    global checks
    checks += 1
    if not ok:
        failures.append(message)


def read(path):
    return (GAME / path).read_text(encoding='utf-8')


def expected_effects():
    """nature -> (stat effects in table column order, liked flavor, disliked flavor)."""
    result = {}
    for raised, row in zip(ref['stats'], ref['grid']):
        for lowered, nature in zip(ref['stats'], row):
            effects = [0] * 5
            liked = disliked = None
            if raised != lowered:
                effects[STAT_INDEX[raised]] = 1
                effects[STAT_INDEX[lowered]] = -1
                liked, disliked = ref['flavor_by_stat'][raised], ref['flavor_by_stat'][lowered]
            result[nature] = (effects, liked, disliked)
    return result


expected = expected_effects()
expect(len(expected) == 25, f'reference grid has {len(expected)} distinct natures')

ids = {name: int(value) for name, value in re.findall(r'#define NATURE_(\w+)\s+(\d+)', read('include/constants/pokemon.h'))}
expect(sorted(ids) == sorted(expected), f'nature constants differ: {sorted(set(ids) ^ set(expected))}')
expect(sorted(ids.values()) == list(range(25)), 'nature ids are not 0..24')

table = {name: [int(v) for v in values.split(',')]
         for name, values in re.findall(r'\[NATURE_(\w+)\]\s*=\s*\{([^}]*)\}', read('src/pokemon.c'))}
for nature, (effects, _, _) in expected.items():
    expect(table.get(nature) == effects, f'{nature}: stat table {table.get(nature)} != reference {effects}')

names = dict(re.findall(r's(\w+)NatureName\[\] = _\("(\w+)"\)', read('src/data/text/nature_names.h')))
pointers = dict(re.findall(r'\[NATURE_(\w+)\] = s(\w+)NatureName', read('src/data/text/nature_names.h')))
for nature in expected:
    expect(names.get(pointers.get(nature)) == nature, f'{nature}: displayed name is {names.get(pointers.get(nature))}')

berry_h = read('include/berry.h')
flavor_enum = re.search(r'enum\s*\{\s*FLAVOR_SPICY,(.*?)FLAVOR_COUNT', berry_h, re.S)
flavors = ['spicy'] + [f.strip().removeprefix('FLAVOR_').lower() for f in flavor_enum.group(1).split(',') if f.strip()]
expect(flavors == ['spicy', 'dry', 'sweet', 'bitter', 'sour'], f'flavor enum order {flavors}')

pokemon_c = read('src/pokemon.c')
body = pokemon_c[pokemon_c.index('sPokeblockFlavorCompatibilityTable[NUM_NATURES * FLAVOR_COUNT] ='):]
rows = re.findall(r'^\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),?\s*//\s*(\w+)', body[:body.index('};')], re.M)
expect(len(rows) == 25, f'flavor table has {len(rows)} rows')
by_id = {v: k for k, v in ids.items()}
for index, (*values, label) in enumerate(rows):
    nature = by_id.get(index)
    expect(label.upper() == nature, f'flavor row {index} is labelled {label}, expected {nature}')
    _, liked, disliked = expected[nature]
    want = [1 if f == liked else -1 if f == disliked else 0 for f in flavors]
    expect([int(v) for v in values] == want, f'{nature}: flavors {values} != reference {want} (likes {liked}, dislikes {disliked})')

holds = {m.group(1): int(m.group(2)) for m in re.finditer(r'#define HOLD_EFFECT_CONFUSE_(\w+)\s+(\d+)', read('include/constants/hold_effects.h'))}
items = {item['itemId']: item.get('holdEffect') for item in json.loads(read('src/data/items.json'))['items']}
battle_util = read('src/battle_util.c')
for flavor, berry in ref['confusion_berry_by_flavor'].items():
    hold = 'HOLD_EFFECT_CONFUSE_' + flavor.upper()
    expect(items.get(f'ITEM_{berry}_BERRY') == hold, f'{berry} berry holds {items.get(f"ITEM_{berry}_BERRY")}, expected {hold}')
    # modern_battle.c derives the flavor as holdEffect - HOLD_EFFECT_CONFUSE_SPICY.
    expect(holds.get(flavor.upper(), -1) - holds['SPICY'] == flavors.index(flavor), f'{hold} is not FLAVOR_{flavor.upper()} away from SPICY')
    expect(re.search(rf'case {hold}:\s*TRY_EAT_CONFUSE_BERRY\(FLAVOR_{flavor.upper()}\)', battle_util) is not None,
           f'battle_util does not eat {hold} as FLAVOR_{flavor.upper()}')

example = ref['example']
effects, liked, disliked = expected[example['nature']]
expect((liked, disliked) == (example['likes'], example['dislikes']), f'reference example {example["nature"]} is inconsistent')
expect(effects[STAT_INDEX[example['raises']]] == 1 and effects[STAT_INDEX[example['lowers']]] == -1, 'reference example stats are inconsistent')

if failures:
    print('\n'.join('FAIL ' + f for f in failures))
    raise SystemExit(f'{len(failures)} of {checks} nature checks failed')
print(f'PASS: {checks} checks; 25 natures match the reference table (stats, names, flavors, confusion berries).')
