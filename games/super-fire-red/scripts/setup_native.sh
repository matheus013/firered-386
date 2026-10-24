#!/usr/bin/env bash
set -euo pipefail
# Run inside Ubuntu/WSL as root, or with sudo.
if [[ $(id -u) != 0 ]]; then
    printf 'Run sudo bash games/super-fire-red/scripts/setup_native.sh (Ubuntu/WSL).\n' >&2
    exit 1
fi
dpkg --add-architecture i386
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y gcc-multilib g++-multilib libsdl2-dev:i386 gcc-mingw-w64-i686 pkg-config curl
sdk_dir="${XDG_CACHE_HOME:-$HOME/.cache}/firered-386/sdl2-mingw"
mkdir -p "$sdk_dir"
curl -fL --retry 2 https://github.com/libsdl-org/SDL/releases/download/release-2.30.11/SDL2-devel-2.30.11-mingw.tar.gz -o "$sdk_dir/sdk.tar.gz"
printf '0590db0a47b564aab92499ed03eaa75db9aee17c371080caa3b05df9d49d9ff9  %s\n' "$sdk_dir/sdk.tar.gz" | sha256sum -c -
tar -xf "$sdk_dir/sdk.tar.gz" -C "$sdk_dir"
