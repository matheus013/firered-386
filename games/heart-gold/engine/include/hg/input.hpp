#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace hg {

// Ações lógicas do jogo. Teclado e gamepad são traduzidos para estas ações;
// o código de jogo nunca vê teclas ou botões físicos.
enum class Action : std::uint8_t {
    Up,
    Down,
    Left,
    Right,
    A,
    B,
    X,
    Y,
    L,
    R,
    Start,
    Select,
    ToggleFullscreen,
    ToggleSpeed,
    Quit,
    Count
};

inline constexpr std::size_t kActionCount = static_cast<std::size_t>(Action::Count);

std::string_view action_name(Action action);
std::optional<Action> action_from_name(std::string_view name);

// Tabela de associações de um tipo de dispositivo (teclas ou botões).
// Os códigos são inteiros opacos: a camada de plataforma decide o que significam.
class InputBindings {
public:
    void bind(std::int32_t code, Action action) { m_bindings[code] = action; }
    void unbind(std::int32_t code) { m_bindings.erase(code); }
    std::optional<Action> lookup(std::int32_t code) const;
    std::size_t size() const { return m_bindings.size(); }

private:
    std::unordered_map<std::int32_t, Action> m_bindings;
};

// Estado das ações no quadro atual. Chame begin_frame() antes de processar os eventos do quadro.
class InputState {
public:
    void begin_frame();
    void press(Action action);
    void release(Action action);

    bool is_down(Action action) const;
    bool was_pressed(Action action) const;
    bool was_released(Action action) const;

private:
    static std::size_t index(Action action) { return static_cast<std::size_t>(action); }

    std::array<bool, kActionCount> m_down{};
    std::array<bool, kActionCount> m_pressed{};
    std::array<bool, kActionCount> m_released{};
};

} // namespace hg
