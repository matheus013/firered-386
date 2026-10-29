#include "sprite.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    Sprite sprite;
    const size_t sizes[] = {sizeof(SpriteAnimationData), sizeof(SpriteAnimationDataWithTransfer), sizeof(SpriteMultiAnimationData)};
    const size_t alignments[] = {__alignof__(SpriteAnimationData), __alignof__(SpriteAnimationDataWithTransfer), __alignof__(SpriteMultiAnimationData)};
    printf("capacity=%zu cell=%zu transfer=%zu multi=%zu offset=%zu\n",
           sizeof(sprite.animationData), sizes[0], sizes[1], sizes[2], offsetof(Sprite, animationData));
    for (unsigned kind = 0; kind < 3; ++kind) {
        if (sizes[kind] > sizeof(sprite.animationData)) {
            fprintf(stderr, "animation storage too small for variant %u\n", kind);
            return 42;
        }
        if ((uintptr_t)sprite.animationData % alignments[kind]) return 43;
        memset(&sprite, 0x5a, sizeof(sprite));
        unsigned char before[sizeof(sprite)];
        memcpy(before, &sprite, sizeof(sprite));
        memset(sprite.animationData, 0xa5, sizes[kind]);
        size_t start = offsetof(Sprite, animationData);
        if (memcmp(&sprite, before, start) ||
            memcmp((unsigned char *)&sprite + start + sizes[kind], before + start + sizes[kind],
                   sizeof(sprite) - start - sizes[kind])) return 44;
    }
    puts("all three variants fit and preserve adjacent sprite fields");
    return 0;
}
