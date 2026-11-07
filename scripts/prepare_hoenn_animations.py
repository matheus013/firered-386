"""Stage General animations needed by Littleroot/Route101; never install assets."""
import argparse
import hashlib
import json
from pathlib import Path
import re
from PIL import Image
from prepare_hoenn_doors import encode_4bpp, decode_4bpp


def read_tracks(source):
    for function, required in (
        ('InitTilesetAnim_General', 'sPrimaryTilesetAnimCounterMax = 256;'),
        ('InitTilesetAnim_Petalburg', 'sSecondaryTilesetAnimCallback = NULL;')):
        body = re.search(r'void ' + function + r'\(void\)\s*\{([^}]+)\}', source)
        if not body or required not in body[1]:
            raise ValueError(f'Unrecognized initializer: {function}')
    tracks = []
    for name, phase in [('Flower', 0), ('Water', 1), ('SandWaterEdge', 2),
                        ('Waterfall', 3), ('LandWaterEdge', 4)]:
        prefix = 'gTilesetAnims_General_' + name
        table = re.search(r'const u16 \*const ' + prefix + r'\[\] = \{(.*?)\};', source, re.S)
        queue = re.search(r'AppendTilesetAnimToBuffer\(' + prefix +
                          r'\[i\], \(u16 \*\)\(BG_VRAM \+ TILE_OFFSET_4BPP\((\d+)\)\), (\d+) \* TILE_SIZE_4BPP\);', source)
        if not table or not queue or not re.search(
                rf'if \(timer % 16 == {phase}\)\s+QueueAnimTiles_General_{name}\(timer / 16\);', source):
            raise ValueError(f'Unrecognized source animation: {name}')
        frames = re.findall(prefix + r'_Frame(\d+)', table[1])
        if not frames:
            raise ValueError('Empty animation sequence')
        paths = {}
        for frame in set(frames):
            match = re.search(prefix + '_Frame' + frame + r'\[\] = INCGFX_U16\("([^"]+)", "\.4bpp"\);', source)
            if not match:
                raise ValueError('Missing frame definition')
            paths[frame] = match[1]
        tracks.append(dict(name=name, phase=phase, start=int(queue[1]), count=int(queue[2]),
                           sequence=frames, paths=paths))
    return tracks


def required_tracks(tracks, used_tiles):
    return [t for t in tracks if used_tiles.intersection(range(t['start'], t['start']+t['count']))]


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for flag in ('source', 'audit', 'out', 'report'):
        p.add_argument('--' + flag, type=Path, required=True)
    a = p.parse_args()
    text = (a.source / 'src/tileset_anims.c').read_text()
    audit = json.loads(a.audit.read_text())
    if {m['map'] for m in audit} != {'LittlerootTown', 'Route101'}:
        raise ValueError('Only the two pilot exteriors are supported')
    used = {tile for m in audit for s in m['tilesets'] for tile in s['tile_ids_used']}
    tracks = read_tracks(text)
    selected = required_tracks(tracks, used)
    outputs, evidence, declarations, calls = {}, [], [], []
    for track in selected:
        name = track['name']
        frames = []
        for frame, relative in sorted(track['paths'].items()):
            with Image.open(a.source / relative) as image:
                data = encode_4bpp(image)
                if len(data) != track['count'] * 32:
                    raise ValueError('Frame size differs from DMA size')
                if decode_4bpp(data, *image.size).tobytes() != image.tobytes():
                    raise ValueError('Animation pixels changed')
            filename = f'{name}_{frame}.4bpp'
            outputs[filename] = data
            frames.append(dict(file=filename, bytes=len(data), sha256=hashlib.sha256(data).hexdigest()))
            declarations.append(f'static const u16 sHoenn{name}{frame}[] = INCBIN_U16("data/tilesets/primary/hoenn_pilot/anim/{filename}");')
        sequence = ', '.join(f'sHoenn{name}{f}' for f in track['sequence'])
        declarations.append(f'static const u16 *const sHoenn{name}[] = {{{sequence}}};')
        calls.append(f'    if (timer % 16 == {track["phase"]})\n        AppendTilesetAnimToBuffer(sHoenn{name}[(timer / 16) % ARRAY_COUNT(sHoenn{name})], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP({track["start"]})), {track["count"]} * TILE_SIZE_4BPP);')
        evidence.append({k: v for k, v in track.items() if k != 'paths'} | {'frames': frames})
    snippet = '// STAGING ONLY: include in tileset_anims.c after AppendTilesetAnimToBuffer.\n'
    snippet += '\n'.join(declarations) + '\nstatic void TilesetAnim_HoennPilot(u16 timer)\n{\n'
    snippet += '\n'.join(calls) + '\n}\n'
    snippet += '''void InitTilesetAnim_HoennPilot(void)
{
    sPrimaryTilesetAnimCounter = 0;
    sPrimaryTilesetAnimCounterMax = 256;
    sPrimaryTilesetAnimCallback = TilesetAnim_HoennPilot;
}
void InitTilesetAnim_HoennPetalburgPilot(void)
{
    sSecondaryTilesetAnimCounter = 0;
    sSecondaryTilesetAnimCounterMax = sPrimaryTilesetAnimCounterMax;
    sSecondaryTilesetAnimCallback = NULL;
}
'''
    a.out.mkdir(parents=True, exist_ok=False)
    for filename, data in outputs.items():
        (a.out / filename).write_bytes(data)
    (a.out / 'hoenn_anims.inc').write_text(snippet)
    a.report.write_text(json.dumps(dict(status='STAGED_NOT_INSTALLED',
        source_sha256=hashlib.sha256(text.encode()).hexdigest(), tracks=evidence,
        omitted=[t['name'] for t in tracks if t not in selected],
        limits='Indexed pixel roundtrip only. No DMA/VRAM or gameplay validation; pilot exteriors only.'), indent=2)+'\n')
    print(f'{len(selected)} animation tracks, {len(outputs)} frames staged')


if __name__ == '__main__':
    main()
