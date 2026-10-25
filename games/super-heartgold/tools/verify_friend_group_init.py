"""Compare native FriendGroup bytes to the pinned ARM9 block initializer."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

IMAGE_SHA256 = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
BASE, RAM = 0x02000000, 0x01000000
CHUNK_TABLE, FRIEND_GROUP_GLOBAL = 0x020F64C4, 0x021D2AF0
BLOCK_SIZE = 0x108
PATTERNS = (0, 0xa5, 0xff, 0x5a)


def native_bytes(compiler):
    root = Path(__file__).resolve().parents[1]
    harness = r'''
#include "hg/friend_group_init.hpp"
#include <cstdio>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main() {
#ifdef _WIN32
 if(_setmode(_fileno(stdout),_O_BINARY)==-1) return 1;
#endif
 const auto block=hg::initialize_friend_group_block();
 return std::fwrite(block.data(),1,block.size(),stdout)==block.size()?0:2;
}
'''
    with tempfile.TemporaryDirectory(prefix='hg-friend-group-proof-') as directory:
        folder = Path(directory)
        source, binary = folder / 'probe.cpp', folder / 'probe.exe'
        source.write_text(harness, encoding='utf-8')
        subprocess.run([compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I', str(root / 'game/core/include'), str(source),
                        str(root / 'game/core/src/friend_group_init.cpp'), '-o', str(binary)],
                       check=True, capture_output=True, timeout=90)
        result = subprocess.run([str(binary)], check=True, capture_output=True,
                                timeout=30).stdout
    if len(result) != BLOCK_SIZE:
        raise ValueError('Native FriendGroup block has an unexpected size')
    return result


def prove(image, expected):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_MEM_WRITE
    from unicorn.arm_const import (UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0,
                                   UC_ARM_REG_SP)
    descriptor = tuple(int.from_bytes(image[CHUNK_TABLE - BASE + 14 * 16 + i:CHUNK_TABLE - BASE + 14 * 16 + i + 4],
                                     'little') for i in range(0, 16, 4))
    if descriptor[:2] != (14, 0) or descriptor[2] & ~1 < BASE or descriptor[3] & ~1 < BASE:
        raise ValueError('Pinned FriendGroup descriptor differs')
    comparisons = 0
    binding_writes = 0
    for pattern in PATTERNS:
        machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        machine.mem_map(BASE, 0x400000)
        machine.mem_write(BASE, image)
        machine.mem_map(RAM, 0x100000)
        start, guard = RAM + 0x10010, bytes([0x3c]) * 16
        machine.mem_write(start - 16, guard + bytes([pattern]) * BLOCK_SIZE + guard)

        def check_write(_machine, _access, address, length, value, _user):
            nonlocal binding_writes
            in_block = start <= address and address + length <= start + BLOCK_SIZE
            in_stack = RAM + 0xe0000 <= address and address + length <= RAM + 0x100000
            if in_block or in_stack:
                return
            if address == FRIEND_GROUP_GLOBAL and length == 4 and value == start:
                binding_writes += 1
                return
            raise ValueError(f'FriendGroup wrote outside block/stack/binding: {address:#x}+{length}')

        machine.hook_add(UC_HOOK_MEM_WRITE, check_write)

        def call(address, argument):
            machine.reg_write(UC_ARM_REG_R0, argument)
            machine.reg_write(UC_ARM_REG_SP, RAM + 0xfff00)
            machine.reg_write(UC_ARM_REG_LR, RAM | 1)
            machine.emu_start(address, RAM, count=200000)
            if machine.reg_read(UC_ARM_REG_PC) != RAM:
                raise ValueError('Original FriendGroup initializer exceeded its budget')
            return machine.reg_read(UC_ARM_REG_R0)

        if call(descriptor[2], 0) != BLOCK_SIZE:
            raise ValueError('Original FriendGroup raw size differs')
        call(descriptor[3], start)
        if bytes(machine.mem_read(start, BLOCK_SIZE)) != expected:
            raise ValueError('Native and original FriendGroup bytes differ')
        if bytes(machine.mem_read(FRIEND_GROUP_GLOBAL, 4)) != start.to_bytes(4, 'little'):
            raise ValueError('Original global binding does not point to the target block')
        if bytes(machine.mem_read(start - 16, 16)) != guard or bytes(machine.mem_read(start + BLOCK_SIZE, 16)) != guard:
            raise ValueError('Original initializer changed guard bytes')
        comparisons += 1
    if binding_writes != len(PATTERNS):
        raise ValueError(f'Unexpected number of global-binding writes: {binding_writes}')
    return dict(result='passed', image_sha256=IMAGE_SHA256, block_id=14,
                comparisons=comparisons, block_bytes=BLOCK_SIZE,
                empty_groups=6, original_global_binding_writes=binding_writes,
                scope='FriendGroup_block_bytes_and_active_save_binding_not_newgame')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path)
    parser.add_argument('--compiler', default='c++')
    args = parser.parse_args()
    try:
        image = args.arm9.read_bytes()
        if len(image) > 0x400000 or hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
            raise ValueError('ARM9 image exceeds limit or differs from pinned reference')
        print(json.dumps(prove(image, native_bytes(args.compiler)), indent=2))
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
