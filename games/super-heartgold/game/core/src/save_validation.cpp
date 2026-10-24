#include "hg/save_validation.hpp"

namespace hg {
std::uint16_t save_crc16(std::span<const std::byte> bytes) noexcept {
    // Verified against the pinned game's actual GF/SDK routines (see docs).
    std::uint16_t crc = 0xffff;
    for (const auto byte : bytes) {
        crc ^= static_cast<std::uint16_t>(std::to_integer<unsigned>(byte) << 8);
        for (int bit = 0; bit < 8; ++bit)
            crc = static_cast<std::uint16_t>((crc << 1) ^ ((crc & 0x8000) ? 0x1021 : 0));
    }
    return crc;
}

PrimarySaveValidation validate_primary_save(const SaveImage& image) {
    PrimarySaveValidation result{};
    std::array<SaveBankChecks, 2> checks{};
    for (unsigned bank = 0; bank < 2; ++bank) {
        for (unsigned chunk = 0; chunk < PrimarySaveChunks.size(); ++chunk) {
            const auto spec = PrimarySaveChunks[chunk];
            const auto payload_size = spec.size - SaveImage::FooterSize;
            auto& validation = result.chunks[bank][chunk];
            validation.footer = image.footer(bank, spec.offset + payload_size);
            const auto& footer = validation.footer;
            validation.calculated_crc = save_crc16(image.bank_range(bank, spec.offset, payload_size));
            if (footer.size != spec.size) validation.error = SaveChunkError::Size;
            else if (footer.magic != 0x20060623) validation.error = SaveChunkError::Magic;
            else if (footer.slot != chunk) validation.error = SaveChunkError::Slot;
            else if (footer.crc != validation.calculated_crc) validation.error = SaveChunkError::Crc;
            auto& check = chunk == 0 ? checks[bank].normal : checks[bank].pc_storage;
            check = {validation.valid(), footer.count};
        }
    }
    result.reference_selection = select_save_bank(checks);
    // The original initial selector can choose a dummy zero count at wrap.
    // Its later load revalidates both chunks. Refuse that choice at this API.
    if (const auto bank = result.reference_selection.bank;
        bank && result.reference_selection.selected_chunks_valid &&
        checks[*bank].normal.counter == checks[*bank].pc_storage.counter)
        result.readable_primary_bank = bank;
    return result;
}
} // namespace hg
