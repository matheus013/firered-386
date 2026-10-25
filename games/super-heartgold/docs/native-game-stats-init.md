# Inicializador de estatísticas com contador explícito

Em 25/10/2026, `initialize_game_stats(vblank_count)` reproduz o bloco bruto
GameStats (ID16, 448 bytes) da edição SoulSilver USA investigada. É uma função
autoral nativa que recebe o contador lógico do chamador; não lê relógio,
hardware, save ou globais. Não implementa incrementos, regras de pontuação ou
persistência das estatísticas.

O contrato mantém o contador estável durante a inicialização. No original,
`GameStats_Init` consulta duas vezes o mesmo contador de VBlank. A função host
reproduz esse caso estável, sem modelar uma interrupção entre as duas leituras.
A escolha e evolução do contador no boot ainda precisam ser integradas.

## Layout e codificação

Os primeiros oito bytes ficam sem codificação. A região de 436 bytes em +8
começa zerada e recebe o XOR do formato Pokémon. Seu checksum inicial é zero
em +444; a chave de 16 bits em +446 é a truncagem de
`counter | (counter << 8)`. O seed do segmento é essa chave deslocada 16 bits.
Não se usa `sizeof` de um struct host para deduzir esse layout.

Os campos e a expressão vêm de `src/game_stats.c` no pin
`6984a3d67592157195940049be2e4bbbf933f9ff`. A comprovação independente executa
as funções originais da imagem ARM9 cujo SHA-256 é
`5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8`.

## Prova privada

```sh
python3 tools/verify_game_stats_init.py --arm9 /imagem/privada/rc_image_0.bin
```

A ferramenta verifica o hash da imagem e o descritor16 original. As funções
de tamanho e inicialização são `0x0202CF1D` e `0x0202CF25`; a referência ao
contador compartilhado `0x027FFC3C` é conferida na imagem. A memória sintética
recebe o contador antes de executar o inicializador. Nenhum serviço que decide
os bytes do bloco é substituído.

O C++ de produção é compilado num harness temporário. Os bytes foram comparados
em **32 execuções**: oito contadores, incluindo zero, fronteiras e
`0x12345678`/`0xFFFFFFFF`, combinados com quatro preenchimentos anteriores.
Também são conferidos tamanho bruto 448, retorno dentro de 100.000 instruções,
guardas antes/depois e ausência de escritas fora do bloco e da stack sintética.
Todas as comparações passaram. A imagem e os payloads não são publicados.

Os testes C++ conferem campos sem codificação, chave, checksum e plaintext
depois de desfazer o XOR. Junto do [inicializador de roamers](native-roamer-init.md),
esta API acrescenta contexto explícito aos
[26 inicializadores independentes de contexto](newgame-dependency-audit.md).
O conjunto continua parcial; não inicia uma partida.
