# Decisão original de check-save com dados controlados

O build privado `HG_REFERENCE_TITLE_FLOW=ON` agora inclui a função original
`CheckSavedataApp_DoMainTask`, compilada diretamente para x64. Depois de o título
solicitar o menu, o adaptador chama essa decisão com flags controladas de teste.
O enum e a função são selecionados de um fonte pinado; o corpo não é
reescrito. Os serviços de impressão/fade são implementações host diagnósticas.

**Isso não é um leitor ou validador de saves.** Não há leitura, escrita, recuperação
ou exclusão de arquivos de save. Flags zero significam somente ausência de bits
de erro no dado controlado; não comprovam ausência de save ou integridade de dados.
A janela e os logs indicam `CONTROLLED ... NO SAVE FILE LOADED`.

## Comportamentos testados

- Prioridade dos seis bits de status.
- Corrupção total suprime o aviso de fallback do mesmo grupo.
- Mensagens de dados principais, Battle Hall e Battle Video seguem a ordem original.
- Confirmação não pula uma mensagem antes de sua apresentação.
- Conclusão e repetição após concluir são estáveis; bits desconhecidos terminam sem
  serem tratados como mensagens conhecidas.
- Sessões intercaladas não misturam flags ou confirmação.

A decisão pode ser executada com `--title-flow --checksave-flags N`, em que N é
um inteiro decimal de diagnóstico, não um resultado obtido de um save. Por padrão
N é zero. X confirma mensagens; o menu completo continua pendente.
`--title-flow-self-test` injeta Enter no frame 40 e pulsos de X a partir do frame
120. Use `--frames 360` para testar várias mensagens. Sem dados de fonte, a UI usa
resumos próprios. A opção `--checksave-text PATH` usa os textos originais do banco
229 e a fonte 1, conforme `CheckSavedataApp_PrintMessage`, decodificados no host.
A duração do fade continua sendo aproximação host.

As mensagens 2 e 4 possuem o comando de limpar página `0x25BC`. O adaptador
separa essas páginas e consome X para passar à próxima, entregando a confirmação
à decisão C somente depois da última página. A impressão é instantânea por
página; o task original de texto, animação da seta, scroll e outros comandos
continuam pendentes. Não interpretar esses comandos como glifos de substituição.

```powershell
SuperHeartGold.exe --title-resources "D:/dados-privados/titulo" --checksave-text "D:/dados-privados/texto" --title-flow --checksave-flags 1
```

O caminho de texto deve conter os NARCs privados `a/0/1/6` e `a/0/2/7`. As texturas
são gerenciadas por RAII e destruídas antes do renderer SDL, inclusive quando a
preparação de uma página falha. Os dados e o executável privado ficam fora do Git.

`tests/test_title_text_renderer.py` valida o binário privado com assets sintéticos:
seleção de mensagens por flags, pixels antes/depois da troca de página, sequência
com flags 63, erro de recursos e encerramento. O job privado do CI compila SDL e
roda esse teste sem ROM e sem publicar seu executável.

No encerramento aparece `DECISION COMPLETE - MAIN MENU / REAL SAVE IO NOT PORTED`.
Essa parada é um limite explícito da integração, não uma partida iniciada.

## Proveniência e fronteira de implementação

Base: `6984a3d67592157195940049be2e4bbbf933f9ff`.

- Fonte: `src/application/check_savedata.c`.
- SHA-256 do fonte: `f0132f1d62e38aa6358144ed971163336fd09377e9d708e3ceef083012e26016`.
- SHA-256 da função selecionada: `e854ccec22b07d8010c166842aafef3f4a644b02d96e05a6e04aee41c0e3f812`.

Os fontes são preparados fora do Git, com verificação de hash. Os hooks `HGCheck_*`
são distintos dos hooks do título e preservam o contexto por chamada, evitando
colisões na ligação e contaminação entre sessões. Somente os campos usados pela
decisão são declarados no adaptador: não são o formato de arquivo de SaveData.

## O que a auditoria encontrou para a próxima etapa real

No pin da base, `Save_GetStatusFlags` (`src/save.c:222`) apenas devolve um campo.
As flags reais vêm de `SaveData_New` após validação e carregamento. O formato usa
flash de 512 KiB, bancos em 0 e `0x40000`, dois chunks principais e 42 blocos com
tamanhos derivados do layout DS. Rodapés incluem tamanho, magic `0x20060623`, índice
e CRC. A escolha entre cópias trata consistência dos chunks e wrap de contador;
não equivale a escolher simplesmente o maior contador.

O CRC chama `MATH_CalcCRC16CCITT` pelo SDK. Embora o SDK não esteja no clone, seus
parâmetros e 14 vetores já foram confirmados nas funções ARM9 originais isoladas.
O leitor/validador nativo dos blocos principais está implementado separadamente;
veja [native-save-validation.md](native-save-validation.md). Os registros extras
possuem redundância própria e podem gerar outros bits de erro.

O menu original seleciona New Game automaticamente se não existe save
(`application/main_menu/main_menu.c:1391–1395`). A inicialização da partida em
`overlay_36.c:251–260` exige os 42 blocos e valores de posição, dinheiro e flags.
Não substituir esses inicializadores por sucesso fictício para avançar de tela.

Próximo trabalho: registros extras, interpretação dos 42 blocos e carregamento de
estado real. O leitor principal ainda não fornece as flags completas deste fluxo.
Escrita e recuperação persistente dependem de testes efetivos de salvamento.

Resultados e hashes desta etapa: [checksave-validation.json](checksave-validation.json).
Integração posterior da fonte: [checksave-native-font-validation.json](checksave-native-font-validation.json).
