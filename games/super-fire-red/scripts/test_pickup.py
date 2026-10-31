"""Static checks for the Pickup tables: level bands, weights, chance and real item ids."""
from pathlib import Path
import json, re
from pickup_table import GAME, pickup_tiers

header = (GAME / 'include/battle_script_commands.h').read_text()
define = {name: int(value) for name, value in re.findall(r'#define (PICKUP_\w+)\s+(\d+)', header)}
items = {item['itemId'] for item in json.loads((GAME / 'src/data/items.json').read_text())['items']}
tiers = pickup_tiers()
failures = []


def expect(ok, message):
    if not ok:
        failures.append(message)


expect(define['PICKUP_CHANCE'] == 30, f'Pickup chance is {define["PICKUP_CHANCE"]}%, expected 30%')
expect(len(tiers) == define['PICKUP_LEVEL_TIERS'] == 5, f'{len(tiers)} level bands')
for band, tier in enumerate(tiers):
    name = f'Lv {band * 20 + 1}-{band * 20 + 20}'
    expect(sum(weight for _, weight in tier) == 100, f'{name}: weights total {sum(w for _, w in tier)}')
    expect(len(tier) <= define['PICKUP_ITEMS_PER_TIER'], f'{name}: {len(tier)} items exceed the row size')
    expect(len({item for item, _ in tier}) == len(tier), f'{name}: repeated item')
    for item, weight in tier:
        expect(item in items and item != 'ITEM_NONE', f'{name}: unknown item {item}')
        expect(weight > 0, f'{name}: {item} has no weight')
# Higher bands must not make the useful items rarer.
for item in ('ITEM_RARE_CANDY', 'ITEM_NUGGET'):
    rates = [dict(tier).get(item, 0) for tier in tiers]
    expect(rates == sorted(rates), f'{item} rate drops with level: {rates}')
if failures:
    raise SystemExit('\n'.join('FAIL ' + f for f in failures))
print(f'PASS: Pickup {define["PICKUP_CHANCE"]}% per battle; {len(tiers)} level bands of weights totalling 100, all real items.')
