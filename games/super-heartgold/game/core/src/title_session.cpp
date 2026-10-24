#include "hg/title_session.hpp"
#include "title_flow_bridge.h"
#include "poke_overlay.h"

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <set>
#include <stdexcept>

namespace {
// This single host context mirrors the original C controller's global services.
struct HostServices {
    bool active{};
    bool prompt_visible{};
    unsigned fade_remaining{}, fade_duration{}, fade_color{};
    unsigned sound_remaining{}, animation_frames{}, audio_requests{};
    bool fade_in{};

    void advance_timers() {
        if (fade_remaining) --fade_remaining;
        if (sound_remaining) --sound_remaining;
    }

    unsigned tint_alpha() const {
        if (!fade_duration) return 0;
        const auto progress = fade_in ? fade_remaining : fade_duration - fade_remaining;
        return 255 * progress / fade_duration;
    }
};
HostServices services;
std::set<void*> live_allocations;

// Exceptions must never escape the host allocation functions into original C.
[[noreturn]] void unsupported_overlay() { std::abort(); }
struct OverlayDeleter {
    void operator()(OverlayManager* manager) const { HGTitle_Destroy(manager); }
};
using OverlayOwner = std::unique_ptr<OverlayManager, OverlayDeleter>;
}

extern "C" {
HGTitleSystem gSystem{};

void* Heap_Alloc(enum HeapID, u32 size) {
    void* pointer = std::malloc(size);
    if (!pointer) std::abort();
    try {
        live_allocations.insert(pointer);
    } catch (...) {
        std::free(pointer);
        std::abort();
    }
    return pointer;
}
void* Heap_AllocAtEnd(enum HeapID heap, u32 size) { return Heap_Alloc(heap, size); }
void Heap_Free(void* pointer) {
    if (!pointer || live_allocations.erase(pointer) != 1) std::abort();
    std::free(pointer);
}
BOOL HandleLoadOverlay(FSOverlayID, PMOverlayLoadType) { unsupported_overlay(); }
void UnloadOverlayByID(FSOverlayID) { unsupported_overlay(); }

BOOL HGTitle_InitCallback(OverlayManager* manager, int*) {
    auto* data = static_cast<TitleScreenOverlayData*>(OverlayManager_CreateAndGetData(
        manager, sizeof(TitleScreenOverlayData), HG_HOST_TEST_HEAP));
    *data = TitleScreenOverlayData{};
    data->heapID = HG_HOST_TEST_HEAP;
    return TRUE;
}
BOOL HGTitle_ExitCallback(OverlayManager* manager, int*) {
    OverlayManager_FreeData(manager);
    return TRUE;
}

// Diagnostic renderer hooks, not implementations of original 3D resources.
BOOL TitleScreenAnim_InitObjectsAndCamera(TitleScreenAnimData*, void*, enum HeapID) { return TRUE; }
BOOL TitleScreenAnim_Run(TitleScreenAnimData* animation, void*, enum HeapID) {
    services.prompt_visible = animation->enableStartInstructionFlash && services.animation_frames % 45 < 30;
    ++services.animation_frames;
    return TRUE;
}
BOOL TitleScreenAnim_UnloadAndRemoveTopScreenResources(TitleScreenAnimData*, void*, enum HeapID) { return TRUE; }
void TitleScreenAnim_GetCameraNextPosition(TitleScreenAnimData*) {}
void TitleScreenAnim_FadeInGameTitleLayer(TitleScreenAnimData*) {}

// Sound requests and their timer are observed; no audio is synthesized here.
void Sound_SetScene(int) { ++services.audio_requests; }
void Sound_SetSceneAndPlayBGM(int, int, int) { ++services.audio_requests; }
void GF_SndStartFadeOutBGM(int, int duration) {
    services.sound_remaining = static_cast<unsigned>(std::max(0, duration));
    ++services.audio_requests;
}
void PlayCry(int, int) { ++services.audio_requests; }
void GF_SetVolumeBySeqNo(int, int) { ++services.audio_requests; }
void StopBGM(int, int) { ++services.audio_requests; }
int GF_SndGetFadeTimer() { return static_cast<int>(services.sound_remaining); }

void BeginNormalPaletteFade(int, int first, int, int color, int duration, int, int) {
    services.fade_duration = static_cast<unsigned>(std::max(1, duration));
    services.fade_remaining = services.fade_duration;
    services.fade_color = static_cast<unsigned>(color);
    services.fade_in = first == FADE_TYPE_BRIGHTNESS_IN;
}
BOOL IsPaletteFadeFinished() { return services.fade_remaining == 0; }
void GfGfx_EngineATogglePlanes(int, int) { services.prompt_visible = false; }
}

namespace hg {
static_assert(static_cast<unsigned>(TitleSession::A) == PAD_BUTTON_A);
static_assert(static_cast<unsigned>(TitleSession::Start) == PAD_BUTTON_START);
static_assert(static_cast<unsigned>(TitleSession::DeleteSave) == TITLESCREEN_EXIT_CLEARSAVE);
static_assert(static_cast<unsigned>(TitleSession::Microphone) == TITLESCREEN_EXIT_MIC_TEST);

struct TitleSession::Impl {
    OverlayOwner manager;
    Frame current;

    Impl() {
        if (services.active) throw std::logic_error("Only one original title session is supported");
        services = HostServices{};
        gSystem = HGTitleSystem{};
        manager.reset(HGTitle_Create());
        services.active = true;
    }

    ~Impl() {
        // Release original C data before its host service context is reset.
        manager.reset();
        services = HostServices{};
        gSystem = HGTitleSystem{};
    }

    void capture_frame() {
        current.phase = static_cast<unsigned>(HGTitle_Phase(manager.get()));
        if (auto* data = static_cast<TitleScreenOverlayData*>(OverlayManager_GetData(manager.get()))) {
            current.initial_delay = data->initialDelay;
            current.play_frames = static_cast<unsigned>(data->timer);
            current.destination = data->exitMode;
        }
        // The original exit callback frees data; keep the last destination.
        current.tint_rgb555 = services.fade_color;
        current.tint_alpha = services.tint_alpha();
        current.prompt_visible = services.prompt_visible && current.destination == Unset && !current.completed;
    }
};

TitleSession::TitleSession() : impl_(std::make_unique<Impl>()) {}
TitleSession::~TitleSession() = default;

TitleSession::Frame TitleSession::tick(std::uint32_t pressed, std::uint32_t held, bool touch) {
    if (impl_->current.completed) return impl_->current;
    services.advance_timers();
    gSystem.newKeys = pressed;
    gSystem.heldKeys = held;
    gSystem.touchNew = touch;
    impl_->current.completed = HGTitle_Run(impl_->manager.get()) != FALSE;
    impl_->capture_frame();
    if (impl_->current.completed) impl_->manager.reset();
    return impl_->current;
}
TitleSession::Frame TitleSession::frame() const { return impl_->current; }
unsigned TitleSession::allocations() { return static_cast<unsigned>(live_allocations.size()); }
}
