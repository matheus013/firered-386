#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace hg {

// Metadata produced by a separate footer validator. No file IO or CRC validation
// occurs here. Invalid chunks have counter zero in the original selection code.
struct SaveChunkCheck {
    bool valid{};
    std::uint32_t counter{};
};

struct SaveBankChecks {
    SaveChunkCheck normal;
    SaveChunkCheck pc_storage;
};

enum class SaveSelectionStatus { NoSave, Corrupt, Recovered, Good };

struct SaveSelection {
    SaveSelectionStatus status{SaveSelectionStatus::NoSave};
    std::optional<unsigned> bank;
    std::uint32_t counter{};
    // An explicit safety signal for a legacy edge case: at counter wrap, the
    // original policy can choose an invalid PC chunk whose dummy counter is 0.
    bool selected_chunks_valid{};
};

// Original comparison special-cases only UINT32_MAX < 0, not arbitrary modular
// distances. Equal counters prefer bank 0. This is not a general serial order.
int compare_save_counters(std::uint32_t first, std::uint32_t second) noexcept;

// Matches the pinned HG/SS primary-chunk selection policy. The original assert
// for unequal counters in a sole same-bank pair becomes Corrupt. Good does not
// validate extra save records, decoded game state, or even a raw save file.
SaveSelection select_save_bank(const std::array<SaveBankChecks, 2>& banks) noexcept;

}
