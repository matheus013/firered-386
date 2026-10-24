#!/usr/bin/env bash
set -euo pipefail
project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)
source_dir=$(cat "$project_dir/games/super-fire-red/output/hack-build-path.txt")
cache_dir="${XDG_CACHE_HOME:-$HOME/.cache}/firered-386"
case "$source_dir" in "$cache_dir"/hack.*) ;; *) printf 'Invalid build directory\n' >&2; exit 1 ;; esac
test -f "$source_dir/Makefile"
test -f "$project_dir/games/super-fire-red/output/changed-files.txt"
while IFS= read -r relative; do
    relative=${relative%$'\r'}
    [[ -n "$relative" ]] || continue
    if [[ ! -f "$project_dir/games/super-fire-red/$relative" ]]; then
        printf 'A source file was removed; use a fresh build instead.\n' >&2
        exit 1
    fi
    mkdir -p "$(dirname "$source_dir/$relative")"
    if ! cmp -s "$project_dir/games/super-fire-red/$relative" "$source_dir/$relative"; then
        cp "$project_dir/games/super-fire-red/$relative" "$source_dir/$relative"
    fi
done < "$project_dir/games/super-fire-red/output/changed-files.txt"
(cd "$source_dir" && make -j4) > "$project_dir/games/super-fire-red/output/hack-build.log" 2>&1
cp "$source_dir/pokefirered.gba" "$project_dir/games/super-fire-red/output/firered-386-dev.gba"
printf 'Incremental build completed\n'
