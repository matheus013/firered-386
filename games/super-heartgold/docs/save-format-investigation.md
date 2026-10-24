# Investigação do formato de save HG/SS

Investigação local de 24/10/2026. Nenhum save do jogador foi aberto, alterado ou
criado nesta investigação. Os dados de teste são sintéticos. O objetivo é permitir
uma leitura nativa segura dos dois blocos principais, preservando a diferença
entre validar esses blocos e carregar um jogo completo.

## Fontes e proveniência

Referência de código: `ClickingMaster/pokeheartgold-single-screen`, commit
`6984a3d67592157195940049be2e4bbbf933f9ff`.

- [`include/save.h`](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/include/save.h): tipos dos rodapés e especificações dos blocos.
- [`include/constants/save_arrays.h`](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/include/constants/save_arrays.h): 42 subestruturas, 35 páginas de `0x1000` bytes.
- [`src/save_arrays.c`](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/save_arrays.c): ordem, agrupamento e funções de tamanho.
- [`src/save.c`](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/save.c): `GetSaveChunkSizePlusCRC`, `SaveData_InitSubstructs`, `SaveData_InitSlotSpecs`, `ValidateSaveSectorFooter`, `Save_GetSaveFilesStatus`.
- [`src/math_util.c`](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/math_util.c): `GF_CalcCRC16` e `GF_CRC16Init`, linhas 159–166.

O SDK não estava disponível nesta cópia do código. Por isso, os detalhes de CRC
foram confirmados no ARM9 original, sem inferir o algoritmo apenas pelo nome da
função. A imagem ARM9 privada já extraída da cópia local autorizada de SoulSilver
USA possui 1.122.052 bytes:

| Identificador | Valor |
| --- | --- |
| SHA-1 da imagem ARM9 | `745d14bdf11bf47412170b724550f4ece9f520f6` |
| SHA-256 da imagem ARM9 | `5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8` |
| Endereço base | `0x02000000` |
| Tabela de 42 entradas | `0x020F64C4` |

O SHA-1 coincide com a seção `main` do layout privado da investigação AOT. Essa
verificação é da seção ARM9; o hash da ROM inteira fornecida pelo usuário difere
da edição retail de referência. Nenhuma ROM, imagem ARM9, instrução original ou
recurso extraído é incluído neste documento ou no Git.

## Dois bancos e dois blocos principais

A flash possui `0x80000` bytes (512 KiB). Os bancos começam em `0` e `0x40000`.
Cada banco contém os seguintes blocos principais; os offsets da tabela são
relativos ao começo do banco.

| Bloco | ID do rodapé | Offset | Tamanho com rodapé | Offset do rodapé | Bytes cobertos pelo CRC |
| --- | --- | --- | --- | --- | --- |
| Dados normais | 0 | `0x00000` | `0x0F628` | `0x0F618` | `0x0F618` |
| PC | 1 | `0x0F700` | `0x12310` | `0x21A00` | `0x12300` |

Assim, os quatro rodapés principais ficam nos offsets absolutos `0x0F618`,
`0x21A00`, `0x4F618` e `0x61A00`. O espaço entre os blocos é alinhamento; não
entra no CRC de nenhum dos dois. Os demais registros da flash exigem tratamento
próprio.

O rodapé principal tem exatamente 16 bytes. Deve ser decodificado explicitamente
em little-endian, sem converter bytes em um ponteiro para uma struct do host.

| Offset no rodapé | Tipo | Campo |
| --- | --- | --- |
| `+0` | u32 | Contador de save |
| `+4` | u32 | Tamanho do bloco, incluindo rodapé |
| `+8` | u32 | Magic `0x20060623` |
| `+12` | u16 | ID do bloco: 0 normal ou 1 PC |
| `+14` | u16 | CRC do conteúdo anterior ao rodapé |

`SaveChunkFooter` e `SaveArrayFooter` possuem o mesmo tamanho de 16 bytes, mas
ordens de campos diferentes. O primeiro é o formato dos dois blocos principais.
O segundo pertence aos registros extras e não deve ser interpretado com a mesma
ordem de campos. O CRC principal exclui o rodapé inteiro; o CRC dos registros
extras segue outra cobertura definida nas funções próprias de `save.c`.

## Derivação dos tamanhos

Não foi usado `sizeof` de structs recompiladas para Windows x64. O tamanho bruto
foi obtido executando cada uma das 42 funções originais de tamanho da tabela
ARM9, em uma instância isolada do Unicorn, limitada a 100 instruções por função.
Todas retornaram em poucas instruções. A tabela foi identificada de forma única
pela sequência de IDs 0–41, grupo 0 nas primeiras 41 entradas, grupo 1 na última
e ponteiros de código dentro da seção ARM9 verificada. A ordem foi comparada com
`gSaveChunkHeaders` no código fixado acima.

| ID | Constante | Tamanho bruto hexadecimal |
| --- | --- | --- |
| 0 | `SAVE_SYSINFO` | `0x5C` |
| 1 | `SAVE_PLAYERDATA` | `0x2C` |
| 2 | `SAVE_PARTY` | `0x5B0` |
| 3 | `SAVE_BAG` | `0x79C` |
| 4 | `SAVE_FLAGS` | `0x44C` |
| 5 | `SAVE_LOCAL_FIELD_DATA` | `0x80` |
| 6 | `SAVE_POKEDEX` | `0x340` |
| 7 | `SAVE_DAYCARE` | `0x1E0` |
| 8 | `SAVE_PALPAD` | `0x880` |
| 9 | `SAVE_MISC` | `0x2E0` |
| 10 | `SAVE_MAP_OBJECTS` | `0x1400` |
| 11 | `SAVE_LINK_BATTLE_RULESET` | `0x20` |
| 12 | `SAVE_DRESSUP_DATA` | `0x834` |
| 13 | `SAVE_MAILBOX` | `0x460` |
| 14 | `SAVE_FRIEND_GROUP` | `0x108` |
| 15 | `SAVE_TRAINER_CARD` | `0x620` |
| 16 | `SAVE_GAMESTATS` | `0x1C0` |
| 17 | `SAVE_SEAL_CASE` | `0x170` |
| 18 | `SAVE_CHATOT` | `0x3EC` |
| 19 | `SAVE_UNK_19` | `0x1628` |
| 20 | `SAVE_SPECIAL_RIBBONS` | `0xE` |
| 21 | `SAVE_ROAMER` | `0x68` |
| 22 | `SAVE_GTS` | `0xF8` |
| 23 | `SAVE_UNK_23` | `0xBC8` |
| 24 | `SAVE_RANKINGS` | `0xEA0` |
| 25 | `SAVE_UNK_25` | `0x8C0` |
| 26 | `SAVE_WIFI_HISTORY` | `0xFF8` |
| 27 | `SAVE_MYSTERY_GIFT` | `0x1680` |
| 28 | `SAVE_UNK_28` | `0x688` |
| 29 | `SAVE_POKEATHLON_FRIENDSHIP_RECORDS` | `0x28` |
| 30 | `SAVE_EASY_CHAT` | `0x8` |
| 31 | `SAVE_UNK_31` | `0x40` |
| 32 | `SAVE_PLAZA_SURVEY` | `0x8` |
| 33 | `SAVE_FOLLOW_MON` | `0x8` |
| 34 | `SAVE_POKEGEAR` | `0x658` |
| 35 | `SAVE_SAFARI_ZONE` | `0x5FC` |
| 36 | `SAVE_PHOTO_ALBUM` | `0x1294` |
| 37 | `SAVE_POKEATHLON` | `0xB80` |
| 38 | `SAVE_APRICORN_BOX` | `0x80` |
| 39 | `SAVE_POKEWALKER` | `0x134` |
| 40 | `SAVE_TRAINER_HOUSE` | `0xF00` |
| 41 | `SAVE_PCSTORAGE` | `0x122FC` |

Aplicando literalmente a aritmética de `GetSaveChunkSizePlusCRC`:

```text
tamanho armazenado da subestrutura = align_up(tamanho bruto, 4) + 4
tamanho do bloco = soma dos tamanhos armazenados do grupo + 16
offset do próximo bloco = align_up(offset + tamanho do bloco, 0x100)
```

Resultam os tamanhos `0xF628` e `0x12310` apresentados acima. Os 4 bytes extras
por subestrutura fazem parte da cobertura do CRC principal; não devem ser
removidos ao calcular os offsets.

Como confirmação independente da transcrição dessa aritmética, também foram
executadas as duas rotinas originais completas na mesma memória sintética:
`SaveData_InitSubstructs` em Thumb `0x02027E5C`, seguida de
`SaveData_InitSlotSpecs` em Thumb `0x02027EFC`. Cada chamada recebeu um limite de
100.000 instruções; nenhuma função foi substituída. Os resultados originais
coincidiram com os offsets e tamanhos acima. Os campos de páginas retornados
foram `(firstPage=0, numPages=16)` para o bloco normal e
`(firstPage=16, numPages=19)` para o PC, totalizando as 35 páginas permitidas.

## CRC confirmado no código original

| Parâmetro | Valor |
| --- | --- |
| Largura | 16 bits |
| Polinômio | `0x1021` |
| Valor inicial | `0xFFFF` |
| Reflexão de entrada/saída | Nenhuma |
| XOR final | `0x0000` |
| Ordem de processamento | Bit mais significativo primeiro |

A identificação é sustentada por este percurso na imagem original:

1. `GF_CalcCRC16`, Thumb em `0x0201FF98`, carrega a tabela pelo ponteiro global
   `0x021D15A4` e encaminha para `0x020E3A2C` em ARM.
2. A rotina em `0x020E3A2C` inicializa o acumulador com `0xFFFF`, chama a
   atualização em `0x020E38E0` e retorna o acumulador de 16 bits sem XOR final.
3. A atualização usa o byte superior do acumulador e deslocamento à esquerda,
   confirmando a ausência de reflexão.
4. `GF_CRC16Init`, Thumb em `0x0201FFB0`, passa o polinômio `0x1021` ao gerador
   de tabela em ARM `0x020E3850`. O literal está em `0x0201FFDC`.

Foram executadas tanto a rotina SDK quanto a função `GF_CalcCRC16` original,
usando uma tabela gerada pela rotina original. Os resultados foram comparados
com uma implementação independente por bits em Python. Todas as 14 entradas
coincidiram nos dois caminhos originais e no cálculo independente.

| Entrada | CRC |
| --- | --- |
| Vazia | `0xFFFF` |
| ASCII `123456789` | `0x29B1` |
| Um byte `00` | `0xE1F0` |
| Um byte `FF` | `0xFF00` |
| Bytes `00` até `FF`, em ordem | `0x3FBD` |

As outras nove entradas possuem comprimentos 2, 3, 15, 16, 255, 512, 4096,
`0xF618` e `0x12300`, gerados sequencialmente por
`random.Random(0x20060623).randrange(256)` em Python. As duas últimas verificam
também os tamanhos integrais cobertos pelos CRCs normal e PC. O limite de cada
execução CRC foi de 3.000.000 instruções. Só foram mapeadas a imagem ARM9
verificada e memória sintética para tabela, dados e pilha. Nenhum periférico,
arquivo de save ou emulador do jogo foi usado nesses testes de função.

O uso de um executor ARM isolado nesta investigação confirma o comportamento
que precisa ser portado. Ele não integra o executável nativo final.

## Limites e próximo passo seguro

Essas informações permitem validar, em modo somente leitura, tamanho, magic,
ID e CRC de cada um dos quatro blocos principais de uma imagem flash bruta.
A escolha do banco precisa seguir `Save_GetSaveFilesStatus`: comparar os
contadores dos blocos normal e PC do mesmo banco, preservar os casos de
recuperação e tratar explicitamente o avanço de `0xFFFFFFFF` para zero.
Escolher simplesmente o maior contador individual não reproduz o original.

Uma imagem inteira com `FF` pode representar flash vazia. Uma imagem cujo CRC
principal passa ainda pode possuir registros extras corrompidos ou conteúdo de
jogo inválido. Os registros extras, a interpretação das 42 subestruturas,
descriptografia de Pokémon, compatibilidade de dados e carregamento do mundo
ainda precisam de implementação e testes separados.

`SaveData_New` usa o resultado principal e depois `Save_CheckFrontierData` para
produzir os bits de diagnóstico consumidos pelo fluxo de verificação do save.
Portanto, o resultado do verificador principal não equivale ao conjunto completo
de `statusFlags`. Um resultado válido também não significa que Continue,
Novo Jogo, salvamento ou campanha já estejam portados.
