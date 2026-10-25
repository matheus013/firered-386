#include "hg/roamer_init.hpp"
#include <cstdint>

namespace hg {

InitializedRoamerBlock initialize_roamer_block(Mt19937& random) {
    InitializedRoamerBlock result{};
    for (std::size_t word = 0; word < 2; ++word) {
        const std::uint32_t value = random.next32();
        for (unsigned byte = 0; byte < 4; ++byte)
            result.bytes[word * 4 + byte] = static_cast<std::byte>((value >> (byte * 8)) & 0xff);
    }
    return result;
}

} // namespace hg
