# Normal e Hardcore Nuzlocke

**NEW GAME** oferece Normal e Hardcore Nuzlocke. A primeira confirmação mostra as regras; B volta e A confirma. A introdução original continua depois. A escolha fica no save, sem opção de mudar durante a campanha. Saves antigos são tratados como Normal, que preserva o comportamento atual da hackrom.

## Hardcore implementado

- Após as primeiras Poké Balls de Oak, desmaiar em batalha ou por veneno no mapa causa perda permanente. Cura, Revive, Centro Pokémon e retirada do PC não removem essa condição. O Pokémon pode ficar guardado no PC.
- O primeiro selvagem elegível gasta a oportunidade da área ao iniciar a batalha. Fugir ou derrotá-lo perde a oportunidade. Pode lançar várias Balls durante esse encontro. Grama, água, pesca e encontros estáticos compartilham a cota.
- Espécies já registradas como capturadas são puladas sem consumir a área. Considera a espécie exata, não toda a família evolutiva. Perdas e liberações não removem esse registro.
- Shinies seguem a cota normal, sem exceção.
- Presentes, ovos, compras de Pokémon e trocas por NPC ficam fora da cota de selvagens. Esta variante usa presentes livres, sem limite adicional de um presente por cidade. Os recebidos ficam sujeitos à morte permanente e ao limite de nível.
- Itens da mochila são bloqueados em batalha, exceto Poké Balls em encontros permitidos. Itens equipados e itens fora de batalha continuam disponíveis; Pokémon perdidos não podem ser ressuscitados. Adversários mantêm seus itens.
- Modo Set fixo, inclusive na tela de opções. As duplas continuam funcionando.
- O limite de nível usa o maior nível do primeiro líder ainda sem insígnia, na ordem das oito insígnias. EXP e Rare Candy não ultrapassam o limite. Após oito insígnias, usa Lance; após a Liga, passa a 100. Um Pokémon recebido acima do limite mantém o nível e fica indisponível até o limite alcançá-lo. Aparece com zero HP disponível nesse período, mas não é marcado como morto por isso.
- Derrota total encerra a tentativa e mostra uma tela de fim, com retorno ao menu para iniciar outra partida.

Capturas e mortes começam após as primeiras cinco Poké Balls de Oak; limite, Set e restrição de itens valem desde a criação. A luta inicial continua em singles. Demonstrações, link e instalações de batalha ficam fora da marcação de mortes e da cota de captura. Andares ou setores com a mesma seção do mapa compartilham a oportunidade.

## Save e validação

Configurações e 256 bits de áreas ocupam espaço antes reservado no SaveBlock2, sem mudar seu tamanho ou os endereços dos campos posteriores. A perda permanente usa um bit reservado do cabeçalho do Pokémon, preservado no PC. Uma assinatura de versão evita ativação em saves antigos.

Salvar pelo menu persiste as condições atuais. **Não há salvamento automático ou proteção contra recarregar saves anteriores, save states, cheats ou edições externas.** A tela de derrota bloqueia a continuação do estado atual em memória; reabrir um save anterior restaura aquele estado. Nenhum save do usuário foi convertido ou apagado nos testes.

No Ubuntu/WSL, após compilar, execute `python3 scripts/test_hardcore.py`. Ele cria ROMs separadas em `output/tests/hardcore/` e verifica regras, layout do save, gravação e recarga em uma nova instância do mGBA, PC/cura, EXP, os dois menus, confirmação/cancelamento, tela de derrota e derrota real em dupla. As capturas são conferidas visualmente. Isso não substitui campanha completa ou validação de trocas no VBA Link. `--case` seleciona uma parte; `--build-dir` reutiliza um build temporário após mudanças apenas nos testes.

Foi corrigida também uma falha anterior em batalhas de um contra dois: o parceiro ausente tinha nome zerado sem terminador de texto. A expansão podia ultrapassar esse nome e corromper memória. Os nomes vazios agora recebem o terminador correto, e o teste avança até a derrota e a tela de fim.

Referências: [regras originais de Nick Franco](https://www.nuzlocke.com/about/), [Hardcore](https://nuzlockeuniversity.ca/nuzlocke-variants/hardcore-nuzlocke-variant/) e [cláusulas opcionais](https://nuzlockeuniversity.ca/optional-rules/). As escolhas específicas desta hackrom estão descritas acima.
