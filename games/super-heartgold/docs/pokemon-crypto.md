# Segmentos XOR e checksum de Pokémon

Validação: 2026-10-25. Base pinada:
[`ClickingMaster/pokeheartgold-single-screen@6984a3d67592157195940049be2e4bbbf933f9ff`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).

`game/core/include/hg/pokemon_crypto.hpp` oferece duas operações nativas autorais:

```cpp
hg::xor_pokemon_segment(segment, seed);
const std::uint16_t checksum = hg::pokemon_checksum(segment);
```

São operações do formato de dados do jogo, sem finalidade de segurança. Não
dependem de CPU ARM, alinhamento, ordem dos bytes do host, chaves globais ou
`offsetof` de estruturas compiladas para x64. Cada chamada começa com sua própria
seed; processar segmentos independentes não compartilha estado.

## Contrato

As duas funções aceitam segmentos vazios e exigem comprimento par de até
**128 KiB**. Comprimentos ímpares ou maiores são rejeitados com `std::runtime_error`.
O XOR valida o segmento inteiro antes da primeira escrita, preservando os dados
quando essa validação falha. Subspans desalinhados são suportados.

O XOR avança o LCRNG de 32 bits antes de cada palavra, usando multiplicador
`1103515245` e incremento `24691`, com overflow sem sinal. Os 16 bits superiores
do estado são aplicados à próxima palavra little-endian. A mesma operação com a
mesma seed desfaz a transformação. O checksum soma as palavras little-endian e
retém os 16 bits inferiores; o segmento vazio produz zero.

O chamador define o segmento e a seed corretos. A API **não** interpreta registros
de Pokémon, não escolhe offsets, não permuta os quatro blocos internos, não decide
quando atualizar o checksum e não implementa toda a validação de `MonData`.

## Comparação com funções originais

`tools/verify_pokemon_crypto.py` lê uma cópia privada da referência. Verifica os
hashes completos de `src/math_util.c` e `src/pokemon.c`, depois extrai quatro
funções exatas para uma pasta temporária fora do Git. O original é compilado em
C11; o código autoral e o harness são compilados em C++20. Nenhuma dessas funções
originais é incorporada ao código público.

| Arquivo/função | SHA-256 |
| --- | --- |
| `src/math_util.c` | `ed76da9a0792494d7b22a029d903db402f5c2cec06212870e0337205be747612` |
| `src/pokemon.c` | `6ce5b2fce2e911772f04913e6b57e580156e08ebc4ef28d25322ab647e5cbf65` |
| `MonEncryptionLCRNG` | `b3b1c585d0f85bd9b41ec56a0f479574472ea5e1797ee56970f550a5469c40b9` |
| `_MonEncryptSegment` | `1b541efad042436e1f5359c9889547f136de5a804d2588d145a14def5e21ee6f` |
| `_MonDecryptSegment` | `cf8aec04d5edff2f0ac4fff8470813b61b0239834f43cf00352b1fe600156280` |
| `CalcMonChecksum` | `09612764762c5781a0b45220b6ad2e3fe318b68bbc7d673db4b1222cc5a05361` |

Comando de reprodução, usando uma referência local privada:

```sh
python3 tools/verify_pokemon_crypto.py /caminho/privado/da/referencia --sanitizers
```

A comparação usa oito seeds, seis padrões de dados e 16 comprimentos entre zero
e 128 KiB. O harness monta palavras numéricas a partir dos bytes little-endian
antes de chamar o original, evitando depender de casts desalinhados. Resultados:

| Verificação | Resultado |
| --- | --- |
| Transformação contra `_MonEncryptSegment` | 768 casos iguais |
| Transformação inversa contra `_MonDecryptSegment` | 768 casos iguais |
| Checksum dos dados iniciais e transformados | 1.536 casos iguais |
| GCC/C11/C++20, `-Wall -Wextra -Werror`, ASan e UBSan | Passou |
| Testes unitários autorais | 6 casos passaram, também com ASan e UBSan |

Os testes unitários incluem vetores conhecidos, dados diferentes de zero,
independência entre chamadas, overflow de checksum, subspans desalinhados,
sentinelas, limite máximo e rejeição antes de mutar o buffer. Esses resultados
validam as duas operações isoladas, sem afirmar compatibilidade completa de
registros Pokémon, saves, batalhas ou campanha.

ROMs, registros de Pokémon, fontes originais extraídas e executáveis do oráculo
não são publicados por esse procedimento. O harness é removido ao terminar.
