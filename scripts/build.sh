#!/usr/bin/env bash
set -euo pipefail

mode=${1:-hack}
case "$mode" in
    baseline|hack) ;;
    *) printf 'Usage: bash scripts/build.sh [baseline|hack]\n' >&2; exit 2 ;;
esac

project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cache_dir="${XDG_CACHE_HOME:-$HOME/.cache}/firered-386"
compiler_dir="$cache_dir/agbcc"
mkdir -p "$cache_dir" "$project_dir/output"

if [[ ! -x "$compiler_dir/agbcc" || ! -f "$compiler_dir/libc.a" ]]; then
    if [[ ! -d "$project_dir/tools/agbcc/gcc" ]]; then
        printf 'Missing tools/agbcc. See README.md for the pinned compiler source.\n' >&2
        exit 1
    fi
    mkdir -p "$compiler_dir"
    cp -a "$project_dir/tools/agbcc/." "$compiler_dir/"
    (cd "$compiler_dir" && bash build.sh) > "$project_dir/output/compiler.log" 2>&1
fi

# Build on the Linux filesystem: Windows mounts make small-file builds much slower.
# Each build gets a fresh directory so deleted source files cannot linger.
source_dir=$(mktemp -d "$cache_dir/$mode.XXXXXX")
printf '%s\n' "$source_dir" > "$project_dir/output/$mode-build-path.txt"
if [[ "$mode" == baseline ]]; then
    git -c safe.directory="$project_dir/game" -C "$project_dir/game" archive ce6406d8c2eaf46d543c8a2606de0c3cb1951982 | tar -x -C "$source_dir"
else
    tar -C "$project_dir/game" --exclude=.git --exclude=build --exclude=tools/agbcc \
        --exclude='./poke*.gba' --exclude='*.elf' --exclude='*.map' --exclude='*.sym' -cf - . | tar -x -C "$source_dir"
fi

(cd "$compiler_dir" && bash install.sh "$source_dir")
if [[ "$mode" == baseline ]]; then
    (cd "$source_dir" && make -j4 compare) > "$project_dir/output/baseline-build.log" 2>&1
    cp "$source_dir/pokefirered.gba" "$project_dir/output/baseline.gba"
else
    (cd "$source_dir" && make -j4) > "$project_dir/output/hack-build.log" 2>&1
    cp "$source_dir/pokefirered.gba" "$project_dir/output/firered-386-dev.gba"
fi
printf 'Build completed: %s\n' "$mode"
