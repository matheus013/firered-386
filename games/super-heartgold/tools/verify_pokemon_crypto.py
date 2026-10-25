"""Private differential test against four exact pinned Pokemon C helpers.

Original functions are selected only into a temporary directory outside Git.
No ROM, Pokemon record, original source or generated binary is published.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile

PIN = '6984a3d67592157195940049be2e4bbbf933f9ff'
SOURCES = {
    'src/math_util.c': 'ed76da9a0792494d7b22a029d903db402f5c2cec06212870e0337205be747612',
    'src/pokemon.c': '6ce5b2fce2e911772f04913e6b57e580156e08ebc4ef28d25322ab647e5cbf65',
}
FUNCTIONS = {
    'MonEncryptionLCRNG': ('src/math_util.c', 'b3b1c585d0f85bd9b41ec56a0f479574472ea5e1797ee56970f550a5469c40b9'),
    '_MonEncryptSegment': ('src/math_util.c', '1b541efad042436e1f5359c9889547f136de5a804d2588d145a14def5e21ee6f'),
    '_MonDecryptSegment': ('src/math_util.c', 'cf8aec04d5edff2f0ac4fff8470813b61b0239834f43cf00352b1fe600156280'),
    'CalcMonChecksum': ('src/pokemon.c', '09612764762c5781a0b45220b6ad2e3fe318b68bbc7d673db4b1222cc5a05361'),
}

HARNESS = r'''
#include "hg/pokemon_crypto.hpp"
#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>
extern "C" {
void _MonEncryptSegment(std::uint16_t*, std::uint32_t, std::uint32_t);
void _MonDecryptSegment(std::uint16_t*, std::uint32_t, std::uint32_t);
std::uint32_t CalcMonChecksum(void*, std::uint32_t);
}
std::uint16_t word(const std::vector<std::byte>& data, std::size_t at) {
    return static_cast<std::uint16_t>(std::to_integer<unsigned>(data[at]) |
        (std::to_integer<unsigned>(data[at+1]) << 8));
}
void equal(const std::vector<std::byte>& own, const std::vector<std::uint16_t>& original) {
    for (std::size_t i = 0; i < original.size(); ++i)
        if (word(own, i*2) != original[i]) throw std::runtime_error("Original Pokemon segment differs");
}
int main() {
    try {
        constexpr std::array<std::uint32_t,8> seeds{
            0, 1, 0xffffffff, 0x12345678, 0x80000000, 0xdeadbeef, 0xffff, 0x10000};
        constexpr std::array<unsigned,16> sizes{
            0, 2, 4, 8, 16, 32, 64, 100, 128, 136, 236, 512, 1024, 4096, 65536, 131072};
        unsigned encryptions = 0, decryptions = 0, checksums = 0;
        for (const auto seed : seeds) for (const auto size : sizes) for (unsigned pattern = 0; pattern < 6; ++pattern) {
            std::vector<std::byte> own(size);
            std::uint32_t random = seed ^ 0x9e3779b9;
            for (unsigned i = 0; i < size; ++i) {
                random ^= random << 13; random ^= random >> 17; random ^= random << 5;
                const auto value = pattern == 0 ? 0u : pattern == 1 ? 255u :
                    pattern == 2 ? i & 255u : pattern == 3 ? (i & 1 ? 0xaau : 0x55u) :
                    pattern == 4 ? (i * 73u + 19u) & 255u : random & 255u;
                own[i] = std::byte(value);
            }
            const auto initial = own;
            std::vector<std::uint16_t> original(size/2);
            for (std::size_t i = 0; i < original.size(); ++i) original[i] = word(own, i*2);
            if (hg::pokemon_checksum(own) != CalcMonChecksum(original.data(), size))
                throw std::runtime_error("Initial Pokemon checksum differs");
            ++checksums;
            _MonEncryptSegment(original.data(), size, seed);
            hg::xor_pokemon_segment(own, seed);
            equal(own, original); ++encryptions;
            if (hg::pokemon_checksum(own) != CalcMonChecksum(original.data(), size))
                throw std::runtime_error("Encoded Pokemon checksum differs");
            ++checksums;
            _MonDecryptSegment(original.data(), size, seed);
            hg::xor_pokemon_segment(own, seed);
            equal(own, original);
            if (own != initial) throw std::runtime_error("Pokemon roundtrip differs");
            ++decryptions;
        }
        std::cout << "{\"encryptions_matched\":" << encryptions
                  << ",\"decryptions_matched\":" << decryptions
                  << ",\"checksums_matched\":" << checksums << ",\"all_match\":true}\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
'''


def extract_function(source, name):
    match = re.search(r'^(?:static\s+)?(?:void|u16|u32)\s+' + re.escape(name) + r'\([^;{}]*\)\s*\{', source, re.M)
    if match is None:
        raise ValueError(f'Missing pinned Pokemon function: {name}')
    end = source.index('{', match.start()) + 1
    depth = 1
    while depth and end < len(source):
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    if depth:
        raise ValueError(f'Incomplete pinned Pokemon function: {name}')
    return source[match.start():end]


def verify(args):
    sources = {}
    for relative, expected in SOURCES.items():
        payload = (args.reference / relative).read_bytes()
        if hashlib.sha256(payload).hexdigest() != expected:
            raise ValueError(f'Pinned Pokemon source differs: {relative}')
        sources[relative] = payload.decode()
    selected = []
    for name, (relative, expected) in FUNCTIONS.items():
        function = extract_function(sources[relative], name)
        if hashlib.sha256(function.encode()).hexdigest() != expected:
            raise ValueError(f'Pinned Pokemon function differs: {name}')
        selected.append(function)
    project = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='hg-pokemon-crypto-') as temporary:
        folder = Path(temporary).resolve()
        if folder.is_relative_to(project) or any((p / '.git').exists() for p in (folder, *folder.parents)):
            raise ValueError('Original Pokemon helpers require a temporary folder outside Git')
        original = folder / 'original.c'
        original.write_text('#include <stdint.h>\ntypedef uint16_t u16;\ntypedef uint32_t u32;\n' +
                            '\n\n'.join(selected) + '\n')
        harness = folder / 'probe.cpp'
        harness.write_text(HARNESS)
        original_object = folder / 'original.o'
        executable = folder / 'probe'
        sanitizers = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer'] if args.sanitizers else []
        subprocess.run([args.cc, '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror',
                        '-Wno-sign-compare', *sanitizers, '-c', str(original), '-o', str(original_object)], check=True)
        subprocess.run([args.cxx, '-std=c++20', '-O1', '-Wall', '-Wextra', '-Werror',
                        *sanitizers, '-I' + str(project / 'game/core/include'),
                        str(project / 'game/core/src/pokemon_crypto.cpp'), str(harness),
                        str(original_object), '-o', str(executable)], check=True)
        result = subprocess.run([str(executable)], capture_output=True, text=True, check=True)
        report = json.loads(result.stdout)
        report.update(base_pin=PIN, original_functions=len(selected), seeds=8, data_patterns=6,
                      segment_sizes=16, maximum_segment_bytes=131072,
                      sanitizers=args.sanitizers, scope='Segment XOR and checksum only; no Pokemon layout')
        print(json.dumps(report, sort_keys=True))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('reference', type=Path, help='Private pinned reference checkout')
    parser.add_argument('--cc', default='cc')
    parser.add_argument('--cxx', default='c++')
    parser.add_argument('--sanitizers', action='store_true')
    args = parser.parse_args()
    try:
        verify(args)
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(1, str(error) + '\n')
