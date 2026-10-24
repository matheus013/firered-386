#!/usr/bin/env bash
set -euo pipefail
project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cache_dir="${XDG_CACHE_HOME:-$HOME/.cache}/super-heartgold/native-windows"
mkdir -p "$cache_dir/source" "$project_dir/out/windows"
tar -C "$project_dir" --exclude='./out' --exclude='./build' --exclude='./build-*' --exclude='./.git' -cf - . | tar -x -C "$cache_dir/source"
cmake -S "$cache_dir/source" -B "$cache_dir/build" -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$cache_dir/source/tools/mingw-x64.cmake" \
    -DCMAKE_BUILD_TYPE=Release -DHG_FETCH_SDL3=ON
cmake --build "$cache_dir/build" --parallel 3
cp "$cache_dir/build/bin/SuperHeartGold.exe" "$cache_dir/build/bin/hg_tests.exe" \
    "$cache_dir/build/bin/hg_save_inspect.exe" "$project_dir/out/windows/"
printf 'Windows native platform build: %s/out/windows/SuperHeartGold.exe\n' "$project_dir"
