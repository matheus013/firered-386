#!/usr/bin/env bash
set -euo pipefail
project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cache_dir="${XDG_CACHE_HOME:-$HOME/.cache}/firered-386"
verify_dir=$(mktemp -d "$cache_dir/release-check.XXXXXX")
printf '%s\n' "$verify_dir" > "$project_dir/output/release-check-build-path.txt"
git -c safe.directory="$project_dir/game" -C "$project_dir/game" archive ce6406d8c2eaf46d543c8a2606de0c3cb1951982 | tar -x -C "$verify_dir"
(cd "$verify_dir" && git apply "$project_dir/patches/current.patch")
(cd "$cache_dir/agbcc" && bash install.sh "$verify_dir")
(cd "$verify_dir" && make -j4) > "$project_dir/output/release-check-build.log" 2>&1
cmp "$verify_dir/pokefirered.gba" "$project_dir/output/firered-386-dev.gba"
printf 'Fresh source-patch build matches the delivered GBA.\n'
