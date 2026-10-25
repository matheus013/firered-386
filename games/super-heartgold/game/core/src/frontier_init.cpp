#include "hg/frontier_init.hpp"
#include <cstdint>
namespace hg {
FrontierBlock initialize_frontier_block() {
    FrontierBlock block{};
    block[0x957] = std::byte{1}; // Initial Wi-Fi rank.
    constexpr std::array<std::uint16_t, 16> greetings{
        0,0,0x459,0xffff, 1,0,0x473,0xffff,
        2,0,0x57b,0xffff, 1,4,0x453,0xffff};
    for (std::size_t i=0; i<greetings.size(); ++i) {
        block[0xabc+i*2] = std::byte(greetings[i]&255);
        block[0xabd+i*2] = std::byte(greetings[i]>>8);
    }
    return block;
}
}
