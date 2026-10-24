# Compilação para GBA e PC

Execute na raiz do repositório:

```sh
make -C games/super-fire-red -f hack.mk gba
make -C games/super-fire-red -f hack.mk pc
```

`make -C games/super-fire-red -f hack.mk` sem argumento equivale a `make -C games/super-fire-red -f hack.mk gba`; `make -C games/super-fire-red -f hack.mk native` equivale a `make -C games/super-fire-red -f hack.mk pc`. A ROM fica em `games/super-fire-red/output/firered-386-dev.gba`. O executável fica em `games/super-fire-red/output/pc/windows/firered-386.exe` ou `games/super-fire-red/output/pc/linux/firered-386`.

A versão PC é um **port nativo experimental**: compila os fontes C da engine para x86 de 32 bits e usa SDL2 para janela, entrada e áudio, com renderização da plataforma em C. Não inclui um interpretador de CPU ARM nem exige abrir uma ROM em tempo de execução. O build GBA é usado como etapa intermediária para gerar os mesmos recursos gráficos, de áudio e scripts.

## Ambiente e comandos

A compilação usa Ubuntu, diretamente ou através do WSL. No Windows deste projeto:

```powershell
wsl -d Ubuntu -u root -- make -C games/super-fire-red -f hack.mk setup-native
wsl -d Ubuntu -u root -- make -C games/super-fire-red -f hack.mk gba
wsl -d Ubuntu -u root -- make -C games/super-fire-red -f hack.mk pc
```

O ambiente GBA precisa das dependências descritas no [README](../README.md). `setup-native` instala GCC multilib, SDL2 de 32 bits e MinGW i686, e baixa o SDK oficial SDL2 2.30.11 com SHA-256 verificado. Em Ubuntu sem privilégios de root, execute `sudo bash games/super-fire-red/scripts/setup_native.sh`.

```sh
make -C games/super-fire-red -f hack.mk pc PC_PLATFORM=windows
make -C games/super-fire-red -f hack.mk pc PC_PLATFORM=linux
make -C games/super-fire-red -f hack.mk baseline
make -C games/super-fire-red -f hack.mk setup
```

O padrão de PC é Windows quando `OS=Windows_NT` ou `WSL_DISTRO_NAME` está definido; fora desses ambientes, é Linux. Com GNU Make instalado no Windows, os alvos chamam wrappers PowerShell que usam o WSL Ubuntu. Ponteiros de 32 bits são necessários para os formatos de dados e scripts existentes.

A compilação trabalha no cache Linux `~/.cache/firered-386`, preservando `games/super-fire-red/`. `games/super-fire-red/native/port.patch` é aplicado somente à cópia usada no alvo PC. IA, regras e equipes continuam compartilhadas com a hack; GBA mantém seu código de plataforma original.

## Execução e controles

No Windows, mantenha `SDL2.dll` ao lado do executável. No Linux, instale o runtime SDL2 i386; o binário depende da versão de libc/SDL2 do ambiente de compilação e não é um pacote universal para qualquer distribuição.

| Função | Tecla |
| --- | --- |
| Direcional | Setas |
| A / B | Z / X |
| Start | Enter |
| Select | Barra invertida |
| L / R | A / S |

O port herda suporte a controle XInput no Windows. Compatibilidade com controles físicos e qualidade do áudio precisam de validação manual.

Saves e configurações ficam no diretório retornado por SDL_GetPrefPath, sob `matheus013/firered-386`, separados dos saves de emuladores. `FIRERED_PC_SAVE_DIR` permite escolher um diretório existente. O compilador verifica tamanhos e offsets essenciais do save. A memória adicional usada pelo deslocamento dos blocos de save é reservada explicitamente no host.

Para importar um save, use uma cópia chamada `firered-386.sav`; a compatibilidade completa entre versões e plataformas ainda não foi validada.

## Testes e limites

```powershell
python games/super-fire-red/scripts/test_native.py --platform windows
```

```sh
python3 games/super-fire-red/scripts/test_native.py --platform linux
```

O script usa drivers SDL de teste, pastas de save isoladas e entrada por quadro. Verifica abertura, escolha Normal/Hardcore e randomizer; gera logs, imagens BMP e `results.json` em `games/super-fire-red/output/native-tests/<plataforma>`. `--save CAMINHO` acrescenta a abertura de uma cópia de um save existente. Esses testes não equivalem a zerar o jogo nem validam áudio, batalhas completas, todos os mapas ou periféricos.

Validação local de 23/10/2026: **12 casos passaram**, seis no Windows e seis no Linux: abertura, Normal, Hardcore, randomizer, importação de save e gravação/recarga pelo menu do jogo. No último caso, o contador de save avançou de 31 para 32 e permaneceu em 32 na recarga. A fixture veio de um teste assistido de GBA e já estava no ginásio de Pewter; não representa progressão da campanha no port. [Hashes, estados e escopo dos testes](../games/super-fire-red/hack-data/native_validation.json).

O comando completo `make -C games/super-fire-red -f hack.mk pc` passou. A ROM gerada por `make -C games/super-fire-red -f hack.mk gba` manteve SHA-256 `0b8e7bda6ee3a63846317f525a4b09e3accd8b87cf0d46db0502d4898fe6f63f`. O alvo PC não mudou os fontes da versão GBA.

Os hooks opcionais `FIRERED_PC_TEST_FRAMES`, `FIRERED_PC_TEST_FAST`, `FIRERED_PC_TEST_INPUT` e `FIRERED_PC_SCREENSHOT` servem aos testes. Cada linha de entrada contém quadro inicial, quadro final exclusivo e máscara hexadecimal dos botões GBA.

Link cable, RFU, multiboot e conectividade com GameCube não estão implementados na versão PC. A inclusão dos programas multiboot nos dados não implementa os periféricos. O port não substitui o VBA Link para partidas em rede. A campanha completa e o pós-jogo também não foram validados no port.

## Origem

A camada de plataforma deriva de [pokefirered-dualscreen](https://github.com/venom13/pokefirered-dualscreen/tree/cd551d2c6ae5536a2599114fed5ed823a9c9678c), revisão `cd551d2c6ae5536a2599114fed5ed823a9c9678c`, adaptada para desktop com janela única. A IA desta hack foi preservada. Licença e atribuições estão em `games/super-fire-red/native/licenses/`; a licença SDL2 acompanha o build Windows.
