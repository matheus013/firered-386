# Contexto nativo de blocos e CRC

Validação: 2026-10-25. Referência pinada:
[`ClickingMaster/pokeheartgold-single-screen@6984a3d67592157195940049be2e4bbbf933f9ff`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).

`game/core/include/hg/save_context.hpp` define `hg::SaveContext`, um proprietário
autoral do armazenamento dos 42 blocos da região dinâmica. Seu propósito atual
é fornecer spans estáveis, metadados DS explícitos e atualização real de CRC para
futuros inicializadores que precisam localizar blocos pelo ID.

Construir o contexto apenas zera os **140 KiB** da região dinâmica (`0x23000`
bytes). Não executa inicializadores, não cria CRCs automaticamente, não produz um
save completo e não reproduz a estrutura `SaveData` do DS.

## Contrato da API

```cpp
hg::SaveContext context;
const auto& metadata = context.layout(23);
auto payload = context.raw_view(23);
auto stored_block = context.view(23);
const auto crc = context.update_crc(23);
```

- `layout(id)` fornece ID, grupo principal (`slot`), tamanho bruto, tamanho
  alinhado, tamanho armazenado e offset na região dinâmica.
- `raw_view(id)` contém somente o tamanho bruto definido pela função original
  de tamanho do bloco.
- `view(id)` inclui payload, padding até quatro bytes e trailer de quatro bytes.
- `storage()` expõe a região dinâmica inteira, inclusive lacunas e regiões
  reservadas para os footers dos dois grupos principais. Não inclui os demais
  campos de `SaveData` ou as duas cópias da flash.
- `layouts()` oferece a tabela constante completa, sem ponteiros do DS.
- IDs fora de `0..41` são rejeitados antes de qualquer escrita.

O armazenamento tem tamanho fixo e nunca é realocado. Cópia e movimentação do
contexto são proibidas: spans e bindings permanecem válidos até a destruição do
proprietário. Isso não mantém vivo um contexto destruído nem fornece acesso
concorrente sincronizado. Como o objeto contém 140 KiB, chamadores com uma pilha
pequena podem alocá-lo por `std::make_unique<hg::SaveContext>()`.

## CRC de subestrutura

O tamanho armazenado original é `align4(raw_size) + 4`. `update_crc(id)` reutiliza
o `save_crc16` autoral, cuja sequência CCITT foi comparada anteriormente com as
rotinas GF/SDK originais em `tools/verify_save_layout.py`.

O cálculo inclui **todo o payload alinhado**, inclusive bytes de padding. O
resultado é gravado little-endian nos dois primeiros bytes do trailer. Os dois
últimos bytes são preservados. Nenhum outro bloco, padding de alinhamento,
footer de grupo ou outro estado é alterado.

Essa operação corresponde ao efeito de `SaveSubstruct_UpdateCRC`, em
`src/save.c:1200–1211`. Ela **não** atualiza `arrayHeaders[id].crc`; o original
calcula esse outro campo em uma etapa distinta de salvar. O contexto autoral
nem contém uma estrutura `SaveArrayHeader` nativa a ser serializada.

O bloco 20 é um caso importante: tamanho bruto `0x0E`, payload alinhado `0x10`,
bloco armazenado `0x14`. Os bytes `+0x0E` e `+0x0F` participam do CRC; o CRC é
gravado em `+0x10..0x11`; `+0x12..0x13` ficam intactos.

## Layout e evidência original

Os 42 tamanhos brutos vêm das funções de tamanho da tabela original
`0x020F64C4`. A regra de alinhamento, trailers e offsets corresponde a
`SaveData_InitSubstructs` (`src/save.c:745–771`). A tabela autoral fica em
`game/core/src/save_context.cpp` e possui verificações de integridade em tempo
de compilação, incluindo os tamanhos dos dois grupos já estabelecidos pela
API de validação de saves.

Nesta rodada houve uma nova comparação direta com a imagem ARM9 privada de
SHA-256 `5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8`.
Foram executados `SaveData_InitSubstructs`, em `0x02027E5D`, e as 42 funções de
tamanho pinadas. O harness compilou e consultou `SaveContext::layouts()`; **todos
os 42 tamanhos brutos e os 42 conjuntos ID/grupo/tamanho armazenado/offset foram
iguais**. Um hook restringiu as escritas originais à tabela sintética de
cabeçalhos e à pilha. Essa investigação executou funções isoladas; não iniciou
o jogo e não usou arquivos de save.

Limites estruturais conferidos:

| Região | Intervalo na região dinâmica |
| --- | --- |
| Blocos 0 a 40, incluindo trailers individuais | `0x00000..0x0F617` |
| Footer reservado do grupo normal | `0x0F618..0x0F627` |
| Lacuna até o próximo grupo | `0x0F628..0x0F6FF` |
| Bloco 41, incluindo trailer individual | `0x0F700..0x219FF` |
| Footer reservado do grupo PC | `0x21A00..0x21A0F` |

O campo `slot` do metadado identifica o grupo normal ou PC, **não** a cópia A/B
de um save na flash. Os offsets não incluem o prefixo de campos da estrutura
`SaveData`. Nenhum `sizeof` de uma estrutura de gameplay x64 determina o layout.

## Testes e próximos consumidores

Sete testes passaram com GCC/C++20, `-Wall -Wextra -Werror`, ASan e UBSan:
integridade e bindings dos 42 blocos, construção zerada, escrita restrita do
CRC, inclusão do padding, preservação do trailer e dos footers, estabilidade
dos spans, limites de IDs e independência entre contextos.

Os valores de regressão incluem `0xD71B` para o payload zerado de `0xBC8` bytes,
`0x6A0A` para 16 bytes zerados e `0x733E` quando os dois últimos bytes desse
payload são `A5 5A`. Os testes não apenas recalculam o valor esperado pela mesma
função que está sendo testada.

Este contexto ainda não inicializa os seis blocos dependentes de CRC, não
fornece as cópias de trabalho Frontier/roster, não cria userdata DWC e não liga
o boot da partida. Em particular, Mystery Gift inicializa apenas seu CRC no
original: seu payload depende da limpeza prévia da região dinâmica. Os futuros
consumidores precisam preservar essa ordem e o binding entre o ponteiro recebido
pelo inicializador e o bloco que `update_crc(id)` consulta.
