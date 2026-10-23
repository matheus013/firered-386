#!/usr/bin/env bash
set -euo pipefail
project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
for source in game/Makefile game/src/pokemon.c tools/agbcc/build.sh; do
    if [[ ! -f "$project_dir/$source" ]]; then
        printf 'Incomplete checkout: missing %s. Clone the complete repository.\n' "$source" >&2
        exit 1
    fi
done
python3 "$project_dir/scripts/source_base.py" > /dev/null
printf 'Complete game/compiler sources and pinned original archive are ready.\n'
