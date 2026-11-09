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
    'Johto_NewBarkTown_Text_RivalSeen': '… … … … … … … …' + P + 'Você ganhou um Pokémon no LABORATÓRIO.' + P + 'Que desperdício. Alguém tão fraco como você.' + P + '… … … … … … … …' + P + 'Não entende o que estou dizendo?' + P + 'Pois eu também tenho um bom Pokémon.' + P + 'Vou mostrar o que quero dizer!',
    'Johto_NewBarkTown_Text_SilverWin': 'Humph. Feliz por ter vencido?',
    'Johto_NewBarkTown_Text_RivalAfter': '… … … … … … … …' + P + 'Quer saber quem eu sou?' + P + 'Vou ser o maior Treinador Pokémon do mundo.',
    'Johto_NewBarkTown_Lab_Text_ElmIntro': 'PROF. ELM: {PLAYER}! Aí está você!' + P + 'Preciso pedir um favor.' + P + 'Estou fazendo uma nova pesquisa sobre Pokémon. Será que você poderia me ajudar, {PLAYER}?' + P + 'Veja…' + P + 'Estou escrevendo um artigo para apresentar numa conferência.' + P + 'Mas ainda há coisas que não entendo muito bem.' + P + 'Então!' + P + 'Gostaria que você criasse um Pokémon que capturei recentemente.',
    'Johto_NewBarkTown_Lab_Text_ElmAccepted': 'Obrigado, {PLAYER}! Você vai ajudar muito!',
    'Johto_NewBarkTown_Lab_Text_ElmRefused': 'Mas… Por favor, preciso da sua ajuda!',
    'Johto_NewBarkTown_Lab_Text_ElmChooseAPokemon': 'Quero que você crie um dos Pokémon destas Poké Balls.' + P + 'Você será a primeira companhia desse Pokémon, {PLAYER}!' + P + 'Vamos. Escolha um!',
    'Johto_NewBarkTown_Lab_Text_ElmResearchAmbitions': 'Quando eu anunciar minhas descobertas, tenho certeza de que vamos entender melhor os muitos mistérios dos Pokémon.' + P + 'Pode contar com isso!',
    'Johto_NewBarkTown_Lab_Text_ChooseChikorita': 'PROF. ELM: Então, você gosta de {STR_VAR_1}, o Pokémon {STR_VAR_2}?',
    'Johto_NewBarkTown_Lab_Text_ChooseCyndaquil': 'PROF. ELM: Você vai escolher {STR_VAR_1}, o Pokémon {STR_VAR_2}?',
    'Johto_NewBarkTown_Lab_Text_ChooseTotodile': 'PROF. ELM: Você quer {STR_VAR_1}, o Pokémon {STR_VAR_2}?',
    'Johto_NewBarkTown_Lab_Text_ReceivedStarter': '{PLAYER} recebeu {STR_VAR_1}!',
    'Johto_NewBarkTown_Lab_Text_ChoseStarter': 'PROF. ELM: Também acho esse Pokémon ótimo!',
    'Johto_NewBarkTown_Lab_Text_DidntChooseStarter': 'PROF. ELM: Pense com cuidado.' + P + 'Sua parceria é importante.',
    'Johto_NewBarkTown_Lab_Text_ShouldntTouch': 'É melhor não mexer!',
    'Hoenn_LittlerootTown_Text_GoodLuckCatchingPokemon': 'Você vai capturar Pokémon? Boa sorte!',
    'Hoenn_LittlerootTown_Text_CanUsePCToStoreItems': 'Com um PC, dá para guardar itens e Pokémon.' + P + 'O poder da ciência é impressionante!',
    'Hoenn_LittlerootTown_Text_BirchSpendsDaysInLab': 'O PROF. BIRCH passa dias estudando no LABORATÓRIO e, de repente, sai pela natureza para pesquisar mais…' + P + 'Quando será que o PROF. BIRCH fica em casa?',
    'Hoenn_LittlerootTown_Text_TownSign': 'LITTLEROOT TOWN' + P + '“Uma cidade que nenhuma cor consegue tingir.”',
    'Hoenn_LittlerootTown_Text_ProfBirchsLab': 'LABORATÓRIO POKéMON\nDO PROF. BIRCH',
    'Hoenn_LittlerootTown_BrendansHouse_2F_Text_Notebook': '{PLAYER} abriu o caderno.' + P + 'REGRA DE AVENTURA 1: abra o MENU com START.' + P + 'REGRA DE AVENTURA 2: registre seu progresso com SALVAR.' + P + 'As outras páginas estão em branco…',
    'Hoenn_LittlerootTown_BrendansHouse_2F_Text_ItsAGameCube': 'É um Nintendo GameCube.' + P + 'Um Game Boy Advance está conectado para servir de controle.',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_Notebook': '{PLAYER} abriu o caderno.' + P + 'REGRA DE AVENTURA 1: abra o MENU com START.' + P + 'REGRA DE AVENTURA 2: registre seu progresso com SALVAR.' + P + 'As outras páginas estão em branco…',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_ItsAGameCube': 'É um Nintendo GameCube.' + P + 'Um Game Boy Advance está conectado para servir de controle.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_BirchIsntOneForDeskWork': 'O PROF. não é de trabalhar sentado à mesa. Ele prefere sair e viver as coisas a ler sobre elas aqui.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_CountlessPokemonAwait': 'PROF. BIRCH: Inúmeros Pokémon esperam por você!' + P + 'Ah, estou louco para sair e fazer trabalho de campo de novo!',
    'Hoenn_LittlerootTown_BrendansHouse_2F_Text_BrendanGettingReady': 'Pokémon totalmente recuperados… Itens todos guardados, e…',
    'Hoenn_LittlerootTown_MaysHouse_1F_Text_ShouldGoHomeEverySoOften': 'Acho maravilhoso as pessoas viajarem com Pokémon.' + P + 'Mas você deveria voltar para casa de vez em quando, para sua mãe saber que você está bem.' + P + 'Ela pode não dizer, mas tenho certeza de que se preocupa com você, {PLAYER}{KUN}.',
    'Hoenn_LittlerootTown_MaysHouse_1F_Text_DoYouHavePokemon': 'Oi, vizinho!' + P + 'Você já tem seus próprios Pokémon?',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_MayGettingReady': 'Pokémon totalmente recuperados! Itens prontos, e…',
    # Phase 3, step 2: Hoenn story (scripts/pilot_story.py)
    'Hoenn_Route101_Text_HelpMe': 'S-socorro!',
    'Hoenn_Route101_Text_PleaseHelp': 'Ei! Você aí!' + P + 'Por favor! Me ajude!' + P + 'Na minha BOLSA! Tem uma POKé BALL!',
    'Hoenn_Route101_Text_DontLeaveMe': 'A-aonde você vai?!' + P + 'Não me deixe assim!',
    'Hoenn_Route101_Text_ChooseStarter': 'Você escolhe {STR_VAR_1}?',
    'Hoenn_Route101_Text_ReceivedStarter': '{PLAYER} recebeu {STR_VAR_1}!',
    'Hoenn_Route101_Text_YouSavedMe': 'PROF. BIRCH: Ufa…' + P + 'Eu estava na grama alta estudando Pokémon selvagens quando fui atacado.' + P + 'Você me salvou. Muito obrigado!' + P + 'Hã?' + P + 'Oi, você é {PLAYER}{KUN}!' + P + 'Aqui não é lugar para conversar. Passe no meu LABORATÓRIO POKéMON mais tarde, combinado?',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_BirchAwayOnFieldwork': 'Hã? O PROF. BIRCH?' + P + 'O PROF. saiu para trabalho de campo. Ergo, ele não está aqui.' + P + 'Ah, deixe eu explicar o que é trabalho de campo.' + P + 'É estudar as coisas no ambiente natural, como campos e montanhas, em vez de num laboratório.' + P + 'O PROF. não é de trabalhar sentado à mesa. Ele prefere sair e viver as coisas a ler sobre elas aqui.',
    'Hoenn_LittlerootTown_ProfessorBirchsLab_Text_MightBeGoodIdeaToGoSeeRival': 'PROF. BIRCH: Se você se dedicar aos Pokémon e ganhar experiência, acho que você vai longe como TREINADOR.' + P + 'Minha filha, MAY, também estuda Pokémon enquanto me ajuda.' + P + '{PLAYER}{KUN}, que tal ir ver a MAY?',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_MayLetsBattle': 'Ah, oi, {PLAYER}{KUN}!' + P + '…Ah, entendi, meu pai te deu um Pokémon de presente.' + P + 'Já que estamos aqui, vamos batalhar rapidinho!' + P + 'Vou te mostrar como é ser TREINADOR.',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_MayDefeated': 'Uau! Que demais! {PLAYER}{KUN}, você manda muito bem!',
    'Hoenn_LittlerootTown_MaysHouse_2F_Text_MayTimeToHeadBack': 'MAY: Acho que entendo por que meu pai está de olho em você.' + P + 'Quer dizer, você acabou de ganhar esse Pokémon, e ele já gosta de você.' + P + 'Você deve conseguir fazer amizade com qualquer tipo de Pokémon.' + P + 'Bom, hora de voltar para o LABORATÓRIO.',
    # Phase 3, step 4: mothers (adapted for a visitor) and Elm's aide gifts
    'Hoenn_LittlerootTown_BrendansHouse_1F_Text_YouShouldRestABit': 'Como vai, {PLAYER}? Parece que o cansaço bateu um pouco.' + P + 'Acho que você deveria descansar um pouco.',
    'Hoenn_LittlerootTown_BrendansHouse_1F_Text_TakeCareHoney': 'Se cuide!',
    'Johto_NewBarkTown_PlayersHouse_1F_Text_MomLeaving2': 'Tome cuidado.' + P + 'Os Pokémon são seus amigos. Vocês precisam trabalhar em equipe.' + P + 'Agora vá!' + P + 'Estou torcendo por você!',
    'Johto_NewBarkTown_Lab_Text_AideGivePotion': '{PLAYER}, quero que você leve isto para a sua missão.',
    'Johto_NewBarkTown_Lab_Text_AideGiveBalls': '{PLAYER}!' + P + 'Use estas na sua missão da Pokédex!',
    'Johto_NewBarkTown_Lab_Text_AideExplainBalls': 'Para completar a Pokédex, você tem que capturar Pokémon.' + P + 'Jogue Poké Balls nos Pokémon selvagens para pegá-los.',
    'Johto_NewBarkTown_Text_Silver1': '. . .' + P + 'Então este é o famoso Laboratório Pokémon do Elm…',
    'Johto_NewBarkTown_Text_Silver2': '…Tá olhando o quê?',
    'Johto_NewBarkTown_PlayersHouse_1F_Text_MomsFriend': 'Oi, {PLAYER}!' + P + 'Vim fazer uma visita!' + P + 'Ficou sabendo?' + P + 'Minha filha está decidida a ser assistente do Prof. Elm.' + P + 'Ela adora Pokémon!',
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
    # SEAGALLOP to the pilots (new texts, no source game).
    'VermilionCity_Text_RegionSailorLittleroot': 'Agora que você venceu a LIGA POKéMON, o SEAGALLOP também navega para terras distantes.' + P + 'Vamos para LITTLEROOT TOWN, em HOENN?',
    'VermilionCity_Text_RegionSailorNewBark': 'Então que tal NEW BARK TOWN, em JOHTO?',
    'VermilionCity_Text_RegionSailorAnytime': 'Volte quando quiser navegar!',
    'Hoenn_LittlerootTown_Text_Ferry': 'O SEAGALLOP pode levar você de volta a VERMILION CITY, em KANTO.' + P + 'Quer zarpar?',
    'Johto_NewBarkTown_Text_Ferry': 'O SEAGALLOP pode levar você de volta a VERMILION CITY, em KANTO.' + P + 'Quer zarpar?',
}
SAME_IN_PT = {'Hoenn_Route101_Text_RouteSign', 'Johto_Route29_Text_Sign1',  # names only
              'Hoenn_LittlerootTown_BrendansHouse_1F_Text_Vigoroth1', 'Hoenn_LittlerootTown_BrendansHouse_1F_Text_Vigoroth2'}  # Vigoroth cries


def english(game=GAME):
    """label -> English key exactly as generate_locale stores it (real newline for \\n, no final $)."""
    out = {}
    paths = sorted(list((game / 'data/maps').glob('Hoenn_*/scripts.inc')) + list((game / 'data/maps').glob('Johto_*/scripts.inc')))
    paths.append(game / 'data/maps/VermilionCity/text.inc')  # the SEAGALLOP sailor to the pilots
    for path in paths:
        text = path.read_text(encoding='utf-8')
        for label, body in re.findall(r'^(\w+_Text_\w+)::\n((?:[ \t]*\.string "[^"]*"\n?)+)', text, re.M):
            if path.parent.name == 'VermilionCity' and not label.startswith('VermilionCity_Text_RegionSailor'):
                continue
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
    # BufferMonCategory reads these directly from the Pokédex (not map text labels).
    rows.update({'LEAF': 'FOLHA', 'FIRE MOUSE': 'RATO DE FOGO', 'BIG JAW': 'MANDIBULA'})
    out = GAME / 'hack-data/localization/region_pilot_ptbr.json'
    out.write_text(json.dumps(dict(sorted(rows.items())), ensure_ascii=False, indent=2) + '\n', encoding='utf-8', newline='\n')
    print(f'{len(rows)} PT-BR entries for {len(en)} pilot texts -> {out.name}')


if __name__ == '__main__':
    main()
