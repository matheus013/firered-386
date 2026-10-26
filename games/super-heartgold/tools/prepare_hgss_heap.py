"""Checked private-source adaptations for native heap alignment."""
from pathlib import Path


def replace_exact(text, old, new, count=1):
    if text.count(old) != count:
        raise ValueError('Heap source contract differs: ' + old)
    return text.replace(old, new)


def prepare_game_heap(reference: Path, output: Path):
    text = reference.read_text()
    # Native pointers require stronger alignment than DS pointers. Leave the
    # 16-byte allocation prefix and serialized game structures unchanged.
    for old, new, count in [
        ('preSize, 4);', 'preSize, sizeof(void *));', 1),
        ('        4);', '        sizeof(void *));', 1),
        ('templates[i].size, 4);', 'templates[i].size, sizeof(void *));', 2),
        ('child, size, 4);', 'child, size, sizeof(void *));', 1),
        ('child, size, -4);', 'child, size, -(s32)sizeof(void *));', 1),
        ('size, 4, heapID);', 'size, sizeof(void *), heapID);', 1),
        ('size, -4, heapID);', 'size, -(s32)sizeof(void *), heapID);', 1),
    ]:
        text = replace_exact(text, old, new, count)
    output.write_text(text)


def prepare_sdk_heap(reference: Path, output: Path):
    text = reference.read_text()
    text = replace_exact(text, '#define MIN_ALIGNMENT           4',
                         '#define MIN_ALIGNMENT           ((int)sizeof(void *))')
    old = '    NNS_ASSERT(alignment % MIN_ALIGNMENT == 0);'
    new = ('    /* Preserve DS callers requesting +/-4 while aligning native headers. */\n'
           '    if (alignment == 4) alignment = MIN_ALIGNMENT;\n'
           '    if (alignment == -4) alignment = -MIN_ALIGNMENT;\n'
           + old)
    text = replace_exact(text, old, new, 2)
    output.write_text(text)
