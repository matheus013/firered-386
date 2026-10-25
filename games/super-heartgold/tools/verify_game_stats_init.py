"""Compare native GameStats initialization with pinned ARM9 and controlled VBlank.

Private function-level oracle only. No game/save execution or output of payloads.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile

IMAGE_HASH = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
COUNTERS = (0, 1, 255, 256, 65535, 65536, 0x12345678, 0xffffffff)


def native_outputs(compiler):
    root = Path(__file__).resolve().parents[1]
    harness = r'''
#include "hg/game_stats_init.hpp"
#include <charconv>
#include <cstdio>
#include <cstring>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main(int argc, char** argv) {
#ifdef _WIN32
    if (_setmode(_fileno(stdout), _O_BINARY) == -1) return 1;
#endif
    for (int i = 1; i < argc; ++i) {
        std::uint32_t counter{};
        const auto result = std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), counter);
        if (result.ec != std::errc{} || *result.ptr) return 1;
        const auto bytes = hg::initialize_game_stats(counter);
        if (std::fwrite(bytes.data(), 1, bytes.size(), stdout) != bytes.size()) return 1;
    }
}
'''
    with tempfile.TemporaryDirectory(prefix='hg-game-stats-') as temporary:
        folder = Path(temporary)
        source, binary = folder / 'probe.cpp', folder / 'probe.exe'
        source.write_text(harness)
        subprocess.run([compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I', str(root / 'game/core/include'), str(source),
                        str(root / 'game/core/src/game_stats_init.cpp'),
                        str(root / 'game/core/src/pokemon_crypto.cpp'), '-o', str(binary)],
                       check=True, capture_output=True, timeout=60)
        return subprocess.run([str(binary), *map(str, COUNTERS)], check=True,
                              capture_output=True, timeout=30).stdout


def prove(image, native):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_MEM_WRITE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC
    base, ram, shared, vblank = 0x02000000, 0x01000000, 0x027ff000, 0x027ffc3c
    block, group, size_function, initializer = struct.unpack_from('<IIII', image, 0xf64c4 + 16 * 16)
    if ((block, group, size_function, initializer) != (16, 0, 0x0202cf1d, 0x0202cf25)
            or struct.unpack_from('<I', image, 0x2cf4c)[0] != vblank):
        raise ValueError('Pinned GameStats descriptor/VBlank reference differs')
    if len(native) != len(COUNTERS) * 448:
        raise ValueError('Unexpected native output size')
    comparisons = 0
    for counter_index, counter in enumerate(COUNTERS):
        expected = native[counter_index * 448:(counter_index + 1) * 448]
        for pattern in (0, 0xa5, 0xff, 0x5a):
            machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
            machine.mem_map(base, 0x400000)
            machine.mem_write(base, image)
            machine.mem_map(ram, 0x100000)
            machine.mem_map(shared, 4096)
            machine.mem_write(vblank, struct.pack('<I', counter))
            start = ram + 0x10010
            guards = bytes([0x3c]) * 16
            machine.mem_write(start - 16, guards + bytes([pattern]) * 448 + guards)

            def check_write(_machine, _access, address, length, _value, _user):
                if not (start <= address and address + length <= start + 448
                        or ram + 0xe0000 <= address and address + length <= ram + 0x100000):
                    raise ValueError('Original GameStats initializer wrote outside block/stack')

            machine.hook_add(UC_HOOK_MEM_WRITE, check_write)

            def call(address, argument):
                machine.reg_write(UC_ARM_REG_R0, argument)
                machine.reg_write(UC_ARM_REG_SP, ram + 0xfff00)
                machine.reg_write(UC_ARM_REG_LR, ram)
                machine.emu_start(address, ram, count=100000)
                if machine.reg_read(UC_ARM_REG_PC) != ram:
                    raise ValueError('Original GameStats function did not return within budget')
                return machine.reg_read(UC_ARM_REG_R0)

            if call(size_function, 0) != 448:
                raise ValueError('Original GameStats size differs')
            call(initializer, start)
            if bytes(machine.mem_read(start, 448)) != expected:
                raise ValueError('Original/native GameStats bytes differ')
            if (bytes(machine.mem_read(start - 16, 16)) != guards
                    or bytes(machine.mem_read(start + 448, 16)) != guards):
                raise ValueError('GameStats guard bytes changed')
            comparisons += 1
    return dict(scope='raw_GameStats_init_with_stable_explicit_VBlank_only',
                original_arm9_sha256=IMAGE_HASH, counters=len(COUNTERS),
                comparisons=comparisons, raw_size=448, result='passed',
                writes='target_block_and_synthetic_stack_only')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9', required=True, type=Path)
    parser.add_argument('--cxx', default='c++')
    args = parser.parse_args()
    try:
        with args.arm9.open('rb') as stream:
            image = stream.read(0x400001)
        if len(image) > 0x400000:
            raise ValueError('ARM9 investigation image exceeds 4 MiB limit')
        if hashlib.sha256(image).hexdigest() != IMAGE_HASH:
            raise ValueError('Pinned private ARM9 image hash differs')
        print(json.dumps(prove(image, native_outputs(args.cxx)), indent=2))
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(1, str(error) + '\n')


if __name__ == '__main__':
    main()
