# Verificação reproduzível de tokens no corpus completo

`tools/verify_text_commands_corpus.py` compila os fontes C++ de produção de
mensagens, comandos de texto e NARC em um executável temporário. A ferramenta
lê um NARC privado, valida sua estrutura, percorre todos os bancos e mensagens,
decodifica cada mensagem e tokeniza seus comandos. Depois reconstrói os code
units a partir dos tokens e exige igualdade exata, incluindo offsets, comandos,
quantidades de argumentos, argumentos e terminador.

```sh
python3 tools/verify_text_commands_corpus.py --archive /caminho/privado/a/0/2/7
```

É necessário um compilador nativo C++20 compatível com opções GNU (`c++` por
padrão; selecione outro com `--compiler`). O executável temporário usa somente
os fontes autorais do projeto e não precisa de ROM, SDK, Unicorn ou código C
original. A entrada é enviada em memória pelo stdin binário, inclusive no Windows;
o caminho privado não é passado ao programa C++.

Limites: arquivo de no máximo 16 MiB, 1.048.576 mensagens, 16.777.216 code units
acumulados, limites individuais dos decoders de produção e 120 segundos por
compilação/execução. O harness fica em diretório temporário fora do Git e é
removido ao terminar. A ferramenta não imprime texto, IDs de glifos ou argumentos
de comandos; a saída contém somente contagens, tamanho, hash e resultado.

Execução local de 24/10/2026, usando o arquivo privado previamente extraído:

| Medida | Resultado |
| --- | --- |
| Bancos | 829 |
| Mensagens | 49.984 |
| Mensagens de nomes compactados | 738 |
| Code units decodificados | 2.106.649 |
| Tokens | 2.055.899 |
| Unidades de argumentos | 22.898 |
| Roundtrip de todas as mensagens | Passou |
| Tamanho do arquivo | 4.622.756 bytes |
| SHA-256 | `570433fadb7f2959c99e73ee446ad7bc770702dd71b472acf51c0e358a51136c` |

Os contadores por tipo seguem a ordem de `TextTokenKind`: Glyph=1.943.460,
LineFeed=32.376, End=49.984, ClearPage=12.399, Scroll=3.754, SkipOperand=0,
ExtendedControl=13.926. O corpus não contém SkipOperand; os testes sintéticos
do tokenizer continuam necessários para esse formato e entradas inválidas.

Esta verificação prova preservação estrutural dos tokens para esse corpus. Não
executa comandos, substitui variáveis, valida renderização de páginas, compara
efeitos com o jogo original ou testa a campanha. O arquivo privado e seu conteúdo
não são adicionados ao repositório.
