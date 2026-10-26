#include "hg/native_overlays.h"

static int valid(const HgNativeOverlayRegistry *registry) {
    return registry && registry->definitions && registry->states && registry->count;
}

static size_t find(const HgNativeOverlayRegistry *registry, uint32_t id) {
    for (size_t i = 0; i < registry->count; ++i)
        if (registry->definitions[i].id == id) return i;
    return registry->count;
}

HgNativeOverlayResult HgNativeOverlays_Init(HgNativeOverlayRegistry *registry,
    const HgNativeOverlayDefinition *definitions, HgNativeOverlayState *states, size_t count) {
    if (!registry || registry->definitions || registry->states || registry->count ||
        !definitions || !states || !count) return HG_OVERLAY_INVALID;
    for (size_t i = 0; i < count; ++i) {
        if (definitions[i].id == UINT32_MAX || !definitions[i].load || !definitions[i].unload)
            return HG_OVERLAY_INVALID;
        for (size_t j = 0; j < i; ++j)
            if (definitions[i].id == definitions[j].id) return HG_OVERLAY_INVALID;
    }
    for (size_t i = 0; i < count; ++i) states[i] = HG_OVERLAY_INACTIVE;
    registry->definitions = definitions;
    registry->states = states;
    registry->count = count;
    return HG_OVERLAY_OK;
}

HgNativeOverlayResult HgNativeOverlays_Load(HgNativeOverlayRegistry *registry, uint32_t id) {
    if (!valid(registry)) return HG_OVERLAY_INVALID;
    size_t index = find(registry, id);
    if (index == registry->count) return HG_OVERLAY_UNKNOWN;
    if (registry->states[index] == HG_OVERLAY_ACTIVE) return HG_OVERLAY_ALREADY_ACTIVE;
    if (registry->states[index] != HG_OVERLAY_INACTIVE) return HG_OVERLAY_BUSY;
    const HgNativeOverlayDefinition *definition = &registry->definitions[index];
    registry->states[index] = HG_OVERLAY_LOADING;
    if (!definition->load(definition->context)) {
        registry->states[index] = HG_OVERLAY_INACTIVE;
        return HG_OVERLAY_LOAD_FAILED;
    }
    registry->states[index] = HG_OVERLAY_ACTIVE;
    return HG_OVERLAY_OK;
}

HgNativeOverlayResult HgNativeOverlays_Unload(HgNativeOverlayRegistry *registry, uint32_t id) {
    if (!valid(registry)) return HG_OVERLAY_INVALID;
    size_t index = find(registry, id);
    if (index == registry->count) return HG_OVERLAY_UNKNOWN;
    if (registry->states[index] == HG_OVERLAY_INACTIVE) return HG_OVERLAY_NOT_ACTIVE;
    if (registry->states[index] != HG_OVERLAY_ACTIVE) return HG_OVERLAY_BUSY;
    const HgNativeOverlayDefinition *definition = &registry->definitions[index];
    registry->states[index] = HG_OVERLAY_UNLOADING;
    definition->unload(definition->context);
    registry->states[index] = HG_OVERLAY_INACTIVE;
    return HG_OVERLAY_OK;
}

HgNativeOverlayResult HgNativeOverlays_Close(HgNativeOverlayRegistry *registry) {
    if (!valid(registry)) return HG_OVERLAY_INVALID;
    for (size_t i = 0; i < registry->count; ++i)
        if (registry->states[i] != HG_OVERLAY_INACTIVE) return HG_OVERLAY_BUSY;
    *registry = (HgNativeOverlayRegistry){0};
    return HG_OVERLAY_OK;
}
