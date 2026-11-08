"""Import HNS Silver's Victory Road teams and portrait, preserving source moves/items.

Only unused Brendan trainer slots are allocated; the old trainer IDs stay fixed.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
from PIL import Image


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--game', type=Path, required=True)
    args = parser.parse_args()
    source, game = args.source, args.game
    raw = (source / 'src/data/trainer_parties.h').read_text(encoding='utf-8')
    constants = '\n'.join((game / f'include/constants/{name}.h').read_text(encoding='utf-8')
                          for name in ('species', 'moves', 'items'))
    known = set(re.findall(r'#define\s+(\w+)', constants))
    manifest = game / 'hack-data/region_pilot_trainers.json'
    payload = json.loads(manifest.read_text(encoding='utf-8'))
    for i, (player, opponent) in enumerate((('CHIKORITA','Cyndaquil'), ('CYNDAQUIL','Totodile'), ('TOTODILE','Chikorita'))):
        label = 'sParty_Rival_' + opponent + '5'
        body = re.search(label + r'\[\] = \{(.*?)\n\};', raw, re.S)[1]
        team = []
        for mon in re.finditer(r'\.iv = (\d+),\s*\.lvl = (\d+),\s*\.species = (\w+),\s*\.heldItem = (\w+),\s*\.moves = \{([^}]+)\}', body):
            iv, level, species, item, moves = mon.groups()
            moves = [m.strip() for m in moves.split(',')]
            if len(moves) != 4 or any(token not in known for token in [species, item, *moves]):
                raise ValueError(f'{label}: unsupported move/species/item')
            team.append(dict(species=species, level=int(level), iv=int(iv), item=item, moves=moves))
        if len(team) != 6:
            raise ValueError(f'{label}: expected six Pokemon')
        payload['trainers']['TRAINER_BRENDAN' + (f'_{i+1}' if i else '')] = dict(
            party='sParty_Brendan' + (str(i+1) if i else ''), name='SILVER',
            player_starter='SPECIES_' + player, source=label, source_repo='resetes12/HNS_modern',
            level_offset=40, trainer_pic='TRAINER_PIC_SILVER', trainer_class='TRAINER_CLASS_RIVAL_LATE', team=team)
    portrait = source / 'graphics/trainers/front_pics/wally.png'
    with Image.open(portrait) as image:
        if image.mode != 'P' or image.size != (64,64) or max(image.get_flattened_data()) > 15:
            raise ValueError('Silver portrait is not indexed 64x64 4bpp')
    shutil.copyfile(portrait, game / 'graphics/trainers/front_pics/johto_silver.png')
    shutil.copyfile(source / 'graphics/trainers/palettes/wally.pal', game / 'graphics/trainers/palettes/johto_silver.pal')
    manifest.write_text(json.dumps(payload, indent=2)+'\n', encoding='utf-8', newline='\n')


if __name__ == '__main__':
    main()
