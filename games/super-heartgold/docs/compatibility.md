# Compatibilidade e validação

| Alvo | Objetivo | Evidência exigida |
| --- | --- | --- |
| Windows x86-64 | Executável nativo C++20/SDL3 | Build, testes e execução no Windows |
| Linux | Ambiente auxiliar de desenvolvimento | Build, testes e execução no Linux |
| macOS ARM64 | Aplicativo nativo Apple Silicon | Build e execução em macOS ARM64 |
| Nintendo DS | Referência de comportamento e formatos | Não é alvo de compilação deste M0 |

As linhas indicam objetivos; consultar os resultados registrados para saber o que foi efetivamente validado.
Não declarar macOS validado apenas porque o código compila em Windows ou Linux.
Uma execução headless verifica parte do ciclo de vida; não comprova apresentação, áudio ou campanha.

## CI no monorepo

Workflows executáveis devem ficar em `.github/workflows/` na raiz do repositório:

- `.github/workflows/super-heartgold-windows.yml`
- `.github/workflows/super-heartgold-macos.yml`

Eles devem selecionar `games/super-heartgold` como diretório de trabalho e declarar dependências e arquitetura.
A matriz planejada usa Windows 2022 x64, macOS 14 ARM64 e macOS 15 Intel x86-64.
Conferir as imagens na [documentação de runners](https://docs.github.com/en/actions/reference/runners/github-hosted-runners).
Arquivos dentro de `games/super-heartgold/.github/workflows/` não são descobertos como workflows desse monorepo.
Configurar um job não significa que ele já passou; registrar URL e resultado da execução quando disponíveis.

## Contratos para os próximos marcos

- Separar lógica determinística de relógio, entrada, arquivos e renderização.
- Exercitar ABI de 64 bits sem casts truncados e sem serializar ponteiros.
- Validar caminhos, tamanhos e índices ao ler arquivos e arquivos NARC.
- Testar prioridades/paletas 2D, transformações 3D e temporização de áudio quando implementados.
- Versionar saves futuros e testar recarga, corrupção, migração e escrita interrompida.

M0 não oferece compatibilidade de saves com DS/FireRed, multiplayer, campanha HeartGold ou as 24 insígnias planejadas.
A compatibilidade desses sistemas será especificada e testada em seus respectivos marcos.

O single-screen upstream reorganiza a UI DS; não elimina dependências de gráficos, áudio, filesystem ou ABI do DS.
Reutilização seletiva exige preservar a proveniência e verificar permissões e dependências por módulo.
