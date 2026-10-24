# Super HeartGold

Projeto de jogo nativo para Windows, com macOS como alvo posterior. A base técnica é
[pokeheartgold-single-screen, de ClickingMaster](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
O objetivo futuro é Johto, Kanto e Hoenn com progressão compartilhada e 24 insígnias.

**Primeiro objetivo: executar o jogo real nativamente no Windows x86-64. Esse objetivo ainda não está concluído.**

Estimativa de engenharia: aproximadamente **10% do port completo**, faixa de 5–15%;
não é porcentagem de campanha jogável. Veja [status geral e critérios](docs/status.md).

A pasta antiga `games/heart-gold` foi substituída por esta a pedido do usuário. Os arquivos do FireRed não são alterados por este projeto.

## Estado atual

- Plataforma própria C++20/SDL3/CMake compilada para Windows x64, com testes locais e CI; os relatórios em `docs/` registram os resultados de cada etapa.
- Testes de viewport, entrada, passo fixo, log, manifesto e inspeção de dados sintéticos.
- Primeira unidade real do código C de HG/SS compilada e testada nativamente em Windows x64: `SysTaskQueue`. Prova privada opcional, sem CPU DS e sem SDK; ainda não executa a engine completa.
- Serviço de arquivos host integrado à biblioteca da engine e conectado, em prova privada, ao `filesystem.c` original: 1.808 membros de recursos de Pokémon, golpes e textos comparados no Windows x64. Corrigido o vazamento na consulta de tamanho. Veja [native-resources.md](docs/native-resources.md).
- Decoder autoral NCGR/NCLR/NSCR e leitura NARC conectados ao renderer SDL3: camadas 2D reais do título SoulSilver exibidas nativamente com Direct3D 11. Disponível via `--title-resources`; não executa o controlador do título ou modelos 3D. Veja [native-graphics.md](docs/native-graphics.md).
- Recursos individuais NCGR/NCLR/NSCR podem usar compressão LZ10/LZ11; limites e comparação com o compressor original em [native-compression.md](docs/native-compression.md).
- Build privado opcional conecta a função C original de decisão do título e o `OverlayManager` à janela: entrada SDL, transições e destino de menu validados. 3D/áudio e os destinos não foram portados. Veja [native-title-flow.md](docs/native-title-flow.md).
- A decisão original de check-save também é executada com flags controladas; ainda não lê ou valida saves reais. Veja [checksave-decision.md](docs/checksave-decision.md).
- Leitor nativo independente e inspetor dos blocos principais de save: tamanho, rodapés, CRC e seleção das cópias redundantes verificados com fixtures e código original. Ainda não carrega a partida. Veja [native-save-validation.md](docs/native-save-validation.md).
- O executável atual oferece a tela de infraestrutura e o teste opcional de gráficos 2D. **Ainda não contém a engine completa do jogo.**
- ROM local SoulSilver USA identificada e inspecionada; nenhum payload foi incluído no repositório.
- Referência privada ARM9 AOT compilada em Windows x64: 1.800 frames chegaram ao título de SoulSilver e ao menu de início de partida; janela Windows/Direct3D testada por 180 frames, ambas com exit 0. Continua usando ARM7 interpretado e fallback ARM9, portanto não conclui o port fonte. Evidências em [windows-execution.md](docs/windows-execution.md).
- Campanha, batalha, saves do jogo, áudio original e Hoenn não estão implementados nesta plataforma.

## Compilar a plataforma própria

Windows com Visual Studio 2022, CMake e Python:

```powershell
cmake -S games/super-heartgold -B games/super-heartgold/build -A x64
cmake --build games/super-heartgold/build --config Release --parallel 3
ctest --test-dir games/super-heartgold/build -C Release --output-on-failure
```

O SDL3 3.4.18 é obtido do projeto oficial e verificado por SHA-256. Para usar uma instalação externa,
configure `-DHG_FETCH_SDL3=OFF` e `CMAKE_PREFIX_PATH`. A compilação não solicita ROM, BIOS, SDK Nintendo ou emulador.

No ambiente WSL Ubuntu deste workspace, com CMake, Ninja e MinGW x64 instalados:

```powershell
wsl -d Ubuntu -u root -- bash games/super-heartgold/scripts/build-windows-wsl.sh
games/super-heartgold/out/windows/SuperHeartGold.exe --video-driver dummy --frames 120 --self-test
games/super-heartgold/out/windows/hg_tests.exe
python games/super-heartgold/tests/test_tools.py
```

Essa saída é **infraestrutura**, não uma distribuição jogável de HG/SS. O caminho investigativo de execução do jogo permanece fora do produto e do Git.

Para compilar a prova do scheduler C original, use uma pasta de build **fora do repositório** e configure
`-DHG_BUILD_APP=OFF -DHG_REFERENCE_TASK_PROBE=ON`. Os dois fontes pinados são obtidos e verificados somente nessa pasta privada.
`hg_reference_tasks.exe` valida prioridade, capacidade, callbacks, remoção e reutilização; não inclui ROM, SDK ou emulador.
Esse teste não transforma uma unidade isolada em jogo completo.

## Entrada e interface de teste

Setas: direção; X/Z: A/B; S/A: X/Y; Q/W: L/R; Enter: Start; Backspace: Select.
F11 alterna tela cheia, Tab alterna velocidade da simulação, Esc fecha.
`--layout unified|stacked|side` e `--scale integer|fit` testam organização e escala sem distorção.
Eventos sintéticos de teclado e gamepad são testados; controle físico ainda não foi validado.

## Fontes, dados e CI

Os documentos [architecture](docs/architecture.md), [porting-strategy](docs/porting-strategy.md),
[compatibility](docs/compatibility.md) e [roadmap](docs/roadmap.md) separam o implementado do planejado.
A referência principal não é substituída por outro fork. `ctrl-cheeb-del` e `pret` são fontes de proveniência e comparação.

ROM, BIOS, SDK, assets extraídos e C++ gerado da ROM ficam em cache privado fora do repositório.
`tools/inspect_nds.py` lê somente um arquivo local e gera metadados; não copia seus recursos.
`tools/check_assets.py` valida manifesto, caminhos e proveniência declarada, sem afirmar autorização jurídica do conteúdo.

Os workflows executáveis estão na **raiz do monorepo**, em `.github/workflows/super-heartgold-*.yml`.
Windows testa a plataforma a cada mudança; macOS fica preparado para execução manual posterior.
Um workflow configurado não comprova que o jogo funciona nem que o job remoto passou.

## Origem da infraestrutura

Somente o framework autoral anteriormente versionado neste repositório foi reaproveitado da pasta antiga:
viewport, entrada, relógio, log e shell SDL. Não foi copiado código da engine Nintendo para essa plataforma.
O frontend upstream e o runtime ssrecomp têm arquitetura e licenças próprias; consulte o registro de execução antes de tratá-los como port fonte.
