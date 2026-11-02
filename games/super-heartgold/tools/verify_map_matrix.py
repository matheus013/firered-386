"""Run original map-matrix loader on private reference resources with sanitizers."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess


def decode(raw,map_id=64):
    width,height,headers,altitudes,length=raw[:5]
    count=width*height
    if not 0<count<=799 or length>16: raise ValueError('Invalid matrix dimensions/name')
    pos=5+length
    names=raw[5:pos]+bytes(16-length)
    maps=list(struct.unpack_from('<'+'H'*count,raw,pos)) if headers else [map_id]*count
    if headers: pos+=count*2
    heights=raw[pos:pos+count] if altitudes else bytes(count)
    if altitudes: pos+=count
    models=struct.unpack_from('<'+'H'*count,raw,pos)
    if pos+2*count!=len(raw): raise ValueError('Unexpected matrix payload size')
    return bytes([width,height])+names+b''.join(struct.pack('<HBH',h,a,m) for h,a,m in zip(maps,heights,models))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('hgss','probe','stack','output'): parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--target',choices=['linux','windows'],default='linux')
    parser.add_argument('--sdl-include',type=Path)
    a=parser.parse_args(); a.output.mkdir(parents=True,exist_ok=False)
    source=a.hgss/'src/map_matrix.c'; files=a.hgss/'files/fielddata/mapmatrix/map_matrix'
    members=sorted(files.glob('*.bin'))
    indices=[int(re.search(r'map_matrix_(\d{4})',p.name)[1]) for p in members]
    if indices!=list(range(len(members))): raise ValueError('Non-contiguous matrix member names')
    header=a.output/'fielddata/mapmatrix/map_matrix.naix'; header.parent.mkdir(parents=True)
    header.write_text('#pragma once\n'+''.join(f'#define NARC_map_matrix_{p.name.replace(".","_")} {i}\n' for i,p in enumerate(members)))
    # Check the actual initial-room mapping, not a guessed member index.
    text=(a.hgss/'src/data/map_headers.h').read_text()
    room=re.search(r'\[MAP_NEW_BARK_PLAYER_HOUSE_2F\]\s*=\s*\{(.*?)\}',text,re.S)[1]
    if '.matrixId = NARC_map_matrix_map_matrix_0072_T20R0202_bin' not in room:
        raise ValueError('Initial-room matrix changed')
    sdk=a.stack/'subprojects/libntr'; nns=a.stack/'subprojects/libntrsystem'
    flags=['gcc','-std=gnu11','-g','-DSDK_PORT','-DSDK_X86','-DSDK_BUILD_LINUX','-DSDK_64BIT',
           '-DSDK_VERSION_MAJOR=4','-DSOULSILVER','-DENGLISH','-DSDK_FINALROM',
           '-ffunction-sections','-fdata-sections','-fsanitize=address,undefined',
           '-fno-sanitize-recover=all','-Wl,--gc-sections','--param=asan-globals=0',
           '-include',str(a.probe/'hgss_backup.h')]
    if a.target=='windows':
        flags[0]='x86_64-w64-mingw32-gcc'
        flags=[f.replace('-DSDK_BUILD_LINUX','-DSDK_BUILD_WIN64') for f in flags
               if f not in ('-fsanitize=address,undefined','-fno-sanitize-recover=all','--param=asan-globals=0')]
        flags += ['-static','-DSDL_MAIN_HANDLED','-fno-asynchronous-unwind-tables','-fno-unwind-tables']
    if a.sdl_include: flags.append('-I'+str(a.sdl_include))
    for inc in (a.output,a.probe,a.probe/'hgss-include',a.probe/'hgss-include/library',sdk/'include',nns/'include',
                a.stack/'build/subprojects/libntr/gen',a.stack/'build/subprojects/libntr/gen/nitro/fx'):
        flags.append('-I'+str(inc))
    binary=a.output/('matrix-test.exe' if a.target=='windows' else 'matrix-test')
    result=subprocess.run(flags+[str(source),str(Path(__file__).with_name('probe_map_matrix.c')),
        str(sdk/'libraries/mi/src/mi_memory.c'),'-o',str(binary)],capture_output=True,text=True)
    (a.output/'build.log').write_text(result.stdout+result.stderr); result.check_returncode()
    cases=[]
    def host_path(path):
        if a.target=='windows':
            return subprocess.check_output(['wslpath','-w',str(path)],text=True).strip()
        return str(path)
    for member in (0,71,72):
        output=a.output/f'decoded-{member}.bin'
        result=subprocess.run([str(binary),host_path(members[member]),str(member),host_path(output)],capture_output=True,text=True,timeout=60)
        (a.output/f'run-{member}.log').write_text(result.stdout+result.stderr); result.check_returncode()
        raw=members[member].read_bytes(); exact=output.read_bytes()==decode(raw)
        cases.append({'member':member,'dimensions':list(raw[:2]),'exact':exact,
                      'sha256':hashlib.sha256(raw).hexdigest()})
    report={'passed':all(c['exact'] for c in cases),'cases':cases,'reloads':192,'platform':a.target+'-x86_64',
            'sanitizers':['address','undefined'] if a.target=='linux' else [],'asan_globals':False,
            'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
            'scope':'original_map_matrix_loader_with_host_file_heap_and_explicit_matrix_selection',
            'limits':'Private pinned source resources, not ROM comparison. No FieldSystem, models, collision or map-header runtime. Windows target has no sanitizers.'}
    (a.output/'result.json').write_text(json.dumps(report,indent=2)+'\n'); print(json.dumps(report))
    if not report['passed']: raise SystemExit('Map matrix differs from independent decoder')

if __name__=='__main__': main()
