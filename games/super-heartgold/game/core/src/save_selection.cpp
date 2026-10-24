#include "hg/save_selection.hpp"

#include <limits>

namespace hg {
namespace {

struct RankedChunks {
    unsigned count{};
    unsigned newest{};
    unsigned oldest{};
};

RankedChunks rank_chunks(const std::array<SaveChunkCheck, 2>& chunks) noexcept {
    if (chunks[0].valid && chunks[1].valid) {
        const unsigned newest = compare_save_counters(chunks[0].counter, chunks[1].counter) < 0 ? 1 : 0;
        return {2, newest, 1 - newest};
    }
    if (chunks[0].valid) return {1, 0, 0};
    if (chunks[1].valid) return {1, 1, 1};
    return {};
}

std::uint32_t checked_counter(const SaveChunkCheck& chunk) noexcept {
    return chunk.valid ? chunk.counter : 0;
}

}

int compare_save_counters(std::uint32_t first, std::uint32_t second) noexcept {
    constexpr auto last = std::numeric_limits<std::uint32_t>::max();
    if (first == last && second == 0) return -1;
    if (first == 0 && second == last) return 1;
    return first > second ? 1 : first < second ? -1 : 0;
}

SaveSelection select_save_bank(const std::array<SaveBankChecks, 2>& banks) noexcept {
    const auto normal = rank_chunks({banks[0].normal, banks[1].normal});
    const auto pc = rank_chunks({banks[0].pc_storage, banks[1].pc_storage});
    if (normal.count == 0 && pc.count == 0) return {};
    const SaveSelection corrupt{SaveSelectionStatus::Corrupt, std::nullopt, 0, false};
    if (normal.count == 0 || pc.count == 0) return corrupt;

    const auto same_counter = [&](unsigned bank) {
        return checked_counter(banks[bank].normal) == checked_counter(banks[bank].pc_storage);
    };
    const auto choose = [&](unsigned bank, SaveSelectionStatus status) {
        return SaveSelection{status, bank, banks[bank].normal.counter,
                             banks[bank].normal.valid && banks[bank].pc_storage.valid};
    };

    if (normal.count == 2 && pc.count == 2) {
        if (same_counter(normal.newest)) return choose(normal.newest, SaveSelectionStatus::Good);
        if (same_counter(normal.oldest)) return choose(normal.oldest, SaveSelectionStatus::Recovered);
        return corrupt;
    }
    if (normal.count == 1 && pc.count == 2) {
        return same_counter(normal.newest) ? choose(normal.newest, SaveSelectionStatus::Recovered) : corrupt;
    }
    if (normal.count == 2 && pc.count == 1) {
        if (same_counter(normal.newest)) return choose(normal.newest, SaveSelectionStatus::Good);
        if (same_counter(normal.oldest)) return choose(normal.oldest, SaveSelectionStatus::Recovered);
        return corrupt;
    }
    if (normal.newest == pc.newest && same_counter(normal.newest)) {
        return choose(normal.newest, SaveSelectionStatus::Good);
    }
    return corrupt;
}

}
