# macOS

`Info.plist.in` é o modelo do `Info.plist` do `SuperHeartGold.app` (preenchido pelo CMake).

Alvo posterior ao jogo real no Windows. Nenhum build macOS desta nova pasta foi executado neste ambiente.

- Com `-DHG_FETCH_SDL3=OFF` e SDL3 do Homebrew: `.app` arm64 com dependência dinâmica SDL3 (para desenvolvimento).
- Com `-DHG_FETCH_SDL3=ON "-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"`: `.app` universal autocontido
  (`scripts/build.sh universal`).

Pendente: ícone (`.icns`), assinatura e notarização.
