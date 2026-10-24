# Workflows

Este projeto é uma subpasta do repositório. Os workflows executáveis ficam na
raiz, em `.github/workflows/super-heartgold-windows.yml` e
`.github/workflows/super-heartgold-macos.yml`.

O Windows compila e testa a infraestrutura nativa a cada mudança nesta pasta.
O macOS está preparado para acionamento manual e é uma etapa posterior.
Esses jobs não executam o jogo HG/SS: validam somente a plataforma inicial,
sem ROM ou emulador. A execução real do jogo no Windows é o primeiro objetivo
funcional, ainda separado desses testes.
