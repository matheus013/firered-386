# Elite Four — adaptações de equipes campeãs

Implementado na versão **v0.1.1-dev**, para a primeira Liga e suas revanches. As equipes mantêm as espécies disponíveis das referências e completam as vagas com Pokémon das gerações I–III. São adaptações, não as equipes históricas exatas.

Estado atual: **v0.3.1-dev**. Os níveis abaixo refletem a curva introduzida na v0.2.0-dev; as 14 variantes de E4/Campeão passaram novamente nos testes de criação das equipes e avanço de turnos. Isso não equivale a uma Liga inteira concluída por um jogador. [Checklist e escopo](TEST_CHECKLIST.md).

## Equipes implementadas

| Membro | Referência mundial | Time na hack | Nível máximo Liga / revanche |
|---|---|---|---|
| Lorelei | Abram Burrows, 2012 Junior | HITMONTOP, CLOYSTER, PILOSWINE, JYNX, TYRANITAR, LAPRAS | 80 / 100 |
| Bruno | Ray Rizzo, 2010 Senior | HARIYAMA, KYOGRE, LUDICOLO, SLOWBRO, GROUDON, MACHAMP | 83 / 100 |
| Agatha | Nikolai Zielinski, 2014 Senior | MAWILE, GENGAR, DUSCLOPS, MISDREAVUS, TYRANITAR, LANTURN | 86 / 100 |
| Lance | Wolfe Glick, 2016 Masters | HITMONTOP, KYOGRE, GENGAR, CLAYDOL, RAICHU, RAYQUAZA | 89 / 100 |

## Estratégias e substituições

### Lorelei

Referência: CLOYSTER, HITMONTOP, LANDORUS, THUNDURUS, TYRANITAR, VOLCARONA.

Helping Hand + Surf/Ice Beam na abertura; quatro tipos Gelo, sono e controle de velocidade; Tyranitar como alternativa de pressão por areia.

| Pokémon | Função na adaptação |
|---|---|
| HITMONTOP | Suporte Intimidate/Helping Hand para Cloyster e os atacantes especiais |
| CLOYSTER | Atacante Água/Gelo com Surf que atinge apenas os adversários nesta engine |
| PILOSWINE | Substitui Landorus: cobertura Terra, imunidade elétrica e Mud Shot para reduzir velocidade |
| JYNX | Substitui Volcarona: atacante especial, cobertura Psíquica e Lovely Kiss |
| TYRANITAR | Referência original preservada; areia é alternativa para pressionar o fim da luta, com custo de dano residual nos parceiros |
| LAPRAS | Substitui Thundurus: cobertura elétrica e Água/Gelo, resistente especial |

### Bruno

Referência: CRESSELIA, DIALGA, GROUDON, HARIYAMA, KYOGRE, LUDICOLO.

Hariyama + Kyogre na abertura com Fake Out; Ludicolo aproveita chuva, Slowbro paralisa; Groudon oferece modo de sol e cobertura separado, sem Trick Room.

| Pokémon | Função na adaptação |
|---|---|
| HARIYAMA | Fake Out protege a abertura de Kyogre; cobertura física e resistência a Fogo/Gelo |
| KYOGRE | Chuva automática e pressão especial com Surf; Thunder só recebe precisão de chuva quando o clima está ativo |
| LUDICOLO | Swift Swim aproveita a chuva; Giga Drain cobre adversários de Água |
| SLOWBRO | Substitui Cresselia: suporte Psíquico resistente e Thunder Wave em lugar de Trick Room |
| GROUDON | Alternativa ao modo de chuva: Drought, Mud Shot seguro para o aliado e cobertura de Fogo/Pedra |
| MACHAMP | Substitui Dialga: reforça o tipo Lutador, cobre Normais/Pedra/Aço e pune status com Guts |

### Agatha

Referência: GARCHOMP, GENGAR, HYDREIGON, MAWILE, ROTOM, TYRANITAR.

Mawile + Gengar: Intimidate e Fake Tears apoiam dano especial; três Fantasmas, queimadura/paralisia, cobertura elétrica/água e Tyranitar como alternativa de areia.

| Pokémon | Função na adaptação |
|---|---|
| MAWILE | Intimidate protege o grupo; Fake Tears abre espaço para ataques especiais do parceiro |
| GENGAR | Atacante especial com Levitate e queimadura; sem depender de Mega ou de Shadow Ball físico |
| DUSCLOPS | Substitui Garchomp: Fantasma defensivo, dano fixo e Will-O-Wisp |
| MISDREAVUS | Substitui Hydreigon: Fantasma com Levitate, Thunder Wave e Pain Split |
| TYRANITAR | Pressão física e resistência especial; areia causa dano residual também nos parceiros não imunes |
| LANTURN | Substitui Rotom-Wash: Água/Elétrico, Volt Absorb e cobertura contra Água/Voador/Terra |

### Lance

Referência: BRONZONG, GENGAR, HITMONTOP, KYOGRE, RAICHU, RAYQUAZA.

Hitmontop + Kyogre: Intimidate e Helping Hand; Claydol usa telas, Raichu paralisa; Rayquaza fecha com dano misto e Air Lock, que também suspende a chuva aliada.

| Pokémon | Função na adaptação |
|---|---|
| HITMONTOP | Intimidate e Helping Hand apoiam Kyogre; sem Fake Out ou Wide Guard indisponíveis para ele nesta base |
| KYOGRE | Pressão de chuva com Surf, Ice Beam e Thunder; sem Reversão Primal |
| GENGAR | Atacante especial e Will-O-Wisp com Levitate; sem Mega/Shadow Tag |
| CLAYDOL | Substitui Bronzong: Psíquico com Levitate e duas telas; sem Trick Room |
| RAICHU | Thunder Wave e Light Screen, com cobertura elétrica; Static, não Lightning Rod moderno |
| RAYQUAZA | Finalizador Dragão misto: Dragon Claw é especial na geração III; Air Lock suspende efeitos de todos os climas |

## Implementação e limites

- Seis integrantes em duplas nos oito elencos. Na primeira Liga, os maiores níveis são 80/83/86/89, seguidos pelo Campeão no 92. O teto de EXP avança a cada membro vencido, nos dois modos. Na revanche, todos os integrantes da E4 e do Campeão estão no 100.
- IVs 31, naturezas fixas, EVs até 252 por atributo e 510 no total, e habilidade escolhida entre as existentes na espécie. Os valores exatos estão em TRAINER_REVIEW.md e data/e4_champion_teams.json.
- As naturezas e habilidades são aplicadas na criação efetiva da equipe inimiga; os atributos são recalculados após os EVs. O Campeão também recebe treinamento competitivo; os demais treinadores usam IVs progressivos, sem EVs específicos. O formato do save preserva seu tamanho.
- Golpes validados por nível, TM/HM, tutor de FireRed ou herança/evolução/ovos descritos nos arquivos da própria base. Helping Hand de Hitmontop vem de Tyrogue; Mud Shot de Piloswine vem de Swinub; Will-O-Wisp de Gengar vem de Gastly.
- Surf atinge somente os adversários nesta engine de geração III. Os elencos não usam ataques que atinjam o parceiro.
- Categorias de dano ainda seguem o tipo: Shadow Ball é físico e Dragon Claw é especial. Os sets foram escolhidos levando isso em conta.
- Sem Mega Evolução, Reversão Primal, Trick Room, Choice Scarf, Assault Vest, Skill Link ou habilidades modernas. Hitmontop não usa Fake Out; Raichu usa Static.
- Groudon e Kyogre oferecem modos alternativos de clima: Drought substitui a chuva. Air Lock de Rayquaza suspende também os benefícios da chuva do próprio Kyogre. Sand Stream de Tyranitar causa dano residual a parceiros não imunes; não se trata de um benefício gratuito para toda a equipe.
- As equipes mistas preservam os integrantes históricos disponíveis e reforçam a especialidade nas substituições; não são monotipo. Bruno e Lance mantêm lendários das referências.
- A sinergia foi planejada para as regras disponíveis, mas não garante decisões perfeitas da IA, repetição do desempenho histórico ou balanceamento de campanha já concluído.

## Reproduzir e verificar

`python3 scripts/balance_trainers.py --check` valida geração e legalidade dos dados. Após compilar, `python3 scripts/test_elite_four.py` executa as oito lutas da E4 e as seis variantes do Campeão em ROMs descartáveis no mGBA, confere os seis Pokémon efetivamente criados (espécies, níveis, golpes, itens, IVs, EVs, naturezas e habilidades), chegada à seleção de ação e avanço de turnos. Logs e capturas ficam em output/tests/elite-four/. Não é um teste de campanha completa.

## Fontes históricas

- [Equipes campeãs por ano e divisão](https://bulbapedia.bulbagarden.net/wiki/World_Championships).
- [Retrospectiva oficial: equipe e estratégia de Ray Rizzo em 2010](https://www.pokemon.com/uk/features/relive-the-eras-of-pokemon-vgc).
- [Análise oficial: equipe e estratégia de Wolfe Glick em 2016](https://www.pokemon.com/us/strategy/what-happened-at-the-world-championships).
- [Ficha oficial da composição de 2016 compartilhada com Brendan Zheng](https://www.pokemon.com/us/play-pokemon/worlds/2016/teams/seniors).
- [Anúncio histórico do Cloyster de Abram, com referência ao site oficial](https://projectpokemon.org/home/forums/topic/24751-bw-b2w2-event-pok%C3%A9mon-championships-regional-cloyster-us/).

A associação de campeões aos membros da E4 e todas as substituições são decisões de design desta hack, sem endosso dos jogadores citados.


Atualização v0.4.0-dev: desafios e Torre recebem IVs 31 e EVs treinados. As equipes temáticas descritas aqui valem com randomizer desligado; ligado, espécies e golpes mudam, e o treinamento é recalculado por função. [Regras](SETTINGS.md).
