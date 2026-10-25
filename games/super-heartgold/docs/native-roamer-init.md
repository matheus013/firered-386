# Inicializador de roamers com RNG explícito

Implementação de 25/10/2026. `initialize_roamer_block(hg::Mt19937&)` recebe um
stream MT do chamador, consome exatamente duas palavras consecutivas de 32 bits
e retorna o bloco bruto DS 21, de 104 bytes. Os valores ocupam +0 e +4 em
little-endian; os outros 96 bytes ficam zerados. Nenhum roamer é ativado e não
há leitura de relógio, reseed, acesso a RNG global, criação ou escrita de save.

Este inicializador é **separado** dos 26 blocos sem RNG de
`PartialNewGameBlocks`. O chamador continua dono do estado aleatório e precisa
preservar a ordem de chamadas ao integrar o fluxo completo de nova partida.
Não se usa uma seed escolhida silenciosamente nem se reinicia o gerador para
cada palavra.

## Referência e endereços comprovados

Referência:
[`ClickingMaster/pokeheartgold-single-screen`, pin `6984a3d67592157195940049be2e4bbbf933f9ff`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
`src/roamer.c:13–21` limpa o bloco e chama `MTRandom()` duas vezes.
`include/roamer.h` define os campos; `src/math_util.c:80–120` define a expansão
da seed e o gerador MT. Isso não é `LCRandom`, que retorna palavras de 16 bits.

A prova usa somente a imagem ARM9 privada já verificada, SHA-256
`5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8`.
Os endereços foram identificados por encadeamento das chamadas e literais dessa
imagem, e não deduzidos de um layout de structs x64:

| Elemento | Evidência |
| --- | --- |
| Tabela de blocos | `0x020F64C4`, entrada 21, grupo 0 |
| Inicializador | Ponteiro da tabela `0x0202D96D` (Thumb) |
| MTRandom | Duas instruções BL no inicializador apontam para `0x0201FDB9` |
| SetMTRNGSeed | BL do caminho de seed padrão de MTRandom aponta para `0x0201FD75` |
| Estado MT | Literal `0x021D15AC`, exatamente 624 words (`0x9C0` bytes) |
| Contador MT | Literal `0x0210F6CC`, exatamente quatro bytes |

A ferramenta verifica o hash completo, o descritor da tabela, os destinos BL
Thumb e os literais de estado antes de executar. O hash garante que esses
endereços só sejam usados na edição exata investigada. A imagem e suas
instruções não são incorporadas no repositório ou no runtime do jogo.

## Reproduzir a prova privada

```sh
python3 tools/verify_roamer_init.py --arm9 /caminho/privado/rc_image_0.bin
```

Exige compilador nativo C++20 compatível com GNU (`--compiler`, padrão `c++`)
e o pacote opcional Unicorn. A ferramenta compila os fontes C++ reais
`roamer_init.cpp` e `random.cpp` num harness temporário fora do Git e captura
somente bytes sintéticos em memória. A entrada ARM9 é limitada a 4 MiB e precisa
coincidir com o hash. Compilação e execução do harness possuem timeout; cada
chamada ARM9 tem limite de 100.000 instruções.

Foram comprovadas **96 comparações**: quatro seeds (0, 5489, `0xFFFFFFFF`,
`0x12345678`), seis posições do stream (0, 1, 622, 623, 624, 1247) e quatro
preenchimentos prévios do bloco (0, `0xA5`, `0xFF`, `0x5A`). As posições abrangem
as fronteiras do twist MT de 624 valores. Para cada caso:

- O tamanho original precisa ser 104 e todos os bytes do bloco devem coincidir.
- As quatro próximas palavras MT também precisam coincidir, verificando a
  continuidade do stream do chamador.
- Um hook confirma exatamente duas entradas em MTRandom e nenhum reseed
  durante o inicializador.
- Guardas de 16 bytes antes/depois permanecem intactas.
- Um hook proíbe escritas fora do bloco, stack sintética e das duas regiões MT
  exatas acima. Outros globais e regiões de memória não são liberados.

Resultado local: passou. SHA-256 agregado dos resultados sintéticos nativos:
`3d84acbc837481e4e8fdb1a550c9b641eda7223e26419ac60c77923130c5ea40`.
Os três testes C++ passaram com `-Wall -Wextra -Werror`, AddressSanitizer e
UndefinedBehaviorSanitizer, incluindo vetores conhecidos, interleaving de
instâncias, chamadas consecutivas e posições nas fronteiras do MT.

Isso valida um bloco com contexto aleatório explícito. Não integra os demais
blocos dependentes de contexto, o seed de boot, Oak, o primeiro mapa ou a campanha.
