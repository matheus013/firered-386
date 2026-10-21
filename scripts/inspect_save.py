"""Read-only FireRed flash sector inspection; preserve a separate snapshot first."""
import argparse
from collections import Counter
from datetime import datetime
import hashlib
from pathlib import Path
import struct

parser = argparse.ArgumentParser()
parser.add_argument('save', type=Path)
args = parser.parse_args()
data = args.save.read_bytes()
backup_dir = args.save.parent / 'save-backups'
backup_dir.mkdir(exist_ok=True)
backup = backup_dir / (args.save.name + '.' + datetime.now().strftime('%Y%m%d-%H%M%S-%f') + '.bak')
with backup.open('xb') as output:
    output.write(data)
print(f'Backup: {backup}')
print(f'Bytes: {len(data)}; SHA256: {hashlib.sha256(data).hexdigest()}')
print(f'Most common bytes: {Counter(data).most_common(4)}')
for bank in range(2):
    print(f'Slot {bank}:')
    for sector in range(14):
        start = (bank * 14 + sector) * 4096
        if start + 4096 > len(data):
            break
        section, checksum, signature, counter = struct.unpack_from('<HHII', data, start + 0xff4)
        print(f'  sector={bank * 14 + sector:02} section={section:04x} checksum={checksum:04x} signature={signature:08x} counter={counter}')
