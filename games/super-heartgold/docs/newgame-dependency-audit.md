# Dependências da nova partida até o primeiro mapa

Auditoria de código de 24/10/2026, sem executar a campanha, abrir saves do jogador
ou modificar dados privados. Referência fixada em
`6984a3d67592157195940049be2e4bbbf933f9ff` de
[`ClickingMaster/pokeheartgold-single-screen`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
As linhas abaixo referem-se a essa versão. Este documento identifica trabalho
pendente; não constitui evidência de uma nova partida funcional.

## Caminho real

1. `src/overlay_36.c:93–114`: NewGame cria heap, inicializa o RNG, chama
   `NewGame_InitSaveData` e só depois registra `gApplication_OakSpeech`.
2. `overlay_36.c:251–260`: inicializa a região dinâmica completa, posiciona o
   jogador no quarto, define dinheiro 3000, recorde Magikarp 56150 e flag 960.
3. `src/save.c:799–812`: limpa a região dinâmica e chama os **42** inicializadores
   de `gSaveChunkHeaders`, na ordem. Substituí-los por zero não preserva o estado.
4. `src/field_comm_cmds.c:15`: Oak corresponde a `OakSpeech_Init/Main/Exit`, OVY53.
5. `src/oaks_speech.c:546–650`: Oak usa options do save, duas estruturas de naming,
   fontes, BG, sprites, yes/no, input/touch e fades. A saída grava nome/gênero e
   nome do rival; registra a aplicação pós-Oak do OVY36. O task principal tem
   estados numerados até 126, com lacunas; isso não significa 126 estados ativos.
6. `overlay_36.c:117–140,170–243`: pós-Oak configura relógio/identidade, RNG,
   treinador, avatar, Safari, apricorns, Pokéwalker e a carta inicial no PC.
   Esta carta exige criar um Marill e usar dados de mail/texto; não é só dinheiro
   e coordenadas. Só então começa o IGT e registra `gApplication_NewGameFieldsys`.
7. `src/field_system.c:70–73,140–160`: cria FieldSystem, carrega temporariamente
   OVY124 e executa `FieldSystem_Init`; depois chama a tarefa de nova partida.
8. `src/field_warp_tasks.c:386–414`: executa init-script; entra no local; carrega
   matriz e modo; inicializa mapa; cria jogador/objetos; restaura overworld/fade.
   São necessários eventos, scripts, colisão/movimento e representação do mapa,
   além do renderer. Um callback que retorna sucesso sem esses efeitos não
   valida a chegada ao quarto.

O local inicial original é `MAP_NEW_BARK_PLAYER_HOUSE_2F` (64), warp -1, x=6,
y=6, direção=1 (`src/location_backup.c:10–18,36–39`). Não há teleporte de teste
necessário no fluxo final; o próprio estado inicial define essas coordenadas.

## Os 42 inicializadores

Esta lista registra dependências diretas observadas no corpo das funções,
não o fechamento transitivo. Todos precisam de tamanhos/offsets DS explícitos;
`sizeof` x64 e ponteiros host não definem um formato de disco válido.

| ID | Inicializador original | Dependências ou particularidades relevantes |
| --- | --- | --- |
| 0 | `Save_SysInfo_Init` | RTC/date/time e conversão do epoch Nitro |
| 1 | `Save_PlayerData_Init` | Options, Profile, Coins, IGT |
| 2 | `SaveArray_Party_Init` | Party size=6, `ZeroMonData`, região extra |
| 3 | `Save_Bag_Init` | Clear16 |
| 4 | `Save_VarsFlags_Init` | Memset; flags temporárias são globais separadas |
| 5 | `Save_LocalFieldData_Init` | PlayerSaveData, spawn da mãe=1 |
| 6 | `Save_Pokedex_Init` | Ordem de formas de Deoxys |
| 7 | `Save_Daycare_Init` | ZeroBoxMonData |
| 8 | `Save_PalPad_Init` | Memset |
| 9 | `Save_Misc_Init` | Berry pots/RTC, apricorns, mail/easy-chat, gymmicks |
| 10 | `Save_MapObjects_Init` | Clear32, 64 registros |
| 11 | `Save_LinkBattleRuleset_Init` | Memset |
| 12 | `Save_FashionData_Init` | FashionCase_Init |
| 13 | `Save_Mailbox_Init` | Mail_Init por slot |
| 14 | `Save_FriendGroup_Init` | ClearFast |
| 15 | `Save_TrainerCard_Init` | Inicializa brilho das insígnias |
| 16 | `GameStats_Init` | VBlank count e GameStats_Release |
| 17 | `Save_SealCase_Init` | Clear8 |
| 18 | `Save_Chatot_Init` | Clear32 |
| 19 | `Save_Frontier_Init` | Tower/Arcade/Factory/Castle/Hall/downloads |
| 20 | `Save_SpecialRibbons_Init` | Memset |
| 21 | `Save_Roamers_Init` | MTRandom |
| 22 | `Save_Gts_Init` | ClearFast |
| 23 | `SaveBlock23_Init` | Atualiza CRC via save global |
| 24 | `Save_Rankings_Init` | Rankings entries e CRC via save global |
| 25 | `Save_FriendRoster_Init` | DWC userdata e cópia em roster global |
| 26 | `Save_WiFiHistory_Init` | CRC via save global |
| 27 | `Save_MysteryGift_Init` | CRC via save global |
| 28 | `MigratedPokemon_Init` | ZeroMonData, GetMonData/assert |
| 29 | `PokeathlonSave_FriendshipRecords_Init` | Clear8 |
| 30 | `Save_EasyChat_Init` | Greeting flags e CRC via save global |
| 31 | `Save_WiiMessage_Init` | Clear8/memset e CRC via save global |
| 32 | `Save_PlazaSurvey_Init` | Clear32 |
| 33 | `Save_FollowMon_Init` | Memset |
| 34 | `SaveData_Pokegear_Init` | SavePokegear_Init_Internal |
| 35 | `Save_SafariZone_Init` | Limpa líder |
| 36 | `Save_PhotoAlbum_Init` | Photo_Init |
| 37 | `PokeathlonSave_Init` | Fill8 |
| 38 | `Save_ApricornBox_Init` | InitApricornBox |
| 39 | `Pokewalker_Init` | Desbloqueia curso inicial |
| 40 | `Save_TrainerHouse_Init` | TrainerHouseSet_SetZero |
| 41 | `PCStorage_Init` | Inicializa boxes, nomes e papéis de parede |

As funções que atualizam CRC precisam do contexto global correto, não apenas de
um ponteiro para um bloco isolado. RTC, VBlank e RNG devem ter serviços host com
semântica definida e entradas injetáveis nos testes. DWC exige uma decisão
explícita de compatibilidade offline; retornar sucesso sem criar userdata é um
stub incorreto. `SaveData_New` inicializa cabeçalhos e o contexto antes de chamar
os inicializadores, inclusive quando não há save na flash.

## Próxima fatia implementável

Uma fatia pequena e verificável é executar nativamente os inicializadores C
originais de **PLAYERDATA, VarsFlags e LocalFieldData**, com dependências exatas
Options/Profile/Coins/IGT, PlayerSaveData e GetMomSpawnId. Estes não exigem renderer,
rede ou dados de Pokémon. O resultado deve ser apresentado como **estado parcial
em memória**, sem habilitar Continue, gravar arquivo ou anunciar NewGame completo.

Critérios de aceitação:

- Seleção privada e hash do código C original, como nos probes já existentes.
- Verificar tamanho bruto original: PLAYERDATA `0x2C`, VarsFlags `0x44C`,
  LocalFieldData `0x80`; o tamanho dos headers do save inclui alinhamento/4 bytes
  adicionais e não é o tamanho bruto do objeto.
- Comparar bytes com as mesmas funções originais ARM9 em memória sintética.
  Use bytes iniciais não nulos e guardas antes/depois para testar o alcance real.
- Validar padding/bitfields com offsets/asserts e comparação; não inferir ABI
  apenas porque o struct compilou. PLAYERDATA inclui alinhamento entre Options
  e Profile; a definição original não usa packing.
- Aplicar separadamente os efeitos originais de nova partida (dinheiro 3000,
  quarto, recorde e flag), preservando a distinção entre inicialização de bloco
  e inicialização da partida inteira.
- Interromper no limite declarado de estado parcial. Enfileirar Oak só após os
  42 blocos completos e os serviços obrigatórios estarem implementados.

Uma alternativa visual independente é carregar os recursos 2D de Oak e o banco
de mensagens 219 como diagnóstico. Isso valida decoders/renderização, mas não
substitui a state machine de Oak, naming ou a inicialização do save.

Os offsets de disco e tamanhos dos 42 blocos já comprovados no ARM9 estão em
[save-format-investigation.md](save-format-investigation.md). Essa comprovação
dos tamanhos não comprova o conteúdo produzido pelos inicializadores.

## Fatia implementada e prova de 26 blocos

`initialize_soulsilver_usa_partial_blocks()` agora produz 26 blocos brutos com
arrays de bytes, sem depender do layout de structs do compilador.
Não aplica os efeitos posteriores de `NewGame_InitSaveData`, não inicia Oak e
não cria um save. Os defaults foram limitados à edição SoulSilver USA verificada:
idioma inglês, versão 8. Não se afirma suporte a defaults de outras edições.

A ferramenta privada `tools/verify_newgame_blocks.py` compila o **mesmo C++ de
produção** num harness temporário, captura seus bytes e compara cada bloco
com sua função inicializadora ARM9 original. Os ponteiros são obtidos da tabela
original de 42 entradas, depois de verificar o hash completo da imagem.
Cada função é executada com quatro preenchimentos diferentes em RAM sintética,
guardas de 16 bytes antes/depois e limite de 100.000 instruções. Um hook também
rejeita qualquer escrita fora do bloco-alvo e da stack sintética, incluindo
escritas em globais. Também é chamada a função de tamanho original correspondente.
Nenhum save real é lido.

```sh
python3 tools/verify_newgame_blocks.py --arm9 /caminho/privado/rc_image_0.bin
```

Prova executada localmente: as 104 comparações (26 blocos × 4 padrões) passaram,
com guardas preservadas. SHA-256 da saída dos três blocos iniciais:

| Bloco | Bytes | SHA-256 |
| --- | --- | --- |
| PLAYERDATA | 44 | `b454c2c268e9c40e843cd7f1f91c229777b5d50f87e5bbce60200fc3390199c5` |
| VarsFlags | 1100 | `a805fd6f7ad225e289321ba6e2b079e92c3accc1675a7ba6626101da38f727f2` |
| LocalFieldData | 128 | `babcac2e871a15d603fc499b90ce598ee48896b3613f4d87b1d01b3dc5e7e2d4` |

Os dez blocos adicionais verificados são bag (ID3, 1948 bytes), Pal Pad (ID8,
2176), map objects (ID10, 5120), link ruleset (ID11, 32), seal case (ID17, 368),
Chatot (ID18, 1004), special ribbons (ID20, 14), GTS (ID22, 248), Pokéathlon
friendship records (ID29, 40) e follow mon (ID33, 8). Seus hashes também são
impressos pela ferramenta. Pal Pad precisa de EOS `0xFFFF` no início dos 16 nomes;
os outros nove blocos adicionais são zerados pelas funções originais. Special
ribbons tem tamanho bruto 14, não deve receber 16 bytes de escrita.

FriendGroup foi deliberadamente excluído desse conjunto: além dos bytes, seu
inicializador registra um ponteiro global. Reproduzir apenas o buffer não
implementaria todos os efeitos daquela função.

Em 25/10/2026, mais oito blocos foram implementados e comprovados com o mesmo
procedimento, incluindo o hook de escritas:

| ID | Bloco | Tamanho bruto | Defaults não nulos relevantes |
| --- | --- | --- | --- |
| 6 | Pokédex | `0x340` | Magic `0xBEEFCAFE`, históricos de formas vazios |
| 15 | Trainer Card | `0x620` | Oito valores de brilho `0x8C`; assinatura vazia |
| 32 | Plaza Survey | `0x8` | ID da pesquisa `0xFFFFFFFF`; resposta zero |
| 36 | Photo Album | `0x1294` | 36 fotos, dois nomes EOS por foto; nenhum Pokémon |
| 37 | Pokéathlon | `0xB80` | 100 escores u16 `0xFFFF`, sem recorde |
| 38 | Apricorn Box | `0x80` | Três nomes EOS nos registros de suco |
| 39 | Pokéwalker | `0x134` | Cursos iniciais 0 e 1 desbloqueados; máscara 3 |
| 40 | Trainer House | `0xF00` | Nomes EOS de dez treinadores e 60 Pokémon vazios |

As fórmulas vêm dos fontes pinados `pokedex.c`, `save_trainer_card.c`,
`save_plaza_survey.c`, `photo_album.c`, `pokeathlon/pokeathlon_save.c`,
`apricorn_box.c`, `pokewalker.c` e `save_trainer_house.c`, junto de seus headers.
As escritas são little-endian explícitas: não se compilam esses structs para
deduzir offsets. Os históricos de Deoxys usam os bytes altos de dois words de
flags da Pokédex; esses sentinelas não concedem capturas nem National Dex.

Mailbox foi excluído deste conjunto: seu inicializador lê globais de idioma e
versão e preserva os campos `msg_no` de `MailMessage`. Na partida original, o clear da
região dinâmica ocorre antes dessa chamada.

Os 13 testes C++ passaram com GCC, `-Wall -Wextra -Werror`, AddressSanitizer
e UndefinedBehaviorSanitizer. Essa é validação de 26 inicializadores parciais;
os outros 16 blocos, a partida completa e a campanha continuam pendentes. A
fração dos blocos inicializados não estima a conclusão do port ou da campanha.

## Pokémon vazios criptografados

Em 25/10/2026, foram acrescentados Party (ID2, `0x5B0`), Daycare (ID7, `0x1E0`)
e MigratedPokemon (ID28, `0x688`). Cada um passou pelas quatro execuções ARM9
com padrões diferentes e pelo hook de escritas restritas. Os três inicializadores
originais limpam o bloco inteiro, incluindo padding, antes de criar os registros.

Um Pokémon vazio armazenado não é uma sequência de zeros. `ZeroMonData` e
`ZeroBoxMonData` de `src/pokemon.c:101–110` limpam o objeto e criptografam seus
segmentos. Os macros `ENCRY_ARGS_BOX/PTY` usam checksum e personalidade como
seed; ambos são zero nesses objetos vazios. O algoritmo de
`src/math_util.c:143–156` é reproduzido com aritmética u32 definida:

```text
para cada halfword little-endian, partindo de seed=0 por segmento:
    seed = (seed * 1103515245 + 24691) modulo 2^32
    ciphertext = plaintext_zero XOR (seed >> 16)
```

Este é um gerador local do cifrador; não lê nem avança o RNG global da partida.
Os inicializadores usam o helper comum `xor_pokemon_segment` de
`hg/pokemon_crypto.hpp`; o algoritmo não é duplicado no módulo de nova partida.
Não há blocos grandes de bytes criptografados copiados como constantes. A região
BoxPokemon começa em +8 e tem 128 bytes. O Pokémon completo tem 236 bytes e
criptografa separadamente seus 100 bytes de dados de party em +136, reiniciando
a seed. Não é necessário implementar geração de personalidade, checksum de
Pokémon não vazios ou embaralhamento de blocos para esses casos vazios.

Party mantém capacidade 6 e quantidade atual 0, seguida de seis registros a
partir de +8; extras e padding ficam zerados. Daycare tem dois registros de
236 bytes, mas criptografa apenas o BoxPokemon de cada um; mail, nomes, passos e
dados de ovo permanecem zerados. MigratedPokemon tem seis registros completos
e mantém IDs, horários de migração, RTC e MAC zerados. Isso não cria Pokémon
capturados nem inventa dados da máquina.

| Bloco | SHA-256 dos bytes comparados |
| --- | --- |
| Party | `9f9721c6e9018093de5fb65927809d5b111bad077cc8b90167d608a81733fdea` |
| Daycare | `967a817cbde7fc39e9b162812adcd51678c52b9e9612dbd2e7c47745917f9131` |
| MigratedPokemon | `f0a5233c9f22975a2c4cf60db93061510cce6e01ca57a0ba105ef368d22c2d1f` |

Os testes também descriptografam os segmentos por uma recorrência independente
com inteiros de 64 bits e módulo explícito, exigem plaintext integralmente zero,
conferem o reinício das seeds e um prefixo observado diretamente no ARM9.

## Fashion e Pokégear; próximos limites

Também em 25/10/2026, os inicializadores Fashion (ID12, `0x834`) e Pokégear
(ID34, `0x658`) passaram pela prova completa de quatro padrões, guardas e
hook de escritas sem globais. Fashion usa magic `0x00001234` nos 16 registros
vazios (11 pequenos e cinco grandes), mantém acessórios zerados e os 18 bytes
de ordem de wallpapers com sentinela 18. Isso não concede wallpapers.

Pokégear inicializa o app anterior como 3, cursor de rádio `(128,128)`, 75
registros de rematch com o bit inicial original, contador de Kenji em sete dias
e somente Mother no primeiro contato. Os outros 74 contatos são `0xFF`; saldo,
presentes, registro de cartões e desbloqueio de mapa ficam zerados. A origem
dos offsets e defaults são `src/save_fashion.c`, `include/fashion_case.h`,
`src/save_pokegear.c`, `include/save_pokegear.h` e
`include/constants/phone_contacts.h` no pin indicado.

| Bloco | SHA-256 dos bytes comparados |
| --- | --- |
| Fashion | `545888bb37dc5b8ab9c37dfb4425a1675e09e296718f07b5a5ce4ec3d2907786` |
| Pokégear | `a49d7ea033fb89c4ff0ebaa7bafc45d574f3fde097419b10ab2bd5ad137618c6` |

O candidato Frontier desta rodada foi excluído por depender de estado global:

- Frontier (ID19) copia o estado para `sFrontierSave`, um global usado pelo jogo.
- Safari (ID35) e PCStorage (ID41) foram implementados depois, com contratos
  explícitos para pré-clear, identidade e mensagens. Veja
  [Mailbox/Safari](native-identity-init.md) e
  [PCStorage](native-pc-storage-init.md).
- FriendGroup (ID14) foi implementado como bytes de bloco isolados; o vínculo
  do ponteiro `sFriendGroupSave` ao save ativo ainda é responsabilidade do host.
  Veja [native-friend-group-init.md](native-friend-group-init.md).

## Blocos adicionais com contexto explícito

O conjunto acima conserva 26 blocos sem contexto externo. Outras duas APIs
separadas foram implementadas: [Roamers, ID21](native-roamer-init.md), que consome
exatamente duas palavras do MT do chamador, e
[GameStats, ID16](native-game-stats-init.md), que recebe um contador de VBlank
estável. Foram comparadas com o ARM9 original em 96 e 32 casos respectivamente.
Naquela etapa intermediária havia 28 inicializadores isolados. As validações
adicionais desta rodada estão registradas abaixo. Essa contagem não estima a
porcentagem da campanha portada.

Em seguida foram acrescentados [Mailbox e Safari](native-identity-init.md),
com identidade e pré-limpeza explícitas: 64 comparações ARM9 e 48 controles
negativos passaram. Depois, seis [inicializadores com CRC](native-crc-block-init.md)
passaram em 24 comparações de bloco e 652 atualizações CRC; [SysInfo](native-system-info-init.md)
passou em 32 comparações com snapshot RTC explícito. [PCStorage](native-pc-storage-init.md)
passou em comparação de 74492 bytes, com 540 BoxMons e 18 nomes. FriendGroup
passou em quatro comparações de 264 bytes, com o ponteiro global conferido. O
total agora é 40 blocos, incluindo Misc (32 comparações ARM9); restam dois blocos (19 e 25) e toda a
integração do boot.
O [SaveContext](native-save-context.md) implementa o armazenamento estável e
CRC real por bloco para a integração restante. Ele não executa os inicializadores
nem cria os rodapés do save, registros extras ou working copies globais.
