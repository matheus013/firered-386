#ifndef HG_NATIVE_OVERLAYS_H
#define HG_NATIVE_OVERLAYS_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Statically linked host modules. No ROM addresses or executable ARM data.
 * Callbacks must not throw. A failed load must clean up its own resources.
 * Definitions and states must outlive the registry. Single-threaded API.
 */
typedef struct HgNativeOverlayDefinition {
    uint32_t id;
    int (*load)(void *context);
    void (*unload)(void *context);
    void *context;
} HgNativeOverlayDefinition;

typedef enum HgNativeOverlayState {
    HG_OVERLAY_INACTIVE, HG_OVERLAY_LOADING, HG_OVERLAY_ACTIVE, HG_OVERLAY_UNLOADING
} HgNativeOverlayState;

typedef enum HgNativeOverlayResult {
    HG_OVERLAY_OK, HG_OVERLAY_INVALID, HG_OVERLAY_UNKNOWN,
    HG_OVERLAY_ALREADY_ACTIVE, HG_OVERLAY_BUSY, HG_OVERLAY_LOAD_FAILED,
    HG_OVERLAY_NOT_ACTIVE
} HgNativeOverlayResult;

/* Initialize the registry object to zero before its first use. */
typedef struct HgNativeOverlayRegistry {
    const HgNativeOverlayDefinition *definitions;
    HgNativeOverlayState *states;
    size_t count;
} HgNativeOverlayRegistry;

HgNativeOverlayResult HgNativeOverlays_Init(HgNativeOverlayRegistry *registry,
    const HgNativeOverlayDefinition *definitions, HgNativeOverlayState *states, size_t count);
HgNativeOverlayResult HgNativeOverlays_Load(HgNativeOverlayRegistry *registry, uint32_t id);
HgNativeOverlayResult HgNativeOverlays_Unload(HgNativeOverlayRegistry *registry, uint32_t id);
HgNativeOverlayResult HgNativeOverlays_Close(HgNativeOverlayRegistry *registry);

#ifdef __cplusplus
}
#endif
#endif
