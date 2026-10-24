# Windows

Alvo: `SuperHeartGold.exe` x86-64. A plataforma inicial já foi compilada e testada no Windows deste workspace.
Isso ainda não executa o jogo HG/SS; veja `docs/windows-execution.md` e o README do projeto.
O workflow executável é `.github/workflows/super-heartgold-windows.yml`, na raiz do monorepo.

Build (Visual Studio 2022 + CMake; o SDL3 é baixado e linkado de forma estática):

```bat
cmake -S . -B build -DHG_FETCH_SDL3=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
build\bin\Release\SuperHeartGold.exe
```

Se preferir um SDL3 já instalado (por exemplo via vcpkg), omita `-DHG_FETCH_SDL3=ON` e aponte o CMake para ele
(`-DCMAKE_PREFIX_PATH=...`); o build copia `SDL3.dll` para junto do `.exe`.

Pendente: ícone, manifesto do aplicativo (DPI awareness, que hoje o SDL3 configura em tempo de execução),
subsistema GUI sem console na versão final e assinatura do executável.
