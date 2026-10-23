# VBA Link: avaliação e melhorias experimentais

O usuário utiliza **VBA Link 1.8 LAN/WiFi**. A entrega principal é uma primeira correção do transporte TCP no **port Linux 1.8** disponível em `vba-link/`. Não é ainda uma nova versão completa do emulador Windows.

- [Avaliação, origem e limites](AUDIT.md).
- `vbalink-1.8-source-improved-dev.zip`: código completo do fork, correções, patch, testes e licença; sem ROMs.
- `vbalink-1.8-transport.patch`: alterações sobre o commit registrado na avaliação.
- `vba-link/src/LinkTransport.h`: transporte com limites e tratamento de erros.
- `vba-link/src/GBALink.cpp`: integração nos métodos reais de cliente e servidor.

## O que já foi verificado

O transporte passou por 265 cenários automatizados. Um segundo teste executa os métodos reais de cliente/servidor para 2, 3 e 4 jogadores. Ambos rodam com AddressSanitizer e UndefinedBehaviorSanitizer no Linux. O arquivo completo `GBALink.cpp` também compila com GCC/C++17; os avisos herdados estão registrados.

No WSL Ubuntu, a partir desta pasta:

```sh
bash test-18.sh
```

O teste de integração usa descarte de seções não utilizadas para testar os métodos LAN sem abrir a interface gráfica. Isso não equivale a compilar ou testar o emulador completo.

## Etapas que faltam para usar no Windows

Obter ou portar uma base Windows 1.8 com a interface e o suporte RFU desejados; integrar as correções nessa base; compilar o executável completo; validar duas instâncias com Pokémon, trocas, batalhas, saves e adaptador wireless. O fork baixado é um port Linux de terceiros e não foi demonstrado que reproduz exatamente o binário Windows 1.8 que você utiliza.

O script antigo `vba-link/setup.sh` não foi executado. O projeto da hackrom e seus saves não foram alterados por este trabalho.

## Experimento anterior: servidor 1.72

`improved-server/` contém um servidor separado, compilado para Windows x64 e Linux, com testes de protocolo. Ele serve apenas para o protocolo **1.72 LAN**. Não substitui o servidor interno do **1.8 LAN/WiFi** e não deve ser apresentado como atualização do seu emulador.
