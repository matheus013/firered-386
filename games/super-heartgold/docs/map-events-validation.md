# Eventos do campo: parser nativo — 02/11/2026

O parser original de eventos e suas funções de consulta executaram em Linux x86-64 e em um EXE Windows x86-64. Os 491 membros do arquivo de eventos da ROM privada passaram na comparação byte a byte. Isso prepara a integração do campo; **não é um mapa jogável**.

Referência consultada antes da adaptação: [map_events.c do pokeheartgold-single-screen](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/map_events.c). O teste inclui essa unidade privada sem modificá-la, para chamar também seu parser estático. Não copia o código original nem os recursos para este repositório.

## Evidências

| Verificação | Resultado |
|---|---|
| Membros de eventos | 491/491 idênticos após leitura pelas funções originais |
| Eventos de cenário / objetos / portas / gatilhos | 1.034 / 2.667 / 1.317 / 195 |
| Layout binário nativo | 20 / 32 / 12 / 16 bytes, verificados na compilação |
| Seções vazias | Ponteiro nulo; memória preenchida previamente com `0xA5` |
| Busca por coordenadas | Retorna a primeira porta correspondente, inclusive coordenadas duplicadas |
| Consultas inválidas | Índice igual à quantidade, `UINT32_MAX` e coordenada negativa rejeitados |
| Escada do quarto inicial | Destino e retorno recíprocos confirmados nos dados dos mapas 64 e 63 |
| Linux | ASan e UBSan aprovados; instrumentação ASan de globais desativada |
| Windows | EXE MinGW executado via interoperabilidade WSL, sem sanitizadores |

Relatórios: [Linux](hgss-map-events-linux-test.json), [Windows](hgss-map-events-windows-test.json).

O leitor Python verifica os limites de todas as seções antes de entregar os dados ao parser original. Isso não demonstra que o parser original suporta arquivos corrompidos. O teste abrange o arquivo de eventos inteiro, mas só verifica a reciprocidade de destinos da escada inicial; não valida todas as transições do mundo.

## Reprodução

Execute no WSL, com as dependências privadas já preparadas:

```sh
python3 tools/verify_map_events.py \
  --hgss /root/.cache/super-heartgold/reference \
  --probe /root/.cache/super-heartgold/hgss-spl-integrated1 \
  --stack /root/.cache/super-heartgold/sdk-stack-scanline1 \
  --archive /root/.cache/super-heartgold/private-events1/a/0/3/2 \
  --output /root/.cache/super-heartgold/map-events-new-run
```

Para Windows, acrescente `--target windows --sdl-include /root/.cache/super-heartgold/aot-reference/windows-static-deps/include` e escolha outra saída. O diretório deve ser novo e externo ao Git. Os membros extraídos, resultados binários e executáveis ficam privados. O arquivo de entrada foi extraído com `import_nds_resources.py`; seus hashes estão nos relatórios.

As opções Windows que removem unwind tables permitem descartar funções não chamadas neste diagnóstico; não definem as opções do executável final.

## Carregamento original integrado ao diagnóstico — 02/11/2026

O modo `--script-archive` agora chama as funções originais `Field_AllocateMapEvents`, `Field_InitMapEvents` e `Field_FreeMapEvents` nos dois andares. São 64 alocações/liberações e 128 carregamentos alternados por plataforma, mantendo uma única alocação durante cada par de mapas. Os bancos de eventos e scripts são derivados do MapHeader original; o adaptador verifica que ambos os mapas não têm encontros selvagens.

O teste verifica que a tabela de encontros fica zerada, que a cauda do cabeçalho de scripts é limpa mesmo partindo de memória preenchida com `0xA5`, e compara os bytes carregados com os membros privados da ROM. A regressão dos 491 membros continua sendo executada no mesmo teste. Linux passou com ASan/UBSan e Windows com execução efetiva do EXE, sem sanitizadores.

Relatórios adicionais: [Linux](hgss-field-load-linux-test.json), [Windows](hgss-field-load-windows-test.json). Para reproduzir, acrescente `--script-archive /root/.cache/super-heartgold/private-field-scripts1/a/0/1/2` ao comando anterior e use outro diretório de saída. A extração usa o importador privado já existente, selecionando `a/0/1/2`.

Os serviços de heap, arquivo e consulta de MapHeader ainda são adaptadores explícitos do diagnóstico. O teste não cobre mapas com encontros selvagens e não inicializa o FieldSystem completo. Carregar o cabeçalho não significa executar o script. Os relatórios anteriores de parser permanecem como evidência do escopo anterior.

## Próxima integração

Ainda falta integrar os serviços reais de arquivos e MapHeader, executar scripts, carregar encontros de mapas externos, criar objetos, integrar colisões e desenho ao FieldSystem e atravessar a escada com comandos do jogador. O próximo consumidor dos cabeçalhos é `TryStartMapScriptByType`, em `script_manager.c`; a execução depende do contexto de scripts e das variáveis reais do save. O teste atual não renderiza o quarto, move o personagem, salva ou retoma uma partida. A issue #12 permanece em 0% de sua entrega jogável.
