#ifndef HG_SINGLE_SCREEN_TOUCH_H
#define HG_SINGLE_SCREEN_TOUCH_H
// Coordinates use logical window pixels, matching SDL mouse events.
static inline int hg_single_screen_touch(int visible, int pressed,
    int x, int y, int left, int top, int width, int height,
    unsigned short *out_x, unsigned short *out_y) {
    if (!visible || !pressed || width <= 0 || height <= 0) return 0;
    const long long dx = (long long)x - left, dy = (long long)y - top;
    if (dx < 0 || dy < 0 || dx >= width || dy >= height) return 0;
    *out_x = (unsigned short)(dx * 256 / width);
    *out_y = (unsigned short)(dy * 192 / height);
    return 1;
}
#endif
