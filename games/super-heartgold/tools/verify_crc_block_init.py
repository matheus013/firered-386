"""Compare original ARM9 block initializers with native blocks and CRC traces.

Only SaveSubstruct_UpdateCRC is a controlled host service: it performs a real
independent CCITT calculation and writes the bound block's two CRC bytes.
No original SaveData ABI or save files are used.
"""
import argparse
import binascii
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile

IMAGE_HASH = '5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8'
IDS = (23,24,26,27,30,31)
PATTERNS = (0,0xa5,0xff,0x5a)
BASE, RAM, CRC_SERVICE = 0x02000000, 0x01000000, 0x0202893c
HARNESS = r'''
#include "hg/crc_block_init.hpp"
#include <algorithm>
#include <cstdio>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
void word(unsigned value, unsigned width=4) {
    for(unsigned i=0;i<width;++i) std::putchar((value>>(8*i))&255);
}
int main() {
#ifdef _WIN32
    if (_setmode(_fileno(stdout),_O_BINARY)==-1) return 1;
#endif
    for(unsigned id : {23u,24u,26u,27u,30u,31u}) for(unsigned pattern : {0u,0xa5u,0xffu,0x5au}) {
        hg::SaveContext context;
        std::fill(context.storage().begin(),context.storage().end(),std::byte(pattern));
        auto result=hg::initialize_soulsilver_usa_crc_block(context,id);
        const auto block=context.view(id);
        word(id);word(static_cast<unsigned>(block.size()));word(static_cast<unsigned>(result.updates.size()));
        if(std::fwrite(block.data(),1,block.size(),stdout)!=block.size()) return 1;
        for(const auto& item:result.updates) {word(item.id);word(item.crc,2);}
    }
    return std::ferror(stdout)?1:0;
}
'''


def native_cases(compiler):
    root = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='hg-crc-init-') as directory:
        folder = Path(directory)
        source, binary = folder/'probe.cpp', folder/'probe.exe'
        source.write_text(HARNESS)
        sources = ['crc_block_init','save_context','save_validation','save_selection','save_image']
        subprocess.run([compiler,'-std=c++20','-O1','-Wall','-Wextra','-Werror',
                        '-I',str(root/'game/core/include'),str(source),
                        *(str(root/f'game/core/src/{name}.cpp') for name in sources),
                        '-o',str(binary)],check=True,capture_output=True,timeout=90)
        data = subprocess.run([str(binary)],check=True,capture_output=True,timeout=60).stdout
    cursor, cases = 0, []
    for expected_id in IDS:
        for pattern in PATTERNS:
            block_id, length, count = struct.unpack_from('<III',data,cursor)
            cursor += 12
            if block_id != expected_id or length > 0x2000 or count > 157:
                raise ValueError('Unexpected native proof metadata')
            payload = data[cursor:cursor+length]
            cursor += length
            trace = [struct.unpack_from('<IH',data,cursor+i*6) for i in range(count)]
            cursor += count*6
            cases.append((block_id,pattern,payload,trace))
    if cursor != len(data): raise ValueError('Trailing native proof data')
    return cases


def prove(image,cases):
    from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_LR,UC_ARM_REG_SP,UC_ARM_REG_PC
    # This is the exact language/greeting map used by the pinned USA build.
    if image[0xf6120:0xf612c] != bytes((1,0,2,1,3,2,4,3,5,4,7,5)):
        raise ValueError('Pinned language/greeting mapping differs')
    total_updates = 0
    for block_id,pattern,expected,expected_trace in cases:
        descriptor = struct.unpack_from('<IIII',image,0xf64c4+block_id*16)
        if descriptor[:2] != (block_id,0): raise ValueError('Unexpected block descriptor')
        machine = Uc(UC_ARCH_ARM,UC_MODE_ARM)
        machine.mem_map(BASE,0x400000); machine.mem_write(BASE,image)
        machine.mem_map(RAM,0x100000)
        start, guard = RAM+0x10010, bytes([0x3c])*16
        machine.mem_write(start-16,guard+bytes([pattern])*len(expected)+guard)
        trace = []
        aligned = len(expected)-4

        def service(_machine,address,_size,_user):
            if address != CRC_SERVICE: return
            requested = machine.reg_read(UC_ARM_REG_R0)
            if requested != block_id: raise ValueError('Original CRC call used a different binding')
            crc = binascii.crc_hqx(bytes(machine.mem_read(start,aligned)),0xffff)
            machine.mem_write(start+aligned,struct.pack('<H',crc))
            trace.append((requested,crc))
            machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR))

        def check_write(_machine,_access,address,length,_value,_user):
            if not (start <= address and address+length <= start+len(expected)
                    or RAM+0xe0000 <= address and address+length <= RAM+0x100000):
                raise ValueError('Original initializer wrote outside target block/stack')

        machine.hook_add(UC_HOOK_CODE,service)
        machine.hook_add(UC_HOOK_MEM_WRITE,check_write)

        def call(address,argument):
            machine.reg_write(UC_ARM_REG_R0,argument)
            machine.reg_write(UC_ARM_REG_SP,RAM+0xfff00)
            machine.reg_write(UC_ARM_REG_LR,RAM)
            machine.emu_start(address,RAM,count=1000000)
            if machine.reg_read(UC_ARM_REG_PC)!=RAM: raise ValueError('Original function exceeded budget')
            return machine.reg_read(UC_ARM_REG_R0)

        raw_size = call(descriptor[2],0)
        if ((raw_size+3)&~3) != aligned: raise ValueError('Original/native sizes differ')
        call(descriptor[3],start)
        if bytes(machine.mem_read(start,len(expected))) != expected:
            raise ValueError(f'Original/native block bytes differ: ID {block_id}, pattern {pattern}')
        if trace != expected_trace:
            raise ValueError(f'Original/native CRC trace differs: ID {block_id}, pattern {pattern}')
        if (bytes(machine.mem_read(start-16,16)) != guard or
                bytes(machine.mem_read(start+len(expected),16)) != guard):
            raise ValueError('Guard bytes changed')
        total_updates += len(trace)
    return dict(image_sha256=IMAGE_HASH,blocks=list(IDS),cases=len(cases),
                crc_updates_compared=total_updates,result='passed',
                substituted_service='bound_SaveSubstruct_UpdateCRC_using_independent_CCITT',
                scope='isolated_initializers_and_real_CRC_service_not_complete_SaveData')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--arm9',required=True,type=Path)
    parser.add_argument('--cxx',default='c++')
    args=parser.parse_args()
    try:
        with args.arm9.open('rb') as stream: image=stream.read(0x400001)
        if hashlib.sha256(image).hexdigest()!=IMAGE_HASH: raise ValueError('Pinned ARM9 hash differs')
        print(json.dumps(prove(image,native_cases(args.cxx)),indent=2))
    except (OSError,ValueError,subprocess.SubprocessError) as error:
        parser.exit(1,str(error)+'\n')

if __name__=='__main__': main()
