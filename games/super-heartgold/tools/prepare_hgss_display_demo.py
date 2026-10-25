"""Prepare private HGSS routing demo sources; does not modify the dependency checkout."""
import argparse,hashlib,subprocess
from pathlib import Path
from verify_hgss_libntr_display import HASH
PIN='380a3d7e60de82aced82075e7b3b9acf5f68b1cd'
def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--hgss',type=Path,required=True)
 p.add_argument('--libntr',type=Path,required=True)
 p.add_argument('--output',type=Path,required=True)
 a=p.parse_args()
 data=(a.hgss/'src/gf_gfx_planes.c').read_bytes()
 if hashlib.sha256(data).hexdigest()!=HASH:p.error('HGSS source hash differs')
 source=data.decode()
 body=source[source.index('const volatile u16 gSingleScreenDispSelectMask'):source.index('BOOL SingleScreen_HandleSwapKey')]
 demo=subprocess.check_output(['git','-C',str(a.libntr),'show',PIN+':standalone/main.c']).decode()
 demo=demo.replace('void NitroMain(void) {','void SingleScreen_ShowSubEngine(BOOL show);\nvoid NitroMain(void) {\n  unsigned routeFrame = 0;')
 demo=demo.replace('  DEMOStartDisplay();','  DEMOStartDisplay();\n  GXS_SetGraphicsMode(GX_BGMODE_0);\n  GXS_SetVisiblePlane(0);\n  GXS_DispOn();\n  u16 subColor = GX_RGB(0,0,31);\n  GXS_LoadBGPltt(&subColor, 0, sizeof(subColor));\n  SingleScreen_ShowSubEngine(FALSE);')
 demo=demo.replace('    G3X_Reset();','    if (++routeFrame == 40) SingleScreen_ShowSubEngine(TRUE);\n    if (routeFrame == 80) SingleScreen_ShowSubEngine(FALSE);\n    G3X_Reset();')
 a.output.mkdir(parents=True,exist_ok=False)
 (a.output/'hgss_display.c').write_text('#include <nitro.h>\nvoid SingleScreen_ShowSubEngine(BOOL show);\n'+body)
 (a.output/'main.c').write_text(demo)
 print('Private sources prepared; link both C units to nitrosdk_dep in the diagnostic build.')
if __name__=='__main__':main()
