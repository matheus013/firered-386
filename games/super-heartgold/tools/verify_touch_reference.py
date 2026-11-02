"""Reuse external ssrecomp layout code as an oracle for the native touch bridge."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

PIN='3146fb2d4d88522b12b4bf25dee74285120975e2'

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--reference',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    a=parser.parse_args()
    if subprocess.check_output(['git','-C',str(a.reference),'rev-parse','HEAD'],text=True).strip()!=PIN:
        raise ValueError('Unexpected ssrecomp pin')
    upstream=subprocess.check_output(['git','-C',str(a.reference),'show',PIN+':frontend/layout.h'])
    if (a.reference/'frontend/layout.h').read_bytes()!=upstream:
        raise ValueError('Reference layout was modified')
    a.output.mkdir(parents=True,exist_ok=False)
    tools=Path(__file__).resolve().parent
    binary=a.output/'touch-reference'
    result=subprocess.run(['g++','-std=c++17','-fsanitize=address,undefined','-fno-sanitize-recover=all',
        '-I'+str(a.reference/'frontend'),'-I'+str(tools/'reference_compat'),'-I/usr/include/SDL2',
        str(tools/'probe_touch_reference.cpp'),'-o',str(binary)],capture_output=True,text=True)
    (a.output/'build.log').write_text(result.stdout+result.stderr); result.check_returncode()
    result=subprocess.run([str(binary)],capture_output=True,text=True,timeout=30)
    (a.output/'run.log').write_text(result.stdout+result.stderr); result.check_returncode()
    report={'passed':True,'coordinate_cases':int(result.stdout),'layouts':6,'window_sizes':8,
            'swap_states':2,'platform':'linux-x86_64','sanitizers':['address','undefined'],
            'reference_pin':PIN,'reference_layout_sha256':hashlib.sha256(upstream).hexdigest(),
            'scope':'native_touch_coordinate_equivalence_with_external_ssrecomp_LayoutTouch',
            'limits':'No rendering, gameplay, merged composition, SDL event loop, high-DPI conversion or Windows touch execution. Reference source remains external.'}
    (a.output/'result.json').write_text(json.dumps(report,indent=2)+'\n'); print(json.dumps(report))

if __name__=='__main__': main()
