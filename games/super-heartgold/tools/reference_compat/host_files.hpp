#pragma once
#include <cstddef>
#include <filesystem>

// Private probe boundary; not a public emulation API.
void hg_host_mount(const std::filesystem::path& root);
std::size_t hg_host_open_files();
std::size_t hg_host_allocations();
