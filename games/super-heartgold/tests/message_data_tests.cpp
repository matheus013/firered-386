#include "test.hpp"
#include "hg/message_data.hpp"

#include <array>
#include <stdexcept>

namespace {
using Units = std::vector<std::uint16_t>;
using Bytes = std::vector<std::byte>;
void write(Bytes& bytes, std::size_t offset, std::uint32_t value, unsigned count) {
    for (unsigned i = 0; i < count; ++i) bytes.at(offset + i) = std::byte((value >> (8 * i)) & 255);
}
Bytes bank_of(const std::vector<Units>& messages, std::uint16_t key = 0x1234) {
    Bytes result(4 + messages.size() * 8);
    write(result, 0, static_cast<std::uint32_t>(messages.size()), 2);
    write(result, 2, key, 2);
    for (std::size_t id = 0; id < messages.size(); ++id) {
        const auto seed = static_cast<std::uint32_t>((key * 765u * (id + 1)) & 65535);
        const auto mask = seed | (seed << 16);
        write(result, 4 + id * 8, static_cast<std::uint32_t>(result.size()) ^ mask, 4);
        write(result, 8 + id * 8, static_cast<std::uint32_t>(messages[id].size()) ^ mask, 4);
        for (std::size_t i = 0; i < messages[id].size(); ++i) {
            const auto cipher = messages[id][i] ^ static_cast<std::uint16_t>((id + 1) * 596947u + i * 18749u);
            result.push_back(std::byte(cipher & 255));
            result.push_back(std::byte(cipher >> 8));
        }
    }
    return result;
}
// Model the original encoder's 15-bit words, including its outer EOS padding.
Units packed(const Units& glyphs) {
    Units result{0xf100};
    std::uint32_t buffer{};
    unsigned bits{};
    for (const auto glyph : glyphs) {
        buffer |= static_cast<std::uint32_t>(glyph) << bits;
        bits += 9;
        if (bits >= 15) {
            result.push_back(static_cast<std::uint16_t>(buffer & 0x7fff));
            buffer >>= 15;
            bits -= 15;
        }
    }
    if (bits > 1) result.push_back(static_cast<std::uint16_t>((buffer | (0xffffu << bits)) & 0x7fff));
    result.push_back(0xffff);
    return result;
}
template<class F> bool rejects(F&& fn) {
    try { fn(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("messages: preserves glyph controls arguments embedded EOS and per-entry keys") {
    const std::vector<Units> inputs{{0xffff}, {0x12, 0x123, 0xe000, 0xfffe, 0x100, 2, 0, 0xffff, 0x34, 0xffff}};
    for (const auto key : {0u, 0x1234u, 0xffffu}) {
        const auto bank = bank_of(inputs, static_cast<std::uint16_t>(key));
        CHECK_EQ(hg::message_count(bank), inputs.size());
        for (std::size_t id = 0; id < inputs.size(); ++id) {
            CHECK(hg::decrypt_message_units(bank, id) == inputs[id]);
            const auto decoded = hg::decode_message_data(bank, id);
            CHECK(!decoded.packed_trainer_name);
            CHECK(decoded.code_units == inputs[id]);
        }
    }
}

TEST_CASE("messages: packed names cross every 15-bit boundary and preserve zero glyph") {
    for (std::size_t count = 0; count < 50; ++count) {
        Units glyphs;
        for (std::size_t i = 0; i < count; ++i) glyphs.push_back(static_cast<std::uint16_t>((i * 73) % 511));
        const auto encoded = packed(glyphs);
        const auto bank = bank_of({encoded});
        CHECK(hg::decrypt_message_units(bank, 0) == encoded);
        const auto decoded = hg::decode_message_data(bank, 0);
        glyphs.push_back(0xffff);
        CHECK(decoded.packed_trainer_name);
        CHECK(decoded.code_units == glyphs);
    }
}

TEST_CASE("messages: rejects truncated header table payload and missing EOS") {
    const auto bank = bank_of({{0x12, 0x34, 0xffff}});
    for (std::size_t size = 0; size < bank.size(); ++size) {
        CHECK(rejects([&] { hg::decrypt_message_units(std::span(bank).first(size), 0); }));
    }
    CHECK(rejects([&] { hg::decrypt_message_units(bank_of({{1, 2}}), 0); }));
    CHECK(rejects([&] { hg::decrypt_message_units(bank_of({{}}), 0); }));
    CHECK(rejects([&] { hg::decrypt_message_units(bank, 1); }));
    const auto empty = bank_of({});
    CHECK_EQ(hg::message_count(empty), 0u);
    CHECK(rejects([&] { hg::decode_message_data(empty, 0); }));
}

TEST_CASE("messages: rejects metadata overflow table alias and unaligned offsets") {
    const auto valid = bank_of({{1, 0xffff}}, 0);
    for (const auto offset : {0u, 4u, 11u, 13u, 0xfffffffeu, 0xffffffffu}) {
        auto bank = valid;
        write(bank, 4, offset, 4); // Zero bank key leaves table entries plain.
        CHECK(rejects([&] { hg::decode_message_data(bank, 0); }));
    }
    auto bank = valid;
    write(bank, 8, 0xffffffff, 4);
    CHECK(rejects([&] { hg::decode_message_data(bank, 0); }));
    write(bank, 0, 0xffff, 2);
    CHECK(rejects([&] { hg::message_count(bank); }));
}

TEST_CASE("messages: honors count encoded and decoded limits including packed expansion") {
    const auto bank = bank_of({{1, 2, 0xffff}});
    CHECK(rejects([&] { hg::message_count(bank, {0, 100, 100}); }));
    CHECK(rejects([&] { hg::decrypt_message_units(bank, 0, {1, 2, 100}); }));
    CHECK(rejects([&] { hg::decode_message_data(bank, 0, {1, 3, 2}); }));
    CHECK_EQ(hg::decode_message_data(bank, 0, {1, 3, 3}).code_units.size(), 3u);
    const auto name_bank = bank_of({packed({0, 1, 2, 3, 4, 5, 6, 7})});
    CHECK(rejects([&] { hg::decode_message_data(name_bank, 0, {1, 100, 8}); }));
    CHECK_EQ(hg::decode_message_data(name_bank, 0, {1, 100, 9}).code_units.size(), 9u);
    CHECK(rejects([&] { hg::decode_message_data(bank_of({{0xf100, 0xffff}}), 0, {1, 10, 0}); }));
}

TEST_CASE("messages: rejects invalid packed spare bits and preserves early EOS") {
    CHECK(rejects([&] { hg::decode_message_data(bank_of({{0xf100, 0x8000, 0xffff}}), 0); }));
    const auto malformed = bank_of({{0xf100, 0x7fff, 0x1234, 0xffff}});
    // An EOS in the first packed bits is legal; later words are ignored.
    CHECK_EQ(hg::decode_message_data(malformed, 0).code_units.size(), 1u);
    // With a malformed final ordinary EOS, decryption must fail before unpacking.
    CHECK(rejects([&] { hg::decode_message_data(bank_of({{0xf100, 0, 0}}), 0); }));
}
