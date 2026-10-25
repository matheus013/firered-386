#include "preview_texture.hpp"
#include <stdexcept>

namespace hg::sdl {
PreviewTexture make_preview_texture(SDL_Renderer* renderer, const RgbaImage& image) {
    if (image.width == 0 || image.width > 4096 || image.height == 0 || image.height > 4096 ||
        image.pixels.size() != static_cast<std::size_t>(image.width)*image.height*4)
        throw std::runtime_error("Invalid preview image");
    PreviewTexture texture(SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,
        static_cast<int>(image.width),static_cast<int>(image.height)));
    if (!texture || !SDL_UpdateTexture(texture.get(),nullptr,image.pixels.data(),static_cast<int>(image.width*4)) ||
        !SDL_SetTextureScaleMode(texture.get(),SDL_SCALEMODE_NEAREST) ||
        !SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_NONE))
        throw std::runtime_error(SDL_GetError());
    return texture;
}
}
