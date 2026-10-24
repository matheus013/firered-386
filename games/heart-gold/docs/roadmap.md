# Roadmap

O projeto tem duas fases, nesta ordem.

**Fase 1: HeartGold/SoulSilver fiel e jogável.** Reimplementar o jogo inteiro (Johto e Kanto, 16 insígnias, pós-jogo)
**sem mudanças** de mecânica, conteúdo ou área visível. A **única** diferença é a interface: uma janela de desktop
com a tela de cima original (4:3) no centro e tudo o que era da tela de baixo num HUD nas laterais.
Todo o conteúdo (mapas, textos, sprites, modelos, músicas) é **recriado pelo projeto**, sem ROM.

**Fase 2: extensões.** Só depois da Fase 1 jogável do começo ao fim: Hoenn, 24 insígnias e os extras.

Cada marco termina com algo que compila, roda e tem teste. Um marco só começa depois que o anterior estiver verificado.

## Fase 1

### M0. Fundação (feito)

- [x] Estrutura de pastas e documentação.
- [x] CMake + C++20 + SDL3; engine separada da plataforma e das regiões.
- [x] Executável mínimo: janela, teclado, gamepad, log, tela cheia, modo unificado (padrão) e clássico.
- [x] Testes unitários com CTest e teste de fumaça headless do executável.
- [x] Workflows de CI escritos para Windows e macOS (ainda não executados, ver README).

### M1. Rodar nativo com a interface unificada

O executável nativo entrega a interface unificada antes de qualquer conteúdo de jogo.

- [x] Layout: vista 4:3 no centro, faixas laterais devolvidas para o HUD (`hg::place_screens`).
- [ ] Renderizador em duas camadas: vista do jogo (256×192, escalada) e HUD em resolução nativa da janela.
- [ ] Sistema de painéis do HUD: posição nas faixas, abrir/fechar, foco e navegação por teclado, gamepad e mouse.
- [ ] Painéis com conteúdo de demonstração: menu de campo, comandos de batalha, golpes, equipe, bolsa.
- [ ] Fallback para janelas sem faixa lateral (mais estreitas que 4:3): painéis sobrepostos à vista.
- [ ] Texto: fonte própria em bitmap, UTF-8, caixa de diálogo.
- [ ] Arquivo de configuração do jogador (controles e vídeo), em `SDL_GetPrefPath`.

**Pronto quando:** `.app` (arm64/universal) e `.exe` (x86-64) abrem direto na interface unificada, todos os painéis
respondem a teclado, gamepad e mouse, a escala funciona em 720p, 1080p, 1440p e ultrawide, há testes de layout e
navegação do HUD, e o build do Windows rodou de verdade.

Referência de design (sem copiar código): `pokeheartgold-single-screen` e `ssrecomp` (ver `porting-strategy.md`, seção 6).

### M2. Base da engine

- Sistema de recursos: carregar assets próprios a partir do manifesto (`assets/manifest.json`).
- Áudio: saída com `SDL_AudioStream`, mixer, sequenciador para as músicas recriadas.
- Save: serialização explícita e versionada, com testes de ida e volta.
- Renderizador 3D para o overworld (avaliar `SDL_GPU`).

### M3. Dados e ferramentas

- Formatos de texto versionáveis para espécies, golpes, itens, tipos, crescimento, encontros, treinadores.
- Ferramentas de autoria para mapas e eventos (o conteúdo será recriado à mão).
- Validadores em `tools/` rodando no CTest.

### M4. Overworld

- Mapas, colisão, movimento em grade, portas e transições, câmera igual à original.
- Máquina virtual de eventos (comandos genéricos em `game/events/`).
- NPCs, diálogos, itens, flags, relógio em tempo real (dia/noite e eventos por dia da semana).
- Pokémon seguindo o treinador (já existe em HG/SS).

### M5. Batalha

- Batalhas simples e duplas com as regras da Gen IV.
- Fórmula de dano, efeitos de golpes, habilidades, itens, status, IA de treinadores.
- HUD de batalha do M1 ligado à batalha real.
- Testes de comportamento com entradas e saídas conhecidas, verificados contra a referência documentada.

### M6. Johto (insígnias 1–8)

- Fatia vertical primeiro: New Bark Town → Violet City, jogável do começo ao fim.
- Depois Johto completa até a Liga Pokémon.

### M7. Kanto (insígnias 9–16) e pós-jogo

- Kanto como em HG/SS, no mesmo save.
- Pós-jogo de HG/SS (Mt. Silver, Red, Battle Frontier).

**Fase 1 pronta quando:** dá para jogar do início ao fim como em HG/SS, com as mesmas mecânicas e o mesmo conteúdo,
mudando só a interface.

Fora da Fase 1: recursos que dependem de rede ou de outro hardware (Wi-Fi, troca, Pokéwalker, GTS).

## Fase 2: extensões (depois da Fase 1)

- **Hoenn (insígnias 17–24):** conteúdo com Pokémon Emerald como referência; engine e mecânicas de HG/SS.
  Tudo como dados em `game/regions/hoenn/`. Ligação com Johto/Kanto, ordem dos ginásios e curva de níveis a definir.
- **Progressão compartilhada:** um save, uma Pokédex nacional, estojo com 24 insígnias, flags com espaço por região.
- **Extras de conveniência, sempre opcionais:** velocidade 2x/4x, vista ampliada em 16:9 (mais área do mundo),
  Pokémon seguidor em Hoenn.

## Notas

- A tecla de velocidade (Tab) já existe no executável como ferramenta de desenvolvimento; como recurso para o
  jogador, é da Fase 2.
- Recriar o conteúdo "igual" (mapas, textos, músicas) continua sendo reproduzir obra protegida. O projeto aceita esse
  risco ao escolher este caminho; ver `porting-strategy.md`, seção 2.
