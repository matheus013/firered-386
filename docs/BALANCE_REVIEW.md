# Balanceamento de Normal e Hardcore — v0.4.0-dev

Auditoria de 22/10/2026, randomizer desligado. ROM SHA-256 `881869fbaa37abe13becbb071759ff1225984910fdb410d11818e88b56a0990d`. [Dados reproduzíveis](../data/balance_audit_v0.4.0.json), gerados por `python scripts/audit_balance.py`.

**Parecer: o Normal está difícil e irregular na preparação; o Hardcore facilita a preparação, mas é muito mais punitivo nas batalhas e capturas. Ainda há desequilíbrios que impedem considerar a curva calibrada.** A existência de testes funcionais aprovados não demonstra chance de vitória de um jogador.

Esta avaliação inspeciona fontes e dados, conta recursos/encontros e calcula modelos explícitos de EXP e atributos. Não conclui uma campanha e não mede taxa de vitória. Os testes nativos anteriores conferiram o treino efetivo dos desafios e as regras, mas suas equipes artificiais não servem como referência de balanceamento. O relatório assistido antigo continua separado.

Os 600 limiares de EXP usados pelos seis grupos de crescimento, níveis 1–100, foram comparados com as tabelas do próprio `.gba` compilado; todos coincidiram. Para repetir essa conferência no WSL: `python3 scripts/audit_balance.py --verify-rom-tables`. Ela valida os números do modelo, sem transformar o modelo em uma campanha jogada.

## Comparação dos modos

| Aspecto | Normal | Hardcore |
|---|---|---|
| Adversários | Mesmos níveis, elencos, IVs 31 e EVs dos desafios | Mesmos níveis, elencos, IVs 31 e EVs dos desafios |
| Preparação antes da Liga | Natureza/IVs dependem da captura; EVs exigem batalhas/vitaminas. Cientista bloqueado | Natureza, EVs e IVs ajustáveis gratuitamente desde o início |
| Subir níveis | EXP global; Candy comum não vendida por ₽1 | Candy por ₽1 até o teto; quase elimina treinamento por EXP |
| Falhas e recuperação | Revives, itens em batalha e novas capturas permitem recuperar a equipe | Perda permanente, cota por área e mochila bloqueada em batalha |
| Cura fora de batalha | Centros ilimitados | Três atendimentos por Centro; PC e camas não entram no contador |
| Experiência resultante | Mais trabalho para preparar a mesma resposta estatística; erros são recuperáveis | Preparo rápido e previsível; decisões e perdas pesam muito mais |

Não é correto dizer que o Hardcore é mais fácil no conjunto. Seu treinamento é mais fácil; sobreviver à sequência é uma exigência diferente.

## 1. Problema mais urgente: EXP na primeira Liga

Os tetos sobem **80 → 83 → 86 → 89 → 92**, depois de cada membro vencido. A EXP da luta é recebida antes da flag que libera o teto seguinte. Portanto, chegar ao máximo da barra do teto 80 não permite guardar livremente a EXP da Lorelei para o 83.

Modelo: entrar no 80, vencer todos os seis adversários de cada um dos quatro membros, EXP global ligada, nenhum desmaio, sem Lucky Egg, bônus de troca, Candy ou outras fontes de EXP. Foram calculados os seis grupos de crescimento e dois pontos de entrada: EXP mínima do nível 80 e barra quase cheia do 80. O participante recebe EXP integral de todos os inimigos; a reserva recebe metade de todos. Não simula rotação intermediária, evoluções ou batalhas para atingir o nível de entrada, nem demonstra que essas vitórias aconteçam.

| Crescimento | Participante fixo antes do Campeão | Reserva fixa antes do Campeão |
|---|---:|---:|
| Fast | 84 | 82 |
| Medium Fast | 83 | 81–82 |
| Medium Slow | 83 | 81–82 |
| Slow | 83 | 81–82 |
| Erratic | 86 | 83 |
| Fluctuating | 82 | 81 |

O Campeão usa **87–92**. Para os grupos mais comuns, o modelo já favorável deixa o titular no 83 e a reserva no 81–82, com diferença relevante de níveis no confronto final. A EXP global melhora o treinamento anterior, mas não produz os nove a doze níveis esperados durante quatro lutas. O Normal pode compensar com mochila, itens e Candy encontrados ou farmados por Pickup; não é uma demonstração de impossibilidade. É um descompasso mensurável da curva.

O orçamento bruto dessas quatro lutas é de 18.103 / 20.223 / 17.896 / 20.193 EXP por participante fixo, antes dos cortes pelo teto. Uma reserva fixa recebe aproximadamente metade. No modelo, a EXP é processada inimigo a inimigo e limitada ao teto vigente, com os arredondamentos da fórmula atual.

No Hardcore, Candy comprado antes da entrada permite acompanhar cada novo teto entre salas, desde que a equipe sobreviva. **A regra atual penaliza a preparação do Normal de uma maneira que o Hardcore consegue contornar.**

Recomendação prioritária: planejar a Liga como um bloco com teto único de entrada e níveis próximos. Uma faixa a testar seria 88/89/90/91/92, com teto 92 e ajuste correspondente de Victory Road e EXP anterior. Isso preserva o Campeão 92, mas é uma proposta, ainda sem calibração de batalhas. Outra opção é manter os tetos atuais e oferecer treinamento entre membros nos dois modos; depender de dezenas de Candy encontrados ou de um farm longo de Pickup é um custo de preparação relevante para seis Pokémon.

## 2. Treinamento completo cedo pesa muito mais no Normal

Os desafios já têm seis IVs 31 e EVs completos. O Normal só libera o cientista após a Liga; o Hardcore consegue igualar esses atributos desde o começo. É uma consequência das regras aprovadas, com efeito importante na acessibilidade.

Exemplos isolando o treino: mesma espécie e nível, referência com IV 15, EV 0 e natureza neutra, comparada ao adversário efetivamente configurado. São atributos de ficha, sem bônus de insígnias, habilidades, itens ou modificadores de batalha; não representam a média de uma equipe real, que ganha EVs durante a campanha.

| Pokémon do chefe | Atributo | Referência | Adversário |
|---|---|---:|---:|
| Onix 16, Brock | HP / Ataque | 39 / 21 | 52 / 37 |
| Nosepass 18, Brock | HP / Ataque | 41 / 23 | 55 / 41 |
| Starmie 28, Misty | Velocidade / At. Esp. | 73 / 65 | 104 / 87 |
| Alakazam 60, Sabrina | Velocidade / At. Esp. | 158 / 176 | 225 / 223 |

EVs completos, natureza e IVs aumentam bastante o peso das primeiras lutas. No Normal, vencer pode exigir capturar vários candidatos e treinar EVs manualmente, contrariando o objetivo de evitar grandes farms. Itens em batalha e capturas ilimitadas atenuam isso, mas não removem o trabalho de preparação.

Mantendo o desbloqueio do cientista somente após a Liga no Normal, recomendo EVs adversários progressivos nesse modo, preservando os IVs 31: por exemplo, começar com 64 nos dois atributos principais e subir gradualmente até 252. Todos continuam treinados, com curva crescente. A distribuição final e o ritmo ainda precisam de testes; não foi aplicada alteração nesta auditoria. No Hardcore, os spreads completos já têm contrapartida disponível ao jogador.

## 3. Brock ainda é um gargalo do Hardcore

Antes de Brock, o trajeto padrão oferece inicial mais quatro cotas: Route 1, Route 22, Route 2 e Viridian Forest. Portanto, **até cinco Pokémon antes de perdas/falhas**, contra seis de Brock. O teto 18 agora permite a primeira evolução dos três iniciais e melhora a situação em relação à curva antiga.

A diversidade dos encontros também melhorou. Route 22 inclui Mankey 20%, Marill 10%, Wooper 10%, Phanpy 5%, Tyrogue 4% e Makuhita 4%; Route 2 e Forest incluem opções de Planta. Isso evita depender apenas do Mankey antigo, mas não garante um elenco equilibrado.

Um cálculo condicionado a todas as capturas elegíveis terem sucesso, repetindo duplicatas até surgir espécie nova na ordem Route 1 → Route 22 → Route 2 → Forest, dá presença de pelo menos um tipo Planta em 50,395%, Água em 32,577%, Lutador em 28,350% e Terra em 16,200% dos conjuntos. O inicial está excluído. Tipos se sobrepõem; os percentuais não podem ser somados, não incluem evolução posterior e **não são chances de vencer Brock**. Capturas falhas, mortes, golpes e o inicial mudam o resultado.

Recomendação: uma sexta opção garantida por presente opcional em local existente, mantendo Brock com seis; confrontar as três escolhas iniciais com elencos realmente obteníveis e sem exigir uma captura de 1%. A regra de duplicatas é por espécie, permitindo duas fases da mesma família; quantidade de capturas não garante variedade de funções.

## 4. Protect está excessivamente espalhado

**1.612 dos 1.773 Pokémon das 636 equipes regulares, ou 90,9%, carregam Protect/Detect.** Todos os seis integrantes de cada líder têm um desses golpes. A contagem inclui variantes e revanches; não equivale à frequência de uso pela IA nem ao total enfrentado numa campanha.

Protect é adequado a duplas, mas essa presença tende a alongar lutas comuns, consumir PP e repetir a mesma resposta tática. Nos chefes, há usos estratégicos; em quase todo adversário de rota, o custo de ritmo provavelmente supera o ganho de variedade.

Recomendação: manter onde clima, status, cura passiva, previsão de foco ou o parceiro justificam; diversificar os demais slots de treinadores comuns com cobertura ou suporte. A frequência-alvo deve vir de testes de duração, sem escolher uma porcentagem arbitrária como prova de qualidade.

## 5. Três curas por Centro não são um limite global de recuperação

Existem 19 contadores independentes, total de até 57 atendimentos, mas guardar/retirar Pokémon vivos no PC e camas continuam fora deles. O próprio teste funcional anterior já cobre cura pelo PC; perdidos continuam bloqueados. Assim, acabar os usos da enfermeira pode virar uma sequência de menus ou uma volta até outra fonte, em vez de uma decisão real sobre recursos.

Recomendação: definir se o objetivo é limitar a enfermeira ou limitar recuperação gratuita. Se for a segunda opção, integrar PC/camas/curandeiros a uma política de cura e testar prevenção de bloqueios da campanha. O limite global seria uma mudança de regra e exigiria novo desenho; não se deve fechar as alternativas e presumir que as 57 curas bastam sem percorrer a campanha.

## 6. Pontos que funcionam e riscos restantes

- EXP global e teto reduzem a necessidade de treinamento repetido e impedem sobrelevelling por EXP. No Normal, Pokémon recebidos acima do teto continuam utilizáveis, uma exceção que precisa continuar explicitada.
- Iniciais evoluídos antes de Brock, maior diversidade inicial, seis integrantes nos líderes e melhora de preparação no Hardcore dão uma base coerente para duplas.
- A partir de Surge, o BST médio sobe de 363,3 em Misty para 475,8; coincide com o nível 36 e evoluções finais dos iniciais, mas merece testar o salto de composição e velocidade. BST sozinho não mede força de uma equipe.
- Koga/Sabrina e Silph permitem ordens diferentes na história original, enquanto o teto segue as insígnias em ordem fixa. Entrar cedo na Silph/Sabrina pode levar a adversários acima do teto. A ordem recomendada precisa estar visível no jogo ou esses caminhos precisam de avaliação específica.
- E4 não sobe uniformemente em poder: Bruno e Lance têm lendários e maior BST médio que Agatha. Clima e tipos podem tornar Bruno mais perigoso que adversários posteriores. O treino completo do Campeão melhorou a comparação com a auditoria antiga; ainda falta medir estratégias reais.
- No pós-jogo, acesso ao cientista nos dois modos e revanche inteira no 100 deixam a comparação estatística mais uniforme. A dificuldade continua dependendo de itens, IA, elencos e perdas acumuladas; não foi medida taxa de vitória.
- Inimigos mantêm itens da mochila, enquanto o Hardcore bloqueia os do jogador. É uma assimetria intencional atualmente documentada, mas amplifica o desgaste; precisa entrar nos testes de economia e PP.

## Randomizer ligado

A avaliação principal acima pressupõe OFF. ON mantém níveis e faixas de BST, mas troca tipos, habilidades e golpes; desfaz as sinergias temáticas e campeãs. Mesmo BST não garante a mesma força: Shedinja e habilidades especiais mostram por que atributos somados não bastam. Uma semente pode aliviar um chefe e agravar outro, especialmente com poucas capturas no Hardcore.

Ainda não foi avaliada uma amostra de campanhas ou taxas de vitória por semente. Não há base para garantir equilíbrio de todas as sementes. A validação estrutural de 386 espécies e níveis preservados demonstra consistência da implementação, não equilíbrio competitivo.

## Prioridades e próximos testes

1. Corrigir o orçamento de níveis da primeira Liga no Normal.
2. Ajustar a progressão de EVs dos desafios do Normal, mantendo as regras de editor e IVs solicitadas, ou rediscutir o acesso à preparação.
3. Validar Brock com as três escolhas iniciais e capturas variadas; avaliar uma sexta opção garantida.
4. Definir o alcance da restrição de cura do Hardcore e reduzir o uso repetitivo de Protect em rotas.
5. Executar campanhas naturais nos dois modos, registrando capturas, derrotas/perdas, curas, dinheiro, PP e níveis por chefe. Repetir chefes com várias equipes/sementes e política de decisões declarada, sem interpretar derrotas da automação como impossibilidade do jogo.

Essas propostas não alteram a ROM da v0.4.0-dev. O diagnóstico identifica desequilíbrios concretos e hipóteses a testar; a viabilidade integral da campanha continua pendente.

## Complemento: combates com equipes disponíveis

[Testes dos 35 desafios e classificação por modo](CHALLENGE_BALANCE_TESTS.md) e [equipes testadas, golpes e origens](CHALLENGE_TEAMS_TESTED.md).

O teste posterior encontrou vitórias nas cinco salas da primeira Liga do Normal com a mesma equipe e recursos mantidos, usando Candy/PP Up previamente farmados por Pickup, TMs e itens. A tabela nativa inclui Rare Candy e PP Up renováveis (aproximadamente 0,5% de cada item por Pokémon elegível por batalha); portanto, não são recursos necessariamente finitos. Esse farm não foi executado. O descompasso de EXP sem Candy permanece, e a compensação exige preparação extensa. Não houve conclusão dos blocos de Liga Hardcore nas coortes testadas; isso não prova impossibilidade. A campanha natural continua pendente.
