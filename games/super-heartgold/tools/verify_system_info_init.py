"""Compare native SysInfo bytes/epoch conversion against original ARM9.

The RTC snapshot copy is the only substituted service. Calendar/epoch routines
and the original initializer run from the hash-checked private image.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile

IMAGE_HASH='5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
SAMPLES=((0,1,1,6,0,0,0),(0,2,28,1,23,59,59),(0,2,29,2,12,34,56),
         (1,3,1,4,0,0,0),(4,2,29,0,0,0,1),(68,1,19,4,3,14,8),
         (99,12,31,4,23,59,59),(26,10,25,0,23,59,59))
BASE,RAM,COPY_RTC=0x02000000,0x01000000,0x02014708


def native(compiler):
    root=Path(__file__).resolve().parents[1]
    initializer=','.join('{'+','.join(map(str,sample))+'}' for sample in SAMPLES)
    harness='''
#include "hg/system_info_init.hpp"
#include <array>
#include <cstdio>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
int main() {
#ifdef _WIN32
    if(_setmode(_fileno(stdout),_O_BINARY)==-1) return 1;
#endif
    for(const auto sample:std::array<hg::RtcSnapshot,8>{{SAMPLES}}) {
        const auto block=hg::initialize_system_info(sample);
        if(std::fwrite(block.data(),1,block.size(),stdout)!=block.size()) return 1;
    }
}
'''.replace('SAMPLES',initializer)
    with tempfile.TemporaryDirectory(prefix='hg-rtc-proof-') as directory:
        folder=Path(directory)
        source,binary=folder/'probe.cpp',folder/'probe.exe'
        source.write_text(harness)
        subprocess.run([compiler,'-std=c++20','-Wall','-Wextra','-Werror','-I',str(root/'game/core/include'),
                        str(source),str(root/'game/core/src/system_info_init.cpp'),'-o',str(binary)],
                       check=True,capture_output=True,timeout=60)
        result=subprocess.run([str(binary)],check=True,capture_output=True,timeout=30).stdout
    if len(result)!=len(SAMPLES)*92: raise ValueError('Native SysInfo size differs')
    return result


def prove(image,outputs):
    from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_LR,UC_ARM_REG_SP,UC_ARM_REG_PC
    descriptor=struct.unpack_from('<IIII',image,0xf64c4)
    if descriptor!=(0,0,0x02028d15,0x02028d19): raise ValueError('Pinned SysInfo descriptor differs')
    comparisons=0
    for index,snapshot in enumerate(SAMPLES):
        expected=outputs[index*92:(index+1)*92]
        for pattern in (0,0xa5,0xff,0x5a):
            machine=Uc(UC_ARCH_ARM,UC_MODE_ARM)
            machine.mem_map(BASE,0x400000);machine.mem_write(BASE,image)
            machine.mem_map(RAM,0x100000)
            start,guard=RAM+0x10010,bytes([0x3c])*16
            machine.mem_write(start-16,guard+bytes([pattern])*92+guard)
            copy_calls=[]

            def clock_service(_machine,address,_size,_user):
                if address!=COPY_RTC: return
                date,time=machine.reg_read(UC_ARM_REG_R0),machine.reg_read(UC_ARM_REG_R1)
                if (date,time)!=(start+20,start+36): raise ValueError('RTC service binding differs')
                machine.mem_write(date,struct.pack('<4I',*snapshot[:4]))
                machine.mem_write(time,struct.pack('<3I',*snapshot[4:]))
                copy_calls.append(1)
                machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR))

            def check_write(_machine,_access,address,length,_value,_user):
                if not (start<=address and address+length<=start+92 or
                        RAM+0xe0000<=address and address+length<=RAM+0x100000):
                    raise ValueError('Original SysInfo wrote outside block/stack')

            machine.hook_add(UC_HOOK_CODE,clock_service)
            machine.hook_add(UC_HOOK_MEM_WRITE,check_write)

            def call(address,argument):
                machine.reg_write(UC_ARM_REG_R0,argument)
                machine.reg_write(UC_ARM_REG_SP,RAM+0xfff00)
                machine.reg_write(UC_ARM_REG_LR,RAM)
                machine.emu_start(address,RAM,count=100000)
                if machine.reg_read(UC_ARM_REG_PC)!=RAM: raise ValueError('Original SysInfo exceeded budget')
                return machine.reg_read(UC_ARM_REG_R0)

            if call(descriptor[2],0)!=92: raise ValueError('Original SysInfo size differs')
            call(descriptor[3],start)
            if bytes(machine.mem_read(start,92))!=expected: raise ValueError('SysInfo bytes/epoch differ')
            if len(copy_calls)!=1: raise ValueError('Original RTC copy count differs')
            if bytes(machine.mem_read(start-16,16))!=guard or bytes(machine.mem_read(start+92,16))!=guard:
                raise ValueError('SysInfo guard changed')
            comparisons+=1
    return dict(result='passed',image_sha256=IMAGE_HASH,snapshots=len(SAMPLES),comparisons=comparisons,
                original_calendar_conversion='executed',substituted_service='RTC_snapshot_copy_only',
                scope='raw_SysInfo_Init_not_owner_identity_or_complete_newgame')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9',required=True,type=Path);parser.add_argument('--cxx',default='c++')
    args=parser.parse_args()
    try:
        with args.arm9.open('rb') as stream:image=stream.read(0x400001)
        if hashlib.sha256(image).hexdigest()!=IMAGE_HASH:raise ValueError('Pinned ARM9 hash differs')
        print(json.dumps(prove(image,native(args.cxx)),indent=2))
    except (OSError,ValueError,subprocess.SubprocessError) as error:parser.exit(1,str(error)+'\n')

if __name__=='__main__':main()
