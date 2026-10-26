"""Adapt pinned HGSS graphics sources to the actual native SDK declarations."""
import hashlib
from pathlib import Path


HASHES = {
    'obj_char_transfer': 'e5bd364a19350f34bf8e04f45b9e75210056b1b3840c560686e3428b2cb93abd',
    'oam_manager': 'f39027c4d9e1547409098100b09105efecc9eb9956739e4c0a8fdd2c7b044b45',
}


def prepare_graphics(source: Path, destination: Path) -> None:
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != HASHES[source.stem]:
        raise ValueError('Graphics source contract differs: ' + str(source))
    text = data.decode()
    if source.stem == 'obj_char_transfer':
        text = text.replace('->mapingType', '->mappingType')
    else:
        text = text.replace('typedef struct OamInstance {\n    u32 data[7];\n} OamInstance;',
                            'typedef NNSG2dOamManagerInstance OamInstance;')
        text = '\n'.join(line for line in text.split('\n') if not (
            line.startswith(('BOOL NNS_G2d', 'void NNS_G2d', 'u16 NNS_G2d'))
            and 'OamInstance *' in line))
    destination.write_text(text)
