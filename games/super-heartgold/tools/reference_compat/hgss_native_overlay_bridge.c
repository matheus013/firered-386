#include "hgss_native_overlay_bridge.h"
/* Resolve the original SDK-backed header via the ordered include path,
 * not the older isolated title-probe shim beside this file. */
#include <poke_overlay.h>
#include <stdio.h>
#include <stdlib.h>

static HgNativeOverlayRegistry *bound_registry;

static void fail(const char *operation, FSOverlayID id, HgNativeOverlayResult result) {
    fprintf(stderr, "Native overlay %s failed: id=%u result=%d\n", operation, (unsigned)id, (int)result);
    GF_AssertFail();
    /* A diagnostic handler that returns must not resume the unported module. */
    abort();
}

int HGSS_BindNativeOverlays(HgNativeOverlayRegistry *registry) {
    if (bound_registry || !registry || !registry->definitions || !registry->states || !registry->count)
        return 0;
    for (size_t i = 0; i < registry->count; ++i)
        if (registry->states[i] != HG_OVERLAY_INACTIVE) return 0;
    bound_registry = registry;
    return 1;
}

int HGSS_UnbindNativeOverlays(void) {
    if (!bound_registry) return 0;
    for (size_t i = 0; i < bound_registry->count; ++i)
        if (bound_registry->states[i] != HG_OVERLAY_INACTIVE) return 0;
    bound_registry = NULL;
    return 1;
}

BOOL HandleLoadOverlay(FSOverlayID id, PMOverlayLoadType type) {
    HgNativeOverlayResult result = HG_OVERLAY_INVALID;
    if (type == OVY_LOAD_NORMAL || type == OVY_LOAD_NOINIT || type == OVY_LOAD_ASYNC)
        result = HgNativeOverlays_Load(bound_registry, id);
    if (result != HG_OVERLAY_OK) {
        /* Original OverlayManager_Run ignores this function's return value.
         * Never let it proceed to an application whose module did not load. */
        fail("load", id, result);
        return FALSE;
    }
    return TRUE;
}

void UnloadOverlayByID(FSOverlayID id) {
    HgNativeOverlayResult result = HgNativeOverlays_Unload(bound_registry, id);
    /* Original HGSS tolerates unloading a known module that is not active. */
    if (result != HG_OVERLAY_OK && result != HG_OVERLAY_NOT_ACTIVE) fail("unload", id, result);
}
