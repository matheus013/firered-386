#pragma once
#include "hg/nitro_graphics.hpp"
#include <SDL3/SDL.h>
#include <memory>

namespace hg::sdl {
struct DestroyTexture {
    void operator()(SDL_Texture* texture) const { SDL_DestroyTexture(texture); }
};
using PreviewTexture = std::unique_ptr<SDL_Texture, DestroyTexture>;
PreviewTexture make_preview_texture(SDL_Renderer* renderer, const RgbaImage& image);
}
