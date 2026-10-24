#!/usr/bin/env python3
"""Private Windows AOT reference: ARM9 translation + ARM7/interpreter fallback.

Requires Windows, WSL Ubuntu, git/cmake/ninja and x86_64-w64-mingw32 compilers
inside WSL. It never downloads ROM, BIOS or proprietary SDK. Game-derived
outputs are private and must not be committed or publicly packaged.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shlex
import subprocess
import sys

SOURCES = {
    "ssrecomp": ("https://github.com/ClickingMaster/ssrecomp.git", "3146fb2d4d88522b12b4bf25dee74285120975e2"),
    "SDL": ("https://github.com/libsdl-org/SDL.git", "5d249570393f7a37e037abf22cd6012a4cc56a71"),
    "zlib": ("https://github.com/madler/zlib.git", "51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf"),
    "libpng": ("https://github.com/pnggroup/libpng.git", "2b978915d82377df13fcbb1fb56660195ded868a"),
}
BASE_PIN = "6984a3d67592157195940049be2e4bbbf933f9ff"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", required=True, type=Path, help="Existing local SoulSilver USA ROM, read only")
    parser.add_argument("--cache", default="/root/.cache/super-heartgold/aot-reference-repro", help="Private absolute Linux cache, outside /mnt")
    parser.add_argument("--output", type=Path, default=Path(os.environ.get("LOCALAPPDATA", str(Path.home()))) / "SuperHeartGold/reference")
    parser.add_argument("--jobs", type=int, choices=range(1, 5), default=3)
    parser.add_argument("--optimization", choices=("0", "1"), default="0", help="Generated gamecode optimization; O0 prioritizes first smoke")
    parser.add_argument("--distro", default="Ubuntu")
    parser.add_argument("--wsl-user", default="root")
    parser.add_argument("--run-smoke", action="store_true", help="Run 900 headless frames with private screenshots and no save writes")
    args = parser.parse_args()
    if sys.platform != "win32":
        parser.error("Run this Windows reference builder with Windows Python and WSL installed.")
    rom = args.rom.expanduser().resolve(strict=True)
    if not rom.is_file():
        parser.error("--rom must name an existing file")
    cache = PurePosixPath(args.cache)
    if not cache.is_absolute() or ".." in cache.parts or str(cache).startswith("/mnt/") or str(cache) == "/":
        parser.error("--cache must be an absolute private Linux directory outside /mnt and without '..'")
    output = args.output.expanduser().resolve()
    # The project lives in a monorepo: all derived artifacts stay outside it.
    here = Path(__file__).resolve()
    git_ancestors = [p for p in here.parents if (p / ".git").exists()]
    if any(output.is_relative_to(p) for p in git_ancestors) or output.is_relative_to(here.parents[1]):
        parser.error("--output must be outside the project and its containing Git repository")
    output.mkdir(parents=True, exist_ok=True)
    wsl = ["wsl", "--distribution", args.distro, "--user", args.wsl_user, "--"]

    def bash(script: str, capture: bool = False) -> bytes:
        result = subprocess.run(wsl + ["bash", "-s"], input=script.encode("utf-8"),
                                stdout=subprocess.PIPE if capture else None, check=True)
        return result.stdout or b""

    print("Investigative reference only: ARM9 AOT, ARM7 interpretation and ARM9 fallback remain.", flush=True)
    print("No proprietary compiler/SDK is needed for this original-ROM baseline.", flush=True)
    q = shlex.quote
    clones = []
    for name, (url, pin) in SOURCES.items():
        folder = str(cache / ("ssrecomp" if name == "ssrecomp" else "deps/" + name))
        clones.append(f"""
if [ ! -d {q(folder + '/.git')} ]; then
  mkdir -p {q(str(PurePosixPath(folder).parent))}
  git clone --no-checkout {q(url)} {q(folder)}
  git -C {q(folder)} checkout --detach {q(pin)}
fi
[ "$(git -C {q(folder)} rev-parse HEAD)" = {q(pin)} ] || {{ echo 'Cache pin differs; choose a fresh cache.' >&2; exit 1; }}
[ -z "$(git -C {q(folder)} diff --name-only -- . {q(":(exclude)CMakeLists.txt") if name == "ssrecomp" else ""})" ] || {{ echo 'Tracked cache sources changed; choose a fresh cache.' >&2; exit 1; }}
""")
    script = f"""set -euo pipefail
for tool in git cmake ninja x86_64-w64-mingw32-gcc x86_64-w64-mingw32-g++ x86_64-w64-mingw32-windres python3; do
 command -v "$tool" >/dev/null || {{ echo "Missing WSL tool: $tool" >&2; exit 1; }}
done
cache={q(str(cache))}
prefix="$cache/windows-deps"
mkdir -p "$cache/deps"
""" + "\n".join(clones) + f"""
cat > "$cache/windows-toolchain.cmake" <<EOF
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
set(CMAKE_FIND_ROOT_PATH "$prefix")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
EOF
for name in SDL zlib libpng; do
 cmake -S "$cache/deps/$name" -B "$cache/deps/build-$name" -G Ninja -DCMAKE_TOOLCHAIN_FILE="$cache/windows-toolchain.cmake" -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DSDL_TEST=OFF -DSDL_STATIC=OFF -DZLIB_BUILD_EXAMPLES=OFF -DPNG_TESTS=OFF -DPNG_TOOLS=OFF -DPNG_SHARED=ON -DPNG_STATIC=OFF > "$cache/deps-config-$name.log" 2>&1
 cmake --build "$cache/deps/build-$name" --parallel {args.jobs} > "$cache/deps-build-$name.log" 2>&1
 cmake --install "$cache/deps/build-$name" > "$cache/deps-install-$name.log" 2>&1
 echo "Dependency ready: $name"
done
rom=$(wslpath -a -u {q(str(rom))})
cd "$cache/ssrecomp"
python3 - <<'PY_OPT'
from pathlib import Path
import subprocess
p=Path('CMakeLists.txt')
original=subprocess.check_output(['git','show','HEAD:CMakeLists.txt']).decode()
changed=original.replace('target_compile_options(gamecode PRIVATE -O1','target_compile_options(gamecode PRIVATE -O0',1)
if p.read_text() not in (original, changed):
 raise SystemExit('Unexpected cached CMake changes; choose a fresh cache.')
p.write_text(original.replace('target_compile_options(gamecode PRIVATE -O1','target_compile_options(gamecode PRIVATE -O{args.optimization}',1))
PY_OPT
python3 tools/recomp.py --rom "$rom" --layout layouts/soulsilver-us.json melonDS/src gen
python3 tools/apps.py --layout layouts/soulsilver-us.json gen
cmake -S . -B "$cache/windows-build" -G Ninja -DCMAKE_TOOLCHAIN_FILE="$cache/windows-toolchain.cmake" -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=RelWithDebInfo > "$cache/windows-config.log" 2>&1
cmake --build "$cache/windows-build" --parallel {args.jobs} > "$cache/windows-build.log" 2>&1
find "$prefix/bin" -maxdepth 1 -name '*.dll' -type f > "$cache/windows-runtime-files.txt"
for dll in libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll; do
 candidate=$(x86_64-w64-mingw32-g++ -print-file-name="$dll")
 [ -f "$candidate" ] || {{ echo "Missing compiler runtime: $dll" >&2; exit 1; }}
 printf '%s\\n' "$candidate" >> "$cache/windows-runtime-files.txt"
done
"""
    try:
        bash(script)
        listing = bash(f"cat {q(str(cache / 'windows-runtime-files.txt'))}\n", True).decode().splitlines()
        paths = [str(cache / "windows-build/ssrecomp.exe"), *listing]
        copied = {}
        for source in paths:
            name = "ssrecomp-reference.exe" if source.endswith("/ssrecomp.exe") else PurePosixPath(source).name
            payload = bash(f"cat {q(source)}\n", True)
            (output / name).write_bytes(payload)
            copied[name] = hashlib.sha256(payload).hexdigest()
        for source in ["ssrecomp/LICENSE", "ssrecomp/melonDS/LICENSE"]:
            payload = bash(f"cat {q(str(cache / source))}\n", True)
            (output / ("runtime-LICENSE" if source == "ssrecomp/LICENSE" else "melonDS-LICENSE")).write_bytes(payload)
        report = {"purpose": "private compatibility reference; not the strict native product", "base_pin": BASE_PIN,
                  "sources": SOURCES, "rom_sha1": hashlib.sha1(rom.read_bytes()).hexdigest(),
                  "cpu": "ARM9 static recompilation with interpreter fallback; ARM7 interpreter", "artifacts_sha256": copied}
        if args.run_smoke:
            shots = output / "smoke-shots"
            shots.mkdir(exist_ok=True)
            command = [str(output / "ssrecomp-reference.exe"), str(rom), "--headless", "--frames", "900",
                       "--no-save", "--config", str(output / "smoke.cfg"), "--shot-dir", str(shots),
                       "--shot-at", "300,900", "--hash-every", "300"]
            with (output / "smoke.log").open("wb") as log:
                result = subprocess.run(command, cwd=output, stdout=log, stderr=subprocess.STDOUT, timeout=180)
            report["smoke_exit_code"] = result.returncode
            if result.returncode:
                raise RuntimeError(f"Windows smoke failed: {result.returncode}; inspect {output / 'smoke.log'}")
        (output / "aot-reference-report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    except (subprocess.SubprocessError, OSError, RuntimeError) as exc:
        print(f"Reference build failed: {exc}. WSL logs are under {cache}.", file=sys.stderr)
        return 1
    print(f"Private reference built at {output}. Do not publicly distribute game-derived artifacts.")
    print("This does not complete the C++20/SDL3 source port or eliminate CPU emulation.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
