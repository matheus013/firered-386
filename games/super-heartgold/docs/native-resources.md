# Recursos nativos — primeira integração com o filesystem original

O serviço autoral `hg::ResourceStore` faz leituras de arquivos comuns em C++20,
com raiz explícita, RAII, limites de leitura e rejeição de caminhos absolutos,
travessia e links que resolvam fora da raiz. Está na biblioteca da engine.
Não exige ROM ou CPU emulada durante a leitura dos recursos importados.

Um alvo privado separado compila o `src/filesystem.c` original do pin
`6984a3d67592157195940049be2e4bbbf933f9ff` diretamente como C x64.
As funções de arquivo desse módulo chamam o serviço host; alocações usam o heap
do processo. Não é recompilação ARM nem execução via ssrecomp/melonDS.

## Escopo e correção

Os três arquivos de referência são obtidos somente em uma pasta de build privada,
fora do Git, e verificados por SHA-256 antes da adaptação:

| Arquivo | SHA-256 original |
|---|---|
| `src/filesystem.c` | `ee526fc2835b2c2259dd393087fb1a5ca036dbf467bcfbd7735b1476819c869d` |
| `include/filesystem.h` | `50d8756e99ae5d980d85c4dfeb63291c99c1e17a47d3f810e693ef0237fa920e` |
| `include/filesystem_files_def.h` | `871b6abe3fb53e5ff810bb0b0680f2e305daff9670565e4d3eb9c13aebc3c5cf` |

`GetNarcMemberSizeByIdPair` abria um arquivo sem fechá-lo. O preparador aplica uma
correção explícita nessa função: `FS_CloseFile(&file)` antes de retornar.
O hash após essa única adaptação é
`f065b7d806a690132b54be35d4aa5c5ff5fd624ab14463d1fecad0009ea23f6b`.
O teste exige zero arquivos e alocações pendentes ao terminar cada arquivo NARC.

As declarações de compatibilidade foram escritas para este host; não incluem
implementação ou headers do Nitro SDK. O adaptador é uma prova privada de leitura
síncrona, não uma implementação completa de FS ou do sistema de heaps do DS.
`Heap_AllocAtEnd` usa a mesma alocação host: não simula arenas nem limites de memória
de cada heap. Falhas de arquivo abortam essa prova em vez de deixar o código C
continuar com dados inválidos. Multithreading e equivalência de erros SDK não são
suportados nesta etapa.

## Importação local

O importador lê FNT/FAT da ROM fornecida, valida diretórios e intervalos e importa
somente os NARC selecionados. Ele não extrai ARM9, ARM7 ou BIOS, não altera a ROM,
não sobrescreve arquivos existentes e recusa saída dentro do projeto ou de um Git.
A assinatura NARC é verificada; isso não substitui validação semântica completa
de cada recurso nem garante que dados arbitrários sejam apropriados para a engine.

Exemplo no Windows, com Python e uma pasta privada nova:

```powershell
python games/super-heartgold/tools/import_nds_resources.py "D:/jogos/SoulSilver-USA.nds" --output "D:/dados-privados/hg-recursos" --file a/0/0/2 --file a/0/1/1 --file a/0/2/7
cmake -S games/super-heartgold -B "D:/builds-privados/hg-recursos" -A x64 -DHG_BUILD_APP=OFF -DHG_REFERENCE_FILESYSTEM_PROBE=ON
cmake --build "D:/builds-privados/hg-recursos" --config Release
ctest --test-dir "D:/builds-privados/hg-recursos" -C Release --output-on-failure
& "D:/builds-privados/hg-recursos/bin/Release/hg_reference_filesystem.exe" "D:/dados-privados/hg-recursos" 2 a/0/0/2
```

O último caminho depende da configuração do gerador CMake. Ninja usa `bin/`;
Visual Studio usa `bin/Release/`. O exemplo é separado do local efetivamente
testado com MinGW x64/Windows via WSL. Os fontes privados não entram no executável
SDL3 padrão e a CI pública usa apenas fixtures sintéticas.

## Resultado local — 24/10/2026

| Recurso | ID NARC | Membros | Bytes de membros comparados |
|---|---:|---:|---:|
| Dados de Pokémon (`a/0/0/2`) | 2 | 508 | 22.352 |
| Dados de golpes (`a/0/1/1`) | 11 | 471 | 7.536 |
| Textos (`a/0/2/7`) | 27 | 829 | 4.615.284 |

Leitura de 1.808 membros comparada com os intervalos do arquivo original, usando
o C original compilado em PE x64: leitura inteira, parcial, por cursor, por posição
absoluta, por IDs e variantes com alocação. Nenhum interpretador de CPU participa.
Os bytes são comparados em memória, sem exportar seu conteúdo nos relatórios públicos.
Isso valida o transporte dos recursos, não decodificação de textos, cálculos de
batalha, gráficos, mapas jogáveis ou campanha.

Testes locais: 9/9 suítes da plataforma Windows, 10/10 da configuração privada
com filesystem e scheduler e 8/8 do núcleo em Linux. O importador possui sete
testes sintéticos, incluindo tabelas truncadas, ciclos, intervalos inválidos,
nomes inseguros, seleção ausente e proteção contra sobrescrita.
Os testes de links são condicionais à permissão do sistema e também passaram
no Linux, onde a criação de symlink estava disponível.
Resumo dos resultados e hashes em [native-resource-validation.json](native-resource-validation.json).

O próximo passo de integração é decodificar os formatos gráficos necessários ao
título e ligar esses recursos ao renderer host. Esta entrega ainda não executa
o fluxo completo da engine do jogo na plataforma SDL3.
