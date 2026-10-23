# Arquitetura

## 1. Arquitetura original (HeartGold/SoulSilver no Nintendo DS)

Resumo do que o jogo original faz, como referência. Os detalhes e as fontes estão em
[porting-strategy.md](porting-strategy.md).

- **ARM9** roda toda a lógica do jogo. **ARM7** roda o driver de som (sequenciador SDAT), toque, RTC e rádio.
- O código é dividido em **overlays** carregados sob demanda: campo, batalha, menus e cada mini-jogo ocupam
  a mesma região de memória em momentos diferentes.
- O jogo é organizado em **"aplicações"**: processos com estado próprio (overworld, batalha, bolsa, Pokédex,
  Pokégear...) que o loop principal troca entre si.
- **Duas telas:** a de cima mostra o mundo e a batalha; a de baixo (sensível ao toque) mostra menus,
  Pokégear, comandos de batalha e o Pokémon que segue o treinador (Pokéwalker/menus).
- **Gráficos:** overworld em 3D (mapas e prédios como modelos, câmera fixa) com personagens em sprites 2D;
  menus em camadas 2D por tiles.
- **Dados:** tudo no NitroFS, em arquivos NARC indexados por número (espécies, golpes, mapas, scripts de evento,
  textos, gráficos). **Scripts de evento** são bytecode interpretado por uma máquina virtual de script.
- **Tempo:** lógica em passo fixo de um quadro do DS (≈59,83 Hz).

## 2. Arquitetura do Super HeartGold

```
┌──────────────────────────────────────────────────────────┐
│ platform/   SDL3: janela, eventos, áudio, caminhos, .app │  ← único lugar que inclui SDL
├──────────────────────────────────────────────────────────┤
│ engine/     loop, passo fixo, ações de entrada, viewport, │  ← C++20 puro, sem SDL,
│             log, (futuro) renderer, recursos, script VM   │    sem regras de Pokémon
├──────────────────────────────────────────────────────────┤
│ game/       regras comuns a todas as regiões:             │
│   core/     estado global, progressão, flags              │
│   battle/   batalha (mecânicas da Gen IV)                 │
│   pokemon/  espécies, golpes, crescimento, natures        │
│   world/    overworld, colisão, NPCs, seguidor            │
│   events/   comandos de script de evento                  │
│   save/     serialização versionada                       │
├──────────────────────────────────────────────────────────┤
│ game/regions/{johto,kanto,hoenn}/   SÓ DADOS + scripts    │  ← mapas, encontros,
│                                                           │    treinadores, eventos
└──────────────────────────────────────────────────────────┘
assets/  conteúdo próprio (arte, áudio, textos) com manifesto
tools/   conversores e validadores que rodam no build, nunca no jogo
```

Regras de dependência:

1. `engine/` não inclui nada de `game/` nem de `platform/`. Hoje isso é verdade: `hg_engine` compila sem SDL.
2. `game/` não inclui SDL. Fala com o mundo pelas interfaces da engine.
3. **Regiões são dados.** Uma região não tem código C++ próprio; se uma região precisar de um comportamento
   novo, ele vira um comando de evento genérico em `game/events/` e a região só o usa. É isso que permite
   colocar Hoenn na mesma engine de HG/SS.
4. Hoenn usa **Pokémon Emerald como referência de conteúdo** (mapas, história, treinadores, ordem das insígnias),
   mas roda **na engine e nas mecânicas de HG/SS** (Gen IV: divisão físico/especial, overworld 3D, seguidor, etc.).
   Nada da engine de Emerald (GBA) é portado.

### Loop principal (implementado de forma mínima)

```
a cada quadro de vídeo:
    input.begin_frame()
    processa eventos do SDL → ações (teclado e gamepad)
    trata ações de sistema (sair, tela cheia, velocidade)
    passos = FixedStep.advance(tempo real)   // 0..N passos de 1/59,83 s
    repete `passos` vezes: lógica do jogo     // ainda vazio
    desenha
```

A lógica sempre roda em passos do DS. Velocidade 2x ou 4x (ferramenta de desenvolvimento por enquanto) só roda mais passos por segundo; nenhuma fórmula muda.

## 3. Apresentação: das duas telas para uma janela

### Modo unificado (padrão, "desktop")

A janela é de desktop (1280×720 por padrão, qualquer tamanho). **A tela de cima original fica no centro, em 4:3,
sem nenhuma mudança na área visível**: mundo, batalha e cenas aparecem exatamente como no DS, só que escalados.
As faixas que sobram dos lados numa tela 16:9 recebem o **HUD**, com tudo o que era da tela de baixo:

```
┌────────┬──────────────────────────┬────────┐
│  HUD   │                          │  HUD   │
│ (esq.) │   tela de cima original  │ (dir.) │
│        │      4:3, 256×192 × N    │        │
│        │                          │        │
└────────┴──────────────────────────┴────────┘
   1080p, escala inteira 5: vista 1280×960, faixas de 320 px
```

| Conteúdo da tela de baixo no DS | No modo unificado |
|---|---|
| Menu de campo (Pokémon, bolsa, Pokédex, salvar...) | Painel numa faixa lateral |
| Comandos de batalha (Lutar, Bolsa, Pokémon, Fugir) e golpes | Painel lateral durante a batalha, por cursor ou mouse |
| Equipe, bolsa, Pokédex, Pokégear | Painéis laterais; telas grandes (mapa do Pokégear) abrem sobre a vista |
| Mensagens e status | Elementos discretos nas faixas |
| Toque na tela | Clique do mouse nos painéis; tudo também acessível por teclado e gamepad |

- `hg::place_screens(..., ScreenLayout::Unified, ...)` devolve a vista (`top`) e as faixas (`hud_left`, `hud_right`).
- O HUD é desenhado na resolução nativa da janela, não na escala de pixels do jogo, para o texto ficar nítido.
- Se a janela for mais estreita que 4:3, não sobram faixas; os painéis passam a aparecer sobre a vista (a implementar).
- Telas que dependem de toque (alguns mini-jogos, Pokéathlon) precisam de um desenho próprio para mouse/controle.
- Referência de design: `pokeheartgold-single-screen` e `ssrecomp` resolveram o mesmo problema
  (ver [porting-strategy.md](porting-strategy.md), seção 6).

### Modos clássicos

- `--layout stacked`: as duas telas 256×192, uma sobre a outra.
- `--layout side`: lado a lado (bom para monitores largos).

### Escala, resolução e janela

- `hg::place_screens` calcula onde desenhar mantendo a proporção (sem distorção), centralizando com faixas pretas.
- `--scale integer` (padrão) usa a maior escala inteira que cabe (pixels nítidos); `--scale fit` preenche o máximo.
- Suporte a HiDPI/Retina: o cálculo usa o tamanho em pixels da saída (`SDL_GetRenderOutputSize`).
- Janela redimensionável; tela cheia com F11 ou `--fullscreen`.

## 4. Entrada

- O jogo só conhece **ações** (`hg::Action`: direções, A, B, X, Y, L, R, Start, Select e ações de sistema).
- `hg::InputBindings` traduz códigos físicos (tecla ou botão do SDL) para ações; existe uma tabela para teclado
  e outra para gamepad. Hoje as tabelas padrão estão no código (`platform/sdl3/default_bindings.cpp`);
  o arquivo de configuração do jogador é o próximo passo.
- `hg::InputState` guarda o estado por quadro (`is_down`, `was_pressed`, `was_released`).
- Gamepads são abertos quando conectados (`SDL_EVENT_GAMEPAD_ADDED`), usando a base de controles do SDL.

| Ação | Teclado padrão | Gamepad padrão |
|---|---|---|
| Direções | Setas | D-pad |
| A / B | X / Z | Botão de baixo / da direita |
| X / Y | S / A | Botão de cima / da esquerda |
| L / R | Q / W | Ombros |
| Start / Select | Enter / Backspace | Start / Back |
| Velocidade (1x→2x→4x) | Tab | Clique do analógico direito |
| Tela cheia | F11 | — |
| Sair | Esc | — |

## 5. O que muda em relação ao original

Na **Fase 1** (ver [roadmap.md](roadmap.md)), só a interface: janela de desktop, HUD lateral, escala, tela cheia,
controles configuráveis e gamepad. Mecânicas, conteúdo e área visível ficam iguais. O Pokémon seguindo o treinador
já existe em HG/SS e entra como no original.

Na **Fase 2**, opcionais:

- **Velocidade configurável:** `hg::FixedStep` já tem multiplicador (1x, 2x, 4x; limite 16x); hoje é ferramenta de desenvolvimento (Tab).
- **Vista ampliada em 16:9:** mostrar mais área do mundo em vez das faixas.
- **Seguidor em Hoenn** e o resto das extensões.
