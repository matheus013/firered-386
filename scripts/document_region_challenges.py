"""Generate the region pilot challenge roster from the same manifest as the game."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def render(payload):
    rows = ['# Desafios dos pilotos regionais', '',
            'Acesso após a primeira Liga pelo SEAGALLOP de Vermilion. Apenas os pilotos,',
            'não as campanhas completas de Johto/Hoenn. Níveis provisórios; teto continua 100.', '',
            'Hoenn: escolha do inicial na bolsa → Zigzagoon nível 84 → May no quarto dela.',
            'Johto: pedido do Elm → escolha de um inicial → Silver em New Bark.', '',
            'May e Silver usam duplas, IVs 31 e EVs por função (508 no total).',
            'Os times preservam as espécies da fonte; não representam uma revisão competitiva',
            'completa da expansão. Os presentes de inicial são de nível 5, um por região.', '',
            '| Rival | Inicial escolhido pelo jogador | Equipe (nível) |',
            '|---|---|---|']
    for entry in payload['trainers'].values():
        offset = entry.get('level_offset', payload['level_offset'])
        team = []
        for mon in entry['team']:
            species, level = (mon['species'], mon['level']) if isinstance(mon, dict) else mon[:2]
            team.append(f'{species.removeprefix("SPECIES_").title()} {level + offset}')
        starter = entry['player_starter'].removeprefix('SPECIES_').title()
        rows.append(f'| {entry["name"]} | {starter} | {", ".join(team)} |')
    rows += ['', '## Golpes e itens do Silver', '',
             'Importados de HNS_modern `sParty_Rival_*5`; +40 níveis. As variantes compartilham',
             'a origem, mas cada linha abaixo é gerada dos dados da respectiva equipe.', '']
    for entry in payload['trainers'].values():
        if not isinstance(entry['team'][0], dict):
            continue
        rows += ['### Contra ' + entry['player_starter'].removeprefix('SPECIES_').title(), '',
                 '| Pokémon | Item | Golpes |', '|---|---|---|']
        for mon in entry['team']:
            rows.append('| ' + mon['species'].removeprefix('SPECIES_').title() + ' | '
                        + mon['item'].removeprefix('ITEM_') + ' | '
                        + ', '.join(m.removeprefix('MOVE_') for m in mon['moves']) + ' |')
        rows.append('')
    rows += ['May usa os golpes por nível da engine atual (não um moveset importado fixo).', '',
             'Gerado por `scripts/document_region_challenges.py`; dados em',
             '`games/super-fire-red/hack-data/region_pilot_trainers.json`.', '']
    return '\n'.join(rows)


if __name__ == '__main__':
    payload = json.loads((ROOT / 'games/super-fire-red/hack-data/region_pilot_trainers.json').read_text(encoding='utf-8'))
    (ROOT / 'docs/REGION_PILOT_CHALLENGES.md').write_text(render(payload), encoding='utf-8', newline='\n')
