"""Route pinned SPL hardware writes through the native SDK graphics API."""
import hashlib
import re
from pathlib import Path

HASH = 'dac327c8e6c31be256103457bb4af956515334b8b8f7335ec5c00a4dc016a9ba'
HELPERS = r'''
#ifndef SDK_PORT
#error This private adaptation requires the native SDK
#endif
static void SplNativePolygonAttr(u32 value) {
    draw_msg_t message = {0};
    message.type = DRAW_CMD_G3_POLYGONATTR;
    message.data.numU32 = value;
    SIM_HandleG3Command(&message);
}
/* DS VTX10 discards six fractional bits, including for negative values. */
static void SplNativeVtx10(fx16 x, fx16 y, fx16 z) {
    G3_Vtx((fx16)((u16)x & 0xffc0), (fx16)((u16)y & 0xffc0),
           (fx16)((u16)z & 0xffc0));
}
'''


def prepare(source: Path, destination: Path):
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != HASH:
        raise ValueError('SPL drawing source hash differs')
    text = data.decode()
    for register, api, count in [('MTX_SCALE', 'G3_Scale', 1), ('MTX_TRANS', 'G3_Translate', 8)]:
        pattern = rf'reg_G3_{register} = x;\s*reg_G3_{register} = y;\s*reg_G3_{register} = z;'
        text, found = re.subn(pattern, f'{api}(x, y, z);', text)
        if found != count:
            raise ValueError('Unexpected vector writes: ' + register)
    replacements = {
        'TEXCOORD': ('G3_TexCoord(s, t);', 1),
        'VTX_10': ('SplNativeVtx10(x, y, z);', 1),
        'BEGIN_VTXS': ('G3_Begin(GX_BEGIN_QUADS);', 2),
        'END_VTXS': ('G3_End();', 2),
        'MTX_IDENTITY': ('G3_Identity();', 12),
        'COLOR': (None, 8),
        'POLYGON_ATTR': (None, 8),
    }
    for register, (replacement, count) in replacements.items():
        def replace(match):
            if replacement is not None:
                return replacement
            api = 'G3_Color' if register == 'COLOR' else 'SplNativePolygonAttr'
            return f'{api}({match[1]});'
        text, found = re.subn(rf'reg_G3_{register} = ([^;]+);', replace, text)
        if found != count:
            raise ValueError(f'Unexpected {register} writes: {found}, expected {count}')
    text = re.sub(r'    \(void\)reg_G3_POLYGON_ATTR;[^\n]*\n', '', text)
    if 'reg_G3_' in text:
        raise ValueError('Unadapted graphics register access')
    text = text.replace('#include "spl_internal.h"', '#include "spl_internal.h"\n' + HELPERS, 1)
    destination.write_text(text)
