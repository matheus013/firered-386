// Executável mínimo do Super HeartGold: abre uma janela, desenha as duas telas
// do DS numa janela só e traduz teclado/gamepad para ações do jogo.

#include "default_bindings.hpp"
#include "title_preview.hpp"
#include "message_preview.hpp"
#ifdef HG_PRIVATE_TITLE_FLOW
#include "title_runtime.hpp"
#include <memory>
#endif
#include "hg/fixed_step.hpp"
#include "hg/input.hpp"
#include "hg/log.hpp"
#include "hg/viewport.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <charconv>
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::filesystem::path path_from_utf8(const char* text) {
    std::u8string value;
    while (*text) value.push_back(static_cast<char8_t>(static_cast<unsigned char>(*text++)));
    return std::filesystem::path(value);
}

struct Options {
    long max_frames = -1; // -1: roda até o usuário fechar
    const char* video_driver = nullptr;
    bool self_test = false;
    bool fullscreen = false;
    bool verbose = false;
    const char* title_resources = nullptr;
    const char* text_resources = nullptr;
    unsigned message_bank = 219;
    unsigned message_id = 1;
    const char* capture = nullptr;
    long capture_at = 0;
    bool title_flow = false;
    bool title_flow_self_test = false;
    std::uint32_t controlled_save_flags = 0;
    hg::ScreenLayout layout = hg::ScreenLayout::Unified;
    hg::ScaleMode scale = hg::ScaleMode::Integer;
};

void print_usage()
{
    SDL_Log("Uso: SuperHeartGold [opções]\n"
            "  --frames N           encerra depois de N quadros (para testes)\n"
            "  --video-driver NOME  força um driver de vídeo do SDL (ex.: dummy, offscreen)\n"
            "  --self-test          injeta eventos de teclado e gamepad e confere o mapeamento\n"
            "  --layout unified|stacked|side  (padrão: unified)\n"
            "  --scale integer|fit\n"
            "  --fullscreen\n"
            "  --verbose");
    SDL_Log("  --title-resources PATH  teste das camadas 2D de SoulSilver (dados locais)\n"
            "  --text-resources PATH   teste estático de fonte e mensagem (dados locais)\n"
            "  --message-bank N        banco de texto (padrão: 219)\n"
            "  --message-id N          mensagem (padrão: 1)\n"
            "  --capture PATH.png      captura um quadro; recusa arquivo existente\n"
            "  --capture-at N          quadro da captura (padrão: 0)\n"
            "  --title-flow            controlador C original (build privado opcional)\n"
            "  --title-flow-self-test  passo fixo e entradas SDL automáticas\n"
            "  --checksave-flags N      flags DECIMAIS controladas; não lê saves");
}

bool parse_args(int argc, char** argv, Options& opt)
{
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg = argv[i];
        const bool has_value = i + 1 < argc;
        if (arg == "--title-resources" && has_value) {
            opt.title_resources = argv[++i];
        } else if (arg == "--text-resources" && has_value) {
            opt.text_resources = argv[++i];
        } else if ((arg == "--message-bank" || arg == "--message-id") && has_value) {
            const std::string_view value = argv[++i];
            auto& destination = arg == "--message-bank" ? opt.message_bank : opt.message_id;
            const auto result = std::from_chars(value.data(),value.data()+value.size(),destination);
            if (result.ec != std::errc{} || result.ptr != value.data()+value.size()) return false;
        } else if (arg == "--capture" && has_value) {
            opt.capture = argv[++i];
        } else if (arg == "--capture-at" && has_value) {
            const std::string_view value=argv[++i];
            const auto result=std::from_chars(value.data(),value.data()+value.size(),opt.capture_at);
            if (result.ec!=std::errc{} || result.ptr!=value.data()+value.size() || opt.capture_at<0) return false;
        } else if (arg == "--checksave-flags" && has_value) {
            const std::string_view value=argv[++i];
            const auto result=std::from_chars(value.data(),value.data()+value.size(),opt.controlled_save_flags);
            if (result.ec!=std::errc{} || result.ptr!=value.data()+value.size()) return false;
        } else if (arg == "--title-flow" || arg == "--title-flow-self-test") {
            opt.title_flow=true;
            opt.title_flow_self_test=opt.title_flow_self_test || arg=="--title-flow-self-test";
            if (opt.title_flow_self_test && opt.max_frames<0) opt.max_frames=180;
        } else if (arg == "--frames" && has_value) {
            opt.max_frames = std::strtol(argv[++i], nullptr, 10);
        } else if (arg == "--video-driver" && has_value) {
            opt.video_driver = argv[++i];
        } else if (arg == "--layout" && has_value) {
            const std::string_view v = argv[++i];
            opt.layout = v == "side"      ? hg::ScreenLayout::SideBySide
                         : v == "stacked" ? hg::ScreenLayout::Stacked
                                          : hg::ScreenLayout::Unified;
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
        if (m_preview_texture) SDL_DestroyTexture(m_preview_texture);
        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool init()
    {
        if (m_opt.text_resources && (m_opt.title_resources || m_opt.title_flow)) {
            hg::log_error("Escolha somente um teste de recursos: título ou texto");
            return false;
        }
        if (m_opt.title_flow && !m_opt.title_resources) {
            hg::log_error("--title-flow exige --title-resources");
            return false;
        }
#ifndef HG_PRIVATE_TITLE_FLOW
        if (m_opt.title_flow) {
            hg::log_error("Este build não inclui o controlador privado; configure HG_REFERENCE_TITLE_FLOW=ON fora do Git");
            return false;
        }
#endif
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

        int w = 1280; // 16:9; a vista 4:3 fica no centro e o HUD nas laterais
        int h = 720;
        if (m_opt.layout == hg::ScreenLayout::Stacked) {
            w = hg::kDsScreenWidth * 2;
            h = hg::kDsScreenHeight * 4;
        } else if (m_opt.layout == hg::ScreenLayout::SideBySide) {
            w = hg::kDsScreenWidth * 4;
            h = hg::kDsScreenHeight * 2;
        }
        SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        if (m_opt.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

        m_window = SDL_CreateWindow("Super HeartGold - native platform test", w, h, flags);
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

        if (m_opt.capture && std::filesystem::exists(path_from_utf8(m_opt.capture))) {
            hg::log_error("Captura já existe; escolha outro caminho");
            return false;
        }
        if (m_opt.title_resources || m_opt.text_resources) {
            try {
                const auto image = m_opt.text_resources ?
                    hg::sdl::load_message_preview(path_from_utf8(m_opt.text_resources),m_opt.message_bank,m_opt.message_id) :
                    hg::sdl::load_title_layers(path_from_utf8(m_opt.title_resources));
                m_preview_texture=SDL_CreateTexture(m_renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,
                    static_cast<int>(image.width),static_cast<int>(image.height));
                if (!m_preview_texture || !SDL_UpdateTexture(m_preview_texture,nullptr,image.pixels.data(),static_cast<int>(image.width*4)) ||
                    !SDL_SetTextureScaleMode(m_preview_texture,SDL_SCALEMODE_NEAREST) || !SDL_SetTextureBlendMode(m_preview_texture,SDL_BLENDMODE_NONE))
                    throw std::runtime_error(SDL_GetError());
                if (m_opt.text_resources) {
                    SDL_SetWindowTitle(m_window,"Super HeartGold - native font/message test (no game logic)");
                    hg::log_info("Fonte e mensagem decodificadas no host; sem impressor original, scripts ou CPU DS");
                } else {
                    SDL_SetWindowTitle(m_window,"Super HeartGold - native 2D resources test (no game logic / 3D)");
                    hg::log_info("Camadas 2D SoulSilver: 256x192, NCGR/NCLR/NSCR decodificados no host; sem CPU DS");
                }
            } catch (const std::exception& e) {
                hg::log_error(std::string("Recursos gráficos inválidos: ")+e.what());
                return false;
            }
        }
#ifdef HG_PRIVATE_TITLE_FLOW
        if (m_opt.title_flow) {
            m_title_runtime=std::make_unique<hg::sdl::TitleRuntime>(m_opt.controlled_save_flags);
            SDL_SetWindowTitle(m_window,"Super HeartGold - original C title flow / host 2D (3D and audio pending)");
            hg::log_info("Controlador C original e OverlayManager nativos; 3D/áudio ausentes, destinos bloqueados");
            hg::log_info("Check-save: flags CONTROLADAS="+std::to_string(m_opt.controlled_save_flags)+"; nenhum save é carregado ou validado");
        }
#endif

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
            inject_title_test_key(frames);
            poll_events();
            handle_meta_actions();

            const int steps = m_opt.title_flow_self_test ? 1 : m_clock.advance(static_cast<std::int64_t>(frame_start - last));
            last = frame_start;
            m_logic_steps += steps; // Source title decision is optional; campaign engine remains pending.
#ifdef HG_PRIVATE_TITLE_FLOW
            if (m_title_runtime) m_title_runtime->advance(m_input,steps);
#endif

            render(frames);
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

        if (m_capture_failed) return EXIT_FAILURE;
        if (m_opt.capture && !m_captured) {
            hg::log_error("Quadro solicitado para captura não foi alcançado");
            return EXIT_FAILURE;
        }
#ifdef HG_PRIVATE_TITLE_FLOW
        if (m_opt.title_flow_self_test) {
            const bool complete=m_title_runtime && m_title_runtime->menu_handoff_complete();
            hg::log_info(complete ? "Title flow self-test PASS: título e decisão check-save completos; menu e saves não executados" : "Title flow self-test FAILED");
            return complete ? EXIT_SUCCESS : EXIT_FAILURE;
        }
#endif
        if (m_opt.self_test) {
            return check_self_test(frames) ? EXIT_SUCCESS : EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

private:
    void inject_title_test_key(long frame) {
        if (!m_opt.title_flow_self_test) return;
        const bool start=frame==40 || frame==41;
        const auto acknowledgement_phase=(frame-120)%12;
        const bool acknowledge=frame>=120 && (acknowledgement_phase==0 || acknowledgement_phase==1);
        if (!start && !acknowledge) return;
        SDL_Event key{};
        const bool down=start ? frame==40 : acknowledgement_phase==0;
        key.type=down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
        key.key.key=start ? SDLK_RETURN : SDLK_X;
        key.key.down=down;
        SDL_PushEvent(&key);
    }

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

    void render(long frame)
    {
        int w = 0;
        int h = 0;
        SDL_GetRenderOutputSize(m_renderer, &w, &h);
        const auto placement = hg::place_screens({w, h}, m_opt.layout, m_opt.scale);

        SDL_SetRenderDrawColor(m_renderer, 12, 12, 16, 255);
        SDL_RenderClear(m_renderer);

        if (m_preview_texture) {
            const auto preview=hg::place_screens({w,std::max(0,h-32)},hg::ScreenLayout::Unified,m_opt.scale);
            const auto rectangle=to_frect(preview.top);
            if (rectangle.w>0 && rectangle.h>0 && !SDL_RenderTexture(m_renderer,m_preview_texture,nullptr,&rectangle)) {
                hg::log_error(SDL_GetError()); m_capture_failed=true; m_running=false;
            }
#ifdef HG_PRIVATE_TITLE_FLOW
            if (m_title_runtime) m_title_runtime->draw(m_renderer,rectangle,h);
            else
#endif
            {
                SDL_SetRenderDrawColor(m_renderer,255,255,255,255);
                SDL_RenderDebugText(m_renderer,8,static_cast<float>(std::max(0,h-24)),m_opt.text_resources ?
                    "NATIVE FONT + MESSAGE TEST - NO GAME LOGIC / TEXT PRINTER" :
                    "NATIVE 2D RESOURCE TEST - NO GAME LOGIC / 3D");
            }
            capture_frame(frame);
            SDL_RenderPresent(m_renderer);
            return;
        }

        const SDL_FRect top = to_frect(placement.top);
        SDL_SetRenderDrawColor(m_renderer, 168, 32, 40, 255);
        SDL_RenderFillRect(m_renderer, &top);

        // Conteúdo da tela de baixo: a segunda tela no modo clássico, ou os painéis
        // de HUD nas faixas laterais no modo unificado (ainda só marcados).
        SDL_FRect bottom = to_frect(placement.bottom);
        if (m_opt.layout == hg::ScreenLayout::Unified) {
            const float margin = 8.0f * placement.scale;
            SDL_SetRenderDrawColor(m_renderer, 32, 32, 42, 255);
            for (const hg::Rect& side : {placement.hud_left, placement.hud_right}) {
                if (side.w > margin * 2) {
                    const SDL_FRect panel{side.x + margin, side.y + margin, side.w - margin * 2, side.h - margin * 2};
                    SDL_RenderFillRect(m_renderer, &panel);
                }
            }
            // As células de ação ficam no painel direito (ou sobre a vista, se não houver faixa).
            const hg::Rect& right = placement.hud_right.w > margin * 2 ? placement.hud_right : placement.top;
            bottom = {right.x + margin, right.y + margin, right.w - margin * 2, right.h - margin * 2};
        } else {
            SDL_SetRenderDrawColor(m_renderer, 48, 48, 60, 255);
            SDL_RenderFillRect(m_renderer, &bottom);
        }

        // Uma célula por ação, acesa enquanto a ação estiver pressionada.
        const float cell = 12.0f * placement.scale;
        for (std::size_t i = 0; i < hg::kActionCount; ++i) {
            const bool down = m_input.is_down(static_cast<hg::Action>(i));
            SDL_SetRenderDrawColor(m_renderer, down ? 250 : 90, down ? 220 : 90, down ? 80 : 100, 255);
            const SDL_FRect r{bottom.x + cell * (0.5f + 1.5f * static_cast<float>(i % 10)),
                              bottom.y + cell * (0.5f + 1.5f * static_cast<float>(i / 10)), cell, cell};
            SDL_RenderFillRect(m_renderer, &r);
        }
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(m_renderer, top.x + 12, top.y + 12, "NATIVE PLATFORM TEST");
        SDL_RenderDebugText(m_renderer, top.x + 12, top.y + 28, "HG GAME ENGINE NOT CONNECTED");
        capture_frame(frame);
        SDL_RenderPresent(m_renderer);
    }

    void capture_frame(long frame) {
        if (!m_opt.capture || m_captured || frame!=m_opt.capture_at) return;
        m_captured=true;
        SDL_Surface* surface=SDL_RenderReadPixels(m_renderer,nullptr);
        SDL_IOStream* output = surface ? SDL_IOFromFile(m_opt.capture,"wbx") : nullptr;
        if (!surface || !output || !SDL_SavePNG_IO(surface,output,true)) {
            hg::log_error(std::string("Falha na captura: ")+SDL_GetError());
            m_capture_failed=true; m_running=false;
        } else hg::log_info(std::string("Captura do renderer: ")+m_opt.capture);
        if (surface) SDL_DestroySurface(surface);
    }

    Options m_opt;
    hg::InputBindings m_keys;
    hg::InputBindings m_buttons;
    hg::InputState m_input;
    hg::FixedStep m_clock;
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_preview_texture = nullptr;
    std::vector<SDL_Gamepad*> m_gamepads;
    bool m_running = true;
    bool m_vsync = false;
    long m_logic_steps = 0;
    bool m_self_test_key_a = false;
    bool m_self_test_pad_b = false;
    bool m_captured = false;
    bool m_capture_failed = false;
#ifdef HG_PRIVATE_TITLE_FLOW
    std::unique_ptr<hg::sdl::TitleRuntime> m_title_runtime;
#endif
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
