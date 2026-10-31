"""Read the Pickup level-band tables from the game source."""
from pathlib import Path
import re

GAME = Path(__file__).resolve().parents[1]


def pickup_tiers():
    source = (GAME / 'src/battle_script_commands.c').read_text()
    body = source[source.index('sPickupItems[PICKUP_LEVEL_TIERS][PICKUP_ITEMS_PER_TIER] ='):]
    body = body[:body.index('\n};')]
    return [[(item, int(weight)) for item, weight in re.findall(r'\{\s*(ITEM_\w+),\s*(\d+)\s*\}', tier)]
            for tier in re.findall(r'\n    \{\n(.*?)\n    \},', body, re.S)]
