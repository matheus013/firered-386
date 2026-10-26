# Avaliação de libntr — 2026-10-25

Objetivo: reutilizar a camada de plataforma para manter a lógica HGSS compilada
nativamente. A integração ainda é experimental e não altera o aplicativo atual.

## Referências fixadas

- https://github.com/cybervisi0n/libntr — 380a3d7e60de82aced82075e7b3b9acf5f68b1cd.
  Licença MIT; manter aviso de copyright e licença na distribuição que a incluir.
- https://github.com/cybervisi0n/libntrdwc — 16246ba7b4269e6686f3e4da068e3c996a103739.
  Inspecionada, não incorporada. Não assumir que a licença MIT da libntr cobre
  automaticamente este repositório e seus componentes GameSpy.

As cópias de avaliação ficam fora do Git, no cache privado do WSL. Não houve
modificação dos fontes externos nem envio de contribuições upstream. A política
upstream rejeita PRs geradas por IA; esta avaliação local não produz uma PR.

## Resultados

- libntr completa, incluindo demo standalone: compilação Linux concluída, 309 tarefas.
- Ponte RTC: 36.525 datas válidas, de 2000-01-01 a 2099-12-31, às 23:59:59,
  comparadas com o calendário do nosso inicializador SysInfo: todas iguais.
- A mesma ponte compilada para Windows x64/MinGW e executada: todas iguais.
- Há avisos upstream de redefinição de macros de memória. Não foram ocultados.
- A demo gráfica não foi executada. Não há validação de imagens ou áudio.
- A biblioteca inteira ainda não foi compilada para Windows nesta avaliação.

## Reprodução

Instalar Meson, Ninja, NASM, Flex, Bison, compiladores C/C++, SDL2 e ENet.
Com uma cópia limpa da libntr na revisão acima:

```sh
meson setup /tmp/libntr-build /caminho/libntr -Dbuild_target=linux -Dstandalone=true -Dcard_game_code=IPGE -Dcard_maker_code=01
meson compile -C /tmp/libntr-build -j4
python3 tools/verify_libntr_rtc.py --source /caminho/libntr
python3 tools/verify_libntr_rtc.py --source /caminho/libntr --target windows --sdl-include /caminho/mingw/include
```

O teste exige o pin e fontes rastreados sem alterações. A variante Windows exige
MinGW x64 e execução Windows (no WSL, interop habilitado). Os objetos e executáveis
são temporários. Somente código de teste autoral fica neste repositório.

## Próximas integrações

1. Compilar a camada completa para Windows e testar a demo 3D com término limitado.
2. Integrar serviços atrás de uma opção de build, mantendo SDL2 da libntr isolado
   da interface SDL3 atual enquanto a posse da janela é definida.
3. Validar estruturas de 32/64 bits e diferenças de SDK entre Platinum e HGSS.
4. Resolver a procedência/licença dos componentes DWC antes de distribuí-los;
   depois comparar o serviço de conta com os testes ARM9 existentes.
5. Usar a camada para introdução e primeira área, sem contabilizar a compilação
   da biblioteca como campanha portada.

## Execução Linux e requisito single screen

Teste posterior em Ubuntu/WSL2 com WSLg, Mesa llvmpipe e OpenGL 4.5:
a demo original abortou em calculateClipMatrix. O memcpy copiava 64 bytes
para um registro global escalar de quatro bytes; os outros registros são
objetos independentes. O patch local tools/patches/libntr-clip-matrix.patch
atribui cada um dos 16 valores ao registro correspondente, sem desativar as
proteções do compilador. Após recompilar, GDB contou 120 chamadas a
SDL_GL_SwapWindow sem crash. O depurador encerrou o processo nesse limite.
Isso prova avanço do ciclo gráfico; ainda não é inspeção visual do cubo,
teste de encerramento normal, áudio ou execução de HGSS.

Single screen é requisito obrigatório da integração: preservar as adaptações
SINGLE_SCREEN da base ClickingMaster e apresentar exploração, diálogos,
menus, bolsa, equipe e comandos de batalha numa única tela, com controle
utilizável sem depender de uma segunda tela. A demo upstream não comprova
esse requisito. Não substituir a base HGSS pelo jogo Platinum.

A cópia externa agora contém apenas a correção local documentada. O teste RTC
que exige checkout limpo deve usar outra cópia limpa no mesmo pin.

## Captura visual e fechamento

A ferramenta autoral `tools/probe_libntr_frames.c` intercepta a apresentação
SDL2 no Linux, captura o framebuffer nos quadros 60 e 120 e envia o evento de
fechar a janela. Compilar como biblioteca compartilhada com gcc -shared -fPIC,
-I/usr/include/SDL2 -lSDL2 -ldl; executar a demo com LD_PRELOAD apontando para ela
em um diretório temporário, sob timeout -k 3s 20s.

Os quadros de 512x768 foram inspecionados: figura branca 3D sobre fundo preto,
com movimento entre os quadros (diferença no retângulo 179,99–341,263).
O evento SDL_WINDOWEVENT_CLOSE encerrou com código 0. O primeiro experimento
com SDL_QUIT foi ignorado pelo loop upstream e precisou ser encerrado; isso
fica registrado como limitação, não como teste de fechamento aprovado.

O upstream só oferece layouts vertical, horizontal e large. Nenhum deles
implementa o single screen solicitado. Preservar SINGLE_SCREEN nos fontes
HGSS e adaptar composição, viewport e coordenadas de entrada será necessário.
A imagem da demo confirma somente a base 3D, não menus ou batalhas de HGSS.

## Protótipo de apresentação em uma tela

O patch local `tools/patches/libntr-single-screen-prototype.patch` acrescenta
ScreenLayout=single ao parser e serializador. Usa um quad com seis vértices,
aspecto 256:192 e janela inicial 512x384. SwapScreens seleciona a outra metade
da textura composta. A tela superior não deve encaminhar cliques à tela touch
oculta; a inferior usa o viewport inteiro. Essa política de entrada ainda não
foi validada com eventos e não substitui a navegação por controle de HGSS.

Testes Linux: tela superior capturada em 512x384, geometria visível e movimento
entre quadros 60/120; seleção inferior preta como esperado para esta demo.
Ambas as execuções encerraram com código zero. O resultado superior foi
inspecionado visualmente. Os patches aplicam-se ao pin registrado; nenhum
fonte externo é incorporado ao aplicativo de produção nesta etapa.

Este é um protótipo de apresentação de uma tela física, não a integração dos
menus SINGLE_SCREEN de HGSS. Ainda faltam conteúdo do jogo, seleção automática
quando necessária, coordenação com a interface e testes de entrada, redimensionamento
e alta densidade de pixels. A opção ainda é configurada pelo arquivo INI.

## Mapeamento de entrada isolado

O protótipo agora utiliza `tools/reference_compat/single_screen_touch.h`.
Ao aplicar o patch externo, copiar esse header autoral para
`include/simulator/hg_single_screen_touch.h` na cópia da libntr.
O mapeamento usa limites exclusivos: x=256 e y=192 nunca são produzidos.
Tela oculta, botão solto, margens e dimensões vazias não produzem toque.
Intermediários de 64 bits evitam overflow na subtração das coordenadas.

`tools/probe_single_screen_touch.cpp` cobre 196608 posições visíveis e as mesmas
posições com tela oculta, além de bordas, margens, botão solto, dimensão zero,
extremos inteiros e viewport de um pixel. Passou em Linux com ASan/UBSan e em
Windows/MinGW. A demo recompilada apresentou 120 quadros e encerrou com código
zero. Esses testes verificam o mapeamento isolado; ainda falta injetar eventos
reais de entrada e conferir os registros touch durante a execução do jogo.

```sh
g++ -std=c++20 -fsanitize=address,undefined tools/probe_single_screen_touch.cpp -o /tmp/hg-touch-test
/tmp/hg-touch-test
```

## Eventos touch durante a execução

`tools/probe_libntr_input.c` injeta botão pressionado no quadro 10, solto no 30
e fechamento no 50 usando SDL_PushEvent. SDL_GetMouseState é substituído por
posição determinística (256,192); portanto este teste não usa um mouse físico.
`tools/probe_libntr_input_gdb.py` observa s_tpData após o mapeamento no loop
real, localizando o ponto de observação no fonte em vez de fixar a linha.

Resultados Linux: tela inferior visível, quadro 20 touch=1 e (128,96); quadro
40 touch=0. Tela inferior oculta: touch=0 nos dois pontos. Ambas as execuções
encerraram normalmente. Não houve alteração do código de produção nesta rodada.

Para repetir: compilar o interpositor com gcc -shared -fPIC e SDL2/dl, definir
ScreenLayout=single e SwapScreens conforme o caso; iniciar GDB batch com
HG_LIBNTR_SOURCE apontando para o checkout e HG_EXPECT_TOUCH=0 ou 1. Dentro do
GDB, definir LD_PRELOAD para input.so e carregar probe_libntr_input_gdb.py via
-x. Usar timeout -k 3s 30s como limite externo. Não aplicar LD_PRELOAD ao GDB.
O teste exige símbolos de depuração. Controle físico e menus HGSS permanecem
fora do escopo validado.

## Ponte com funções originais HGSS

O global.h da base SINGLE_SCREEN mantém o motor principal na tela física
inferior. SingleScreen_ShowSubEngine modifica o bit DSEL de POWCNT para levar
o motor secundário à mesma tela. Portanto a integração HGSS deve apresentar
a tela inferior; mostrar apenas a superior da demo não reproduz o contrato.
SwapScreens do frontend e troca de motor do jogo são operações diferentes.

`tools/verify_hgss_libntr_display.py` seleciona as funções originais de
src/gf_gfx_planes.c depois de conferir seu SHA-256 e as compila contra headers
libntr. O código original gerado permanece temporário, fora do repositório.
O teste usa armazenamento escalar para POWCNT e percorre os 65536 estados:
ShowSubEngine preserva todos os outros bits, IsSubEngineShown reflete DSEL,
e os campos twoScreenScene, subEngineShown e battleMenu são atualizados.
Linux ASan/UBSan passou. HandleSwapKey e efeitos sonoros não são incluídos.

Reprodução:
```sh
python3 tools/verify_hgss_libntr_display.py --hgss /caminho/hgss --libntr /caminho/libntr --build /caminho/libntr-build
```

Esta prova cobre compatibilidade de headers e lógica de roteamento. Ainda
falta ligar essas funções ao renderer real, conferir a composição dos dois
motores e tratar a introdução que solicita twoScreenScene sem perder conteúdo.
A campanha e seus menus ainda não executam no novo backend.

## Roteamento HGSS no renderer real

A demo privada agora liga SingleScreen_ShowSubEngine e os metadados originais
HGSS ao registrador POWCNT real da libntr. O teste escalar anterior é separado.
Com ScreenLayout=single e SwapScreens=true, a janela 512x384 mostra a tela
física inferior. O motor principal desenha a figura branca 3D; o secundário
recebe uma cor de fundo azul pela API GXS_LoadBGPltt.

Primeira execução: quadro 60 com figura, quadro 120 totalmente azul após troca
no quadro lógico 80. Segunda: troca para secundário no quadro lógico 40 e
retorno ao principal no 80; capturas 60/120 confirmaram azul e figura branca,
respectivamente. Ambas encerraram normalmente. A imagem azul foi inspecionada;
os valores RGB e dimensões também foram conferidos programaticamente.

`tools/prepare_hgss_display_demo.py` reproduz as duas unidades C da segunda
execução, conferindo o hash HGSS e lendo a demo da revisão libntr fixada. Exige
uma pasta de saída nova e não sobrescreve a dependência. Ligar main.c e
hgss_display.c a nitrosdk_dep no build privado standalone. Os fontes gerados
foram comparados com os efetivamente compilados: idênticos após normalizar
quebras de linha. Nenhum fonte HGSS gerado foi incluído no Git.

O caso valida troca e retorno de motor numa tela física, não conteúdo de jogo.
Ainda faltam inicialização completa, overlays, recursos e menus de HGSS.

## Comando único de integração Linux

`tools/verify_hgss_renderer.py` prepara uma pasta privada nova, clona a revisão
libntr fixada, aplica os dois patches versionados, instala o helper de touch,
gera as unidades originais privadas e compila a demo completa. Depois captura
os quadros 60 e 120, exige 512x384, verifica o secundário azul e o retorno da
geometria principal e exige saída normal. Falhas de build, execução, dimensões
ou conteúdo fazem o comando falhar. Resultados ficam em result.json; fontes,
binários e capturas permanecem na pasta privada, fora do Git.

```sh
python3 tools/verify_hgss_renderer.py   --hgss /caminho/referencia-hgss   --libntr /caminho/libntr   --workspace /caminho/privado/novo-teste
```

Requer Linux com display OpenGL (WSLg foi o ambiente usado), SDL2, ENet,
GCC/G++, Meson, Ninja, NASM, Flex e Bison. O destino deve ser inexistente.
As fontes de entrada não são modificadas. O teste ainda é uma demo de
roteamento gráfico, não inicialização de jogo, menus ou campanha.

## Compilação de módulos completos

Avaliação com libntrsystem 97fa995ed33fe2ab8c83ec7068aa3aa5b4c3e12f,
libntr no pin acima e HGSS original: 12 de 17 unidades C completas compilaram.
Inclui main, heap, sys_task, overlay_manager, gf_gfx_planes, bg_window,
pokemon, save_arrays, gf_rtc, render_text, font e palette.

A ferramenta `tools/probe_hgss_modules.py` reproduz a avaliação com argumentos
--hgss, --libntr, --libntrsystem, --build e --output (pasta nova).
Usa uma ponte MSL para headers C padrão, um include agregador nitro/fx.h e
alias OS_IE_VBLANK=OS_IE_V_BLANK. Não altera os fontes originais.
O resultado detalhado está em hgss-module-compilation.json; logs completos
ficam na pasta privada. O comando registra falhas esperadas por módulo.

Bloqueios: task tem conflitos de tipos de animação/material e fog; save exige
CARD_BACKUP_TYPE_FLASH_4MBITS_EX; system exige alias HBlank; message_format e
msgdata precisam do header gerado msgdata/msg.naix. Não substituir constantes
de backup ou tipos 3D por valores arbitrários apenas para compilar.

Todos os objetos geraram avisos de casts de ponteiros nos headers NNS, e
bg_window também possui casts próprios de endereço para u32. Logo, compilar
não prova segurança de ABI 64-bit. Ainda não foi feito link desses módulos
nem execução da inicialização completa. libntrsystem permanece externo;
licenciamento e procedência precisam de revisão antes de redistribuição.

## Segunda rodada de compatibilidade de módulos

14/17 módulos agora compilam. task e system foram desbloqueados. A ferramenta
copia os headers HGSS para o diretório privado de saída, remove as declarações
redundantes de NNSG3dMatAnmResult, NNSG3dFuncAnmMat e G3X_SetFogTable e usa as
definições dos SDKs nativos. Isso evita inventar layouts de structs: a declaração
HGSS usava uma tag de struct diferente daquela definida por libntrsystem, e a
assinatura de fog omitia const. O alias HBlank acompanha o alias VBlank.
Cada declaração removida deve ocorrer exatamente uma vez; divergência falha.
Os fontes e headers de entrada permanecem intactos.

Restam save (constante de tipo de flash), message_format e msgdata (header de
índices de mensagens ausente). Os avisos de ponteiro continuam visíveis e não
foram suprimidos. Não houve link ou validação de execução desses 14 módulos.

## Terceira rodada: 17 módulos e execução do agendador

Em 2026-10-26, os 17 módulos selecionados compilaram integralmente com GCC
no Linux x86-64. O conjunto avaliado é limitado: não representa todos os
fontes do jogo. Save, message_format e msgdata foram desbloqueados.

A constante CARD_BACKUP_TYPE_FLASH_4MBITS_EX é 0x00ff1302, conferida no
literal ARM9 em 0x02028750; o fallback em 0x02028754 é 0x00001302.
O vendor 255 corresponde à variante IRC do SDK, com capacidade de 524288
bytes. `verify_hgss_backup_constant.py` exige o SHA-256 da imagem ARM9
conhecida antes de conferir os valores. A ponte de compilação não testa
leitura, escrita, persistência ou recuperação de save.

`prepare_hgss_message_headers.py` gera headers privados para os dois módulos
de mensagens. Usa GMM do commit pret fixado pelo próprio fork e confere os
índices de cada uma das oito tabelas contra a contagem no arquivo NARC local
SoulSilver USA. Exige o hash desse arquivo, que contém 829 membros. Os 46
identificadores de bancos usados pelos módulos seguem a numeração dos bancos
originais. O relatório de hashes e contagens está em
`hgss-message-header-provenance.json`. Nenhum texto, recurso ou ROM é
incluído no repositório. Isso ainda não valida renderização das mensagens.

```sh
python3 tools/prepare_hgss_message_headers.py --hgss "$HGSS" --archive "$MSG_NARC" --output "$PRIVATE_HEADERS"
python3 tools/probe_hgss_modules.py --hgss "$HGSS" --libntr "$LIBNTR" --libntrsystem "$LIBNTRSYSTEM" --build "$SDK_BUILD" --message-headers "$PRIVATE_HEADERS" --verify-scheduler --require-all --output "$NEW_PROBE"
python3 tools/probe_hgss_link_dependencies.py --objects "$NEW_PROBE" --sdk-build "$SDK_BUILD" --output "$NEW_LINK_PROBE"
```

As variáveis representam caminhos locais; os três destinos de saída devem
ser pastas novas. A geração dos headers consulta os arquivos públicos no
GitHub. O NARC permanece local.

O link relocável dos 17 objetos passou sem conflitos de símbolos. Permanecem
434 referências externas: 162 têm possíveis fornecedores nos arquivos do
SDK compilado; 272 não têm fornecedor nesse conjunto. O inventário em
`hgss-link-dependencies.json` separa referências por módulo. Essas contagens
não são uma estimativa de trabalho restante, e encontrar um símbolo no SDK
não prova compatibilidade de assinatura, ABI ou comportamento.

O módulo original completo sys_task.c também foi compilado e executado com
AddressSanitizer e UndefinedBehaviorSanitizer. Passaram prioridade, ordem
FIFO em prioridades iguais, fila cheia, remoção da próxima tarefa, criação
durante callback com execução adiada e 10000 ciclos de criação/autorremoção.
A arena do teste é zerada com calloc, como pré-condição explícita do ensaio.
O teste usa ponteiros nativos; não executa código ARM nem substitui a lógica
do agendador por uma implementação de teste.

Continuam pendentes o link executável do jogo, os avisos de ponteiros dos
headers NNS, a inicialização completa, os recursos, os overlays e a campanha.
O teste single screen anterior permanece separado destes testes de módulos.

## Ampliação para 27 módulos e correções de configuração nativa

A avaliação foi ampliada com filesystem, pm_string, brightness, poke_overlay,
timer3, touchpad, swap_3d_buffers, math_util, igt_counter e
string_control_code. Os 27 compilam e passam pelo link relocável.
Os relatórios JSON de compilação e dependências agora correspondem a esse
conjunto ampliado. A contagem atual é de 418 referências externas, das quais
191 têm candidatos no SDK e 227 não têm. Ela substitui a contagem da rodada
anterior; ainda não é um link executável.

**Correção importante do probe:** PM_KEEP_ASSERTS passa a ser definido.
HGSS coloca chamadas com efeitos necessários dentro de GF_ASSERT, incluindo
operações de overlays. Compilar sem essa macro removia também essas chamadas.
As rodadas anteriores eram insuficientes para representar o comportamento
completo do jogo. A nova rodada preserva essas expressões; o teste de fila
também verifica explicitamente que uma expressão de GF_ASSERT é executada.

Timer3_GetTicks calculava um endereço a partir de TM0, reduzia o ponteiro
para u32 e o dereferenciava. Em libntr os registradores são variáveis nativas
separadas, portanto não se pode assumir a disposição contígua do hardware DS.
O probe gera uma cópia privada de timer3.c e troca somente essa expressão
por reg_OS_TM3CNT_L. Exige exatamente uma ocorrência da expressão original
e preserva a referência de entrada. O restante do algoritmo não muda.

`--verify-timer` compila essa cópia com ASan/UBSan e verifica todos os 65536
valores baixos do contador, com e sem IRQ pendente, totalizando 131072 casos,
além da restauração de interrupções e conversão para segundos. Registradores
e funções de seção crítica são fornecidos pelo teste; não se validam aqui
precisão em tempo real, entrega de IRQ ou inicialização do temporizador no SDK.

Para reproduzir a rodada ampliada, adicionar ao comando do probe:

```sh
--verify-timer --extra-modules filesystem pm_string brightness poke_overlay timer3 touchpad swap_3d_buffers math_util igt_counter string_control_code
```

O aviso de truncamento próprio de timer3 foi eliminado. Permanecem os avisos
NNS, os de bg_window e o de HW_DTCM usado em poke_overlay; esse último exige
tratamento antes de executar o carregador de overlays nativamente.

## Build conjunto dos SDKs

`tools/build_hgss_sdk_stack.py` agora reproduz o build Linux x86-64 de libntr
e libntrsystem juntos. Clona as revisões fixadas para uma pasta privada nova,
aplica os patches de clip matrix e single screen, instala o helper de touch
e configura os dois como subprojetos Meson. O projeto temporário recebe um
commit local porque o gerador de metadados do SDK exige um HEAD válido.
Nenhuma alteração é feita nas referências de entrada ou no Git do usuário.

```sh
python3 tools/build_hgss_sdk_stack.py --libntr "$LIBNTR" --libntrsystem "$LIBNTRSYSTEM" --output "$NEW_SDK_STACK"
python3 tools/probe_hgss_link_dependencies.py --objects "$NEW_PROBE" --sdk-build "$NEW_SDK_STACK/build" --output "$NEW_DEPENDENCY_REPORT"
```

A reprodução limpa passou: foram geradas as bibliotecas fnd, g2d, g3d, gfd,
mcs e snd de NitroSystem, além das bibliotecas de libntr. O relatório está
em `hgss-sdk-stack-build.json`. Os arquivos .a gerados pelo Meson são thin
archives e dependem dos objetos na árvore de build; não são um pacote para
redistribuir isoladamente. Os hashes registrados identificam o resultado
local, não garantem builds idênticos em diretórios ou toolchains diferentes.

O inventário atualizado mantém 418 referências externas nos 27 objetos,
com candidatos para 199 e sem fornecedor para 219. Oito referências adicionais
encontraram símbolos em libntrsystem. A existência das bibliotecas não valida
execução, compatibilidade de ABI, qualidade dos algoritmos ou todos os seus
subsistemas. Não houve build Windows nesta rodada, nem avanço de campanha.
As questões de procedência/licença de libntrsystem continuam pendentes antes
de redistribuição; os fontes e binários avaliados permanecem privados.
