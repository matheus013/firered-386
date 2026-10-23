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

## Build para macOS (Apple Silicon)

Requer as Command Line Tools do Xcode e `brew install sdl12-compat libpng`. A partir desta pasta:

```sh
bash build-mac.sh
./VisualBoyAdvance-mac --link 0 --linknum 2 rom.gba          # servidor
./VisualBoyAdvance-mac --link 1 --linkaddr IP_DO_SERVIDOR rom.gba   # cliente
```

Sem ROM na linha de comando, o emulador abre um seletor de arquivos.

Usa o frontend SDL e o núcleo em C, porque o núcleo em assembly é exclusivo de i386. Um servidor e um cliente se conectaram em `127.0.0.1` e rodaram uma ROM multiboot. Trocas e batalhas reais não foram testadas.

### Barra de menus

No macOS, o emulador ganha uma barra de menus nativa com o essencial do VBA Link para Windows:

- **Arquivo:** abrir ROM, carregar/salvar estado (arquivo ou slots 1–10), pausar, reiniciar, importar/exportar `.sav`, capturar tela.
- **Opções:** velocidade (sem limite, 25–200%, turbo), frame skip, vídeo x1–x4 e tela cheia, camadas, filtros, tipo de save (Automático, EEPROM, SRAM, Flash, EEPROM+Sensor, Nenhum; Flash 64K/128K), som (mudo, volume), autofire.
- **Link:** hospedar partida para 2–4 jogadores, conectar a um servidor pelo IP, desconectar, tempo limite e IP deste Mac.

Trocar ROM, tipo de save ou modo de link reinicia o emulador depois de gravar o `.sav`. As opções ficam salvas entre execuções. O padrão no Mac é Flash 128K, que os jogos de Pokémon exigem, frame skip 0 (60 fps) e janela x3. Ainda faltam os menus Cheats, Gameboy e a configuração de teclas, que continua em `~/.vba/VisualBoyAdvance.cfg`.

A release `v0.5.0-preview.4` carregou e rodou a ~100% de velocidade, a 60 fps, da introdução até a tela "PRESS START", e gravou um `.sav` de 128K ao fechar.
