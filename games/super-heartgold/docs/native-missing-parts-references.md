# Referências para dependências nativas

Pesquisa de 2026-10-26. A base single screen permanece a mesma. Nenhum dos
fontes externos abaixo foi incorporado ao produto nesta rodada.

## Partículas: candidato prioritário

[ctrl-cheeb-del/pokeheartgold, spl_manager.c](https://github.com/ctrl-cheeb-del/pokeheartgold/blob/01eeeb4b96087cb94c84de3045e9f2d4ef00061b/lib/src/spl_manager.c)
contém implementações de SPL_Init, SPL_Load, SPL_Calc, SPL_Draw, criação e
exclusão de emissores, além do envio de texturas e paletas. Os nomes e a
assinatura de SPL_Init correspondem ao header SPL da nossa base HGSS.
Isso o torna o primeiro candidato para avaliação, não uma integração validada.
O arquivo também depende de rotinas spl_calc, spl_init, spl_draw e listas.

Próxima verificação: obter o conjunto de fontes SPL desse commit em pasta
privada; comparar os headers com a base; compilar com o SDK nativo; testar
alinhamento e recursos reais com ASan/UBSan antes de integrar a abertura.
A API do GitHub não identificou licença de repositório; a proveniência e
as permissões dos arquivos continuam pendentes para redistribuição.

## Referência de adaptação para outro hardware

[IbrahimIrfan/pspoke, build.py de partículas](https://github.com/IbrahimIrfan/pspoke/blob/7477648fbe904a27eef8ba56bcc11c1799475c1a/port/soulsilver-native-particles/build.py)
adapta a biblioteca SPL de Platinum para SoulSilver, mapeando nomes de funções.
O código contém tratamentos para divisão por zero, exclusão de emissor nulo
e diferenças de flags dos recursos. São casos para nossa matriz de testes;
o comportamento comentado pelo autor ainda precisa de confirmação local.

O Makefile desse port usa SDK_BUILD_PSP e suprime avisos de tipos incompatíveis.
Não é uma solução pronta para Windows x86-64; não copiar packing ou supressões
para contornar problemas de layout. O repositório é identificado como GPL-3.0
pela API do GitHub.

## Implementação alternativa para comparação

[pret/pokeplatinum, spl_manager.c](https://github.com/pret/pokeplatinum/blob/6eeb1b04c6433761f3049811b7e1383619c59743/lib/spl/src/spl_manager.c)
fornece a biblioteca usada como referência pelo port PSP. Sua API usa nomes
como SPLManager_New e tipos de argumentos diferentes dos headers HGSS.
É útil para comparação de algoritmos; a compatibilidade binária dos recursos
e estruturas não deve ser presumida. A API do GitHub não identificou licença
global para esse repositório.

## Resultado local desta investigação

Na extensão do ensaio de 47 módulos, compilaram também sprite_transfer,
sprite_resource, sprite_resource_header, gf_3d_vramman, vram_transfer_manager
e gf_3d_loader: 53 unidades bem-sucedidas entre 54 solicitadas. Não houve
execução desses seis módulos nem novo link completo nesta rodada.

particle_system falhou inicialmente pela ausência de nnsys/gfd_internal.h.
Uma experiência privada usando o header público revelou quatro erros de tipos
nas chamadas de estado de VRAM e 34 avisos sobre tamanhos de ponteiros.
Trocar apenas o include é insuficiente; a adaptação experimental foi retirada
do gerador público. É necessário resolver armazenamento tipado dos estados e
os acessos que pressupõem ponteiros de 32 bits antes de registrar esse módulo
como portado. O relatório está em hgss-sprite-investigation.json.

Rodada posterior: a ponte particle_system passou a compilar com cópias
tipadas dos estados de VRAM e alinhamento nativo nos alocadores. Os 2032
casos de alocação passaram com ASan/UBSan. Isso não integra a biblioteca SPL
externa; consultar a seção de partículas em libntr-integration.md e os novos
relatórios hgss-particles-* para o estado atual.

## Avaliação privada da SPL HGSS

Os nove fontes lib/src/spl_*.c do commit 01eeeb4b96087cb94c84de3045e9f2d4ef00061b
de ctrl-cheeb-del/pokeheartgold foram obtidos em pasta privada e compilados
sem adaptações com os headers da base single screen. O probe aceita
`--spl-sources <diretorio>` e registra o SHA256 de cada fonte usado.
Os fontes externos não foram adicionados ao repositório nem ao launcher.

`--verify-spl` testa SPL_Init e as rotinas originais de listas, com malloc
e preenchimento de bytes como fronteiras controladas. Passaram 100 ciclos
com pools de 20 emissores e 200 partículas, verificando contagem, alinhamento,
ligações anterior/próxima, remoção intermediária/final e esvaziamento.
ASan/UBSan estavam ativos. O relatório é hgss-spl-runtime-test.json.

Esse teste ainda não carrega arquivos SPA, não atualiza emissores e não
desenha partículas; validar o layout dos recursos e a renderização permanece
necessário antes de usar a biblioteca na abertura real.

## Busca antes de implementar — 2026-10-27

Por orientação do usuário, novas partes do port devem começar com uma busca
no GitHub. Registrar a referência, revisão, compatibilidade, licença e o que
foi realmente validado; reutilizar componentes adequados antes de criar outra
implementação. Resultado vazio não prova que nenhuma solução exista.

Nesta rodada, buscas por `SPL_Draw SDK_PORT`, `SPL_Draw` no pspoke,
`SPLManager_Draw` no pokeplatinum e repositórios `heartgold native` retornaram:

- [pspoke: teste de partículas](https://github.com/IbrahimIrfan/pspoke/blob/7477648fbe904a27eef8ba56bcc11c1799475c1a/port/soulsilver-native-particles/main.c):
  testa pools e atualização; o main consultado não chama o desenho. Seu glue
  oferece a sequência load/upload/update/draw, mas pressupõe tamanhos de
  estruturas e ponteiros de 32 bits. Referência GPL-3.0 já identificada na
  investigação anterior; nenhum fonte desse teste foi incorporado.
- [HGSS: tabelas SPL](https://github.com/ctrl-cheeb-del/pokeheartgold/blob/01eeeb4b96087cb94c84de3045e9f2d4ef00061b/lib/asm/spl_data.s):
  confirma a ordem XY/XZ dos planos e Y/XYZ das rotações, eixo Y em ponto fixo
  e seed inicial zero. Essas definições conectam os nove fontes privados
  existentes; não foi necessário reimplementar a biblioteca de partículas.
- [Platinum: interface SPL](https://github.com/pret/pokeplatinum/blob/6eeb1b04c6433761f3049811b7e1383619c59743/lib/spl/include/spl_manager.h):
  confirma a separação entre upload, atualização e desenho com matriz de câmera.
  Mantida como referência, sem substituir os tipos HGSS já validados.
- [ApricotGS](https://github.com/BaZZtian/PS5-ApricotGS): README declara versão
  experimental PS5, preparação estática local, ROMs alemãs revision-0 e layout
  de duas telas; campanha completa não verificada pelo projeto. Seus avisos
  identificam componentes MIT/GPL/LGPL e dependências PS5/Vulkan. Não é um
  substituto Windows/single screen pronto; implementação interna e portabilidade
  ainda não avaliadas. Nenhum componente foi importado.
- [HGSS macOS Engine](https://github.com/dylanfernandezdelara/hgss-macos-engine):
  README descreve scaffold Swift e player de uma abertura extraída, sem port
  direto do código DS. API do GitHub não identificou licença. Não adotado como
  engine do projeto.

Decisão: integrar os emissores HGSS existentes ao SDK e ao teste de tela única,
usando as tabelas confirmadas na referência. Testes anteriores de SPA e comandos
estão em `libntr-integration.md`; as limitações dos testes históricos acima não
descrevem sozinhas o estado atual da integração.
