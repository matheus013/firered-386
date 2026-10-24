#include "hg/input.hpp"

namespace hg {

namespace {

constexpr std::array<std::string_view, kActionCount> kActionNames = {
    "Up", "Down", "Left", "Right", "A", "B", "X", "Y", "L", "R",
    "Start", "Select", "ToggleFullscreen", "ToggleSpeed", "Quit",
};

} // namespace

std::string_view action_name(Action action)
{
    const auto i = static_cast<std::size_t>(action);
    return i < kActionCount ? kActionNames[i] : "Unknown";
}

std::optional<Action> action_from_name(std::string_view name)
{
    for (std::size_t i = 0; i < kActionCount; ++i) {
        if (kActionNames[i] == name) {
            return static_cast<Action>(i);
        }
    }
    return std::nullopt;
}

std::optional<Action> InputBindings::lookup(std::int32_t code) const
{
    const auto it = m_bindings.find(code);
    if (it == m_bindings.end()) {
        return std::nullopt;
    }
    return it->second;
}

void InputState::begin_frame()
{
    m_pressed.fill(false);
    m_released.fill(false);
}

void InputState::press(Action action)
{
    const auto i = index(action);
    if (!m_down[i]) {
        m_pressed[i] = true;
    }
    m_down[i] = true;
}

void InputState::release(Action action)
{
    const auto i = index(action);
    if (m_down[i]) {
        m_released[i] = true;
    }
    m_down[i] = false;
}

bool InputState::is_down(Action action) const { return m_down[index(action)]; }
bool InputState::was_pressed(Action action) const { return m_pressed[index(action)]; }
bool InputState::was_released(Action action) const { return m_released[index(action)]; }

} // namespace hg
