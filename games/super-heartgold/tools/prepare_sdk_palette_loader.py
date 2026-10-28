"""Adapt pinned native palette descriptors without writing host pointers into DS data."""
import hashlib
from pathlib import Path

HASH = '0f3084de363b5585d49b6d4c5c17a5c119383b14c7e3f3e239086122f043519c'


def prepare(source: Path, destination: Path):
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != HASH:
        raise ValueError('Palette loader source hash differs')
    text = data.decode()
    old = 'static u8 paletteDataOut[NCL_PALETTE_DATA_OUT_MAX][NCL_PALETTE_DATA_OUT_MAX_SIZE] = {0};'
    if text.count(old) != 1: raise ValueError('Palette storage contract differs')
    text = text.replace(old, 'static NNSG2dPaletteData paletteDataOut[NCL_PALETTE_DATA_OUT_MAX] = {0};')
    start = text.index('                void * rawDataPtr;')
    stop = text.index('                #else', start)
    text = text[:start] + '''                const WIN_NNSG2dPaletteData * disk =
                    (const WIN_NNSG2dPaletteData *)&pBinBlk->paletteData;
                NNSG2dPaletteData * native = &paletteDataOut[curPaletteDataOut];
                native->fmt = disk->fmt;
                native->bExtendedPlt = disk->bExtendedPlt;
                native->szByte = disk->szByte;
                native->pRawData = (u8 *)disk + disk->pRawData;
                *ppPltData = native;
                curPaletteDataOut = (curPaletteDataOut + 1) % NCL_PALETTE_DATA_OUT_MAX;
''' + text[stop:]
    destination.write_text(text)
