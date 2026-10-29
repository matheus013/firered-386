"""Size and align native HGSS inline animation storage for all animation variants."""
import hashlib
from pathlib import Path

HASH = '93b45857cb99ec9f69e9a8675081a8ec839a47bb0bb730ded1aefa390616e7b1'


def prepare(source: Path, destination: Path):
    raw = source.read_bytes()
    if hashlib.sha256(raw).hexdigest() != HASH:
        raise ValueError('Sprite header source hash differs')
    text = raw.decode()
    marker = '// UB: this is too small'
    native = '''#ifdef SDK_PORT
typedef union NativeSpriteAnimationStorage {
    SpriteAnimationData cell;
    SpriteAnimationDataWithTransfer transfer;
    SpriteMultiAnimationData multi;
} NativeSpriteAnimationStorage;
#endif

'''
    old = '    u32 animationData[SPRITE_ANIMATION_DATA_WORD_COUNT]; // 040'
    new = '''#ifdef SDK_PORT
    union {
        NativeSpriteAnimationStorage nativeAnimationStorage;
        u32 animationData[(sizeof(NativeSpriteAnimationStorage) + sizeof(u32) - 1) / sizeof(u32)];
    };
#else
''' + old + '\n#endif'
    if text.count(marker) != 1 or text.count(old) != 1:
        raise ValueError('Sprite storage contract differs')
    destination.write_text(text.replace(marker, native + marker).replace(old, new))
