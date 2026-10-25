#include "hg/random.hpp"

namespace hg {

std::uint16_t Lcrng::next16() noexcept {
    state_ = state_ * 1103515245u + 24691u;
    return static_cast<std::uint16_t>(state_ >> 16);
}

std::uint32_t Mt19937::next32() {
    return static_cast<std::uint32_t>(engine_());
}

void Mt19937::reseed(std::uint32_t seed) {
    engine_.seed(seed);
}

} // namespace hg
