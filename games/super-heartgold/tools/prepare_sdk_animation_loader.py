"""Initialize optional native NANR bank pointers in the pinned SDK source."""
import hashlib
from pathlib import Path

HASH = 'b622fe84e401df0a98a9beb5ac63ade51d4912603743a777b0518082c992246c'


def prepare(source: Path, destination: Path):
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != HASH:
        raise ValueError('Animation loader source hash differs')
    text = data.decode()
    old = '    pDataOut->pAnimContents      = (void *)((u64)pDataWin->pAnimContents + (u64)pDataWin);'
    if text.count(old) != 1:
        raise ValueError('Native animation descriptor contract differs')
    # The later branch reconstructs nonzero extended-data offsets separately.
    new = '''    pDataOut->pStringBank = pDataWin->pStringBank
        ? (u8 *)pDataWin + pDataWin->pStringBank : NULL;
    pDataOut->pExtendedData = NULL;
''' + old
    destination.write_text(text.replace(old, new))
