#include "message_preview.hpp"
#include "hg/message_data.hpp"
#include "hg/nitro_font.hpp"
#include "hg/resource_store.hpp"

#include <array>
#include <stdexcept>

namespace hg::sdl {
namespace {
std::vector<std::byte> load_archive(const ResourceStore& store, const char* path) {
    auto file = store.open(path);
    if (file.size() > 16*1024*1024) throw std::runtime_error("Text archive exceeds diagnostic limit");
    std::vector<std::byte> bytes(static_cast<std::size_t>(file.size()));
    file.read(0, bytes);
    return bytes;
}
void pixel(RgbaImage& image, unsigned x, unsigned y, const std::array<std::uint8_t,4>& color) {
    const auto at = (y*image.width+x)*4;
    for (unsigned channel = 0; channel < 4; ++channel) image.pixels[at+channel] = color[channel];
}
}

RgbaImage load_message_preview(const std::filesystem::path& root, unsigned bank, unsigned message) {
    const ResourceStore store(root);
    const auto font_archive = load_archive(store, "a/0/1/6");
    const auto messages = load_archive(store, "a/0/2/7");
    const NitroFont font(narc_member(font_archive, 0));
    const auto decoded = decode_message_data(narc_member(messages, bank), message);
    RgbaImage image{256,192,std::vector<std::uint8_t>(256*192*4)};
    for (unsigned y = 0; y < image.height; ++y) for (unsigned x = 0; x < image.width; ++x) {
        const bool panel = x >= 8 && x < 248 && y >= 8 && y < 184;
        const bool interior = x >= 10 && x < 246 && y >= 10 && y < 182;
        pixel(image,x,y,interior ? std::array<std::uint8_t,4>{248,250,252,255} :
              panel ? std::array<std::uint8_t,4>{80,176,208,255} : std::array<std::uint8_t,4>{20,26,40,255});
    }
    constexpr std::array<std::array<std::uint8_t,4>,4> colors{{
        {0,0,0,0}, {12,16,24,255}, {128,144,160,255}, {248,250,252,255}
    }};
    unsigned x = 16, y = 20;
    for (const auto code : decoded.code_units) {
        if (code == 0xffff) break;
        if (code == 0xe000) { x = 16; y += 18; continue; }
        if (code == 0xfffe) throw std::runtime_error("Text commands/placeholders are not supported by this preview");
        const auto glyph = font.glyph(code);
        if (x + glyph.width > 240 || y + glyph.height > 176)
            throw std::runtime_error("Message exceeds static preview; paging/wrapping is not ported");
        for (unsigned gy = 0; gy < glyph.height; ++gy) for (unsigned gx = 0; gx < glyph.width; ++gx) {
            const auto role = glyph.roles[gy*glyph.width+gx];
            if (role) pixel(image,x+gx,y+gy,colors[role]);
        }
        x += glyph.width;
    }
    return image;
}
} // namespace hg::sdl
