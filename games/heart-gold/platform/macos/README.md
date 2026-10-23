# macOS

`Info.plist.in` é o modelo do `Info.plist` do `SuperHeartGold.app` (preenchido pelo CMake).

- Com SDL3 do Homebrew: `.app` arm64 que depende de `/opt/homebrew/opt/sdl3/lib/libSDL3.0.dylib` (bom para desenvolver).
- Com `-DHG_FETCH_SDL3=ON "-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"`: `.app` universal autocontido
  (`scripts/build.sh universal`).

Pendente: ícone (`.icns`), assinatura e notarização.
