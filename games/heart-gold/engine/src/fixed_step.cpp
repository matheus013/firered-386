#include "hg/fixed_step.hpp"

#include <algorithm>

namespace hg {

FixedStep::FixedStep(std::int64_t step_ns, int max_steps_per_update)
    : m_step_ns(std::max<std::int64_t>(step_ns, 1))
    , m_max_steps(std::max(max_steps_per_update, 1))
{
}

void FixedStep::set_speed(int multiplier) { m_speed = std::clamp(multiplier, 1, 16); }

int FixedStep::advance(std::int64_t elapsed_ns)
{
    if (elapsed_ns > 0) {
        m_accumulator += elapsed_ns * m_speed;
    }
    const std::int64_t limit = static_cast<std::int64_t>(m_max_steps) * m_speed;
    const std::int64_t steps = std::min(m_accumulator / m_step_ns, limit);
    m_accumulator -= steps * m_step_ns;
    // Descarta o atraso que excedeu o limite em vez de acumulá-lo.
    m_accumulator = std::min(m_accumulator, m_step_ns);
    return static_cast<int>(steps);
}

} // namespace hg
