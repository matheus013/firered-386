#include "hg/native_overlays.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "Overlay check failed at %d: %s\n", __LINE__, #c); abort(); } } while (0)
typedef struct Context {
    HgNativeOverlayRegistry *registry;
    uint32_t id;
    unsigned loads, unloads;
    int fail;
} Context;
static int load(void *argument) {
    Context *context = argument;
    ++context->loads;
    CHECK(HgNativeOverlays_Load(context->registry, context->id) == HG_OVERLAY_BUSY);
    CHECK(HgNativeOverlays_Unload(context->registry, context->id) == HG_OVERLAY_BUSY);
    CHECK(HgNativeOverlays_Close(context->registry) == HG_OVERLAY_BUSY);
    return !context->fail;
}
static void unload(void *argument) {
    Context *context = argument;
    ++context->unloads;
    CHECK(HgNativeOverlays_Load(context->registry, context->id) == HG_OVERLAY_BUSY);
    CHECK(HgNativeOverlays_Unload(context->registry, context->id) == HG_OVERLAY_BUSY);
    CHECK(HgNativeOverlays_Close(context->registry) == HG_OVERLAY_BUSY);
}
int main(void) {
    HgNativeOverlayRegistry registry = {0};
    HgNativeOverlayState states[2] = {HG_OVERLAY_ACTIVE, HG_OVERLAY_ACTIVE};
    Context a = {&registry, 7, 0, 0, 0}, b = {&registry, 8, 0, 0, 1};
    HgNativeOverlayDefinition definitions[] = {{7, load, unload, &a}, {8, load, unload, &b}};
    CHECK(HgNativeOverlays_Load(NULL, 7) == HG_OVERLAY_INVALID);
    CHECK(HgNativeOverlays_Unload(&registry, 7) == HG_OVERLAY_INVALID);
    CHECK(HgNativeOverlays_Init(&registry, definitions, states, 0) == HG_OVERLAY_INVALID);
    definitions[1].id = 7;
    CHECK(HgNativeOverlays_Init(&registry, definitions, states, 2) == HG_OVERLAY_INVALID);
    CHECK(!registry.count && states[0] == HG_OVERLAY_ACTIVE);
    definitions[1].id = UINT32_MAX;
    CHECK(HgNativeOverlays_Init(&registry, definitions, states, 2) == HG_OVERLAY_INVALID);
    definitions[1].id = 8;
    CHECK(HgNativeOverlays_Init(&registry, definitions, states, 2) == HG_OVERLAY_OK);
    CHECK(HgNativeOverlays_Init(&registry, definitions, states, 2) == HG_OVERLAY_INVALID);
    CHECK(HgNativeOverlays_Load(&registry, 0) == HG_OVERLAY_UNKNOWN);
    CHECK(HgNativeOverlays_Unload(&registry, 0) == HG_OVERLAY_UNKNOWN);
    CHECK(!a.loads && !b.loads);
    CHECK(HgNativeOverlays_Load(&registry, 7) == HG_OVERLAY_OK);
    CHECK(HgNativeOverlays_Load(&registry, 7) == HG_OVERLAY_ALREADY_ACTIVE);
    CHECK(a.loads == 1 && states[0] == HG_OVERLAY_ACTIVE);
    CHECK(HgNativeOverlays_Load(&registry, 8) == HG_OVERLAY_LOAD_FAILED);
    CHECK(states[1] == HG_OVERLAY_INACTIVE && !b.unloads);
    b.fail = 0;
    CHECK(HgNativeOverlays_Load(&registry, 8) == HG_OVERLAY_OK);
    CHECK(b.loads == 2);
    CHECK(HgNativeOverlays_Close(&registry) == HG_OVERLAY_BUSY);
    CHECK(HgNativeOverlays_Unload(&registry, 7) == HG_OVERLAY_OK);
    CHECK(HgNativeOverlays_Unload(&registry, 7) == HG_OVERLAY_NOT_ACTIVE);
    CHECK(a.unloads == 1 && states[1] == HG_OVERLAY_ACTIVE);
    CHECK(HgNativeOverlays_Unload(&registry, 8) == HG_OVERLAY_OK);
    CHECK(b.unloads == 1);
    CHECK(HgNativeOverlays_Close(&registry) == HG_OVERLAY_OK);
    CHECK(!registry.count && !registry.definitions && !registry.states);
    CHECK(HgNativeOverlays_Init(&registry, definitions, states, 2) == HG_OVERLAY_OK);
    CHECK(HgNativeOverlays_Close(&registry) == HG_OVERLAY_OK);
    puts("Native overlays: identity, rejection, reentrancy, failure recovery and lifetime passed");
    return 0;
}
