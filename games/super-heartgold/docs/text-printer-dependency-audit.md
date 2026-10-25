# Auditoria do impressor de texto para o port nativo

Data: 2026-10-24. Referência: `ClickingMaster/pokeheartgold-single-screen`, commit
[`6984a3d67592157195940049be2e4bbbf933f9ff`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).

O decodificador de mensagens, a fonte e o tokenizer próprios já oferecem os dados
necessários para começar um impressor incremental. Esta auditoria **não implementa
o `RenderText` original nem uma introdução jogável**. Os previews atuais continuam
distintos do sistema completo de texto do jogo.

## Fontes verificadas

Os arquivos foram lidos na cópia privada da referência. Não foram incorporados ao
repositório público. SHA-256 dos arquivos completos:

| Arquivo na referência | SHA-256 |
| --- | --- |
| `src/render_text.c` | `b5105ecb2b62d14081645c532e28018f22c41c6c937ee2fbd35f5bb0b2d11458` |
| `src/text.c` | `bd21839cb0d414a2fbe0b9f3c3ca29e3b101375ed3bf756f8e1fb9c95a24ce65` |
| `src/string_control_code.c` | `c93abcedbc4b9149f64f9caab1ced682deb3c21c4fb87743934ffca6887da694` |
| `src/font.c` | `43c7616ceba37f7bf86364c75c7236d9dee28781429465b4fc301f14f34e79ff` |
| `src/font_data.c` | `308e5083177eb936e1eb342ea191e3accdcc46fcc58fbb0bdeb86eaa84773ce5` |
| `include/render_text.h` | `2c9967396f4b0e1b059a8da43f38ca05a98b9010fd54b5bb6111fe8fc13e9ebf` |
| `include/text.h` | `6e870972ce09740816659647b99e879e8f83867daf7ebc0b04386102efccbaa3` |
| `include/font_types_def.h` | `322f1b4bd3ad4536987d70a0ee132b0dd1422869eb6f785ae32c2d17349809c3` |

## Protocolo de execução

`RenderText` está em `src/render_text.c:80–378`. Seus resultados numéricos, definidos
em `font_types_def.h`, são `PRINT=0`, `FINISH=1`, `REPEAT=2` e `UPDATE=3`.
O chamador `RenderFont` (`text.c:229–234`) executa novamente enquanto receber
`REPEAT`; um controle sem atraso não implica necessariamente um quadro adicional.

O caminho assíncrono de `AddTextPrinter` (`text.c:149–197`) subtrai um da velocidade
solicitada e registra uma tarefa. As velocidades 0 e 255 seguem o caminho imediato:
no máximo `0x400` chamadas a `RenderFont`, sem tarefa; 0 transfere a janela ao final,
255 adia essa transferência. Isso não equivale a executar todo texto arbitrário
instantaneamente, pois estados de espera continuam podendo retornar `UPDATE`.

`RunTextPrinter` (`text.c:199–227`) reseta o parâmetro de callback antes de renderizar.
`PRINT` atualiza a janela e pode chamar o callback; `UPDATE` também pode chamá-lo.
Callback com retorno diferente de zero suspende o avanço do texto e é chamado
novamente nas próximas execuções até liberar. `FINISH` destrói a tarefa. Existe
também uma suspensão global (`_021D1F6C`). O limite declarado é oito impressores.

## Estados exatos de `RenderText`

| Estado | Comportamento |
| --- | --- |
| 0 | Trata aceleração, atraso e próxima unidade. Desenha no máximo um glifo por retorno `PRINT`; LF e vários controles retornam `REPEAT`. |
| 1 | Espera confirmação sem desenhar a seta, limpa a seta ao confirmar e retorna ao estado 0. Retorna `UPDATE`. |
| 2 | Espera confirmação com seta; limpa a seta e o buffer da janela, restaura X/Y iniciais e volta a 0. |
| 3 | Espera confirmação com seta; restaura X inicial, define distância como altura máxima da fonte mais espaçamento entre linhas e vai a 4. |
| 4 | Rola para cima até quatro pixels por chamada; preenche com cor de fundo duplicada nos dois nibbles e transfere a janela. Quando já não há distância, volta a 0 em uma chamada posterior. |
| 5 | Volta a 0, retornando `UPDATE`. |
| 6 | Enquanto o contador é diferente de zero, decrementa; numa chamada posterior com zero, volta a 0. Retorna `UPDATE`. |
| 7 | Mesmo efeito de confirmação/limpeza do estado 2; é usado pelo controle `0x207`. |
| 8 | Mesmo efeito de confirmação/rolagem do estado 3; é usado pelo controle `0x208`. |
| Outros | Retorna `FINISH`. |

Não há atribuição dos estados 1 e 5 dentro de `RenderText`; isso não prova que
nenhum chamador externo os use. Não devem ser removidos como supostamente mortos.

No estado 0, a aceleração mantida zera o atraso. A/B mantidos exigem que uma
aceleração anterior tenha sido registrada no subestado; toque mantido depende das
flags de toque e da hitbox opcional. A/B novos ou toque novo podem zerar o atraso
durante a espera de caracteres. Não é correto substituir todos esses caminhos
por uma única flag de botão pressionado.

## Unidades e controles

As unidades são IDs do jogo, não Unicode. `0xF100` é um marcador de nome compactado
que deve ter sido expandido antes: o original o rejeita com uma asserção.

| Unidade | Efeito no estado 0 |
| --- | --- |
| `0xFFFF` | Finaliza. |
| `0xE000` | X inicial; soma altura máxima da fonte e espaçamento a Y; `REPEAT`. |
| `0xF0FD` | Consome exatamente uma unidade seguinte sem desenhá-la; `REPEAT`. |
| `0x25BC` | Inicializa seta/espera e vai ao estado 2; `UPDATE`. |
| `0x25BD` | Inicializa seta/espera e vai ao estado 3; `UPDATE`. |
| `0xFFFE` | Cabeçalho estendido: comando de 16 bits, quantidade de argumentos de 16 bits e essa quantidade de unidades. |
| Outras | Carrega/desenha o glifo, incrementa X pela largura mais espaçamento e retorna `PRINT`. |

Controles estendidos reconhecidos pelo original:

| Comando | Efeito |
| --- | --- |
| `0xFF00` | Usa argumento 0 para cores. Valores a partir de 100 guardam a seleção; `0xFF` troca seleção atual e guardada quando esta está entre 100 e 106. A seleção efetiva define frente como `2*n+1` e sombra como `2*n+2`, reconstruindo a tabela de cores. |
| `0x0200` | Indicador de foco de tela com argumento 0, **compilado fora quando `SINGLE_SCREEN` está definido**. |
| `0x0201` | Argumento 0 define atraso; avança o comando e vai ao estado 6; `UPDATE`. |
| `0x0202` | Argumento 0 define o parâmetro de callback; avança o comando; `UPDATE`. |
| `0x0203` | Define X atual pelo argumento 0. |
| `0x0204` | Define Y atual pelo argumento 0. |
| `0x0205` | Centraliza a primeira linha restante se couber na largura da janela; caso contrário usa X inicial. A consulta de largura recebe o ponteiro ainda no comando atual. |
| `0x0206` | Alinha à direita a primeira linha restante se couber; caso contrário usa X inicial. Quando cabe, o cálculo original usa largura da janela menos largura do texto, sem somar X inicial. |
| `0x0207` | Vai ao estado 7; avança o comando e consome um LF imediatamente seguinte, se houver; `UPDATE`. |
| `0x0208` | Vai ao estado 8; avança o comando e consome um LF imediatamente seguinte, se houver; `UPDATE`. |
| `0xFF01` | Argumento 0 igual a 100 seleciona `glyphTable=0`; igual a 200 seleciona `glyphTable=0xFFFC`. Ambos zeram `unk1A`; outros valores não alteram esses campos. |
| `0xFE06` | Argumento 0 igual a `0xFE00` vai ao estado 3; igual a `0xFE01` vai ao estado 2. Inicializa a espera e retorna `UPDATE`; demais valores apenas avançam. |
| Outros | Avança pelo tamanho declarado e retorna `REPEAT`; não expande variáveis. |

Os controles que não retornam antecipadamente avançam pelo cabeçalho/argumentos
e retornam `REPEAT`. A classificação de variáveis de `MsgArray_ControlCodeIsStrVar`
reconhece os bytes altos `0x01`, `0x03`, `0x04` e `0x34`; a expansão correspondente
deve ocorrer antes da renderização. Preservar um comando desconhecido no tokenizer
não equivale a implementá-lo no impressor.

## Dependências que precisam de implementação nativa

| Área | Contrato necessário |
| --- | --- |
| Estado | Cursor, template, subestado de fonte/seta, cores, parâmetros e callback com vida útil garantida. Estruturas com ponteiros/bitfields do DS não podem ser serializadas ou copiadas como ABI x64. |
| Fontes | Carregar glifos e suas dimensões, atributos da fonte, largura da primeira linha e espaçamento. O decoder de fonte existente é uma base, mas não substitui automaticamente todos esses serviços. |
| Janelas | Buffer com dimensões, preenchimento, cópia de glifo com a tabela selecionada, recorte e rolagem. Em `bg_window.c`, os pontos principais são `CopyGlyphToWindow:1933` e `ScrollWindow:2017`. |
| Apresentação | Equivalente nativo de transferir a janela ao quadro e de atualizar as regiões da seta. A janela SDL existente pode apresentar pixels; ainda falta ligar os efeitos do impressor à janela de diálogo real. |
| Entrada | A/B novos e mantidos, toque novo/mantido e teste de hitbox. Testes devem separar eventos novos de estado mantido. |
| Temporização | Uma etapa lógica por tick; repetição interna de `REPEAT`, atrasos, callbacks suspensivos e descarte/cancelamento de tarefas. |
| Seta | Carregamento dos tiles, animação `0,1,2,1`, atualização/limpeza e modo alternativo. O índice ocupa **dois bits**, portanto sua volta a zero não requer presumir acesso fora da tabela após a quarta posição. |
| Áudio | Confirmação chama `PlaySE(SEQ_SE_DP_SELECT)`. Registrar o pedido é possível em testes, mas não deve ser anunciado como som reproduzido. |
| Flags globais | Flags de autoavanço, aceleração, toque e continuação são globais ao módulo original. Sessões independentes precisam de um contrato explícito para compartilhar ou isolar esse estado. |

No modo automático, o contador de espera começa em zero e só libera quando uma
chamada já o encontra em 100: são 100 incrementos e liberação na chamada seguinte.
A confirmação antecipada é permitida somente quando a flag correspondente está
habilitada. O modo alternativo impede desenhar e limpar a seta pelos caminhos
usuais. Esses detalhes exigem testes por tick, não apenas capturas finais.

`GetStringWidthFirstLine` (`font_data.c:228–246`) ignora controles estendidos e para
em EOS/LF. Soma largura mais espaçamento e subtrai o espaçamento ao final em `u32`;
linha vazia com espaçamento não zero pode produzir underflow. Também não possui
tratamento específico para `0xF0FD` nessa função. Um port precisa registrar
explicitamente qualquer correção em vez de supor que sua política própria já é
idêntica ao original.

## Subconjunto útil e validação realizada

O primeiro subconjunto real recomendado é texto incremental comum, LF, pausa,
confirmação e limpeza de página em uma janela de pixels, com fonte nativa e
observação do cursor/estado a cada tick. Isso pode atender trechos da introdução
antes de existir o sistema completo de mapas. Precisa de testes próprios e de
comparação privada com as decisões originais; esta auditoria não declara esse
subconjunto implementado.

As quatro funções pequenas de `string_control_code.c` são um subconjunto puro
aproveitável como oráculo privado: obter comando, obter campo, avançar controle e
classificar variável. Nesta auditoria, os corpos originais foram compilados em
um harness C11 privado, com `-Wall -Wextra -Werror`, ASan e UBSan. O harness testou
os **65.536 comandos**, combinados com tamanhos **0, 1, 2 e 65.535**:
**262.144 casos passaram**. Foram conferidos o avanço exato, a classificação de
variáveis e os campos primeiro/último quando presentes. Entradas eram válidas e
inteiramente alocadas; isso não prova segurança dessas funções originais para
dados truncados, pois elas usam ponteiros e asserções sem um limite de buffer.

Separadamente, o tokenizer autoral passou sete testes com os mesmos sanitizers,
incluindo truncamentos, limites, preservação de EOS em argumentos e propriedade
dos dados. Os **829 bancos / 49.984 mensagens** privados foram tokenizados sem
rejeição, gerando **2.055.899 tokens**. Essa cobertura valida a leitura estrutural
do corpus disponível; não demonstra cores, rolagem, expansão de variáveis,
callbacks ou temporização de todo o jogo.

Artefatos da auditoria dos helpers permanecem fora do Git em
`/root/.cache/super-heartgold/text-command-tests/original-control-audit.c` e no
executável correspondente. Nenhuma ROM, mensagem original ou função original foi
adicionada ao repositório por esta auditoria.
