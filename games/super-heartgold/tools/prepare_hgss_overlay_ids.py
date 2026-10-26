"""Preserve original overlay IDs instead of libntr's SDK_PORT constant zero."""
import hashlib
import re
from pathlib import Path

LSF_SHA256 = '39d41b5e22fa10762f0fbdd060612d7b0c19f328bc0c49477a6788244b067e3b'


def prepare_overlay_ids(reference: Path, output: Path):
    data = reference.read_bytes()
    if hashlib.sha256(data).hexdigest() != LSF_SHA256:
        raise ValueError('Unsupported HGSS main.lsf revision')
    names = re.findall(r'^Overlay (\w+)\s*$', data.decode(), re.MULTILINE)
    if len(names) != 129 or len(set(names)) != len(names):
        raise ValueError('Overlay declaration inventory differs')
    # makelcf assigns t->id = list->num++ when appending overlays. Validate
    # every explicitly numbered name as an additional ordering contract.
    for index, name in enumerate(names):
        if name.startswith('OVY_') and int(name[4:]) != index:
            raise ValueError('Unexpected overlay ordering: ' + name)
    header = '#pragma once\n#include <nitro.h>\n#undef FS_OVERLAY_ID\n'
    header += '#define FS_OVERLAY_ID(name) HGSS_NATIVE_OVERLAY_ID_##name\n'
    header += ''.join(f'#define HGSS_NATIVE_OVERLAY_ID_{name} {index}\n'
                      for index, name in enumerate(names))
    (output / 'hgss_overlay_ids.h').write_text(header)
    check = '#include <stddef.h>\n#include <stdio.h>\n#include <nitro.h>\n'
    check += ''.join(f'_Static_assert(FS_OVERLAY_ID({name}) == {index}, "{name}");\n'
                     for index, name in enumerate(names))
    check += '''int main(void) {
    printf("{\\"pointer_bytes\\":%zu,\\"overlay_header_bytes\\":%zu,"
           "\\"ram_address_offset\\":%zu,\\"file_id_offset\\":%zu}\\n",
           sizeof(void *), sizeof(FSOverlayInfoHeader),
           offsetof(FSOverlayInfoHeader, ram_address), offsetof(FSOverlayInfoHeader, file_id));
    return 0;
}
'''
    (output / 'overlay-contract.c').write_text(check)
    return {'source_sha256': LSF_SHA256, 'overlay_count': len(names),
            'intro_title': names.index('intro_title'), 'continue_menu': names.index('OVY_36')}
