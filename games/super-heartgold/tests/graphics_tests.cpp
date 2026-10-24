#include "test.hpp"
#include "hg/nitro_graphics.hpp"
#include <cstring>
#include <utility>

namespace {
using Buffer=std::vector<std::byte>;
void put(Buffer& b,std::size_t p,std::uint32_t n,unsigned size=4) {
    for (unsigned i=0;i<size;++i) b.at(p+i)=static_cast<std::byte>((n>>(8*i))&255);
}
Buffer container(const char* magic,const char* section,std::size_t section_size) {
    Buffer b(16+section_size);
    std::memcpy(b.data(),magic,4); put(b,4,0x0100feff); put(b,8,static_cast<std::uint32_t>(b.size()));
    put(b,12,16,2); put(b,14,1,2);
    std::memcpy(b.data()+16,section,4); put(b,20,static_cast<std::uint32_t>(section_size));
    return b;
}
struct Fixture {
    Buffer characters,palette,screen;
    explicit Fixture(bool eight=false,unsigned entry=0) {
        characters=container("RGCN","RAHC",32+(eight ? 64 : 32));
        put(characters,24,1,2); put(characters,26,1,2); put(characters,28,eight ? 4 : 3);
        put(characters,40,eight ? 64 : 32); put(characters,44,24);
        for (const auto& pair : {std::pair{0u,1u},std::pair{7u,2u},std::pair{56u,3u},std::pair{63u,4u}}) {
            const auto index=48+(eight ? pair.first : pair.first/2);
            characters[index]|=static_cast<std::byte>(pair.second<<(eight ? 0 : (pair.first%2)*4));
        }
        palette=container("RLCN","TTLP",24+512);
        put(palette,24,eight ? 4 : 3); put(palette,32,512); put(palette,36,16);
        for (unsigned bank=0;bank<16;++bank)
            for (unsigned i=1;i<=4;++i) put(palette,40+(bank*16+i)*2,i==1 ? 31 : i==2 ? 992 : i==3 ? 31744 : 32767,2);
        screen=container("RCSN","NRCS",22);
        put(screen,24,8,2); put(screen,26,8,2); put(screen,28,eight ? 1 : 0,2); put(screen,32,2); put(screen,36,entry,2);
    }
    hg::RgbaImage decode() const { return hg::decode_text_background(characters,palette,screen); }
};
template<class F> bool rejects(F&& fn) { try { fn(); return false; } catch (const std::exception&) { return true; } }
}

TEST_CASE("graphics: 4bpp nibble order RGB555 transparency and palette banks") {
    Fixture fixture(false,15u<<12);
    const auto image=fixture.decode();
    CHECK_EQ(image.width,8u); CHECK_EQ(image.height,8u);
    CHECK_EQ(image.pixels[0],255); CHECK_EQ(image.pixels[1],0); CHECK_EQ(image.pixels[3],255);
    CHECK_EQ(image.pixels[7*4+1],255); CHECK_EQ(image.pixels[56*4+2],255);
    CHECK_EQ(image.pixels[63*4],255); CHECK_EQ(image.pixels[63*4+1],255);
    CHECK_EQ(image.pixels[4+3],0);
    // Bank 15 must be used, not the identical colors initially in bank 0.
    put(fixture.palette,40+1*2,0,2);
    CHECK_EQ(fixture.decode().pixels[0],255);
}

TEST_CASE("graphics: horizontal vertical and combined tile flips") {
    const auto horizontal=Fixture(false,0x400).decode();
    CHECK_EQ(horizontal.pixels[1],255); // green from original top-right
    const auto vertical=Fixture(false,0x800).decode();
    CHECK_EQ(vertical.pixels[2],255); // blue from original bottom-left
    const auto both=Fixture(false,0xc00).decode();
    CHECK_EQ(both.pixels[0],255); CHECK_EQ(both.pixels[1],255); CHECK_EQ(both.pixels[2],255);
}

TEST_CASE("graphics: 8bpp ignores standard palette bank bits") {
    const auto image=Fixture(true,0xf000).decode();
    CHECK_EQ(image.pixels[0],255); CHECK_EQ(image.pixels[7*4+1],255);
    CHECK_EQ(image.pixels[56*4+2],255); CHECK_EQ(image.pixels[7],0);
}

TEST_CASE("graphics: reject truncated containers out of range tiles and unsupported modes") {
    Fixture fixture;
    for (std::size_t n=0;n<fixture.characters.size();++n)
        CHECK(rejects([&] { hg::decode_text_background(std::span(fixture.characters).first(n),fixture.palette,fixture.screen); }));
    for (std::size_t n=0;n<fixture.palette.size();++n)
        CHECK(rejects([&] { hg::decode_text_background(fixture.characters,std::span(fixture.palette).first(n),fixture.screen); }));
    for (std::size_t n=0;n<fixture.screen.size();++n)
        CHECK(rejects([&] { hg::decode_text_background(fixture.characters,fixture.palette,std::span(fixture.screen).first(n)); }));
    put(fixture.screen,36,1,2); CHECK(rejects([&] { fixture.decode(); }));
    put(fixture.screen,36,0,2); put(fixture.characters,36,1); CHECK(rejects([&] { fixture.decode(); }));
    put(fixture.characters,36,0); put(fixture.screen,24,512,2); CHECK(rejects([&] { fixture.decode(); }));
    put(fixture.screen,24,8,2); put(fixture.characters,44,0xffffffff); CHECK(rejects([&] { fixture.decode(); }));
    put(fixture.characters,44,24); put(fixture.palette,28,1); CHECK(rejects([&] { fixture.decode(); }));
    put(fixture.palette,28,0); put(fixture.characters,4,0,2); CHECK(rejects([&] { fixture.decode(); }));
}

TEST_CASE("graphics: alpha composition clips crop without replacing transparent pixels") {
    auto layer=Fixture().decode();
    hg::RgbaImage canvas{4,4,std::vector<std::uint8_t>(4*4*4,7)};
    hg::composite_layer(canvas,layer);
    CHECK_EQ(canvas.pixels[0],255); CHECK_EQ(canvas.pixels[3],255);
    CHECK_EQ(canvas.pixels[4],7); CHECK_EQ(canvas.pixels[7],7);
    layer.pixels[3]=127; CHECK(rejects([&] { hg::composite_layer(canvas,layer); }));
}

TEST_CASE("graphics: NARC member bounds and block structure") {
    Buffer b(64);
    std::memcpy(b.data(),"NARC",4); put(b,4,0x0100fffe); put(b,8,64); put(b,12,16,2); put(b,14,3,2);
    std::memcpy(b.data()+16,"BTAF",4); put(b,20,20); put(b,24,1,2); put(b,28,0); put(b,32,4);
    std::memcpy(b.data()+36,"BTNF",4); put(b,40,16);
    std::memcpy(b.data()+52,"GMIF",4); put(b,56,12); put(b,60,0x12345678);
    const auto member=hg::narc_member(b,0);
    CHECK_EQ(member.size(),4u); CHECK(member[0]==std::byte{0x78});
    CHECK(rejects([&] { hg::narc_member(b,1); }));
    for (std::size_t n=0;n<b.size();++n) CHECK(rejects([&] { hg::narc_member(std::span(b).first(n),0); }));
    put(b,32,5); CHECK(rejects([&] { hg::narc_member(b,0); }));
    put(b,32,0); put(b,28,1); CHECK(rejects([&] { hg::narc_member(b,0); }));
}
