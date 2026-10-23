#!/usr/bin/env python3
"""Valida assets/manifest.json: cada asset é próprio ou tem licença, e nada fica fora do manifesto.

Uso: check_assets.py <pasta assets>
"""

import json
import sys
from pathlib import Path

REQUIRED = ("path", "author", "license", "origin")
ORIGINS = {"original", "third-party"}
# Extensões típicas de dados de DS/GBA que nunca devem aparecer em assets/.
FORBIDDEN_SUFFIXES = {".nds", ".gba", ".narc", ".sdat", ".nsbmd", ".nsbtx", ".nclr", ".ncgr", ".nscr", ".bin", ".sav"}


def check(assets_dir: Path) -> list[str]:
    errors = []
    manifest_path = assets_dir / "manifest.json"
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as e:
        return [f"{manifest_path}: {e}"]

    listed = set()
    for i, entry in enumerate(manifest.get("assets", [])):
        missing = [k for k in REQUIRED if not entry.get(k)]
        if missing:
            errors.append(f"asset #{i}: faltam campos {missing}")
            continue
        if entry["origin"] not in ORIGINS:
            errors.append(f"{entry['path']}: origin deve ser um de {sorted(ORIGINS)}")
        if not (assets_dir / entry["path"]).is_file():
            errors.append(f"{entry['path']}: arquivo não existe")
        listed.add(entry["path"])

    for f in sorted(assets_dir.rglob("*")):
        if not f.is_file() or f.name in ("manifest.json", "README.md", ".gitkeep"):
            continue
        rel = f.relative_to(assets_dir).as_posix()
        if f.suffix.lower() in FORBIDDEN_SUFFIXES:
            errors.append(f"{rel}: tipo de arquivo proibido (dado de ROM)")
        elif rel not in listed:
            errors.append(f"{rel}: não está no manifest.json")
    return errors


def main() -> int:
    if len(sys.argv) != 2:
        print(__doc__.strip())
        return 2
    errors = check(Path(sys.argv[1]))
    for e in errors:
        print(f"erro: {e}")
    print("manifesto de assets OK" if not errors else f"{len(errors)} erro(s)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
