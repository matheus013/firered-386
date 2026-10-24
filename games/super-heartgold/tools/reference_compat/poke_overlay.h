/* Host boundary: actual DS overlay binary loads are unsupported. */
#ifndef HG_NATIVE_POKE_OVERLAY_H
#define HG_NATIVE_POKE_OVERLAY_H
#include "global.h"
typedef enum PMOverlayLoadType { OVY_LOAD_NORMAL, OVY_LOAD_NOINIT, OVY_LOAD_ASYNC } PMOverlayLoadType;
#ifdef __cplusplus
extern "C" {
#endif
BOOL HandleLoadOverlay(FSOverlayID id, PMOverlayLoadType type);
void UnloadOverlayByID(FSOverlayID id);
#ifdef __cplusplus
}
#endif
#endif
