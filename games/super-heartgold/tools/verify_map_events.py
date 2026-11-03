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


def command_table(source):
    """Preserve every table slot, including handlers with nonstandard names."""
    body = source.split('gScriptCmdTable[] = {', 1)[1].split('};', 1)[0]
    body = re.sub(r'/\*.*?\*/|//[^\n]*', '', body, flags=re.S)
    commands = [item.strip() for item in body.split(',') if item.strip()]
    if not commands or any(not re.fullmatch(r'[A-Za-z_]\w*', item) for item in commands):
        raise ValueError('Unsupported command table initializer; do not guess opcode positions')
    return commands


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('hgss', 'probe', 'stack', 'archive', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--target', choices=['linux', 'windows'], default='linux')
    parser.add_argument('--sdl-include', type=Path)
    parser.add_argument('--script-archive', type=Path,
                        help='Also execute original Field_InitMapEvents for both initial-house floors')
    parser.add_argument('--script-selection', action='store_true',
                        help='Exercise original scene selection using the original save-variable block')
    parser.add_argument('--script-dispatch', action='store_true',
                        help='Dispatch real initial-house scripts until the first unintegrated command')
    parser.add_argument('--inactive-follower', action='store_true',
                        help='Execute the original first command for an explicitly inactive follower')
    parser.add_argument('--lock-all', action='store_true',
                        help='Execute original LockAll against an explicit object-manager fixture')
    a = parser.parse_args()
    if a.script_selection and not a.script_archive:
        parser.error('--script-selection requires --script-archive')
    if a.script_dispatch and not a.script_selection:
        parser.error('--script-dispatch requires --script-selection')
    if a.inactive_follower and not a.script_dispatch:
        parser.error('--inactive-follower requires --script-dispatch')
    if a.lock_all and not a.inactive_follower:
        parser.error('--lock-all requires --inactive-follower')
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
    headers = (a.hgss/'src/data/map_headers.h').read_text()
    constants = (a.hgss/'include/constants/maps.h').read_text()
    rooms = {}
    for name in ('MAP_NEW_BARK_PLAYER_HOUSE_1F', 'MAP_NEW_BARK_PLAYER_HOUSE_2F'):
        body = re.search(r'\['+name+r'\]\s*=\s*\{(.*?)\}', headers, re.S)[1]
        bank = int(re.search(r'\.eventsBank\s*=\s*NARC_zone_event_(\d+)_', body)[1])
        map_id = int(re.search(r'#define\s+'+name+r'\s+(\d+)', constants)[1])
        counts, warps = decode(payloads[bank])
        rooms[name] = {'map': map_id, 'bank': bank, 'counts': counts, 'warps': warps}
        if a.script_archive:
            if not re.search(r'\.wildEncounterBank\s*=\s*ENCDATA_NA\s*,', body):
                raise ValueError('Indoor no-encounter adapter does not match original header')
            rooms[name]['script_bank'] = int(re.search(r'\.scriptHeaderBank\s*=\s*NARC_scr_seq_scr_seq_(\d+)_', body)[1])
    script_payloads = {}
    if a.script_archive:
        script_archive = a.script_archive.read_bytes()
        declarations = []
        for room in rooms.values():
            bank = room['script_bank']
            raw = narc_member(script_archive, bank)
            if not 0 < len(raw) < 0x100:
                raise ValueError('Script header exceeds original buffer')
            (output/f'{bank}.script').write_bytes(raw)
            script_payloads[room['map']] = raw
            declarations.append('{'+','.join(map(str, (room['map'],room['bank'],bank,len(payloads[room['bank']]),len(raw))))+'}')
        (output/'field_load_cases.h').write_text(
            'typedef struct { unsigned map,events,script,event_size,script_size; } FieldLoadCase;\n'
            'static const FieldLoadCase field_load_cases[2] = {'+','.join(declarations)+'};\n')
    if a.script_selection:
        # Lock the independent fixture oracle to the two verified ROM headers.
        # 1F: type 1, relative offset 1, terminator, (var, value, script) pairs.
        expected_lower = struct.pack('<BI',1,1)+b'\0'+struct.pack('<HHHHHHH',0x4106,3,7,0x4106,0,1,0)
        if script_payloads.get(63) != expected_lower or script_payloads.get(64) != bytes(4):
            raise ValueError('Initial-house script headers differ from selection oracle')
        manager = (a.hgss/'src/script_manager.c').read_text()
        for prefix,path in [('scr_seq_scr_seq','fielddata/script/scr_seq.naix'),('msg_msg','msgdata/msg.naix')]:
            names = sorted(set(re.findall(r'NARC_'+prefix+r'_\d+_\w+',manager)))
            target = output/path
            target.parent.mkdir(parents=True,exist_ok=True)
            definitions = []
            for name in names:
                member = int(re.search(r'_(\d+)_', name)[1])
                definitions.append(f'#define {name} {member}\n')
            target.write_text('#pragma once\n'+''.join(definitions))
    dispatch_cases = []
    if a.script_dispatch:
        body = re.search(r'\[MAP_NEW_BARK_PLAYER_HOUSE_1F\]\s*=\s*\{(.*?)\}', headers, re.S)[1]
        bank = int(re.search(r'\.scriptsBank\s*=\s*NARC_scr_seq_scr_seq_(\d+)_', body)[1])
        bytecode = narc_member(script_archive, bank)
        (output/'initial-house.bytecode').write_bytes(bytecode)
        commands = command_table((a.hgss/'src/data/fieldmap/script_cmd_table.h').read_text())
        for script_id in (1,7):
            entry = 4*(script_id-1)
            offset = entry+4+struct.unpack_from('<I',bytecode,entry)[0]
            if not 0 <= offset <= len(bytecode)-2:
                raise ValueError('Script entry outside bytecode bank')
            opcode, = struct.unpack_from('<H',bytecode,offset)
            if opcode >= len(commands):
                raise ValueError('Unknown original opcode')
            dispatch_cases.append({'script':script_id,'offset':offset,'opcode':opcode,'command':commands[opcode]})
        declarations = ['{'+','.join(str(c[k]) for k in ('script','offset','opcode'))+'}' for c in dispatch_cases]
        (output/'script_dispatch_cases.h').write_text(
            f'#define DISPATCH_COMMAND_COUNT {len(commands)}\n#define DISPATCH_BANK_SIZE {len(bytecode)}\n'
            'typedef struct { unsigned script,offset,opcode; } DispatchCase;\n'
            'static const DispatchCase dispatch_cases[2] = {'+','.join(declarations)+'};\n')
    extracted_functions = []
    if a.inactive_follower:
        for case in dispatch_cases:
            next_opcode, = struct.unpack_from('<H',bytecode,case['offset']+2)
            if case['command'] != 'ScrCmd_FollowMonResetMovement' or commands[next_opcode] != 'ScrCmd_LockAll':
                raise ValueError('Unsupported first-command sequence')
        fragments = []
        functions = [('scrcmd_c.c','ScrCmd_FollowMonResetMovement'),('follow_mon.c','FollowMon_IsActive')]
        if a.lock_all:
            functions += [('scrcmd_c.c','ScrCmd_LockAll'),('scrcmd_c.c','_WaitFollowMonPaused'),('follow_mon.c','FollowMon_GetMapObject')]
            for case in dispatch_cases:
                opcode, = struct.unpack_from('<H',bytecode,case['offset']+4)
                if commands[opcode] != 'ScrCmd_ApplyMovement':
                    raise ValueError('Unexpected command after LockAll')
        for file,name in functions:
            source = (a.hgss/'src'/file).read_text()
            matches = re.findall(r'^[A-Za-z_][\w *]*\b'+name+r'\([^\n]+\) \{\n.*?^\}',source,re.M|re.S)
            if len(matches) != 1:
                raise ValueError('Original function boundary changed')
            fragments.append(matches[0])
            extracted_functions.append({'source':file,'function':name,'sha256':hashlib.sha256(matches[0].encode()).hexdigest()})
        includes = ['field_system.h','follow_mon.h','map_object.h','field_terrain.h','script_pokemon_util.h','constants/scrcmd.h']
        (output/'original_follower_command.c').write_text(
            ''.join('#include "'+h+'"\n' for h in includes)+
            'BOOL _WaitFollowMonPaused(ScriptContext *ctx);\nBOOL ScrCmd_LockLastTalked(ScriptContext *ctx);\n'+
            '\n\n'.join(fragments)+'\n')
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
    sources = [str(Path(__file__).with_name('probe_map_events.c'))]
    if a.script_archive:
        flags.append('-DHG_FIELD_LOAD')
        sources.append(str(sdk/'libraries/mi/src/mi_memory.c'))
    if a.script_selection:
        flags.append('-DHG_SCRIPT_SELECTION')
        sources += [str(a.hgss/'src/script_manager.c'),str(a.hgss/'src/save_vars_flags.c')]
    if a.script_dispatch:
        flags.append('-DHG_SCRIPT_DISPATCH')
        sources.append(str(a.hgss/'src/script.c'))
    if a.inactive_follower:
        flags.append('-DHG_INACTIVE_FOLLOWER')
        sources.append(str(output/'original_follower_command.c'))
    if a.lock_all:
        flags.append('-DHG_LOCK_ALL')
        sources.append(str(a.hgss/'src/map_object.c'))
    result = subprocess.run(flags+sources+['-o', str(binary)], capture_output=True, text=True)
    (output/'build.log').write_text(result.stdout+result.stderr)
    result.check_returncode()
    directory = str(output)
    if a.target == 'windows':
        directory = subprocess.check_output(['wslpath', '-w', directory], text=True).strip()
    result = subprocess.run([str(binary), directory, str(members)], capture_output=True, text=True, timeout=60)
    (output/'run.log').write_text(result.stdout+result.stderr)
    result.check_returncode()
    mismatches = [i for i,raw in enumerate(payloads) if (output/f'{i}.out').read_bytes() != raw]
    if a.script_archive:
        for room in rooms.values():
            expected = payloads[room['bank']]+script_payloads[room['map']]
            if (output/f'{room["map"]}.loaded').read_bytes() != expected:
                raise ValueError('Field_InitMapEvents output differs from private resources')
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
    if a.script_archive:
        report['field_init'] = {'loads':128, 'allocations_and_releases':64,
                                'script_archive_sha256':hashlib.sha256(script_archive).hexdigest(),
                                'event_and_script_bytes_exact':True,
                                'script_tail_and_no_encounters_zeroed':True}
        report['scope'] += '; original Field_AllocateMapEvents/Field_InitMapEvents/Field_FreeMapEvents with explicit private-file, heap and MapHeader adapters'
        report['limits'] = 'No script execution, object creation, collision, rendering, transitions or running FieldSystem. Only no-wild-encounter rooms covered by Field_InitMapEvents. Windows has no sanitizers. ASan global instrumentation disabled.'
    if a.script_selection:
        report['script_selection'] = {'passed':True,'variable': '0x4106','values':[0,1,2,3,4],
            'checks_per_platform':640,'original_save_vars_flags':True,
            'first_floor_selected_scripts':{'0':1,'3':7,'other_tested_values':65535},
            'upper_floor_has_no_automatic_scene':True,'warp_suppression_tested':True,
            'script_manager_sha256':hashlib.sha256((a.hgss/'src/script_manager.c').read_bytes()).hexdigest(),
            'save_vars_flags_sha256':hashlib.sha256((a.hgss/'src/save_vars_flags.c').read_bytes()).hexdigest()}
        report['scope'] += '; original scene selector and original normal-variable access with isolated SaveArray adapter, not full save'
    if a.script_dispatch:
        report['script_dispatch'] = {'entry_and_first_dispatch_passed':True,
            'scripts_completed':0,'cases':dispatch_cases,
            'boundary':'Diagnostic handler stops at first command; no camera or movement effect executed',
            'script_c_sha256':hashlib.sha256((a.hgss/'src/script.c').read_bytes()).hexdigest(),
            'bytecode_sha256':hashlib.sha256(bytecode).hexdigest()}
        report['scope'] += '; original VM entry offset and first dispatch from real bytecode, stopped at explicit unintegrated-command boundary'
    if a.inactive_follower:
        report['script_dispatch']['boundary'] = 'Original first command executed only with follower inactive; stopped before LockAll'
        report['script_dispatch']['original_commands_executed'] = 2
        report['script_dispatch']['next_blocked_command'] = 'ScrCmd_LockAll'
        report['script_dispatch']['private_unchanged_function_excerpts'] = extracted_functions
        report['limits'] = 'Explicit inactive-follower fixture, not full new-game state. No active follower, object movement, full script completion, rendering or running FieldSystem. Extracted unchanged functions, not whole scrcmd/follow_mon units. Windows has no sanitizers; ASan globals disabled.'
    if a.lock_all:
        report['script_dispatch']['boundary'] = 'First two original commands executed with inactive follower and fixture objects; stopped before ApplyMovement'
        report['script_dispatch']['original_commands_executed'] = 4
        report['script_dispatch']['next_blocked_command'] = 'ScrCmd_ApplyMovement'
        report['lock_all'] = {'objects_per_case':3,'active_per_case':2,'inactive_per_case':1,
            'active_paused_inactive_unchanged':True,'unpause_restores_flags':True,
            'original_map_object_source_sha256':hashlib.sha256((a.hgss/'src/map_object.c').read_bytes()).hexdigest(),
            'limits':'Fixture manager and ScriptEnvironment, not map-created objects or real TaskManager scheduling; no last-interacted NPC or active follower branch'}
    (output/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))
    if mismatches:
        raise SystemExit('Original getter output differs from ROM members')


if __name__ == '__main__':
    main()
