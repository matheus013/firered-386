#pragma once

#include "hg/input.hpp"

namespace hg::sdl {

// Associações padrão. Serão substituídas pelo arquivo de configuração do jogador (ver docs/roadmap.md).
InputBindings default_keyboard_bindings();
InputBindings default_gamepad_bindings();

} // namespace hg::sdl
