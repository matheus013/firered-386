"""Compare native glyph pixels with exact original tile expansion, privately."""

import argparse
import hashlib
from pathlib import Path
import subprocess
import tempfile

from verify_messages_reference import extract


TEXT_SOURCE_SHA256 = 'bd21839cb0d414a2fbe0b9f3c3ca29e3b101375ed3bf756f8e1fb9c95a24ce65'
HOST = r'''
#include "hg/nitro_font.hpp"
#include "hg/nitro_graphics.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
static u16 sFgColor, sShadowColor, sBgColor;
static u16 sFontHalfRowLookupTable[256];
'''
HARNESS = r'''
unsigned get(std::span<const std::byte> bytes, std::size_t at, unsigned length) {
    if (at > bytes.size() || length > bytes.size() - at) throw std::runtime_error("Bounds");
    unsigned value = 0;
    for (unsigned i = 0; i < length; ++i) value |= std::to_integer<unsigned>(bytes[at+i]) << (8*i);
    return value;
}
int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Expected private font NARC");
        std::ifstream file(argv[1],std::ios::binary|std::ios::ate);
        const auto size = file.tellg();
        if (!file || size < 0 || size > 16*1024*1024) throw std::runtime_error("Archive size");
        std::vector<std::byte> archive(static_cast<std::size_t>(size));
        file.seekg(0); file.read(reinterpret_cast<char*>(archive.data()),size);
        if (!file) throw std::runtime_error("Archive read");
        GenerateFontHalfRowLookupTable(1,3,2);
        unsigned glyphs{}, pixels{};
        for (const auto member : {0u,1u,2u,3u,4u,10u}) {
            const auto resource = hg::narc_member(archive,member);
            const hg::NitroFont font(resource);
            const auto start = get(resource,0,4);
            const auto width_start = get(resource,4,4);
            const auto wide = get(resource,14,1), high = get(resource,15,1);
            for (unsigned id = 1; id <= font.glyph_count(); ++id) {
                const auto actual = font.glyph(id);
                if (actual.width != get(resource,width_start+id-1,1) || actual.height != get(resource,13,1))
                    throw std::runtime_error("Glyph metrics mismatch");
                for (unsigned tile = 0; tile < wide*high; ++tile) {
                    std::array<u16,8> packed{};
                    std::array<u16,16> expanded{};
                    const auto offset = start+(id-1)*wide*high*16+tile*16;
                    if (offset > resource.size() || 16 > resource.size()-offset) throw std::runtime_error("Tile bounds");
                    std::memcpy(packed.data(),resource.data()+offset,16);
                    DecompressGlyphTile(reinterpret_cast<const u8*>(packed.data()),reinterpret_cast<u8*>(expanded.data()));
                    for (unsigned y = 0; y < 8; ++y) for (unsigned x = 0; x < 8; ++x) {
                        const auto gx = (tile%wide)*8+x, gy = (tile/wide)*8+y;
                        if (gx >= actual.width || gy >= actual.height) continue;
                        const auto original = (expanded[y*2+x/4] >> ((x%4)*4)) & 15;
                        if (original != actual.roles[gy*actual.width+gx]) throw std::runtime_error("Glyph pixels mismatch");
                        ++pixels;
                    }
                }
                ++glyphs;
            }
        }
        std::cout << "{\"fonts\":6,\"glyphs\":" << glyphs << ",\"visible_pixels\":" << pixels
                  << ",\"all_match\":true}\n";
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', required=True, type=Path, help='Pinned local src/text.c')
    parser.add_argument('--archive', required=True, type=Path, help='Private a/0/1/6')
    parser.add_argument('--compiler', default='g++')
    args = parser.parse_args()
    source = args.source.read_bytes()
    if hashlib.sha256(source).hexdigest() != TEXT_SOURCE_SHA256:
        parser.error('Pinned text.c hash mismatch')
    code = source.decode('utf-8')
    functions = '\n'.join(extract(code, name) for name in ('GenerateFontHalfRowLookupTable', 'DecompressGlyphTile'))
    project = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='hg-font-reference-') as directory:
        private = Path(directory).resolve()
        if private.is_relative_to(project) or any((parent/'.git').exists() for parent in (private,*private.parents)):
            parser.error('System temporary directory must be outside Git repositories')
        harness, executable = private/'comparison.cpp', private/'comparison.exe'
        harness.write_text(HOST+functions+HARNESS)
        subprocess.run([args.compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror',
                        '-I',str(project/'engine/include'),str(harness),
                        *(str(project/'engine/src'/name) for name in ('nitro_font.cpp','nitro_graphics.cpp','nitro_compression.cpp')),
                        '-o',str(executable)],check=True,timeout=120)
        subprocess.run([str(executable),str(args.archive.resolve())],check=True,timeout=30)


if __name__ == '__main__':
    main()
