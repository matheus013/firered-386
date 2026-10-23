# Revisão dos líderes e da Elite Four

Revisão da prévia v0.5.0-preview.2. Mantidos os seis integrantes, tipos temáticos, tetos de nível e adaptações dos campeões oficiais. A revanche da E4 continua no nível 100. Os resultados e o SHA da ROM ficam em [data/ai_review_validation.json](../data/ai_review_validation.json).

## Correções de decisões

- O desempate por dano agora compara os golpes de cada Pokémon, além dos alvos. Antes, golpes com a mesma nota ainda podiam ser sorteados sem comparar o dano.
- Golpes em área somam o dano estimado nos dois adversários presentes. O cálculo continua obedecendo à redução de dano da engine atual.
- Corrigido o uso do retorno de `TypeCalc`: imunidades não podem contar como dano ou nocaute nas consultas nativas. A estimativa também descarta Water Absorb, Volt Absorb e Flash Fire.
- Dano fixo por nível, Dragon Rage, Sonic Boom e Super Fang deixam de usar a potência fictícia 1 para o desempate. Return/Frustration usam amizade; Water Spout/Eruption usam HP. Thunder considera chuva e sol, enquanto o clima estiver funcionando.
- Air Lock e Cloud Nine são considerados nos quatro combatentes vivos. Adversários ausentes ou desmaiados não suprimem clima.
- Earthquake pode combinar com o primeiro Protect/Detect já escolhido pelo próprio parceiro, desde que ele não esteja dormindo, congelado ou no turno de Truant. Tentativas consecutivas continuam penalizadas.
- Tentativas de aplicar dois status principais no mesmo alvo recebem penalidade quando o primeiro plano aliado já é conhecido. Helping Hand, telas, clima, velocidade e limitações de PP continuam considerados.

## Histórico da batalha

Aplicadas as correções locais dos ramos de IA `BUGFIX` da base, sem ativar esse sinalizador globalmente para outros sistemas. O histórico agora separa golpes observados, habilidade e efeito de item dos quatro combatentes, limita a leitura aos quatro golpes e não registra o mesmo golpe repetidamente. A troca de integrante limpa o histórico daquele campo. Consultas sobre o parceiro passam a consultar o parceiro correto; consultas sobre o adversário usam os golpes observados, sem revelar sua lista completa.

## Ajustes nos times

- Surge: Manectric usa **Static**. Na regra da geração III desta engine, Lightning Rod podia desviar Thunder/Thunderbolt dos próprios aliados.
- Agatha e Lance: Gengar usa **Shadow Ball** no lugar de Psychic, aproveitando o bônus Ghost e a categoria especial moderna. Thunderbolt e suporte continuam disponíveis.
- Conferidos os oito ginásios e as quatro adaptações da E4 contra a compatibilidade de golpes importada, habilidades, natureza e EVs. Todos continuam com IVs 31; as tabelas completas são geradas em [TRAINER_REVIEW.md](TRAINER_REVIEW.md) e no [README](../README.md#times-dos-desafios-em-sequência).

## Revisão dos demais sistemas

O checklist desta revisão também inclui criação de equipes, limites de nível, treinamento, menus, encontros, Hardcore e as 23 lojas nos dois modos. Categorias, poder e precisão dos 354 golpes existentes são comparados com o snapshot oficial de dados usado pelo projeto; os geradores conferem os 386 encontros e os guias. Consulte os resultados consolidados para distinguir verificação de dados, execução no emulador e pendências.

## Limites

A IA continua heurística. A escolha do segundo integrante pode usar o plano aliado já escolhido; o primeiro não recebe um planejamento conjunto das duas ações. Trocas, reposições e itens da mochila mantêm a lógica nativa. A IA não consulta a ação enfileirada pelo jogador, mas conserva suposições nativas sobre atributos e habilidades adversárias.

O desempate não é um simulador completo: precisão por estágios, dano de vários acertos, todos os golpes variáveis, Hidden Power e Weather Ball ainda exigem cobertura específica. Counter/Mirror Coat não recebem uma previsão de dano futuro no desempate. A decisão de clima usa heurísticas de tipos/habilidades, sem otimizar todos os confrontos de chuva e sol de Bruno ou a entrada de Air Lock de Rayquaza. Surf continua atingindo somente os adversários nesta engine. Efeitos modernos, Fairy e os 225 golpes ausentes continuam pendentes, conforme [MODERNIZATION.md](MODERNIZATION.md).

As batalhas completas desta revisão são isoladas, com um bot simples controlando cópias dos times dos chefes, HP e dano normais, sem itens de cura da mochila do jogador. Verificam conclusão e travamentos; não medem a dificuldade de uma campanha nem demonstram acesso legal a esses times naquele ponto do mapa. Não certificam campanha natural, sobrevivência Nuzlocke, randomizer ou conclusão do pós-jogo caminhando.
