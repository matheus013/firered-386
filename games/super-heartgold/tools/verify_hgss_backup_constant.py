"""Verify backup literals in the user's private, decompressed SS USA ARM9."""
import argparse
import hashlib
import json
import struct
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('arm9', type=Path)
args = parser.parse_args()
data = args.arm9.read_bytes()
expected = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
if hashlib.sha256(data).hexdigest() != expected:
    raise SystemExit('Unsupported ARM9: SHA-256 mismatch')
extended, fallback = struct.unpack_from('<II', data, 0x28750)
if (extended, fallback) != (0xff1302, 0x1302):
    raise SystemExit('Backup literal contract differs')
print(json.dumps({'passed': True, 'extended': hex(extended),
                  'fallback': hex(fallback), 'vendor': extended >> 16,
                  'capacity_bytes': 1 << ((extended >> 8) & 255),
                  'storage_runtime_tested': False}, indent=2))
