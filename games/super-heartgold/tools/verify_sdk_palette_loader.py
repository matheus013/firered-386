"""Reproduce palette-loader memory errors and test the native adaptation on scene 4 assets."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
from prepare_hgss_opening_headers import ARCHIVE_SHA256
from prepare_hgss_message_headers import read_banks
from prepare_sdk_palette_loader import prepare, HASH


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--stack',type=Path,required=True)
    p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    raw=a.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest()!=ARCHIVE_SHA256: raise ValueError('Archive hash differs')
    a.output.mkdir(parents=True,exist_ok=False)
    files=[]
    banks=read_banks(raw)
    for index in (53,81,85,89,93):
        path=a.output/f'{index}.nclr'; path.write_bytes(banks[index]); files.append(str(path))
    sdk=a.stack/'subprojects/libntr'
    nns=a.stack/'subprojects/libntrsystem'
    original=nns/'libraries/g2d/src/load/g2d_NCL_load.c'
    adapted=a.output/'g2d_NCL_native.c'; prepare(original,adapted)
    flags=['gcc','-g','-DSDK_PORT','-DSDK_X86','-DSDK_BUILD_LINUX','-DSDK_64BIT',
           '-DSDK_VERSION_MAJOR=4','-DSDK_FINALROM','-DNNS_FINALROM',
           '-ffunction-sections','-fdata-sections','-fsanitize=address,undefined',
           '-fno-sanitize-recover=all','-Wl,--gc-sections']
    for path in (sdk/'include',nns/'include',nns/'libraries/g2d/include',
                 a.stack/'build/subprojects/libntr/gen',a.stack/'build/subprojects/libntr/gen/nitro/fx'):
        flags.append('-I'+str(path))
    for label,source in [('original',original),('adapted',adapted)]:
        binary=a.output/label
        result=subprocess.run(flags+[str(Path(__file__).with_name('probe_sdk_palette_loader.c')),
            str(source),str(nns/'libraries/g2d/src/load/g2d_Load.c'),'-o',str(binary)],capture_output=True,text=True)
        (a.output/(label+'-build.log')).write_text(result.stdout+result.stderr); result.check_returncode()
        result=subprocess.run([str(binary),*files],capture_output=True,text=True)
        (a.output/(label+'.log')).write_text(result.stdout+result.stderr)
        if label=='original':
            if result.returncode==0 or 'AddressSanitizer' not in result.stderr: raise ValueError('Expected original ASan failure')
        else: result.check_returncode()
    report={'passed':True,'original_asan_failure_reproduced':True,'source_sha256':HASH,
            'palette_members':[53,81,85,89,93],'input_unchanged':True,
            'sanitizers':['address','undefined'],'gpu_tested':False}
    (a.output/'result.json').write_text(json.dumps(report,indent=2)+'\n'); print(json.dumps(report))


if __name__=='__main__': main()
