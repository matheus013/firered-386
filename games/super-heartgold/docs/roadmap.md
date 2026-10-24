# Roadmap

Este documento distingue entregas verificáveis de funcionalidades planejadas.
O primeiro objetivo é executar o jogo real nativamente no Windows x86-64.
Infraestrutura M0 não conclui esse objetivo; macOS e expansões de conteúdo são posteriores.
O baseline AOT e suas limitações estão em [windows-execution.md](windows-execution.md).

## Gate prioritário — Jogo real no Windows

- Validar a ROM local por checksums e executar o pipeline privado de código real.
- Registrar título/menu, entrada e início de partida no Windows.
- Identificar recompilação ARM9, fallback e ARM7 interpretado no baseline ssrecomp.
- Para o aceite estrito sem CPU emulada, demonstrar o fluxo real da engine com adaptadores host.
- Preservar saves existentes e excluir ROM, SDK, código gerado do jogo e saves da publicação.

## M0 — Fundação nativa (infraestrutura)

- Configurar C++20, CMake, SDL3 e testes do framework autoral.
- Abrir janela, processar eventos e fechar sem erros.
- Exibir tela demonstrativa identificada como protótipo.
- Registrar comandos, resultados e limitações das plataformas realmente testadas.
- Configurar CI Windows na raiz; preparar macOS como alvo posterior, sem declarar validação antecipada.

A aceitação exige build e testes executados; um workflow criado ainda não comprova sucesso remoto.
Esse marco não contém campanha HeartGold, batalha completa ou assets originais do jogo.

## M1 — Prova de lógica e pipeline host

Implementado parcialmente: scheduler original nativo e integração do módulo C de
recursos ao serviço de arquivos host, com validação de 1.808 membros no Windows.
Veja [native-resources.md](native-resources.md). Ainda faltam decodificação gráfica
e integração do fluxo da engine à janela própria.

- Inventariar módulos e suas dependências no pin obrigatório.
- Selecionar uma unidade real e pequena de lógica HeartGold, com decisão documentada sobre permissões.
- Integrar ou implementar a unidade conforme essa decisão, com testes de comportamento e proveniência.
- Validar leitura de um formato de recurso usando fixtures próprias ou autorizadas.
- Demonstrar lógica real executando no host, distinguindo-a do shell.

## M2 — Fatia jogável

- Consolidar regras, entrada, renderização, recursos e persistência.
- Demonstrar exploração, interação e um ciclo de batalha em cenário pequeno.
- Integrar áudio e UI progressivamente, com testes de regressão.
- Rever fidelidade das regras e compatibilidade de dados antes de ampliar conteúdo.

## M3 — Johto e Kanto

Planejar campanha e pós-jogo após a engine demonstrar funcionamento integrado.
A progressão, mapas, conteúdo e sistemas serão implementados e validados por etapas.
Não há declaração de campanha completa nem data de conclusão.

## M4 — Hoenn e expansão

Hoenn somente após a engine e a base Johto/Kanto funcionarem.
Uma campanha com 24 insígnias é objetivo planejado, não funcionalidade entregue.
Escopo e critérios de conclusão serão revisados com evidência dos marcos anteriores.
