#include "hg/save_image.hpp"
#include "hg/save_validation.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>

namespace {
const char* status_name(hg::SaveSelectionStatus status) noexcept {
    switch (status) {
    case hg::SaveSelectionStatus::NoSave: return "no_save";
    case hg::SaveSelectionStatus::Corrupt: return "corrupt";
    case hg::SaveSelectionStatus::Recovered: return "recovered";
    case hg::SaveSelectionStatus::Good: return "good";
    }
    return "unknown";
}

const char* error_name(hg::SaveChunkError error) noexcept {
    switch (error) {
    case hg::SaveChunkError::None: return "none";
    case hg::SaveChunkError::Size: return "size";
    case hg::SaveChunkError::Magic: return "magic";
    case hg::SaveChunkError::Slot: return "slot";
    case hg::SaveChunkError::Crc: return "crc";
    }
    return "unknown";
}

void write_bank(std::optional<unsigned> bank) {
    if (bank) std::cout << *bank;
    else std::cout << "null";
}

void write_result(const hg::PrimarySaveValidation& result) {
    std::cout << "{\"scope\":\"primary_chunks_only\",\"reference_status\":\""
              << status_name(result.reference_selection.status) << "\",\"reference_bank\":";
    write_bank(result.reference_selection.bank);
    std::cout << ",\"readable_primary_bank\":";
    write_bank(result.readable_primary_bank);
    std::cout << ",\"chunks\":[";
    for (unsigned bank = 0; bank < 2; ++bank) {
        if (bank) std::cout << ',';
        std::cout << '[';
        for (unsigned chunk = 0; chunk < 2; ++chunk) {
            if (chunk) std::cout << ',';
            const auto& check = result.chunks[bank][chunk];
            std::cout << "{\"error\":\"" << error_name(check.error)
                      << "\",\"footer\":{\"count\":" << check.footer.count
                      << "},\"calculated_crc\":" << check.calculated_crc << '}';
        }
        std::cout << ']';
    }
    std::cout << "]}\n";
}
}

namespace {
int inspect(const std::filesystem::path& path) {
    try {
        const auto image = hg::SaveImage::load(path);
        const auto result = hg::validate_primary_save(image);
        write_result(result);
        return result.readable_primary_bank ? 0 : 2;
    } catch (const std::exception&) {
        // Keep paths, raw data and platform-specific exception details private.
        std::cerr << "Unable to inspect raw 512 KiB save image.\n";
        return 1;
    }
}
int usage() {
    std::cerr << "Usage: hg_save_inspect <raw-512KiB-save>\n";
    return 1;
}
}

#ifdef _WIN32
// Preserve the UTF-16 path provided by Windows, independent of the user's ACP.
// MinGW builds require -municode; MSVC selects the wide entry point directly.
int wmain(int argc, wchar_t** argv) {
    return argc == 2 ? inspect(std::filesystem::path(argv[1])) : usage();
}
#else
int main(int argc, char** argv) {
    return argc == 2 ? inspect(std::filesystem::path(argv[1])) : usage();
}
#endif
