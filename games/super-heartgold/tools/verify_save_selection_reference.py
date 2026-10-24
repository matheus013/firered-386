"""Compare the native save selection policy with exact pinned original functions.

The original source must already exist locally. No download or save-file access
is performed. Extracted functions and the comparison executable exist only in a
temporary directory outside Git and are deleted when the run finishes.
"""

import argparse
import hashlib
from pathlib import Path
import re
import subprocess
import tempfile


SOURCE_SHA256 = '4eed1844a349b1b761f51ee41b53922c1edf0095de2d88c8ed1a31108e545584'
FUNCTIONS = (
    'SaveCounterCompare',
    'SaveSlotCheckCompare',
    'Save_RecordWhichLatestGoodSector',
    'Save_GetSaveFilesStatus',
)

# Host fixtures replace IO/allocation/footer validation only. The selection and
# original counter comparison are inserted verbatim from the hash-checked source.
HOST_FIXTURES = r'''
#include "hg/save_selection.hpp"
#include <cstdint>
#include <iostream>
#include <stdexcept>
using u8 = std::uint8_t;
using u32 = std::uint32_t;
struct SaveSlotCheck { bool valid; u32 count; };
struct SaveData {
    u32 saveCounter = 0;
    unsigned lastGoodSector = 0;
    unsigned sectorCleanFlag[2]{};
};
SaveSlotCheck fixture[2][2];
constexpr int HEAP_ID_3 = 0, SAVE_PAGE_MAX = 1, SAVE_SECTOR_SIZE = 1;
constexpr int LOAD_STATUS_NOT_EXIST = 0, LOAD_STATUS_TOTAL_FAIL = 1;
constexpr int LOAD_STATUS_SLOT_FAIL = 2, LOAD_STATUS_IS_GOOD = 3;
u8* Heap_AllocAtEnd(int, int) { return new u8[1]; }
void Heap_Free(u8* memory) { delete[] memory; }
bool FlashLoadChunk(unsigned address, u8* data, int) {
    *data = static_cast<u8>(address / 0x40000);
    return true;
}
void SaveSlotCheck_InitFromSavedat(SaveSlotCheck* check, SaveData*, u8* data, int index) {
    *check = fixture[*data][index];
    if (!check->valid) check->count = 0;
}
void SaveSlotCheck_InitDummy(SaveSlotCheck* check) { *check = {false, 0}; }
#define GF_ASSERT(test) do { if (!(test)) throw std::logic_error("reference assertion"); } while (false)
#define DebugPrn_MirrorValid(value) ((void)0)
'''

COMPARISON = r'''
int main() {
    constexpr u32 counters[] = {0, 1, 2, 0x7fffffff, 0x80000000, 0xfffffffe, 0xffffffff};
    unsigned tested = 0, assertions = 0, unsafe_selections = 0;
    for (unsigned mask = 0; mask < 16; ++mask)
    for (u32 a : counters) for (u32 b : counters)
    for (u32 c : counters) for (u32 d : counters) {
        std::array<hg::SaveBankChecks, 2> banks{{
            {{bool(mask & 1), a}, {bool(mask & 2), b}},
            {{bool(mask & 4), c}, {bool(mask & 8), d}}
        }};
        for (unsigned i = 0; i < 2; ++i) {
            fixture[i][0] = {banks[i].normal.valid, banks[i].normal.counter};
            fixture[i][1] = {banks[i].pc_storage.valid, banks[i].pc_storage.counter};
        }
        SaveData data;
        int expected;
        try { expected = Save_GetSaveFilesStatus(&data); }
        catch (const std::logic_error&) {
            expected = LOAD_STATUS_TOTAL_FAIL;
            ++assertions;
        }
        const auto actual = hg::select_save_bank(banks);
        const auto status = expected == LOAD_STATUS_NOT_EXIST ? hg::SaveSelectionStatus::NoSave
                          : expected == LOAD_STATUS_TOTAL_FAIL ? hg::SaveSelectionStatus::Corrupt
                          : expected == LOAD_STATUS_SLOT_FAIL ? hg::SaveSelectionStatus::Recovered
                          : hg::SaveSelectionStatus::Good;
        const bool selected = expected == LOAD_STATUS_SLOT_FAIL || expected == LOAD_STATUS_IS_GOOD;
        const bool both_valid = selected && banks[data.lastGoodSector].normal.valid
                                        && banks[data.lastGoodSector].pc_storage.valid;
        if (actual.status != status || actual.bank.has_value() != selected ||
            actual.selected_chunks_valid != both_valid ||
            (selected && (actual.bank != data.lastGoodSector || actual.counter != data.saveCounter)) ||
            (!selected && actual.counter != 0)) {
            std::cerr << "Mismatch mask " << mask << " counters "
                      << a << ',' << b << ',' << c << ',' << d << '\n';
            return 1;
        }
        if (selected && !both_valid) ++unsafe_selections;
        ++tested;
    }
    std::cout << "Exact original selection differential: " << tested << " cases passed\n"
              << "Original assertions normalized to Corrupt: " << assertions << '\n'
              << "Original selections with an invalid chosen chunk, explicitly exposed: "
              << unsafe_selections << '\n';
}
'''


def extract_function(source, name):
    declaration = re.search(r'^static [^\n]+ ' + re.escape(name) + r'\([^;]+?\) \{', source, re.M)
    if not declaration:
        raise ValueError(f'Pinned function not found: {name}')
    start = declaration.start()
    position = source.index('{', start) + 1
    depth = 1
    while depth and position < len(source):
        depth += (source[position] == '{') - (source[position] == '}')
        position += 1
    if depth:
        raise ValueError(f'Incomplete pinned function: {name}')
    return source[start:position]


def verify(source_path, project, compiler):
    payload = source_path.read_bytes()
    if hashlib.sha256(payload).hexdigest() != SOURCE_SHA256:
        raise ValueError('Source SHA-256 differs from the pinned original save.c')
    source = payload.decode('utf-8')
    project = project.resolve()
    implementation = project / 'game/core/src/save_selection.cpp'
    include = project / 'game/core/include'
    with tempfile.TemporaryDirectory(prefix='hg-save-selection-reference-') as directory:
        private = Path(directory).resolve()
        if private.is_relative_to(project) or any((parent / '.git').exists() for parent in (private, *private.parents)):
            raise ValueError('Set the system temporary directory outside all Git repositories')
        harness = private / 'comparison.cpp'
        executable = private / 'comparison.exe'
        harness.write_text(HOST_FIXTURES + '\n'.join(extract_function(source, name) for name in FUNCTIONS) + COMPARISON)
        subprocess.run([
            compiler, '-std=c++20', '-Wall', '-Wextra', '-Wno-unused-parameter',
            '-Wno-unknown-pragmas', '-Wno-sign-compare', '-I', str(include),
            str(implementation), str(harness), '-o', str(executable),
        ], check=True, timeout=120)
        subprocess.run([str(executable)], check=True, timeout=30)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', required=True, type=Path, help='Local pinned reference src/save.c')
    parser.add_argument('--compiler', default='g++', help='Host C++20 GCC/Clang executable')
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    try:
        verify(args.source, args.project, args.compiler)
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
