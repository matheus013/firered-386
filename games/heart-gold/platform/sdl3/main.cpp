// Executável mínimo do Super HeartGold: abre uma janela, desenha as duas telas
// do DS numa janela só e traduz teclado/gamepad para ações do jogo.

#include "default_bindings.hpp"
#include "hg/fixed_step.hpp"
#include "hg/input.hpp"
#include "hg/log.hpp"
#include "hg/viewport.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Options {
    long max_frames = -1; // -1: roda até o usuário fechar
    const char* video_driver = nullptr;
    bool self_test = false;
    bool fullscreen = false;
    bool verbose = false;
    hg::ScreenLayout layout = hg::ScreenLayout::Stacked;
    hg::ScaleMode scale = hg::ScaleMode::Integer;
};

void print_usage()
{
    SDL_Log("Uso: SuperHeartGold [opções]\n"
            "  --frames N           encerra depois de N quadros (para testes)\n"
            "  --video-driver NOME  força um driver de vídeo do SDL (ex.: dummy, offscreen)\n"
            "  --self-test          injeta eventos de teclado e gamepad e confere o mapeamento\n"
            "  --layout stacked|side\n"
            "  --scale integer|fit\n"
            "  --fullscreen\n"
            "  --verbose");
}

bool parse_args(int argc, char** argv, Options& opt)
{
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg = argv[i];
        const bool has_value = i + 1 < argc;
        if (arg == "--frames" && has_value) {
            opt.max_frames = std::strtol(argv[++i], nullptr, 10);
        } else if (arg == "--video-driver" && has_value) {
            opt.video_driver = argv[++i];
        } else if (arg == "--layout" && has_value) {
            const std::string_view v = argv[++i];
            opt.layout = v == "side" ? hg::ScreenLayout::SideBySide : hg::ScreenLayout::Stacked;
        } else if (arg == "--scale" && has_value) {
            const std::string_view v = argv[++i];
            opt.scale = v == "fit" ? hg::ScaleMode::Fit : hg::ScaleMode::Integer;
        } else if (arg == "--self-test") {
            opt.self_test = true;
        } else if (arg == "--fullscreen") {
            opt.fullscreen = true;
        } else if (arg == "--verbose") {
            opt.verbose = true;
        } else if (arg == "--help" || arg == "-h") {
            print_usage();
            return false;
        } else if (arg.starts_with("-psn_")) {
            // Argumento que o Finder do macOS pode passar ao abrir o .app.
        } else {
            hg::log_error("Argumento desconhecido: " + std::string(arg));
            print_usage();
            return false;
        }
    }
    return true;
}

SDL_FRect to_frect(const hg::Rect& r)
{
    return {static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.w), static_cast<float>(r.h)};
}

class App {
public:
    explicit App(const Options& opt)
        : m_opt(opt)
        , m_keys(hg::sdl::default_keyboard_bindings())
        , m_buttons(hg::sdl::default_gamepad_bindings())
    {
    }

    ~App()
    {
        for (SDL_Gamepad* pad : m_gamepads) {
            SDL_CloseGamepad(pad);
        }
        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool init()
    {
        if (m_opt.video_driver) {
            SDL_SetHint(SDL_HINT_VIDEO_DRIVER, m_opt.video_driver);
        }
        SDL_SetAppMetadata("Super HeartGold", "0.1.0", "io.github.superheartgold");

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            hg::log_error(std::string("SDL_Init falhou: ") + SDL_GetError());
            return false;
        }
        const int v = SDL_GetVersion();
        hg::log_info("SDL " + std::to_string(SDL_VERSIONNUM_MAJOR(v)) + "." + std::to_string(SDL_VERSIONNUM_MINOR(v)) +
                     "." + std::to_string(SDL_VERSIONNUM_MICRO(v)) + ", driver de vídeo: " +
                     SDL_GetCurrentVideoDriver());

        const bool stacked = m_opt.layout == hg::ScreenLayout::Stacked;
        const int w = (stacked ? hg::kDsScreenWidth : hg::kDsScreenWidth * 2) * 2;
        const int h = (stacked ? hg::kDsScreenHeight * 2 : hg::kDsScreenHeight) * 2;
        SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        if (m_opt.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

        m_window = SDL_CreateWindow("Super HeartGold", w, h, flags);
        if (!m_window) {
            hg::log_error(std::string("SDL_CreateWindow falhou: ") + SDL_GetError());
            return false;
        }
        m_renderer = SDL_CreateRenderer(m_window, nullptr);
        if (!m_renderer) {
            hg::log_error(std::string("SDL_CreateRenderer falhou: ") + SDL_GetError());
            return false;
        }
        m_vsync = SDL_SetRenderVSync(m_renderer, 1);
        hg::log_info(std::string("Janela ") + std::to_string(w) + "x" + std::to_string(h) +
                     ", renderer: " + SDL_GetRendererName(m_renderer) + (m_vsync ? ", vsync" : ", sem vsync"));

        if (m_opt.self_test) {
            push_self_test_events();
        }
        return true;
    }

    int run()
    {
        long frames = 0;
        Uint64 last = SDL_GetTicksNS();
        while (m_running) {
            const Uint64 frame_start = SDL_GetTicksNS();
            m_input.begin_frame();
            poll_events();
            handle_meta_actions();

            const int steps = m_clock.advance(static_cast<std::int64_t>(frame_start - last));
            last = frame_start;
            m_logic_steps += steps; // a lógica do jogo entrará aqui (ver docs/roadmap.md)

            render();
            ++frames;
            if (m_opt.max_frames >= 0 && frames >= m_opt.max_frames) {
                m_running = false;
            }

            // Sem vsync (ex.: driver dummy), limita o laço ao ritmo do DS.
            const Uint64 spent = SDL_GetTicksNS() - frame_start;
            const auto step = static_cast<Uint64>(m_clock.step_ns());
            if (!m_vsync && spent < step) {
                SDL_DelayNS(step - spent);
            }
        }
        hg::log_info("Encerrando após " + std::to_string(frames) + " quadros e " + std::to_string(m_logic_steps) +
                     " passos de lógica");

        if (m_opt.self_test) {
            return check_self_test(frames) ? EXIT_SUCCESS : EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

private:
    void push_self_test_events()
    {
        SDL_Event key{};
        key.type = SDL_EVENT_KEY_DOWN;
        key.key.key = SDLK_X;
        key.key.down = true;
        SDL_PushEvent(&key);
        key.type = SDL_EVENT_KEY_UP;
        key.key.down = false;
        SDL_PushEvent(&key);

        SDL_Event pad{};
        pad.type = SDL_EVENT_GAMEPAD_BUTTON_DOWN;
        pad.gbutton.button = SDL_GAMEPAD_BUTTON_EAST;
        pad.gbutton.down = true;
        SDL_PushEvent(&pad);
        pad.type = SDL_EVENT_GAMEPAD_BUTTON_UP;
        pad.gbutton.down = false;
        SDL_PushEvent(&pad);
        hg::log_info("Autoteste: eventos sintéticos de teclado (X) e gamepad (East) enfileirados");
    }

    bool check_self_test(long frames) const
    {
        int out_w = 0;
        int out_h = 0;
        SDL_GetRenderOutputSize(m_renderer, &out_w, &out_h);
        const bool ok = m_self_test_key_a && m_self_test_pad_b && frames > 0 && out_w > 0 && out_h > 0;
        hg::log(ok ? hg::LogLevel::Info : hg::LogLevel::Error,
                std::string("Autoteste ") + (ok ? "OK" : "FALHOU") + ": teclado->A=" +
                    (m_self_test_key_a ? "sim" : "não") + ", gamepad->B=" + (m_self_test_pad_b ? "sim" : "não") +
                    ", saída " + std::to_string(out_w) + "x" + std::to_string(out_h));
        return ok;
    }

    void apply(std::optional<hg::Action> action, bool down, const char* source, const std::string& name)
    {
        if (!action) {
            hg::log_debug(std::string(source) + " sem associação: " + name);
            return;
        }
        if (down) {
            m_input.press(*action);
        } else {
            m_input.release(*action);
        }
        const std::string_view action_name = hg::action_name(*action);
        hg::log(down ? hg::LogLevel::Info : hg::LogLevel::Debug,
                std::string(source) + " " + name + (down ? " pressionado -> " : " solto -> ") + std::string(action_name));
    }

    void poll_events()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_EVENT_QUIT:
                hg::log_info("Pedido de saída recebido");
                m_running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP: {
                if (e.key.repeat) break;
                const auto action = m_keys.lookup(static_cast<std::int32_t>(e.key.key));
                if (m_opt.self_test && e.key.down && action == hg::Action::A) m_self_test_key_a = true;
                apply(action, e.key.down, "Tecla", SDL_GetKeyName(e.key.key));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                const auto button = static_cast<SDL_GamepadButton>(e.gbutton.button);
                const auto action = m_buttons.lookup(button);
                if (m_opt.self_test && e.gbutton.down && action == hg::Action::B) m_self_test_pad_b = true;
                const char* name = SDL_GetGamepadStringForButton(button);
                apply(action, e.gbutton.down, "Botão", name ? name : "?");
                break;
            }
            case SDL_EVENT_GAMEPAD_ADDED:
                if (SDL_Gamepad* pad = SDL_OpenGamepad(e.gdevice.which)) {
                    m_gamepads.push_back(pad);
                    const char* name = SDL_GetGamepadName(pad);
                    hg::log_info(std::string("Gamepad conectado: ") + (name ? name : "sem nome"));
                } else {
                    hg::log_warn(std::string("Falha ao abrir gamepad: ") + SDL_GetError());
                }
                break;
            case SDL_EVENT_GAMEPAD_REMOVED:
                if (SDL_Gamepad* pad = SDL_GetGamepadFromID(e.gdevice.which)) {
                    std::erase(m_gamepads, pad);
                    SDL_CloseGamepad(pad);
                }
                hg::log_info("Gamepad desconectado");
                break;
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                hg::log_debug("Janela redimensionada para " + std::to_string(e.window.data1) + "x" +
                              std::to_string(e.window.data2) + " pixels");
                break;
            default:
                break;
            }
        }
    }

    void handle_meta_actions()
    {
        if (m_input.was_pressed(hg::Action::Quit)) {
            m_running = false;
        }
        if (m_input.was_pressed(hg::Action::ToggleFullscreen)) {
            const bool fullscreen = (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
            SDL_SetWindowFullscreen(m_window, !fullscreen);
            hg::log_info(fullscreen ? "Modo janela" : "Tela cheia");
        }
        if (m_input.was_pressed(hg::Action::ToggleSpeed)) {
            const int next = m_clock.speed() >= 4 ? 1 : m_clock.speed() * 2;
            m_clock.set_speed(next);
            hg::log_info("Velocidade: " + std::to_string(next) + "x");
        }
    }

    void render()
    {
        int w = 0;
        int h = 0;
        SDL_GetRenderOutputSize(m_renderer, &w, &h);
        const auto placement = hg::place_screens({w, h}, m_opt.layout, m_opt.scale);

        SDL_SetRenderDrawColor(m_renderer, 12, 12, 16, 255);
        SDL_RenderClear(m_renderer);

        const SDL_FRect top = to_frect(placement.top);
        const SDL_FRect bottom = to_frect(placement.bottom);
        SDL_SetRenderDrawColor(m_renderer, 168, 32, 40, 255);
        SDL_RenderFillRect(m_renderer, &top);
        SDL_SetRenderDrawColor(m_renderer, 48, 48, 60, 255);
        SDL_RenderFillRect(m_renderer, &bottom);

        // Uma célula por ação, acesa enquanto a ação estiver pressionada.
        const float cell = 12.0f * placement.scale;
        for (std::size_t i = 0; i < hg::kActionCount; ++i) {
            const bool down = m_input.is_down(static_cast<hg::Action>(i));
            SDL_SetRenderDrawColor(m_renderer, down ? 250 : 90, down ? 220 : 90, down ? 80 : 100, 255);
            const SDL_FRect r{bottom.x + cell * (0.5f + 1.5f * static_cast<float>(i % 10)),
                              bottom.y + cell * (0.5f + 1.5f * static_cast<float>(i / 10)), cell, cell};
            SDL_RenderFillRect(m_renderer, &r);
        }
        SDL_RenderPresent(m_renderer);
    }

    Options m_opt;
    hg::InputBindings m_keys;
    hg::InputBindings m_buttons;
    hg::InputState m_input;
    hg::FixedStep m_clock;
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    std::vector<SDL_Gamepad*> m_gamepads;
    bool m_running = true;
    bool m_vsync = false;
    long m_logic_steps = 0;
    bool m_self_test_key_a = false;
    bool m_self_test_pad_b = false;
};

} // namespace

int main(int argc, char** argv)
{
    Options opt;
    if (!parse_args(argc, argv, opt)) {
        return EXIT_FAILURE;
    }
    hg::set_log_level(opt.verbose ? hg::LogLevel::Debug : hg::LogLevel::Info);
    hg::log_info("Super HeartGold 0.1.0 iniciando");

    App app(opt);
    if (!app.init()) {
        return EXIT_FAILURE;
    }
    return app.run();
}
