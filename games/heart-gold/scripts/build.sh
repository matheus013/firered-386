#!/usr/bin/env bash
# Configura, compila e testa o Super HeartGold.
#
#   scripts/build.sh            build com o SDL3 instalado (Homebrew no macOS), em build/
#   scripts/build.sh universal  .app universal (arm64 + x86-64) com SDL3 estático, em build-universal/
set -euo pipefail

cd "$(dirname "$0")/.."

mode="${1:-dev}"
case "$mode" in
dev)
    dir=build
    args=()
    ;;
universal)
    dir=build-universal
    args=(-DHG_FETCH_SDL3=ON "-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0)
    ;;
*)
    echo "uso: $0 [dev|universal]" >&2
    exit 2
    ;;
esac

cmake -S . -B "$dir" -DCMAKE_BUILD_TYPE=Release ${args[@]+"${args[@]}"}
cmake --build "$dir" --parallel
ctest --test-dir "$dir" --output-on-failure
