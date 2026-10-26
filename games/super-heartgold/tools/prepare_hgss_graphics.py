"""Adapt pinned HGSS graphics sources to the actual native SDK declarations."""
import hashlib
from pathlib import Path


HASHES = {
    'obj_char_transfer': 'e5bd364a19350f34bf8e04f45b9e75210056b1b3840c560686e3428b2cb93abd',
    'oam_manager': 'f39027c4d9e1547409098100b09105efecc9eb9956739e4c0a8fdd2c7b044b45',
    'gf_g2d_renderer': '2ce29ac2f92166d9aa4dfcf3e2b71c78093c866b6d9887b3fbdaa375c3a5af17',
    'screen_fade_effects': 'c22dd25244a58cea8885c70fd99c0b86227e7c65b649d0ed4a5471378e77149d',
}


def prepare_graphics(source: Path, destination: Path) -> None:
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != HASHES[source.stem]:
        raise ValueError('Graphics source contract differs: ' + str(source))
    text = data.decode()
    if source.stem == 'obj_char_transfer':
        text = text.replace('->mapingType', '->mappingType')
    elif source.stem == 'gf_g2d_renderer':
        # Renderer structures are already exposed by this SDK's public header.
        text = text.replace('#include <nnsys/g2d_renderer_internal.h>',
                            '#include <nnsys/g2d/g2d_Renderer.h>')
    elif source.stem == 'screen_fade_effects':
        # Use the SDK implementation, which also owns the host register boundary.
        text = text.replace('static inline int GX_IsHBlank(void) {\n'
                            '    return reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK;\n}', '')
    else:
        text = text.replace('typedef struct OamInstance {\n    u32 data[7];\n} OamInstance;',
                            'typedef NNSG2dOamManagerInstance OamInstance;')
        text = '\n'.join(line for line in text.split('\n') if not (
            line.startswith(('BOOL NNS_G2d', 'void NNS_G2d', 'u16 NNS_G2d'))
            and 'OamInstance *' in line))
    destination.write_text(text)
