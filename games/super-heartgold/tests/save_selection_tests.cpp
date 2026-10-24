#include "hg/save_selection.hpp"

#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
using Status = hg::SaveSelectionStatus;
using Banks = std::array<hg::SaveBankChecks, 2>;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void expect(Banks banks, Status status, std::optional<unsigned> bank, bool valid = true) {
    const auto result = hg::select_save_bank(banks);
    require(result.status == status && result.bank == bank, "Unexpected save selection");
    require(result.selected_chunks_valid == (bank.has_value() && valid), "Selected validity is incorrect");
    require(result.counter == (bank ? banks[*bank].normal.counter : 0), "Selected counter is incorrect");
}

void test_statuses() {
    expect({}, Status::NoSave, {});
    expect({hg::SaveBankChecks{{true, 5}, {}}, {}}, Status::Corrupt, {});
    expect({hg::SaveBankChecks{{}, {true, 5}}, {}}, Status::Corrupt, {});
    expect({hg::SaveBankChecks{{true, 5}, {true, 5}}, {}}, Status::Good, 0);
    expect({hg::SaveBankChecks{{true, 5}, {true, 6}}, {}}, Status::Corrupt, {});
    expect({hg::SaveBankChecks{{true, 5}, {}}, hg::SaveBankChecks{{}, {true, 5}}}, Status::Corrupt, {});
    expect({hg::SaveBankChecks{{true, 6}, {true, 7}}, hg::SaveBankChecks{{true, 5}, {true, 5}}}, Status::Recovered, 1);
    expect({hg::SaveBankChecks{{true, 6}, {true, 7}}, hg::SaveBankChecks{{true, 5}, {true, 4}}}, Status::Corrupt, {});
    expect({hg::SaveBankChecks{{true, 6}, {true, 6}}, hg::SaveBankChecks{{}, {true, 5}}}, Status::Recovered, 0);
    expect({hg::SaveBankChecks{{true, 6}, {true, 6}}, hg::SaveBankChecks{{true, 5}, {}}}, Status::Good, 0);
    expect({hg::SaveBankChecks{{true, 6}, {}}, hg::SaveBankChecks{{true, 5}, {true, 5}}}, Status::Recovered, 1);
    expect({hg::SaveBankChecks{{true, 6}, {true, 6}}, hg::SaveBankChecks{{true, 6}, {true, 6}}}, Status::Good, 0);
    constexpr auto last = std::numeric_limits<std::uint32_t>::max();
    expect({hg::SaveBankChecks{{true, last}, {true, last}}, hg::SaveBankChecks{{true, 0}, {true, 0}}}, Status::Good, 1);
    // Preserve, but expose, original dummy-counter behaviour at wrap.
    expect({hg::SaveBankChecks{{true, 0}, {false, 123}}, hg::SaveBankChecks{{true, last}, {true, last}}}, Status::Good, 0, false);
}

void test_counter_order() {
    constexpr auto last = std::numeric_limits<std::uint32_t>::max();
    constexpr std::array<std::uint32_t, 7> counters{0, 1, 2, 0x7fffffff, 0x80000000, last - 1, last};
    for (const auto a : counters) for (const auto b : counters) {
        const auto actual = hg::compare_save_counters(a, b);
        const auto expected = a == last && b == 0 ? -1 : a == 0 && b == last ? 1 : a > b ? 1 : a < b ? -1 : 0;
        require(actual == expected, "Counter wrap/tie comparison differs");
        require(actual == -hg::compare_save_counters(b, a), "Counter comparison is not antisymmetric");
    }
}

void test_all_validity_masks() {
    // Every combination of primary-chunk validity, with all valid counters equal.
    for (unsigned mask = 0; mask < 16; ++mask) {
        Banks banks{};
        banks[0] = {{bool(mask & 1), 7}, {bool(mask & 2), 7}};
        banks[1] = {{bool(mask & 4), 7}, {bool(mask & 8), 7}};
        const auto result = hg::select_save_bank(banks);
        const bool normal0 = mask & 1, pc0 = mask & 2, normal1 = mask & 4, pc1 = mask & 8;
        Status expected = Status::Corrupt;
        std::optional<unsigned> selected;
        if (mask == 0) expected = Status::NoSave;
        else if (normal0 && pc0) {
            selected = 0;
            expected = !normal1 && pc1 ? Status::Recovered : Status::Good;
        } else if (normal1 && pc1) {
            selected = 1;
            expected = normal0 || pc0 ? Status::Recovered : Status::Good;
        }
        require(result.status == expected && result.bank == selected, "Validity-mask coverage differs");
        // An unvalidated counter cannot influence selection, including UINT32_MAX.
        for (auto& bank : banks) {
            if (!bank.normal.valid) bank.normal.counter = UINT32_MAX;
            if (!bank.pc_storage.valid) bank.pc_storage.counter = UINT32_MAX;
        }
        const auto poisoned = hg::select_save_bank(banks);
        require(poisoned.status == result.status && poisoned.bank == result.bank, "Invalid counters influenced selection");
    }
}
}

int main() {
    try {
        test_statuses();
        test_counter_order();
        test_all_validity_masks();
        std::cout << "Save selection tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
