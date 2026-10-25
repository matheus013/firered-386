#include "hg/crc_block_init.hpp"

#include <algorithm>
#include <stdexcept>

namespace hg {
CrcBlockInitResult initialize_soulsilver_usa_crc_block(SaveContext& context, unsigned id) {
    unsigned count = 1;
    switch (id) {
    case 23: case 26: case 27: case 31: break;
    case 24: count = 157; break;
    case 30: count = 2; break;
    default: throw std::runtime_error("CRC block initializer supports only IDs 23,24,26,27,30,31");
    }
    CrcBlockInitResult result;
    result.updates.reserve(count); // Complete allocation before changing data.
    const auto update = [&] { result.updates.push_back({id, context.update_crc(id)}); };
    auto payload = context.raw_view(id);
    if (id == 27) {
        // The original function does not clear MysteryGiftSave.
        update();
        return result;
    }
    std::fill(payload.begin(), payload.end(), std::byte{0});
    if (id == 24) {
        // 26 stat groups, six records each; groupId/stat clear, eight EOS words.
        for (unsigned entry = 0; entry < 156; ++entry) {
            std::fill_n(payload.begin() + entry * 24, 8, std::byte{0});
            std::fill_n(payload.begin() + entry * 24 + 8, 16, std::byte{0xff});
            update();
        }
    } else if (id == 30) {
        // Exact pinned language/greeting map: English (2) uses flag index 1.
        payload[0] = std::byte{2};
        update(); // Save_EasyChat_SetGreetingFlag performs its own update.
    } else if (id == 31) {
        payload[0x34] = std::byte{1}; // u32 notificationsEnabled, little endian.
    }
    update();
    return result;
}
} // namespace hg
