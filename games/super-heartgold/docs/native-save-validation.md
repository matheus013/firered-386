# Leitura e validação nativa dos blocos principais

Implementação autoral C++20 em `hg_game_data`, independente de SDL, CPU DS e ABI
de estruturas x64. O componente lê uma imagem flash bruta de exatamente 512 KiB,
mantém uma cópia imutável e verifica os quatro blocos principais normal/PC.
Não há função de escrita, reparo, exclusão ou criação de save nessa interface.

## Evidência do formato

Os tamanhos, offsets e CRC foram confirmados nas funções originais executadas
isoladamente sobre memória sintética, a partir da imagem ARM9 privada verificada.
Os resultados e a derivação das 42 subestruturas estão em
[save-format-investigation.md](save-format-investigation.md).

É possível repetir essa prova privada sem copiar código original para o Git:

```sh
python3 tools/verify_save_layout.py --arm9 /caminho/privado/rc_image_0.bin
```

Essa ferramenta opcional exige o pacote Python `unicorn`. Ela verifica o SHA-256
da imagem antes de executar funções com limites de instruções. O runtime do jogo
e o inspetor nativo não usam esse pacote. A saída contém apenas metadados e CRCs
de entradas sintéticas. As 42 funções de tamanho, a inicialização completa dos
offsets e os 14 vetores CRC coincidiram na execução local.

A política de escolha de banco foi comparada em **38.416 casos** com quatro
funções C exatas da referência pinada. A comparação inclui empate, escrita
interrompida, pares inconsistentes e avanço `0xFFFFFFFF → 0`. Procedimento e
limites em [save-selection.md](save-selection.md).

## Usar o inspetor

Compile pelo CMake habitual. A ferramenta também é copiada pelo script WSL para
`out/windows/hg_save_inspect.exe`:

```powershell
.\games\super-heartgold\out\windows\hg_save_inspect.exe "C:\copia-local\jogo.sav"
```

Ela imprime JSON com `scope: "primary_chunks_only"`, resultado da seleção
original, banco aceito para leitura dos blocos principais e erros por bloco.
Não imprime nome do jogador ou conteúdo de Pokémon. Caminhos Unicode foram
testados no executável Windows. Códigos de saída:

| Código | Significado |
| --- | --- |
| 0 | Par principal selecionado com tamanho, magic, ID, CRC e contadores coerentes |
| 2 | Nenhum par principal aceito |
| 1 | Argumentos inválidos ou falha de leitura/formato de arquivo |

Arquivos menores, maiores ou com cabeçalho/trailer de emulador são rejeitados.
Não existe conversão implícita. `no_save` reproduz o resultado do seletor quando
nenhum bloco é válido; não distingue toda corrupção possível de uma flash vazia.

## Proteção contra um caso de seleção original

O seletor original pode escolher, no retorno do contador a zero, um bloco PC
inválido cujo contador fictício também é zero. A seleção de referência é
preservada para comparação, mas `readable_primary_bank` fica vazio nesse caso.
Assim como a etapa posterior de carga original volta a verificar os blocos, o
consumidor nativo não deve tratar o status inicial `good` como validação completa.

## Testes e limites

Testes C++ cobrem leitura com limites, overflow, snapshot independente, movimento
do objeto, endianness, fronteira do rodapé, tamanho de arquivo, preservação dos
bytes/data de modificação, cinco vetores CRC, identidade dos rodapés, corrupção
do primeiro/último byte de payload, recuperação e retorno do contador.

O teste Python executa o binário real com fixtures geradas independentemente por
`binascii.crc_hqx`, verifica o JSON, códigos de saída e preservação dos arquivos.
Inclui diretórios e nomes Unicode, ausência de arquivo, truncamento, padding,
todos os tipos de erro de rodapé e casos inconsistentes.

O comando de teste independente é:

```powershell
python games/super-heartgold/tests/test_save_inspector.py games/super-heartgold/out/windows/hg_save_inspect.exe
```

Essas fixtures são sintéticas e não representam partidas jogáveis. Os registros
extras, os 42 blocos de estado de jogo, a descriptografia de Pokémon, as rotinas
de nova partida, a persistência host e a opção Continue ainda não estão
integrados. O launcher continua usando flags controladas no check-save; não
consome automaticamente arquivos locais nem declara um save real válido.
