"""Compile the actual native window helper against explicit register fixtures."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile
from prepare_libntr_scanline_windows import HEADER

FIXTURES = r'''
#include "window.h"
#include <cassert>
HGWindowLine hg_window_lines[2][192];
int main() {
    for (unsigned engine=0; engine<2; ++engine) {
        auto &s=hg_window_lines[engine][40];
        s={0,0,0,0,0,0,0};
        assert(HG_WindowPass(engine,20,40,1)); // Disabled windows do not mask.
        s={1u<<14,0,0x0a1e,0,0x1e32,0x0200,0x10};
        assert(HG_WindowPass(engine,10,40,1)); // WIN1 independent of WIN0.
        assert(!HG_WindowPass(engine,30,40,1)); // Exclusive right edge.
        assert(HG_ObjWindow0Pass(engine,30,40)); // Outside OBJ mask.
        s.display=3u<<13; s.h0=0x1428; s.v0=0x1e32;
        assert(!HG_WindowPass(engine,20,40,1)); // WIN0 wins overlapping WIN1.
        assert(HG_WindowPass(engine,19,40,1));
        s.inside=0x0210;
        assert(HG_ObjWindow0Pass(engine,20,40));
        assert(!HG_ObjWindow0Pass(engine,19,40));
        s.h0=0x0100; s.h1=0x0001; s.inside=0x0202;
        for (unsigned x=0; x<256; ++x) assert(HG_WindowPass(engine,x,40,1));
        s.display=1u<<13; s.h0=0xf00a;
        assert(HG_WindowPass(engine,255,40,1));
        assert(HG_WindowPass(engine,0,40,1));
        assert(!HG_WindowPass(engine,10,40,1));
        s.h0=0x1414;
        assert(!HG_WindowPass(engine,20,40,1)); // Equal edges are empty.
        s.h0=0x001e; s.v0=0xb432;
        assert(HG_WindowPass(engine,20,40,1)); // Vertical wrap.
        s.v0=0x0028;
        assert(!HG_WindowPass(engine,20,40,1)); // Exclusive bottom edge.
    }
}
'''

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    with tempfile.TemporaryDirectory() as folder:
        path=Path(folder)
        (path/'nitro.h').write_text('#include <cstdint>\nusing u32=uint32_t; using u16=uint16_t;\n')
        (path/'window.h').write_text(HEADER)
        (path/'test.cpp').write_text(FIXTURES)
        subprocess.run(['c++','-std=c++17','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-I',folder,str(path/'test.cpp'),'-o',str(path/'test')],check=True)
        subprocess.run([str(path/'test')],check=True)
    report={'passed':True,'scope':'native_window_helper_register_fixtures','engines':2,
            'checks_per_engine':270,'sanitizers':['address','undefined'],
            'limits':'No OBJ window, 3D or hardware timing validation'}
    args.output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))

if __name__=='__main__': main()
