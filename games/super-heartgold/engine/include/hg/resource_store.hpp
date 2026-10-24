#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <span>
#include <string_view>

namespace hg {

// Host resources use ordinary files. No ROM, DS addresses or CPU state required.
class ResourceFile {
public:
    explicit ResourceFile(const std::filesystem::path& path);
    std::uint64_t size() const noexcept { return size_; }
    void read(std::uint64_t offset, std::span<std::byte> destination);
private:
    std::ifstream stream_;
    std::uint64_t size_{};
};

class ResourceStore {
public:
    explicit ResourceStore(const std::filesystem::path& root);
    ResourceFile open(std::string_view relative_path) const;
private:
    std::filesystem::path root_;
};

} // namespace hg
