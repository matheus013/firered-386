# Mínimo jogável nativo Windows — levantamento de 01/11/2026

## Resultado

O projeto ainda não tem uma área jogável integrada. A abertura cinematográfica
completa não é requisito técnico para o primeiro marco jogável. Recomenda-se
priorizar uma fatia real do jogo: inicializar uma partida, explorar os mapas
iniciais, conversar/receber um Pokémon, concluir uma batalha e salvar/retomar
no Windows, mantendo single screen. Essa proposta não implementa atalhos nem
altera o fluxo final do jogo; reordena o trabalho necessário para demonstrá-lo.

## Dois critérios diferentes

- **Primeiro controle jogável:** executável Windows nativo, estado válido,
  quarto inicial renderizado, personagem controlável, colisão, diálogo e
  transição pela escada/porta. Ainda não representa uma versão com batalha/save.
- **Mínimo jogável completo:** além do anterior, receber o inicial por evento,
  acessar a primeira área de encontros pelo caminho normal, terminar uma
  batalha, voltar ao mapa e salvar/reabrir preservando o progresso.

Campanha completa, pós-jogo e fidelidade integral da apresentação são marcos
posteriores. Nenhum percentual dos componentes gráficos mede esses critérios.

## Dependências e ordem recomendada

| Ordem | Entrega indispensável | Evidência atual | Falta demonstrar / aceite |
|---|---|---|---|
| 1 | Um único runtime Windows | Launcher autoral SDL3 e componentes Windows; diagnósticos libntr/SDL2 em Linux | Consolidar a rota escolhida de engine original + libntr, compilar/linkar para Windows e executar recursos privados, entrada e troca single screen. Não presumir que os dois protótipos já se integram. |
| 2 | Partida válida e transição de aplicações | Scheduler/overlay manager e inicializadores isolados; documentação registra 41/42 inicializadores comparados | Resolver o inicializador restante, integrar os 42 na ordem, RNG/RTC/identidade, perfil e configuração pós-Oak. Nenhum retorno de sucesso vazio para simular inicialização. |
| 3 | Primeiro mapa | Recursos, 2D e partículas isolados; nenhum mapa integrado | FieldSystem, matriz/mapa, modelos/texturas/câmera, jogador/NPCs, eventos, colisão e movimento. Andar no quarto original e usar a escada. Renderizar partículas não comprova renderização do mundo. |
| 4 | Scripts e interação | Mensagens/fontes e impressão parcial com testes | Interpretador e comandos usados pelo trecho inicial, variáveis/flags, diálogos, escolhas, movimentos comandados, mudanças de mapa e entrega do inicial. Evento deve terminar sem travar ou repetir indevidamente. |
| 5 | Batalha e retorno | Fontes originais disponíveis; sem batalha nativa integrada validada | Encontro, equipe/dados, controlador/comandos, seleção de golpe/alvo, HP/PP/status, resultado e retorno ao FieldSystem. Vitória e derrota não podem deixar o loop preso; alterações devem voltar ao estado do jogo. |
| 6 | Save/load integrado | Leitura, CRC e seleção dos blocos principais testados; SaveContext parcial | Serializar/gravar estado real com layout explícito, preservar banco válido diante de falha, carregar via Continue. Fechar o processo e confirmar mapa, posição, flags, equipe, HP/PP e itens. |

O build Windows deve ser verificado cedo e novamente em cada marco, em vez de
ficar para depois de terminar a integração Linux. Single screen é requisito em
todos os passos: interface e toque precisam acompanhar o motor exibido, sem
menus inacessíveis na tela oculta.

## Caminho original encontrado no código

Referência consultada no GitHub e na cópia local pinada em
`6984a3d67592157195940049be2e4bbbf933f9ff`:

1. [overlay_36.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/overlay_36.c):
   `NewGame_InitSaveData` → Oak → `InitGameStateAfterOakSpeech_Internal` →
   `gApplication_NewGameFieldsys`. O pós-Oak tem efeitos de identidade, RNG,
   relógio e dados do save; não pode ser substituído só por nome e coordenadas.
2. [field_system.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/field_system.c):
   `Field_NewGame_AppInit` cria FieldSystem e chama `CallFieldTask_NewGame`.
3. [field_warp_tasks.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/field_warp_tasks.c):
   init-script, entrada no local, matriz/modo, mapa, jogador/objetos e restauração
   do overworld. O início original é o quarto, mapa 64, posição (6,6).
4. [battle_setup.h](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/include/battle/battle_setup.h):
   a batalha transporta equipe e resultado. Os módulos `battle_main.c`,
   `battle_system.c`, `battle_controller.c` e `battle_command.c` existem na base,
   mas existência de fonte não comprova compatibilidade nativa ou link.

A auditoria detalhada anterior de nova partida está em
[newgame-dependency-audit.md](newgame-dependency-audit.md). O levantamento atual
não é uma auditoria transitiva completa de símbolos do FieldSystem e da batalha.
Essa é a próxima investigação de integração; não há contagem honesta de funções
faltantes nem prazo fechado antes dela.

## O que pode ficar fora da primeira fatia

- Cinemática inicial, título com todos os efeitos, brilho e partículas decorativas.
- Animações elaboradas de golpes, desde que ações, resultados e mensagens estejam
  visíveis e corretos. Não omitir a lógica dos efeitos dos golpes.
- Áudio completo, comunicação/link/online, Pokéwalker e conteúdo opcional.
- Pokédex completa, Johto/Kanto inteiros, pós-jogo, expansão Hoenn e outros extras.

Serviços opcionais chamados durante o boot ainda precisam de uma fronteira
explícita de indisponibilidade; não basta apagar chamadas ou retornar sucesso.
Um ponto de entrada de desenvolvimento pode dispensar a apresentação, mas deve
executar inicialização real e ser identificado como tal. Um save de teste pode
ajudar a isolar FieldSystem; não valida Nova Partida, Continue ou persistência.

## Checklist de aceite do mínimo completo

- [ ] Build Windows reproduzível; sem emulação de CPU DS no runtime.
- [ ] Criar partida e alcançar o quarto com estado inicial válido.
- [ ] Andar, colidir, conversar e trocar de mapa sem teleporte de diagnóstico.
- [ ] Cumprir eventos necessários para receber o Pokémon inicial.
- [ ] Entrar numa batalha pelo mapa, escolher ações e voltar com o estado correto.
- [ ] Salvar, encerrar o processo, reabrir e continuar com o progresso preservado.
- [ ] Repetir o percurso; validar single screen, teclado/toque e ausência de menus ocultos.
- [ ] Registrar commit, plataforma, comandos, logs e limites; manter assets/saves privados.

## Prioridade prática

Suspender a expansão de testes cosméticos da abertura como caminho crítico.
Primeiro: build Windows da rota libntr e inventário transitivo de Nova Partida /
FieldSystem. Depois: quarto controlável, eventos iniciais, batalha e save/load.
As issues #10–#12 atuais devem distinguir apresentação completa de acesso ao
gameplay; terminar cada cena da abertura não deve bloquear o primeiro mapa.

Este documento é um levantamento de código e evidências existentes, não o
resultado de uma nova execução de gameplay. O estado continua **não jogável**.
