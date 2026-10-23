# Roadmap

Objetivo final: um jogo nativo para PC e Mac com **três regiões** (Johto, Kanto e Hoenn) e **24 insígnias**,
com progressão compartilhada, mecânicas da Gen IV de HG/SS e apresentação em uma janela.

Ordem fixa: **Johto → Kanto → Hoenn.** Hoenn só começa quando Johto e Kanto estiverem jogáveis do começo ao fim.

Cada marco termina com algo que compila, roda e tem teste. Marcos não começam antes do anterior estar verificado.

## M0. Fundação (esta entrega)

- [x] Estrutura de pastas e documentação.
- [x] CMake + C++20 + SDL3; engine separada da plataforma e das regiões.
- [x] Executável mínimo: janela, teclado, gamepad, log, tela cheia, velocidade, modo unificado (padrão) e clássico.
- [x] Testes unitários com CTest e teste de fumaça headless do executável.
- [x] Workflows de CI escritos para Windows e macOS (ainda não executados, ver README).

## M1. Primeiro objetivo: rodar nativo com a interface unificada

Antes de qualquer conteúdo de jogo, o executável nativo precisa entregar a **interface unificada completa**:
uma vista 16:9 com o HUD que substitui a tela de baixo, funcionando em macOS e Windows.

- Renderizador em duas camadas: vista principal (mundo/batalha) e HUD por cima, primeiro com `SDL_Renderer`.
- Sistema de painéis do HUD: âncoras e tamanhos na vista lógica, abrir/fechar, foco e navegação por
  teclado, gamepad e mouse (clique e hover).
- Os painéis que herdam a tela de baixo, com conteúdo de demonstração: menu de campo (lateral),
  comandos de batalha (inferior), equipe/mensagens (cantos).
- Texto: fonte própria em bitmap, UTF-8, caixa de diálogo.
- Arquivo de configuração do jogador (controles, vídeo, velocidade), lido e gravado em `SDL_GetPrefPath`.
- Cena de demonstração com arte provisória própria para exercitar tudo isso.

**Pronto quando:** o `.app` (arm64/universal) e o `.exe` (x86-64) abrem direto na interface unificada, todos os
painéis respondem a teclado, gamepad e mouse, a escala funciona em 720p, 1080p, 1440p e ultrawide, e há testes
de layout e navegação do HUD no CTest. O build do Windows precisa ter rodado de verdade (CI ou máquina).

## M2. Base da engine

- Renderizador: avaliar `SDL_GPU` quando o 3D do overworld chegar.
- Áudio: saída com `SDL_AudioStream`, mixer simples, formato de áudio próprio.
- Sistema de recursos: carregar assets próprios a partir do manifesto (`assets/manifest.json`).
- Save: serialização explícita e versionada, com testes de ida e volta.

## M3. Dados e ferramentas

- Formatos de texto versionáveis (JSON/TOML) para espécies, golpes, itens, tipos, crescimento e encontros.
- Validadores em `tools/` rodando no CTest.
- Conteúdo de dados escrito pelo projeto, com a tabela de mecânicas da Gen IV como especificação.

## M4. Overworld

- Mapas, colisão, movimento em grade, portas e transições.
- Máquina virtual de eventos (comandos genéricos em `game/events/`).
- NPCs, diálogos, itens no chão, flags.
- Pokémon seguindo o treinador.
- Câmera e enquadramento para a vista 16:9 (confirmar o tamanho da vista lógica).

## M5. Batalha

- Batalhas simples e duplas com as regras da Gen IV.
- Fórmula de dano, efeitos de golpes, habilidades, itens, status, IA de treinadores.
- Ligar o HUD de batalha do M1 à batalha real (comandos, golpes, equipe).
- Testes de comportamento: casos com entradas e saídas conhecidas, verificados contra a referência documentada.

## M6. Johto (8 insígnias)

- Fatia vertical primeiro: New Bark Town → Violet City (1ª insígnia), jogável do começo ao fim.
- Depois Johto completa até a Liga Pokémon.

## M7. Kanto (insígnias 9–16)

- Kanto como em HG/SS, ligada a Johto pelo mesmo save e pela mesma progressão.

## M8. Hoenn (insígnias 17–24)

- **Conteúdo com Pokémon Emerald como referência** (mapas, história, ginásios, Frontier opcional);
  **engine e mecânicas de HG/SS** (Gen IV, overworld 3D, seguidor). Nada da engine do GBA é portado.
- Tudo como dados de região em `game/regions/hoenn/`, sem código específico da região.
- Como chegar a Hoenn, ordem dos ginásios e curva de níveis: a definir quando M7 terminar.

## Progressão compartilhada (vale para M6–M8)

- Um único save, um treinador, uma equipe, uma Pokédex nacional.
- Estojo com 24 insígnias; cada região tem suas 8 e seus próprios requisitos de HM/obediência.
- Curva de níveis contínua entre as regiões.
- Flags de evento com espaço por região, para que uma região nunca reutilize as flags de outra.

## Fora do escopo por enquanto

- Rede (troca e batalha online), Pokéwalker, conectividade com outros jogos.
- Mods e editor de mapas próprios (as ferramentas internas vêm primeiro).
