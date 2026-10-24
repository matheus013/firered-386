/* Authorial host ABI for one selected original C function, not a Nitro SDK ABI.
 * Data structures intentionally contain ONLY the fields used by that function.
 * Never use them to load a DS memory image or the full original title module.
 */
#ifndef HG_TITLE_FLOW_BRIDGE_H
#define HG_TITLE_FLOW_BRIDGE_H
#include "heap.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct OverlayManager OverlayManager;
typedef struct TitleScreenAnimData { u32 state; BOOL enableStartInstructionFlash; } TitleScreenAnimData;
typedef struct TitleScreenOverlayData {
    enum HeapID heapID;
    void *bgConfig;
    TitleScreenAnimData animData;
    u32 initialDelay;
    int timer;
    u16 exitMode;
} TitleScreenOverlayData;
typedef struct HGTitleSystem { u32 newKeys,heldKeys; BOOL touchNew; int unk70; } HGTitleSystem;
extern HGTitleSystem gSystem;
enum { PAD_BUTTON_A=1, PAD_BUTTON_B=2, PAD_BUTTON_SELECT=4, PAD_BUTTON_START=8,
    PAD_KEY_UP=64, PAD_KEY_DOWN=128, PAD_BUTTON_X=1024, PAD_BUTTON_Y=2048 };
enum { TITLESCREEN_EXIT_UNSET, TITLESCREEN_EXIT_MENU, TITLESCREEN_EXIT_CLEARSAVE,
    TITLESCREEN_EXIT_TIMEOUT, TITLESCREEN_EXIT_UNUSED, TITLESCREEN_EXIT_MIC_TEST };
enum { SOUND_SCENE_NONE=0, SEQ_NONE=0, SEQ_GS_POKEMON_THEME=1,
    FADE_BOTH_SCREENS=0, FADE_TYPE_BRIGHTNESS_OUT=0, FADE_TYPE_BRIGHTNESS_IN=1,
    HEAP_ID_TITLE_SCREEN=HG_HOST_TEST_HEAP, GX_PLANEMASK_BG3=8, GF_PLANE_TOGGLE_OFF=0 };
#define RGB(r,g,b) ((r)|((g)<<5)|((b)<<10))
#define RGB_WHITE RGB(31,31,31)
#define RGB_BLACK RGB(0,0,0)
#define TITLE_SCREEN_SPECIES 249
#define TITLE_SCREEN_DURATION 2340
#define CLEAR_SAVE_KEY_COMBO (PAD_BUTTON_B | PAD_BUTTON_SELECT | PAD_KEY_UP)
#define MIC_TEST_KEY_COMBO (PAD_BUTTON_X | PAD_BUTTON_Y | PAD_KEY_DOWN)
OverlayManager *HGTitle_Create(void);
BOOL HGTitle_Run(OverlayManager *manager);
void HGTitle_Destroy(OverlayManager *manager);
int HGTitle_Phase(OverlayManager *manager);
void *OverlayManager_CreateAndGetData(OverlayManager *manager,u32 size,enum HeapID heap);
void *OverlayManager_GetData(OverlayManager *manager);
void OverlayManager_FreeData(OverlayManager *manager);
BOOL HGTitle_InitCallback(OverlayManager *manager,int *state);
BOOL HGTitle_ExitCallback(OverlayManager *manager,int *state);
BOOL TitleScreenAnim_InitObjectsAndCamera(TitleScreenAnimData *anim,void *bg,enum HeapID heap);
BOOL TitleScreenAnim_Run(TitleScreenAnimData *anim,void *bg,enum HeapID heap);
BOOL TitleScreenAnim_UnloadAndRemoveTopScreenResources(TitleScreenAnimData *anim,void *bg,enum HeapID heap);
void TitleScreenAnim_GetCameraNextPosition(TitleScreenAnimData *anim);
void TitleScreenAnim_FadeInGameTitleLayer(TitleScreenAnimData *anim);
void Sound_SetScene(int scene);
void Sound_SetSceneAndPlayBGM(int scene,int sequence,int mode);
void GF_SndStartFadeOutBGM(int sequence,int duration);
void PlayCry(int species,int mode);
void GF_SetVolumeBySeqNo(int sequence,int volume);
void StopBGM(int sequence,int mode);
void BeginNormalPaletteFade(int screens,int first,int second,int color,int duration,int speed,int heap);
BOOL IsPaletteFadeFinished(void);
int GF_SndGetFadeTimer(void);
void GfGfx_EngineATogglePlanes(int plane,int mode);
#ifdef __cplusplus
}
#endif
#endif
