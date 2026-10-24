#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p test-results build-18
flags=(-std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer)
g++ "${flags[@]}" -Wall -Wextra -Wpedantic -Werror test_transport18.cpp -pthread -o build-18/test-transport
./build-18/test-transport 2>&1 | tee test-results/transport18.txt
g++ "${flags[@]}" -ffunction-sections -fdata-sections test_integration18.cpp -pthread -Wl,--gc-sections -o build-18/test-integration 2>test-results/compile18.txt
./build-18/test-integration 2>&1 | tee test-results/integration18.txt
g++ -std=c++17 -c ../vba-link/src/GBALink.cpp -o build-18/GBALink.o 2>>test-results/compile18.txt
printf 'PASS: complete GBALink.cpp translation unit compiled (legacy warnings recorded).\n' | tee -a test-results/compile18.txt
