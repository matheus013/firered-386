"""Write hack-data/localization/region_pilot_ptbr.json: reviewed PT-BR for the region pilot map texts.

Translations are keyed here by the generated text label, and the English key is read from the map's
scripts.inc, so the lookup key always matches what the game prints. Pages are separated with \\p and left
unwrapped: generate_locale.py wraps this file like other map dialogue. Place names stay in English, as in
the rest of the PT-BR version; texts made only of names (route signs) need no entry.
"""
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT / 'games/super-fire-red'
P = r'\p'
PT = {
    'Hoenn_LittlerootTown_Text_GoodLuckCatchingPokemon': 'Você vai capturar Pokémon? Boa sorte!',
    'Hoenn_LittlerootTown_Text_CanUsePCToStoreItems': 'Com um PC, dá para guardar itens e Pokémon.' + P + 'O poder da ciência é impressionante!',
    'Hoenn_LittlerootTown_Text_BirchSpendsDaysInLab': 'O PROF. BIRCH passa dias estudando no LABORATÓRIO e, de repente, sai pela natureza para pesquisar mais…' + P + 'Quando será que o PROF. BIRCH fica em casa?',
    'Hoenn_LittlerootTown_Text_TownSign': 'LITTLEROOT TOWN' + P + '“Uma cidade que nenhuma cor consegue tingir.”',
    'Hoenn_LittlerootTown_Text_ProfBirchsLab': 'LABORATÓRIO POKéMON\nDO PROF. BIRCH',
    'Hoenn_LittlerootTown_BrendansHouse_2F_Text_Notebook': '{PLAYER} abriu o caderno.' + P + 'REGRA DE AVENTURA 1: abra o MENU com START.' + P + 'REGRA DE AVENTURA 2: registre seu progresso com SALVAR.' + P + 'As outras páginas estão em branco…',
    'Hoenn_LittlerootTown_BrendansHouse_2F_Text_ItsAGameCube': 'É um Nintendo GameCube.' + P + 'Um Game Boy Advance está conectado para servir de controle.',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_Notebook': '{PLAYER} abriu o caderno.' + P + 'REGRA DE AVENTURA 1: abra o MENU com START.' + P + 'REGRA DE AVENTURA 2: registre seu progresso com SALVAR.' + P + 'As outras páginas estão em branco…',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_ItsAGameCube': 'É um Nintendo GameCube.' + P + 'Um Game Boy Advance está conectado para servir de controle.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_BirchAwayOnFieldwork': 'Hã? PROF. BIRCH?' + P + 'O PROF. está em trabalho de campo. Ou seja, não está aqui.' + P + 'Ah, deixe eu explicar o que é trabalho de campo.' + P + 'É estudar as coisas no ambiente natural, como campos e montanhas, em vez de num laboratório.' + P + 'O PROF. não é de trabalhar sentado à mesa. Ele prefere sair e viver as coisas a ler sobre elas aqui.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine': 'É uma máquina de aparência séria. O PROF. deve usá-la nas pesquisas.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead': 'É um livro difícil demais de ler.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon': 'Está cheia de livros sobre Pokémon.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch': 'É um PC usado em pesquisas. Melhor não mexer nele.',
    'Hoenn_Route101_Text_TakeTiredPokemonToPokeCenter': 'Se os Pokémon se cansarem, leve-os a um CENTRO POKéMON.' + P + 'Tem um CENTRO POKéMON pertinho, em OLDALE TOWN.',
    'Hoenn_Route101_Text_WildPokemonInTallGrass': 'Pokémon selvagens pulam em você na grama alta.' + P + 'Se quiser capturar Pokémon, tem que entrar na grama alta e procurar.',
    'Johto_Gate_Route29_Route46_Text_Officer': 'Não dá para subir os desníveis.' + P + 'Mas dá para pular deles e pegar um atalho.',
    'Johto_Gate_Route29_Route46_Text_Youngster': 'Daqui em diante aparecem outros tipos de Pokémon.' + P + 'Se quiser pegar todos, tem que procurar em todo lugar.',
    'Johto_NewBarkTown_Text_FatManState3': 'E aí, {PLAYER}!' + P + 'Ouvi dizer que o Prof. Elm descobriu novos Pokémon.',
    'Johto_NewBarkTown_Text_LassState3': 'Ah! Seu Pokémon é uma graça! Queria ter um!',
    'Johto_NewBarkTown_Text_Sign': 'New Bark Town' + P + 'A cidade onde sopram os ventos de um novo começo',
    'Johto_NewBarkTown_Text_Door': 'A porta está trancada.',
    'Johto_NewBarkTown_Text_ElmsHouse': 'Casa do Prof. Elm',
    'Johto_NewBarkTown_Text_LabSign': 'Laboratório do Prof. Elm',
    'Johto_NewBarkTown_House1_Text_NeighborsDaughter': 'Pikachu é um Pokémon evoluído.' + P + 'Fiquei impressionada com as descobertas do Prof. Elm.' + P + 'Ele é muito famoso pelas pesquisas sobre a evolução dos Pokémon.' + P + '…suspiro…' + P + 'Queria ser pesquisadora como ele…',
    'Johto_NewBarkTown_House1_Text_Neighbor': 'Minha filha está decidida a ser assistente do Prof. Elm.' + P + 'Ela adora Pokémon!' + P + 'Mas eu também adoro!',
    'Johto_NewBarkTown_House1_Text_BookShelf': 'Está cheia de todo tipo de livro.',
    'Johto_NewBarkTown_House2_Text_ElmsSon': 'Quando eu crescer, vou ajudar meu pai!' + P + 'Vou ser um grande professor Pokémon!',
    'Johto_NewBarkTown_House2_Text_ElmsWife': 'Oi, {PLAYER}! Meu marido vive tão ocupado… Espero que ele esteja bem.' + P + 'Quando se envolve nas pesquisas sobre Pokémon, ele até esquece de comer.',
    'Johto_NewBarkTown_House2_Text_BookShelf': 'Está cheia de todo tipo de livro.',
    'Johto_NewBarkTown_Lab_Text_AideAlwaysBusy': 'Somos só três, então vivemos ocupados.',
    'Johto_NewBarkTown_Lab_Text_AideSave': 'Não se esqueça de salvar o jogo com frequência!' + P + 'Aperte START e escolha SALVAR!',
    'Johto_NewBarkTown_Lab_Text_SeriousLookingMachine': 'É uma máquina de aparência séria. O Prof. deve usá-la nas pesquisas.',
    'Johto_NewBarkTown_Lab_Text_CrammedWithBooksOnPokemon': 'Está cheia de livros sobre Pokémon.',
    'Johto_NewBarkTown_Lab_Text_PCUsedForResearch': 'É um PC usado em pesquisas. Melhor não mexer nele.',
    'Johto_NewBarkTown_Lab_Text_EmptyTrashCan': 'Está vazia.',
    'Johto_NewBarkTown_PlayersHouse_2F_Text_NES': '{PLAYER} jogou no NES.' + P + '…Pronto! Hora de ir!',
    'Johto_NewBarkTown_PlayersHouse_2F_Text_BookShelf': 'Está cheia de todo tipo de livro.',
    'Johto_Route29_Text_OldMan1': 'Os Pokémon se escondem na grama. Quem sabe quando vão aparecer…',
    'Johto_Route29_Text_Youngster': 'E aí, como estão seus Pokémon?' + P + 'Se estiverem fracos e despreparados para batalhar, fique longe da grama.',
    'Johto_Route29_Text_Boy_WaitingForDay': 'Estou esperando Pokémon que só aparecem mais tarde no dia.',
    'Johto_Route29_Text_Girl': 'Queria fazer uma pausa, então salvei para registrar meu progresso.',
    'Johto_Route29_Text_Lass': 'Está vendo aqueles desníveis? Dá medo pular deles.' + P + 'Mas dá para ir até New Bark sem passar pela grama.',
}
SAME_IN_PT = {'Hoenn_Route101_Text_RouteSign', 'Johto_Route29_Text_Sign1'}  # names only


def english(game=GAME):
    """label -> English key exactly as generate_locale stores it (real newline for \\n, no final $)."""
    out = {}
    for path in sorted(list((game / 'data/maps').glob('Hoenn_*/scripts.inc')) + list((game / 'data/maps').glob('Johto_*/scripts.inc'))):
        text = path.read_text(encoding='utf-8')
        for label, body in re.findall(r'^(\w+_Text_\w+)::\n((?:\t\.string "[^"]*"\n?)+)', text, re.M):
            en = ''.join(re.findall(r'\.string "([^"]*)"', body)).replace('\\n', '\n')
            out[label] = en[:-1] if en.endswith('$') else en
    return out


def main():
    en = english()
    missing = set(en) - set(PT) - SAME_IN_PT
    extra = set(PT) - set(en)
    if missing or extra:
        raise SystemExit(f'Labels without translation: {sorted(missing)}; translations without label: {sorted(extra)}')
    rows = {}
    for label, pt in PT.items():
        if en[label] in rows and rows[en[label]] != pt:
            raise SystemExit(f'Same English text translated two ways: {en[label]!r}')
        rows[en[label]] = pt
    out = GAME / 'hack-data/localization/region_pilot_ptbr.json'
    out.write_text(json.dumps(dict(sorted(rows.items())), ensure_ascii=False, indent=2) + '\n', encoding='utf-8', newline='\n')
    print(f'{len(rows)} PT-BR entries for {len(en)} pilot texts -> {out.name}')


if __name__ == '__main__':
    main()
