"""Private ARM9 proof of pre-clear + Mailbox/Safari initialization with identity.

The real native C++ is compared against a verified ARM9 image on synthetic RAM.
Original language/version bytes are configured before write hooks are enabled.
No private inputs, saves or extracted assets are written or emitted.
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
LANGUAGE, VERSION = 0x020f5670, 0x020f566c
ORIGINAL_MEMSET = 0x020e5b44
IDENTITIES = ((0, 0), (1, 7), (2, 8), (3, 7), (4, 8), (5, 7), (7, 8), (255, 255))
BLOCKS = ((13, 0x460), (35, 0x5fc))

HARNESS = r'''
#include "hg/identity_init.hpp"
#include <array>
#include <cstdio>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main() {
#ifdef _WIN32
    if (_setmode(_fileno(stdout), _O_BINARY) == -1) return 1;
#endif
    for (const auto identity : std::array<hg::GameIdentity,8>{{
        {0,0},{1,7},{2,8},{3,7},{4,8},{5,7},{7,8},{255,255}}}) {
        const auto mailbox = hg::initialize_precleared_mailbox(identity);
        const auto safari = hg::initialize_precleared_safari(identity);
        if (std::fwrite(mailbox.bytes.data(), 1, mailbox.bytes.size(), stdout) != mailbox.bytes.size()) return 1;
        if (std::fwrite(safari.bytes.data(), 1, safari.bytes.size(), stdout) != safari.bytes.size()) return 1;
    }
}
'''


def native_bytes(compiler):
    root = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='hg-identity-proof-') as directory:
        temporary = Path(directory).resolve()
        if any((folder / '.git').exists() for folder in (temporary, *temporary.parents)):
            raise ValueError('Temporary proof must stay outside Git repositories')
        source, binary = temporary / 'identity.cpp', temporary / 'identity.exe'
        source.write_text(HARNESS, encoding='utf-8')
        subprocess.run([compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I', str(root / 'game/core/include'), str(source),
                        str(root / 'game/core/src/identity_init.cpp'), '-o', str(binary)],
                       check=True, capture_output=True, timeout=120)
        return subprocess.run([str(binary)], check=True, capture_output=True, timeout=30).stdout


def prove(image, native):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_MEM_WRITE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC)
    # Original helper literal pools must identify these exact metadata bytes.
    for literal, target in ((0x0202b0ac, LANGUAGE), (0x0202b0b0, VERSION),
                            (0x0202fa34, LANGUAGE), (0x0202fa38, VERSION)):
        if struct.unpack_from('<I', image, literal - BASE)[0] != target:
            raise RuntimeError('Original identity literal pool mismatch')
    if len(native) != len(IDENTITIES) * sum(size for _, size in BLOCKS):
        raise RuntimeError('Native harness emitted an unexpected result size')
    cursor, comparisons, retained_cases = 0, 0, 0
    for language, version in IDENTITIES:
        for index, size in BLOCKS:
            expected = native[cursor:cursor + size]
            cursor += size
            block_id, group, size_fn, init_fn = struct.unpack_from(
                '<IIII', image, CHUNK_TABLE - BASE + index * 16)
            if (block_id, group) != (index, 0):
                raise RuntimeError('Unexpected identity block descriptor')
            for pointer in (size_fn, init_fn):
                if not BASE <= (pointer & ~1) < BASE + len(image):
                    raise RuntimeError('Original initializer escapes verified image')
            for pattern in (0, 0xa5, 0xff, 0x5a):
                machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
                machine.mem_map(BASE, 0x400000)
                machine.mem_write(BASE, image)
                machine.mem_map(RAM, 0x100000)
                machine.mem_write(LANGUAGE, bytes([language]))
                machine.mem_write(VERSION, bytes([version]))
                start = RAM + 0x10010
                guard = bytes([0x3c]) * 16
                machine.mem_write(start - 16, guard + bytes([pattern]) * size + guard)

                def check_write(_machine, _access, address, length, _value, _user):
                    in_block = start <= address and address + length <= start + size
                    in_stack = RAM + 0xe0000 <= address and address + length <= RAM + 0x100000
                    if not in_block and not in_stack:
                        raise RuntimeError('Original initializer wrote outside target/stack or modified identity')

                machine.hook_add(UC_HOOK_MEM_WRITE, check_write)

                def call(address, *arguments):
                    for register, value in zip((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2), arguments):
                        machine.reg_write(register, value)
                    machine.reg_write(UC_ARM_REG_SP, RAM + 0xfff00)
                    machine.reg_write(UC_ARM_REG_LR, RAM)
                    machine.emu_start(address, RAM, count=100000)
                    if machine.reg_read(UC_ARM_REG_PC) != RAM:
                        raise RuntimeError('Original function exceeded instruction budget')
                    return machine.reg_read(UC_ARM_REG_R0)

                if call(size_fn) != size:
                    raise RuntimeError('Original raw block size differs')
                # Negative control proves why this is a pre-clear contract:
                # standalone original init preserves named fields from callers.
                call(init_fn, start)
                naked = bytes(machine.mem_read(start, size))
                retained = (tuple(slot * 56 + 34 + message * 8 + byte
                                  for slot in range(20) for message in range(3) for byte in range(2))
                            if index == 13 else (0x5f8, 0x5f9, 0x5fa, 0x5fb))
                if any(naked[offset] != pattern for offset in retained):
                    raise RuntimeError('Original standalone initializer changed expected retained fields')
                if pattern:
                    if naked == expected:
                        raise RuntimeError('Missing pre-clear negative control failed to distinguish state')
                    retained_cases += 1
                # Execute an original bounded memset primitive for the explicit
                # pre-clear, then the exact original block initializer again.
                call(ORIGINAL_MEMSET, start, 0, size)
                call(init_fn, start)
                if bytes(machine.mem_read(start, size)) != expected:
                    raise RuntimeError('Original pre-clear/init and native block disagree')
                if (bytes(machine.mem_read(start - 16, 16)) != guard
                        or bytes(machine.mem_read(start + size, 16)) != guard):
                    raise RuntimeError('Original pre-clear/initializer changed guards')
                if (bytes(machine.mem_read(LANGUAGE, 1)) != bytes([language])
                        or bytes(machine.mem_read(VERSION, 1)) != bytes([version])):
                    raise RuntimeError('Original initializer changed configured identity globals')
                comparisons += 1
    return dict(image_sha256=IMAGE_SHA256, identities=len(IDENTITIES), blocks=[13, 35],
                patterns=4, native_original_comparisons=comparisons,
                negative_controls_without_preclear=retained_cases,
                identity_globals='configured_before_hooks_and_preserved',
                original_writes='target_block_and_synthetic_stack_only',
                native_metadata_sha256=hashlib.sha256(native).hexdigest(),
                result='passed', scope='explicit_preclear_identity_blocks_not_complete_newgame')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path)
    parser.add_argument('--compiler', default='c++')
    args = parser.parse_args()
    try:
        with args.arm9.open('rb') as stream:
            image = stream.read(0x400001)
        if len(image) > 0x400000 or hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
            raise ValueError('ARM9 image exceeds limit or differs from pinned private reference')
        print(json.dumps(prove(image, native_bytes(args.compiler)), indent=2))
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
