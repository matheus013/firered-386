"""Generate player guides from the current FireRed trainer and encounter sources."""
from collections import defaultdict
from pathlib import Path
import argparse
import json
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT / 'game'
START = '<!-- GENERATED CHALLENGE TEAMS START -->'
END = '<!-- GENERATED CHALLENGE TEAMS END -->'


def read(path):
    return path.read_text(encoding='utf-8-sig')


def pretty(name):
    return name.replace('SPECIES_', '').replace('ITEM_', '').replace('MOVE_', '').replace('_', ' ').title().replace('Mr Mime', 'Mr. Mime').replace('Nidoran F', 'Nidoran♀').replace('Nidoran M', 'Nidoran♂').replace('Ho Oh', 'Ho-Oh').replace('Farfetchd', "Farfetch’d")


def area_name(folder):
    text = folder.replace('_', ' — ')
    return re.sub(r'(?<=[a-z])(?=[A-Z0-9])|(?<=[0-9])(?=[A-Z])', ' ', text)


def slug(name):
    return re.sub(r'[^a-z0-9-]', '', name.lower().replace('_', '-'))


def natural(name):
    return [int(s) if s.isdigit() else s for s in re.split(r'(\d+)', name)]


def challenge_guide():
    teams = json.loads(read(ROOT / 'data/trainer_balance.json'))['teams']
    sequence = [
        ('Rival — laboratório de Oak', 'RivalOaksLab', 'Primeira luta, simples; nível 5.'),
        ('Rival — Route 22, primeira visita', 'RivalRoute22Early', 'Opcional, antes de Brock.'),
        ('Brock — Pewter Gym', 'LeaderBrock', ''),
        ('Rival — Cerulean City', 'RivalCerulean', 'Pode ocorrer antes ou depois de Misty.'),
        ('Misty — Cerulean Gym', 'LeaderMisty', ''),
        ('Rival — S.S. Anne', 'RivalSsAnne', ''),
        ('Lt. Surge — Vermilion Gym', 'LeaderLtSurge', ''),
        ('Erika — Celadon Gym', 'LeaderErika', 'A ordem entre este ginásio e o esconderijo Rocket pode variar.'),
        ('Giovanni — Rocket Hideout', 'BossGiovanni', ''),
        ('Rival — Pokémon Tower', 'RivalPokemonTower', ''),
        ('Koga — Fuchsia Gym', 'LeaderKoga', ''),
        ('Koichi — Fighting Dojo de Saffron', 'BlackBeltKoichi', 'Opcional; pode ser visitado antes de Koga. O nível acompanha a etapa de Saffron.'),
        ('Rival — Silph Co.', 'RivalSilph', ''),
        ('Giovanni — Silph Co.', 'BossGiovanni2', ''),
        ('Sabrina — Saffron Gym', 'LeaderSabrina', ''),
        ('Blaine — Cinnabar Gym', 'LeaderBlaine', ''),
        ('Biker Goon — Three Island, primeira luta', 'BikerGoon', 'Visita às ilhas após Blaine; a sequência dos Bikers pode ser deixada para depois.'),
        ('Biker Goon — Three Island, segunda luta', 'BikerGoon2', ''),
        ('Biker Goon — Three Island, terceira luta', 'BikerGoon3', ''),
        ('Paxton — líder dos Bikers de Three Island', 'CueBallPaxton', ''),
        ('Giovanni — Viridian Gym', 'LeaderGiovanni', ''),
        ('Rival — Route 22, antes da Liga', 'RivalRoute22Late', ''),
        ('Lorelei — primeira Liga', 'EliteFourLorelei', ''),
        ('Bruno — primeira Liga', 'EliteFourBruno', ''),
        ('Agatha — primeira Liga', 'EliteFourAgatha', ''),
        ('Lance — primeira Liga', 'EliteFourLance', ''),
        ('Campeão — primeira Liga', 'ChampionFirst', ''),
        ('Administradora Rocket — Rocket Warehouse', 'TeamRocketAdmin', 'Pós-jogo: etapa de recuperação da Sapphire.'),
        ('Administrador Rocket — Rocket Warehouse', 'TeamRocketAdmin2', ''),
        ('Gideon — Rocket Warehouse', 'ScientistGideon', ''),
        ('Lorelei — revanche', 'EliteFourLorelei2', ''),
        ('Bruno — revanche', 'EliteFourBruno2', ''),
        ('Agatha — revanche', 'EliteFourAgatha2', ''),
        ('Lance — revanche', 'EliteFourLance2', ''),
        ('Campeão — revanche', 'ChampionRematch', ''),
    ]
    # The three laboratory exceptions are preserved in the ROM rather than in
    # the regenerated doubles catalog. Read their actual species and levels.
    parties = read(GAME / 'src/data/trainer_parties.h')
    lab = {}
    for suffix in ('Squirtle', 'Bulbasaur', 'Charmander'):
        body = re.search(r'sParty_RivalOaksLab' + suffix + r'\[\] = \{(.*?)\n\};', parties, re.S)[1]
        lab[suffix] = [{'species': re.search(r'\.species = SPECIES_(\w+)', body)[1], 'level': int(re.search(r'\.lvl = (\d+)', body)[1])}]
    out = [START, '## Times dos desafios em sequência', '',
           'Roteiro sugerido dos desafios principais e opcionais de destaque da versão atual. A exploração permite mudar algumas ordens; os tetos continuam seguindo a sequência de insígnias. Os demais treinadores e a Torre estão em [TRAINER_REVIEW.md](TRAINER_REVIEW.md).', '',
           'Cada equipe aparece na ordem do elenco: nas duplas, os dois primeiros abrem a luta. No rival e no Campeão, **a variante indica o inicial do rival**, não o seu: você escolhe Bulbasaur → rival Charmander; Charmander → rival Squirtle; Squirtle → rival Bulbasaur. Não existe equipe única comum às três variantes.', '',
           'Os detalhes de golpes, itens e treinamento de cada adversário estão no catálogo completo; abaixo, espécies e níveis extraídos dos mesmos dados usados na ROM.', '']
    for n, (title, key, note) in enumerate(sequence, 1):
        out += [f'### {n:02d}. {title}', '']
        if note:
            out += [note, '']
        variants = [(suffix, lab[suffix]) for suffix in lab] if key == 'RivalOaksLab' else []
        if key != 'RivalOaksLab':
            direct = teams.get('sParty_' + key)
            if direct:
                variants = [('Equipe', direct['team'])]
            else:
                for suffix in ('Squirtle', 'Bulbasaur', 'Charmander'):
                    variants.append((suffix, teams['sParty_' + key + suffix]['team']))
        out += ['| Variante | Time em ordem de entrada |', '|---|---|']
        for variant, team in variants:
            assert team and all(1 <= mon['level'] <= 100 for mon in team)
            out.append('| ' + (pretty(variant) if variant != 'Equipe' else variant) + ' | ' + ' → '.join(f"{pretty(m['species'])} **{m['level']}**" for m in team) + ' |')
        out.append('')
    out += ['As novas revanches de líderes ainda não foram implementadas; por isso, não há times de revanche de ginásio inventados neste roteiro. Para planejar capturas, consulte [ENCOUNTERS.md](ENCOUNTERS.md).', '', END]
    return '\n'.join(out), len(sequence)


def range_text(values):
    result = []
    for number in sorted(set(values)):
        if result and result[-1][1] + 1 == number:
            result[-1][1] = number
        else:
            result.append([number, number])
    return ', '.join(str(a) if a == b else f'{a}–{b}' for a, b in result)


def encounters_guide():
    group = json.loads(read(GAME / 'src/data/wild_encounters.json'))['wild_encounter_groups'][0]
    fields = {field['type']: field for field in group['fields']}
    all_rows = [row for row in group['encounters'] if row['base_label'].endswith('_FireRed')]
    assert len(all_rows) == 132
    map_data = {json.loads(read(path))['id']: json.loads(read(path)) for path in (GAME/'data/maps').glob('*/map.json')}
    maps = {key: data['name'] for key, data in map_data.items()}
    layouts = {layout['id']: layout for layout in json.loads(read(GAME/'data/layouts/layouts.json'))['layouts'] if 'id' in layout}
    headers = read(GAME/'src/data/tilesets/headers.h')
    attributes = {name: [v[0] for v in struct.iter_unpack('<I', (GAME/path).read_bytes())] for name, path in re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)', read(GAME/'src/data/tilesets/metatiles.h'))}
    tilesets = {name: attributes[re.search(r'\.metatileAttributes = (\w+)', body)[1]] for name, body in re.findall(r'const struct Tileset (\w+) =\s*\{(.*?)\};', headers, re.S)}

    def has_land_tiles(map_id):
        layout = layouts[map_data[map_id]['layout']]
        for block, in struct.iter_unpack('<H', (GAME/layout['blockdata_filepath']).read_bytes()):
            tile = block & 0x3FF
            attrs = tilesets[layout['primary_tileset']][tile] if tile < 640 else tilesets[layout['secondary_tileset']][tile-640]
            if (attrs >> 24) & 7 == 1:
                return True
        return False
    rows, inactive = [], []
    seen = set()
    for row in all_rows:
        if row['map'] in seen:
            assert row['map'] == 'MAP_SIX_ISLAND_ALTERING_CAVE'
            inactive.append(row)
        else:
            seen.add(row['map']); rows.append(row)
    routes = sorted((row for row in rows if row['map'].startswith('MAP_ROUTE')), key=lambda r: natural(maps[r['map']]))
    others = sorted((row for row in rows if not row['map'].startswith('MAP_ROUTE')), key=lambda r: natural(maps[r['map']]))
    out = ['# Encontros por rota e área — FireRed 386', '',
           'Guia dos encontros válido para v0.3.1-dev. A distribuição foi introduzida na v0.3.0-dev: as 386 espécies estão distribuídas e podem ser capturadas diretamente no modo Normal, sem troca, reprodução ou tickets de evento. Veja também o [índice das 386 espécies](SPECIES_LOCATIONS.md). No Hardcore, os mesmos encontros existem, mas a cota por seção limita as capturas da tentativa; não é possível prometer a coleção completa sob essa regra.', '',
           '## Como ler as probabilidades', '',
           '- **Chance** é a participação da espécie na tabela, condicionada a ocorrer um encontro pelo método indicado. Não é chance por passo, por lançamento de vara ou de captura.',
           '- Grama/caverna, Surf, Rock Smash, Old Rod, Good Rod e Super Rod têm tabelas separadas. Cada uma soma 100%; slots repetidos da mesma espécie foram somados.',
           '- A coluna **Níveis por slot** informa a faixa e seu peso dentro da tabela. Uma faixa como `3–5 (20%)` significa que esse slot pesa 20%; o nível é sorteado na faixa. Slots sobrepostos não significam distribuição uniforme entre todos os níveis da espécie.',
           '- Repel, taxa da área, deslocamento, itens/habilidades e estado do jogo afetam se a batalha ocorre. As porcentagens abaixo são os pesos nominais dos slots do jogo; não incluem o pequeno viés do sorteio por módulo do gerador aleatório.',
           '- Em Pokémon Tower, o Silph Scope é necessário para identificar/capturar os fantasmas comuns. Nas câmaras Tanoby, é necessário resolver Tanoby Key; as formas de Unown são discriminadas.',
           '- Os 21 lendários e míticos têm novos encontros aleatórios de 1% em áreas de pós-jogo, com trava pela primeira vitória na Liga. Encontros fixos originais continuam existindo; lendários derrotados em eventos também podem ser procurados nos novos encontros aleatórios.',
           '- Roamer, batalhas fixas e eventos aparecem em seções próprias. Tabelas alternativas de Altering Cave estão separadas das acessíveis normalmente. Tabelas de terra sem terreno de encontro no layout recebem aviso explícito; a presença de dados não cria grama onde ela não existe.', '',
           'Os caminhos `game/...` citados abaixo são fontes locais reconstruídos por `scripts/setup.sh`, não arquivos versionados diretamente neste repositório. As alterações da hack estão em [patches/current.patch](patches/current.patch).', '',
           '## Índice por rota', '']
    for row in routes:
        folder = maps[row['map']]
        out.append(f'- [{area_name(folder)}](#{slug(folder)})')
    out += ['', '## Índice das demais áreas', '']
    for row in others:
        folder = maps[row['map']]
        out.append(f'- [{area_name(folder)}](#{slug(folder)})')
    unown = [[0]*11+[27], [2,2,2,3,3,3,7,7,7,20,20,14], [13,13,13,13,18,18,18,18,8,8,4,4], [15,15,11,11,9,9,17,17,17,16,16,16], [24,24,19,19,6,6,6,5,5,5,10,10], [21,21,21,22,22,22,23,23,12,12,1,1], [25]*11+[26]]
    chambers = ['Monean', 'Liptoo', 'Weepth', 'Dilford', 'Scufib', 'Rixy', 'Viapois']
    # Check this special-case annotation against the production C table.
    source_unown = read(GAME/'src/wild_encounter.c').split('sUnownLetterSlots[][LAND_WILD_COUNT] = {', 1)[1].split('\n};', 1)[0]
    source_unown = re.sub(r'//[^\n]*', '', source_unown)
    assert [list(map(int, re.findall(r'\d+', line))) for line in re.findall(r'\{([^{}]+)\}', source_unown)] == unown
    methods = [('land_mons', 'Grama / caverna'), ('water_mons', 'Surf'), ('rock_smash_mons', 'Rock Smash'), ('fishing_mons', 'Pesca')]
    method_count = 0

    def render(row, alternate=False):
        nonlocal method_count
        folder = maps[row['map']]
        if not alternate:
            out.extend(['', f'<a id="{slug(folder)}"></a>', '', f'## {area_name(folder)}', ''])
        else:
            out.extend(['', f"### {row['base_label']}", ''])
        out.extend([f"Fonte: tabela `{row['base_label']}` de `game/src/data/wild_encounters.json`; mapa `{folder}`.", ''])
        if row['map'] == 'MAP_SIX_ISLAND_ALTERING_CAVE' and not alternate:
            out.extend(['Conjunto 0 redesenhado para esta hack, com Deoxys raro após a Liga. Os conjuntos alternativos originais constam somente no apêndice.', ''])
        if row['map'] == 'MAP_SSANNE_EXTERIOR':
            out.extend(['**Tabelas originais de acesso limitado:** a S.S. Anne parte antes da obtenção normal de Surf e das varas avançadas. Estes dados foram preservados, mas não contam como prova de captura das 386 espécies. Todas elas têm opções em áreas permanentes.', ''])
        for field, title in methods:
            if field not in row:
                continue
            data = row[field]
            rates = fields[field]['encounter_rates']
            assert len(rates) == len(data['mons'])
            groups = fields[field].get('groups', {title: list(range(len(rates)))})
            for label, slots in groups.items():
                method_count += 1
                assert sum(rates[i] for i in slots) == 100
                label = {'old_rod': 'Old Rod', 'good_rod': 'Good Rod', 'super_rod': 'Super Rod'}.get(label, label)
                out.extend([f'### {label}', '', '| Pokémon | Níveis possíveis | Chance | Níveis por slot (peso) |', '|---|---|---:|---|'])
                if field == 'land_mons' and not has_land_tiles(row['map']):
                    out[-4:-4] = ['**Tabela sem terreno de encontro terrestre no layout atual:** estes slots constam nos dados, mas não representam captura por grama/caverna neste mapa.', '']
                species = defaultdict(lambda: {'weight': 0, 'levels': set(), 'bands': defaultdict(int)})
                for i in slots:
                    mon = data['mons'][i]
                    name = pretty(mon['species'])
                    if mon['species'] == 'SPECIES_UNOWN':
                        chamber = next(index for index, c in enumerate(chambers) if c + 'Chamber' in folder)
                        letter = unown[chamber][i]
                        name += ' ' + ('ABCDEFGHIJKLMNOPQRSTUVWXYZ!?'[letter])
                    entry = species[name]
                    entry['weight'] += rates[i]
                    entry['levels'].update(range(mon['min_level'], mon['max_level']+1))
                    entry['bands'][(mon['min_level'], mon['max_level'])] += rates[i]
                assert sum(entry['weight'] for entry in species.values()) == 100
                for name, entry in sorted(species.items(), key=lambda x: (-x[1]['weight'], x[0])):
                    bands = '; '.join(f'{range_text(range(lo, hi+1))} ({weight}%)' for (lo, hi), weight in sorted(entry['bands'].items()))
                    out.append(f"| {name} | {range_text(entry['levels'])} | {entry['weight']}% | {bands} |")
                out.append('')
    for row in routes + others:
        render(row)
    out.extend(['## Encontros fixos e eventos', '', 'A espécie é fixa (100% ao acionar o evento disponível); isso não garante captura. As flags de história, itens e derrotas anteriores continuam necessárias. Eventos fixos derrotados não foram resetados; os novos encontros aleatórios oferecem outra forma de capturar todos os lendários, repetível no Normal e sujeita à cota de área no Hardcore.', '', '| Local | Pokémon | Nível | Condição / disponibilidade |', '|---|---|---:|---|'])
    static_count = 0
    for path in sorted((GAME/'data/maps').glob('*/scripts.inc')):
        for match in re.finditer(r'\b(setwildbattle|seteventmon)\s+SPECIES_(\w+),\s*(\d+)', read(path)):
            _, species, level = match.groups()
            note = 'Encontro fixo de história; uma oportunidade por objeto/evento.'
            if species == 'SNORLAX': note = 'Exige Poké Flute; um encontro nesta rota.'
            if species == 'ELECTRODE': note = 'Poké Ball falsa; duas ocorrências na Power Plant, listadas separadamente.'
            if species == 'MAROWAK': note = '**Não capturável**: fantasma de história, exige Silph Scope.'
            if species == 'MEWTWO': note = 'Pós-jogo; concluir a rede de Celio libera Cerulean Cave.'
            if species in {'DEOXYS','LUGIA','HO_OH'}: note = '**Área de evento**: requer acesso por ticket/flags correspondentes; esta hack ainda não fornece novo acesso normal a esses eventos.'
            out.append(f'| {area_name(path.parent.name)} | {pretty(species)} | {level} | {note} |')
            static_count += 1
    out.extend(['', '## Lendário errante', '',
                'Após concluir a rede de Celio (entrega da Sapphire), a base ativa um único errante de nível 50: **Entei** se você escolheu Bulbasaur, **Suicune** se escolheu Charmander, ou **Raikou** se escolheu Squirtle. Ele se desloca pelas rotas de Kanto (1–25; Route 21 North, não South).', '',
                'Quando ele está ativo e no mesmo mapa do jogador, o teste de substituição tem **25%** de chance nominal, após a verificação de encontro e antes de gerar o selvagem comum. Não é 25% em cada rota em todos os momentos: sua localização muda e Repel ainda pode impedir a batalha. Pesca e Rock Smash não usam essa substituição. Fonte: `game/src/roamer.c` e `game/src/wild_encounter.c`.', '',
                '## Presentes, compras, fósseis e trocas', '',
                'Recebimentos não são sorteios de grama: a espécie é garantida quando você escolhe/paga/cumpre os requisitos (não uma probabilidade de encontro). Escolhas exclusivas e pré-requisitos originais continuam vigentes. No Hardcore, presentes e trocas por NPC ficam fora da cota de selvagens.', '',
                '| Local | Recebimento | Nível | Requisito |', '|---|---|---:|---|',
                '| Laboratório de Oak | Bulbasaur / Charmander / Squirtle | 5 | Escolher um inicial |',
                '| Route 4 — Centro Pokémon | Magikarp | 5 | Comprar por ₽500 |',
                '| Celadon — Condominiums, Roof Room | Eevee | 25 | Presente único |',
                '| Saffron — Fighting Dojo | Hitmonlee / Hitmonchan | 25 | Vencer Koichi; escolher um |',
                '| Silph Co. — 7F | Lapras | 25 | Presente do funcionário |',
                '| Cinnabar — laboratório, Experiment Room | Omanyte / Kabuto / Aerodactyl | 5 | Entregar Helix Fossil / Dome Fossil / Old Amber correspondente |',
                '| Five Island — Water Labyrinth | Ovo de Togepi | 5 ao nascer | Mostrar ao idoso um líder com amizade alta e ter espaço |',
                '| Celadon — Game Corner | Abra | 9 | 180 moedas |',
                '| Celadon — Game Corner | Clefairy | 8 | 500 moedas |',
                '| Celadon — Game Corner | Dratini | 18 | 2.800 moedas |',
                '| Celadon — Game Corner | Scyther | 25 | 5.500 moedas |',
                '| Celadon — Game Corner | Porygon | 26 | 9.999 moedas |', '',
                'Trocas por NPC, com nível igual ao Pokémon entregue: Route 2 House, Abra → Mr. Mime; Cerulean House 3, Poliwhirl → Jynx; Underground Path North Entrance, Nidoran♂ → Nidoran♀; Vermilion House 2, Spearow → Farfetch’d; Route 11 East Entrance 2F, Nidorino → Nidorina; Route 18 East Entrance 2F, Golduck → Lickitung; Cinnabar Lab Lounge, Raichu → Electrode e Venonat → Tangela; Cinnabar Lab Experiment Room, Ponyta → Seel. Fonte: `game/src/data/ingame_trades.h` e scripts dos mapas.', '',
                '## Altering Cave — tabelas alternativas inativas', '',
                'O jogo usa o conjunto 0 redesenhado por padrão. Os oito conjuntos abaixo foram preservados da base e dependem de `VAR_ALTERING_CAVE_WILD_SET`; não foi implementado evento normal para selecioná-los. Essas espécies também foram distribuídas em áreas acessíveis, sem depender destas tabelas. Os conjuntos alternativos não se somam à tabela principal.', ''])
    for row in inactive:
        render(row, alternate=True)
    out.extend(['## Cobertura e atualização', '', f'{len(rows)} mapas com tabelas comuns de FireRed, {len(routes)} entradas de rotas (Route 21 separada em Norte/Sul), {len(inactive)} tabelas alternativas de Altering Cave e {static_count} ocorrências fixas/eventos catalogadas. As tabelas de LeafGreen não entram neste guia. Áreas sem tabela aleatória não recebem encontros inventados.', '',
                'Distribuição reproduzível: `python3 scripts/balance_encounters.py --check`, com cobertura por espécie em `data/encounter_distribution.json`. Gerar novamente os guias: `python3 scripts/generate_guides.py`. Conferir sem escrever: `python3 scripts/generate_guides.py --check`. O gerador valida soma de 100% por método, número de slots, separação das varas, formas de Unown e duplicação somente em Altering Cave. Não altera a ROM.'])
    return '\n'.join(out) + '\n', len(rows), method_count


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    readme = read(ROOT/'README.md')
    section, challenges = challenge_guide()
    if START in readme:
        a, tail = readme.split(START, 1)
        _, b = tail.split(END, 1)
        readme = a + section + b
    else:
        readme = readme.replace('## Todas as mudanças implementadas', section+'\n\n## Todas as mudanças implementadas', 1)
    encounters, count, methods = encounters_guide()
    distribution = json.loads(read(ROOT/'data/encounter_distribution.json'))
    maps={json.loads(read(p))['id']:p.parent.name for p in (GAME/'data/maps').glob('*/map.json')}
    index=['# Onde capturar as 386 espécies — v0.3.1-dev','','Distribuição introduzida na v0.3.0-dev e mantida na v0.3.1-dev. Uma opção de captura direta por espécie, escolhida entre as áreas de menor etapa de acesso da distribuição. Todas as opções são independentes de inicial, troca, reprodução ou ticket. Mais locais e faixas de níveis estão no [guia de encontros](ENCOUNTERS.md).','',
           'A coluna de chance soma os slots da espécie no mesmo método, antes dos modificadores de encontro. No Hardcore, a regra de captura por seção pode impedir a coleção completa. Lendários/míticos aleatórios exigem a primeira vitória na Liga; eventos fixos originais conservam seus requisitos.','',
           '| Nº Nacional | Pokémon | Um local de captura | Método | Níveis | Chance |','|---:|---|---|---|---:|---:|']
    for row in distribution['species']:
        grouped={}
        for e in row['encounters']:
            method={'land_mons':'Grama / caverna','water_mons':'Surf','rock_smash_mons':'Rock Smash'}.get(e['field'])
            if method is None:method='Old Rod' if e['index']<2 else 'Good Rod' if e['index']<5 else 'Super Rod'
            key=(e['map'],method)
            if key not in grouped:grouped[key]={'weight':0,'levels':set(),'stage':e['stage']}
            grouped[key]['weight']+=e['weight'];grouped[key]['levels'].update(range(e['min_level'],e['max_level']+1))
        (map_id,method),e=min(grouped.items(),key=lambda item:(item[1]['stage'],-item[1]['weight'],item[0]))
        folder=maps[map_id]
        index.append(f"| {row['national_dex']:03d} | {pretty(row['species'])} | [{area_name(folder)}](ENCOUNTERS.md#{slug(folder)}) | {method} | {range_text(e['levels'])} | {e['weight']}% |")
    species_index='\n'.join(index)+'\n'
    for path, content in ((ROOT/'README.md', readme), (ROOT/'ENCOUNTERS.md', encounters), (ROOT/'SPECIES_LOCATIONS.md',species_index)):
        if args.check:
            assert read(path) == content, f'Out of date: {path.name}'
        else:
            path.write_text(content, encoding='utf-8', newline='\n')
    print(f'PASS: {challenges} challenges, {count} encounter maps; {methods} method tables validated (including inactive alternatives).')


if __name__ == '__main__':
    main()
