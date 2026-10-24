# Mensagens e fontes no renderer nativo

O port possui agora leitura autoral das mensagens criptografadas e dos glifos da
fonte de HG/SS. A opção `--text-resources` mostra uma mensagem estática usando os
pixels originais no renderer SDL3. Não executa a introdução, o impressor de texto
original, scripts, escolhas, placeholders ou estado de partida.

## Formato e comportamento

`message_data` lê um membro do NARC de mensagens: cabeçalho count/key, tabela
offset/comprimento cifrada e palavras de 16 bits cifradas por entrada. Retorna
IDs de glifos e códigos de controle, **não Unicode**. O caminho bruto preserva
todos os códigos e argumentos, incluindo o EOS final. Os nomes com marcador
`0xF100` usam glifos de 9 bits em palavras de **15 bits**, com EOS `0x1FF`.
O decoder expande esses nomes, convertendo o EOS para `0xFFFF`.

Offsets, alinhamento, tamanho da tabela, comprimentos, EOS e limites de alocação
são verificados. O decoder rejeita mensagens sem EOS final e bits reservados
inválidos nas palavras compactadas. Isso é validação estrutural; não é um MAC ou
outra autenticação criptográfica da mensagem.

`NitroFont` lê o formato próprio de fonte usado por `font_data.c`, não NFTR.
Suporta células de 8x8, 8x16, 16x8 e 16x16, com larguras fixas ou variáveis.
As palavras de cada linha armazenam pixels de 2 bits, começando pelos bits mais
significativos; as quatro funções visuais são transparente, frente, sombra e
fundo. Os IDs são baseados em 1. A substituição original usa o glifo 428 quando
ele existe; ID zero e referências impossíveis são rejeitados.

O preview usa a fonte 0 de `a/0/1/6` e mensagens de `a/0/2/7`, com uma moldura
diagnóstica autoral. Suporta glifos, EOS e LF (`0xE000`). Comandos, placeholders,
mensagens que exigem quebra automática ou paginação causam erro explícito. Eles
continuam preservados no decoder bruto, para a integração futura do impressor.

## Executar

Importe os dois arquivos para uma pasta privada nova, fora do Git:

```powershell
python games/super-heartgold/tools/import_nds_resources.py "D:/jogos/SoulSilver-USA.nds" --output "D:/dados-privados/hg-texto" --file a/0/1/6 --file a/0/2/7
games/super-heartgold/out/windows/SuperHeartGold.exe --text-resources "D:/dados-privados/hg-texto"
```

O padrão é banco **219**, mensagem **1**, usado pela introdução do Oak no pin da
base. `--message-bank N --message-id N` selecionam outra entrada. Trata-se apenas
da apresentação de uma mensagem existente, sem transição para uma partida.
Não combine `--text-resources` com o teste do título.

O teste estático não abre a ROM, não acessa saves e não executa uma CPU DS.
Fonte, textos e capturas permanecem privados. O launcher local de texto fica em
`out/windows/Abrir-Texto-Nativo.cmd`; não é um artefato público com dados do jogo.

## Verificação independente

As funções C originais são extraídas somente durante os testes opcionais para
uma pasta temporária fora do Git. Os arquivos fonte são conferidos por SHA-256;
nenhum desses trechos é incorporado ao produto ou versionado pelos testes.

```sh
python3 tools/verify_messages_reference.py --source-dir /referencia-pinada/src --archive /dados-privados/a/0/2/7
python3 tools/verify_fonts_reference.py --source /referencia-pinada/src/text.c --archive /dados-privados/a/0/1/6
```

Resultados locais em 24/10/2026:

| Verificação | Resultado |
| --- | --- |
| Mensagens versus `Decrypt1`/`Decrypt2` originais | 49.984 mensagens de 829 bancos coincidiram |
| Nomes versus `String_Cat_HandleTrainerName` original | 738 nomes compactados coincidiram |
| Expansão de glifos versus duas funções originais de `text.c` | 3.054 glifos em 6 fontes; 336.112 pixels visíveis coincidiram |
| Testes C++ de mensagens | 6 casos, incluindo controles preservados, fronteiras de packing, limites e corrupção |
| Testes C++ de fonte | 5 casos, incluindo formas, ordem dos pixels, métricas, truncamento e substituição |
| Renderer Windows com fixtures próprias | Glifos, LF, escala nearest, caminhos/capturas Unicode e erros passaram |
| Execução com recursos originais | Mensagens estáticas exibidas em software e em Direct3D 11 |
| Core com ASan/UBSan | 16/16 suítes passaram |

Os testes de recursos originais imprimem somente contagens, não o conteúdo das
mensagens. O teste público `tests/test_text_renderer.py` usa fonte e mensagens
sintéticas autorais e confere os pixels do executável real.

Ainda faltam formatação de variáveis, comandos de impressão, paginação, animação
de texto e ligação desses serviços aos overlays do jogo. Exibir uma fala do Oak
não significa que o controlador da introdução ou a campanha já foram portados.
