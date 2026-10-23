"""Export a cumulative game-source patch against the pinned checkout."""
from pathlib import Path
import subprocess
import shutil
import tempfile
from source_base import extract_base, game_files

root = Path(__file__).resolve().parents[1]
game = root / "game"
if (game / '.git').exists():
    patch = subprocess.check_output(["git", "-C", str(game), "diff", "--binary"])
    untracked = subprocess.check_output(["git", "-C", str(game), "ls-files", "--others", "--exclude-standard"], text=True).splitlines()
    for path in untracked:
        extra = subprocess.run(["git", "diff", "--no-index", "--binary", "--", "/dev/null", path], cwd=game, capture_output=True)
        if extra.returncode != 1:
            raise SystemExit(extra.stderr.decode(errors="replace"))
        patch += extra.stdout
else:
    with tempfile.TemporaryDirectory(prefix='firered-export-') as folder:
        temporary=Path(folder);a=temporary/'a';b=temporary/'b'
        a.mkdir();b.mkdir();extract_base(a)
        for name in game_files():
            target=b/name;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copy2(game/name,target)
            if (a/name).exists():target.chmod((a/name).stat().st_mode)
        diff=subprocess.run(['git','-c','core.autocrlf=false','diff','--no-index','--binary','--','a','b'],cwd=temporary,capture_output=True)
        if diff.returncode not in (0,1):raise SystemExit(diff.stderr.decode(errors='replace'))
        lines=[]
        for line in diff.stdout.splitlines(keepends=True):
            if line.startswith((b'diff --git ',b'--- ',b'+++ ',b'Binary files ')):
                line=line.replace(b'a/a/',b'a/').replace(b'a/b/',b'a/').replace(b'b/b/',b'b/').replace(b'b/a/',b'b/')
            lines.append(line)
        patch=b''.join(lines)
(root / "patches/current.patch").write_bytes(patch)
print("Exported patches/current.patch (cumulative source patch)")
