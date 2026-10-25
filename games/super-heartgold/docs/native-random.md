# RNGs nativos por instância

Validação: 2026-10-25. Referência:
[`ClickingMaster/pokeheartgold-single-screen@6984a3d67592157195940049be2e4bbbf933f9ff`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).

`game/core/include/hg/random.hpp` disponibiliza dois fluxos independentes:

```cpp
hg::Lcrng lc(seed32);
const std::uint16_t word = lc.next16();
const std::uint32_t current_state = lc.state();

hg::Mt19937 mt(seed32);
const std::uint32_t value = mt.next32();
```

Os construtores exigem uma seed explícita. Ambos aceitam `reseed(seed32)`, que
descarta a posição anterior do fluxo. Copiar uma instância preserva sua posição
atual em uma cópia independente. Não há seed automática por relógio, estado
global, CPU DS ou acesso a disco nessa API.

## Compatibilidade das sequências

`Lcrng` avança o estado sem sinal de 32 bits usando multiplicador `1103515245` e
incremento `24691`, retornando os 16 bits superiores do **novo** estado.
`Mt19937` usa `std::mt19937` com a seed de 32 bits. A recorrência, expansão da seed
e tempering foram comparados com as funções C originais, incluindo várias
passagens pelo limite de 624 saídas.

O original tem um caminho de MT sem inicialização explícita: contador 625 provoca
semeadura com 5489 na primeira chamada. A API autoral exige o construtor explícito;
`hg::Mt19937(5489)` reproduz esse fluxo. Isso não define a política final de
semeadura de uma nova partida, que pertence à inicialização do jogo.

Os bytes internos de `std::mt19937` podem variar entre bibliotecas e plataformas.
Não são um formato de save. O projeto não usa `sizeof` ou cópia bruta dessa classe
para representar a estrutura RNG do DS.

O inicializador original de roamers (`src/roamer.c`) consome **duas chamadas
consecutivas a `MTRandom()`**. Esse consumidor deve receber uma instância MT e
chamar `next32()` duas vezes; reiniciar a seed entre chamadas ou substituir esse
fluxo por LCRNG altera os valores originais.

## Oráculo privado reproduzível

`tools/verify_random_reference.py` verifica `src/math_util.c`, extrai cinco
funções exatas e compila um oráculo temporário C11 fora do Git. O oráculo mantém
as variáveis globais exigidas pelo original; elas não aparecem na API autoral.
As duas implementações são comparadas em execução.

| Fonte/função | SHA-256 |
| --- | --- |
| `src/math_util.c` | `ed76da9a0792494d7b22a029d903db402f5c2cec06212870e0337205be747612` |
| `GetLCRNGSeed` | `80edfa6fa96fc6769c5bcd2ab621f7a675f5fe61f29f95f6c9a66661b0788f37` |
| `SetLCRNGSeed` | `42385497a4374b048a70c63bb8fbfbfe254a1d1b4e99bfbc6ec1aae14932ba7c` |
| `LCRandom` | `58c4cf9adf4e07c0625a95153a43f7143d0bd2de5d8cf8de1e75cba7221e7afd` |
| `SetMTRNGSeed` | `b46cb7ae177f36c478f32eddb592bd63ec61b3416588d0cd219e785b7d2da44e` |
| `MTRandom` | `003861256bc64e44dc35b0afeea6fc167c8575a564bbf60b0a6cf8a097f7cf46` |

Comando, executado a partir da pasta do projeto:

```sh
python3 tools/verify_random_reference.py /caminho/privado/da/referencia --sanitizers
```

Foram usadas 264 seeds distintas: oito casos explícitos e 256 casos gerados
deterministicamente. Cada seed produziu 2.500 saídas por RNG. Resultados com GCC,
C11/C++20, `-Wall -Wextra -Werror`, ASan e UBSan:

| Verificação | Resultado |
| --- | --- |
| Saídas LCRNG | 660.000 iguais |
| Estado LCRNG após cada saída | 660.000 iguais |
| Saídas MT19937 | 660.000 iguais |
| Caminho original MT com seed padrão 5489 | 2.500 saídas iguais |
| Quatro pares de instâncias intercaladas contra vetores originais | 10.000 pares iguais |
| Testes unitários autorais | 4 casos passaram |

Os testes unitários incluem vetores conhecidos, seed zero, overflow de seed,
instâncias intercaladas, mais de quatro ciclos MT de 624 saídas, cópia após
avanço e reinicialização depois de várias chamadas.

Nenhuma fonte original, ROM ou executável do oráculo é publicado. A validação
cobre as sequências RNG e a independência das instâncias; não comprova a ordem
de consumo dos geradores por todos os sistemas de batalha, mapa ou campanha.
