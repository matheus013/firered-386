"""Install the Johto pilot (New Bark Town, Route 29 and their six interiors) from HNS_modern. Idempotent.

HNS_modern already uses FireRed's partition (640 primary tiles/metatiles, 7 primary palettes), so tiles,
palettes, metatiles and blocks are copied unchanged. Only the 16-bit Emerald-style attributes are converted
to FireRed's 32-bit format, with an explicit rule per behavior; metatiles not used by the installed maps
or borders are zeroed. Warps and connections to maps outside the pilot are dropped and the remaining
warp indices renumbered everywhere. Terrain, warps, doors and tile animation only: no events or scripts.

Art by the HNS team and the tileset artists credited in docs/CREDITS.md. HNS_modern has no license file.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
from region_tilesets import behavior_names, convert_attributes

MAPS = ['NewBarkTown', 'Route29', 'NewBarkTown_PlayersHouse_1F', 'NewBarkTown_PlayersHouse_2F',
        'NewBarkTown_House1', 'NewBarkTown_House2', 'NewBarkTown_Lab', 'Gate_Route29_Route46']
TILESETS = {  # HNS symbol -> (FireRed name, HNS folder, FireRed folder, animation init)
    'gTileset_Johto_General': ('JohtoPilotGeneral', 'primary/johto_general', 'primary/johto_pilot_general', 'InitTilesetAnim_JohtoPilot'),
    'gTileset_NewBarkTown': ('JohtoPilotNewBarkTown', 'secondary/new_bark_town', 'secondary/johto_pilot_new_bark_town', 'NULL'),
    'gTileset_Johto_Building': ('JohtoPilotBuilding', 'primary/johto_building', 'primary/johto_pilot_building', 'NULL'),
    'gTileset_PlayersHouse': ('JohtoPilotPlayersHouse', 'secondary/players_house', 'secondary/johto_pilot_players_house', 'NULL'),
    'gTileset_House_Lab': ('JohtoPilotHouseLab', 'secondary/house_lab', 'secondary/johto_pilot_house_lab', 'NULL'),
    'gTileset_Gate_Standard': ('JohtoPilotGateStandard', 'secondary/gate_standard', 'secondary/johto_pilot_gate_standard', 'NULL'),
}
# FireRed attribute without layer bits: behavior | terrain << 9 | encounter << 24, taken from the pattern
# FireRed's own tilesets use most for that behavior.
RULES = {
    'MB_NORMAL': (0x00000000, 'normal'),
    'MB_TALL_GRASS': (0x01000202, 'tall grass, grass terrain, land encounters'),
    'MB_OCEAN_WATER': (0x02000415, 'ocean water, water terrain, water encounters'),
    'MB_SAND': (0x00000021, 'same name'),
    'MB_JUMP_EAST': (0x00000038, 'same name'), 'MB_JUMP_WEST': (0x00000039, 'same name'),
    'MB_JUMP_SOUTH': (0x0000003B, 'same name'),
    'MB_ROCK_STAIRS': (0x0000002A, 'same name'),
    'MB_IMPASSABLE_EAST': (0x00000030, 'same name'), 'MB_IMPASSABLE_WEST': (0x00000031, 'same name'),
    'MB_IMPASSABLE_NORTH': (0x00000032, 'same name'), 'MB_IMPASSABLE_SOUTH': (0x00000033, 'same name'),
    'MB_COUNTER': (0x00000080, 'same name'), 'MB_BOOKSHELF': (0x00000081, 'same name'),
    'MB_PC': (0x00000083, 'same name'), 'MB_TELEVISION': (0x00000086, 'same name'),
    'MB_SOUTH_ARROW_WARP': (0x00000065, 'same name'),
    'MB_UP_RIGHT_STAIR_WARP': (0x0000006C, 'same name (FireRed-style stairs)'),
    'MB_DOWN_LEFT_STAIR_WARP': (0x0000006F, 'same name (FireRed-style stairs)'),
    'MB_ANIMATED_DOOR': (0x00000069, 'FR MB_WARP_DOOR; door graphics registered for the New Bark pair'),
    'MB_NON_ANIMATED_DOOR': (0x00000060, 'FR MB_CAVE_DOOR, accepted by MetatileBehavior_IsNonAnimDoor'),
    'MB_SHORT_GRASS': (0x00000000, 'FireRed has no short grass (only a walking visual effect in HNS)'),
    'MB_HEADBUTT_TREE': (0x00000000, 'FireRed has no Headbutt; trees keep their collision'),
    'MB_HOLDS_SMALL_DECORATION': (0x00000000, 'secret-base slot, absent from FireRed'),
    'MB_HOLDS_LARGE_DECORATION': (0x00000000, 'secret-base slot, absent from FireRed'),
    'MB_SECRET_BASE_SPOT_TREE_RIGHT': (0x00000000, 'secret-base spot, absent from FireRed'),
    'MB_UNUSED_81': (0x00000000, 'no code uses it in HNS'),
    'MB_SLOT_MACHINE': (0x00000000, 'no code uses it in HNS'),
    'MB_SIDEWAYS_STAIRS_LEFT_SIDE_TOP': (0x00000000, 'FireRed has no sideways stairs; walks straight'),
}
MUSIC = {'MUS_HG_NEW_BARK': 'MUS_PALLET', 'MUS_HG_ROUTE29': 'MUS_ROUTE1', 'MUS_HG_ELM_LAB': 'MUS_OAK_LAB'}
DOORS = [  # metatile, HNS door image, FireRed name, palette (all 8 entries)
    (0x2CB, 'cerulean.png', 'new_bark_blue', 7),
    (0x309, 'NewBarkTown_Door_Yellow.png', 'new_bark_yellow', 5),
]
MARK = '// Johto pilot (scripts/install_johto_pilot.py)'


def target_map(name):
    return 'Johto_' + name


def append_once(path, text):
    content = path.read_text(encoding='utf-8')
    if MARK not in content:
        path.write_text(content.rstrip('\n') + '\n\n' + MARK + '\n' + text, encoding='utf-8', newline='\n')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--hns', type=Path, required=True)
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    h, g = a.hns, a.game
    names = behavior_names(h / 'include/constants/metatile_behaviors.h')
    layouts = {l['id']: l for l in json.loads((h / 'data/layouts/layouts.json').read_text())['layouts'] if 'id' in l}
    headers = {m: json.loads((h / f'data/maps/{m}/map.json').read_text()) for m in MAPS}
    source_ids = {headers[m]['id']: m for m in MAPS}
    map_constant = {m: 'MAP_JOHTO_' + headers[m]['id'][len('MAP_'):] for m in MAPS}

    # Warps that leave the pilot are dropped; build old -> new index per map before rewriting destinations.
    kept, renumber, dropped = {}, {}, []
    for m in MAPS:
        kept[m], renumber[m] = [], {}
        for i, w in enumerate(headers[m]['warp_events']):
            if w['dest_map'] in source_ids:
                renumber[m][i] = len(kept[m])
                kept[m].append(w)
            else:
                dropped.append({'map': m, 'index': i, 'x': w['x'], 'y': w['y'], 'destination': w['dest_map']})

    # Used metatiles per tileset, from the blocks and borders actually installed.
    used = {s: set() for s in TILESETS}
    for m in MAPS:
        layout = layouts[headers[m]['layout']]
        for f in ('blockdata_filepath', 'border_filepath'):
            for (v,) in struct.iter_unpack('<H', (h / layout[f]).read_bytes()):
                tile = v & 0x3FF
                symbol = layout['primary_tileset'] if tile < 640 else layout['secondary_tileset']
                used[symbol].add(tile if tile < 640 else tile - 640)

    report = {'status': 'JOHTO_PILOT_INSTALLED', 'source': 'resetes12/HNS_modern', 'tilesets': {}, 'dropped_warps': dropped,
              'behavior_rules': {k: {'firered_attribute': hex(v), 'reason': r} for k, (v, r) in RULES.items()}}
    graphics, metatiles, tileset_headers = [], [], []
    for symbol, (name, src, dst, anim) in TILESETS.items():
        source, target = h / 'data/tilesets' / src, g / 'data/tilesets' / dst
        if target.exists():
            shutil.rmtree(target)
        (target / 'palettes').mkdir(parents=True)
        shutil.copyfile(source / 'tiles.png', target / 'tiles.png')
        shutil.copyfile(source / 'metatiles.bin', target / 'metatiles.bin')
        # HNS ships 16 palettes for primaries and 13 for secondaries; FireRed loads at most slots 0-12.
        palettes = sorted(int(f.stem) for f in (source / 'palettes').glob('*.pal'))
        if palettes != list(range(len(palettes))) or len(palettes) < 13:
            raise ValueError(f'{symbol}: unexpected palette files {palettes}')
        for i in palettes:
            shutil.copyfile(source / f'palettes/{i:02}.pal', target / f'palettes/{i:02}.pal')
        raw = (source / 'metatile_attributes.bin').read_bytes()
        out, behaviors = bytearray(len(raw) * 2), {}
        for index in sorted(used[symbol]):
            value = struct.unpack_from('<H', raw, index * 2)[0]
            behavior = names.get(value & 255)
            if behavior not in RULES:
                raise ValueError(f'{symbol}: behavior requires review: {behavior or value & 255}')
            out[index * 4:index * 4 + 4] = convert_attributes(struct.pack('<H', value), {value & 255: RULES[behavior][0]})
            behaviors[behavior] = behaviors.get(behavior, 0) + 1
        (target / 'metatile_attributes.bin').write_bytes(out)
        report['tilesets'][symbol] = {'firered': 'gTileset_' + name, 'metatiles_converted': len(used[symbol]), 'behaviors': behaviors,
                                      'sha256': {f: hashlib.sha256((target / f).read_bytes()).hexdigest()
                                                 for f in ('tiles.png', 'metatiles.bin', 'metatile_attributes.bin')}}
        base = f'data/tilesets/{dst}'
        graphics.append(f'const u32 gTilesetTiles_{name}[] = INCBIN_U32("{base}/tiles.4bpp.lz");\n'
                        f'const u16 gTilesetPalettes_{name}[][16] = {{\n'
                        + ''.join(f'    INCBIN_U16("{base}/palettes/{i:02}.gbapal"),\n' for i in palettes) + '};\n')
        metatiles.append(f'const u16 gMetatiles_{name}[] = INCBIN_U16("{base}/metatiles.bin");\n'
                         f'const u32 gMetatileAttributes_{name}[] = INCBIN_U32("{base}/metatile_attributes.bin");\n')
        tileset_headers.append(f'const struct Tileset gTileset_{name} =\n{{\n    .isCompressed = TRUE,\n'
                               f'    .isSecondary = {"FALSE" if src.startswith("primary") else "TRUE"},\n'
                               f'    .tiles = gTilesetTiles_{name},\n    .palettes = gTilesetPalettes_{name},\n'
                               f'    .metatiles = gMetatiles_{name},\n    .metatileAttributes = gMetatileAttributes_{name},\n'
                               f'    .callback = {anim},\n}};\n')
    append_once(g / 'src/data/tilesets/graphics.h', '\n'.join(graphics))
    append_once(g / 'src/data/tilesets/metatiles.h', '\n'.join(metatiles))
    append_once(g / 'src/data/tilesets/headers.h', '\n'.join(tileset_headers))

    # Tile animation: the two HNS Johto General animations whose tiles the pilot uses (sand/water edge, flowers).
    anim_dir = g / 'data/tilesets/primary/johto_pilot_general/anim'
    for folder, count in (('flower', 5), ('water_current_landwatersedge', 8)):
        (anim_dir / folder).mkdir(parents=True, exist_ok=True)
        for i in range(count):
            shutil.copyfile(h / f'data/tilesets/primary/johto_general/anim/{folder}/{i}.png', anim_dir / folder / f'{i}.png')
    base = 'data/tilesets/primary/johto_pilot_general/anim'
    (g / 'src/data/tilesets/johto_pilot_anims.h').write_text(
        '// Johto pilot (New Bark Town, Route 29). Source: HNS_modern TilesetAnim_JohtoGeneral, same schedule.\n'
        + ''.join(f'static const u16 sJohtoFlower{i}[] = INCBIN_U16("{base}/flower/{i}.4bpp");\n' for i in range(5))
        + ''.join(f'static const u16 sJohtoWater{i}[] = INCBIN_U16("{base}/water_current_landwatersedge/{i}.4bpp");\n' for i in range(8))
        + 'static const u16 *const sJohtoFlower[] = {' + ', '.join(f'sJohtoFlower{i}' for i in range(5)) + '};\n'
        + 'static const u16 *const sJohtoWater[] = {' + ', '.join(f'sJohtoWater{i}' for i in range(8)) + '};\n'
        + 'static void TilesetAnim_JohtoPilot(u16 timer)\n{\n'
        + '    // Sand/water edge: first 18 tiles of each water frame. The HNS land/water edge animation is disabled\n'
        + '    // and its waterfall tiles are not used by the pilot maps.\n'
        + '    if (timer % 16 == 0)\n'
        + '        AppendTilesetAnimToBuffer(sJohtoWater[(timer / 16) % ARRAY_COUNT(sJohtoWater)], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP(416)), 18 * TILE_SIZE_4BPP);\n'
        + '    if (timer % 16 == 2)\n'
        + '        AppendTilesetAnimToBuffer(sJohtoFlower[(timer >> 4) % ARRAY_COUNT(sJohtoFlower)], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP(508)), 4 * TILE_SIZE_4BPP);\n'
        + '}\n\nvoid InitTilesetAnim_JohtoPilot(void)\n{\n    sPrimaryTilesetAnimCounter = 0;\n'
        + '    sPrimaryTilesetAnimCounterMax = 256;\n    sPrimaryTilesetAnimCallback = TilesetAnim_JohtoPilot;\n}\n',
        encoding='utf-8', newline='\n')
    anims_c = g / 'src/tileset_anims.c'
    if '#include "data/tilesets/johto_pilot_anims.h"' not in anims_c.read_text(encoding='utf-8'):
        anims_c.write_text(anims_c.read_text(encoding='utf-8').rstrip('\n') + '\n#include "data/tilesets/johto_pilot_anims.h"\n', encoding='utf-8', newline='\n')
    anims_h = g / 'include/tileset_anims.h'
    text = anims_h.read_text(encoding='utf-8')
    if 'InitTilesetAnim_JohtoPilot' not in text:
        anims_h.write_text(text.replace('#endif // GUARD_TILESET_ANIMS_H', 'void InitTilesetAnim_JohtoPilot(void);\n\n#endif // GUARD_TILESET_ANIMS_H'), encoding='utf-8', newline='\n')

    # Doors of the New Bark pair, same 1x1 frame layout in both engines.
    (g / 'graphics/door_anims/johto').mkdir(parents=True, exist_ok=True)
    lines = ['// Johto pilot doors. Source: HNS_modern src/field_door.c (blue reuses its cerulean.png).']
    for metatile, image, name, pal in DOORS:
        shutil.copyfile(h / 'graphics/door_anims' / image, g / f'graphics/door_anims/johto/{name}.png')
        lines.append(f'static const u8 sJohtoDoorTiles_{name}[] = INCBIN_U8("graphics/door_anims/johto/{name}.4bpp");')
        lines.append(f'static const u8 sJohtoDoorPalettes_{name}[] = {{{", ".join([str(pal)] * 8)}}};')
    lines.append('static const struct DoorGraphics sJohtoPilotDoors[] = {')
    lines += [f'    {{{hex(m).upper().replace("X", "x")}, DOOR_SOUND_NORMAL, DOOR_SIZE_1x1, sJohtoDoorTiles_{n}, sJohtoDoorPalettes_{n}}},' for m, _, n, _ in DOORS]
    lines += ['    {}', '};']
    (g / 'src/data/johto_pilot_doors.h').write_text('\n'.join(lines) + '\n', encoding='utf-8', newline='\n')
    doors = g / 'src/data/door_tilesets.h'
    text = doors.read_text(encoding='utf-8')
    if 'sJohtoPilotDoors' not in text:
        text = text.replace('#include "hoenn_pilot_doors.h"\n', '#include "hoenn_pilot_doors.h"\nextern const struct Tileset gTileset_JohtoPilotGeneral;\n'
                            'extern const struct Tileset gTileset_JohtoPilotNewBarkTown;\n#include "johto_pilot_doors.h"\n')
        text = text.replace('    {NULL, NULL, NULL}', '    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotNewBarkTown, sJohtoPilotDoors},\n    {NULL, NULL, NULL}')
        doors.write_text(text, encoding='utf-8', newline='\n')

    # Layouts, maps, group 44, script includes.
    layouts_path = g / 'data/layouts/layouts.json'
    fr_layouts = json.loads(layouts_path.read_text(encoding='utf-8'))
    known = {l.get('id') for l in fr_layouts['layouts']}
    groups_path = g / 'data/maps/map_groups.json'
    groups = json.loads(groups_path.read_text(encoding='utf-8'))
    if 'gMapGroup_Johto' not in groups['group_order']:
        groups['group_order'].append('gMapGroup_Johto')
    groups['gMapGroup_Johto'] = [target_map(m) for m in MAPS]
    scripts_path = g / 'data/event_scripts.s'
    scripts = scripts_path.read_text(encoding='utf-8')
    for m in MAPS:
        header, layout = headers[m], layouts[headers[m]['layout']]
        name = target_map(m)
        layout_id = 'LAYOUT_JOHTO_' + header['layout'][len('LAYOUT_'):]
        (g / f'data/layouts/{name}').mkdir(parents=True, exist_ok=True)
        shutil.copyfile(h / layout['blockdata_filepath'], g / f'data/layouts/{name}/map.bin')
        shutil.copyfile(h / layout['border_filepath'], g / f'data/layouts/{name}/border.bin')
        if layout_id not in known:
            fr_layouts['layouts'].append({
                'id': layout_id, 'name': name + '_Layout', 'width': layout['width'], 'height': layout['height'],
                'border_width': 2, 'border_height': 2,
                'primary_tileset': 'gTileset_' + TILESETS[layout['primary_tileset']][0],
                'secondary_tileset': 'gTileset_' + TILESETS[layout['secondary_tileset']][0],
                'border_filepath': f'data/layouts/{name}/border.bin', 'blockdata_filepath': f'data/layouts/{name}/map.bin'})
        warps = [{'x': w['x'], 'y': w['y'], 'elevation': w['elevation'], 'dest_map': map_constant[source_ids[w['dest_map']]],
                  'dest_warp_id': str(renumber[source_ids[w['dest_map']]][int(w['dest_warp_id'])])} for w in kept[m]]
        connections = [{'map': map_constant[source_ids[c['map']]], 'offset': c['offset'], 'direction': c['direction']}
                       for c in header.get('connections') or [] if c['map'] in source_ids]
        out = {'id': map_constant[m], 'name': name, 'layout': layout_id,
               'music': MUSIC.get(header['music'], header['music']),
               # Johto sections are appended to FireRed's list (JOHTO_MAPSEC_START); same names as HNS.
               'region_map_section': header['region_map_section'],
               'requires_flash': header['requires_flash'], 'weather': header['weather'],
               'map_type': 'MAP_TYPE_INDOOR' if header['map_type'] == 'MAP_TYPE_NONE' else header['map_type'],
               'allow_cycling': header['allow_cycling'], 'allow_escaping': header['allow_escaping'],
               'allow_running': header['allow_running'], 'show_map_name': header['show_map_name'], 'floor_number': 0,
               'battle_scene': header['battle_scene'], 'connections': connections or None,
               'object_events': [], 'warp_events': warps, 'coord_events': [], 'bg_events': []}
        (g / f'data/maps/{name}').mkdir(parents=True, exist_ok=True)
        (g / f'data/maps/{name}/map.json').write_text(json.dumps(out, indent=2) + '\n', encoding='utf-8', newline='\n')
        (g / f'data/maps/{name}/scripts.inc').write_text(
            f'@ Terrain-only pilot. HNS events are not imported.\n{name}_MapScripts::\n\t.byte 0\n', encoding='utf-8', newline='\n')
        if connections and name not in groups['connections_include_order']:
            groups['connections_include_order'].append(name)
        line = f'\t.include "data/maps/{name}/scripts.inc"\n'
        if line not in scripts:
            last = max((l for l in scripts.splitlines(True) if l.startswith('\t.include "data/maps/Hoenn_') or l.startswith('\t.include "data/maps/Johto_')),
                       key=scripts.index)
            scripts = scripts.replace(last, last + line, 1)
    layouts_path.write_text(json.dumps(fr_layouts, indent=2) + '\n', encoding='utf-8', newline='\n')
    groups_path.write_text(json.dumps(groups, indent=2) + '\n', encoding='utf-8', newline='\n')
    scripts_path.write_text(scripts, encoding='utf-8', newline='\n')
    report['maps'] = {target_map(m): {'id': map_constant[m], 'number': i} for i, m in enumerate(MAPS)}
    a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('gMapGroup_Johto =', groups['gMapGroup_Johto'], '| dropped warps:', len(dropped))


if __name__ == '__main__':
    main()
