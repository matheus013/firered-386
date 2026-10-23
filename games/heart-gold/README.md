# Super HeartGold

Jogo **nativo para PC e Mac** inspirado em Pokémon HeartGold/SoulSilver. **Não é uma hackrom**: não usa ROM,
emulador nem dados extraídos. As decompilações de HG/SS servem só como referência técnica e de comportamento.

Objetivo final: três regiões (Johto e Kanto como em HG/SS; Hoenn com Emerald como referência de conteúdo,
rodando na engine de HG/SS), 24 insígnias e progressão compartilhada, com as mecânicas originais.

**Estado:** fundação (M0). Há um executável que abre uma janela e processa teclado e gamepad. Ainda não há jogo.

## Documentação

| Documento | Conteúdo |
|---|---|
| [docs/architecture.md](docs/architecture.md) | Arquitetura original do DS, arquitetura deste projeto, modo de tela única com HUD, escala, controles |
| [docs/porting-strategy.md](docs/porting-strategy.md) | Investigação de pret/pokeheartgold e ctrl-cheeb-del/pokeheartgold, licenças, dependências do DS, port × reimplementação, fontes |
| [docs/compatibility.md](docs/compatibility.md) | Plataformas, vídeo, entrada, saves e mecânicas |
| [docs/roadmap.md](docs/roadmap.md) | Marcos M0–M7: engine, dados, overworld, batalha, Johto, Kanto, Hoenn |

## Estrutura

```
games/heart-gold/
  CMakeLists.txt        projeto CMake (C++20, SDL3)
  engine/               biblioteca hg_engine: log, ações de entrada, viewport, passo fixo (sem SDL)
  platform/sdl3/        executável SuperHeartGold (janela, eventos, render mínimo)
  platform/macos/       Info.plist do .app
  platform/windows/     notas do build Windows
  game/                 regras do jogo: core, battle, pokemon, world, events, save (vazio por enquanto)
  game/regions/         johto, kanto, hoenn: só dados (vazio por enquanto)
  assets/               conteúdo próprio + manifest.json
  tools/                check_assets.py (validação do manifesto)
  tests/                testes unitários (framework mínimo próprio em tests/test.hpp) e CTest
  scripts/build.sh      configura, compila e testa
  .github/workflows/    CI para Windows e macOS (ver abaixo)
```

## Compilar e rodar

### macOS (Apple Silicon)

```sh
brew install cmake sdl3
cd games/heart-gold
scripts/build.sh                 # build/ com o SDL3 do Homebrew
open build/bin/SuperHeartGold.app
```

`.app` universal (arm64 + x86-64) e autocontido, com SDL3 baixado e linkado de forma estática:

```sh
scripts/build.sh universal       # build-universal/
```

### Windows

Ver [platform/windows/README.md](platform/windows/README.md).

### Opções do executável

```
--layout unified|stacked|side   unified (padrão): vista única 16:9 com HUD; stacked/side: duas telas clássicas
--scale integer|fit             escala inteira (padrão) ou preencher a janela, sem distorção
--fullscreen                    começa em tela cheia (F11 alterna)
--frames N                      encerra depois de N quadros
--video-driver dummy            roda sem janela visível (testes e CI)
--self-test                     injeta eventos de teclado e gamepad e falha se o mapeamento não funcionar
--verbose                       log de depuração
```

Controles padrão: setas, X (A), Z (B), S (X), A (Y), Q/W (L/R), Enter (Start), Backspace (Select),
Tab (velocidade 1x/2x/4x), F11 (tela cheia), Esc (sair). Tabela completa em `docs/architecture.md`.

## Workflows do GitHub Actions

Os workflows estão em `games/heart-gold/.github/workflows/` (`windows.yml` e `macos.yml`). **O GitHub só executa
workflows que estão em `.github/workflows/` na raiz do repositório**, então hoje eles **não rodam**. Eles já foram
escritos para a raiz (usam `working-directory: games/heart-gold` e filtram por `games/heart-gold/**`); basta movê-los.
A decisão de mover fica para depois, junto com a reorganização do repositório em `games/`.

## Dados originais

Nenhuma ROM, BIOS, firmware ou asset extraído faz parte deste projeto, e o executável não lê nada disso.
O conteúdo do jogo virá de assets próprios (ver [assets/README.md](assets/README.md)). As decompilações não têm
licença; o código delas não é copiado (ver [docs/porting-strategy.md](docs/porting-strategy.md)).

## Registro desta entrega

### Implementado e verificado

Máquina: macOS 26.3, Apple Silicon (arm64), Apple clang 16.0.0, CMake 4.4.2, SDL3 3.4.18 (Homebrew).

1. **Build e testes** (`scripts/build.sh`, sem avisos de compilação com `-Wall -Wextra -Wpedantic`):

   ```
   $ ctest --test-dir build
   1/6 Test #1: engine.viewport ..................   Passed    0.00 sec
   2/6 Test #2: engine.input .....................   Passed    0.00 sec
   3/6 Test #3: engine.fixed_step ................   Passed    0.00 sec
   4/6 Test #4: engine.log .......................   Passed    0.00 sec
   5/6 Test #5: app.headless_smoke ...............   Passed    2.06 sec
   6/6 Test #6: assets.manifest ..................   Passed    0.05 sec
   100% tests passed out of 6
   ```

   `hg_tests` tem 15 casos (viewport nos três layouts, ligações e bordas de entrada, passo fixo, log).

2. **Executável headless** (driver de vídeo dummy, eventos sintéticos):

   ```
   $ build/bin/SuperHeartGold.app/Contents/MacOS/SuperHeartGold --video-driver dummy --frames 120 --self-test
   [    0.089] INFO  SDL 3.4.18, driver de vídeo: dummy
   [    0.277] INFO  Janela 1152x648, renderer: software, vsync
   [    0.278] INFO  Tecla X pressionado -> A
   [    0.278] INFO  Botão b pressionado -> B
   [    2.264] INFO  Encerrando após 120 quadros e 117 passos de lógica
   [    2.264] INFO  Autoteste OK: teclado->A=sim, gamepad->B=sim, saída 1152x648
   ```
   Código de saída 0.

3. **Janela real** (Cocoa + Metal, encerrada sozinha após 60 quadros; código de saída 0):

   ```
   $ SuperHeartGold --frames 60 --self-test
   [    2.008] INFO  Janela 1152x648, renderer: metal, vsync
   [    3.210] INFO  Autoteste OK: teclado->A=sim, gamepad->B=sim, saída 2304x1296
   ```

   Também verificado com `--layout side` (saída Retina 2048x768). Com vsync, 120 quadros levam ≈2 s (60 quadros/s).

4. **`.app` universal** (`scripts/build.sh universal`): 6/6 testes passando;
   `lipo -info` → `x86_64 arm64`; nenhuma dependência de `libSDL3` dinâmica (`otool -L`);
   a fatia x86-64 passou no autoteste sob Rosetta 2 (`arch -x86_64 ... --video-driver dummy --self-test` → `Autoteste OK`).

5. **Validador de assets:** aceita o manifesto vazio e rejeita origem inválida, extensão `.narc` e arquivo fora
   do manifesto (testado com um diretório temporário).

6. **Pesquisa** das duas decompilações com fontes e comandos em `docs/porting-strategy.md`.

### Só planejado (documentado, sem código)

- Todo o jogo: renderizador, áudio, overworld, eventos, batalha, save, dados e as três regiões (`docs/roadmap.md`).
- HUD real do modo unificado (hoje há só um retângulo marcando onde ele fica).
- Arquivo de configuração de controles e vídeo; mouse nos painéis.
- Pipeline de conversão de assets (hoje só a validação do manifesto).

### Falta validar

- **Windows:** nada foi compilado nem executado em Windows. O workflow nunca rodou (e não roda enquanto estiver
  fora da raiz). Possíveis avisos do MSVC com `/W4` ainda não vistos.
- **Mac Intel real:** só testado via Rosetta 2.
- **Gamepad físico:** o caminho de botões foi testado com eventos sintéticos; conexão e desconexão de um controle
  real não foram testadas.
- **Workflow do macOS:** escrito e com YAML válido, mas nunca executado no GitHub.
- Tamanho da vista lógica do modo unificado (384×216) é provisório.
