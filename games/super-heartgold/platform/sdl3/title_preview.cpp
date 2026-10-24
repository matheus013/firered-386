#include "title_preview.hpp"
#include "hg/resource_store.hpp"
#include <stdexcept>
#include <utility>

namespace hg::sdl {
RgbaImage load_title_layers(const std::filesystem::path& private_root) {
    auto file=ResourceStore(private_root).open("a/0/4/6");
    if (file.size()>16*1024*1024) throw std::runtime_error("Title archive exceeds diagnostic limit");
    std::vector<std::byte> archive(static_cast<std::size_t>(file.size()));
    file.read(0,archive);
    RgbaImage canvas{256,192,std::vector<std::uint8_t>(256*192*4,0)};
    for (std::size_t p=3;p<canvas.pixels.size();p+=4) canvas.pixels[p]=255;
    const auto palette=narc_member(archive,2);
    for (const auto& pair : {std::pair{36u,37u}, std::pair{1u,0u}, std::pair{15u,17u}}) {
        const auto layer=decode_text_background(narc_member(archive,pair.first),palette,narc_member(archive,pair.second));
        composite_layer(canvas,layer);
    }
    return canvas;
}
}
