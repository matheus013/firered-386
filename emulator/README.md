# VBA Link 1.8

Port Linux do **VBA Link 1.8 LAN/WiFi** (VisualBoyAdvance 1.8.0 com Link para até 4 jogadores), com transporte TCP corrigido e build para macOS.

- `vba-link/`: código do emulador. `src/LinkTransport.h` traz o transporte com limites e tratamento de erros; `src/GBALink.cpp`, a integração nos métodos de cliente e servidor.
- `build-mac.sh`: build para macOS.
- `tests/`: testes do transporte e da integração LAN.

Origem, problemas corrigidos e pendências: [docs/VBA_LINK_AUDIT.md](../docs/VBA_LINK_AUDIT.md).

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

## Testes do transporte

O transporte passou por 265 cenários automatizados. Um segundo teste executa os métodos reais de cliente/servidor para 2, 3 e 4 jogadores. Ambos rodam com AddressSanitizer e UndefinedBehaviorSanitizer no Linux. No WSL Ubuntu ou no Linux:

```sh
bash tests/test-18.sh
```

O teste de integração descarta seções não utilizadas para exercitar os métodos LAN sem a interface gráfica. Isso não equivale a testar o emulador completo. Os resultados ficam em `tests/test-results/`.

## Windows

Ainda falta uma base Windows 1.8 com a interface e o suporte RFU desejados para integrar estas correções e gerar um `.exe`. Este fork é um port Linux de terceiros e não foi demonstrado que reproduz exatamente o binário Windows 1.8.
