"""Compile pinned HGSS display routing functions against external libntr headers.
Generated original source stays in a temporary directory. No complete game.
"""
import argparse,hashlib,subprocess,tempfile
from pathlib import Path
HASH="cab1ef384b3c232e9b47b76cabfca29e14eec0f57084d10f7429c71faadc2740"
PRE='#include <nitro.h>\n#include <assert.h>\n#include <stdio.h>\nvoid SingleScreen_ShowSubEngine(BOOL show);\nREGType16v s_reg_GX_POWCNT;\n'
MAIN='int main(void) {\n for(unsigned initial=0;initial<65536;++initial) {\n  s_reg_GX_POWCNT=initial;\n  SingleScreen_ShowSubEngine(0);\n  assert(!SingleScreen_IsSubEngineShown());\n  assert(s_reg_GX_POWCNT==(initial & 0x7fff));\n  assert(gSingleScreenBuildInfo.subEngineShown==0);\n  SingleScreen_ShowSubEngine(1);\n  assert(SingleScreen_IsSubEngineShown());\n  assert(s_reg_GX_POWCNT==(initial | 0x8000));\n  assert(gSingleScreenBuildInfo.subEngineShown==1);\n }\n SingleScreen_SetTwoScreenScene(1);\n assert(gSingleScreenBuildInfo.twoScreenScene==1);\n SingleScreen_SetTwoScreenScene(0);\n assert(gSingleScreenBuildInfo.twoScreenScene==0);\n SingleScreen_SetBattleMenu(2,3);\n assert(gSingleScreenBuildInfo.battleMenu==2 && gSingleScreenBuildInfo.battleSelection==3);\n puts("HGSS original single-screen functions passed against libntr headers: 65536 register states");\n}\n'
def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--hgss',type=Path,required=True)
 p.add_argument('--libntr',type=Path,required=True)
 p.add_argument('--build',type=Path,required=True)
 a=p.parse_args()
 pin=subprocess.check_output(["git","-C",str(a.libntr),"rev-parse","HEAD"],text=True).strip()
 if pin!="380a3d7e60de82aced82075e7b3b9acf5f68b1cd":p.error("libntr revision differs")
 data=(a.hgss/'src/gf_gfx_planes.c').read_bytes()
 if hashlib.sha256(data).hexdigest()!=HASH:p.error('HGSS source hash differs')
 source=data.decode()
 body=source[source.index('const volatile u16 gSingleScreenDispSelectMask'):source.index('BOOL SingleScreen_HandleSwapKey')]
 with tempfile.TemporaryDirectory(prefix='hgss-display-') as d:
  folder=Path(d);c=folder/'probe.c';exe=folder/'probe'
  c.write_text(PRE+body+MAIN)
  subprocess.run(['gcc','-DSDK_PORT','-DSDK_X86','-DSDK_BUILD_LINUX','-DSDK_64BIT','-DSDK_VERSION_MAJOR=4',
   '-I'+str(a.libntr/'include'),'-I'+str(a.build/'gen'),'-I'+str(a.build/'gen/nitro/fx'),
   '-fsanitize=address,undefined',str(c),'-o',str(exe)],check=True)
  subprocess.run([str(exe)],check=True,timeout=30)
if __name__=='__main__':main()
