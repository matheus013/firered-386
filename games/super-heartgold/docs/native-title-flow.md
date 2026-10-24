# Controlador original do título com serviços host

O build privado opcional agora conecta à janela própria duas unidades C da base:

- O `OverlayManager` original completo, que executa init/exec/exit e guarda estado.
- A função `TitleScreen_Main` original, selecionada sem alterações no corpo.

Ambos são compilados diretamente para x64. Não há tradução ARM, interpretador,
SDK Nintendo ou acesso à ROM durante a execução desse teste. Os dados 2D são os
arquivos locais importados na etapa anterior.

## O que está integrado

O caminho de entrada é SDL → bindings → estado de botões → passo fixo → controlador
C → estado de saída e efeitos visuais host. A/Start e as prioridades dos atalhos
são decididos pelo código original. Eventos de tecla não são descartados quando
um frame renderizado não contém passo de simulação.

O teste respeita o atraso inicial de 30 ticks e o timeout após mais de 2.340 ticks
de reprodução. Branco/preto e o ciclo de transições são apresentados pelo renderer.
Os overlays e seus dados têm RAII; são liberados ao concluir ou fechar a janela.
Nenhum destino solicitado é executado automaticamente nesta etapa.

| Parte | Situação |
|---|---|
| Decisão de entrada/saída do título | Função C original compilada para host |
| Ciclo init/exec/exit do gerenciador | Código C original compilado para host |
| Init/exit do título | Callbacks host de alocação e liberação, não os originais completos |
| Camadas 2D | Recursos reais decodificados pelo renderer autoral |
| Fades | Efeito host; duração aproximada, não equivalência ao serviço do DS |
| Áudio | Pedidos contados e timer host; não há reprodução |
| Objetos 3D, câmera, brilho do logo | Não implementados |
| Check-save/menu, apagar save, introdução e microfone | Somente destinos de diagnóstico |

As funções de inicialização/execução/liberação de animação usadas pela decisão
retornam prontidão para o teste 2D já carregado. Não inicializam modelos ou câmera.
Os hooks de câmera e brilho permanecem vazios. Isso permite testar o controlador
isolado, não equivale a portar `title_screen.c` inteiro. A janela identifica
explicitamente que 3D e áudio estão pendentes.

Ao pressionar Enter ou X depois do atraso, o título faz a transição e para em
`MENU REQUESTED - CHECK-SAVE NOT PORTED`. A tela preta nesse ponto é o estado final
do fade, com o destino indicado no rodapé; não é uma partida iniciada.
O atalho original de apagar save apenas seleciona um resultado: nenhum save é
aberto, alterado ou apagado. Fechar e abrir o launcher inicia outro teste.

## Compilação privada

O comportamento padrão de compilação continua sendo a plataforma autoral. Ative
explicitamente `HG_REFERENCE_TITLE_FLOW` em uma pasta de build fora do Git:

```powershell
cmake -S games/super-heartgold -B "D:/builds-privados/hg-title" -A x64 -DHG_REFERENCE_TITLE_FLOW=ON
cmake --build "D:/builds-privados/hg-title" --config Release
ctest --test-dir "D:/builds-privados/hg-title" -C Release --output-on-failure
& "D:/builds-privados/hg-title/bin/Release/SuperHeartGold.exe" --title-resources "D:/dados-privados/hg-titulo" --title-flow
```

O preparador baixa os três fontes pinados apenas na pasta privada, valida SHA-256
e seleciona o corpo de `TitleScreen_Main` e seu enum de estados. O restante do
arquivo de título não é compilado. O wrapper autoral faz a fronteira com o host.
As estruturas do adaptador contêm só os campos usados pela função selecionada;
não são layouts compatíveis com memória DS ou com o módulo de título completo.

| Fonte no pin `6984a3d67592157195940049be2e4bbbf933f9ff` | SHA-256 |
|---|---|
| `src/title_screen.c` | `138232b1af55084d54408888bf53dcf49dce1316584cf214d06826099781b650` |
| `src/overlay_manager.c` | `9fccc34260dd4099b54b463b93cc909dd9c0728b13fce216db92a517d63bc029` |
| `include/overlay_manager.h` | `8ef3461aae09ab92063af9ef29b6094e5b7a3b1f9e1dba83db4aeb068f2762bf` |

Essa saída e seus fontes selecionados ficam fora dos artefatos públicos. A CI
Windows tem um job separado que compila a decisão C e seus testes numa pasta
temporária do runner, sem ROM, sem SDL e sem publicar fontes ou executáveis desse
job. O build
padrão recusa `--title-flow` com uma mensagem explicando a opção necessária.

## Testes e Clean Code

Dois agentes fizeram trabalhos separados: leitor gráfico e controle do título.
O leitor foi dividido em leitura de contêiner, caracteres, paleta, pixels e
composição, com nomes explícitos e constantes. Foram comparados 512 casos contra
a implementação anterior, sem diferença nos pixels; os dez casos gráficos
passaram também sob ASan/UBSan.

O controlador recebeu proprietário RAII do overlay, contexto host com sessão
única, tratamento da fronteira C e testes de cancelamento. Seus testes verificam
A/Start/touch, combinações e prioridades, atraso inicial, timeout exato, sequência
de fades, destino preservado, rejeição de sessão simultânea e zero alocações
pendentes ao concluir ou cancelar. Passaram em Linux com ASan/UBSan e em Windows x64.

`--title-flow-self-test` usa um tick por frame e injeta um evento SDL Enter no frame
40, seguido de key-up. Ao fim exige destino menu concluído e zero alocações do
overlay. É um teste determinístico, não uma medição de velocidade ou latência.
`--capture-at N` captura o frame escolhido, contado a partir de zero. Se o limite
de execução não alcançar esse frame, o programa retorna falha.

Resultados locais e hashes estão em [native-title-validation.json](native-title-validation.json).

O próximo alvo de integração é o destino check-save e seus serviços de dados.
Modelos 3D, áudio e a primeira área jogável continuam pendentes.
