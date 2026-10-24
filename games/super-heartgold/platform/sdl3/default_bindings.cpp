#include "default_bindings.hpp"

#include <SDL3/SDL.h>

namespace hg::sdl {

InputBindings default_keyboard_bindings()
{
    InputBindings b;
    b.bind(SDLK_UP, Action::Up);
    b.bind(SDLK_DOWN, Action::Down);
    b.bind(SDLK_LEFT, Action::Left);
    b.bind(SDLK_RIGHT, Action::Right);
    b.bind(SDLK_X, Action::A);
    b.bind(SDLK_Z, Action::B);
    b.bind(SDLK_S, Action::X);
    b.bind(SDLK_A, Action::Y);
    b.bind(SDLK_Q, Action::L);
    b.bind(SDLK_W, Action::R);
    b.bind(SDLK_RETURN, Action::Start);
    b.bind(SDLK_BACKSPACE, Action::Select);
    b.bind(SDLK_F11, Action::ToggleFullscreen);
    b.bind(SDLK_TAB, Action::ToggleSpeed);
    b.bind(SDLK_ESCAPE, Action::Quit);
    return b;
}

InputBindings default_gamepad_bindings()
{
    // Botões posicionais do SDL: "South" é o de baixo (A no Xbox, B no Switch).
    // Confirmar fica no botão de baixo, como a maioria dos controles de PC espera.
    InputBindings b;
    b.bind(SDL_GAMEPAD_BUTTON_DPAD_UP, Action::Up);
    b.bind(SDL_GAMEPAD_BUTTON_DPAD_DOWN, Action::Down);
    b.bind(SDL_GAMEPAD_BUTTON_DPAD_LEFT, Action::Left);
    b.bind(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Action::Right);
    b.bind(SDL_GAMEPAD_BUTTON_SOUTH, Action::A);
    b.bind(SDL_GAMEPAD_BUTTON_EAST, Action::B);
    b.bind(SDL_GAMEPAD_BUTTON_NORTH, Action::X);
    b.bind(SDL_GAMEPAD_BUTTON_WEST, Action::Y);
    b.bind(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, Action::L);
    b.bind(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, Action::R);
    b.bind(SDL_GAMEPAD_BUTTON_START, Action::Start);
    b.bind(SDL_GAMEPAD_BUTTON_BACK, Action::Select);
    b.bind(SDL_GAMEPAD_BUTTON_RIGHT_STICK, Action::ToggleSpeed);
    return b;
}

} // namespace hg::sdl
