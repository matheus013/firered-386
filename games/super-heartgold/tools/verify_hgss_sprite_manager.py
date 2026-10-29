"""Link the original HGSS SpriteList lifecycle with native SDK functions under sanitizers."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from prepare_hgss_sprite_storage import prepare
from prepare_hgss_message_headers import read_banks
from prepare_hgss_opening_headers import ARCHIVE_SHA256, decompress_lz10


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('hgss', 'probe', 'stack', 'archive', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    a = parser.parse_args()
    raw = a.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ARCHIVE_SHA256: raise ValueError('Archive differs')
    a.output.mkdir(parents=True, exist_ok=False)
    prepare(a.hgss/'include/sprite.h', a.output/'sprite.h')
    banks = read_banks(raw)
    for i in (87,88): (a.output/f'{i}.bin').write_bytes(decompress_lz10(banks[i]))
    sdk, nns = [a.stack/'subprojects'/n for n in ('libntr','libntrsystem')]
    sources = [a.hgss/'src/sprite.c', Path(__file__).with_name('probe_hgss_sprite_manager.c')]
    units = ['g2d_Image.c','g2d_RendererCore.c','g2d_Animation.c','g2d_CellAnimation.c',
             'g2d_CellTransferManager.c','g2d_MultiCellAnimation.c','g2d_Node.c','g2d_SRTControl.c',
             'load/g2d_NAN_load.c','load/g2d_NOB_load.c','load/g2d_NMC_load.c','load/g2d_Load.c']
    sources += [nns/'libraries/g2d/src'/u for u in units]
    sources += [sdk/'libraries/fx/src/fx.c', sdk/'libraries/mi/src/mi_memory.c']
    flags = ['gcc','-std=gnu11','-g','-DSDK_PORT','-DSDK_X86','-DSDK_BUILD_LINUX','-DSDK_64BIT',
             '-DSDK_VERSION_MAJOR=4','-DSOULSILVER','-DENGLISH','-DSDK_FINALROM','-DNNS_FINALROM',
             '-ffunction-sections','-fdata-sections','-fsanitize=address,undefined',
             '-fno-sanitize-recover=all','-Wl,--gc-sections','--param=asan-globals=0','-include',str(a.probe/'hgss_backup.h')]
    for inc in (a.output,a.probe,a.probe/'hgss-include',a.probe/'hgss-include/library',
                sdk/'include',nns/'include',nns/'libraries/g2d/include',
                a.stack/'build/subprojects/libntr/gen',a.stack/'build/subprojects/libntr/gen/nitro/fx'):
        flags.append('-I'+str(inc))
    binary = a.output/'manager'
    # Only fixed-point fallback symbols; rendering is discarded by section GC.
    link = [str(a.stack/'build/subprojects/libntr/libraries/fx/libfx.a'), '-lm']
    result = subprocess.run(flags+list(map(str,sources))+link+['-o',str(binary)],capture_output=True,text=True)
    (a.output/'build.log').write_text(result.stdout+result.stderr); result.check_returncode()
    result = subprocess.run([str(binary),str(a.hgss/'files/data/clact_default.NANR'),
                             str(a.output/'88.bin'),str(a.output/'87.bin')],capture_output=True,text=True)
    (a.output/'run.log').write_text(result.stdout+result.stderr); result.check_returncode()
    report = {'passed':True,'platform':'linux-x86_64','sanitizers':['address','undefined'],
              'asan_globals':False, 'prebuilt_fx_archive_instrumented':False,
              'scope':'original_SpriteList_lifecycle_with_host_heap_and_file_adapters_no_rendering',
              'cycles':64,'sprite_creations':448,'animation_ticks':61440,'log':result.stdout,
              'sprite_source_sha256':hashlib.sha256(sources[0].read_bytes()).hexdigest()}
    (a.output/'result.json').write_text(json.dumps(report,indent=2)+'\n'); print(json.dumps(report))


if __name__=='__main__': main()





