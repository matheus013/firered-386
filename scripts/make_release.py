"""Package an IPS patch and ROM, validating the patch by reconstructing the ROM."""
from pathlib import Path
import hashlib
import shutil

root = Path(__file__).resolve().parents[1]
out = root / 'output' / 'release'
out.mkdir(parents=True, exist_ok=True)
baseline = (root / 'output/baseline.gba').read_bytes()
modified = (root / 'output/firered-386-dev.gba').read_bytes()
assert hashlib.sha1(baseline).hexdigest() == '41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc'
assert len(baseline) == len(modified) == 0x1000000
patch = bytearray(b'PATCH')
pos = 0
while pos < len(modified):
    if baseline[pos] == modified[pos]:
        pos += 1
        continue
    start = pos
    if start == 0x454f46:  # The record offset must not be mistaken for EOF.
        start -= 1
    pos += 1
    while pos < len(modified) and pos - start < 65535 and baseline[pos] != modified[pos]:
        pos += 1
    patch.extend(start.to_bytes(3, 'big'))
    patch.extend((pos - start).to_bytes(2, 'big'))
    patch.extend(modified[start:pos])
patch.extend(b'EOF')

restored = bytearray(baseline)
cursor = 5
while patch[cursor:cursor + 3] != b'EOF':
    offset = int.from_bytes(patch[cursor:cursor + 3], 'big')
    length = int.from_bytes(patch[cursor + 3:cursor + 5], 'big')
    assert length > 0
    restored[offset:offset + length] = patch[cursor + 5:cursor + 5 + length]
    cursor += 5 + length
assert bytes(restored) == modified, 'IPS reconstruction failed'
prefix = 'firered-386-v0.1.0-dev'
(out / f'{prefix}.gba').write_bytes(modified)
(out / f'{prefix}.ips').write_bytes(patch)
shutil.copyfile(root / 'patches/current.patch', out / f'{prefix}-source.patch')
shutil.copyfile(root / 'README.md', out / 'LEIA-ME.md')
files = sorted(p for p in out.iterdir() if p.name != 'SHA256SUMS.txt' and p.is_file())
(out / 'SHA256SUMS.txt').write_text(''.join(
    f'{hashlib.sha256(p.read_bytes()).hexdigest()}  {p.name}\n' for p in files), encoding='utf-8')
print(f'IPS verified against baseline; {len(patch)} bytes. Assets: {out}')
