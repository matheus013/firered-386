#include "test.hpp"
#include "hg/nitro_compression.hpp"

#include <algorithm>
#include <array>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace {
using Buffer = std::vector<std::byte>;

Buffer bytes(std::initializer_list<unsigned> values) {
    Buffer result;
    for (const auto value : values)
        result.push_back(static_cast<std::byte>(value));
    return result;
}

Buffer header(unsigned type, std::size_t size) {
    return bytes({type, static_cast<unsigned>(size & 255u),
        static_cast<unsigned>((size >> 8) & 255u),
        static_cast<unsigned>((size >> 16) & 255u)});
}

template<class F> bool rejects(F&& fn) {
    try { fn(); } catch (const std::runtime_error&) { return true; }
    return false;
}

void check_run(unsigned type, std::size_t run_length, Buffer encoded_reference) {
    auto input = header(type, run_length + 1);
    input.push_back(std::byte{0x40}); // Literal A, followed by a distance-one run.
    input.push_back(std::byte{'A'});
    input.insert(input.end(), encoded_reference.begin(), encoded_reference.end());
    const auto output = hg::decompress_nitro_lz(input);
    CHECK_EQ(output.size(), run_length + 1);
    CHECK(std::all_of(output.begin(), output.end(), [](auto value) { return value == std::byte{'A'}; }));
    for (std::size_t n = 0; n < input.size(); ++n)
        CHECK(rejects([&] { hg::decompress_nitro_lz(std::span(input).first(n)); }));
}

Buffer full_window(unsigned type, Buffer token, std::size_t length) {
    auto input = header(type, 4096 + length);
    for (unsigned group = 0; group < 512; ++group) {
        input.push_back(std::byte{0});
        for (unsigned i = 0; i < 8; ++i)
            input.push_back(static_cast<std::byte>((group * 8 + i) % 251));
    }
    input.push_back(std::byte{0x80});
    input.insert(input.end(), token.begin(), token.end());
    return input;
}
} // namespace

TEST_CASE("compression: literal groups, partial final flag group and trailing padding") {
    for (const auto type : {0x10u, 0x11u}) {
        auto input = header(type, 9);
        const auto payload = bytes({0, '1', '2', '3', '4', '5', '6', '7', '8', 0, '9'});
        input.insert(input.end(), payload.begin(), payload.end());
        const auto expected = bytes({'1', '2', '3', '4', '5', '6', '7', '8', '9'});
        CHECK(hg::decompress_nitro_lz(input) == expected);
        for (std::size_t n = 0; n < input.size(); ++n)
            CHECK(rejects([&] { hg::decompress_nitro_lz(std::span(input).first(n)); }));
        input.insert(input.end(), {std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0xff}});
        CHECK(hg::decompress_nitro_lz(input) == expected);
    }
    // Unused low flag bits must not require more input after output is complete.
    CHECK(hg::decompress_nitro_lz(bytes({0x10, 1, 0, 0, 0x7f, 'Z'})) == bytes({'Z'}));
}

TEST_CASE("compression: LZ10 all lengths allow overlapping distance-one references") {
    for (unsigned length = 3; length <= 18; ++length)
        check_run(0x10, length, bytes({(length - 3) << 4, 0}));
}

TEST_CASE("compression: every flag position and distance-two overlapping pattern") {
    for (const auto type : {0x10u, 0x11u}) {
        auto input = header(type, 20);
        input.insert(input.end(), {std::byte{0x3f}, std::byte{'A'}, std::byte{'B'}});
        for (unsigned i = 0; i < 6; ++i) {
            input.push_back(type == 0x10 ? std::byte{0} : std::byte{0x20});
            input.push_back(std::byte{1});
        }
        const auto output = hg::decompress_nitro_lz(input);
        CHECK_EQ(output.size(), 20u);
        for (std::size_t i = 0; i < output.size(); ++i)
            CHECK(output[i] == (i % 2 == 0 ? std::byte{'A'} : std::byte{'B'}));
    }
}

TEST_CASE("compression: LZ11 short and medium token boundaries") {
    for (unsigned length = 3; length <= 16; ++length)
        check_run(0x11, length, bytes({(length - 1) << 4, 0}));
    for (unsigned length = 17; length <= 272; ++length) {
        const auto code = length - 17;
        check_run(0x11, length, bytes({code >> 4, (code & 15u) << 4, 0}));
    }
}

TEST_CASE("compression: LZ11 long form including maximum encoded run") {
    for (const unsigned length : {273u, 274u, 288u, 289u, 528u, 529u, 4096u, 65808u}) {
        const auto code = length - 273;
        check_run(0x11, length, bytes({0x10u | (code >> 12),
            (code >> 4) & 255u, (code & 15u) << 4, 0}));
    }
}

TEST_CASE("compression: maximum distance in LZ10 and every LZ11 length form") {
    const std::array<std::pair<unsigned, Buffer>, 4> tokens{{
        {0x10u, bytes({0x0f, 0xff})},
        {0x11u, bytes({0x2f, 0xff})},
        {0x11u, bytes({0x00, 0x0f, 0xff})},
        {0x11u, bytes({0x10, 0x00, 0x0f, 0xff})}
    }};
    const std::array<std::size_t, 4> lengths{3, 3, 17, 273};
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const auto input = full_window(tokens[i].first, tokens[i].second, lengths[i]);
        const auto output = hg::decompress_nitro_lz(input);
        CHECK_EQ(output.size(), 4096 + lengths[i]);
        for (std::size_t n = 0; n < lengths[i]; ++n)
            CHECK(output[4096 + n] == output[n]);
    }
}

TEST_CASE("compression: reject bad type zero size and truncated headers") {
    CHECK(rejects([] { hg::decompress_nitro_lz({}); }));
    for (unsigned type = 0; type <= 255; ++type) {
        if (type != 0x10 && type != 0x11)
            CHECK(rejects([&] { hg::decompress_nitro_lz(bytes({type, 1, 0, 0, 0, 'A'})); }));
    }
    for (const auto type : {0x10u, 0x11u}) {
        const auto empty = header(type, 0);
        CHECK(rejects([&] { hg::decompress_nitro_lz(empty); }));
        const auto declared = header(type, 1);
        CHECK(rejects([&] { hg::decompress_nitro_lz(declared); }));
    }
}

TEST_CASE("compression: reject references before beginning and runs beyond declared size") {
    for (const auto type : {0x10u, 0x11u}) {
        const auto short_code = type == 0x10 ? 0u : 0x20u;
        CHECK(rejects([&] { hg::decompress_nitro_lz(bytes({type, 3, 0, 0, 0x80, short_code, 0})); }));
        CHECK(rejects([&] { hg::decompress_nitro_lz(bytes({type, 4, 0, 0, 0x40, 'A', short_code, 1})); }));
        CHECK(rejects([&] { hg::decompress_nitro_lz(bytes({type, 2, 0, 0, 0x40, 'A', short_code, 0})); }));
    }
    CHECK(rejects([] { hg::decompress_nitro_lz(bytes({0x11, 4, 0, 0, 0x40, 'A', 0, 0, 0})); }));
    CHECK(rejects([] { hg::decompress_nitro_lz(bytes({0x11, 4, 0, 0, 0x40, 'A', 0x10, 0, 0, 0})); }));
}

TEST_CASE("compression: caller output cap checked before allocation or token decoding") {
    const auto one = bytes({0x10, 1, 0, 0, 0, 'A'});
    CHECK(rejects([&] { hg::decompress_nitro_lz(one, 0); }));
    CHECK(hg::decompress_nitro_lz(one, 1) == bytes({'A'}));
    CHECK(rejects([] { hg::decompress_nitro_lz(bytes({0x11, 0xff, 0xff, 0xff}), 4096); }));
    const auto run = bytes({0x11, 18, 0, 0, 0x40, 'A', 0, 0, 0});
    CHECK(rejects([&] { hg::decompress_nitro_lz(run, 17); }));
    CHECK_EQ(hg::decompress_nitro_lz(run, 18).size(), 18u);
}
