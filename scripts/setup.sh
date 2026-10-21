#!/usr/bin/env bash
set -euo pipefail
project_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
game_ref=ce6406d8c2eaf46d543c8a2606de0c3cb1951982
compiler_ref=da598c1d918402c42c0c0d7128ba14567f3175e9

prepare_checkout() {
    local directory=$1 url=$2 revision=$3
    if [[ ! -e "$directory" ]]; then
        git clone --config core.autocrlf=false "$url" "$directory"
        git -C "$directory" checkout --detach "$revision"
    fi
    if [[ $(git -C "$directory" rev-parse HEAD) != "$revision" ]]; then
        printf 'Unexpected revision in %s; existing checkout was preserved.\n' "$directory" >&2
        exit 1
    fi
}

mkdir -p "$project_dir/tools"
prepare_checkout "$project_dir/game" https://github.com/pret/pokefirered.git "$game_ref"
prepare_checkout "$project_dir/tools/agbcc" https://github.com/pret/agbcc.git "$compiler_ref"
if git -C "$project_dir/game" apply --reverse --check "$project_dir/patches/current.patch" 2>/dev/null; then
    printf 'Source patch already applied.\n'
elif [[ -z $(git -C "$project_dir/game" status --porcelain) ]]; then
    git -C "$project_dir/game" apply --check "$project_dir/patches/current.patch"
    git -C "$project_dir/game" apply "$project_dir/patches/current.patch"
    printf 'Source patch applied.\n'
else
    printf 'Existing game sources differ; preserved without applying patch.\n' >&2
    exit 1
fi
