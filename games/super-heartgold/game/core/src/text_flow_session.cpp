#include "hg/text_flow_session.hpp"
#include "hg/nitro_font.hpp"
#include "hg/text_commands.hpp"
#include "text_flow_bridge.h"

#include <algorithm>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <vector>

namespace {
struct Host {
    TextFlags flags{};
    HGTextInput input{};
    std::map<unsigned, hg::FontGlyph> glyphs;
    std::map<int, u32> glyph_widths;
    const hg::FontGlyph* current_glyph{};
    GlyphInfo glyph_info{};
    std::vector<std::uint8_t> pixels;
    unsigned width{}, height{}, font_height{};
    unsigned glyphs_drawn{}, clears{}, pixels_scrolled{}, sound_requests{};
    bool arrow{};
    const char* error{};
};
thread_local Host* active_host{};
Host& host() {
    if (!active_host) std::abort();
    return *active_host;
}
class Scope {
public:
    explicit Scope(Host& value) : previous_(active_host) { active_host = &value; }
    ~Scope() { active_host = previous_; }
    Scope(const Scope&) = delete;
    Scope& operator=(const Scope&) = delete;
private:
    Host* previous_;
};
void validate_controls(const std::vector<hg::TextToken>& tokens) {
    for (const auto& token : tokens) {
        if (token.kind != hg::TextTokenKind::ExtendedControl) continue;
        const auto& args = token.arguments;
        if (token.command == 0x201 && args.size() == 1 && args[0] <= 255) continue;
        if ((token.command == 0x203 || token.command == 0x204) && args.size() == 1) continue;
        if ((token.command == 0x205 || token.command == 0x206) && args.empty()) continue;
        if ((token.command == 0x207 || token.command == 0x208) && args.empty()) continue;
        if (token.command == 0xfe06 && args.size() == 1 &&
            (args[0] == 0xfe00 || args[0] == 0xfe01)) continue;
        throw std::runtime_error("Text flow does not implement this extended command/arguments");
    }
}
} // namespace

extern "C" {
TextFlags* HGText_Flags() { return &host().flags; }
HGTextInput* HGText_Input() { return &host().input; }
void HGText_Assert(BOOL condition) {
    if (!condition) host().error = "Original text printer assertion failed";
}
int HGText_GetControlCode(const u16* units) { return units[1]; }
int HGText_ControlField(const u16* units, u32 field) {
    if (field >= units[2]) { host().error = "Text control field exceeds validated arguments"; return 0; }
    return units[3 + field];
}
const u16* HGText_SkipControlCode(const u16* units) { return units + 3 + units[2]; }
void HGText_DrawDownArrow(TextPrinter*) { host().arrow = true; }
void HGText_ClearDownArrow(TextPrinter*) { host().arrow = false; }
void HGText_LoadDownArrowTiles(Window*, u16) { /* Arrow is an observed request, not original tiles. */ }
void HGText_PlaySelectSound() { ++host().sound_requests; }
void HGText_ColorLookup(u8, u8, u8) { host().error = "Color controls are not implemented"; }
GlyphInfo* HGText_LoadGlyph(u8 font, u16 code) {
    auto& state = host();
    const auto found = state.glyphs.find(code);
    if (font != 0 || found == state.glyphs.end()) {
        state.error = "Printer requested an unvalidated glyph/font";
        state.glyph_info = {};
        state.current_glyph = nullptr;
    } else {
        state.current_glyph = &found->second;
        state.glyph_info.width = static_cast<u8>(found->second.width);
        state.glyph_info.height = static_cast<u8>(found->second.height);
    }
    return &state.glyph_info;
}
void HGText_CopyGlyph(Window*, const u8*, u16 width, u16 height, u16 x, u16 y, u16 table) {
    auto& state = host();
    const auto* glyph = state.current_glyph;
    if (!glyph || table || glyph->width != width || glyph->height != height) {
        state.error = "Unsupported glyph copy";
        return;
    }
    for (unsigned row = 0; row < height && row + y < state.height; ++row)
        for (unsigned column = 0; column < width && column + x < state.width; ++column) {
            const auto role = glyph->roles[row * width + column];
            if (role) state.pixels[(row + y) * state.width + column + x] = role == 3 ? 0 : role;
        }
    ++state.glyphs_drawn;
}
u8 HGText_FontAttribute(u8 font, int attribute) {
    if (font != 0 || attribute != 1) host().error = "Unsupported font attribute";
    return static_cast<u8>(host().font_height);
}
u32 HGText_WindowWidth(Window*) { return host().width / 8; }
void HGText_FillWindow(Window*, u8 color) {
    if (color != 15) host().error = "Unsupported fill color";
    std::fill(host().pixels.begin(), host().pixels.end(), 0);
    ++host().clears;
}
void HGText_ScrollWindow(Window*, u8 direction, u8 distance, u8 color) {
    auto& state = host();
    if (direction || color != 0xff) { state.error = "Unsupported scroll direction/color"; return; }
    const auto rows = std::min<unsigned>(distance, state.height);
    const auto offset = rows * state.width;
    // Forward copy is safe for this overlapping upward movement.
    for (std::size_t index = 0; index < state.pixels.size() - offset; ++index)
        state.pixels[index] = state.pixels[index + offset];
    std::fill(state.pixels.end() - offset, state.pixels.end(), 0);
    state.pixels_scrolled += distance;
}
void HGText_PresentWindow(Window*) { /* pixels() exposes the current authorial window. */ }
u32 HGText_GlyphWidthZeroBased(int glyph) {
    const auto found = host().glyph_widths.find(glyph);
    if (found == host().glyph_widths.end()) {
        host().error = "Width query requested an unvalidated glyph";
        return 0;
    }
    return found->second;
}
u32 HGText_WidthFirstLine(u8 font, const u16* text, u32 spacing) {
    if (font != 0 || spacing != 0) {
        host().error = "Unsupported width query font/spacing";
        return 0;
    }
    return HGText_MeasureWidth(text, spacing);
}
} // extern C

namespace hg {
struct TextFlowSession::Impl {
    Host host_state;
    Window window{};
    TextPrinter printer{};
    std::vector<std::uint16_t> units;
    Frame current;
    unsigned repeat_limit{};

    Impl(std::span<const std::byte> font_bytes, std::span<const std::uint16_t> text, Options options) {
        if (options.width < 8 || options.width > 256 || options.width % 8 ||
            options.height < 1 || options.height > 192 || options.character_delay > 127 ||
            options.line_spacing > 32 || options.font_line_height < 1 || options.font_line_height > 32)
            throw std::runtime_error("Text flow options exceed supported bounds");
        const auto tokens = tokenize_text_commands(text);
        validate_controls(tokens);
        const bool alignment = std::any_of(tokens.begin(), tokens.end(), [](const auto& token) {
            return token.kind == TextTokenKind::ExtendedControl &&
                (token.command == 0x205 || token.command == 0x206);
        });
        if (alignment && std::any_of(tokens.begin(), tokens.end(), [](const auto& token) {
                return token.kind == TextTokenKind::SkipOperand;
            }))
            throw std::runtime_error("Alignment with skipped operands is not supported");
        units.assign(text.begin(), text.end());
        NitroFont font(font_bytes);
        host_state.font_height = options.font_line_height;
        if (host_state.font_height < font.glyph(1).height ||
            host_state.font_height + options.line_spacing > 255)
            throw std::runtime_error("Text line height exceeds original counter");
        for (const auto& token : tokens) {
            if (token.kind == TextTokenKind::Glyph) {
                host_state.glyphs.try_emplace(token.code, font.glyph(token.code));
                host_state.glyph_widths.try_emplace(static_cast<int>(token.code) - 1,
                    host_state.glyphs.at(token.code).width);
            } else if (alignment && (token.kind == TextTokenKind::ClearPage || token.kind == TextTokenKind::Scroll)) {
                // The original width helper treats these flow markers as glyphs;
                // preserve its replacement-glyph measurement when available.
                host_state.glyph_widths.try_emplace(static_cast<int>(token.code) - 1, font.glyph(token.code).width);
            }
        }
        host_state.width = options.width;
        host_state.height = options.height;
        host_state.pixels.resize(options.width * options.height);
        host_state.flags.canABSpeedUpPrint = options.ab_speedup;
        host_state.flags.autoScroll = options.auto_advance;
        host_state.flags.autoScrollCanSpeedUp = options.auto_advance_speedup;
        printer.layout.currentChar.raw = units.data();
        printer.layout.window = &window;
        printer.layout.fgColor = 1;
        printer.layout.shadowColor = 2;
        // Original font 0 background is palette 15, normalized to index 0 in
        // our exposed pixel buffer. Role 0 remains transparent; role 3 writes.
        printer.layout.bgColor = 15;
        printer.layout.lineSpacing = static_cast<u16>(options.line_spacing);
        printer.textSpeedBottom = static_cast<u8>(options.character_delay);
        repeat_limit = static_cast<unsigned>(units.size() + 1);
    }

    Frame tick(Input input) {
        if (current.completed) return current;
        host_state.input.newKeys = (input.a_pressed ? 1u : 0u) | (input.b_pressed ? 2u : 0u);
        host_state.input.heldKeys = (input.a_held ? 1u : 0u) | (input.b_held ? 2u : 0u);
        Scope scope(host_state);
        printer.callbackParam = 0;
        for (unsigned count = 0; ; ++count) {
            if (count >= repeat_limit) throw std::runtime_error("Text repeat budget exceeded");
            if (printer.layout.currentChar.raw < units.data() ||
                printer.layout.currentChar.raw >= units.data() + units.size())
                throw std::runtime_error("Text cursor escaped validated stream");
            const auto result = HGText_Run(&printer);
            if (host_state.error) throw std::runtime_error(host_state.error);
            if (result == RENDER_REPEAT) continue;
            current.completed = result == RENDER_FINISH;
            break;
        }
        current.state = printer.state;
        current.x = printer.layout.currentX;
        current.y = printer.layout.currentY;
        current.delay = printer.delayCounter;
        current.scroll_remaining = printer.scrollDistance;
        current.glyphs_drawn = host_state.glyphs_drawn;
        current.clears = host_state.clears;
        current.pixels_scrolled = host_state.pixels_scrolled;
        current.confirmation_sound_requests = host_state.sound_requests;
        current.waiting_for_confirmation = printer.state == 1 || printer.state == 2 ||
            printer.state == 3 || printer.state == 7 || printer.state == 8;
        current.arrow_requested = host_state.arrow;
        return current;
    }
};
TextFlowSession::TextFlowSession(std::span<const std::byte> font,
    std::span<const std::uint16_t> units, Options options)
    : impl_(std::make_unique<Impl>(font, units, options)) {}
TextFlowSession::~TextFlowSession() = default;
TextFlowSession::Frame TextFlowSession::tick(Input input) { return impl_->tick(input); }
TextFlowSession::Frame TextFlowSession::frame() const { return impl_->current; }
std::span<const std::uint8_t> TextFlowSession::pixels() const { return impl_->host_state.pixels; }
unsigned TextFlowSession::width() const { return impl_->host_state.width; }
unsigned TextFlowSession::height() const { return impl_->host_state.height; }
} // namespace hg
