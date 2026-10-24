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

    if not isinstance(manifest, dict) or manifest.get('version') != 1 or not isinstance(manifest.get('assets'), list):
        return ['manifest.json: expected version=1 and an assets array']
    listed = set()
    for i, entry in enumerate(manifest.get("assets", [])):
        if not isinstance(entry, dict):
            errors.append(f'asset #{i}: expected an object')
            continue
        missing = [k for k in REQUIRED if not entry.get(k)]
        if missing:
            errors.append(f"asset #{i}: faltam campos {missing}")
            continue
        if not all(isinstance(entry[k], str) for k in REQUIRED):
            errors.append(f'asset #{i}: required fields must be strings')
            continue
        path = Path(entry['path'])
        if path.is_absolute() or '..' in path.parts or '\\' in entry['path'] or ':' in entry['path']:
            errors.append(f"{entry['path']}: path must stay inside assets/")
            continue
        try:
            (assets_dir / path).resolve().relative_to(assets_dir.resolve())
        except ValueError:
            errors.append(f"{entry['path']}: resolved path leaves assets/")
            continue
        if entry['path'] in listed:
            errors.append(f"{entry['path']}: duplicate asset")
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
