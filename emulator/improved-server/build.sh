#!/usr/bin/env bash
set -euo pipefail
project=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
mkdir -p "$project/build" "$project/bin"
flags=(-std=c++17 -Wall -Wextra -Wpedantic -Werror -O2)
g++ "${flags[@]}" "$project/src/main.cpp" -o "$project/bin/vbalink-server-linux"
x86_64-w64-mingw32-g++ "${flags[@]}" -static "$project/src/main.cpp" -o "$project/bin/vbalink-server.exe" -lws2_32
g++ "${flags[@]}" -DVBALINK_TEST_FRAGMENT_SEND=1 "$project/src/main.cpp" -o "$project/build/server-short-writes-linux"
x86_64-w64-mingw32-g++ "${flags[@]}" -DVBALINK_TEST_FRAGMENT_SEND=1 -static "$project/src/main.cpp" -o "$project/build/server-short-writes.exe" -lws2_32
printf 'Built Windows x64 and Linux servers, plus short-write test variants.\n'
