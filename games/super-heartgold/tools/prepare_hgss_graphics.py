"""Adapt pinned HGSS graphics sources to the actual native SDK declarations."""
import hashlib
from pathlib import Path


HASHES = {
    'obj_char_transfer': 'e5bd364a19350f34bf8e04f45b9e75210056b1b3840c560686e3428b2cb93abd',
    'oam_manager': 'f39027c4d9e1547409098100b09105efecc9eb9956739e4c0a8fdd2c7b044b45',
    'gf_g2d_renderer': '2ce29ac2f92166d9aa4dfcf3e2b71c78093c866b6d9887b3fbdaa375c3a5af17',
    'screen_fade_effects': 'c22dd25244a58cea8885c70fd99c0b86227e7c65b649d0ed4a5471378e77149d',
    'particle_system': '1af2e73b1668a53b20b9b6c5a778520e0785b0133099d32f317fd2e4a9225a66',
}


def prepare_graphics(source: Path, destination: Path) -> None:
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != HASHES[source.stem]:
        raise ValueError('Graphics source contract differs: ' + str(source))
    text = data.decode()
    if source.stem == 'obj_char_transfer':
        text = text.replace('->mapingType', '->mappingType')
    elif source.stem == 'particle_system':
        text = text.replace('#include "nnsys/gfd_internal.h"',
                            '#include <nnsys/gfd.h>\n#include <stdint.h>\n#include <string.h>')
        # Preserve the original storage without aliasing an array as an SDK struct.
        for kind, field in [('Tex', 'texKeys'), ('Pltt', 'plttKeys')]:
            state = f'NNSGfdFrm{kind}VramState'
            for operation in ('Get', 'Set'):
                call = f'NNS_Gfd{operation}Frm{kind}VramState'
                before = f'{call}(system->{field});'
                copy_in = f'memcpy(&nativeState, system->{field}, sizeof(nativeState));'
                copy_out = f'memcpy(system->{field}, &nativeState, sizeof(nativeState));'
                body = [f'{state} nativeState;',
                        f'_Static_assert(sizeof(nativeState) <= sizeof(system->{field}), "VRAM state storage");']
                if operation == 'Set': body.append(copy_in)
                body.append(f'{call}(&nativeState);')
                if operation == 'Get': body.append(copy_out)
                text = text.replace(before, '{ ' + ' '.join(body) + ' }')
        # SPL runtime structs contain host pointers; allocations need host alignment.
        text = text.replace('(u32)next & 3', '(uintptr_t)next & (sizeof(void *) - 1)')
        text = text.replace('next += 4 - ', 'next += sizeof(void *) - ')
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
