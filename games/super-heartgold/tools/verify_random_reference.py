"""Compare native per-instance RNGs with exact pinned original C functions.

The original global RNGs exist only in the private temporary oracle, never in
the authorial API or published build. No ROM or DS memory structures are used.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile

PIN = '6984a3d67592157195940049be2e4bbbf933f9ff'
SHA256 = 'ed76da9a0792494d7b22a029d903db402f5c2cec06212870e0337205be747612'
FUNCTIONS = {
    'GetLCRNGSeed': '80edfa6fa96fc6769c5bcd2ab621f7a675f5fe61f29f95f6c9a66661b0788f37',
    'SetLCRNGSeed': '42385497a4374b048a70c63bb8fbfbfe254a1d1b4e99bfbc6ec1aae14932ba7c',
    'LCRandom': '58c4cf9adf4e07c0625a95153a43f7143d0bd2de5d8cf8de1e75cba7221e7afd',
    'SetMTRNGSeed': 'b46cb7ae177f36c478f32eddb592bd63ec61b3416588d0cd219e785b7d2da44e',
    'MTRandom': '003861256bc64e44dc35b0afeea6fc167c8575a564bbf60b0a6cf8a097f7cf46',
}

HARNESS = r'''
#include "hg/random.hpp"
#include <array>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>
extern "C" {
std::uint32_t GetLCRNGSeed(void);
void SetLCRNGSeed(std::uint32_t);
std::uint16_t LCRandom(void);
void SetMTRNGSeed(std::uint32_t);
std::uint32_t MTRandom(void);
}
int main() {
    try {
        constexpr unsigned steps = 2500;
        hg::Mt19937 default_mt(5489);
        // Original oracle starts with cycles=625, exercising its default seed.
        for (unsigned i = 0; i < steps; ++i)
            if (default_mt.next32() != MTRandom()) throw std::runtime_error("Default MT differs");
        std::vector<std::uint32_t> seeds{0, 1, 5489, 0xffffffff, 0x12345678,
            0x80000000, 0xdeadbeef, 0xffff};
        std::uint32_t seed_state = 0x6d2b79f5;
        for (unsigned i = 0; i < 256; ++i) {
            seed_state ^= seed_state << 13; seed_state ^= seed_state >> 17; seed_state ^= seed_state << 5;
            seeds.push_back(seed_state);
        }
        if (std::set<std::uint32_t>(seeds.begin(), seeds.end()).size() != seeds.size())
            throw std::runtime_error("Duplicate verification seed");
        std::array<std::vector<std::uint32_t>,4> saved_mt;
        std::array<std::vector<std::uint16_t>,4> saved_lc;
        unsigned lc_matches = 0, mt_matches = 0, state_matches = 0;
        for (std::size_t seed_index = 0; seed_index < seeds.size(); ++seed_index) {
            const auto seed = seeds[seed_index];
            SetLCRNGSeed(seed); SetMTRNGSeed(seed);
            hg::Lcrng lc(seed), lc_noise(~seed);
            hg::Mt19937 mt(seed), mt_noise(~seed);
            if (lc.state() != GetLCRNGSeed()) throw std::runtime_error("LC initial state differs");
            for (unsigned i = 0; i < steps; ++i) {
                const auto expected_lc = LCRandom();
                const auto expected_mt = MTRandom();
                lc_noise.next16(); mt_noise.next32();
                if (lc.next16() != expected_lc) throw std::runtime_error("LC output differs");
                if (mt.next32() != expected_mt) throw std::runtime_error("MT output differs");
                if (lc.state() != GetLCRNGSeed()) throw std::runtime_error("LC state differs");
                ++lc_matches; ++mt_matches; ++state_matches;
                if (seed_index < 4) {
                    saved_lc[seed_index].push_back(expected_lc);
                    saved_mt[seed_index].push_back(expected_mt);
                }
            }
        }
        std::vector<hg::Lcrng> interleaved_lc;
        std::vector<hg::Mt19937> interleaved_mt;
        for (unsigned stream = 0; stream < 4; ++stream) {
            interleaved_lc.emplace_back(seeds[stream]);
            interleaved_mt.emplace_back(seeds[stream]);
        }
        unsigned interleaved_pairs = 0;
        for (unsigned i = 0; i < steps; ++i) for (unsigned stream = 0; stream < 4; ++stream) {
            if (interleaved_mt[stream].next32() != saved_mt[stream][i] ||
                interleaved_lc[stream].next16() != saved_lc[stream][i])
                throw std::runtime_error("Interleaved original-reference stream differs");
            ++interleaved_pairs;
        }
        std::cout << "{\"seeds\":" << seeds.size() << ",\"steps_per_seed\":" << steps
                  << ",\"lc_outputs_matched\":" << lc_matches << ",\"mt_outputs_matched\":" << mt_matches
                  << ",\"lc_states_matched\":" << state_matches << ",\"default_mt_outputs_matched\":" << steps
                  << ",\"interleaved_stream_pairs_matched\":" << interleaved_pairs << ",\"all_match\":true}\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
'''


def extract_function(source, name):
    match = re.search(r'^(?:void|u16|u32)\s+' + re.escape(name) + r'\([^;{}]*\)\s*\{', source, re.M)
    if match is None:
        raise ValueError(f'Missing pinned random function: {name}')
    end = source.index('{', match.start()) + 1
    depth = 1
    while depth and end < len(source):
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    if depth:
        raise ValueError(f'Incomplete pinned random function: {name}')
    return source[match.start():end]


def verify(args):
    payload = (args.reference / 'src/math_util.c').read_bytes()
    if hashlib.sha256(payload).hexdigest() != SHA256:
        raise ValueError('Pinned math_util.c differs')
    source = payload.decode()
    selected = []
    for name, expected in FUNCTIONS.items():
        function = extract_function(source, name)
        if hashlib.sha256(function.encode()).hexdigest() != expected:
            raise ValueError(f'Pinned random function differs: {name}')
        selected.append(function)
    project = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='hg-random-oracle-') as temporary:
        folder = Path(temporary).resolve()
        if folder.is_relative_to(project) or any((p / '.git').exists() for p in (folder, *folder.parents)):
            raise ValueError('Original random helpers require a temporary folder outside Git')
        original = folder / 'original.c'
        original.write_text('#include <stdint.h>\ntypedef uint16_t u16;\ntypedef uint32_t u32;\n'
                            'typedef int32_t s32;\nu32 sMTRNG_State[624];\nu32 sLCRNG_State;\n'
                            'int sMTRNG_Cycles = 625;\nu32 sMTRNG_XOR[2] = {0,0x9908b0df};\n' +
                            '\n\n'.join(selected) + '\n')
        harness = folder / 'probe.cpp'
        harness.write_text(HARNESS)
        original_object = folder / 'original.o'
        executable = folder / 'probe'
        sanitizers = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer'] if args.sanitizers else []
        subprocess.run([args.cc, '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror',
                        *sanitizers, '-c', str(original), '-o', str(original_object)], check=True)
        subprocess.run([args.cxx, '-std=c++20', '-O1', '-Wall', '-Wextra', '-Werror',
                        *sanitizers, '-I' + str(project / 'game/core/include'),
                        str(project / 'game/core/src/random.cpp'), str(harness),
                        str(original_object), '-o', str(executable)], check=True)
        result = subprocess.run([str(executable)], capture_output=True, text=True, check=True)
        report = json.loads(result.stdout)
        report.update(base_pin=PIN, original_functions=len(selected), sanitizers=args.sanitizers,
                      scope='Per-instance LCRNG and MT19937 only; no game/DS global state')
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
