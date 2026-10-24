# Descompressão LZ na engine host

Decoder autoral C++20 para LZ10 e LZ11, integrado ao caminho de leitura das
camadas NCGR/NCLR/NSCR. Cada recurso pode estar cru ou ter uma camada LZ.
O NARC externo continua sem compressão, para preservar o contrato de views
emprestadas por `narc_member`.

O formato foi conferido com `tools/nitrogfx/lz.c` no commit de referência
`6984a3d67592157195940049be2e4bbbf933f9ff`. Nenhum código original foi copiado
para o decoder. A função `decompress_nitro_lz` retorna um buffer próprio; os
buffers temporários permanecem vivos até a conclusão da conversão RGBA.

## Escopo aceito

- Tipos exatos `0x10` e `0x11`, tamanho de saída não zero em 24 bits.
- Todas as três formas de comprimento LZ11, incluindo referências longas.
- Distâncias até 4096 e cópia sobreposta, necessária para sequências repetidas.
- Limite de saída configurável, padrão 16 MiB, conferido antes de alocar.
- Bytes posteriores à saída declarada são ignorados, permitindo alinhamento.

Entradas truncadas, referência anterior ao começo e token que excede a saída
declarada são rejeitados. Neste último caso o comportamento é mais restrito que
o utilitário original, que limita a cópia e emite aviso para alguns tilesets de
Ruby/Sapphire. Portanto, não se afirma compatibilidade de aceitação com todo
arquivo tolerado por aquele utilitário.

Cabeçalho estendido de 32 bits com tamanho inicial zero, compressão em camadas,
Huffman, RLE, LZ reverso e NARC externo comprimido ainda não são suportados.

## Validação

- Nove casos de teste do decoder passaram em Windows x64 e em Linux com
  ASan/UBSan. Exercitam comprimentos, flags, distância máxima, sobreposição,
  truncamento em cada posição dos exemplos, limites e erros de formato.
- O decoder gráfico compara pixels crus/comprimidos em 32 combinações de
  recursos, tipos LZ e profundidades de cor.
- O teste do renderer SDL executa o binário real com fixtures próprias cruas,
  LZ10 e LZ11, conferindo os pixels capturados, composição e escala nearest.
- O arquivo privado real de título possui 44 membros e nenhum deles está
  originalmente comprimido em LZ10/LZ11. Para a comparação diferencial, esses
  membros foram comprimidos pelo `LZCompress` original, nos dois formatos e
  nas duas direções de busca, com alinhamento. Nas **176 codificações geradas**,
  nosso decoder e o `LZDecompress` original retornaram os bytes de origem.
  Essas codificações são fixtures derivadas para teste, não amostras de
  compressão originalmente presentes na ROM.

Recursos de origem, codificações geradas e resultados individuais privados ficam
fora do Git. O relatório local é
`/root/.cache/super-heartgold/compression-review/title-generated-differential.json`.
O executável final usa somente C++ nativo nesse caminho, sem executor ARM ou SDK.
