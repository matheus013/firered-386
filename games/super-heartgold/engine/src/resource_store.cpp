#include "hg/resource_store.hpp"

#include <limits>
#include <stdexcept>
#include <string>

namespace hg {

ResourceFile::ResourceFile(const std::filesystem::path& path)
    : stream_(path, std::ios::binary)
{
    if (!stream_) throw std::runtime_error("Cannot open resource");
    stream_.seekg(0, std::ios::end);
    const auto end = stream_.tellg();
    if (end < 0) throw std::runtime_error("Cannot determine resource size");
    size_ = static_cast<std::uint64_t>(end);
}

void ResourceFile::read(std::uint64_t offset, std::span<std::byte> destination)
{
    if (offset > size_ || destination.size() > size_ - offset)
        throw std::out_of_range("Resource read exceeds file");
    if (destination.size() > static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max()))
        throw std::out_of_range("Resource read too large");
    if (destination.empty()) return;
    stream_.clear();
    stream_.seekg(static_cast<std::streamoff>(offset));
    stream_.read(reinterpret_cast<char*>(destination.data()), static_cast<std::streamsize>(destination.size()));
    if (!stream_) throw std::runtime_error("Resource read failed");
}

ResourceStore::ResourceStore(const std::filesystem::path& root)
    : root_(std::filesystem::canonical(root))
{
    if (!std::filesystem::is_directory(root_)) throw std::invalid_argument("Resource root is not a directory");
}

ResourceFile ResourceStore::open(std::string_view name) const
{
    if (name.empty() || name.find_first_of("\\:") != std::string_view::npos || name.find('\0') != std::string_view::npos)
        throw std::invalid_argument("Invalid resource path");
    const auto relative = std::filesystem::path(std::string(name));
    if (relative.is_absolute() || relative.has_root_name()) throw std::invalid_argument("Resource must be relative");
    for (const auto& part : relative)
        if (part == ".." || part == ".") throw std::invalid_argument("Invalid resource path component");
    const auto resolved = std::filesystem::canonical(root_ / relative);
    auto candidate = resolved.begin();
    for (auto base = root_.begin(); base != root_.end(); ++base, ++candidate)
        if (candidate == resolved.end() || *base != *candidate) throw std::invalid_argument("Resource escapes root");
    if (!std::filesystem::is_regular_file(resolved)) throw std::invalid_argument("Resource is not a regular file");
    return ResourceFile(resolved);
}

} // namespace hg
