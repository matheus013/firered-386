"""Private differential test against pinned original message/string functions.

Uses a locally available message NARC and pinned source files. Extracted C code
exists only in an external temporary directory. Does not print message contents.
"""

import argparse
import hashlib
from pathlib import Path
import re
import struct
import subprocess
import tempfile


SOURCE_HASHES = {
    'msgdata.c': '2fcd674561c88d39615407f204d21fab4420b5de9506e4fc78651b1c67a1c8d2',
    'pm_string.c': '051bd1380bec7d261c8d5772a0ae77cdb46a7531d74c5d0be4a5f6cb90433e82',
}

HOST = r'''
#include "hg/message_data.hpp"
#include "hg/nitro_graphics.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using BOOL = bool;
constexpr u16 TRNAMECODE = 0xf100, TRNAME_MASK = 0x1ff, EOS_TRNAME = 0x1ff, EOS = 0xffff;
struct MAT_ENTRY { u32 offset, length; };
struct String { u32 size; u16* data; };
void String_Cat(String* dest, String* src) {
    std::copy_n(src->data, src->size, dest->data + dest->size);
    dest->size += src->size;
    dest->data[dest->size] = EOS;
}
'''

HARNESS = r'''
unsigned read16(std::span<const std::byte> data, std::size_t at) {
    if (at > data.size() || data.size() - at < 2) throw std::runtime_error("Read bounds");
    return std::to_integer<unsigned>(data[at]) | (std::to_integer<unsigned>(data[at+1]) << 8);
}
u32 read32(std::span<const std::byte> data, std::size_t at) {
    return read16(data,at) | (static_cast<u32>(read16(data,at+2)) << 16);
}
int main(int argc, char** argv) {
    try {
        if (argc != 3) throw std::runtime_error("Expected archive and bank count");
        std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
        const auto size = file.tellg();
        if (!file || size < 0 || size > 16*1024*1024) throw std::runtime_error("Archive size");
        std::vector<std::byte> archive(static_cast<std::size_t>(size));
        file.seekg(0);
        file.read(reinterpret_cast<char*>(archive.data()), size);
        if (!file) throw std::runtime_error("Archive read");
        const auto banks = std::stoul(argv[2]);
        if (banks > 65535) throw std::runtime_error("Bank limit");
        std::size_t messages{}, packed{}, total_units{};
        for (unsigned group = 0; group < banks; ++group) {
            const auto bank = hg::narc_member(archive, group);
            const auto count = hg::message_count(bank);
            for (unsigned id = 0; id < count; ++id) {
                const auto own_raw = hg::decrypt_message_units(bank, id);
                MAT_ENTRY allocation{read32(bank, 4 + id*8), read32(bank, 8 + id*8)};
                Decrypt1(&allocation, id, read16(bank,2));
                if (allocation.length > 65536 || allocation.offset > bank.size() ||
                    allocation.length > (bank.size() - allocation.offset)/2)
                    throw std::runtime_error("Original allocation outside verified bank");
                std::vector<u16> original(allocation.length);
                for (std::size_t i = 0; i < original.size(); ++i) original[i] = static_cast<u16>(read16(bank, allocation.offset+i*2));
                Decrypt2(original.data(), allocation.length, id);
                if (original != own_raw) throw std::runtime_error("Decryption mismatch");
                const auto decoded = hg::decode_message_data(bank,id);
                if (decoded.packed_trainer_name) {
                    // The native decoder first establishes a bounded packed EOS;
                    // the original function is only exercised on that valid data.
                    std::vector<u16> expanded(131073, 0);
                    String source{static_cast<u32>(original.size()-1),original.data()};
                    String dest{0,expanded.data()};
                    String_Cat_HandleTrainerName(&dest,&source);
                    expanded.resize(dest.size+1);
                    if (expanded != decoded.code_units) throw std::runtime_error("Packed name mismatch");
                    ++packed;
                } else if (decoded.code_units != original) throw std::runtime_error("Ordinary message mismatch");
                ++messages;
                total_units += original.size();
            }
        }
        std::cout << "{\"banks\":" << banks << ",\"messages\":" << messages
                  << ",\"packed_names\":" << packed << ",\"encoded_units\":" << total_units
                  << ",\"all_match\":true}\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
'''


def extract(source, name):
    found = re.search(r'^[^\n;{}]*\b' + re.escape(name) + r'\([^;{}]*\)\s*\{', source, re.M)
    if found is None:
        raise ValueError(f'Missing pinned function {name}')
    end = source.index('{', found.start()) + 1
    depth = 1
    while depth and end < len(source):
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    if depth:
        raise ValueError('Incomplete pinned function')
    return source[found.start():end]


def bank_count(path):
    data = path.read_bytes()
    if len(data) < 16 or len(data) > 16*1024*1024 or data[:4] != b'NARC':
        raise ValueError('Expected a bounded raw message NARC')
    if struct.unpack_from('<I', data, 8)[0] != len(data):
        raise ValueError('Invalid NARC size')
    offset = 16
    for _ in range(struct.unpack_from('<H', data, 14)[0]):
        if offset + 8 > len(data):
            raise ValueError('Truncated archive block')
        size = struct.unpack_from('<I', data, offset+4)[0]
        if size < 8 or size > len(data) - offset:
            raise ValueError('Invalid archive block range')
        if data[offset:offset+4] == b'BTAF':
            if size < 12:
                raise ValueError('Truncated archive allocation table')
            return struct.unpack_from('<H', data, offset+8)[0]
        offset += size
    raise ValueError('Missing archive allocation table')


def verify(source_dir, archive, project, compiler):
    sources = {}
    for name, expected in SOURCE_HASHES.items():
        data = (source_dir / name).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError(f'Pinned source hash mismatch: {name}')
        sources[name] = data.decode('utf-8')
    functions = '\n'.join(extract(sources['msgdata.c'], name) for name in ('Decrypt1', 'Decrypt2'))
    functions += '\n' + '\n'.join(extract(sources['pm_string.c'], name)
                                  for name in ('String_IsTrainerName', 'String_Cat_HandleTrainerName'))
    count = bank_count(archive)
    with tempfile.TemporaryDirectory(prefix='hg-message-reference-') as directory:
        private = Path(directory).resolve()
        if private.is_relative_to(project) or any((parent / '.git').exists() for parent in (private, *private.parents)):
            raise ValueError('System temporary directory must be outside all Git repositories')
        harness = private / 'comparison.cpp'
        executable = private / 'comparison.exe'
        harness.write_text(HOST + functions + HARNESS)
        subprocess.run([compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I', str(project/'game/core/include'), '-I', str(project/'engine/include'),
                        str(harness), str(project/'game/core/src/message_data.cpp'),
                        str(project/'engine/src/nitro_graphics.cpp'),
                        str(project/'engine/src/nitro_compression.cpp'), '-o', str(executable)],
                       check=True, timeout=120)
        subprocess.run([str(executable), str(archive.resolve()), str(count)], check=True, timeout=120)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-dir', required=True, type=Path, help='Pinned local src directory')
    parser.add_argument('--archive', required=True, type=Path, help='Private a/0/2/7 message archive')
    parser.add_argument('--compiler', default='g++')
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    try:
        verify(args.source_dir, args.archive, args.project.resolve(), args.compiler)
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
