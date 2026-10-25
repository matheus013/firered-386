"""Private ARM9 proof of context-explicit native Roamer save block initialization.

Compiles the real production C++ initializer/RNG in a temporary harness. Compares
synthetic block bytes and subsequent MT output with original ARM9 functions.
Requires optional Unicorn and a verified private extracted ARM9 image; no saves,
ROM extraction, clock, game execution, asset output or writes to private inputs.
"""

import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile


IMAGE_SHA256 = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
BASE, RAM = 0x02000000, 0x01000000
CHUNK_TABLE = 0x020f64c4
MT_SEED, MT_NEXT = 0x0201fd75, 0x0201fdb9
MT_STATE, MT_STATE_SIZE, MT_CYCLES = 0x021d15ac, 624 * 4, 0x0210f6cc
SEEDS = (0, 5489, 0xffffffff, 0x12345678)
ADVANCES = (0, 1, 622, 623, 624, 1247)
BLOCK_SIZE = 0x68

HARNESS = r'''
#include "hg/roamer_init.hpp"
#include <array>
#include <cstdio>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif
int main() {
#ifdef _WIN32
    if (_setmode(_fileno(stdout), _O_BINARY) == -1) return 1;
#endif
    for (const auto seed : std::array<std::uint32_t,4>{0,5489,0xffffffff,0x12345678}) {
        for (const auto advance : std::array<unsigned,6>{0,1,622,623,624,1247}) {
            hg::Mt19937 random(seed);
            for (unsigned index = 0; index < advance; ++index) random.next32();
            const auto block = hg::initialize_roamer_block(random);
            if (std::fwrite(block.bytes.data(), 1, block.bytes.size(), stdout) != block.bytes.size()) return 1;
            for (unsigned index = 0; index < 4; ++index) {
                const auto value = random.next32();
                for (unsigned byte = 0; byte < 4; ++byte)
                    if (std::fputc(static_cast<int>((value >> (byte * 8)) & 255), stdout) == EOF) return 1;
            }
        }
    }
}
'''


def native_bytes(compiler):
    root = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='hg-roamer-proof-') as directory:
        temporary = Path(directory).resolve()
        if any((folder / '.git').exists() for folder in (temporary, *temporary.parents)):
            raise ValueError('Temporary proof must stay outside Git repositories')
        source, binary = temporary / 'roamer.cpp', temporary / 'roamer.exe'
        source.write_text(HARNESS, encoding='utf-8')
        subprocess.run([compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I', str(root / 'game/core/include'), str(source),
                        str(root / 'game/core/src/roamer_init.cpp'),
                        str(root / 'game/core/src/random.cpp'), '-o', str(binary)],
                       check=True, capture_output=True, timeout=120)
        return subprocess.run([str(binary)], check=True, capture_output=True, timeout=30).stdout


def prove(image, native):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_MEM_WRITE, UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC

    descriptor = struct.unpack_from('<IIII', image, CHUNK_TABLE - BASE + 21 * 16)
    block_id, group, size_fn, init_fn = descriptor
    if (block_id, group) != (21, 0):
        raise RuntimeError('Unexpected original Roamer descriptor')
    for pointer in (size_fn, init_fn):
        if not BASE <= (pointer & ~1) < BASE + len(image):
            raise RuntimeError('Original Roamer function escapes verified image')

    def thumb_bl_targets(start, end):
        targets = []
        for address in range(start, end - 2, 2):
            first, second = struct.unpack_from('<HH', image, address - BASE)
            if (first & 0xf800) == 0xf000 and (second & 0xf800) == 0xf800:
                offset = ((first & 0x7ff) << 12) | ((second & 0x7ff) << 1)
                if offset & 0x400000:
                    offset -= 0x800000
                targets.append((address + 4 + offset) | 1)
        return targets

    if thumb_bl_targets(init_fn & ~1, (init_fn & ~1) + 0x32) != [MT_NEXT, MT_NEXT]:
        raise RuntimeError('Original Roamer initializer does not call the expected MT function twice')
    if MT_SEED not in thumb_bl_targets(MT_NEXT & ~1, 0x0201fe9c):
        raise RuntimeError('Original MT function does not reference the expected seed function')
    # Literal-pool evidence from the verified SetMTRNGSeed/MTRandom functions.
    for at, expected in ((0x0201fda8, MT_STATE - 8), (0x0201fdac, MT_CYCLES),
                         (0x0201fdb0, MT_STATE + 4), (0x0201fe9c, MT_CYCLES),
                         (0x0201fea4, MT_STATE)):
        if struct.unpack_from('<I', image, at - BASE)[0] != expected:
            raise RuntimeError('Original MT literal pool disagrees with bounded state regions')
    record_size = BLOCK_SIZE + 16
    if len(native) != len(SEEDS) * len(ADVANCES) * record_size:
        raise RuntimeError('Native harness emitted an unexpected result size')
    cursor, comparisons = 0, 0
    for seed in SEEDS:
        for advance in ADVANCES:
            expected = native[cursor:cursor + record_size]
            cursor += record_size
            for pattern in (0, 0xa5, 0xff, 0x5a):
                machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
                machine.mem_map(BASE, 0x400000)
                machine.mem_write(BASE, image)
                machine.mem_map(RAM, 0x100000)
                block = RAM + 0x10010
                guard = bytes([0x3c]) * 16
                machine.mem_write(block - 16, guard + bytes([pattern]) * BLOCK_SIZE + guard)

                def check_write(_machine, _access, address, length, _value, _user):
                    allowed = ((block, block + BLOCK_SIZE), (RAM + 0xe0000, RAM + 0x100000),
                               (MT_STATE, MT_STATE + MT_STATE_SIZE), (MT_CYCLES, MT_CYCLES + 4))
                    if not any(start <= address and address + length <= end for start, end in allowed):
                        raise RuntimeError('Original Roamer/MT function wrote outside bounded regions')

                machine.hook_add(UC_HOOK_MEM_WRITE, check_write)
                tracked = dict(active=False, next_calls=0, seed_calls=0)

                def count_calls(_machine, address, _size, _user):
                    if tracked['active']:
                        if address == (MT_NEXT & ~1):
                            tracked['next_calls'] += 1
                        if address == (MT_SEED & ~1):
                            tracked['seed_calls'] += 1

                machine.hook_add(UC_HOOK_CODE, count_calls, begin=MT_SEED & ~1, end=MT_NEXT & ~1)

                def call(address, argument=0):
                    machine.reg_write(UC_ARM_REG_R0, argument)
                    machine.reg_write(UC_ARM_REG_SP, RAM + 0xfff00)
                    machine.reg_write(UC_ARM_REG_LR, RAM)
                    machine.emu_start(address, RAM, count=100000)
                    if machine.reg_read(UC_ARM_REG_PC) != RAM:
                        raise RuntimeError('Original function exceeded instruction budget')
                    return machine.reg_read(UC_ARM_REG_R0)

                if call(size_fn) != BLOCK_SIZE:
                    raise RuntimeError('Original Roamer block size mismatch')
                call(MT_SEED, seed)
                for _ in range(advance):
                    call(MT_NEXT)
                tracked['active'] = True
                call(init_fn, block)
                tracked['active'] = False
                if tracked['next_calls'] != 2 or tracked['seed_calls'] != 0:
                    raise RuntimeError('Original initializer did not consume exactly two MT values without reseeding')
                actual = bytes(machine.mem_read(block, BLOCK_SIZE))
                actual += struct.pack('<IIII', *(call(MT_NEXT) for _ in range(4)))
                if actual != expected:
                    raise RuntimeError('Original/native Roamer block or MT stream disagrees')
                if (bytes(machine.mem_read(block - 16, 16)) != guard
                        or bytes(machine.mem_read(block + BLOCK_SIZE, 16)) != guard):
                    raise RuntimeError('Original initializer changed guard bytes')
                comparisons += 1
    return dict(image_sha256=IMAGE_SHA256, block_id=21, block_size=BLOCK_SIZE,
                seeds=len(SEEDS), stream_positions=list(ADVANCES), patterns=4,
                native_original_comparisons=comparisons, following_mt_words_checked=4,
                original_MT_calls_per_initializer=2, original_reseeds_per_initializer=0,
                native_metadata_sha256=hashlib.sha256(native).hexdigest(),
                original_writes='block_stack_and_exact_MT_state_regions_only',
                result='passed', scope='one_context_explicit_initializer_not_complete_newgame')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path)
    parser.add_argument('--compiler', default='c++')
    args = parser.parse_args()
    try:
        with args.arm9.open('rb') as stream:
            image = stream.read(0x400001)
        if len(image) > 0x400000:
            raise ValueError('ARM9 investigation image exceeds 4 MiB limit')
        if hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
            raise ValueError('ARM9 image hash does not match the pinned private reference')
        print(json.dumps(prove(image, native_bytes(args.compiler)), indent=2))
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
