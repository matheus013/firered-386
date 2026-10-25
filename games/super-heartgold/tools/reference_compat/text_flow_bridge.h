/* Authorial ABI for a PRIVATE selected original text decision module.
 * Native host structures: not Nintendo DS memory layouts or serialized data.
 * Source aliases exist only in generated C, never in C++ consumers.
 */
#ifndef HG_TEXT_FLOW_BRIDGE_H
#define HG_TEXT_FLOW_BRIDGE_H
#include "global.h"
typedef uint8_t u8;
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Window {
    unsigned reserved; /* Host window identity; geometry belongs to the host. */
} Window;

typedef struct TextFlags {
    u8 canABSpeedUpPrint;
    u8 useAlternateDownArrow;
    u8 autoScroll;
    u8 forceMidTextSpeed;
    u8 canTouchSpeedUpPrint; /* Host must reject TRUE: touch is not ported. */
    u8 autoScrollCanSpeedUp;
    u8 hasSpedUpInput;
    u8 hasContinuedInput;
    u8 touchIsSpeedingUpPrint;
    u8 touchHitboxActive;
} TextFlags;

typedef struct TextPrinterSubStruct {
    u8 fontId;
    u8 hasPrintBeenSpedUp;
    u8 unk;
    u8 downArrowDelay;
    u8 downArrowYPosIdx; /* Host Draw hook must wrap modulo 4 explicitly. */
    u8 hasFontIdBeenSet;
    u8 autoScrollDelay;
} TextPrinterSubStruct;

typedef struct HGTextInput {
    u32 heldKeys;
    u32 newKeys;
    BOOL touchHeld;
    BOOL touchNew;
} HGTextInput;

typedef struct GlyphInfo {
    u8 data[128];
    u8 width;
    u8 height;
} GlyphInfo;

typedef union HGText_StringCursor { const u16 *raw; } HGText_StringCursor;
typedef struct TextPrinterTemplate {
    HGText_StringCursor currentChar;
    Window *window;
    u8 fontId;
    u8 x, y;
    u16 currentX, currentY;
    u16 letterSpacing, lineSpacing;
    u8 unk14;
    u8 fgColor, bgColor, shadowColor;
    u16 glyphTable;
    u8 unk1A;
    u8 savedColor;
} TextPrinterTemplate;

typedef struct TextPrinter {
    TextPrinterTemplate layout;
    TextPrinterSubStruct subStructFields;
    u8 active;
    u8 state;
    u8 textSpeedBottom;
    u8 textSpeedTop;
    u8 delayCounter;
    u8 scrollDistance;
    u8 id;
    u8 callbackResult;
    u16 callbackParam;
} TextPrinter;

typedef enum RenderResult {
    RENDER_PRINT = 0,
    RENDER_FINISH = 1,
    RENDER_REPEAT = 2,
    RENDER_UPDATE = 3
} RenderResult;

/* Context accessors must return valid per-session storage during HGText_Run.
 * Only validated, terminated units may be passed to the original function.
 * C++ exceptions must not escape any service across the C call boundary.
 */
TextFlags *HGText_Flags(void);
HGTextInput *HGText_Input(void);
RenderResult HGText_Run(TextPrinter *printer);
void HGText_Assert(BOOL condition);

void HGText_ColorLookup(u8 foreground, u8 background, u8 shadow);
GlyphInfo *HGText_LoadGlyph(u8 font, u16 glyph);
void HGText_CopyGlyph(Window *window, const u8 *data, u16 width, u16 height,
    u16 x, u16 y, u16 table);
u8 HGText_FontAttribute(u8 font, int attribute);
u32 HGText_WindowWidth(Window *window);
u32 HGText_WidthFirstLine(u8 font, const u16 *text, u32 spacing);
void HGText_FillWindow(Window *window, u8 background);
void HGText_ScrollWindow(Window *window, u8 direction, u8 distance, u8 fill);
void HGText_PresentWindow(Window *window);
void HGText_LoadDownArrowTiles(Window *window, u16 tile);
void HGText_DrawDownArrow(TextPrinter *printer);
void HGText_ClearDownArrow(TextPrinter *printer);
void HGText_PlaySelectSound(void);
int HGText_GetControlCode(const u16 *units);
int HGText_ControlField(const u16 *units, u32 field);
const u16 *HGText_SkipControlCode(const u16 *units);

#ifdef __cplusplus
}
#endif
#endif
