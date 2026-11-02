"""Exercise original event parser/getters against all members of a private NARC."""
import argparse
import hashlib
import json
import re
from pathlib import Path
import struct
import subprocess
from verify_pc_storage_init import narc_member


def decode(raw):
    offset = 0
    counts = []
    warps = []
    if not 16 <= len(raw) < 0x800:
        raise ValueError('Event member outside original buffer limit')
    for size in (20, 32, 12, 16):
        if offset + 4 > len(raw):
            raise ValueError('Missing event count')
        count, = struct.unpack_from('<I', raw, offset)
        offset += 4
        if count > (len(raw)-offset)//size:
            raise ValueError('Event count exceeds member')
        counts.append(count)
        if size == 12:
            warps = [struct.unpack_from('<HHHHI', raw, offset+i*size) for i in range(count)]
        offset += count*size
    if offset != len(raw):
        raise ValueError('Trailing event bytes')
    return counts, warps


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('hgss', 'probe', 'stack', 'archive', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--target', choices=['linux', 'windows'], default='linux')
    parser.add_argument('--sdl-include', type=Path)
    a = parser.parse_args()
    output = a.output.resolve()
    if any((p/'.git').exists() for p in (output, *output.parents)):
        raise ValueError('Private output must be outside Git repositories')
    output.mkdir(parents=True, exist_ok=False)
    archive = a.archive.read_bytes()
    # The canonical archive has BTAF as its first block; the shared decoder
    # validates all block extents independently for every member.
    if archive[16:20] != b'BTAF':
        raise ValueError('Expected canonical allocation block')
    members, = struct.unpack_from('<H', archive, 24)
    if not members:
        raise ValueError('Empty event archive')
    totals = [0]*4
    payloads = []
    for i in range(members):
        raw = narc_member(archive, i)
        counts, _ = decode(raw)
        totals = [x+y for x,y in zip(totals, counts)]
        (output/f'{i}.bin').write_bytes(raw)
        payloads.append(raw)
    # Only macro definitions in this generated header are unused by this probe.
    header = output/'fielddata/encountdata/s_enc_data.naix'
    header.parent.mkdir(parents=True)
    header.write_text('#pragma once\n')
    sdk = a.stack/'subprojects/libntr'
    nns = a.stack/'subprojects/libntrsystem'
    flags = ['gcc', '-std=gnu11', '-g', '-DSDK_PORT', '-DSDK_X86',
             '-DSDK_BUILD_LINUX', '-DSDK_64BIT', '-DSDK_VERSION_MAJOR=4',
             '-DSOULSILVER', '-DENGLISH', '-DSDK_FINALROM',
             '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections',
             '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
             '--param=asan-globals=0', '-include', str(a.probe/'hgss_backup.h')]
    if a.target == 'windows':
        flags[0] = 'x86_64-w64-mingw32-gcc'
        flags = [f.replace('-DSDK_BUILD_LINUX', '-DSDK_BUILD_WIN64') for f in flags
                 if f not in ('-fsanitize=address,undefined', '-fno-sanitize-recover=all', '--param=asan-globals=0')]
        flags += ['-static', '-DSDL_MAIN_HANDLED', '-fno-asynchronous-unwind-tables', '-fno-unwind-tables']
    includes = [output, a.hgss/'src', a.probe, a.probe/'hgss-include',
                a.probe/'hgss-include/library', sdk/'include', nns/'include',
                a.stack/'build/subprojects/libntr/gen', a.stack/'build/subprojects/libntr/gen/nitro/fx']
    if a.sdl_include:
        includes.append(a.sdl_include)
    flags += ['-I'+str(p) for p in includes]
    binary = output/('events-test.exe' if a.target == 'windows' else 'events-test')
    result = subprocess.run(flags+[str(Path(__file__).with_name('probe_map_events.c')), '-o', str(binary)], capture_output=True, text=True)
    (output/'build.log').write_text(result.stdout+result.stderr)
    result.check_returncode()
    directory = str(output)
    if a.target == 'windows':
        directory = subprocess.check_output(['wslpath', '-w', directory], text=True).strip()
    result = subprocess.run([str(binary), directory, str(members)], capture_output=True, text=True, timeout=60)
    (output/'run.log').write_text(result.stdout+result.stderr)
    result.check_returncode()
    mismatches = [i for i,raw in enumerate(payloads) if (output/f'{i}.out').read_bytes() != raw]
    headers = (a.hgss/'src/data/map_headers.h').read_text()
    constants = (a.hgss/'include/constants/maps.h').read_text()
    rooms = {}
    for name in ('MAP_NEW_BARK_PLAYER_HOUSE_1F', 'MAP_NEW_BARK_PLAYER_HOUSE_2F'):
        body = re.search(r'\['+name+r'\]\s*=\s*\{(.*?)\}', headers, re.S)[1]
        bank = int(re.search(r'\.eventsBank\s*=\s*NARC_zone_event_(\d+)_', body)[1])
        map_id = int(re.search(r'#define\s+'+name+r'\s+(\d+)', constants)[1])
        counts, warps = decode(payloads[bank])
        rooms[name] = {'map': map_id, 'bank': bank, 'counts': counts, 'warps': warps}
    upper = rooms['MAP_NEW_BARK_PLAYER_HOUSE_2F']
    lower = rooms['MAP_NEW_BARK_PLAYER_HOUSE_1F']
    if len(upper['warps']) != 1:
        raise ValueError('Initial room should have one staircase')
    stair = upper['warps'][0]
    if stair[2] != lower['map'] or stair[3] >= len(lower['warps']):
        raise ValueError('Initial-room destination does not exist')
    returning = lower['warps'][stair[3]]
    if returning[2:4] != (upper['map'], 0):
        raise ValueError('Staircase destination does not lead back to initial room')
    report = {'passed': not mismatches, 'members': members, 'event_totals_bg_object_warp_coord': totals,
              'mismatched_members': mismatches, 'platform': a.target+'-x86_64',
              'sanitizers': ['address', 'undefined'] if a.target == 'linux' else [],
              'source_sha256': hashlib.sha256((a.hgss/'src/map_events.c').read_bytes()).hexdigest(),
              'archive_sha256': hashlib.sha256(archive).hexdigest(),
              'initial_house': rooms, 'staircase_reciprocal_data': True,
              'scope': 'Original static event parser, getters and coordinate lookup; all private ROM event members',
              'limits': 'No Field_InitMapEvents, script execution, object creation, collision, rendering, transitions or playable FieldSystem. Windows has no sanitizers. ASan global instrumentation disabled.'}
    (output/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))
    if mismatches:
        raise SystemExit('Original getter output differs from ROM members')


if __name__ == '__main__':
    main()
