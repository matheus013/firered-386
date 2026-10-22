> **Auditoria histórica de v0.1.1-dev.** Os níveis e algumas limitações abaixo foram alterados em v0.2.0-dev; consulte [PROGRESSION.md](PROGRESSION.md) para o estado atual da v0.3.1-dev e o [checklist](TEST_CHECKLIST.md) para os testes e pendências.

# Auditoria de dificuldade e progressão — v0.1.1-dev

Avaliação em 21/10/2026. **Veredito: há desafio e estratégias reais, mas a curva ainda é irregular. O modo Normal permite sobrelevel acentuado; o Hardcore concentra riscos altos no início e na Liga. Não considero o balanceamento finalizado.**

Esta é uma auditoria dos dados e do código, com um modelo aritmético de experiência. Não houve campanha completa, estudo de jogadores ou medição de taxa de vitória. Os testes anteriores da E4 verificaram criação das equipes e avanço de turnos com uma equipe artificial; não mediram dificuldade com um jogador em progressão normal.

## 1. Curva dos chefes e limites

| Etapa | Níveis inimigos | Teto Hardcore antes de vencer |
|---|---:|---:|
| Brock | 12–14 | 14 |
| Misty | 19–21 | 21 |
| Surge | 22–24 | 24 |
| Erika | 27–29 | 29 |
| Koga | 41–43 | 43 |
| Sabrina | 41–43 | 43 |
| Blaine | 45–47 | 47 |
| Giovanni | 48–50 | 50 |
| Lorelei | 52–54 | 60 |
| Bruno | 54–56 | 60 |
| Agatha | 56–58 | 60 |
| Lance | 58–60 | 60 |
| Campeão, primeira Liga | 57–63 | 60 |
| Revanches da E4 | 64–66 / 66–68 / 68–70 / 70–72 | 100 após zerar |
| Campeão, revanche | 72–75 | 100 após zerar |

O salto 29 → 43 e o patamar 43 → 43 são herdados dos níveis da base. Não demonstram sozinhos que falta conteúdo entre os ginásios, mas o limite por insígnia libera 14 níveis de uma vez e depois não sobe ao vencer o próximo líder. O limite procura a primeira insígnia ausente na ordem fixa, não o ginásio escolhido: enfrentar líderes fora de ordem não recalibra o teto.

A Liga mantém teto 60 mesmo contra o Campeão no 63. Isso pode ser uma escolha de desafio, mas não está alinhado ao maior nível do percurso inteiro. Também significa que a experiência das quatro lutas anteriores não permite subir acima de 60. Recomendo decidir explicitamente entre teto 63 para toda a Liga, tetos por luta ou um desafio deliberado em desvantagem de nível.

## 2. Brock e formação da equipe

Sem glitches, sem trocas externas e sem mudar os mapas, há quatro áreas de captura terrestre antes de Brock: Route 1, Route 22, Route 2 e Viridian Forest. No Hardcore, isso permite no máximo **quatro capturas + o inicial = cinco integrantes**, antes de considerar perdas ou capturas fracassadas. Não há um sexto Pokémon garantido neste trecho. Brock sempre usa seis.

Encontros atuais de FireRed, antes da cláusula de duplicatas:

| Área | Distribuição |
|---|---|
| Route 1 | Pidgey 50%, Rattata 50% |
| Route 22 | Rattata 45%, Mankey 45%, Spearow 10% |
| Route 2 | Rattata 45%, Pidgey 45%, Caterpie 5%, Weedle 5% |
| Viridian Forest | Caterpie 40%, Weedle 40%, Metapod 5%, Kakuna 10%, Pikachu 5% |

Duplicatas mudam a distribuição do primeiro encontro elegível; as porcentagens acima não são probabilidades finais de uma run Nuzlocke. A floresta não garante Pikachu e Route 22 não garante Mankey.

Brock abre com Onix e Geodude e também usa Rhyhorn, Omanyte, Kabuto e Nosepass. O tema Pedra pesa contra boa parte das aves/insetos disponíveis. Bulbasaur tem uma combinação especialmente favorável contra as várias fraquezas a Planta; Charmander depende mais de parceiros e planejamento. O teto 14 bloqueia a evolução dos três iniciais, que ocorre no 16. Isso aumenta o peso da escolha inicial e das primeiras capturas; não prova que alguma opção seja impossível.

**Prioridade:** garantir uma sexta opção por presente opcional em local existente (compatível com a cláusula de presentes livres) e revisar diversidade/suporte dos encontros iniciais. Preservar seis Pokémon para Brock, mas testar as três escolhas iniciais com equipes legalmente obtidas, sem exigir uma captura rara específica.

## 3. Experiência no Normal

A fórmula atual concede experiência integral a cada participante e metade a cada reserva apta. Com dois participantes e quatro reservas, uma derrota inimiga distribui aproximadamente **4× a experiência-base total pela equipe**, antes de arredondamentos, em comparação com o orçamento total repartido da geração III. Isso não significa quatro vezes a experiência para cada Pokémon. Trocar mais integrantes para participar pode ampliar esse total.

O catálogo de 636 equipes tem 1.773 integrantes contra 1.647 nos mesmos registros da base: +126, cerca de 7,7%. Há 113 registros com quantidade alterada. Esses totais incluem variantes e revanches; não são a quantidade enfrentada em uma única campanha.

### Modelo ilustrativo, não previsão de nível típico

Hipóteses: inicial do grupo de crescimento Medium Slow no nível 5, dois participantes fixos presentes contra todos os oponentes, todas as vitórias sem desmaios, Exp. Share ligado, sem selvagens, sem Lucky Egg/trocas/Pokérus, sem limite de nível e sem rotação dos titulares. Foi selecionada uma única variante do rival. O roteiro inclui treinadores opcionais e exploração ampla, como a S.S. Anne; não representa o caminho mínimo nem uma ordem obrigatória. Não se afirmou que esses combates são vencíveis sem treinamento prévio com qualquer inicial.

| Antes do chefe | Nível possível do titular, Exp. Share ligado | Mesmos times e dois participantes, compartilhamento global desligado | Maior nível do chefe |
|---|---:|---:|---:|
| Brock | 14 | 11 | 14 |
| Misty | 25 | 20 | 21 |
| Surge | 33 | 27 | 24 |
| Erika | 40 | 32 | 29 |

O modelo soma 1.649 EXP por titular no trecho pré-Brock selecionado, 9.016 entre Brock e Misty, 16.797 entre Misty e Surge e 24.202 entre Surge e Erika, acrescentando as vitórias nos líderes anteriores. Os números variam com o caminho e com quem participa. A parte desligada compara as mesmas equipes modificadas, não uma campanha vanilla.

**Conclusão sustentada:** há capacidade suficiente de ganho de experiência para ultrapassar bastante os chefes no Normal explorando e mantendo uma dupla fixa. Não é necessário afirmar que todo jogador chegará nesses níveis. Desligar o compartilhamento reduz o problema, mas não substitui calibrar a progressão. O teto no Normal, já planejado, é uma solução mais direta sem retirar a comodidade do Exp. Share.

## 4. Aumento de poder na E4

As oito equipes da E4 têm IVs 31, naturezas e habilidades fixas e EVs distribuídos por função. Os outros treinadores regulares, incluindo o Campeão, ainda não recebem esse treinamento específico. A base de encounters e as ferramentas de treinamento do jogador não acompanharam a mesma mudança: a distribuição completa dos 386 e a interface de EVs ainda não existem.

Exemplo para isolar somente o treinamento: Kyogre no nível 54 com IVs 26, sem EVs e natureza neutra teria 186 HP e 181 de Ataque Especial. O set atual de Bruno, IVs 31, 252 EVs em HP e Ataque Especial e Modest, tem 222 HP e 238 de Ataque Especial: cerca de +19% e +31%, respectivamente. Não é uma comparação com o Pokémon médio real do jogador, que acumula EVs e pode ter naturezas/IVs diversos.

Bruno usa Kyogre e Groudon; Lance usa Kyogre e Rayquaza. O aumento de força também envolve espécies e suporte, não apenas níveis. A média da soma dos atributos base é 503 em Lorelei, 548 em Bruno, 472 em Agatha e 547 em Lance. Essa média não mede sozinha dificuldade, mas mostra que a sequência não é uma subida uniforme de poder bruto. Status, tipos, clima e escolhas da IA podem mudar a ordem de dificuldade.

O Campeão tem níveis maiores, mas não o mesmo treinamento específico nem a mesma concentração de lendários. Há risco de Bruno/Lance serem desafios superiores ao confronto final; isso precisa ser medido com equipes de jogador realistas, não inferido apenas do nível.

**Prioridade:** disponibilizar preparação acessível (EVs, informações e itens renováveis), calibrar a E4 com equipes possíveis naquele ponto e alinhar o Campeão ao padrão de treinamento dos quatro anteriores. Manter as referências campeãs aprovadas; avaliar spreads, níveis, itens e consistência da IA antes de remover espécies.

## 5. Regras e duração das lutas

- No Hardcore, o jogador não pode usar cura da mochila em batalha. A IA não tem o mesmo bloqueio e a E4 mantém duas Full Restores por membro. É uma assimetria concreta em relação à proposta de bloquear itens dos dois lados em lutas importantes.
- Entre lutas, cura por itens continua permitida; isso evita interpretar a Liga como cinco lutas sem qualquer recuperação. Pokémon perdidos não podem ser revividos. Recursos para restaurar PP continuam relevantes.
- No catálogo, 1.262 de 1.773 Pokémon têm Protect e outros sete têm Detect: aproximadamente **71,6%** com um desses golpes. O número conta variantes e revanches, não encontros únicos. O uso efetivo depende da IA. Ainda assim, a distribuição sugere risco de duração e consumo de PP excessivos, mesmo em treinadores comuns.
- EVs continuam sendo ganhos em batalha, inclusive na chamada que ocorre após limitar EXP. O teto impede níveis, mas não representa treinamento de EVs controlado por interface.
- Na Diglett's Cave há Dugtrio nos níveis 29 e 31; se encontrado antes de Surge, fica acima do teto 24 e indisponível no Hardcore. Uma captura dessas consome a oportunidade da área e não resolve o ginásio imediatamente.
- Após a primeira Liga, o teto passa a 100. As revanches são os mesmos seis sets com níveis maiores, sem escala dinâmica. Permanecem desafiadoras perto de seus níveis, mas podem ser superadas por sobrelevel e não adicionam estratégias novas.

## 6. Ordem recomendada de ajustes

1. **Coerência das regras:** decidir o teto da Liga/Campeão e aplicar a restrição de mochila dos dois lados nas lutas importantes.
2. **Início viável com os três iniciais:** sexta opção pré-Brock, distribuição de capturas e teste de duplas a nível 14 sem depender de um encontro raro.
3. **Normal sem sobrelevel acidental:** implementar o limite já planejado e revisar a curva de níveis/EXP por marcos de progresso.
4. **Treinamento acessível antes da Liga:** interface de EVs e recursos/informações para preparar respostas à E4; equilibrar acesso às espécies e itens usados nos sets.
5. **Campeão e ritmo:** aproximar seu treinamento ao da E4 e reduzir Protect nos treinadores comuns quando não houver função clara.
6. **Playtest de campanha:** comparar caminhos curtos/exploração, os três iniciais, Normal/Hardcore, seis capturas legais por etapa, tempo de treino, perdas, consumo de itens/PP e lutas em que a IA prejudica seu próprio clima.

Metas observáveis propostas: chegar perto do teto com batalhas de percurso sem farm prolongado; ter respostas acessíveis para cada chefe; justificar cada salto de poder; reduzir turnos sem decisão relevante; nenhuma escolha inicial exigir uma captura rara obrigatória. Não há nota numérica de dificuldade ou taxa de vitória validada neste estudo.

## Reprodução e evidência

Execute `python scripts/audit_progression.py`. O relatório de dados sai em `output/analysis/progression.json`, incluindo IDs de treinadores e mapas do roteiro ilustrativo. O script usa `data/trainer_balance.json`, a comparação com os dados da revisão-base, encounters e atributos da engine. As regras foram lidas em `game/src/nuzlocke.c`, a EXP em `game/src/battle_script_commands.c` e o uso de itens da IA em `game/src/battle_ai_switch_items.c`.

Esta rodada produziu apenas a análise e seu script; a ROM e as releases não receberam alterações de balanceamento.


Atualização v0.4.0-dev: desafios e Torre recebem IVs 31 e EVs treinados. As equipes temáticas descritas aqui valem com randomizer desligado; ligado, espécies e golpes mudam, e o treinamento é recalculado por função. [Regras](SETTINGS.md).
