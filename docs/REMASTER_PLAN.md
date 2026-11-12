# Plano de remaster para desktop em Godot — Super FireRed

Rascunho de 12/11/2026. **Decisão do usuário: o remaster é feito em Godot.**

**Quando começar (decisão do usuário, 12/11/2026): depois da expansão Johto/Hoenn.** Nenhuma fase,
nem o G1, começa antes da expansão fechar.

Skills usadas:

- `router`: projeto existente em engine próprio (decomp `pokefirered` em C); frontend novo em Godot 4.7;
- `godot-nodes-scenes`, `godot-tilemap`, `godot-ui-control`, `godot-3d-essentials`, `godot-export`: API e verificação;
- `create-game-assets`: direção de arte, manifesto, QA e procedência;
- `game-ui-ux`: interface em 16:9 e controle;
- `save-systems`: saves, migração e escrita segura;
- `input-systems`, `camera-systems` e `performance-optimization`: entrada, câmera e desempenho.

O objetivo é levar o jogo atual (Kanto + pilotos de Johto/Hoenn, regras, IA, times, PT-BR) para
desktop com **pixel art de alta qualidade** e, depois, **HD-2D/3D**, sem perder o que a hack já tem.

## 0. Antes de tudo: risco de propriedade intelectual

Pokémon pertence à Nintendo, à Game Freak e à The Pokémon Company. Fan remasters com arte nova
e versão de PC distribuída costumam receber DMCA; quanto mais "produto" parecem, maior o risco. O
repositório da hack é **público**.

- Desenvolver o remaster em um **repositório privado** e não publicar builds sem uma decisão
  explícita sua.
- O build GBA e o PC atual continuam como estão: o remaster é um projeto separado.
- Isto não é aconselhamento jurídico. Para distribuir, vale consultar alguém da área.

## 1. Ponto de partida

| Peça | Estado | No remaster |
|---|---|---|
| Engine C (batalha, scripts, IA, mapas, menus) | Completa | Vira o **núcleo** do jogo |
| Port nativo PC (`docs/NATIVE_BUILD.md`) | SDL2, 32 bits, desenha em software a tela do GBA (240×160) | Base do núcleo e da tela de compatibilidade |
| Dados (mapas, tilesets, metatiles, objetos, espécies, textos PT-BR) | Gerados e auditados | Convertidos para recursos Godot |
| Testes de ROM instrumentada (167+ casos) | Funcionando | Regressão da lógica |
| Arte | Original do GBA (16 px) | Referência para o redesenho |

## 2. Arquitetura

```
┌────────────── Godot 4.7 (64 bits) ───────────────┐      ┌──── Núcleo C (32 bits) ─────┐
│ Mundo HD: TileMapLayer (2D) ou GridMap/meshes (3D)│      │ mesma lógica da hack        │
│ Personagens HD, câmera, luz, interface Control    │◄────►│ avança 1 quadro por pedido  │
│ Tela de compatibilidade: quadro do GBA ampliado   │ IPC  │ exporta estado + quadro GBA │
│ Entrada, áudio, opções, saves de configuração     │      │ save do jogo (formato atual)│
└───────────────────────────────────────────────────┘      └─────────────────────────────┘
```

1. **Núcleo em processo separado** (primeira versão):
   - O port nativo ganha um modo sem janela. A cada quadro ele recebe os botões do Godot, roda a
     lógica e devolve um **estado semântico** e o quadro 240×160 do GBA.
   - O estado semântico descreve o que está na tela: mapa e câmera, objetos com sprite e quadro,
     metatiles que mudaram, janela de texto, menu aberto, cursor e dados da batalha.
   - A comunicação é por memória compartilhada ou socket local.
   - **Por que separado:** o núcleo depende de ponteiros de 32 bits (formatos de script e save),
     e o editor do Godot é de 64 bits. O processo separado funciona igual no editor e no jogo
     exportado.
2. **Núcleo dentro do Godot** (depois):
   - Portar o núcleo para 64 bits (ponteiros dos scripts) e carregá-lo como **GDExtension**,
     sem IPC.
   - É uma tarefa de engenharia própria, que só vale a pena quando o resto estiver estável.
3. **Tela de compatibilidade** (o que torna o projeto incremental):
   - Qualquer tela que ainda não tenha versão HD aparece com o quadro original do GBA, ampliado
     em escala inteira e centralizado. Isso vale para batalha, menus, Pokédex e cenas especiais.
   - O jogo inteiro fica jogável no Godot desde o primeiro dia. Cada tela ganha versão HD quando
     ficar pronta.
4. **Dados estáticos convertidos uma vez:**
   - Um conversor (script headless do Godot, como na skill `godot-tilemap`) gera, a partir do
     decomp, os `TileSet .tres` de cada tileset e as cenas de cada mapa.
   - Também gera camadas de dados: comportamento do metatile, elevação, colisão, portas e
     escadas.
   - Em tempo real só trafega o que muda.
5. **Reescrever tudo em GDScript: não.** Seriam anos para refazer batalha, scripts e IA, e a
   hack inteira se perderia.

## 3. Fases

### G0. Decisões e direção (sem código de jogo)
- Respostas da seção 7, brief de arte (`create-game-assets/assets/art-direction-brief.md`) e
  manifesto inicial (`asset-manifest.json`).

### G1. Núcleo + Godot com a tela original
- **Núcleo:** o modo headless do port nativo e o protocolo de quadro (entrada → estado + quadro).
- **Projeto Godot 4.7:**
  - um autoload `Core` que inicia o núcleo e troca dados com ele a cada quadro;
  - a cena principal mostra o quadro do GBA ampliado (nearest, escala inteira) e lê o controle.
- **Verificação:**
  - os testes da ROM continuam passando;
  - `check_godot_project.py` (skill `godot-nodes-scenes`) sai sem erros;
  - um teste headless anda de Pallet até a Route 1 pelo Godot.
- **Pronto quando:** o jogo inteiro roda no Godot, idêntico ao atual.

### G2. Mundo HD em 2D (o primeiro ganho visível)
- **Conversor de mapas:**
  - um `TileMapLayer` por camada (fundo, meio, frente), com o `TileSet` gerado por script;
  - camadas de dados do TileSet para comportamento e elevação.
- **Estado semântico:** câmera, posição e direção do jogador, objetos (NPCs, itens, árvores de
  corte) e mudanças de metatile (portas).
- **Resolução:**
  - lógica 640×360, escala inteira ×3 (1080p) ou ×4 (1440p);
  - `canvas_items` com `aspect=expand`, filtro nearest, sem mipmaps (`game-ui-ux` e gotchas da
    skill de arte).
- **Widescreen:** o núcleo precisa carregar mapa e objetos num raio maior que o da tela do GBA
  (`fieldmap`, `field_camera` e o spawn de objetos). A visão dos treinadores e os scripts que
  movem a câmera precisam ser conferidos.
- **Arte:** começa com a arte original ampliada; a HD substitui por tile e por sprite quando
  existir.
- **Pronto quando:** Kanto inteira anda em 16:9 com a arte original nítida, sem objetos
  sumindo na borda.

### G3. Alvo de arte aprovado
- Pallet Town, jogador e um NPC em HD, vistos na escala real dentro do jogo.
- Seguir o passo "approve one visual target" da skill: produção em massa só depois.

### G4. Interface e batalha HD
- **Telas com `Control`** (`godot-ui-control` + `game-ui-ux`):
  - janela de texto, menu de pausa, bolsa, resumo, grupo;
  - referência 1920×1080, âncoras e containers, área segura, navegação só com controle,
    telas em pilha.
- **Batalha em 16:9:** caixas de HP, menu de golpes e sprites grandes. O núcleo exporta o
  estado; a lógica não muda.
- **Textos:** continuam vindo do núcleo, incluindo o PT-BR. O Godot só decodifica e desenha
  com a fonte HD.

### G5. Produção de arte
Famílias, em ordem:

1. tilesets de Kanto;
2. personagens do mapa (4 direções × 3 quadros);
3. interface;
4. fundos de batalha;
5. **Pokémon (386 × frente, costas e shiny)**, a maior família, em lotes 151 → 251 → 386;
6. pilotos de Johto e Hoenn.

Em todas as famílias:

- QA com `asset_report.py --require-alpha --max-colors` e folhas de contato;
- teste de repetição 3×3 nos tiles e margem de 1–2 px nos atlas;
- procedência por arquivo, registrando inclusive o uso de IA.

### G6. HD-2D / 3D (opcional, mesmo projeto Godot)
- **Mundo 3D gerado dos mesmos dados:**
  - metatiles viram blocos de um `MeshLibrary`/`GridMap`, ou malhas geradas;
  - a elevação vira altura; água, grama e saliências viram materiais e formas.
- **Personagens** em billboard (`Sprite3D`) com a arte HD do G5.
- **Renderizador Forward+** (SSAO, luz, sombras, profundidade de campo).
  `check_3d_scene.gd` (skill `godot-3d-essentials`) no CI.
- **Câmera** inclinada com zona morta e limites do mapa (`camera-systems`).
- **Protótipo:** só Pallet Town. Se ler bem e rodar a 60 fps, o 3D vira modo alternativo ou
  principal.

### G7. Desktop
- **Entrada** (`input-systems`): ações nomeadas, remapeamento com checagem de conflito, controle e
  zona morta.
- **Saves** (`save-systems`):
  - o save do jogo continua no formato atual, para importar `.sav` do GBA;
  - opções do remaster em JSON versionado, com escrita atômica (arquivo temporário, `.bak`,
    renomear).
- **Desempenho** (`performance-optimization`): 60 fps estáveis, medido antes de otimizar; atlas
  por tileset.
- **Export** (`godot-export`):
  - presets Windows e Linux, com o executável do núcleo junto;
  - smoke test headless do `.pck` exportado;
  - macOS só se houver como assinar e testar.

| Fase | Entrega principal | Gargalo |
|---|---|---|
| G0 | Decisões, brief, manifesto | Você |
| G1 | Jogo inteiro rodando no Godot (tela original) | Engenharia (IPC) |
| G2 | Mundo em 16:9 nítido | Engenharia (estado semântico, widescreen) |
| G3 | Alvo de arte aprovado | Arte |
| G4 | Interface e batalha HD | Engenharia + arte |
| G5 | Kanto e 386 Pokémon em HD | **Arte** |
| G6 | Protótipo HD-2D | Engenharia + arte 3D |
| G7 | Builds desktop privados | Testes |

## 4. Quadro técnico da arte 2D

| Item | Proposta |
|---|---|
| Resolução lógica | 640×360 (escala inteira ×3/×4) |
| Tile | 32×32 (o dobro do GBA); visão de 20×11 tiles contra 15×10 no GBA |
| Personagens no mapa | 32×64 |
| Pokémon na batalha | 128×128 ou 192×192 (decidir no G3) |
| Paleta | Papéis fixos (contorno, sombra, destaque, pele) e limite de cores por família |
| Filtragem | Nearest, sem mipmaps, câmera em pixel inteiro |

## 5. Estrutura proposta do projeto Godot

```
remaster/                  (repositório privado)
  project.godot            main scene, autoload Core, stretch canvas_items/expand
  core/                    executável do núcleo + protocolo (README do formato)
  autoload/core.gd         inicia o núcleo, troca entrada/estado por quadro
  tools/convert_*.gd       conversores headless (tilesets, mapas, sprites)
  world2d/  world3d/       cenas do mundo (TileMapLayer / GridMap)
  ui/                      telas Control, tema, fonte
  compat/                  tela de compatibilidade (quadro GBA)
  art/                     arte HD + manifesto + procedência
  tests/                   testes headless (godot --headless --script)
```

## 6. Relação com a expansão Johto/Hoenn

- A expansão continua no GBA e no PC atual: ela produz a lógica e os dados que o Godot vai
  desenhar.
- Decidido: o remaster só começa depois da expansão (G1 e G2 também esperam).
- A produção de arte (G5) faz mais sentido depois que a expansão definir os mapas finais, junto
  com o rebalanceamento geral e o level cap 255 (já no backlog).

## 7. Decisões que preciso de você

1. **Distribuição:** repositório privado e uso pessoal, ou pretende publicar? (Ver seção 0.)
2. **Origem da arte:** desenhar/comissionar, rascunho por IA com limpeza manual, ou misto?
3. **Tile:** 32 px (proposto) ou 48 px (mais detalhe e muito mais arte)?
4. **Visual alvo:** só 2D HD, ou 2D HD agora e HD-2D (G6) como meta?
5. ~~**Começo:**~~ decidido: depois da expansão.
