# Normal e Hardcore Nuzlocke

**NEW GAME** oferece Normal e Hardcore Nuzlocke. A primeira confirmação mostra as regras; B volta e A confirma. Depois, escolha Randomizer OFF/ON; B retorna à seleção do modo. A introdução original continua depois. A escolha fica no save, sem opção de mudar durante a campanha. Saves antigos são tratados como Normal, com melhorias e teto de EXP/nível, sem as demais restrições do Hardcore.

Idioma EN/PT-BR pode ser alterado nas opções. Saves sem os novos campos iniciam em EN e randomizer desligado, preservando o modo e os contadores existentes. [Configurações](SETTINGS.md).

## Hardcore implementado

- Após as primeiras Poké Balls de Oak, desmaiar em batalha ou por veneno no mapa causa perda permanente. Cura, Revive, Centro Pokémon e retirada do PC não removem essa condição. O Pokémon pode ficar guardado no PC.
- O primeiro selvagem elegível gasta a oportunidade da área ao iniciar a batalha. Fugir ou derrotá-lo perde a oportunidade. Pode lançar várias Balls durante esse encontro. Grama, água, pesca e encontros estáticos compartilham a cota.
- Espécies já registradas como capturadas são puladas sem consumir a área. Considera a espécie exata, não toda a família evolutiva. Perdas e liberações não removem esse registro.
- Shinies seguem a cota normal, sem exceção. Fantasmas não capturáveis da Torre, incluindo Marowak revelado, não consomem a cota (corrigido na v0.3.1-dev).
- Presentes, ovos, compras de Pokémon e trocas por NPC ficam fora da cota de selvagens. Esta variante usa presentes livres, sem limite adicional de um presente por cidade. Os recebidos ficam sujeitos à morte permanente e ao limite de nível.
- Itens da mochila são bloqueados em batalha, exceto Poké Balls em encontros permitidos. Itens equipados e itens fora de batalha continuam disponíveis; Pokémon perdidos não podem ser ressuscitados. Adversários mantêm seus itens.
- Modo Set fixo, inclusive na tela de opções. As duplas continuam funcionando.
- O limite de nível usa o maior nível do primeiro líder ainda sem insígnia, na ordem das oito insígnias. EXP, Rare Candy e treinamento na creche não ultrapassam o limite. A previsão de níveis e cobrança da creche, incluindo Route 5, também respeitam o teto vigente; EXP excedente é descartada ao retirar. Os tetos dos ginásios são 18/28/36/44/52/60/68/76. Após oito insígnias, o teto sobe conforme cada membro é vencido: Lorelei 80, Bruno 83, Agatha 86, Lance 89 e Campeão 92; após a Liga, passa a 100. Um Pokémon recebido acima do limite mantém o nível e fica indisponível até o limite alcançá-lo. Aparece com zero HP disponível nesse período, mas não é marcado como morto por isso.
- Rare Candy custa ₽1 e é acrescentado às lojas comuns somente no Hardcore; sua revenda rende ₽0. O Normal mantém preço/estoque próprios. Máquinas, prêmios e decoração não são lojas comuns.
- Cada Centro Pokémon permite três curas completas pela enfermeira por partida, com contador individual salvo. Recusar não consome; o quarto atendimento é bloqueado. Camas e cura por PC não são contadas. Normal não tem essa limitação.
- Cientista nos 19 Centros edita natureza, EVs e IVs gratuitamente desde o início no Hardcore; no Normal, somente após a Liga. Não cura, revive ou permite editar perdidos/ovos; respeita o teto de disponibilidade do Hardcore. EVs de batalha continuam ativos.
- Derrota total encerra a tentativa e mostra uma tela de fim, com retorno ao menu para iniciar outra partida.

Capturas e mortes começam após as primeiras cinco Poké Balls de Oak; limite, Set e restrição de itens valem desde a criação. A luta inicial continua em singles. Demonstrações, link e instalações de batalha ficam fora da marcação de mortes e da cota de captura. Andares ou setores com a mesma seção do mapa compartilham a oportunidade.

## Encontros e lojas em v0.3.0-dev

As 386 espécies têm encontros diretos, com locais e probabilidades em [ENCOUNTERS.md](ENCOUNTERS.md) e [SPECIES_LOCATIONS.md](SPECIES_LOCATIONS.md). A coleção completa em um save é possível no Normal; no Hardcore, a cota por seção continua limitando as capturas da tentativa. Os novos lendários/míticos aleatórios exigem a primeira vitória na Liga.

A saída das lojas após compra/venda foi corrigida em v0.3.0-dev. Foram aprovados 23 catálogos nos dois modos (46 casos), incluindo compra do Rare Candy por ₽1 no Hardcore. [Escopo e resultados](SHOP_TEST_RESULTS.md).

## Validação da captura — v0.3.1-dev

Passaram 23 casos, incluindo a decisão de cota em todos os 124 mapas com tabelas de encontros, fuga/KO simulados, duplicatas, shiny, andares/Safari, comandos nativos de captura e bloqueio da próxima Ball. Um save de 128 KiB recarregado em outra instância preservou a área consumida e outra área livre. [Escopo, limites e reprodução](CAPTURE_TEST_RESULTS.md). O teste não percorre toda a campanha nem valida conexão no VBA Link.

## Save e validação

Configurações e 256 bits de áreas ocupam espaço antes reservado no SaveBlock2, sem mudar seu tamanho ou os endereços dos campos posteriores. A perda permanente usa um bit reservado do cabeçalho do Pokémon, preservado no PC. Uma assinatura de versão evita ativação em saves antigos. Os novos contadores de cura usam uma assinatura adicional; Hardcore antigo inicia esses contadores em zero. O tamanho do Pokémon e do save permanece igual.

Salvar pelo menu persiste as condições atuais. **Não há salvamento automático ou proteção contra recarregar saves anteriores, save states, cheats ou edições externas.** A tela de derrota bloqueia a continuação do estado atual em memória; reabrir um save anterior restaura aquele estado. Nenhum save do usuário foi convertido ou apagado nos testes.

No Ubuntu/WSL, após compilar, execute `python3 scripts/test_hardcore.py`. Ele cria ROMs separadas em `output/tests/hardcore/` e verifica regras, layout do save, gravação e recarga em uma nova instância do mGBA, PC/cura, EXP, os dois menus, confirmação/cancelamento, tela de derrota e derrota real em dupla. As capturas são conferidas visualmente. Isso não substitui campanha completa ou validação de trocas no VBA Link. `--case` seleciona uma parte; `--build-dir` reutiliza um build temporário após mudanças apenas nos testes.

Foi corrigida também uma falha anterior em batalhas de um contra dois: o parceiro ausente tinha nome zerado sem terminador de texto. A expansão podia ultrapassar esse nome e corromper memória. Os nomes vazios agora recebem o terminador correto, e o teste avança até a derrota e a tela de fim.

Referências: [regras originais de Nick Franco](https://www.nuzlocke.com/about/), [Hardcore](https://nuzlockeuniversity.ca/nuzlocke-variants/hardcore-nuzlocke-variant/) e [cláusulas opcionais](https://nuzlockeuniversity.ca/optional-rules/). As escolhas específicas desta hackrom estão descritas acima.
