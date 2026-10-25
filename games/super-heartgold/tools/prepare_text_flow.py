"""Select exact pinned RenderText decisions into a private native build.

The generated C retains original function bodies and depends on an authorial
host ABI. It is not the full original text/window/font or task subsystem.
"""
import argparse
import hashlib
from pathlib import Path
import re
import urllib.request

PIN = '6984a3d67592157195940049be2e4bbbf933f9ff'
SOURCE = 'src/render_text.c'
SHA256 = 'b5105ecb2b62d14081645c532e28018f22c41c6c937ee2fbd35f5bb0b2d11458'
FONT_SOURCE = 'src/font_data.c'
FONT_SHA256 = '308e5083177eb936e1eb342ea191e3accdcc46fcc58fbb0bdeb86eaa84773ce5'
WIDTH_SHA256 = '3e68523efc8e0d246f2cbdb6f7ba0d521beded4c49a211bb8a44d1c79c6f0b79'
FUNCTIONS = {
    'TextPrinter_ContinueInputHeld': 'e5ef250c8eb8f7cf7870a024e70dd9e15927225caef1b9f88cd1825ba8569c27',
    'TextPrinter_ContinueInputNew': '8d006605e2a00f7a05ed3efb5990ae0953325b0e5e02083e3fcb71188b467287',
    'RenderText': 'fe08857cc641ff03768d27004c744418aa86ab955e15585ac1b9c92a16e70f1b',
    'TextPrinter_InitDownArrowCounters': 'c6e9439ccc5057a073560e15ac910a5a24c87fd0cbafaa381ab82d1f693670f5',
    'TextPrinter_Continue': 'e5eb3132a7f77af600d61c70fd46999ea9ea298cc7a0b305c86293252a06522d',
    'TextPrinter_WaitAutoMode': '86c5043ef3ab39bc69897bb8286bee73cf6d87129337081b90a5edfaab16c632',
    'TextPrinter_WaitWithDownArrow': '13c547bbef58541636ece01a00f31c64f3fde8463a4af109b41f7d999d701ffb',
    'TextPrinter_Wait': 'ab05781f9c761c87148676eaae78edd6ccb2ee2eea15cdd2b66f00cab4c99a00',
}

PREAMBLE = r'''#include "text_flow_bridge.h"
#define SINGLE_SCREEN 1
#undef GF_ASSERT
#define GF_ASSERT(expression) HGText_Assert((expression) ? TRUE : FALSE)
#define template layout
#define sTextFlags (*HGText_Flags())
#define gSystem (*HGText_Input())
#define sDownArrowBaseTile 0
#define PAD_BUTTON_A 1
#define PAD_BUTTON_B 2
#define EOS 0xffff
#define CHAR_LF 0xe000
#define EXT_CTRL_CODE_BEGIN 0xfffe
#define SEQ_SE_DP_SELECT 0
#define TouchscreenHitbox_TouchHeldIsIn(hitbox) FALSE
#define TouchscreenHitbox_TouchNewIsIn(hitbox) FALSE
#define RenderText HGText_SelectedRenderText
#define GenerateFontHalfRowLookupTable HGText_ColorLookup
#define FontID_TryLoadGlyph HGText_LoadGlyph
#define CopyGlyphToWindow HGText_CopyGlyph
#define GetFontAttribute HGText_FontAttribute
#define GetWindowWidth HGText_WindowWidth
#define FontID_FlatArray_GetWidthFirstLine HGText_WidthFirstLine
#define FillWindowPixelBuffer HGText_FillWindow
#define ScrollWindow HGText_ScrollWindow
#define CopyWindowToVram HGText_PresentWindow
#define LoadDownArrowTiles HGText_LoadDownArrowTiles
#define TextPrinter_DrawDownArrow HGText_DrawDownArrow
#define TextPrinter_ClearDownArrow HGText_ClearDownArrow
#define PlaySE(sequence) HGText_PlaySelectSound()
#define MsgArray_GetControlCode HGText_GetControlCode
#define MsgArray_ControlCodeGetField HGText_ControlField
#define MsgArray_SkipControlCode HGText_SkipControlCode
#define GetStringWidthFirstLine HGText_SelectedWidthFirstLine
struct FontData { u32 (*glyphWidthFunc)(struct FontData *font, int glyph); };
'''


def extract_function(source, name):
    match = re.search(r'^(?:static\s+)?(?:void|BOOL|u8|u32|RenderResult)\s+' + re.escape(name) + r'\([^;{}]*\)\s*\{', source, re.M)
    if match is None:
        raise ValueError(f'Missing selected text function: {name}')
    end = source.index('{', match.start()) + 1
    depth = 1
    while depth and end < len(source):
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    if depth:
        raise ValueError(f'Incomplete selected text function: {name}')
    return source[match.start():end]


def load_pinned_source(destination, filename, source_path, expected):
    original = destination / filename
    if not original.exists() or hashlib.sha256(original.read_bytes()).hexdigest() != expected:
        url = f'https://raw.githubusercontent.com/ClickingMaster/pokeheartgold-single-screen/{PIN}/{source_path}'
        with urllib.request.urlopen(url, timeout=60) as response:
            payload = response.read()
        if hashlib.sha256(payload).hexdigest() != expected:
            raise ValueError(f'Pinned original text source differs: {source_path}')
        original.write_bytes(payload)
    return original.read_text()


def prepare(destination):
    destination = destination.resolve()
    project = Path(__file__).resolve().parents[1]
    if destination.is_relative_to(project) or any((p / '.git').exists() for p in (destination, *destination.parents)):
        raise ValueError('Original text sources require a private build folder outside Git')
    destination.mkdir(parents=True, exist_ok=True)
    source = load_pinned_source(destination, 'render_text.c', SOURCE, SHA256)
    selected = []
    declarations = []
    for name, expected in FUNCTIONS.items():
        function = extract_function(source, name)
        if hashlib.sha256(function.encode()).hexdigest() != expected:
            raise ValueError(f'Selected text function differs: {name}')
        declarations.append(function[:function.index('{')].rstrip() + ';')
        selected.append(function)
    font_source = load_pinned_source(destination, 'font_data.c', FONT_SOURCE, FONT_SHA256)
    width_function = extract_function(font_source, 'GetStringWidthFirstLine')
    if hashlib.sha256(width_function.encode()).hexdigest() != WIDTH_SHA256:
        raise ValueError('Selected text width function differs')
    declarations.append(width_function[:width_function.index('{')].rstrip() + ';')
    selected.append(width_function)
    generated = PREAMBLE + '\n' + '\n'.join(declarations) + '\n\n'
    generated += '\n\n'.join(selected)
    generated += '\n\nRenderResult HGText_Run(TextPrinter *printer) { return RenderText(printer); }\n'
    generated += '''
static u32 HGText_WidthThunk(struct FontData *font, int glyph) {
    (void)font;
    return HGText_GlyphWidthZeroBased(glyph);
}
u32 HGText_MeasureWidth(const u16 *text, u32 spacing) {
    struct FontData font = { HGText_WidthThunk };
    return GetStringWidthFirstLine(&font, text, spacing);
}
'''
    (destination / 'text_decision.c').write_text(generated)
    print(f'Private exact original text decision prepared: {len(selected)} verified functions')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path)
    args = parser.parse_args()
    try:
        prepare(args.destination)
    except (OSError, ValueError) as error:
        parser.exit(1, str(error) + '\n')
