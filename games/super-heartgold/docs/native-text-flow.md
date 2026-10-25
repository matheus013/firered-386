# Impressor original em execução nativa privada

O build opcional `HG_REFERENCE_TEXT_FLOW=ON` executa o corpo original de
`RenderText`, sete helpers de decisão e a medição original de primeira linha
como C compilado para x64. A preparação
confere o SHA-256 do arquivo e de cada função na referência pinada. Os fontes
selecionados ficam numa pasta de build fora do Git; não integram o executável
público padrão. Não há CPU DS nesse caminho.

O adaptador `TextFlowSession` fornece janela de pixels, fonte própria de HG/SS,
entrada A/B e estado por sessão. Cada tick repete somente os resultados
`REPEAT`, com orçamento limitado pelo texto validado, e para em `PRINT`,
`UPDATE` ou `FINISH`. O texto e os argumentos são verificados antes da entrada
na função C. Nenhum hook lança exceções C++ através dessa fronteira.

## Escopo implementado

- Glifos incrementais, LF e operandos descartados por `F0FD`.
- Atraso entre caracteres e aceleração por A/B novos ou mantidos.
- Pausa `0201` com argumento de 0 a 255.
- Limpeza de página `25BC` e `0207`; rolagem `25BD` e `0208`.
- Ambas as variantes `FE06`: `FE00` para rolar e `FE01` para limpar.
- Posição X/Y por `0203`/`0204`, centralização `0205` e alinhamento à direita
  `0206`, com a função original de medição e espaçamento entre letras zero.
- Espera automática de 100 incrementos, liberada na chamada seguinte, com
  confirmação antecipada opcional.
- Limpeza e rolagem reais do buffer, incluindo passos de até quatro pixels.

O serviço de altura usa 16 pixels por padrão, conforme `maxLetterHeight` da
fonte 0 original. Fixtures podem declarar outra altura. O fundo original de
índice 15 é representado pelo índice 0 no buffer público; papéis transparentes
não escrevem, e o papel de fundo escreve. A janela host recorta os desenhos.

Seta e áudio são **pedidos observáveis**: o adaptador SDL mostra uma seta
autoral, e o core conta pedidos do som de confirmação. Tiles/animacão originais
da seta e reprodução sonora não foram portados. Touch, controles
de cor, callbacks suspensivos, expansão de variáveis e scheduler global de
impressores ficam pendentes. Controles fora da lista são rejeitados antes da
execução; não são ignorados para fabricar uma impressão bem-sucedida.
Alinhamento combinado com `F0FD` é rejeitado: o helper original de largura não
trata esse marcador como o impressor, e essa combinação ainda não foi portada.

## Compilar e executar

Use uma pasta privada fora de qualquer checkout Git:

```sh
cmake -S games/super-heartgold -B /pasta/privada/hg-text-flow -DHG_REFERENCE_TEXT_FLOW=ON
cmake --build /pasta/privada/hg-text-flow --parallel 3
ctest --test-dir /pasta/privada/hg-text-flow --output-on-failure
```

Com as fontes e mensagens importadas privadamente, o executável aceita:

```sh
SuperHeartGold --text-resources /dados/privados/texto --text-flow
SuperHeartGold --text-resources /dados/privados/texto --text-flow-self-test --frames 360 --video-driver dummy
```

Os argumentos existentes `--message-bank` e `--message-id` selecionam a mensagem.
O padrão é 219/1. X/Z confirmam páginas e aceleram a impressão. O autoteste usa
um tick por quadro, confirmações sintéticas e falha se o texto não terminar.
Isso não inicia o overlay da introdução, não cria uma partida e não acessa saves.

## Validação

Os testes unitários executam a função C original, não uma cópia da sua lógica
em C++. Conferem ticks de atraso, pausa e retomada; glifos e posições; limpeza;
rolagem de pixels; consumo de LF após controles; autoavanço; aceleração;
isolamento entre sessões; rejeição de comandos e transparência após wrap do
cursor original de 16 bits. O teste SDL usa recursos sintéticos autorais e
capturas do executável Windows real.

A leitura de todo o corpus está documentada separadamente em
[text-command-corpus.md](text-command-corpus.md). Tokenizar as 49.984 mensagens
não significa que todos os comandos do jogo já possam ser executados.
