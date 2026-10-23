#!/usr/bin/env bash
set -euo pipefail

mode=${1:-hack}
case "$mode" in
    baseline|hack) ;;
    *) printf 'Usage: bash game/scripts/build.sh [baseline|hack]\n' >&2; exit 2 ;;
esac

project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
cache_dir="${XDG_CACHE_HOME:-$HOME/.cache}/firered-386"
compiler_dir="$cache_dir/agbcc"
mkdir -p "$cache_dir" "$project_dir/game/output"

if [[ ! -x "$compiler_dir/agbcc" || ! -f "$compiler_dir/libc.a" ]]; then
    if [[ ! -d "$project_dir/game/tools/agbcc/gcc" ]]; then
        printf 'Missing game/tools/agbcc. See README.md for the pinned compiler source.\n' >&2
        exit 1
    fi
    mkdir -p "$compiler_dir"
    cp -a "$project_dir/game/tools/agbcc/." "$compiler_dir/"
    (cd "$compiler_dir" && bash build.sh) > "$project_dir/game/output/compiler.log" 2>&1
fi

# Build on the Linux filesystem: Windows mounts make small-file builds much slower.
# Each build gets a fresh directory so deleted source files cannot linger.
source_dir=$(mktemp -d "$cache_dir/$mode.XXXXXX")
printf '%s\n' "$source_dir" > "$project_dir/game/output/$mode-build-path.txt"
if [[ "$mode" == baseline ]]; then
    python3 "$project_dir/game/scripts/source_base.py" --extract "$source_dir"
else
    # Copy only the decomp sources; the hack tooling also lives in game/.
    python3 "$project_dir/game/scripts/source_base.py" --files |
        tar -C "$project_dir/game" -cf - -T - | tar -x -C "$source_dir"
fi

(cd "$compiler_dir" && bash install.sh "$source_dir")
if [[ "$mode" == baseline ]]; then
    (cd "$source_dir" && make -j4 compare) > "$project_dir/game/output/baseline-build.log" 2>&1
    cp "$source_dir/pokefirered.gba" "$project_dir/game/output/baseline.gba"
else
    (cd "$source_dir" && make -j4) > "$project_dir/game/output/hack-build.log" 2>&1
    cp "$source_dir/pokefirered.gba" "$project_dir/game/output/firered-386-dev.gba"
fi
printf 'Build completed: %s\n' "$mode"
