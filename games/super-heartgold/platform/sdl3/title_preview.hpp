#pragma once
#include "hg/nitro_graphics.hpp"
#include <filesystem>
namespace hg::sdl {
// Diagnostic composition from the pinned SoulSilver title resource layout.
// This does not run the original title controller or any 3D model.
RgbaImage load_title_layers(const std::filesystem::path& private_root);
}
