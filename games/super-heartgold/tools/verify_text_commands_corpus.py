"""Compile production C++ and roundtrip every message/token in a private NARC.

Only aggregate metadata is printed. No text, glyph IDs, argument values, decoded
assets or private inputs are persisted. Requires a GNU-compatible native C++20
compiler. The input is limited to 16 MiB and the subprocess to 120 seconds.
"""

import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile


MAX_ARCHIVE_BYTES = 16 * 1024 * 1024

HARNESS = r'''
#include "hg/message_data.hpp"
#include "hg/text_commands.hpp"
#include "hg/nitro_graphics.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

int main(int argc, char** argv) {
    try {
#ifdef _WIN32
        if (_setmode(_fileno(stdin), _O_BINARY) == -1)
            throw std::runtime_error("Cannot set binary corpus input");
#endif
        if (argc != 2) throw std::runtime_error("Expected validated bank count");
        const auto banks = std::stoul(argv[1]);
        if (banks == 0 || banks > 65535) throw std::runtime_error("Invalid bank count");
        constexpr std::size_t max_archive = 16 * 1024 * 1024;
        constexpr std::size_t max_total_units = 16 * 1024 * 1024;
        constexpr std::size_t max_total_messages = 1024 * 1024;
        std::vector<std::byte> archive(max_archive + 1);
        const auto length = std::fread(archive.data(), 1, archive.size(), stdin);
        if (std::ferror(stdin) || length > max_archive)
            throw std::runtime_error("Archive input exceeds limit or cannot be read");
        archive.resize(length);
        // Production NARC parser validates the entire outer container and the
        // allocation entry/range for every requested member.
        (void)hg::narc_member(archive, static_cast<unsigned>(banks - 1));
        std::size_t messages = 0, units = 0, tokens = 0, packed = 0, arguments = 0;
        std::array<std::size_t, 7> kinds{};
        for (unsigned bank = 0; bank < banks; ++bank) {
            const auto member = hg::narc_member(archive, bank);
            const auto count = hg::message_count(member);
            if (count > max_total_messages - messages)
                throw std::runtime_error("Corpus message limit exceeded");
            for (std::size_t message = 0; message < count; ++message) {
                const auto decoded = hg::decode_message_data(member, message);
                if (decoded.code_units.size() > max_total_units - units)
                    throw std::runtime_error("Corpus code unit limit exceeded");
                const auto parsed = hg::tokenize_text_commands(decoded.code_units);
                std::vector<std::uint16_t> reconstructed;
                reconstructed.reserve(decoded.code_units.size());
                for (const auto& token : parsed) {
                    if (token.unit_offset != reconstructed.size())
                        throw std::runtime_error("Token offset roundtrip mismatch");
                    reconstructed.push_back(token.code);
                    if (token.kind == hg::TextTokenKind::ExtendedControl) {
                        reconstructed.push_back(token.command);
                        reconstructed.push_back(static_cast<std::uint16_t>(token.arguments.size()));
                    }
                    reconstructed.insert(reconstructed.end(), token.arguments.begin(), token.arguments.end());
                    ++kinds.at(static_cast<std::size_t>(token.kind));
                    arguments += token.arguments.size();
                }
                if (reconstructed != decoded.code_units)
                    throw std::runtime_error("Token code unit roundtrip mismatch");
                ++messages;
                units += decoded.code_units.size();
                tokens += parsed.size();
                packed += decoded.packed_trainer_name;
            }
        }
        std::cout << "{\"banks\":" << banks << ",\"messages\":" << messages
                  << ",\"decoded_code_units\":" << units << ",\"tokens\":" << tokens
                  << ",\"argument_units\":" << arguments << ",\"packed_messages\":" << packed
                  << ",\"token_kind_counts\":[";
        for (std::size_t i = 0; i < kinds.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << kinds[i];
        }
        std::cout << "],\"roundtrip\":\"passed\"}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Corpus validation failed: " << error.what() << '\n';
        return 1;
    }
}
'''


def validated_bank_count(data):
    if len(data) < 16 or data[:4] != b'NARC':
        raise ValueError('Expected a raw NARC archive')
    bom, version, size, header, blocks = struct.unpack_from('<HHIHH', data, 4)
    if (bom != 0xfffe or version not in (0x100, 0x101) or size != len(data)
            or header != 16 or not 0 < blocks < 256):
        raise ValueError('Invalid NARC header')
    cursor, count, image_found = 16, None, False
    for _ in range(blocks):
        if len(data) - cursor < 8:
            raise ValueError('Truncated NARC block header')
        signature, block_size = struct.unpack_from('<4sI', data, cursor)
        if block_size < 8 or block_size > len(data) - cursor:
            raise ValueError('Invalid NARC block range')
        if signature == b'BTAF':
            if count is not None or block_size < 12:
                raise ValueError('Duplicate or truncated NARC allocation table')
            count = struct.unpack_from('<H', data, cursor + 8)[0]
            if count == 0 or count > (block_size - 12) // 8:
                raise ValueError('Invalid NARC member count')
        elif signature == b'GMIF':
            if image_found:
                raise ValueError('Duplicate NARC image block')
            image_found = True
        cursor += block_size
    if cursor != len(data) or count is None or not image_found:
        raise ValueError('Incomplete NARC or trailing data')
    return count


def verify(archive, compiler):
    with archive.open('rb') as stream:
        data = stream.read(MAX_ARCHIVE_BYTES + 1)
    if len(data) > MAX_ARCHIVE_BYTES:
        raise ValueError('Archive exceeds 16 MiB limit')
    count = validated_bank_count(data)
    root = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='shg-text-corpus-') as directory:
        temporary = Path(directory).resolve()
        if any((folder / '.git').exists() for folder in (temporary, *temporary.parents)):
            raise ValueError('Temporary harness must stay outside Git repositories')
        source, binary = temporary / 'corpus.cpp', temporary / 'corpus.exe'
        source.write_text(HARNESS, encoding='utf-8')
        compilation = subprocess.run([
            compiler, '-std=c++20', '-O2', '-Wall', '-Wextra', '-Werror',
            '-I', str(root / 'game/core/include'), '-I', str(root / 'engine/include'),
            str(source), str(root / 'game/core/src/message_data.cpp'),
            str(root / 'game/core/src/text_commands.cpp'),
            str(root / 'engine/src/nitro_graphics.cpp'),
            str(root / 'engine/src/nitro_compression.cpp'), '-o', str(binary)],
            capture_output=True, timeout=120)
        if compilation.returncode:
            raise ValueError('Production C++ harness compilation failed')
        execution = subprocess.run([str(binary), str(count)], input=data,
                                   capture_output=True, timeout=120)
        if execution.returncode:
            raise ValueError(execution.stderr.decode('utf-8', errors='replace').strip())
        result = json.loads(execution.stdout)
    result.update(archive_bytes=len(data), archive_sha256=hashlib.sha256(data).hexdigest(),
                  scope='decoded_token_roundtrip_not_rendering_or_control_execution')
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', required=True, type=Path)
    parser.add_argument('--compiler', default='c++', help='Native GNU-compatible C++20 compiler')
    args = parser.parse_args()
    try:
        print(json.dumps(verify(args.archive, args.compiler), indent=2))
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')


if __name__ == '__main__':
    main()
