#include "hg/viewport.hpp"

#include <algorithm>
#include <cmath>

namespace hg {

ScreenPlacement place_screens(Size window, ScreenLayout layout, ScaleMode mode, int gap)
{
    ScreenPlacement out;
    gap = std::max(gap, 0);

    const bool stacked = layout == ScreenLayout::Stacked;
    const int logical_w = stacked ? kDsScreenWidth : kDsScreenWidth * 2;
    const int logical_h = stacked ? kDsScreenHeight * 2 : kDsScreenHeight;

    // O espaço entre as telas não escala; só as telas escalam.
    const int avail_w = window.w - (stacked ? 0 : gap);
    const int avail_h = window.h - (stacked ? gap : 0);
    if (avail_w <= 0 || avail_h <= 0) {
        return out;
    }

    float scale = std::min(static_cast<float>(avail_w) / static_cast<float>(logical_w),
                           static_cast<float>(avail_h) / static_cast<float>(logical_h));
    if (mode == ScaleMode::Integer && scale >= 1.0f) {
        scale = std::floor(scale);
    }

    const int screen_w = static_cast<int>(std::floor(static_cast<float>(kDsScreenWidth) * scale));
    const int screen_h = static_cast<int>(std::floor(static_cast<float>(kDsScreenHeight) * scale));
    const int total_w = stacked ? screen_w : screen_w * 2 + gap;
    const int total_h = stacked ? screen_h * 2 + gap : screen_h;
    const int origin_x = (window.w - total_w) / 2;
    const int origin_y = (window.h - total_h) / 2;

    out.scale = scale;
    out.top = {origin_x, origin_y, screen_w, screen_h};
    if (stacked) {
        out.bottom = {origin_x, origin_y + screen_h + gap, screen_w, screen_h};
    } else {
        out.bottom = {origin_x + screen_w + gap, origin_y, screen_w, screen_h};
    }
    return out;
}

} // namespace hg
