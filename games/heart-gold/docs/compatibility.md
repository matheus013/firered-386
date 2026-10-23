# Compatibilidade

## Plataformas

| Plataforma | Saída | Estado |
|---|---|---|
| macOS 11+ Apple Silicon (arm64) | `SuperHeartGold.app` | **Compilado e testado nesta máquina** (SDL3 do Homebrew e SDL3 estático via `HG_FETCH_SDL3`) |
| macOS 11+ Intel (x86-64) | `.app` universal (arm64 + x86-64) | Compilado como binário universal com `HG_FETCH_SDL3=ON`; a fatia x86-64 passou no autoteste headless **sob Rosetta 2**, não em um Mac Intel real |
| Windows 10+ x86-64 | `SuperHeartGold.exe` | **Não verificado.** Workflow escrito, ainda não executado |
| Linux | — | Fora do alvo; provavelmente compila (CMake + SDL3), sem garantia |

Notas:

- Com o SDL3 do Homebrew, o `.app` aponta para `/opt/homebrew/opt/sdl3/lib/libSDL3.0.dylib` e **só roda em máquinas com
  esse SDL instalado**. Para distribuir, use `-DHG_FETCH_SDL3=ON` (SDL3 estático dentro do executável).
- O Homebrew instala SDL3 só para arm64, então um `.app` universal precisa do SDL3 estático (`HG_FETCH_SDL3=ON`).
- Assinatura e notarização do `.app` e assinatura do `.exe` ainda não existem.

## Vídeo

- Renderizador escolhido pelo SDL: Metal no macOS (verificado), Direct3D/Vulkan/OpenGL no Windows.
- HiDPI/Retina: sim (`SDL_WINDOW_HIGH_PIXEL_DENSITY`, cálculo em pixels de saída).
- Qualquer resolução e proporção: a vista mantém a proporção e centraliza (`hg::place_screens`).
- Janela redimensionável e tela cheia (F11, `--fullscreen`).
- `--video-driver dummy` roda sem janela visível, para testes e CI.

## Entrada

- Teclado: sim. Remapeamento por arquivo: planejado (M1).
- Gamepad: qualquer controle reconhecido pela base do SDL3 (Xbox, PlayStation, Switch Pro, genéricos com mapeamento).
  Conexão a quente funciona por evento; **não testado com um controle físico nesta entrega** (o teste usa eventos
  sintéticos).
- Mouse/toque para os painéis do HUD: planejado.

## Dados e saves

- **Não precisa e não usa ROM, BIOS ou firmware.** Nenhum arquivo do jogo original é lido.
- Saves do DS (`.sav`) não são compatíveis e não há plano de conversão.
- Saves do Super HeartGold terão formato próprio e versionado (M2).

## Mecânicas

- Meta: comportamento igual ao de HG/SS (Gen IV) em batalha, crescimento, captura, encontros e eventos.
- Diferenças intencionais só em apresentação e conveniência (vista única, escala, velocidade, controles),
  sempre opcionais quando afetarem a experiência original.
- Hoenn segue as mecânicas de HG/SS, não as de Emerald.
