# FireRed — projeto de melhorias

## Requisitos confirmados

- Base: Pokémon FireRed para GBA.
- Ao criar o save: escolher Normal ou Hardcore Nuzlocke, com revisão antes da confirmação; opção fixa naquela partida. As regras implementadas e suas variantes estão em HARDCORE.md. Saves antigos permanecem em Normal.
- Preservar a história e os mapas de Kanto.
- Todas as batalhas contra treinadores devem ser em dupla, inclusive rival, líderes e Liga, com exceção da primeira luta no laboratório, que permanece simples.
- Revisar as equipes de todos os treinadores para oferecer sinergia em duplas e melhorar as decisões dos NPCs em batalha, incluindo golpes, alvos e suporte ao parceiro.
- Aumentar a dificuldade.
- Permitir evoluções sem trocas.
- Aumentar a frequência de Pokémon shiny.
- Separar golpes físicos e especiais por golpe, em vez de determinar a categoria pelo tipo; manter golpes de status em categoria própria.
- Compartilhar experiência com toda a equipe (Exp. Share global).
- Exp. Share global sem item equipado: participantes recebem a experiência normal; demais Pokémon aptos recebem 50%; incluir opção de ativar/desativar.
- Todos os 386 Pokémon das gerações I–III devem ser obtíveis em um único save, incluindo todos os lendários e míticos, sem depender de trocas ou eventos externos.
- Distribuir espécies pelos mapas existentes conforme raridade, poder e progressão; evitar acesso precoce a Pokémon que desequilibrem o início.
- Todos os líderes de ginásio devem usar seis Pokémon, com times fortes e preparados para duplas desde o primeiro ginásio.
- TMs reutilizáveis.
- Usar HMs fora de batalha sem ocupar espaços de golpes, preservando bloqueios de progresso por insígnias e eventos.
- Revanche contra líderes de ginásio.
- Nova oportunidade de capturar lendários derrotados.
- Pokédex Nacional disponível cedo, sem antecipar o progresso da história.
- Limite de nível por ginásio.
- Atualizar atributos base dos Pokémon e dados dos golpes para uma geração recente, cuja referência exata ainda deve ser definida.
- Incluir todos os novos golpes até a referência escolhida; revisar separadamente compatibilidade da engine, efeitos, animações, descrições e distribuição de aprendizado entre as 386 espécies.
- Adicionar tipo Fada, incluindo tabela de efetividade e atualização dos tipos dos Pokémon e golpes correspondentes.
- O elenco continua limitado às 386 espécies; formas novas, habilidades modernas, Mega Evolução, Z-Moves, Dynamax e Terastalização não estão automaticamente incluídos.
- Excluir mecânicas especiais como Mega Evolução, Z-Moves, Dynamax/Gigantamax e Terastalização, assim como golpes que dependam delas. Preservar as mecânicas de batalha comuns e as melhorias explicitamente aprovadas.
- Elite Four e revanches dos líderes após zerar devem usar times competitivos voltados para duplas e coerentes com o tipo de especialidade de cada treinador.
- As revanches dos líderes continuam com seis Pokémon, conforme a regra já aprovada para todos os líderes.
- Exp. Share global liberado assim que o jogador entregar o Parcel ao Professor Oak.
- Após vencer a Liga e entrar no Hall da Fama pela primeira vez, disponibilizar compra de todos os itens em Celadon City; interpretação inicial: itens utilizáveis e equipáveis, mantendo itens-chave de história vinculados aos eventos. Confirmar exceções especiais ao montar o catálogo.
- Lembrete de golpes gratuito e acessível cedo, incluindo golpes esquecidos.
- Permitir ajustar o efeito da natureza e trocar entre habilidades disponíveis para a espécie.
- Treinamento de EVs por interface, evitando farm de batalhas; permitir distribuir e remover EVs dentro dos limites da referência escolhida.
- Ajuste de IVs disponível no pós-jogo.
- Exibir IVs, EVs, efeito da natureza, habilidade e categoria dos golpes de forma clara.
- Garantir acesso renovável a itens de batalha.
- Exibir informações sobre espécies encontradas por área.
- Exibir o limite de nível atual e sua relação com a próxima insígnia no menu.
- Desativar o uso de itens da mochila durante batalhas importantes para jogador e adversário; permitir itens equipados e preservar ações necessárias fora dessas lutas, incluindo captura de selvagens.

## Proposta inicial — valores ainda ajustáveis

- Shiny: probabilidade de 1/256 por Pokémon gerado; aplicar a mesma regra à geração e à identificação de shinies, incluindo iniciais e presentes.
- Dificuldade: times preparados para duplas, com golpes complementares, cobertura e itens progressivos; calibrar níveis durante os testes.
- Cada treinador deve ter pelo menos dois Pokémon.
- Evoluções por troca simples: evolução por nível, inicialmente nível 37.
- Evoluções por troca com item: usar diretamente o item correspondente; implementar suporte se necessário e garantir acesso ao item na campanha.
- Manter encontros selvagens no formato original.
- Exp. Share global: liberação na entrega do Parcel, regra de distribuição e opção de ativar/desativar confirmadas.
- Não acrescentar outras melhorias sem definir o escopo com o usuário.

## Distribuição implementada — v0.3.0-dev

As 386 espécies têm captura direta, sem trocas, reprodução ou escolha exclusiva. Parâmetros e cobertura em data/encounter_distribution.json; roteiro por espécie em SPECIES_LOCATIONS.md. Lendários/míticos têm um slot de 1% cada em áreas avançadas e trava de vitória na Liga; encontros fixos antigos permanecem. Celio passa a exigir vitória na Liga para iniciar a Ruby, preservando a progressão apesar da Nacional cedo. A coleção completa é possível no Normal; cotas Hardcore continuam limitando capturas. As diretrizes abaixo orientam ajustes posteriores.

## Diretrizes de distribuição

- Início: variedade de tipos e espécies básicas para formar uma equipe de seis antes do primeiro ginásio; avaliar habilidades e golpes além dos atributos base.
- Meio da campanha: ampliar opções de composição e disponibilizar espécies mais fortes gradualmente.
- Fim da campanha e pós-jogo: reservar os Pokémon de maior impacto e a maior parte dos lendários e míticos para marcos avançados de progresso.
- Lendários e míticos: propor encontros fixos em locais existentes, condicionados a insígnias ou eventos já concluídos; raridade baixa sozinha não impede captura precoce.
- Auditar as 386 espécies individualmente por encontro, presente, fóssil, reprodução ou evolução; resolver escolhas exclusivas e acesso a iniciais, itens e áreas.

## Interface de treinamento e lojas — implementação parcial

- Implementado: cientista nos 19 Centros. EVs gratuitos desde o início nos dois modos; natureza/IVs desde o início no Hardcore e após a Liga no Normal. Ajustes não curam nem revivem; EVs ganhos em batalha continuam ativos.
- Implementado: EVs até 252 por atributo/510 total e IVs até 31, com edição, cancelamento e confirmação. Troca de habilidade permanece pendente.
- Definir acesso e custos de ajuste de natureza e habilidade e quais habilidades são suportadas pela referência da engine.
- Propor o uso da loja de departamentos existente em Celadon, organizado por categorias, sem alterar a geometria dos mapas.
- Catalogar todos os itens da base escolhida, incluindo itens novos; definir preços, itens especiais, tratamento de itens-chave e capacidade da mochila. A compra de Master Ball e Rare Candy não foi excluída do pedido.
- Definir quais batalhas contam como importantes para a restrição da mochila, incluindo no mínimo líderes, Elite Four, Campeão e suas revanches.

## Rare Candy e curas — regra Hardcore implementada

- Rare Candy por ₽1 em lojas comuns somente no Hardcore; revenda por ₽0.
- Cada um dos 19 Centros admite três curas pela enfermeira por partida, com contador individual salvo e aviso. Normal mantém preço/estoque e curas sem essa restrição.
- Camas e cura por PC não são limitadas por esse contador. Não há autosave ou proteção contra recarga de um estado anterior.
- Curva implementada: ginásios 18/28/36/44/52/60/68/76; Liga 80/83/86/89/92; revanche inteira no 100. Teto de EXP nos dois modos. Encontros selvagens recalibrados em v0.3.0-dev: 386 capturas diretas no Normal, com 21 lendários/míticos aleatórios de pós-jogo e preservação da cota Hardcore.

## Referência moderna e regras pendentes

- Propor geração IX como referência para atributos base, tipos, tabela de efetividade, dados de golpes e learnsets; confirmar antes da implementação.
- Interpretar inicialmente “status dos Pokémon” como atributos base; mudanças em condições de status e habilidades precisam de escopo próprio.
- “Todos os novos golpes” significa disponibilizar os golpes comuns até a referência escolhida no sistema, com efeitos funcionais; não permitir que todas as espécies aprendam todos os golpes. Excluir golpes dependentes das mecânicas especiais rejeitadas. Golpes ligados a espécies ou formas ausentes ainda exigem avaliação de compatibilidade e distribuição.
- Definir o teto de cada ginásio após montar os times; propor impedir ganho de níveis acima do teto atual sem bloquear evolução e liberar o teto seguinte após a insígnia.
- Definir limites para Liga e pós-jogo e tratamento de capturas acima do teto.
- Definir gatilho de reaparecimento de lendários derrotados e regras de revanche dos líderes.

## Elite Four: adaptação de campeões implementada

O usuário aprovou completar as equipes campeãs com substitutos coerentes entre as 386 espécies. Foram implementadas as quatro adaptações de E4_CHAMPIONS.md, na Liga e nas revanches, com seis integrantes, naturezas, habilidades, EVs e IVs definidos. As composições são mistas: preservam os integrantes disponíveis dos campeões e reforçam o tema nas substituições. Não exigem monotipo nem reproduzem mecânicas excluídas. As diretrizes abaixo continuam orientando a revisão após a modernização da engine.

## Diretrizes para Elite Four e revanches pós-jogo

- Montar equipes com funções complementares: controle de velocidade, suporte, pressão ofensiva, cobertura e respostas às fraquezas do tipo de especialidade.
- Escolher golpes, itens e atributos de treinamento de forma coerente com a estratégia de duplas e as mecânicas disponíveis no projeto.
- Evitar golpes que atinjam aliados sem proteção, imunidade ou outra interação planejada; verificar as decisões da IA durante os testes.
- Restringir as opções às 386 espécies e aos golpes e mecânicas aprovados.
- Ainda definir se a especialidade exige que todos os integrantes compartilhem o tipo ou permite exceções temáticas; a exigência confirmada é respeitar o foco elemental.
- Ainda definir uso de lendários pelos adversários, níveis, limites da Liga e composição final de cada equipe.

## Regra do início — definida

A primeira luta contra o rival permanece simples, nas três variantes de inicial. Não dar um segundo Pokémon ao jogador ou rival para essa luta. Nas demais lutas, se o jogador tiver apenas um Pokémon apto, manter dois adversários e deixar o segundo espaço do jogador vazio, sem duplicação de Pokémon ou bloqueio de progresso.

## Revisão de equipes e IA — primeira passagem implementada

- 636 equipes regulares revisadas, incluindo rivais posteriores, líderes, Elite Four, Campeão e revanches existentes; três variantes do laboratório preservadas.
- 60 elencos internos da Torre revisados. Seus modos de batalha agora usam duplas, incluindo um segundo Pokémon para adversários do modo Knockout. Dados externos de e-Reader não foram revisados.
- Golpes de suporte na abertura, cobertura, clima em equipes compatíveis e equipamento progressivo. Líderes e Elite Four usam seis Pokémon coerentes com seu tipo.
- Sem golpes que atinjam aliados nos elencos gerados, sem OHKO ou evasão acumulada. Evitar clima em equipes que possam ter Cloud Nine/Air Lock.
- IA avalia os dois alvos adversários com os scripts existentes e considera parceiro apto, Helping Hand, telas, controle de velocidade, clima e risco de dano aliado.
- Esta passagem usa categorias por golpe e dados numéricos modernos para os golpes implementados; efeitos e tipos ainda estão em migração. Os desafios recebem IVs 31/EVs treinados; novas revanches de líderes continuam pendentes. Estado atual: [MODERNIZATION.md](MODERNIZATION.md) e [AI_REVIEW.md](AI_REVIEW.md).
- Auditoria por treinador: TRAINER_REVIEW.md; dados reproduzíveis: data/trainer_balance.json; gerador: scripts/balance_trainers.py.
- Verificações: auditoria estrutural dos elencos e testes nativos das regras de pontuação. Fixtures no mGBA alcançaram a seleção de ação na luta inicial, em dupla normal e em um contra dois. Não foi realizado um teste completo da campanha ou da Torre.

## Direção técnica

### Alphas e raids aleatórias — escopo aprovado

Inclusão de Alphas e raids aleatórias aprovada pelo usuário. Os detalhes abaixo são os parâmetros iniciais propostos, ainda ajustáveis e sem implementação. Adaptar o conceito de Alpha como encontro especial; não implica habilitar Mega Evolução, Dynamax ou as demais mecânicas excluídas.

- Alphas raros das espécies disponíveis na área, identificados por marca e apresentação visual própria.
- Raids opcionais em pontos dos mapas existentes; espécies e dificuldade limitadas por insígnias e acesso à área.
- Propor desbloqueio após a segunda insígnia e eventos de dificuldade superior no pós-jogo.
- Propor renovação por passos ou batalhas, sem depender de relógio em tempo real; persistir o evento no save para evitar rerrolagem a cada entrada no mapa.
- Formato proposto: dois Pokémon do jogador contra um chefe Alpha; exige implementação e teste próprios do fluxo assimétrico, seleção de alvos, IA, interface e captura. O suporte a duplas comum não confirma esse formato.
- Bônus de chefe, como HP aumentado, limitados ao encontro; após captura, usar atributos normais da espécie e manter a marca Alpha, evitando desequilíbrio permanente.
- Avaliar captura em fase posterior à vitória, garantindo que a derrota do chefe não finalize o evento antes da oportunidade de captura.
- Recompensas renováveis e espécies fortes condicionadas à progressão; não criar rota precoce para lendários.
- Tamanho de sprite maior é opcional e depende de validação visual; não está prometido suporte a escala arbitrária.
- Sem promessa de multiplayer; proposta inicial inteiramente solo.

A base escolhida e configurada é pret/pokefirered, na revisão fixada abaixo. Este documento registra o escopo aprovado; o estado detalhado e atualizado de cada funcionalidade está no README.md.

Fonte: https://github.com/pret/pokefirered

## Verificação necessária antes de entregar uma versão jogável

- Primeira batalha do rival, batalhas por contato visual e por interação.
- Treinadores que originalmente possuem um único Pokémon e duplas já existentes.
- Rival, ginásios, Liga, revanche, Torre de Treinadores e demais lutas especiais.
- Comportamento quando o jogador tem apenas um Pokémon apto, inclusive após desmaios ou depósito no PC; evitar bloqueios de progresso.
- Evoluções e disponibilidade dos itens necessários, incluindo restrições da Pokédex Nacional.
- Identificação e persistência de shinies em batalha, resumo, evolução e save.
- Categoria físico/especial/status consistente no cálculo de dano, na IA, nos efeitos de batalha e na informação exibida ao jogador.
- Experiência global em batalhas simples e duplas, incluindo múltiplos participantes, Pokémon desmaiados, ovos, equipe no nível máximo e opção de ativar/desativar; recalibrar a dificuldade considerando a progressão da equipe inteira.
- Cobertura das 386 espécies em um único save, sem escolhas que tornem espécies permanentemente inacessíveis; garantir lendários e míticos sem eventos externos.
- Equipes de seis de cada líder e acesso a opções suficientes de captura e treinamento antes do primeiro ginásio.
- Bloqueios de progresso dos encontros poderosos e preservação da história e da geometria dos mapas.
- TMs preservadas após uso e ações de HMs respeitando os requisitos de progresso sem exigir golpe aprendido.
- Limites de nível em experiência compartilhada, Rare Candy, creche e evolução; tratamento definido para Pokémon capturados acima do teto.
- Revanche de líderes, reaparecimento de lendários e Pokédex Nacional sem avanço acidental de flags de história.
- Consistência da referência moderna em atributos, tipos, efetividades e golpes, incluindo IA de duplas e interações de efeitos.
- Coerência temática e funcionamento em duplas das equipes da Elite Four e das revanches pós-jogo; verificar golpes, itens, velocidade, cobertura e decisões da IA.
- Liberação do Exp. Share pela entrega do Parcel, sem acesso antecipado acidental e com estado persistente no save.
- Interface de EVs respeitando limites individuais e totais, recalculando atributos e persistindo mudanças no save; compatibilidade com natureza, IVs e ganhos de batalha.
- Catálogo de Celadon disponível após a primeira vitória na Liga, com compras renováveis e sem quebra de eventos de história por itens-chave.
- Ajustes de natureza e habilidade, lembrete gratuito, informações de atributos e espécies por área e exibição do limite de nível.
- Restrição de itens da mochila aplicada aos dois lados nas batalhas importantes, preservando itens equipados e captura de selvagens.
- Compilação e teste em emulador.

## Estado atual — v0.4.0-dev

- 386 espécies com captura direta, níveis e raridades por etapa, incluindo os 21 lendários/míticos em encontros aleatórios pós-Liga. Cobertura em [SPECIES_LOCATIONS.md](SPECIES_LOCATIONS.md) e [ENCOUNTERS.md](ENCOUNTERS.md).
- 636 equipes regulares e 60 elencos internos da Torre revisados para duplas; laboratório preservado em simples. E4 com adaptações de campeões e Campeão com treinamento definido.
- Ginásios até 18/28/36/44/52/60/68/76; primeira Liga até 80/83/86/89/92; revanche inteira no 100. Teto de EXP nos dois modos.
- Normal/Hardcore persistidos no save, EXP global após o Parcel, TMs reutilizáveis, shiny 1/256, Nacional cedo e quatro evoluções por troca simples no nível 37.
- Cientista nos 19 Centros: EVs gratuitos desde o início nos dois modos; natureza/IVs sempre no Hardcore e após a Liga no Normal. Rare Candy por ₽1 e três curas por enfermeira/Centro exclusivamente no Hardcore.
- Travamento de saída das lojas corrigido. [23 catálogos × 2 modos aprovados](SHOP_TEST_RESULTS.md).
- Recompilação do patch sobre fontes limpas reproduziu a ROM entregue. Base e compilador fixados nos commits documentados no README; formato do save preservado.

Ainda pendentes: concluir a engine moderna (golpes e efeitos restantes), HMs sem ocupar golpes, novas revanches de líderes, catálogo completo pós-Liga em Celadon, troca de habilidade, Alphas/raids e outras melhorias listadas no [README](README.md). Os encontros aleatórios já oferecem captura alternativa dos lendários; reset dos eventos fixos é uma melhoria separada.

Os testes de dados, menus, batalhas e save não substituem a validação da campanha inteira, Torre completa ou VBA Link. As etapas anteriores estão no [CHANGELOG](CHANGELOG.md).

Correções da v0.3.1-dev: IVs compactados e status dos errantes, teto/previsão/cobrança das creches e cota preservada ao enfrentar o Marowak fantasma. [Captura validada](CAPTURE_TEST_RESULTS.md) e [checklist atualizado](TEST_CHECKLIST.md); saves mantêm seu layout.

Validação adicional: [114 checkpoints assistidos de campanha/pós-jogo](CAMPAIGN_TEST_RESULTS.md), incluindo os quatro cursos da Torre e saves finais. A caminhada, ordem legítima de acesso, recursos e dificuldade de uma partida normal/Hardcore permanecem sem homologação.


Novidades v0.4.0-dev: [idioma e randomizer](SETTINGS.md), interface visual do treinamento e IVs perfeitos/EVs treinados nos desafios, incluindo a Torre. A referência mecânica continua geração III. [Testes e limites](FEATURE_TEST_RESULTS.md).
