from pathlib import Path
import subprocess,json,argparse,shutil
parser=argparse.ArgumentParser(description="Compile complete HGSS translation units; does not link or validate ABI")
parser.add_argument('--hgss',type=Path,required=True)
parser.add_argument('--libntr',type=Path,required=True)
parser.add_argument('--libntrsystem',type=Path,required=True)
parser.add_argument('--build',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--message-headers',type=Path,help='Privately generated msgdata headers')
parser.add_argument('--verify-scheduler',action='store_true',help='Execute original sys_task.c with ASan/UBSan')
parser.add_argument('--require-all',action='store_true',help='Fail if any selected module fails to compile')
parser.add_argument('--extra-modules',nargs='+',default=[],help='Additional source basenames to compile')
parser.add_argument('--verify-timer',action='store_true',help='Execute Timer3 register-boundary test with ASan/UBSan')
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
flags=['gcc','-std=gnu99','-DPM_KEEP_ASSERTS','-DOS_IE_VBLANK=OS_IE_V_BLANK','-DOS_IE_HBLANK=OS_IE_H_BLANK','-DSOULSILVER','-DENGLISH','-DSDK_PORT','-DSDK_X86','-DSDK_BUILD_LINUX','-DSDK_64BIT','-DSDK_VERSION_MAJOR=4']
flags+=['-include',str(out/'hgss_backup.h')]
if a.message_headers:flags+=['-I'+str(a.message_headers.resolve())]
for p in [out,headers,a.libntr/'include',a.libntrsystem/'include',a.build/'gen',a.build/'gen/nitro/fx']:flags+=['-I'+str(p)]
report=[]
modules=['gf_rtc','heap','task','sys_task','overlay_manager','gf_gfx_planes','bg_window','pokemon','save','save_arrays','main','system','render_text','font','palette','message_format','msgdata']
if a.verify_timer:modules.append('timer3')
for name in dict.fromkeys(modules+a.extra_modules):
 if not name.isidentifier():raise ValueError('Invalid module basename: '+name)
 source=ref/'src'/f'{name}.c'
 if not source.exists():raise FileNotFoundError(source)
 if name=='timer3':
  original='*(REGType16 *)((u32)(REG_TM0CNT_L_ADDR + 4 * OS_TIMER_3))'
  content=source.read_text()
  if content.count(original)!=1:raise ValueError('Timer3 source contract differs')
  source=out/'timer3-native.c'
  source.write_text(content.replace(original,'reg_OS_TM3CNT_L'))
 result=subprocess.run(flags+['-c',str(source),'-o',str(out/f'{name}.o')],capture_output=True,text=True,timeout=60)
 (out/f'{name}.log').write_text(result.stderr)
 errors=[line.strip() for line in result.stderr.splitlines() if 'error:' in line]
 report.append({'module':name,'compiled':result.returncode==0,'first_errors':errors[:3],'pointer_cast_warnings':sum('different size' in line for line in result.stderr.splitlines())})
print(json.dumps(report,indent=2));(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
if a.verify_scheduler:
 executable=out/'scheduler-test'
 command=flags+['-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',
                str(ref/'src/sys_task.c'),str(Path(__file__).with_name('probe_hgss_scheduler.c')),
                '-o',str(executable)]
 result=subprocess.run(command,capture_output=True,text=True,timeout=60)
 (out/'scheduler-build.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 result=subprocess.run([str(executable)],capture_output=True,text=True,timeout=30)
 (out/'scheduler-run.log').write_text(result.stdout+result.stderr)
 print(result.stdout+result.stderr)
 result.check_returncode()
 (out/'scheduler-result.json').write_text(json.dumps({'passed':True,'original_module':'sys_task.c',
   'sanitizers':['address','undefined'],'recycle_iterations':10000,'campaign_tested':False},indent=2)+'\n')
if a.verify_timer:
 executable=out/'timer-test'
 command=flags+['-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',
                '-ffunction-sections','-fdata-sections','-Wl,--gc-sections',
                str(out/'timer3-native.c'),str(Path(__file__).with_name('probe_hgss_timer.c')),
                '-o',str(executable)]
 result=subprocess.run(command,capture_output=True,text=True,timeout=60)
 (out/'timer-build.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 result=subprocess.run([str(executable)],capture_output=True,text=True,timeout=30)
 (out/'timer-run.log').write_text(result.stdout+result.stderr)
 print(result.stdout+result.stderr)
 result.check_returncode()
 (out/'timer-result.json').write_text(json.dumps({'passed':True,'samples':131072,
   'sanitizers':['address','undefined'],'hardware_boundary':'test registers',
   'real_time_accuracy_tested':False},indent=2)+'\n')
if a.require_all and any(not module['compiled'] for module in report):
 raise SystemExit('Some HGSS modules failed to compile; see report.json')
