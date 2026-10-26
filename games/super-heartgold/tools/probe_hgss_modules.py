from pathlib import Path
import subprocess,json,argparse,shutil
parser=argparse.ArgumentParser(description="Compile complete HGSS translation units; does not link or validate ABI")
parser.add_argument('--hgss',type=Path,required=True)
parser.add_argument('--libntr',type=Path,required=True)
parser.add_argument('--libntrsystem',type=Path,required=True)
parser.add_argument('--build',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
a=parser.parse_args()
ref=a.hgss.resolve();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
compat=Path(__file__).resolve().parent/'reference_compat/hgss_sdk'
shutil.copytree(compat,out,dirs_exist_ok=True)
headers=out/'hgss-include'
shutil.copytree(ref/'include',headers)
# Host SDK already owns these declarations. Remove only the redundant ones
# from a private header copy, retaining all game structs and functions.
for filename,declarations in {
 'field/map_prop_render.h':[
  'typedef struct NNSG3dMatAnmResult NNSG3dMatAnmResult;',
  'typedef void (*NNSG3dFuncAnmMat)(NNSG3dMatAnmResult *, const NNSG3dAnmObj *, u32);'],
 'field/field_fog.h':['void G3X_SetFogTable(u32 *fogTable);']
}.items():
 header=headers/filename;text=header.read_text()
 for declaration in declarations:
  if text.count(declaration)!=1:raise ValueError('Header contract differs: '+filename)
  text=text.replace(declaration,'/* Declaration supplied by native SDK. */')
 header.write_text(text)
flags=['gcc','-std=gnu99','-DOS_IE_VBLANK=OS_IE_V_BLANK','-DOS_IE_HBLANK=OS_IE_H_BLANK','-DSOULSILVER','-DENGLISH','-DSDK_PORT','-DSDK_X86','-DSDK_BUILD_LINUX','-DSDK_64BIT','-DSDK_VERSION_MAJOR=4']
for p in [out,headers,a.libntr/'include',a.libntrsystem/'include',a.build/'gen',a.build/'gen/nitro/fx']:flags+=['-I'+str(p)]
report=[]
for name in ['gf_rtc','heap','narc','task','sys_task','overlay_manager','gf_gfx_planes','bg_window','pokemon','save','save_arrays','main','system','render_text','font','palette','message_format','msgdata']:
 source=ref/'src'/f'{name}.c'
 if not source.exists():continue
 result=subprocess.run(flags+['-c',str(source),'-o',str(out/f'{name}.o')],capture_output=True,text=True,timeout=60)
 (out/f'{name}.log').write_text(result.stderr)
 errors=[line.strip() for line in result.stderr.splitlines() if 'error:' in line]
 report.append({'module':name,'compiled':result.returncode==0,'first_errors':errors[:3],'pointer_cast_warnings':sum('different size' in line for line in result.stderr.splitlines())})
print(json.dumps(report,indent=2));(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
