#pragma once
#include "hg/nitro_graphics.hpp"
#include <filesystem>

namespace hg::sdl {
// Static diagnostic of a plain message: original glyphs and LF only.
// Commands/placeholders, paging and the original text-printer task are pending.
RgbaImage load_message_preview(const std::filesystem::path& root,
                              unsigned bank, unsigned message, unsigned font_member = 0);
// Clear-page marker 0x25BC splits pages; scroll commands remain unsupported.
std::vector<RgbaImage> load_message_pages(const std::filesystem::path& root,
                              unsigned bank, unsigned message, unsigned font_member = 0);
}
