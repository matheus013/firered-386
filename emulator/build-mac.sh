#!/usr/bin/env bash
# Compila o port VBA Link 1.8 (frontend SDL) no macOS.
# Requer: Xcode Command Line Tools e Homebrew com sdl12-compat e libpng.
set -euo pipefail
cd "$(dirname "$0")"

prefix="$(brew --prefix)"
command -v sdl-config >/dev/null || { echo "Instale o SDL 1.2: brew install sdl12-compat" >&2; exit 1; }

# Copia preservando timestamps para o make não tentar regenerar os arquivos do autotools.
rm -rf build-mac
cp -Rp vba-link build-mac
cd build-mac
chmod +x configure config.guess config.sub install-sh depcomp missing mkinstalldirs
# O git não preserva datas: deixa os arquivos gerados mais novos que suas fontes.
touch aclocal.m4
touch configure
find . -name Makefile.in -exec touch {} +

# Apple Silicon não tem o núcleo em assembly i386: usar o núcleo em C.
CC=clang CXX=clang++ \
CFLAGS="-O2 -Wno-everything" \
CXXFLAGS="-O2 -std=gnu++98 -Wno-everything" \
CPPFLAGS="-I$prefix/include" \
LDFLAGS="-L$prefix/lib" \
  ./configure --enable-c-core --without-mmx --disable-profiling --enable-sdl --disable-dev

# O macOS não tem librt; shm/sem já estão na libc.
sed -i '' 's/^LIBS = \(.*\) -lrt$/LIBS = \1/' src/sdl/Makefile

# Barra de menus nativa (Arquivo, Opções, Link, Ajuda), só no executável principal.
clang++ -O2 -fobjc-arc -c src/sdl/MacMenu.mm -o src/sdl/MacMenu.o
sed -i '' 's/^VisualBoyAdvance_LDADD = \(.*\)$/VisualBoyAdvance_LDADD = \1 MacMenu.o -framework Cocoa/' src/sdl/Makefile

make -j"$(sysctl -n hw.ncpu)"
cp src/sdl/VisualBoyAdvance ../VisualBoyAdvance-mac
echo "OK: emulator/VisualBoyAdvance-mac"
