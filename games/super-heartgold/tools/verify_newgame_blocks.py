"""Compare twenty-six native partial block initializers with original ARM9 functions.

Private bounded investigation only: exact extracted ARM9 image, optional Unicorn,
synthetic RAM, no game/save execution or writing. Compiles the actual authorial
C++ implementation into a temporary host harness; reports metadata only.
"""

import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile


IMAGE_SHA256 = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
BASE = 0x02000000
RAM = 0x01000000
TABLE_OFFSET = 0xf64c4
BLOCKS = ((1, 0x2c, 'player_data'), (4, 0x44c, 'vars_flags'),
          (5, 0x80, 'local_field_data'), (3, 0x79c, 'bag'),
          (8, 0x880, 'pal_pad'), (10, 0x1400, 'map_objects'),
          (11, 0x20, 'link_battle_ruleset'), (17, 0x170, 'seal_case'),
          (18, 0x3ec, 'chatot'), (20, 0xe, 'special_ribbons'),
          (22, 0xf8, 'gts'), (29, 0x28, 'pokeathlon_friendship_records'),
          (33, 0x8, 'follow_mon'), (6, 0x340, 'pokedex'),
          (15, 0x620, 'trainer_card'), (32, 0x8, 'plaza_survey'),
          (36, 0x1294, 'photo_album'), (37, 0xb80, 'pokeathlon'),
          (38, 0x80, 'apricorn_box'), (39, 0x134, 'pokewalker'),
          (40, 0xf00, 'trainer_house'), (2, 0x5b0, 'party'),
          (7, 0x1e0, 'daycare'), (28, 0x688, 'migrated_pokemon'),
          (12, 0x834, 'fashion_data'), (34, 0x658, 'pokegear'))


def native_bytes(cxx):
    root = Path(__file__).resolve().parents[1]
    harness = r'''
#include "hg/newgame_blocks.hpp"
#include <cstdio>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main() {
#ifdef _WIN32
    if (_setmode(_fileno(stdout), _O_BINARY) == -1) return 1;
#endif
    const auto state = hg::initialize_soulsilver_usa_partial_blocks();
    for (const auto block : {std::span<const std::byte>(state.player_data),
                             std::span<const std::byte>(state.vars_flags),
                             std::span<const std::byte>(state.local_field_data),
                             std::span<const std::byte>(state.bag),
                             std::span<const std::byte>(state.pal_pad),
                             std::span<const std::byte>(state.map_objects),
                             std::span<const std::byte>(state.link_battle_ruleset),
                             std::span<const std::byte>(state.seal_case),
                             std::span<const std::byte>(state.chatot),
                             std::span<const std::byte>(state.special_ribbons),
                             std::span<const std::byte>(state.gts),
                             std::span<const std::byte>(state.pokeathlon_friendship_records),
                             std::span<const std::byte>(state.follow_mon),
                             std::span<const std::byte>(state.pokedex),
                             std::span<const std::byte>(state.trainer_card),
                             std::span<const std::byte>(state.plaza_survey),
                             std::span<const std::byte>(state.photo_album),
                             std::span<const std::byte>(state.pokeathlon),
                             std::span<const std::byte>(state.apricorn_box),
                             std::span<const std::byte>(state.pokewalker),
                             std::span<const std::byte>(state.trainer_house),
                             std::span<const std::byte>(state.party),
                             std::span<const std::byte>(state.daycare),
                             std::span<const std::byte>(state.migrated_pokemon),
                             std::span<const std::byte>(state.fashion_data),
                             std::span<const std::byte>(state.pokegear)}) {
        if (std::fwrite(block.data(), 1, block.size(), stdout) != block.size()) return 1;
    }
    return 0;
}
'''
    harness = '#include <span>\n' + harness
    with tempfile.TemporaryDirectory(prefix='shg-newgame-proof-') as temporary:
        folder = Path(temporary)
        source, executable = folder / 'probe.cpp', folder / 'probe.exe'
        source.write_text(harness, encoding='utf-8')
        subprocess.run([cxx, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I', str(root / 'game/core/include'), str(source),
                        str(root / 'game/core/src/newgame_blocks.cpp'),
                        str(root / 'game/core/src/pokemon_crypto.cpp'),
                        '-o', str(executable)], check=True, capture_output=True)
        return subprocess.run([str(executable)], check=True, capture_output=True).stdout


def prove(image, native):
    # Import only after verifying the exact supplied image.
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_MEM_WRITE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC

    if len(native) != sum(size for _, size, _ in BLOCKS):
        raise RuntimeError('Native harness emitted an unexpected number of bytes')
    results, cursor = [], 0
    for index, size, name in BLOCKS:
        block_id, group, size_fn, initializer = struct.unpack_from(
            '<IIII', image, TABLE_OFFSET + index * 16)
        if block_id != index or group != 0:
            raise RuntimeError('Unexpected original save block descriptor')
        for pointer in (size_fn, initializer):
            if not BASE <= (pointer & ~1) < BASE + len(image):
                raise RuntimeError('Original function escapes verified image')
        expected = native[cursor:cursor + size]
        cursor += size
        for pattern in (0, 0xa5, 0xff, 0x5a):
            machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
            machine.mem_map(BASE, 0x400000)
            machine.mem_write(BASE, image)
            machine.mem_map(RAM, 0x100000)
            start = RAM + 0x10010
            guards = bytes([0x3c]) * 16
            machine.mem_write(start - 16, guards + bytes([pattern]) * size + guards)

            def check_write(_machine, _access, address, length, _value, _user):
                in_block = start <= address and address + length <= start + size
                in_stack = RAM + 0xe0000 <= address and address + length <= RAM + 0x100000
                if not in_block and not in_stack:
                    raise RuntimeError(f'Original initializer wrote outside its block/stack: block {index}')

            machine.hook_add(UC_HOOK_MEM_WRITE, check_write)

            def call(address, argument):
                machine.reg_write(UC_ARM_REG_R0, argument)
                machine.reg_write(UC_ARM_REG_SP, RAM + 0xfff00)
                machine.reg_write(UC_ARM_REG_LR, RAM)
                machine.emu_start(address, RAM, count=100000)
                if machine.reg_read(UC_ARM_REG_PC) != RAM:
                    raise RuntimeError('Original function exceeded instruction limit')
                return machine.reg_read(UC_ARM_REG_R0)

            if call(size_fn, 0) != size:
                raise RuntimeError(f'Original raw size differs for block {index}')
            call(initializer, start)
            if bytes(machine.mem_read(start, size)) != expected:
                raise RuntimeError(f'Native/original bytes disagree for block {index}')
            if (bytes(machine.mem_read(start - 16, 16)) != guards
                    or bytes(machine.mem_read(start + size, 16)) != guards):
                raise RuntimeError(f'Original initializer changed guard bytes: block {index}')
        results.append(dict(id=index, name=name, size=size, patterns_compared=4,
                            bytes_sha256=hashlib.sha256(expected).hexdigest()))
    return dict(image_sha256=IMAGE_SHA256, blocks=results,
                scope='twenty_six_raw_initializers_only_not_complete_newgame',
                native_original_byte_comparison='passed',
                original_writes='limited_to_target_block_and_synthetic_stack')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path)
    parser.add_argument('--cxx', default='c++', help='Native GNU-compatible C++20 compiler')
    args = parser.parse_args()
    image = args.arm9.read_bytes()
    if hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
        parser.error('ARM9 image hash does not match the pinned private reference')
    print(json.dumps(prove(image, native_bytes(args.cxx)), indent=2))


if __name__ == '__main__':
    main()
