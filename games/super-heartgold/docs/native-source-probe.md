# Primeira unidade C compilada para o host

A fila `SysTaskQueue`, usada pelo loop `NitroMain`, foi compilada como C nativo
para Windows x86-64 a partir da base ClickingMaster pinada. O fonte original
não foi reescrito nem traduzido de instruções ARM: o compilador host compilou
diretamente `src/sys_task.c`, com declarações de tipos host de largura fixa.

O alvo opcional `HG_REFERENCE_TASK_PROBE` está desligado por padrão. Os dois
arquivos de referência são baixados somente para um diretório privado de build
fora do repositório e validados por SHA-256. Não fazem parte dos fontes públicos
nem dos artefatos distribuídos da plataforma. Isto não concede novas permissões
de redistribuição para o código de terceiros.

Fonte original: [sys_task.c](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/sys_task.c).
Hashes usados pelo preparador:

- C: `9ef0caac3b9c0f869e50e5df5278e4de50f65b0b76813ea4ecd533acc5bc767a`.
- Header: `100b3c1d5a0cc2c26ff743679e8782b039e1fb7bf135fad0b30fb5ff5cd04ffb`.

Verificado em 23/10/2026: binário PE x86-64 executado no Windows, saída zero.
Prioridade, ordem estável para prioridades iguais, limite da fila, ponteiros de
callback/dados, remoção durante execução e reutilização de slots passaram.
A suíte privada também passou em CTest. Não há interpretador, CPU ARM, ROM,
BIOS, implementação Nitro SDK ou hardware DS nesse alvo.

O teste comprova que esta unidade real pode usar a ABI de 64 bits; não comprova
o restante da engine. O próximo trabalho de port fonte é ligar serviços host
de memória, recursos, gráficos, áudio e overlays ao fluxo real de inicialização.
Título, menu e começo de partida desse fluxo continuam sendo o primeiro aceite
funcional, sem confundir essa prova com o baseline híbrido de recompilação.
