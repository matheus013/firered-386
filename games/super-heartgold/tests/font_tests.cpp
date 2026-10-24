#include "test.hpp"
#include "hg/nitro_font.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

namespace {
using Bytes = std::vector<std::byte>;
void put(Bytes& data, std::size_t at, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i) data.at(at+i) = std::byte((value >> (8*i)) & 255);
}
Bytes fixture(unsigned wide = 2, unsigned high = 2) {
    const auto glyph_size = wide*high*16;
    Bytes result(16+glyph_size*2+2);
    put(result, 0, 16); put(result, 4, 16+glyph_size*2); put(result, 8, 2);
    result[12] = std::byte(wide*8); result[13] = std::byte(high*8);
    result[14] = std::byte(wide); result[15] = std::byte(high);
    for (unsigned tile = 0; tile < wide*high; ++tile)
        std::fill_n(result.begin()+16+tile*16, 16, std::byte(tile*0x55));
    // First glyph: tile quadrants roles 0,1,2,3. Second glyph solid foreground.
    std::fill_n(result.begin()+16+glyph_size, glyph_size, std::byte{0x55});
    result[result.size()-2] = std::byte(wide*8);
    result.back() = std::byte{3};
    return result;
}
template<class F> bool rejects(F&& fn) {
    try { fn(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("font: all tile shapes preserve tile order and variable crop") {
    for (unsigned wide = 1; wide <= 2; ++wide) for (unsigned high = 1; high <= 2; ++high) {
        const hg::NitroFont font(fixture(wide, high));
        CHECK_EQ(font.glyph_count(), 2u);
        const auto first = font.glyph(1), second = font.glyph(2);
        CHECK_EQ(first.width, wide*8); CHECK_EQ(first.height, high*8);
        for (unsigned y = 0; y < first.height; ++y) for (unsigned x = 0; x < first.width; ++x)
            CHECK_EQ(first.roles[y*first.width+x], (y/8)*wide+x/8);
        CHECK_EQ(second.width, 3u);
        CHECK(std::all_of(second.roles.begin(), second.roles.end(), [](auto role) { return role == 1; }));
    }
}

TEST_CASE("font: row bit order matches original high byte first MSB pairs") {
    auto bytes = fixture(1, 1);
    bytes[16] = std::byte{0xe4}; bytes[17] = std::byte{0x1b};
    const auto glyph = hg::NitroFont(bytes).glyph(1);
    const std::array<unsigned,8> expected{0,1,2,3,3,2,1,0};
    for (unsigned x = 0; x < 8; ++x) CHECK_EQ(glyph.roles[x], expected[x]);
}

TEST_CASE("font: fixed width works without variable table and owns source bytes") {
    auto bytes = fixture(1,1);
    put(bytes,4,0); bytes.resize(bytes.size()-2);
    const hg::NitroFont font(bytes, true);
    bytes.assign(bytes.size(), std::byte{0});
    CHECK_EQ(font.glyph(2).width, 8u);
    CHECK_EQ(font.glyph(2).roles[0], 1u);
    CHECK(rejects([&] { hg::NitroFont invalid(bytes); }));
    CHECK(rejects([&] { font.glyph(0); }));
    CHECK(rejects([&] { font.glyph(3); }));
}

TEST_CASE("font: truncation invalid widths shapes counts and offsets fail safely") {
    const auto original = fixture();
    for (std::size_t size = 0; size < original.size(); ++size)
        CHECK(rejects([&] { hg::NitroFont font{std::span(original).first(size)}; }));
    for (const auto field : {0u, 4u, 8u}) {
        auto bytes = original; put(bytes, field, 0xffffffff);
        CHECK(rejects([&] { hg::NitroFont font(bytes); }));
    }
    for (const auto field : {13u, 14u, 15u}) {
        auto bytes = original; bytes[field] = std::byte{0};
        CHECK(rejects([&] { hg::NitroFont font(bytes); }));
    }
    auto bytes = original; bytes.back() = std::byte{17};
    CHECK(rejects([&] { hg::NitroFont font(bytes); }));
}

TEST_CASE("font: missing IDs use original replacement only when present") {
    Bytes bytes(16+428*16+428, std::byte{0});
    put(bytes,0,16); put(bytes,4,16+428*16); put(bytes,8,428);
    bytes[12] = std::byte{8}; bytes[13] = std::byte{8};
    bytes[14] = std::byte{1}; bytes[15] = std::byte{1};
    std::fill(bytes.begin()+16+428*16,bytes.end(),std::byte{8});
    std::fill_n(bytes.begin()+16+427*16,16,std::byte{0xaa});
    const hg::NitroFont font(bytes);
    CHECK(font.glyph(65535).roles == font.glyph(428).roles);
    CHECK_EQ(font.glyph(65535).roles.front(), 2u);
}
