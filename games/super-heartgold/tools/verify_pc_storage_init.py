"""Compare the pre-cleared native PCStorage block with the pinned ARM9 initializer.

The original allocator/NARC/message-container boundary is replaced by bounded,
synthetically mapped services. The original PCStorage initializer and empty
BoxMon encryption code run from the hash-checked ARM9 image.
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
PC_INIT = 0x02073B29
NEW_MSG = 0x0200BAF8
READ_MSG = 0x0200BBF4
DESTROY_MSG = 0x0200BB44
BLOCK_SIZE = 0x122FC


def narc_member(archive, member):
    if len(archive) < 16 or len(archive) > 16 * 1024 * 1024 or archive[:4] != b'NARC':
        raise ValueError('Expected a bounded private message NARC')
    if struct.unpack_from('<I', archive, 8)[0] != len(archive):
        raise ValueError('NARC size field differs from its file')
    cursor, blocks = 16, struct.unpack_from('<H', archive, 14)[0]
    allocations = image = None
    for _ in range(blocks):
        if cursor + 8 > len(archive):
            raise ValueError('Truncated NARC block header')
        signature, size = struct.unpack_from('<4sI', archive, cursor)
        if size < 8 or size > len(archive) - cursor:
            raise ValueError('Invalid NARC block size')
        if signature == b'BTAF':
            if allocations is not None or size < 12:
                raise ValueError('Invalid NARC allocation table')
            count = struct.unpack_from('<H', archive, cursor + 8)[0]
            if count > (size - 12) // 8 or member >= count:
                raise ValueError('Requested message bank is outside the NARC')
            allocations = struct.unpack_from('<II', archive, cursor + 12 + member * 8)
        elif signature == b'GMIF':
            if image is not None:
                raise ValueError('Duplicate NARC image block')
            image = (cursor + 8, cursor + size)
        cursor += size
    if cursor != len(archive) or allocations is None or image is None:
        raise ValueError('Incomplete NARC or trailing bytes')
    begin, end = allocations
    image_start, image_end = image
    start, finish = image_start + begin, image_start + end
    if begin > end or finish > image_end:
        raise ValueError('Message bank range escapes the NARC image block')
    return archive[start:finish]


def read_names(bank):
    if len(bank) < 4:
        raise ValueError('Message bank is truncated')
    count, key = struct.unpack_from('<HH', bank)
    if count < 24 or count > 65535 or 4 + count * 8 > len(bank):
        raise ValueError('Message bank does not contain PC box names')
    names = []
    for message in range(6, 24):
        seed = ((message + 1) * 596947) & 0xffff
        mask = ((key * 765 * (message + 1)) & 0xffff)
        offset, length = struct.unpack_from('<II', bank, 4 + message * 8)
        offset ^= mask | (mask << 16)
        length ^= mask | (mask << 16)
        if not length or length > 20 or offset < 4 + count * 8 or offset & 1 or offset + length * 2 > len(bank):
            raise ValueError(f'PC box name {message} has an invalid encoded range')
        words = []
        for unit in range(length):
            encoded = struct.unpack_from('<H', bank, offset + unit * 2)[0]
            words.append(encoded ^ seed)
            seed = (seed + 18749) & 0xffff
        if words[-1] != 0xffff or words[0] == 0xf100:
            raise ValueError(f'PC box name {message} is packed or lacks a terminator')
        names.append(words)
    return names


def native_bytes(compiler, bank_path):
    root = Path(__file__).resolve().parents[1]
    harness = r'''
#include "hg/pc_storage_init.hpp"
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main(int argc,char**argv) {
#ifdef _WIN32
 if(_setmode(_fileno(stdout),_O_BINARY)==-1) return 1;
#endif
 if(argc!=2) return 2;
 std::ifstream input(argv[1],std::ios::binary);
 std::vector<char> raw((std::istreambuf_iterator<char>(input)),{});
 std::vector<std::byte> bank(raw.size());
 for(std::size_t i=0;i<raw.size();++i) bank[i]=static_cast<std::byte>(static_cast<unsigned char>(raw[i]));
 const auto block=hg::initialize_precleared_pc_storage(bank);
 return std::fwrite(block.bytes.data(),1,block.bytes.size(),stdout)==block.bytes.size()?0:3;
}
'''
    with tempfile.TemporaryDirectory(prefix='hg-pc-init-proof-') as directory:
        folder = Path(directory)
        source, binary = folder / 'probe.cpp', folder / 'probe.exe'
        source.write_text(harness, encoding='utf-8')
        sources = ('pc_storage_init', 'message_data', 'pokemon_crypto')
        subprocess.run([compiler, '-std=c++20', '-O1', '-Wall', '-Wextra', '-Werror',
                        '-I', str(root / 'game/core/include'), str(source),
                        *(str(root / f'game/core/src/{name}.cpp') for name in sources),
                        '-o', str(binary)], check=True, capture_output=True, timeout=90)
        result = subprocess.run([str(binary), str(bank_path)], check=True,
                                capture_output=True, timeout=30).stdout
    if len(result) != BLOCK_SIZE:
        raise ValueError('Native PCStorage block has an unexpected size')
    return result


def prove(image, expected, names):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
    from unicorn.arm_const import (UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0,
                                   UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_SP)
    machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    machine.mem_map(BASE, 0x400000)
    machine.mem_write(BASE, image)
    machine.mem_map(RAM, 0x100000)
    start, guard = RAM + 0x10010, bytes([0x3c]) * 16
    machine.mem_write(start - 16, guard + bytes(BLOCK_SIZE) + guard)
    counts = {'new': 0, 'read': 0, 'destroy': 0}
    allowed_service = {NEW_MSG, READ_MSG, DESTROY_MSG}

    def service(_machine, address, _size, _user):
        if address not in allowed_service:
            return
        if address == NEW_MSG:
            # Arguments: lazy load, msgdata_msg NARC, member 24, default heap.
            regs = tuple(machine.reg_read(reg) for reg in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2))
            if regs != (1, 27, 24):
                raise ValueError(f'Original PCStorage requested unexpected message bank: {regs}')
            machine.reg_write(UC_ARM_REG_R0, RAM + 0x8000)
            counts['new'] += 1
        elif address == READ_MSG:
            msg_data = machine.reg_read(UC_ARM_REG_R0)
            message_id = machine.reg_read(UC_ARM_REG_R1)
            destination = machine.reg_read(UC_ARM_REG_R2)
            if msg_data != RAM + 0x8000 or not 6 <= message_id < 24:
                raise ValueError('Original PCStorage requested an unexpected name message')
            name = names[message_id - 6]
            if destination < start or destination + 40 > start + BLOCK_SIZE:
                raise ValueError('Original PCStorage name destination escaped its block')
            machine.mem_write(destination, struct.pack('<20H', *(name + [0] * (20 - len(name)))))
            counts['read'] += 1
        else:
            if machine.reg_read(UC_ARM_REG_R0) != RAM + 0x8000:
                raise ValueError('Original PCStorage destroyed an unknown message handle')
            counts['destroy'] += 1
        machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))

    def check_write(_machine, _access, address, length, _value, _user):
        if not (start <= address and address + length <= start + BLOCK_SIZE or
                RAM + 0xe0000 <= address and address + length <= RAM + 0x100000):
            raise ValueError(f'Original PCStorage wrote outside target/stack: {address:#x}+{length}')

    machine.hook_add(UC_HOOK_CODE, service)
    machine.hook_add(UC_HOOK_MEM_WRITE, check_write)
    machine.reg_write(UC_ARM_REG_R0, start)
    machine.reg_write(UC_ARM_REG_SP, RAM + 0xfff00)
    machine.reg_write(UC_ARM_REG_LR, RAM | 1)
    machine.emu_start(PC_INIT, RAM, count=2_000_000)
    if machine.reg_read(UC_ARM_REG_PC) != RAM:
        raise ValueError('Original PCStorage initializer exceeded instruction budget')
    actual = bytes(machine.mem_read(start, BLOCK_SIZE))
    if actual != expected:
        first = next((i for i, (a, b) in enumerate(zip(actual, expected)) if a != b), None)
        raise ValueError(f'Native/ARM PCStorage data differs at offset {first:#x}' if first is not None
                         else 'Native/ARM PCStorage lengths differ')
    if counts != {'new': 1, 'read': 18, 'destroy': 1}:
        raise ValueError(f'Original message service call trace differs: {counts}')
    if bytes(machine.mem_read(start - 16, 16)) != guard or bytes(machine.mem_read(start + BLOCK_SIZE, 16)) != guard:
        raise ValueError('Original PCStorage initializer changed boundary guards')
    return dict(result='passed', image_sha256=IMAGE_SHA256, block_bytes=BLOCK_SIZE,
                empty_boxmons=540, name_messages=counts['read'], controlled_services=counts,
                executed_original='PCStorage_Init_and_ZeroBoxMonData',
                substituted_services='bounded_message_bank_load_read_destroy',
                scope='precleared_PCStorage_block_not_complete_newgame')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path)
    parser.add_argument('--archive', required=True, type=Path,
                        help='private msgdata_msg NARC (member 24 is extracted in memory)')
    parser.add_argument('--member', type=int, default=24)
    parser.add_argument('--compiler', default='c++')
    args = parser.parse_args()
    try:
        image = args.arm9.read_bytes()
        if len(image) > 0x400000 or hashlib.sha256(image).hexdigest() != IMAGE_SHA256:
            raise ValueError('ARM9 image exceeds limit or differs from pinned reference')
        if args.member != 24:
            raise ValueError('Pinned SoulSilver USA PCStorage uses message member 24')
        archive = args.archive.read_bytes()
        bank = narc_member(archive, args.member)
        names = read_names(bank)
        with tempfile.TemporaryDirectory(prefix='hg-pc-bank-') as directory:
            private_bank = Path(directory) / 'message-bank.bin'
            private_bank.write_bytes(bank)
            expected = native_bytes(args.compiler, private_bank)
        print(json.dumps(prove(image, expected, names), indent=2))
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
