# Roadmap

Objetivo final: um jogo nativo para PC e Mac com **três regiões** (Johto, Kanto e Hoenn) e **24 insígnias**,
com progressão compartilhada, mecânicas da Gen IV de HG/SS e apresentação em uma janela.

Ordem fixa: **Johto → Kanto → Hoenn.** Hoenn só começa quando Johto e Kanto estiverem jogáveis do começo ao fim.

Cada marco termina com algo que compila, roda e tem teste. Marcos não começam antes do anterior estar verificado.

## M0. Fundação (esta entrega)

- [x] Estrutura de pastas e documentação.
- [x] CMake + C++20 + SDL3; engine separada da plataforma e das regiões.
- [x] Executável mínimo: janela, teclado, gamepad, log, tela cheia, velocidade, modo unificado e clássico.
- [x] Testes unitários com CTest e teste de fumaça headless do executável.
- [x] Workflows de CI escritos para Windows e macOS (ainda não executados, ver README).

## M1. Base da engine

- Arquivo de configuração do jogador (controles, vídeo, áudio, velocidade), lido e gravado em `SDL_GetPrefPath`.
- Renderizador: duas camadas de saída (vista principal e HUD), primeiro com `SDL_Renderer`; avaliar `SDL_GPU` quando o 3D chegar.
- Áudio: saída com `SDL_AudioStream`, mixer simples, formato de áudio próprio.
- Sistema de recursos: carregar assets próprios a partir do manifesto (`assets/manifest.json`).
- Save: serialização explícita e versionada, com testes de ida e volta.
- Fonte e texto (UTF-8, quebra de linha, caixas de diálogo).

## M2. Dados e ferramentas

- Formatos de texto versionáveis (JSON/TOML) para espécies, golpes, itens, tipos, crescimento e encontros.
- Validadores em `tools/` rodando no CTest.
- Conteúdo de dados escrito pelo projeto, com a tabela de mecânicas da Gen IV como especificação.

## M3. Overworld

- Mapas, colisão, movimento em grade, portas e transições.
- Máquina virtual de eventos (comandos genéricos em `game/events/`).
- NPCs, diálogos, itens no chão, flags.
- Pokémon seguindo o treinador.
- Câmera e enquadramento para a vista 16:9 (confirmar o tamanho da vista lógica).

## M4. Batalha

- Batalhas simples e duplas com as regras da Gen IV.
- Fórmula de dano, efeitos de golpes, habilidades, itens, status, IA de treinadores.
- HUD de batalha no modo unificado (comandos, golpes, equipe).
- Testes de comportamento: casos com entradas e saídas conhecidas, verificados contra a referência documentada.

## M5. Johto (8 insígnias)

- Fatia vertical primeiro: New Bark Town → Violet City (1ª insígnia), jogável do começo ao fim.
- Depois Johto completa até a Liga Pokémon.

## M6. Kanto (insígnias 9–16)

- Kanto como em HG/SS, ligada a Johto pelo mesmo save e pela mesma progressão.

## M7. Hoenn (insígnias 17–24)

- **Conteúdo com Pokémon Emerald como referência** (mapas, história, ginásios, Frontier opcional);
  **engine e mecânicas de HG/SS** (Gen IV, overworld 3D, seguidor). Nada da engine do GBA é portado.
- Tudo como dados de região em `game/regions/hoenn/`, sem código específico da região.
- Como chegar a Hoenn, ordem dos ginásios e curva de níveis: a definir quando M6 terminar.

## Progressão compartilhada (vale para M5–M7)

- Um único save, um treinador, uma equipe, uma Pokédex nacional.
- Estojo com 24 insígnias; cada região tem suas 8 e seus próprios requisitos de HM/obediência.
- Curva de níveis contínua entre as regiões.
- Flags de evento com espaço por região, para que uma região nunca reutilize as flags de outra.

## Fora do escopo por enquanto

- Rede (troca e batalha online), Pokéwalker, conectividade com outros jogos.
- Mods e editor de mapas próprios (as ferramentas internas vêm primeiro).
