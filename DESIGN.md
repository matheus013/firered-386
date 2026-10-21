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

## Proposta de distribuição — ainda ajustável

- Início: variedade de tipos e espécies básicas para formar uma equipe de seis antes do primeiro ginásio; avaliar habilidades e golpes além dos atributos base.
- Meio da campanha: ampliar opções de composição e disponibilizar espécies mais fortes gradualmente.
- Fim da campanha e pós-jogo: reservar os Pokémon de maior impacto e a maior parte dos lendários e míticos para marcos avançados de progresso.
- Lendários e míticos: propor encontros fixos em locais existentes, condicionados a insígnias ou eventos já concluídos; raridade baixa sozinha não impede captura precoce.
- Auditar as 386 espécies individualmente por encontro, presente, fóssil, reprodução ou evolução; resolver escolhas exclusivas e acesso a iniciais, itens e áreas.

## Interface de treinamento e lojas — detalhes pendentes

- A interface de EVs substitui a necessidade de farm; o momento de liberação ainda deve ser definido. Não restringir automaticamente ao pós-jogo após o pedido específico de treino via interface.
- Definir se EVs de batalha continuam ativos e como interagem com a distribuição manual, para evitar alterações involuntárias na equipe preparada.
- Definir acesso e custos de ajuste de natureza e habilidade e quais habilidades são suportadas pela referência da engine.
- Propor o uso da loja de departamentos existente em Celadon, organizado por categorias, sem alterar a geometria dos mapas.
- Catalogar todos os itens da base escolhida, incluindo itens novos; definir preços, itens especiais, tratamento de itens-chave e capacidade da mochila. A compra de Master Ball e Rare Candy não foi excluída do pedido.
- Definir quais batalhas contam como importantes para a restrição da mochila, incluindo no mínimo líderes, Elite Four, Campeão e suas revanches.

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
- Esta passagem usa os golpes, tipos e cálculo físico/especial da base atual de geração III. Não representa a revisão final para geração IX; EVs/naturezas competitivos dos treinadores regulares e novas revanches de líderes ainda precisam de implementação.
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

## Estado atual

Etapa de treinadores compilada: 636 equipes regulares e 60 elencos internos da Torre revisados; primeira luta preservada em singles; IA com comparação entre alvos e pontuação de suporte. Auditoria dos dados e testes nativos passaram. Três fixtures no mGBA chegaram à seleção de ação em 1.800 frames cada (laboratório, dupla normal e um contra dois), com inspeção das capturas. O patch cumulativo de fontes também passou por `git apply --check` sobre a base original. Ainda falta validar a campanha e a Torre completas, assim como as regras modernas e os demais sistemas abaixo.

Base pret/pokefirered obtida em game/, revisão ce6406d8c2eaf46d543c8a2606de0c3cb1951982. Ubuntu/WSL, ferramentas de compilação e mGBA instalados. Base compilada com checksum original confirmado. Alterações iniciais compiladas: TMs reutilizáveis, chance shiny de 1/256, quatro evoluções por troca simples convertidas para nível 37, Pokédex Nacional cedo e Exp. Share global com opção de ligar/desligar após o Parcel. A ROM de desenvolvimento passou por teste de inicialização de 900 frames no mGBA, sem validação de campanha ou dos sistemas em batalha. Evoluções por troca com item e os demais sistemas, incluindo Alphas e raids, continuam pendentes. A ROM gerada ainda não representa o escopo completo. Ver README.md para comandos, artefatos e limites das verificações.
