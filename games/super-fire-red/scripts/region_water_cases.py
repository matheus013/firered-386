"""Choose unobstructed coastal test cells from installed map geometry."""
import json
import struct


def water_cases(game, layouts, tilesets):
    def grid(name):
        header = json.loads((game / 'data/maps' / name / 'map.json').read_text())
        layout = layouts[header['layout']]
        blocks = [v for (v,) in struct.iter_unpack('<H', (game / layout['blockdata_filepath']).read_bytes())]
        w, h = layout['width'], layout['height']
        occupied = {(o['x'], o['y']) for o in header.get('object_events', [])}
        water, land = set(), set()
        for i, v in enumerate(blocks):
            pos = (i % w, i // w)
            if v >> 10 & 3 or pos in occupied:
                continue
            tile = v & 1023
            behavior = (tilesets[layout['primary_tileset']][tile] if tile < 640
                        else tilesets[layout['secondary_tileset']][tile - 640]) & 511
            if behavior in (0x10, 0x12, 0x15):
                water.add(pos)
            elif behavior in (0, 0x17, 0x21):
                land.add(pos)
        return header, w, h, water, land

    result = []
    name = 'Hoenn_DewfordTown'
    header, w, h, water, land = grid(name)
    for x, y in sorted(land):
        for dx, dy, key, back in ((1, 0, 'RIGHT', 'LEFT'), (-1, 0, 'LEFT', 'RIGHT'),
                                  (0, 1, 'DOWN', 'UP'), (0, -1, 'UP', 'DOWN')):
            if (x + dx, y + dy) in water:
                result.append((header['id'], x, y, [(key, 'SURF'), (back, 'POS')],
                               [(header['id'], {(x + dx, y + dy)}), (header['id'], {(x, y)})], [1, 0]))
                break
        if result:
            break
    if not result:
        raise ValueError('No unobstructed Dewford shore found')

    for source, dest in (('Route104', 'Route105'), ('Route105', 'Route106'),
                         ('Route106', 'DewfordTown'), ('DewfordTown', 'Route107'),
                         ('Route121', 'Route122'), ('Route122', 'Route123')):
        a, w, h, water, _ = grid('Hoenn_' + source)
        b, bw, bh, target, _ = grid('Hoenn_' + dest)
        connection = next(c for c in a['connections'] if c['map'] == b['id'])
        direction, offset = connection['direction'], connection['offset']
        found = False
        for x, y in sorted(water):
            if direction == 'down' and y == h - 1:
                cell, key, back = (x - offset, 0), 'DOWN', 'UP'
            elif direction == 'up' and y == 0:
                cell, key, back = (x - offset, bh - 1), 'UP', 'DOWN'
            elif direction == 'right' and x == w - 1:
                cell, key, back = (0, y - offset), 'RIGHT', 'LEFT'
            elif direction == 'left' and x == 0:
                cell, key, back = (bw - 1, y - offset), 'LEFT', 'RIGHT'
            else:
                continue
            if cell in target:
                result.append((a['id'], x, y, [(key, 'MAP'), (back, 'MAP')],
                               [(b['id'], {cell}), (a['id'], {(x, y)})], [1, 1]))
                found = True
                break
        if not found:
            raise ValueError(f'No unobstructed water connection: {source} -> {dest}')
    return result
