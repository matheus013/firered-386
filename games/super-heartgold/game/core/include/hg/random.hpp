#pragma once

#include <cstdint>
#include <random>

namespace hg {

// Independent game RNG streams. Seeds are explicit; no shared global state,
// wall-clock seeding, DS memory layout or disk representation is implied.
class Lcrng {
public:
    explicit Lcrng(std::uint32_t seed) noexcept : state_(seed) {}
    std::uint16_t next16() noexcept;
    std::uint32_t state() const noexcept { return state_; }
    void reseed(std::uint32_t seed) noexcept { state_ = seed; }
private:
    std::uint32_t state_;
};

// std::mt19937 has the same 32-bit recurrence, seed expansion and tempering as
// the pinned HG/SS MTRandom implementation. Copying an instance snapshots its
// current stream; the standard engine's object bytes are not a save format.
class Mt19937 {
public:
    explicit Mt19937(std::uint32_t seed) : engine_(seed) {}
    std::uint32_t next32();
    void reseed(std::uint32_t seed);
private:
    std::mt19937 engine_;
};

} // namespace hg
