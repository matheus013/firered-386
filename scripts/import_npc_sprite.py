"""Import a 16x32 overworld NPC sprite from pokeemerald or HNS_modern into Super FireRed. Idempotent.

FireRed draws townspeople with four shared palettes (blue, pink, green, white) in fixed slots and has a
single "special" slot per map, so, like FireRed's own Ruby/Sapphire Brendan, an imported sprite is
redrawn on whichever of the four palettes fits it best: every used colour goes to the nearest colour
of that palette (index 0 stays transparent). The colour error is reported so the choice is auditable.
A 9-frame Emerald sheet gets a tenth frame (a copy of frame 0) because FireRed's standard animation
table also uses frame 9 (raise hand).
"""
import argparse
import json
from pathlib import Path
import re
from PIL import Image

PALETTES = {  # FireRed tag -> (palette file, slot)
    'OBJ_EVENT_PAL_TAG_NPC_BLUE': ('npc_blue.pal', 'PALSLOT_NPC_1'),
    'OBJ_EVENT_PAL_TAG_NPC_PINK': ('npc_pink.pal', 'PALSLOT_NPC_2'),
    'OBJ_EVENT_PAL_TAG_NPC_GREEN': ('npc_green.pal', 'PALSLOT_NPC_3'),
    'OBJ_EVENT_PAL_TAG_NPC_WHITE': ('npc_white.pal', 'PALSLOT_NPC_4'),
}


def jasc(path):
    lines = Path(path).read_text().split()
    if lines[:3] != ['JASC-PAL', '0100', '16']:
        raise ValueError(f'Not a 16-colour JASC palette: {path}')
    values = list(map(int, lines[3:3 + 48]))
    return [tuple(values[i:i + 3]) for i in range(0, 48, 3)]


def remap(image, source, target):
    """Map each used non-transparent index to the nearest target colour 1-15; return mapping and error."""
    counts = {}
    for value in image.get_flattened_data():
        counts[value] = counts.get(value, 0) + 1
    mapping, cost, worst = {0: 0}, 0, 0
    for index, count in counts.items():
        if index == 0:
            continue
        r, g, b = source[index]
        best = min(range(1, 16), key=lambda i: (target[i][0] - r) ** 2 + (target[i][1] - g) ** 2 + (target[i][2] - b) ** 2)
        distance = ((target[best][0] - r) ** 2 + (target[best][1] - g) ** 2 + (target[best][2] - b) ** 2) ** 0.5
        mapping[index] = best
        cost += count * distance ** 2
        worst = max(worst, distance)
    return mapping, cost, worst


def insert_once(path, marker, text, anchor=None, before=True):
    content = path.read_text(encoding='utf-8')
    if marker in content:
        return
    if anchor is None:
        content = content.rstrip('\n') + '\n\n' + text + '\n'
    else:
        if content.count(anchor) != 1:
            raise ValueError(f'{path}: anchor not unique: {anchor!r}')
        content = content.replace(anchor, text + anchor if before else anchor + text)
    path.write_text(content, encoding='utf-8', newline='\n')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--png', type=Path, required=True)
    p.add_argument('--pal', type=Path, required=True, help='source JASC palette the sprite is drawn with')
    p.add_argument('--name', required=True, help='file name, e.g. twin')
    p.add_argument('--const', required=True, help='OBJ_EVENT_GFX_ suffix, e.g. TWIN')
    p.add_argument('--symbol', required=True, help='C name part, e.g. Twin')
    p.add_argument('--source', required=True, help='provenance note')
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    g = a.game
    image = Image.open(a.png)
    if image.mode != 'P' or image.height != 32 or image.width % 16 or max(image.get_flattened_data()) > 15:
        raise ValueError('Expected a 4bpp indexed 16x32 frame sheet')
    frames = image.width // 16
    if frames == 9:
        sheet = Image.new('P', (160, 32))
        sheet.paste(image, (0, 0))
        sheet.paste(image.crop((0, 0, 16, 32)), (144, 0))
        image = sheet
    elif frames != 10:
        raise ValueError(f'{frames} frames; expected 9 or 10')
    source = jasc(a.pal)
    choices = {}
    for tag, (file, slot) in PALETTES.items():
        target = jasc(g / 'graphics/object_events/palettes' / file)
        choices[tag] = (*remap(image, source, target), target, slot)
    tag = min(choices, key=lambda t: choices[t][1])
    mapping, cost, worst, target, slot = choices[tag]
    out = Image.new('P', image.size)
    out.putdata([mapping[v] for v in image.get_flattened_data()])
    out.putpalette([c for colour in target for c in colour])
    png = g / f'graphics/object_events/pics/people/{a.name}.png'
    out.save(png, bits=4)

    sym, const = a.symbol, 'OBJ_EVENT_GFX_' + a.const
    events = g / 'include/constants/event_objects.h'
    text = events.read_text(encoding='utf-8')
    if f'#define {const} ' not in text:
        count = int(re.search(r'#define NUM_OBJ_EVENT_GFX\s+(\d+)', text)[1])
        text = re.sub(r'\n\n#define NUM_OBJ_EVENT_GFX\s+\d+',
                      f'\n#define {const} {count}\n\n#define NUM_OBJ_EVENT_GFX     {count + 1}', text, count=1)
        events.write_text(text, encoding='utf-8', newline='\n')
    data = g / 'src/data/object_events'
    insert_once(data / 'object_event_graphics.h', f'gObjectEventPic_{sym}[]',
                f'const u16 gObjectEventPic_{sym}[] = INCBIN_U16("graphics/object_events/pics/people/{a.name}.4bpp"); // {a.source}')
    insert_once(data / 'object_event_pic_tables.h', f'sPicTable_{sym}[]',
                f'static const struct SpriteFrameImage sPicTable_{sym}[] = {{\n'
                + ''.join(f'    overworld_frame(gObjectEventPic_{sym}, 2, 4, {i}),\n' for i in range(10)) + '};')
    insert_once(data / 'object_event_graphics_info.h', f'gObjectEventGraphicsInfo_{sym} =',
                f'// {a.source}; redrawn on FireRed palette {tag}.\n'
                f'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_{sym} = {{\n'
                f'    .tileTag = TAG_NONE,\n    .paletteTag = {tag},\n    .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,\n'
                f'    .size = 256,\n    .width = 16,\n    .height = 32,\n    .paletteSlot = {slot},\n'
                f'    .shadowSize = SHADOW_SIZE_M,\n    .inanimate = FALSE,\n    .disableReflectionPaletteLoad = FALSE,\n'
                f'    .tracks = TRACKS_FOOT,\n    .oam = &gObjectEventBaseOam_16x32,\n'
                f'    .subspriteTables = gObjectEventSpriteOamTables_16x32,\n    .anims = sAnimTable_Standard,\n'
                f'    .images = sPicTable_{sym},\n    .affineAnims = gDummySpriteAffineAnimTable,\n}};')
    pointers = data / 'object_event_graphics_info_pointers.h'
    insert_once(pointers, f'gObjectEventGraphicsInfo_{sym};', f'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_{sym};\n',
                anchor='\nconst struct ObjectEventGraphicsInfo *const gObjectEventGraphicsInfoPointers')
    insert_once(pointers, f'[{const}]', f'    [{const}] = &gObjectEventGraphicsInfo_{sym},\n', anchor='};', before=True)
    insert_once(g / 'spritesheet_rules.mk', f'people/{a.name}.4bpp',
                f'$(OBJEVENTGFXDIR)/people/{a.name}.4bpp: %.4bpp: %.png\n\t$(GFX) $< $@ -mwidth 2 -mheight 4')

    report = json.loads(a.report.read_text(encoding='utf-8')) if a.report.exists() else {'sprites': {}}
    report['sprites'][const] = {
        'source': a.source, 'source_png': str(a.png).replace('\\', '/').split('map-references/')[-1],
        'palette_tag': tag, 'frames_in_source': frames, 'mean_squared_rgb_error_per_pixel':
        round(cost / max(1, sum(1 for v in image.get_flattened_data() if v)), 2), 'worst_colour_distance': round(worst, 1),
        'alternatives': {t: round(c[1]) for t, c in choices.items()}}
    a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(const, tag, 'worst colour distance', round(worst, 1))


if __name__ == '__main__':
    main()
