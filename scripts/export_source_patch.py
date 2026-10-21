"""Export a cumulative game-source patch against the pinned checkout."""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
game = root / "game"
patch = subprocess.check_output(["git", "-C", str(game), "diff", "--binary"])
untracked = subprocess.check_output(["git", "-C", str(game), "ls-files", "--others", "--exclude-standard"], text=True).splitlines()
for path in untracked:
    extra = subprocess.run(["git", "diff", "--no-index", "--binary", "--", "/dev/null", path], cwd=game, capture_output=True)
    if extra.returncode != 1:
        raise SystemExit(extra.stderr.decode(errors="replace"))
    patch += extra.stdout
(root / "patches/current.patch").write_bytes(patch)
print("Exported patches/current.patch (cumulative source patch)")
