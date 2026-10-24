"""Private function-level proof of HG/SS save layout and CRC.

Requires an already extracted, exact local ARM9 image and the optional Python
unicorn package. Does not extract a ROM, read saves, or run the game. No original
instructions or data are emitted. The executor is investigation-only.
"""

import argparse
import binascii
import hashlib
import json
from pathlib import Path
import random
import struct


IMAGE_SHA256 = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
BASE = 0x02000000
RAM = 0x01000000
TABLE = RAM + 0x1000
HEADERS = RAM + 0x3000
SPECS = RAM + 0x4000
DATA = RAM + 0x10000
STACK = RAM + 0xfff00
CHUNK_TABLE = 0x020f64c4


def prove(image):
    # Imported only after the caller verifies the exact image hash.
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC)
    machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    machine.mem_map(BASE, 0x400000)
    machine.mem_write(BASE, image)
    machine.mem_map(RAM, 0x100000)

    def call(address, *arguments, limit=100000):
        machine.reg_write(UC_ARM_REG_SP, STACK)
        machine.reg_write(UC_ARM_REG_LR, RAM)
        for register, value in zip((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2), arguments):
            machine.reg_write(register, value)
        machine.emu_start(address, RAM, count=limit)
        if machine.reg_read(UC_ARM_REG_PC) != RAM:
            raise RuntimeError(f'Function {address:#x} did not return within instruction limit')
        return machine.reg_read(UC_ARM_REG_R0)

    sizes = []
    for index in range(42):
        offset = CHUNK_TABLE - BASE + index * 16
        block_id, group, size_function, _initializer = struct.unpack_from('<IIII', image, offset)
        if block_id != index or group != (index == 41):
            raise RuntimeError('Unexpected save chunk table entry')
        if not BASE <= (size_function & ~1) < BASE + len(image):
            raise RuntimeError('Size function escapes verified image')
        sizes.append(call(size_function, limit=100))

    call(0x02027e5d, HEADERS)
    call(0x02027efd, SPECS, HEADERS)
    specs = []
    for index in range(2):
        chunk, page, pages, _padding, offset, size = struct.unpack(
            '<BBBBII', machine.mem_read(SPECS + index * 12, 12))
        specs.append(dict(id=chunk, first_page=page, pages=pages, offset=offset, size=size))
    expected_specs = [dict(id=0, first_page=0, pages=16, offset=0, size=0xf628),
                      dict(id=1, first_page=16, pages=19, offset=0xf700, size=0x12310)]
    if specs != expected_specs:
        raise RuntimeError('Original slot specification differs from native format')
    stored_sizes = [((size + 3) & ~3) + 4 for size in sizes]
    if sum(stored_sizes[:41]) + 16 != specs[0]['size'] or stored_sizes[41] + 16 != specs[1]['size']:
        raise RuntimeError('Substructure sizes disagree with original complete initialization')

    # Actual SDK table generator and original GF wrapper on synthetic memory.
    call(0x020e3850, TABLE, 0x1021)
    machine.mem_write(0x021d15a4, struct.pack('<I', TABLE))
    rng = random.Random(0x20060623)
    vectors = [b'', b'123456789', b'\x00', b'\xff', bytes(range(256))]
    vectors += [bytes(rng.randrange(256) for _ in range(size))
                for size in (2, 3, 15, 16, 255, 512, 4096, 0xf618, 0x12300)]
    results = []
    for payload in vectors:
        if payload:
            machine.mem_write(DATA, payload)
        sdk_crc = call(0x020e3a2c, TABLE, DATA, len(payload), limit=3000000)
        wrapper_crc = call(0x0201ff99, DATA, len(payload), limit=3000000)
        independent_crc = binascii.crc_hqx(payload, 0xffff)
        if sdk_crc != independent_crc or wrapper_crc != independent_crc:
            raise RuntimeError(f'CRC disagreement for length {len(payload)}')
        results.append(dict(bytes=len(payload), crc=independent_crc))
    return dict(image_sha256=IMAGE_SHA256, raw_substructure_sizes=sizes,
                original_slot_specs=specs, crc_vectors=results,
                scope='bounded_original_functions_on_synthetic_memory_only')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path, help='Exact private decompressed ARM9 image')
    args = parser.parse_args()
    image = args.arm9.read_bytes()
    if hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
        parser.error('ARM9 image hash does not match the pinned private reference')
    print(json.dumps(prove(image), indent=2))


if __name__ == '__main__':
    main()
