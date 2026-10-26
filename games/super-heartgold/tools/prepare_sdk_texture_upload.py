"""Repair host address handling in the pinned SDK's direct texture uploads."""
import hashlib
from pathlib import Path

SOURCE_SHA256 = '2bcf9c43a8e19f0cb37a3f57703449db35a5cb1542bc4750039302e0b975872b'


def prepare(source: Path, destination: Path):
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ValueError('Unexpected SDK texture upload source')
    text = data.decode()
    text = text.replace('u32 base1, base2, szBlk1;',
        '#ifdef SDK_PORT\n\tu64 base1, base2;\n\tu32 szBlk1;\n#else\n\tu32 base1, base2, szBlk1;\n#endif')
    old = 'base1 = (u32)(sTexStartAddrTable[tex].blk1 << 12);\n\tbase2 = (u32)(sTexStartAddrTable[tex].blk2 << 12);'
    text = text.replace(old, '#ifdef SDK_PORT\n\tbase1 = sTexStartAddrTable[tex].blk1;\n\tbase2 = sTexStartAddrTable[tex].blk2;\n#else\n\t' + old + '\n#endif')
    text = text.replace('base = (u64)(sTexPlttStartAddrTable[texPltt >> 4] << 12);',
                        'base = sTexPlttStartAddrTable[texPltt >> 4];')
    destination.write_text(text)
