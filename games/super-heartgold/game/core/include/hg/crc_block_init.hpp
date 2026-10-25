#pragma once

#include "hg/save_context.hpp"

#include <vector>

namespace hg {
struct CrcBlockUpdate {
    unsigned id;
    std::uint16_t crc;
};
struct CrcBlockInitResult {
    // Every original update is performed immediately, in order. This trace
    // contains the IDs and values actually written to the owning context.
    std::vector<CrcBlockUpdate> updates;
};

// Initializes one raw block with pinned SoulSilver USA defaults and real CRCs.
// Supports IDs 23,24,26,27,30,31. Other IDs reject before mutation. Payload/trailer
// are those of context.view(id), never a separately supplied unbound pointer.
// Mystery Gift (27) preserves its existing payload and updates only its CRC;
// zero initialization of a new game belongs to the outer SaveContext lifecycle.
// Alignment padding and the trailer's final two bytes are preserved throughout.
[[nodiscard]] CrcBlockInitResult initialize_soulsilver_usa_crc_block(
    SaveContext& context, unsigned id);
} // namespace hg
