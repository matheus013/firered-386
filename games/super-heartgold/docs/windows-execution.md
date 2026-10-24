# Primeiro objetivo: jogo real no Windows

Uma janela demonstrativa não conclui este objetivo. O aceite exige executar código real do jogo, registrar o caminho de execução e identificar CPU interpretada, recompilada ou portada.
A base mandatória permanece [ClickingMaster/pokeheartgold-single-screen, pin 6984a3d](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
O runtime associado analisado é [ssrecomp, pin 3146fb2](https://github.com/ClickingMaster/ssrecomp/tree/3146fb2d4d88522b12b4bf25dee74285120975e2).
A auditoria dos fontes foi seguida de geração AOT privada; o estado executado está registrado abaixo. Os demais comandos não implicam sucesso de execução até sua validação explícita.

## Caminho imediato sem reconstruir a ROM

O ssrecomp aceita a ROM original SoulSilver USA diretamente, sem NEF, Metrowerks ou Nitro SDK local.
O [setup L493–505](https://github.com/ClickingMaster/ssrecomp/blob/3146fb2d4d88522b12b4bf25dee74285120975e2/ssrecomp_setup.py#L493) gera código da ROM usando `layouts/soulsilver-us.json`, gera a tabela de aplicativos e executa CMake/Ninja.
Esse layout contém 121 seções, endereços, ranges e checksums; a ROM fornece os bytes de código.
O [loader L79–105](https://github.com/ClickingMaster/ssrecomp/blob/3146fb2d4d88522b12b4bf25dee74285120975e2/tools/recomp.py#L79) valida o SHA1 de cada imagem de seção e rejeita jogo/região/revisão incompatível.
Header IPGE e tamanho de 128 MiB sozinhos não comprovam compatibilidade. O SHA1 varejo esperado é `f8dc38ea20c17541a43b58c5e6d18c1732c7e582`; verificar o arquivo fornecido e os checksums por seção.
Esse caminho serve como baseline de execução do jogo original; não incorpora automaticamente as mudanças `SINGLE_SCREEN` do fork nem as melhorias planejadas Super HeartGold.

Em uma cópia privada do ssrecomp pinado, com MSYS2 UCRT64 e dependências instaladas:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-zlib mingw-w64-ucrt-x86_64-python
python tools/recomp.py --rom "$ROM_PATH" --layout layouts/soulsilver-us.json melonDS/src gen
python tools/apps.py --layout layouts/soulsilver-us.json gen
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
ninja -C build -j2
./build/ssrecomp.exe "$ROM_PATH" --save "$PRIVATE_SAVE_PATH"
```

`ROM_PATH` e `PRIVATE_SAVE_PATH` são caminhos locais explicitamente definidos pelo operador. Não sobrescrever saves existentes.
O setup também oferece `python ssrecomp_setup.py --rom ... --jobs 2` e copia DLLs necessárias para execução fora do terminal MSYS2.
Não é preciso baixar `gen/`: o código é produzido localmente da ROM fornecida. Mantê-lo, ROMs e saves fora do Git e dos artefatos públicos.

## O que esse executável realmente faz

O [CMake L19–45](https://github.com/ClickingMaster/ssrecomp/blob/3146fb2d4d88522b12b4bf25dee74285120975e2/CMakeLists.txt#L19) desliga o JIT melonDS e compila C++ gerado em `gen/`, com SDL2/C++17.
Não compila diretamente os `src/*.c` de HeartGold para o host.
[RecompRun L189–228](https://github.com/ClickingMaster/ssrecomp/blob/3146fb2d4d88522b12b4bf25dee74285120975e2/runtime/recomp_rt.cpp#L189) despacha chunks ARM9 previamente traduzidos; pode retornar false para código ausente.
[ARM.cpp L693–728](https://github.com/ClickingMaster/ssrecomp/blob/3146fb2d4d88522b12b4bf25dee74285120975e2/melonDS/src/ARM.cpp#L693) executa o interpretador no fallback ARM9.
O ARM7 usa handlers interpretados em L855/L872. Falhas de guarda de opcode também interpretam instruções.
Portanto o modelo é AOT ARM9 com hardware DS em software e interpretação residual; não satisfaz sozinho um requisito estrito de ausência de CPU emulada.
A primeira validação deve registrar título/menu, entrada, início de partida e logs de execução, sem apresentar esse baseline como port fonte SDL3 concluído.

## Incorporar mudanças do fork

Para modificar o jogo via decompilação e gerar uma ROM `SINGLE_SCREEN`, o [INSTALL L232–245](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/INSTALL.md#L232) exige ROM própria, Metrowerks e Nitro SDK/templates/bibliotecas fornecidos localmente.
A base não distribui esses componentes. O comando documentado é `make -j8 soulsilver COMPARE=0`; o checksum varejo não se aplica ao layout modificado.
Com `.nef/.sbin` resultantes, `tools/recomp.py BUILD_DIR melonDS/src gen` e `tools/apps.py BUILD_DIR gen` substituem o layout varejo.
Um layout de endereços/checksums varejo não deve ser aplicado silenciosamente a uma ROM modificada.
A presença de fontes SDK em outro repositório não demonstra autorização para importá-los ou redistribuí-los; não usar isso como atalho automático para preencher a base.

## Port fonte sem CPU DS

O ponto real de entrada é [NitroMain L48–130](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/main.c#L48): inicializa gráficos, entrada, RTC, fontes, save e áudio; depois executa overlays e filas por frame.
Um alvo host precisa compilar módulos reais e substituir serviços SDK/ARM7 por interfaces host, respeitando ABI64 e permissões por arquivo.
Título/menu alcançados por esse fluxo são uma prova incremental; janela vazia ou regras isoladas não comprovam jogo executando.
C++20/SDL3 continua como arquitetura do projeto próprio; a rota ssrecomp é avaliação separada, com suas licenças e limites explícitos.
Windows real é a prioridade; macOS e expansão Hoenn vêm depois dessa evidência.

## Evidência local — 23–24/10/2026

- A ROM fornecida tem 128 MiB, header IPGE revisão 0 e SHA1 completo `0959fe9e64dc749382460f1ccf90996f5ef4088d`, diferente do checksum varejo citado acima.
- Sem alterar a ROM ou desabilitar verificações, `tools/recomp.py --rom ... --layout layouts/soulsilver-us.json melonDS/src gen` terminou com sucesso: 122 seções, 25.120 chunks, 1.685.113 instruções e 71 arquivos.
- `tools/apps.py --layout layouts/soulsilver-us.json gen` gerou 59 aplicativos.
- Os checksums de todas as imagens de seção ARM9 solicitadas pelo layout passaram. Isso comprova compatibilidade do código selecionado com o layout, não identidade de todos os assets/header/dados da ROM com o varejo.
- Os arquivos gerados ficam em cache privado, sem distribuição. A geração inclui uma seção adicional para BIOS livre do runtime.
- Compilação completa dos 71 arquivos gerados concluída: PE32+ x86-64, Clang 21.1.8 para código gerado em O0, GCC 13 para core/link. Nenhum subconjunto limitado à introdução foi usado.
- A primeira execução com DLLs externas falhou antes de `main` (`0xC0E90002`); os eventos CodeIntegrity 3077/3033 identificaram bloqueio de SDL2/libgcc. Não foram alteradas políticas de segurança. A ligação estática de SDL2, libpng, zlib e runtimes GCC resolveu a inicialização; o executável final importa somente 12 DLLs do Windows.
- Execução real no Windows com `--headless --frames 1800`, comandos de Start/A, save novo isolado e `--no-save`: exit 0 em 52,47 segundos. Imagens inspecionadas: título SoulSilver com Lugia no frame 900 e menu de ajuda de nova partida nos frames 1500/1799.
- Janela real SDL2 2.32.10, driver Windows e renderer Direct3D: 180 frames, exit 0. Isso confirma abertura da janela, não valida áudio percebido, controle físico ou campanha.
- Contadores privados: 6.105.337 entradas em chunks ARM9, 12.233 fallbacks (0,20% das entradas), 27 falhas de guarda e 103.459.454 chamadas a handlers ARM7 interpretados. A porcentagem é de despachos, não de tempo de CPU ou instruções. O contador ARM7 é instrumentação privada; não altera a ROM e não está incluído no script público.
- Executável privado estático: 1.085.111.854 bytes, SHA-256 `aae7b66c2ee7fc44fd0472cfa588e697dbcdfb4563539de802aec6c3dd268637`. O tamanho reflete a primeira compilação O0, sem otimização de distribuição.
- Nenhum save existente foi reutilizado ou sobrescrito nos testes. ROM, código gerado, executável e screenshots ficam fora do Git. O launcher privado `Jogar-Referencia-Hibrida.cmd` usa um save separado para uso manual; persistência desse save ainda não foi validada.

O título/menu renderizados comprovam a referência híbrida funcionando. Não comprovam exploração, batalha, save/load, campanha completa, alterações single-screen do fork ou o produto C++20/SDL3 sem CPU DS interpretada. O primeiro objetivo estrito continua aberto. Um resumo público sem recursos do jogo está em [windows-reference-validation.json](windows-reference-validation.json).

## Reproduzir a referência privada

Em Windows, a partir desta pasta de projeto:

```powershell
python scripts/build-reference-windows.py --rom "D:\jogos\SoulSilver-USA.nds" --jobs 4 --run-smoke
```

O caminho é exemplo e deve apontar para a cópia local fornecida pelo usuário.
O script fixa commits do runtime, SDL2, zlib e libpng, usa cache Linux privado no WSL e grava artefatos fora do Git, em AppData por padrão.
Dependências: Python 3.12+ no Windows; Git, Python 3, CMake, Ninja, MinGW x64 e Clang no WSL. O launcher Clang usa os headers libstdc++ GCC 13 POSIX da configuração inspecionada.
`--gamecode-compiler gcc` usa somente GCC; `--optimization 0` é o padrão para a primeira prova e `1` permite avaliar otimização posteriormente.
O core e o linker continuam GCC; Clang compila somente o target de código gerado, com o mesmo ABI MinGW.
O teste isolado local de `rc_000.cpp` com Clang 21.1.8 produziu COFF x64 em 39,67 segundos, pico de 858.636 KiB e exit 0; isso não substitui o teste do executável.
O script foi validado por sintaxe e ajuda; sua execução integral em um cache novo ainda precisa de registro próprio.
As bibliotecas são ligadas estaticamente; não é necessário instalar SDL2 ou desativar controles do Windows. `--run-smoke` usa save UUID inexistente e `--no-save`; logs e screenshots são privados. O smoke de 900 frames do script é mais curto que o teste manual de 1.800 frames descrito acima e não reproduz sua sequência de entrada.
Não publicar o executável derivado, `gen/`, ROM ou saves junto ao produto sem decisão explícita de distribuição aplicável.
