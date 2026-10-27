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

## Memória nativa: erro reproduzido e corrigido

O primeiro teste executável de heap.c com o alocador expansível real de
libntrsystem falhou no UBSan: NNSiFndHeapHead foi acessado num endereço
alinhado a 4 bytes, mas a estrutura exige 8 bytes no host de 64 bits.
Compilar os módulos não revelava esse erro de execução.

`prepare_hgss_heap.py` faz adaptações verificadas em cópias privadas:

- heap.c solicita alinhamento de sizeof(void*) para a tabela de heaps,
  arenas e alocações nas duas direções. O prefixo de 16 bytes dos blocos
  permanece igual; formatos persistentes não são alterados.
- expheap.c usa sizeof(void*) como alinhamento mínimo de cabeçalhos e
  arredondamento dos blocos. Pedidos legados de +4/-4 são elevados ao mínimo
  nativo, preservando a direção. O ajuste também vale para consultas de
  tamanho alocável e para a criação de arenas inicialmente desalinhadas.

O build conjunto dos SDKs aplica a mesma adaptação em sua cópia privada de
libntrsystem. O hash do expheap.c resultante aparece no relatório de build.

O teste `--verify-heap` compila heap.c adaptado, pm_string.c inalterado e os
fontes reais heapcommon.c, list.c e expheap.c. Passaram, com ASan/UBSan e
falha imediata em comportamento indefinido:

- 32000 alocações/liberações alternadas pelo início e fim de heaps filhos;
- preservação de conteúdo em blocos de tamanhos diferentes, exaustão,
  destruição dos filhos e recuperação integral do espaço livre;
- 1000 ciclos de criação, cópia, concatenação e liberação de strings;
- 1024 casos diretos do SDK, com alinhamentos ±4/±8/±16/±32, tamanhos de
  0 a 127 e uma arena inicial deslocada em quatro bytes;
- redução de bloco e recuperação do espaço após liberação.

O resultado está em `hgss-heap-native-test.json`. Arena do sistema, seção
crítica e limpeza de recursos gráficos são fronteiras controladas do teste.
Nenhum recurso gráfico é registrado nesse ensaio. Não se validam concorrência,
liberação de assets gráficos nem o heap completo dentro do jogo.

Adicionar `--verify-heap` ao probe reproduz a validação Linux. Para gerar
também o teste Windows, adicionar `--build-windows-heap` e
`--windows-sdl-include /caminho/headers-windows`. Exige MinGW e gera
heap-test.exe na pasta privada; a execução no Windows é uma verificação
separada. A versão Windows desse ensaio não usa ASan/UBSan.

O heap-test.exe compilado com MinGW foi executado diretamente no Windows
x86-64 e terminou com código 0, passando os mesmos casos de memória e strings.
O relatório `hgss-heap-windows-test.json` registra o hash do executável privado.
SDL_MAIN_HANDLED evita que o header SDL redirecione o main do teste; LTO
elimina as funções não exercitadas antes do link COFF. Não foram acrescentadas
implementações fictícias de fontes ou RNG para satisfazer funções não usadas.
Este resultado valida esse conjunto de módulos, não um executável do jogo.

Na revisão de overlays, confirmou-se que o FS_LoadOverlayInfo do SDK ainda
interpreta a tabela de overlays da ROM e FS_LoadOverlay carrega a imagem
correspondente. Isso não fornece automaticamente um carregador de funções
nativas do jogo. Corrigir apenas o cast de HW_DTCM seria insuficiente:
o port precisa definir a ligação e o ciclo de vida de cada overlay nativo.

## Identidade e ABI dos overlays

Foi identificado outro bloqueio: sob SDK_PORT, libntr define FS_OVERLAY_ID
como zero para qualquer nome. Em main.c isso confundia intro_title (60) com
OVY_36 (36), usado para continuar o jogo. O probe agora gera um header
privado com os 129 IDs do main.lsf original, cujo hash é obrigatório.
`prepare_hgss_overlay_ids.py` segue a numeração sequencial usada por
makelcf (`t->id = list->num++`) e confere todos os nomes OVY_N contra o
índice N. Todos os 129 valores são verificados por static assertions na
compilação, incluindo os overlays com nomes sem número.

O teste também mediu FSOverlayInfoHeader no host: 48 bytes, ram_address no
offset 8 e file_id no offset 40. O formato da ROM tem 32 bytes, com esses
campos nos offsets 4 e 24. Portanto, copiar a tabela da ROM diretamente para
essa estrutura nativa não é válido. O relatório `hgss-overlay-contract.json`
registra essa incompatibilidade; preservar os IDs não resolve o carregamento
nem torna os ponteiros ARM utilizáveis pelo executável nativo.

Após essas mudanças, os 27 módulos continuam compilando e os testes de
agendador, temporizador e memória passaram novamente no Linux. O teste de
memória Windows foi executado antes da inclusão desse header de IDs; ele
não usa overlays. Nenhuma dessas validações equivale a iniciar a campanha.

## Gerenciador de aplicações com memória e tela única

O ensaio de memória foi ampliado para compilar também overlay_manager.c e
gf_gfx_planes.c completos e inalterados. Executa 1000 ciclos de uma aplicação
de teste registrada no carregador nativo, com init em duas chamadas,
exec em três e exit em duas. Confere passagem de argumentos, dados alocados
com alinhamento nativo, preservação de um valor de 64 bits e liberação dos
dados e do gerenciador ao terminar.

Cada ciclo começa com o motor secundário selecionado. O gerenciador original
deve retornar ao principal antes de chamar init, preservando os demais bits
do registrador gráfico. O callback de execução seleciona o secundário outra
vez, permitindo conferir a mesma restauração na próxima aplicação.

Passou no Linux com ASan/UBSan e no executável MinGW rodando diretamente no
Windows (código 0). Os dois relatórios de heap foram atualizados para incluir
esses 1000 ciclos; essa rodada Windows também usa o header de IDs corrigidos.
Os callbacks são do teste e o registrador gráfico é controlado. O ensaio
valida a interação do código original com a memória nativa e o controle
single screen, mas não renderização de cenas, inicialização do menu ou gameplay.

## Registro de módulos nativos

`game/native` fornece um registro C de módulos já ligados ao executável,
com estados de carga/descarga, rejeição de IDs duplicados e controle de
reentrada. O teste CMake `game.native_overlays` cobre falha de carregamento,
nova tentativa, identidade e fechamento com módulos ativos.

A ponte `hgss_native_overlay_bridge.c` conecta esse registro ao gerenciador
original. Os três modos de carga são síncronos para módulos já ligados;
não se carregam imagens ARM da ROM. Um ID ausente encerra o processo com
diagnóstico antes de executar callbacks da aplicação. Esse caminho negativo
foi validado no Linux. Os mil ciclos positivos passaram com ASan/UBSan no
Linux e em processo nativo Windows, incluindo alocação/liberação de recurso
do módulo e retorno à tela principal.

O ID sintético 0x7ffffffe pertence apenas ao teste. Nenhum overlay real do
jogo foi registrado: este carregador ainda não está integrado ao launcher.
A vinculação requer registro inativo e armazenamento válido até a desvinculação;
o uso é de uma única thread, sem suporte a callbacks que lançam exceções.

## Compilação da introdução e suporte gráfico

A rodada `hgss-intro-compilation.json` amplia de 27 para 47 módulos C
compilados no Linux. Inclui intro_movie, sys_task_api, screen_fade,
gf_3d_render, camera, gf_gfx_loader, obj_char_transfer, obj_pltt_transfer,
oam_manager, sprite, intro_movie_scenes_lib, gf_g2d_renderer, g2d_cell_culling,
screen_fade_effects, screen_window_wipe e as cinco intro_movie_scene_1 a
intro_movie_scene_5. O probe acrescenta o diretório original include/library
para resolver os headers de partículas.

`prepare_hgss_graphics.py` verifica hashes dos fontes antes de adaptar cópias
privadas: mapingType passa a mappingType, nome fornecido pelo SDK; a estrutura
opaca de sete palavras usada pelo gerenciador OAM passa a usar o tipo
NNSG2dOamManagerInstance, com os protótipos reais do SDK. Esses dois módulos
foram compilados, mas ainda não executados nesta integração. O adaptador também
substitui o header interno antigo do renderer pelo header público do SDK e
remove a definição local duplicada de GX_IsHBlank, utilizando a do SDK.

O link relocável dos 47 módulos passou. O relatório separado
`hgss-intro-link-dependencies.json` registra 588 referências externas, das
quais 325 possuem candidatos nos SDKs e 263 ainda não têm provedor nesse
conjunto. O aumento em relação à rodada de 27 módulos corresponde à ampliação
do código analisado. Não houve link de executável completo nem execução da
introdução; avisos sobre ponteiros de tamanhos diferentes continuam pendentes.

Para reproduzir esse conjunto, usar `--intro-support --require-all` no probe,
os mesmos caminhos de SDKs e headers de mensagens, `--opening-headers` com
o diretório gerado abaixo e `--extra-modules intro_movie_scene_1
intro_movie_scene_2 intro_movie_scene_3 intro_movie_scene_4 intro_movie_scene_5`.
SINGLE_SCREEN permanece definido pelo global.h da base original.

`prepare_hgss_opening_headers.py --hgss <base> --archive <arquivo> --output <novo-diretorio>`
gera demo/opening/gs_opening.naix após validar o hash do arquivo privado
a/2/6/2 da SoulSilver USA. Verifica as 99 referências das cenas contra os
106 membros: índice, compressão LZ10 e igualdade byte a byte do recurso
descomprimido com o recurso extraído localmente. Não precisa reconstruir
o arquivo usando a regra gs_opening.mk ausente. O relatório público
`hgss-opening-indices.json` contém somente índices, hashes e resultados;
nenhum recurso original é incluído no repositório.

O teste `tools.opening_headers` usa dados sintéticos para conferir literais,
referências sobrepostas e rejeição de streams truncados ou referências fora
dos limites. A verificação dos recursos reais foi executada separadamente
na cópia privada. Compilar as cenas não valida sua renderização nem o áudio.

O inventário consulta apenas os objetos selecionados e os arquivos estáticos
dos SDKs; a lista sem provedor também contém símbolos do runtime C e do linker
(por exemplo memset e __stack_chk_fail). Portanto, sua contagem não equivale
ao número de funções do jogo a implementar.

## Ponte de partículas e recursos de sprites

A rodada `hgss-particles-compilation.json` reúne 54 unidades compiladas:
as 47 anteriores, sprite_transfer, sprite_resource, sprite_resource_header,
gf_3d_vramman, vram_transfer_manager, gf_3d_loader e particle_system.
Adicionar esses sete nomes a `--extra-modules` reproduz o conjunto.

O adaptador de particle_system verifica o hash do fonte e usa o header público
gfd.h. Nas quatro operações de estado de VRAM, cria uma estrutura SDK local
e copia os bytes entre ela e o armazenamento original; uma static assertion
confere a capacidade. Assim não converte o array em um ponteiro incompatível.
Os 16 alocadores SPL passam de alinhamento 4 para sizeof(void*), com uintptr_t
na operação de alinhamento. As estruturas SPL de runtime contêm ponteiros.

`--verify-particles` executa os próprios alocadores estáticos da unidade
adaptada em arenas controladas. Passaram 2032 casos (16 slots, tamanhos 1–127),
com ASan/UBSan, verificando alinhamento, escrita de uintptr_t e preservação dos
dados da alocação anterior. Resultado: hgss-particle-allocator-test.json.
Não foram testados esgotamento da arena, estados de VRAM em execução,
carregamento SPL ou renderização. As funções não usadas são descartadas no link.

O link relocável dos 54 módulos passou: 600 referências externas, 348 com
candidatos no SDK e 252 fora desse conjunto. O relatório é
hgss-particles-link-dependencies.json. A biblioteca SPL continua pendente;
compilar a ponte particle_system não significa que seus emissores executam.

## Biblioteca SPL em avaliação privada

A rodada hgss-spl-compilation.json acrescenta os nove fontes SPL HGSS,
totalizando 63 módulos compilados. Sua origem e o teste de inicialização
estão descritos em native-missing-parts-references.md. Não houve alteração
do código externo; os hashes usados estão no relatório de compilação.

O link relocável dos 63 módulos passou. hgss-spl-link-dependencies.json
registra 617 referências externas: 379 com candidatos nos SDKs, 238 sem
provedor nesse conjunto. O número de referências externas pode crescer ao
adicionar código, mesmo resolvendo dependências da camada anterior.
Não foi ligado um executável completo. Os testes SPL cobrem apenas pools
e listas, no Linux com ASan/UBSan; o launcher Windows não foi alterado.

## Carregamento do recurso SPL da abertura

O ensaio foi ampliado para chamar SPL_Load com o membro 4 do arquivo privado
a/0/5/9, exatamente o recurso selecionado por intro_movie_scene_4.c.
O arquivo NARC é identificado por SHA256 antes da extração local. O teste
carregou 9 definições de emissores e 9 texturas, com ASan/UBSan e sem falhas.
Confere contagens do cabeçalho e limites dos ponteiros base. Os 100 ciclos
anteriores de pools/listas também passaram nessa execução.

Reprodução: acrescentar `--spl-resource-archive <a/0/5/9>` e
`--spl-fx-library <libfx.a>` a `--verify-spl --spl-sources <fontes>`.
Usa spl_field.c original, matemática nativa do SDK e sim_vars.c original.
As fronteiras controladas são malloc, preenchimento/cópia de bytes,
armazenamento da semente RNG e um handler de assert que aborta.
A biblioteca matemática pré-compilada não está instrumentada; os fontes SPL
e o ensaio estão. O RNG e os callbacks de campos não são executados.

Resultado: hgss-spl-resource-test.json. O recurso fica fora do Git.
Este teste não cobre arquivos malformados, todos os recursos do jogo,
atualização dos emissores, upload de texturas/paletas ou renderização.

## Simulação dos emissores da abertura

`--verify-spl-update`, junto das opções do teste de recurso SPL, cria um
emissor por definição do recurso e executa SPL_Calc por 600 quadros antes de
SPL_DeleteAll. Passaram as 9 definições (5400 atualizações), com pico de 150
partículas. O ensaio confere listas e conservação dos pools de 20 emissores
e 200 partículas durante a simulação e a recuperação integral após excluir.
Resultado em hgss-spl-update-test.json; fontes/hashes em
hgss-spl-update-compilation.json. RNG reinicializado em 0x12345678 por definição.

O primeiro ensaio UBSan detectou deslocamento à esquerda de -2071 em
spl_generate.c, no cálculo de velocidade angular. A cópia privada passa a usar
`rtt_min * 4096`: rtt_min é s16, portanto o produto cabe em s32 e preserva
o valor esperado sem deslocar um inteiro negativo. A execução corrigida passou
com ASan/UBSan. Os fontes de referência permanecem intactos; o probe registra
hashes originais e adaptados. Essa validação não compara quadro a quadro com ARM.

A constante gSplAxisY vem de lib/asm/spl_data.s do commit 01eeeb4b da referência:
três valores de 16 bits, 0, 0x1000 e 0. A biblioteca matemática do SDK continua
pré-compilada sem sanitizers. Não há upload de texturas nem desenho neste teste.

## Contrato de upload SPL

`--verify-spl-upload`, junto do teste de recurso, executa as funções originais
SPL_LoadTexByCallbackFunction e SPL_LoadTexPlttByCallbackFunction. As chamadas
GX são interceptadas por uma fronteira de gravação em memória limitada,
com validação de origem dentro do recurso, destino dentro da reserva,
conteúdo copiado e sequência Begin/Load/End. Esta fronteira não representa
o alocador VRAM real, suas restrições de formato, bancos ou DMA.

No recurso da abertura passaram 9 chamadas de textura e 9 de paleta,
reservando 11840 e 88 bytes respectivamente. O teste também confere que os
endereços armazenados nas 9 texturas ficam dentro das reservas. A mesma
execução passou os 100 ciclos de pools e as 5400 atualizações de emissores
com ASan/UBSan. Relatório: hgss-spl-upload-test.json.

Não foi testado upload pela implementação GX do SDK, OpenGL, paletas
compartilhadas de outros recursos ou qualquer imagem renderizada. O próximo
passo continua sendo substituir essa fronteira pelo caminho gráfico real.

## Endereçamento nativo dos uploads diretos GX

A inspeção do SDK encontrou um defeito adicional nos caminhos Ex:
GX_LoadTexEx truncava os endereços da tabela nativa para u32 e deslocava-os
12 bits; GX_LoadTexPlttEx mantinha u64, mas ainda deslocava o endereço.
As tabelas SDK_PORT já contêm ponteiros completos, ao contrário das tabelas DS.

prepare_sdk_texture_upload.py verifica o hash do fonte e adapta somente esses
endereços no ramo SDK_PORT, mantendo o ramo DS. O build conjunto dos SDKs
aplica a correção à sua cópia privada e registra o hash resultante.

verify_sdk_texture_upload.py extrai as duas funções do fonte original e da
cópia adaptada para um teste com estado de bancos e cópias controlados.
O original falhou na verificação do endereço de destino; a versão adaptada
passou quatro casos: banco único, fronteira entre bancos, segundo banco e
paleta. Os endereços são superiores a UINT32_MAX e ASan/UBSan ficam ativos.
Resultado em hgss-sdk-direct-upload-test.json.

Reprodução: `verify_sdk_texture_upload.py --source <libntr/libraries/gx/src/gx_load3d.c>
--output <novo-diretorio>`. É um teste das funções extraídas, não execução do
driver completo, DMA, OpenGL ou apresentação na tela. O caminho SPL usual
Begin/Load/End já trata esses endereços de forma diferente; essa descoberta
não demonstra falha desse caminho nem conclui sua integração gráfica.

## Execução dos uploads com unidades completas do SDK

`verify_sdk_upload_runtime.py --build <sdk-stack/build> --output <novo-diretorio>`
recompila seis unidades completas do SDK com ASan/UBSan, usando os parâmetros
do build Meson e seu fonte privado adaptado. Não extrai funções nem substitui
as cópias GX/MI. O teste usa o modo CPU do SDK (`GX_DMA_NOT_USE`).

Foram verificadas as 15 combinações de bancos A/B/C/D, tanto pelo caminho
Begin/Load/End quanto pelo upload Ex: comparação de todos os bytes transferidos,
preservação integral dos bancos não selecionados e restauração da seleção.
Também passaram uma transferência com deslocamento cruzando A/C e uma paleta
no banco E. Resultado e hashes em `hgss-sdk-upload-runtime-test.json`.

O ambiente é headless, com globais de plataforma fornecidas pelo teste.
Assertions, término e callbacks de DMA abortam se acionados. A configuração
SDK_FINALROM é a mesma do build existente; portanto, não valida as verificações
internas de exclusividade desabilitadas nessa configuração. DMA, OpenGL,
desenho de partículas e apresentação em tela única continuam sem validação
por este teste. O jogo completo ainda não inicia.

## Comandos de desenho SPL no port nativo

`spl_draw_ptcl.c` escrevia diretamente nos registradores 3D. No SDK nativo,
essas escritas não enviam as mensagens necessárias ao renderizador.
`prepare_spl_drawing.py` verifica o hash do fonte privado e converte os acessos
para chamadas da API gráfica: coordenadas de textura, vértices, início/fim de
primitivas, identidade, escala, translação, cor e atributos de polígonos.
O probe de módulos aplica essa adaptação automaticamente à cópia privada.

Os vértices preservam o arredondamento VTX10 do DS antes de serem enviados como
VTX16; enviar os valores sem essa conversão alteraria sua precisão.
`verify_spl_draw_commands.py` reproduz a ausência de comandos no original e
verifica o adaptado contra um dispatcher que grava as mensagens do SDK.
Passaram os planos XY/XZ, todos os 65.536 valores de coordenadas de 16 bits,
escala e atributos, com ASan/UBSan. Resultado:
`hgss-spl-draw-commands-test.json`. A unidade completa compila, mas esse teste
executa somente os helpers e planos, não todos os tipos de partículas.

O teste gráfico `verify_hgss_renderer.py` aceita `--spl-source <spl_draw_ptcl.c>`
e `--spl-probe <probe-com-headers>` para substituir o cubo pelo plano XY SPL.
A cena usa orientação fixa e nenhuma face descartada para não ocultar o plano
durante a captura. A execução em WSLg verifica a janela de 512×384, a troca para
o motor secundário azul e a volta ao plano no motor principal usando as funções
originais de roteamento do HGSS. Não carrega texturas da ROM nem executa um
emissor completo; integrar esses dois caminhos continua pendente.

O build limpo desse teste terminou com sucesso; resultado em
`hgss-spl-plane-render-test.json`. A captura final foi inspecionada: plano branco
centralizado sobre fundo preto, sem uma segunda tela empilhada. Esse resultado
valida o plano XY sem textura, não a abertura completa nem o executável Windows.

Para repetir o teste de comandos:

```text
python3 tools/verify_spl_draw_commands.py --source <spl_draw_ptcl.c-original> \
  --probe <probe-com-headers> --sdk <libntr> --sdk-build <build-libntr> \
  --output <novo-diretorio-privado>
```

## Emissor da abertura com upload e desenho reais

Após a busca registrada em `native-missing-parts-references.md`, o teste
`verify_hgss_renderer.py` ganhou `--spl-resource <intro-particles.spa>` e
`--libntrsystem <checkout>` além das opções SPL anteriores. O SHA256 exigido
para o SPA é `748eded76a142bcf2247df73ed54bfec6e24d15d48d2c1c9b825cf1c061c916e`:
o membro 4 do arquivo privado a/0/5/9, já validado nos testes de recursos.

O executável diagnóstico liga os nove fontes SPL privados e usa os uploads
GX reais, sem gravadores ou cópias simuladas no lugar dessas funções. As tabelas
de planos e rotações seguem `spl_data.s` da revisão HGSS consultada. O harness
fornece alocação de memória e alocadores de VRAM limitados aos bancos A/E; aborta
para texturas comprimidas, que exigiriam uma política adicional de alocação.

O recurso de emissor 0 é criado com seed fixa, atualizado e desenhado usando
uma câmera diagnóstica. Como o efeito termina rapidamente, uma segunda criação
no frame 100 permite capturá-lo no frame 120. O frame 60 verifica o motor
secundário azul e o frame 120 verifica partículas após retornar à tela principal.
Uma segunda execução desativa somente `SPL_Draw`; sua captura final deve ser
inteiramente preta. O áudio usa o driver dummy do SDL e não é validado.

Esse teste não executa a cena original da abertura, não verifica os demais oito
recursos de emissor, não valida a fidelidade contra um DS e não testa Windows.
ROM, SPA, capturas com recursos do jogo e binários permanecem fora do Git.

Resultado do build limpo: `hgss-spl-emitter-render-test.json`, aprovado em
2026-10-27. A captura final apresentou 29.722 pixels não pretos e o controle
sem desenho ficou preto. O log registrou pico de 150 partículas, upload de
11.840 bytes de textura e 132 bytes de espaço de paleta, incluindo alinhamento
de 16 bytes do alocador diagnóstico. O conteúdo da captura foi inspecionado.
