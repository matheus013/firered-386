# Estratégia de port

Este documento registra a investigação das decompilações de Pokémon HeartGold/SoulSilver
e a decisão de como usá-las. Levantamento feito em **23/10/2026**; os números mudam a cada commit
desses repositórios, então refaça as consultas (seção [Fontes](#fontes)) antes de depender deles.

**Resumo:** o Super HeartGold é uma **reimplementação nativa em C++20**. As decompilações
(pret e, desde 23/10/2026, [pokeheartgold-single-screen](#6-existe-outra-decompilação-ou-port-com-licença) no lugar do ctrl-cheeb-del)
servem como **referência de comportamento** (fórmulas, ordem de eventos, temporização, formatos),
não como código a ser copiado ou compilado no produto. Nenhuma das duas compila para Windows
ou macOS como está, e nenhuma tem licença que permita redistribuição.

## 1. Os dois repositórios

| | [pret/pokeheartgold](https://github.com/pret/pokeheartgold) | [ctrl-cheeb-del/pokeheartgold](https://github.com/ctrl-cheeb-del/pokeheartgold) |
|---|---|---|
| O que é | Decompilação comunitária original, do grupo pret | Derivado do pret ("fork" no README, mas não marcado como fork no GitHub), **sem afiliação ao pret** |
| Como é feito | Manual, revisado pela comunidade | Declarado no README como feito "com agentes LLM"; o pret não aceita esse código e pede que não seja enviado para lá |
| Estado declarado | "WIP disassembly", aviso de que mudar tamanho/layout de funções pode quebrar a ROM | "Fully decompiled", as duas ROMs (HG e SS, US) saem byte a byte idênticas |
| Progresso medido | **32,59 % do código** e **29,19 % dos dados** em C (calcrom no CI, ver abaixo) | **99,25 %** do código mapeado em C/C++ (97,87 % C + 1,38 % C++), 0,64 % assembly original (`docs/progress/progress.json`) |
| Arquivos (árvore do `master`) | `src/`: 406 `.c`; `asm/`: 283 `.s` (≈20,4 MB de texto) | `src/`: 1.012 `.c` + 56 `.cpp`; `asm/`: 80 `.s`; `lib/`: 531 `.c` |
| Último push | 19/10/2026 | 23/10/2026 |
| Licença | **Nenhuma** (sem arquivo LICENSE; API do GitHub retorna `license: null`) | **Nenhuma** (idem) |
| Compilador | Metrowerks CodeWarrior `mwccarm` 2.0/sp2p2 (proprietário, rodando via Wine) | O mesmo; o script instala Wine 11.16 em `.local-tools/` |
| SDK | NitroSDK proprietário (`tools/bin` do SDK precisa ser fornecido pelo usuário) | O mesmo |

### Como os percentuais foram obtidos

- **pret:** log do job `build` do workflow `build.yml`, execução
  [39249751161](https://github.com/pret/pokeheartgold/actions/runs/39249751161) (master, 19/10/2026), ação `calcrom`:
  `978696 bytes of code in src (32.59%)` de `3003112 total bytes of code`;
  `117669 bytes of data in src (29.19%)` de `403161`.
- **ctrl-cheeb-del:** `docs/progress/progress.json` (schema 3, commit `a02a63f`), métrica
  "bytes de código mapeados classificados por linguagem de origem", denominador de 4.075.354 bytes (ARM9 + ARM7,
  incluindo bibliotecas). O próprio arquivo marca `tracked_worktree_dirty: true`.

As métricas **não são comparáveis diretamente** (denominadores diferentes: o calcrom do pret não conta as
bibliotecas do SDK do mesmo jeito). O que dá para afirmar: o pret está em torno de um terço em C; o
ctrl-cheeb-del afirma ter quase tudo em C, com verificação por hash da ROM. **Não compilamos nenhum dos dois**
(exigem compilador e SDK proprietários que não temos nem devemos obter para este projeto), então os números
são os publicados por eles, não verificados aqui.

### Ressalvas sobre o ctrl-cheeb-del

- "Bate byte a byte" garante que o binário é igual, não que nomes, tipos e estrutura estejam corretos ou
  legíveis. Código gerado por LLM que casa com o binário pode ter nomes enganosos.
- Por isso, quando os dois divergirem em nomes ou interpretação, **o pret é a referência preferida** para
  entender intenção; o ctrl-cheeb-del é útil para cobrir as partes que o pret ainda tem só em assembly.
- Os documentos `docs/DECOMP-NOTES.md` dele registram cada lote decompilado, o que ajuda a rastrear origem.

## 2. Licença e restrições de redistribuição

- **Nenhum dos dois repositórios tem licença.** Sem licença, vale o padrão do direito autoral: todos os
  direitos ficam com os autores. Não há permissão para copiar e redistribuir o código deles.
- Além disso, o código decompilado é uma tradução do código original da Game Freak/Nintendo; o direito
  sobre o programa e os dados originais continua deles.
- Ferramentas de terceiros dentro dos repositórios têm licenças próprias (`tools/jsonproc/nlohmann/LICENSE.MIT`,
  `tools/msgenc/LICENSE.pugixml.md`, `tools/nitroarc/COPYING`), mas não precisamos delas.
- NitroSDK e `mwccarm` são proprietários; os próprios INSTALL.md dizem que não podem ser distribuídos.

**Regras deste projeto, derivadas disso:**

1. Não copiar código das decompilações para `games/heart-gold/`. Ler para entender, escrever do zero.
2. Não distribuir ROM, BIOS, firmware nem dados extraídos (gráficos, músicas, textos, mapas).
3. Nenhum artefato do produto pode depender de ROM ou emulador para rodar.
4. O conteúdo do jogo precisa vir de um pipeline de assets próprios (ver `assets/README.md`).

> Isto é uma leitura técnica das restrições, não parecer jurídico. Um jogo de fã com a marca Pokémon tem
> risco de propriedade intelectual independentemente de como o código é escrito.

## 3. Dependências da arquitetura do Nintendo DS

| Aspecto | No DS / nas decompilações | Consequência para um port nativo |
|---|---|---|
| CPUs | ARM9 (ARM946E-S, 67 MHz) roda o jogo; ARM7 (ARM7TDMI, 33 MHz) cuida de som, toque, RTC e rádio. No repo, o ARM7 é o binário `sub/` (7 `.s`, `ichneumon_sub`), que vem do SDK | Só a lógica do ARM9 interessa. O papel do ARM7 é substituído por SDL3 (áudio, entrada, relógio) |
| Overlays | O código é dividido em dezenas de overlays carregados sob demanda (`main.lsf`, `include/*/overlay_XX_*.h`; o pret tem 173 arquivos `asm/overlay*`) porque a RAM era de 4 MB | No PC tudo fica residente. Overlays viram módulos normais; o gerenciador de overlays deixa de existir |
| Gráficos 2D | Dois motores 2D (A e B), camadas de BG por tiles, sprites OAM, paletas, bancos de VRAM, efeitos de janela/blend | Reimplementar um compositor de camadas (CPU ou shader) que gere duas imagens 256×192 |
| Gráficos 3D | Motor 3D com FIFO de geometria (GX/G3), modelos NSBMD, texturas NSBTX, animações NSBCA/NSBTA/NSBTP. O overworld de HG/SS é 3D (mapas e prédios) com personagens em sprite | Reimplementar o subconjunto usado sobre uma API moderna (SDL_GPU: Metal/D3D12/Vulkan), já em resolução maior |
| NitroSDK | OS (threads, IRQ, heaps), FS, GX/G2/G3, SND, PAD, TP (toque), RTC, CARD (save), WM/WFC (rede) | Uma camada de plataforma própria substitui cada módulo; nada do SDK entra |
| Sistema de arquivos | NitroFS com arquivos NARC (arquivos de arquivos indexados por número) em `files/`; o pret lista ~70 caminhos `.narc`/`.sdat` | Formato próprio com nomes legíveis; NARC só seria lido por uma ferramenta offline opcional, nunca pelo jogo |
| Áudio | SDAT: sequências SSEQ (tipo MIDI), bancos SBNK, ondas SWAR, streams STRM, sequenciador no ARM7 | Precisa de sequenciador + sintetizador próprio ou áudio pré-renderizado, sempre com assets próprios |
| Memória | Endereços fixos, heaps com IDs do jogo, ponteiros de 32 bits, structs com layout do compilador MW | Em 64 bits os tamanhos mudam; save e dados nunca podem ser gravados com `memcpy` de struct |
| Compilador | `mwccarm` com extensões e comportamento específico (inline asm, alinhamentos) | O código não compila em Clang/MSVC sem retrabalho, mesmo o que já está em C |

## 4. O que é reaproveitável, por subsistema

"Reaproveitável" aqui significa **conhecimento**, não código.

| Subsistema | Reaproveitável (como referência) | Precisa ser novo |
|---|---|---|
| Gráficos | Ordem de camadas, prioridades, timing de animações, transições de tela, layout das telas de menu e batalha, câmera do overworld | Todo o renderizador, formatos de imagem/modelo, arte (própria) |
| Áudio | Quando cada música/efeito toca, prioridades, volumes, fades | Mixer, sequenciador, sintetizador, músicas e efeitos (próprios) |
| Entrada | Semântica de botões (A confirma, B cancela, Y registrado, toque na tela de baixo), repetição de cursor | Tradução teclado/gamepad/mouse, remapeamento, perfis de controle |
| Arquivos | Estrutura lógica dos dados (espécies, golpes, itens, encontros, treinadores, scripts de evento) | Formatos de disco, carregador, empacotamento e o próprio conteúdo |
| Memória/save | Quais campos o save guarda, regras de validação | Serialização explícita e versionada, independente de arquitetura |
| Lógica de jogo | Fórmulas de dano, efeitos de golpes, IA, crescimento, encontros, RNG, scripts de evento | Código C++ escrito do zero e testado contra os comportamentos documentados |

## 5. Port versus reimplementação

| Critério | Port (compilar a decomp com uma camada que imita o NitroSDK) | Reimplementação (escolhida) |
|---|---|---|
| Fidelidade inicial | Alta | Depende de testes de comportamento |
| Redistribuição | **Impossível**: o produto conteria código sem licença derivado do original | Possível com código e assets próprios |
| Esforço técnico | Emular 2D/3D do DS, SDAT, overlays, ponteiros de 32 bits, extensões do MWCC; o pret ainda tem ~2/3 em assembly | Escrever engine e jogo, mas sem arrastar restrições do hardware |
| Três regiões e 24 insígnias | Exigiria editar código preso ao layout do jogo original | O desenho já parte de várias regiões com dados separados |
| Widescreen, escala, controles | Remendos sobre um renderer que pressupõe 256×192 | Parte do desenho desde o início |

**Decisão:** reimplementar. Manter as mecânicas originais significa reproduzir o comportamento, com testes
unitários que codificam os resultados esperados (por exemplo, a fórmula de dano com entradas conhecidas).

## 6. Existe outra decompilação ou port com licença?

Busca feita em 23/10/2026 (`gh search repos` por pokeheartgold, pokeplatinum, pokediamond e ports para PC).

| Repositório | O que é | Licença declarada | Precisa de ROM? |
|---|---|---|---|
| [pret/pokeplatinum](https://github.com/pret/pokeplatinum), [pret/pokediamond](https://github.com/pret/pokediamond) | Decompilações de Platinum e Diamond/Pearl (mesma engine da Gen IV) | **Nenhuma** | Sim, para os assets |
| [ClickingMaster/pokeheartgold-single-screen](https://github.com/ClickingMaster/pokeheartgold-single-screen) | Fork do ctrl-cheeb-del com modo `SINGLE_SCREEN`: batalha, bolsa e equipe desenhadas na tela de cima; feito "com uso pesado de IA" | MIT | Sim (ROM SoulSilver US + mwccarm + NitroSDK) |
| [ClickingMaster/ssrecomp](https://github.com/ClickingMaster/ssrecomp) | Recompilação estática do ARM9 para C++, rodando sobre o núcleo do melonDS; 16:9 e controle. Descontinuado em favor do anterior | GPL-3.0 | Sim |
| [AKCore/pokeplatinum-port](https://github.com/AKCore/pokeplatinum-port) | Fork do pret/pokeplatinum que compila o C da decomp como programa nativo (Linux, Windows, ARM, Android, 3DS), com modelos de hardware do DS derivados do melonDS | GPL-3.0 | Sim (lê o sistema de arquivos da ROM) |
| [GabeSud0/pokeplatinum-3ds](https://github.com/GabeSud0/pokeplatinum-3ds) | Port nativo de Platinum para 3DS a partir do pret; diz que HG/SS é o alvo futuro | GPL-3.0 | Sim |
| [CVGisBored/PokeHeartGold_PC](https://github.com/CVGisBored/PokeHeartGold_PC) | Port nativo de HG em C++ com carregadores de NARC/NSBMD/SDAT, feito com IA | Nenhuma | Sim (`prepare_rom.sh`) |

**Conclusão: não existe atalho com licença limpa.**

- A licença de um fork (MIT ou GPL) só cobre o que **os autores do fork** escreveram. Ela não pode relicenciar o
  código do pret (que não tem licença) nem o código original da Game Freak/Nintendo que a decomp traduz.
  Os próprios READMEs dizem que não contêm "código nem dados do jogo".
- Todos dependem da ROM do jogador para ter conteúdo. Isso contraria a decisão deste projeto de recriar todo o
  conteúdo e não depender de ROM.
- O código realmente licenciável deles é a parte de plataforma (frontends, modelos de hardware derivados do melonDS),
  útil para um caminho de port/emulação, que não é o nosso. Usar qualquer trecho GPL obrigaria o projeto inteiro
  a ser GPL-3.0.

**Referência adotada:** o `pokeheartgold-single-screen` **substitui o ctrl-cheeb-del como decomp de referência**.
É um fork dele (mesma decompilação completa) e acrescenta o modo de tela única, que é exatamente o problema do
nosso HUD. Continua valendo para ele o mesmo que para o ctrl-cheeb-del: só **leitura** de comportamento, sem copiar
código, e o pret como referência preferida de nomes e intenção. Ressalvas: o modo de tela única só foi testado com
SoulSilver (US), e quase todas as mudanças foram escritas por IA, segundo o próprio README.

Ele **não** serve como base de código do projeto: o MIT não cobre o código decompilado que herda, e o build exige
ROM, `mwccarm` e NitroSDK, o que contraria as decisões de recriar todo o conteúdo e não depender de ROM.

**O que dá para aproveitar sem copiar código:** o `pokeheartgold-single-screen` e o `ssrecomp` resolveram o mesmo
problema de interface que o nosso M1 (comandos de batalha, bolsa e equipe numa tela só, em 16:9, com controle).
Seus READMEs e capturas de tela servem de referência de design para o HUD.

**O caminho alternativo, registrado para decisão futura:** esses projetos mostram que um HG/SS nativo,
100% fiel e jogável **com a ROM do jogador** já é viável (o `pokeplatinum-port` e o `pokeplatinum-3ds` declaram
Platinum jogável). Seria muito mais rápido que recriar todo o conteúdo, mas inverte duas regras daqui:
o jogo passaria a depender da ROM e herdaria código sem licença. Hoje a decisão é **recriar tudo**.

## 7. Fontes

- https://github.com/pret/pokeheartgold — README.md, INSTALL.md, árvore do `master`, `.github/workflows/build.yml`,
  `.github/actions/calcrom/action.yml`.
- https://github.com/pret/pokeheartgold/actions/runs/39249751161 — log com a saída do calcrom.
- https://github.com/ctrl-cheeb-del/pokeheartgold — README.md, INSTALL.md, `docs/DECOMP-NOTES.md`,
  `docs/progress/progress.json`, árvore do `master`.
- API do GitHub (`gh api repos/<dono>/pokeheartgold`) para licença, data de push e se é fork.

Comandos usados (repita para atualizar os números):

```sh
gh api repos/pret/pokeheartgold --jq '{license: .license.spdx_id, pushed_at, fork}'
gh run list -R pret/pokeheartgold --workflow build.yml --branch master --status success -L 1
gh run view <id> -R pret/pokeheartgold --log | grep 'bytes of'
gh api repos/ctrl-cheeb-del/pokeheartgold/contents/docs/progress/progress.json --jq .content | base64 -d | jq .coverage
```
