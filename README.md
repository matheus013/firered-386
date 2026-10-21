# FireRed 386

Hack de Pokémon FireRed para GBA, com história e mapas preservados, foco em duplas e melhorias de qualidade de vida. **Versão de desenvolvimento, ainda incompleta.** O nome 386 representa a meta do projeto: a distribuição de todas as espécies ainda está pendente.

## Download

Baixe o **`.gba`** em [Releases](https://github.com/matheus013/firered-386/releases) e abra em um emulador de GBA. Também estão disponíveis um patch `.ips`, o patch de fontes e checksums SHA-256.

O IPS exige uma ROM limpa de **FireRed USA 1.0**, SHA-1 `41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc`. O `.gba` já compilado dispensa aplicar patches. Para escolher Hardcore, use **NEW GAME**. Saves existentes são tratados como Normal. Faça backup do seu save antes de experimentar outra versão.

O jogo continua majoritariamente em inglês. A tela de seleção de modos está em português, sem acentos na interface do GBA.

## Todas as mudanças implementadas

### Batalhas e dificuldade

- Batalhas regulares de treinadores convertidas para duplas. A primeira luta contra o rival no laboratório permanece simples, nas três opções de inicial.
- **636 equipes regulares** revisadas, incluindo rivais posteriores, líderes, Elite Four, Campeão e revanches já existentes na base.
- Equipes de dois a seis integrantes; **líderes e Elite Four com seis Pokémon**, respeitando a especialidade de tipo quando aplicável.
- Suporte de abertura, cobertura, parceiros complementares, controle de velocidade, telas e clima em equipes compatíveis.
- Itens equipados e IVs progressivos. Os níveis dos treinadores regulares foram preservados nesta etapa; a dificuldade vem principalmente das equipes e duplas.
- Elencos gerados evitam golpes que atinjam aliados, golpes de nocaute em um acerto e acúmulo de evasão. Clima é evitado em equipes que possam ter Cloud Nine ou Air Lock.
- **60 elencos internos da Torre de Treinadores** revisados para duplas; segundo Pokémon adicionado aos adversários do modo Knockout. Níveis continuam acompanhando a equipe do jogador; EVs, IVs e naturezas originais da Torre foram preservados. Dados externos de e-Reader não foram revisados.
- Com apenas um Pokémon apto, o jogador enfrenta dois adversários com o segundo espaço vazio, sem duplicação ou exigência de um segundo integrante.

As equipes atuais usam golpes, tipos e regras de **geração III**. EVs/naturezas competitivos dos treinadores regulares e a calibração da campanha inteira continuam pendentes. Cada equipe, nível, item e golpe pode ser consultado em [TRAINER_REVIEW.md](TRAINER_REVIEW.md), com dados em [data/trainer_balance.json](data/trainer_balance.json).

### Decisões dos NPCs

- A IA compara os dois adversários como possíveis alvos, aproveitando os scripts existentes.
- Helping Hand considera a presença de um parceiro apto e capaz de atacar.
- Telas e clima já ativos recebem penalidade para reduzir ações repetidas sem efeito.
- Controle de velocidade e clima recebem pontuação conforme sua utilidade.
- Golpes contra ambos os adversários recebem incentivo quando os dois estão presentes.
- Dano ao aliado recebe penalidade quando o parceiro está presente e não é imune.
- Um golpe desabilitado não volta a ser escolhido apenas por receber bônus de suporte.

A revisão melhora a pontuação de ações, mas não garante decisões perfeitas em todas as situações.

### Experiência, itens e evolução

- **Exp. Share global após entregar o Parcel ao Professor Oak**, como item-chave que liga/desliga o efeito, sem ocupar item equipado.
- Cada participante recebe experiência integral; cada reserva apta recebe metade. Ovos e Pokémon desmaiados não recebem. No Hardcore, também vale o teto de nível.
- **TMs reutilizáveis:** não são consumidas ao ensinar golpes.
- **Kadabra → Alakazam, Machoke → Machamp, Graveler → Golem e Haunter → Gengar no nível 37**, sem troca. Evoluções por troca com item continuam pendentes.
- **Pokédex Nacional junto com a Pokédex inicial**, sem antecipar os outros eventos da história.
- **Shiny em 1/256** nas rotinas comuns de geração e identificação. Rotinas que explicitamente impedem shiny continuam impedindo.

### Normal ou Hardcore Nuzlocke

- Escolha ao criar a partida, com revisão das regras antes da confirmação. B volta e A confirma; a introdução original continua em seguida.
- Escolha fixa, persistida no save. Saves antigos permanecem em Normal.
- Normal conserva as melhorias da hack sem aplicar as restrições do Hardcore.

### Regras do Hardcore desta versão

1. **Perda permanente:** após as primeiras cinco Poké Balls de Oak, desmaios em batalha ou por veneno no mapa impedem o Pokémon de lutar novamente. Cura, Revive, Centro Pokémon e PC não desfazem a perda. Ele pode ficar guardado no PC.
2. **Primeiro encontro por área:** o primeiro selvagem elegível consome a oportunidade ao iniciar a batalha. Fugir ou derrotá-lo perde a captura. É permitido usar várias Poké Balls no mesmo encontro.
3. **Cota compartilhada:** grama, água, pesca e encontros estáticos na mesma seção do mapa compartilham a oportunidade. Andares/setores com a mesma seção também compartilham a cota.
4. **Duplicatas por espécie exata:** espécies registradas como capturadas são ignoradas sem consumir a área e não podem ser capturadas novamente. Não abrange toda a família evolutiva. Perdas e liberações não apagam o registro.
5. **Sem exceção shiny:** shinies seguem a cota normal.
6. **Presentes livres:** presentes, ovos, compras de Pokémon e trocas por NPC ficam fora da cota de selvagens, sem limite adicional por cidade. Continuam sujeitos à perda permanente e ao teto de nível.
7. **Mochila bloqueada em batalha**, exceto Poké Balls em encontros permitidos. Itens equipados e uso fora de batalha continuam disponíveis. Os adversários mantêm seus itens.
8. **Modo Set fixo**, inclusive na tela de opções.
9. **Limite de nível:** maior nível do primeiro líder ainda sem insígnia, na ordem dos oito ginásios. Após oito insígnias, usa Lance, atualmente nível 60; após a Liga, 100. EXP e Rare Candy não ultrapassam o teto.
10. **Pokémon acima do teto** mantém o nível, mas fica indisponível até o limite alcançá-lo. Aparece com zero HP disponível nesse período, sem ser marcado como perdido por isso.
11. **Derrota total** encerra a tentativa atual e mostra a tela de fim, com retorno ao menu para iniciar outra partida.

Capturas e perdas começam após as primeiras Poké Balls de Oak; limite, Set e restrição de itens valem desde a criação. Demonstrações, link e instalações de batalha ficam fora da marcação de perdas e da cota de captura.

Salvar pelo menu persiste o estado. **Não há autosave nem proteção contra recarregar um save anterior, save states, cheats ou edições externas.** A derrota bloqueia o estado atual em memória; recarregar um save anterior restaura aquele estado. Estas são as cláusulas desta hack, que podem diferir de outras variantes. Detalhes em [HARDCORE.md](HARDCORE.md).

### Correções e save

- Corrigida corrupção de memória em batalhas de um contra dois: nomes vazios do parceiro ausente agora recebem o terminador correto antes da expansão de texto.
- Configuração Hardcore e áreas armazenadas em espaço reservado do save, com assinatura de versão para evitar ativação em saves antigos.
- Tamanho do SaveBlock2 e endereços dos campos posteriores preservados; marca de perda permanente preservada no PC.
- Gravação de save de **128 KiB** e recarga em outra instância do mGBA verificadas em testes próprios.

## Melhorias aprovadas, ainda não implementadas

Estas funcionalidades **não estão disponíveis nesta release**:

- Todas as 386 espécies, incluindo lendários e míticos, obtíveis em um único save; distribuição nos mapas conforme raridade, poder e progresso, sem desequilibrar o início.
- Demais evoluções sem troca, incluindo as que exigem item.
- Separação físico/especial por golpe, mantendo categoria de status própria.
- Tipo Fada, tabela de efetividade e tipos correspondentes atualizados.
- Atributos base, dados de golpes e learnsets de uma geração recente, cuja referência exata ainda será definida.
- Novos golpes comuns dessa referência, com efeitos, animações, descrições e aprendizado compatíveis.
- HMs fora de batalha sem ocupar golpes, preservando exigências de insígnias e história.
- Novas revanches contra líderes, com seis integrantes e equipes competitivas de duplas coerentes com seus tipos; revisão competitiva final da Elite Four.
- Nova oportunidade de capturar lendários derrotados.
- Teto de nível também no modo Normal e exibição do teto atual no menu.
- Todos os itens utilizáveis/equipáveis compráveis em Celadon após a primeira vitória na Liga; catálogo, preços e exceções de itens-chave ainda serão definidos.
- Interface para distribuir/remover EVs sem farm; definir interação com EVs ganhos em batalha.
- Ajuste do efeito da natureza, troca entre habilidades disponíveis e ajuste de IVs no pós-jogo.
- Exibição clara de IVs, EVs, natureza, habilidade e categoria dos golpes.
- Lembrete de golpes gratuito e acessível cedo; itens de batalha renováveis.
- Informações sobre espécies encontradas por área.
- Restrição de mochila dos dois lados em batalhas importantes; atualmente o Hardcore restringe o jogador, enquanto os adversários mantêm seus itens.
- Alphas raros e raids aleatórias opcionais em mapas existentes, limitados pela progressão. Formato, captura, recompensas e persistência ainda exigem implementação.

Mega Evolução, Z-Moves, Dynamax/Gigantamax e Terastalização e os golpes dependentes dessas mecânicas estão fora do escopo aprovado. Novas formas e espécies após a geração III não estão automaticamente incluídas. Não há versão nativa para PC ou multiplayer de raids implementados. O planejamento completo está em [DESIGN.md](DESIGN.md).

## Compilação do código-fonte

Base: [pret/pokefirered](https://github.com/pret/pokefirered), revisão `ce6406d8c2eaf46d543c8a2606de0c3cb1951982`. Compilador: [pret/agbcc](https://github.com/pret/agbcc), revisão `da598c1d918402c42c0c0d7128ba14567f3175e9`. As modificações estão em um patch cumulativo; `game/` e `tools/agbcc/` são preparados localmente, sem submódulos.

No Linux ou Ubuntu/WSL:

```bash
sudo apt update
sudo apt install build-essential binutils-arm-none-eabi libpng-dev pkg-config git ca-certificates python3
git clone https://github.com/matheus013/firered-386.git
cd firered-386
bash scripts/setup.sh
bash scripts/build.sh hack
```

Resultado: `output/firered-386-dev.gba`. `bash scripts/build.sh baseline` compila a base original e verifica seu checksum. O build usa cache no sistema de arquivos Linux e deixa logs em `output/`. No Windows, execute pelo Ubuntu/WSL; wrappers PowerShell existentes usam a distribuição `Ubuntu`.

`patches/current.patch` é um patch de **fontes**, não de ROM. `0001-initial-improvements.patch` é histórico da primeira etapa: não aplique os dois. Para exportar mudanças locais de `game/`, use `python3 scripts/export_source_patch.py`. O setup preserva checkouts existentes com mudanças diferentes das esperadas.

## Verificação e limitações

Verificações realizadas em 20 e 21/10/2026:

- Base original com `make compare`: checksum confirmado. Hack compilada e inicializada no mGBA.
- Auditoria das equipes: quantidade, especialidade, legalidade dos golpes, repetições e dano ao aliado.
- Testes nativos da pontuação de suporte e condições de alvo/parceiro.
- Entrada em batalha: laboratório simples, dupla normal e um contra dois, alcançando seleção de ação.
- Hardcore: captura, duplicatas, shinies, presentes, perdas, cura/PC, limites, Normal, estrutura do save e gravação/recarga em nova instância.
- Seleção dos modos, cancelamento, confirmação, tela de fim e derrota real de um contra dois.
- Patch de fontes conferido contra a base original e alterações sem erros de whitespace.

Ainda não há validação de campanha completa, Torre inteira ou trocas no VBA Link. Podem existir problemas de balanceamento/compatibilidade. O resultado continua sendo uma ROM de GBA para emuladores.

Para repetir os testes após compilar, instale também `libmgba-dev` e `mgba-sdl` no Ubuntu/WSL:

```bash
python3 scripts/balance_trainers.py --check
python3 scripts/test_trainer_battles.py
python3 scripts/test_hardcore.py
```

## Créditos

Base: [pret/pokefirered](https://github.com/pret/pokefirered). Compilador: [pret/agbcc](https://github.com/pret/agbcc). Testes de execução: [mGBA](https://mgba.io/). Pokémon pertence aos seus respectivos titulares. Projeto de fãs, sem afiliação oficial.
