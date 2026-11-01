# Entrada do gameplay nativo — 01/11/2026

Prioridade: primeiro mapa controlável, conforme [mínimo jogável](minimum-playable.md).
A cinematica não é mais o caminho crítico desta investigação.

## Bloqueio removido

`overlay_36.c`, responsável pela inicialização de Nova Partida/Continue, falhava
por falta dos headers de índices de mensagens 0292, 0293 e 0445. Ampliado
`prepare_hgss_message_headers.py` para aceitar módulos explicitamente e gerar
também os índices dos bancos incluídos por header. Não foram inventados IDs.

Os índices vieram dos GMM pinados do projeto pret, conferidos contra a quantidade
de entradas do arquivo privado SoulSilver USA, com checksum obrigatório. Foram
gerados dez headers, incluindo 20, 20 e 2 entradas nos três novos bancos.
Só os metadados de proveniência são publicados; texto e recursos ficam privados.

Consultados antes da adaptação:
[Nova Partida original](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/overlay_36.c)
e os índices de mensagem no repositório pret. A geração utiliza o pin
`a6ebf56ab7a454d91cc4715c73b62e93c305dc29`, não o formato mais recente do upstream.

## Verificação realizada

Compilados integralmente para Linux x86-64, com os flags atuais do diagnóstico:

- `overlay_36.c`: entrada de Nova Partida/Continue e inicialização pós-Oak.
- `field_system.c`: criação e ciclo do sistema de campo.
- `field_warp_tasks.c`: entrada e mudanças de mapa.
- `battle/battle_setup.c`: preparação dos dados de batalha.

O link relocável dos quatro objetos passou; **não é um executável**.
[Relatório](hgss-gameplay-entry-report.json) e
[proveniência dos índices](hgss-gameplay-message-provenance.json).

O conjunto referencia 284 símbolos externos diretos após descontar definições
entre os quatro módulos. Sete estão definidos no diagnóstico gráfico atual e
277 não estão. **Isso não significa 277 funções para implementar**: quase todos
têm candidatos nas fontes originais, alguns são globais e símbolos de toolchain;
o diagnóstico também elimina código não usado no link. Nem um símbolo presente
prova compatibilidade de ABI ou comportamento.

## Próximo trabalho essencial

1. Expandir a integração a `FieldSystem_Init`, objetos/jogador, carregamento de
   mapa e script manager. Inspecionar as dependências transitivas antes de
   substituir qualquer serviço por um adaptador.
2. Definir a fronteira de recursos/SaveData e registrar as aplicações originais
   necessárias ao quarto, preservando os efeitos da inicialização pós-Oak.
3. Ligar um executável de campo com erros explícitos para serviços ausentes,
   então verificar o quarto no Windows com entrada e single screen.

Não houve execução de nova partida, mapa ou batalha nesta etapa. Não resolve
FriendRoster/DWC, criação completa do save ou os ponteiros globais da partida.
A issue #12 permanece em 0% de aceite funcional; o progresso aqui é remover um
bloqueio de build e tornar a integração de gameplay reproduzível.

## Reprodução

```sh
python3 tools/prepare_hgss_message_headers.py --hgss "$HGSS" \
  --archive "$MSG_NARC" --output "$HEADERS_NOVOS" \
  --modules message_format msgdata overlay_36
python3 tools/audit_gameplay_dependencies.py --build "$BUILD_LIBNTR_DIAGNOSTICO" \
  --hgss "$HGSS" --opening-headers "$OPENING_HEADERS" \
  --message-headers "$HEADERS_NOVOS" --output "$AUDITORIA_NOVA"
```

Os diretórios de saída devem ser novos. A auditoria usa `compile_commands.json`
do diagnóstico que compilou `intro_movie_scenes_lib.c`, para reutilizar seus
headers/flags nativos; compila fontes de gameplay frescas. Falha de compilação
retorna erro e mantém logs. A busca por fornecedores é lexical, não um linker
ou uma análise transitiva completa. ROM, fontes e objetos privados não entram
no repositório público.
