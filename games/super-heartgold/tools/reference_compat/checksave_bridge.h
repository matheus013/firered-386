/* Own host ABI for the private original status-decision function.
 * No on-disk SaveData representation, file access, or SDK implementation.
 */
#ifndef HG_CHECKSAVE_BRIDGE_H
#define HG_CHECKSAVE_BRIDGE_H
#include "global.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct HGCheck_ControlledFlags { u32 status; } HGCheck_ControlledFlags;
typedef struct HGCheck_Data {
    int heapID;
    int mainState;
    u32 msgNum;
    void *bgConfig;
    HGCheck_ControlledFlags *saveData;
    u32 saveStatusFlags;
} HGCheck_Data;

u32 HGCheck_GetStatusFlags(HGCheck_ControlledFlags *flags);
void HGCheck_BeginFade(int screens, int first, int second, int color, int duration, int wait, int heap);
BOOL HGCheck_FadeFinished(void);
void HGCheck_SetMaskColor(int layer, int color);
BOOL HGCheck_PrintMessage(HGCheck_Data *data, u32 message, BOOL skip_waiting, u32 speed);
BOOL HGCheck_Run(HGCheck_Data *data);

/* Aliases are private to this adapter, preventing collisions with title hooks. */
#define CheckSavedataApp_Data HGCheck_Data
#define Save_GetStatusFlags HGCheck_GetStatusFlags
#define BeginNormalPaletteFade HGCheck_BeginFade
#define IsPaletteFadeFinished HGCheck_FadeFinished
#define BG_SetMaskColor HGCheck_SetMaskColor
#define CheckSavedataApp_PrintMessage HGCheck_PrintMessage
#define GF_BG_LYR_MAIN_0 0
#define GF_BG_LYR_SUB_0 4
#define RGB(r,g,b) ((r) | ((g) << 5) | ((b) << 10))
#define RGB_BLACK 0
#define FADE_BOTH_SCREENS 0
#define FADE_TYPE_BRIGHTNESS_IN 1
#define FADE_TYPE_BRIGHTNESS_OUT 0
#define msg_0229_00000 0
#define msg_0229_00001 1
#define msg_0229_00002 2
#define msg_0229_00003 3
#define msg_0229_00004 4
#define msg_0229_00005 5
#ifdef __cplusplus
}
#endif
#endif
