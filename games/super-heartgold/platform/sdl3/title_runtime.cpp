#include "title_runtime.hpp"
#include <algorithm>
#include <array>

namespace hg::sdl {
namespace {
struct Key { Action action; TitleSession::Button button; };
constexpr std::array keys{
    Key{Action::A,TitleSession::A},Key{Action::B,TitleSession::B},
    Key{Action::Start,TitleSession::Start},Key{Action::Select,TitleSession::Select},
    Key{Action::Up,TitleSession::Up},Key{Action::Down,TitleSession::Down},
    Key{Action::X,TitleSession::X},Key{Action::Y,TitleSession::Y}
};
Uint8 channel(unsigned color,unsigned shift) {
    const auto value=(color>>shift)&31;
    return static_cast<Uint8>((value<<3)|(value>>2));
}
const char* destination_label(unsigned destination) {
    switch (destination) {
    case TitleSession::Menu: return "STARTING CONTROLLED CHECK-SAVE DECISION";
    case TitleSession::DeleteSave: return "DELETE-SAVE REQUESTED - BLOCKED, NO SAVE ACCESS";
    case TitleSession::Intro: return "INTRO REQUESTED - INTRO NOT PORTED";
    case TitleSession::Microphone: return "MIC TEST REQUESTED - NOT PORTED";
    default: return "UNSUPPORTED DESTINATION";
    }
}
const char* status_message(std::uint32_t message) {
    constexpr std::array descriptions{
        "PRIMARY DATA: BACKUP STATUS FLAG", "PRIMARY DATA: CORRUPTION STATUS FLAG",
        "BATTLE VIDEO: BACKUP STATUS FLAG", "BATTLE VIDEO: CORRUPTION STATUS FLAG",
        "BATTLE HALL: BACKUP STATUS FLAG", "BATTLE HALL: CORRUPTION STATUS FLAG"
    };
    return message<descriptions.size() ? descriptions[message] : "UNKNOWN CONTROLLED MESSAGE";
}
}
void TitleRuntime::advance(const InputState& input,int steps) {
    std::uint32_t held{};
    for (const auto& key:keys) {
        if (input.was_pressed(key.action)) pending_keys_|=key.button;
        if (input.is_down(key.action)) held|=key.button;
    }
    // Preserve key edges when a rendered frame has no fixed simulation step.
    for (int i=0;i<steps;++i) {
        auto pressed=pending_keys_;
        pending_keys_=0;
        if (!frame_.completed) frame_=session_.tick(pressed,held);
        else if (frame_.destination==TitleSession::Menu) {
            if (!checksave_) checksave_=std::make_unique<ChecksaveSession>(controlled_flags_);
            check_frame_=checksave_->tick((pressed&TitleSession::A)!=0);
        }
    }
}
void TitleRuntime::draw(SDL_Renderer* renderer,const SDL_FRect& image,int height) const {
    if (frame_.tint_alpha && image.w>0 && image.h>0) {
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,channel(frame_.tint_rgb555,0),channel(frame_.tint_rgb555,5),
            channel(frame_.tint_rgb555,10),static_cast<Uint8>(frame_.tint_alpha));
        SDL_RenderFillRect(renderer,&image);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
    }
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    if (checksave_) {
        SDL_RenderDebugText(renderer,8,static_cast<float>(std::max(0,height-24)),"CONTROLLED CHECK-SAVE DECISION - NO SAVE FILE LOADED");
        const char* status=check_frame_.completed ? "DECISION COMPLETE - MAIN MENU / REAL SAVE IO NOT PORTED" :
            check_frame_.awaiting_ack ? "X: ACKNOWLEDGE FIXTURE MESSAGE" : "PROCESSING CONTROLLED STATUS FLAGS";
        SDL_RenderDebugText(renderer,8,static_cast<float>(std::max(0,height-12)),status);
        if (check_frame_.awaiting_ack && image.w>0 && image.h>0)
            SDL_RenderDebugText(renderer,image.x+16,image.y+16,status_message(check_frame_.message));
        return;
    }
    SDL_RenderDebugText(renderer,8,static_cast<float>(std::max(0,height-24)),"ORIGINAL TITLE DECISION C / HOST 2D - 3D AND AUDIO PENDING");
    const char* status="TITLE INITIALIZING";
    if (frame_.completed) status=destination_label(frame_.destination);
    else if (frame_.destination!=TitleSession::Unset) status="TITLE TRANSITION IN PROGRESS";
    else if (frame_.initial_delay==0 && frame_.phase==TitleSession::Play)
        status=frame_.prompt_visible ? "ENTER / X: REQUEST MENU" : "";
    SDL_RenderDebugText(renderer,8,static_cast<float>(std::max(0,height-12)),status);
}
bool TitleRuntime::menu_handoff_complete() const {
    return frame_.completed && frame_.destination==TitleSession::Menu && TitleSession::allocations()==0 &&
        checksave_ && check_frame_.completed;
}
}
