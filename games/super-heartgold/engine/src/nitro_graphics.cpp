#include "hg/nitro_graphics.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string_view>

namespace hg {
namespace {
using Bytes = std::span<const std::byte>;
void require(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
Bytes slice(Bytes b, std::size_t start, std::size_t count) {
    require(start<=b.size() && count<=b.size()-start,"Truncated resource range");
    return b.subspan(start,count);
}
unsigned u16(Bytes b,std::size_t p) {
    const auto s=slice(b,p,2);
    return std::to_integer<unsigned>(s[0]) | (std::to_integer<unsigned>(s[1])<<8);
}
std::uint32_t u32(Bytes b,std::size_t p) {
    return u16(b,p) | (static_cast<std::uint32_t>(u16(b,p+2))<<16);
}
bool magic(Bytes b,std::size_t p,std::string_view name) {
    const auto s=slice(b,p,name.size()); return std::memcmp(s.data(),name.data(),name.size())==0;
}
Bytes block(Bytes b,std::string_view container,std::string_view wanted) {
    require(magic(b,0,container),"Unsupported resource signature (compression not supported)");
    const auto marker=container=="NARC" ? 0xfffeu : 0xfeffu;
    require(u16(b,4)==marker && (u16(b,6)==0x100 || u16(b,6)==0x101),"Unsupported resource byte order/version");
    require(u32(b,8)==b.size() && u16(b,12)==16,"Invalid resource header size");
    const auto count=u16(b,14);
    require(count>0 && count<256,"Invalid resource block count");
    Bytes found;
    std::size_t cursor=16;
    for (unsigned i=0;i<count;++i) {
        const auto size=u32(b,cursor+4);
        require(size>=8,"Invalid resource block size");
        const auto candidate=slice(b,cursor,size);
        if (magic(candidate,0,wanted)) { require(found.empty(),"Duplicate resource block"); found=candidate; }
        cursor+=size;
    }
    require(cursor==b.size() && !found.empty(),"Missing resource block or trailing bytes");
    return found;
}
std::uint8_t expand5(unsigned value) { return static_cast<std::uint8_t>((value<<3)|(value>>2)); }
}

Bytes narc_member(Bytes archive,unsigned member) {
    const auto fat=block(archive,"NARC","BTAF");
    const auto image=block(archive,"NARC","GMIF");
    const auto count=u16(fat,8);
    require(fat.size()>=12 && count<=(fat.size()-12)/8,"Invalid archive allocation table");
    require(member<count,"NARC member outside archive");
    const auto begin=u32(fat,12+8*member),end=u32(fat,16+8*member);
    require(begin<=end,"Reversed NARC member range");
    return slice(image,8+static_cast<std::size_t>(begin),end-begin);
}

RgbaImage decode_text_background(Bytes ncgr,Bytes nclr,Bytes nscr) {
    const auto chars=block(ncgr,"RGCN","RAHC");
    const auto palette=block(nclr,"RLCN","TTLP");
    const auto screen=block(nscr,"RCSN","NRCS");
    const auto format=u32(chars,12);
    require(format==3 || format==4,"Unsupported character color format");
    require(u32(chars,20)==0,"Bitmap/scanned characters are not supported");
    require(u32(palette,8)==3 || u32(palette,8)==4,"Unsupported palette format");
    require(u32(palette,12)==0,"Extended palette resources are not supported");
    require(u16(screen,12)==(format==3 ? 0u : 1u) && u16(screen,14)==0,"Unsupported screen color/affine format");
    const auto tiles=slice(chars,8+static_cast<std::size_t>(u32(chars,28)),u32(chars,24));
    const auto colors=slice(palette,8+static_cast<std::size_t>(u32(palette,20)),u32(palette,16));
    require(!colors.empty() && colors.size()%2==0 && colors.size()<=512,"Invalid standard palette size");
    const unsigned bytes_per_tile=format==3 ? 32u : 64u;
    require(!tiles.empty() && tiles.size()%bytes_per_tile==0,"Invalid character data length");
    const auto width=u16(screen,8),height=u16(screen,10);
    require(width>0 && height>0 && width<=256 && height<=256 && width%8==0 && height%8==0,"Unsupported screen dimensions");
    const auto map=slice(screen,20,u32(screen,16));
    require(map.size()==static_cast<std::size_t>(width)*height/32,"Invalid tilemap length");
    RgbaImage result{width,height,std::vector<std::uint8_t>(static_cast<std::size_t>(width)*height*4)};
    for (unsigned y=0;y<height;++y) for (unsigned x=0;x<width;++x) {
        const auto entry=u16(map,((y/8)*(width/8)+x/8)*2);
        const auto tile=entry&1023u;
        require(tile<tiles.size()/bytes_per_tile,"Tile index outside character data");
        const auto px=(entry&0x400) ? 7-x%8 : x%8;
        const auto py=(entry&0x800) ? 7-y%8 : y%8;
        const auto pixel=py*8+px;
        const auto packed=std::to_integer<unsigned>(tiles[tile*bytes_per_tile+(format==3 ? pixel/2 : pixel)]);
        const auto index=format==3 ? (packed>>((pixel%2)*4))&15u : packed;
        const auto color_index=index+(format==3 ? (entry>>12)*16 : 0);
        const auto color=u16(colors,color_index*2);
        const auto destination=(static_cast<std::size_t>(y)*width+x)*4;
        result.pixels[destination]=expand5(color&31);
        result.pixels[destination+1]=expand5((color>>5)&31);
        result.pixels[destination+2]=expand5((color>>10)&31);
        result.pixels[destination+3]=index==0 ? 0 : 255;
    }
    return result;
}

void composite_layer(RgbaImage& destination,const RgbaImage& layer) {
    require(destination.width<=4096 && destination.height<=4096 && layer.width<=4096 && layer.height<=4096,"Composition dimensions exceed limit");
    require(destination.pixels.size()==static_cast<std::size_t>(destination.width)*destination.height*4 &&
        layer.pixels.size()==static_cast<std::size_t>(layer.width)*layer.height*4,"Invalid RGBA image buffer");
    for (unsigned y=0;y<std::min(destination.height,layer.height);++y)
        for (unsigned x=0;x<std::min(destination.width,layer.width);++x) {
            const auto source=(static_cast<std::size_t>(y)*layer.width+x)*4;
            const auto target=(static_cast<std::size_t>(y)*destination.width+x)*4;
            require(layer.pixels[source+3]==0 || layer.pixels[source+3]==255,"Partial layer alpha not supported");
            if (layer.pixels[source+3]) std::copy_n(layer.pixels.begin()+source,4,destination.pixels.begin()+target);
        }
}
} // namespace hg
