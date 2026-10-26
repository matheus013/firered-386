from pathlib import Path
import subprocess,json,argparse,shutil,hashlib
from prepare_hgss_heap import prepare_game_heap, prepare_sdk_heap
from prepare_hgss_overlay_ids import prepare_overlay_ids
from prepare_hgss_graphics import HASHES as GRAPHICS_SOURCES, prepare_graphics
parser=argparse.ArgumentParser(description="Compile complete HGSS translation units; does not link or validate ABI")
parser.add_argument('--hgss',type=Path,required=True)
parser.add_argument('--libntr',type=Path,required=True)
parser.add_argument('--libntrsystem',type=Path,required=True)
parser.add_argument('--build',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--message-headers',type=Path,help='Privately generated msgdata headers')
parser.add_argument('--opening-headers',type=Path,help='Opening indices verified against private resources')
parser.add_argument('--spl-sources',type=Path,help='Private SPL C sources for compatibility evaluation')
parser.add_argument('--verify-spl',action='store_true',help='Test original SPL pool initialization and lists')
parser.add_argument('--verify-scheduler',action='store_true',help='Execute original sys_task.c with ASan/UBSan')
parser.add_argument('--require-all',action='store_true',help='Fail if any selected module fails to compile')
parser.add_argument('--extra-modules',nargs='+',default=[],help='Additional source basenames to compile')
parser.add_argument('--intro-support',action='store_true',help='Compile the verified 42-module intro support set (not the five asset-dependent scenes)')
parser.add_argument('--verify-timer',action='store_true',help='Execute Timer3 register-boundary test with ASan/UBSan')
parser.add_argument('--verify-heap',action='store_true',help='Execute original heap with the native NNS allocator')
parser.add_argument('--verify-particles',action='store_true',help='Test adapted particle slot allocators with ASan/UBSan')
parser.add_argument('--build-windows-heap',action='store_true',help='Also compile the heap test with MinGW; run the EXE separately on Windows')
parser.add_argument('--windows-sdl-include',type=Path,help='Directory containing the target Windows SDL2 headers')
a=parser.parse_args()
if a.verify_spl and not a.spl_sources:parser.error('--verify-spl requires --spl-sources')
if a.spl_sources and len(list(a.spl_sources.glob('spl_*.c')))!=9:parser.error('Expected nine private SPL C sources')
if a.build_windows_heap and not a.verify_heap:parser.error('--build-windows-heap requires --verify-heap')
if a.build_windows_heap and not a.windows_sdl_include:parser.error('--build-windows-heap requires --windows-sdl-include')
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
overlay_contract=prepare_overlay_ids(ref/'main.lsf',out)
flags+=['-include',str(out/'hgss_overlay_ids.h')]
if a.message_headers:flags+=['-I'+str(a.message_headers.resolve())]
if a.opening_headers:flags+=['-I'+str(a.opening_headers.resolve())]
for p in [out,headers,headers/'library',a.libntr/'include',a.libntrsystem/'include',a.build/'gen',a.build/'gen/nitro/fx']:flags+=['-I'+str(p)]
result=subprocess.run(flags+[str(out/'overlay-contract.c'),'-o',str(out/'overlay-contract')],
                      capture_output=True,text=True,timeout=60)
(out/'overlay-contract-build.log').write_text(result.stdout+result.stderr)
result.check_returncode()
result=subprocess.run([str(out/'overlay-contract')],capture_output=True,text=True,check=True,timeout=10)
overlay_contract['native_layout']=json.loads(result.stdout)
overlay_contract['rom_layout']={'overlay_header_bytes':32,'ram_address_offset':4,'file_id_offset':24}
overlay_contract['direct_rom_struct_copy_compatible']=all(
 overlay_contract['native_layout'][key]==value for key,value in overlay_contract['rom_layout'].items())
overlay_contract['ids_compile_time_checked']=True
overlay_contract['native_overlay_loading_tested']=False
(out/'overlay-contract.json').write_text(json.dumps(overlay_contract,indent=2)+'\n')
report=[]
prepare_game_heap(ref/'src/heap.c',out/'heap-native.c')
modules=['gf_rtc','heap','task','sys_task','overlay_manager','gf_gfx_planes','bg_window','pokemon','save','save_arrays','main','system','render_text','font','palette','message_format','msgdata']
if a.verify_timer:modules.append('timer3')
if a.verify_particles:modules.append('particle_system')
if a.intro_support:
 modules += ['filesystem','pm_string','brightness','poke_overlay','timer3','touchpad',
             'swap_3d_buffers','math_util','igt_counter','string_control_code','intro_movie',
             'sys_task_api','screen_fade','gf_3d_render','camera','gf_gfx_loader',
             'obj_char_transfer','obj_pltt_transfer','oam_manager','sprite',
             'intro_movie_scenes_lib','gf_g2d_renderer','g2d_cell_culling',
             'screen_fade_effects','screen_window_wipe']
for name in dict.fromkeys(modules+a.extra_modules):
 if not name.isidentifier():raise ValueError('Invalid module basename: '+name)
 source=ref/'src'/f'{name}.c'
 if not source.exists():raise FileNotFoundError(source)
 if name=='heap':source=out/'heap-native.c'
 if name in GRAPHICS_SOURCES:
  adapted=out/f'{name}-native.c'
  prepare_graphics(source,adapted)
  source=adapted
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
if a.spl_sources:
 for source in sorted(a.spl_sources.glob('spl_*.c')):
  result=subprocess.run(flags+['-c',str(source),'-o',str(out/(source.stem+'.o'))],capture_output=True,text=True,timeout=60)
  (out/(source.stem+'.log')).write_text(result.stderr)
  report.append({'module':source.stem,'compiled':result.returncode==0,
    'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
    'first_errors':[line.strip() for line in result.stderr.splitlines() if 'error:' in line][:3],
    'pointer_cast_warnings':sum('different size' in line for line in result.stderr.splitlines())})
print(json.dumps(report,indent=2));(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
if a.verify_scheduler:
 executable=out/'scheduler-test'
 command=flags+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-omit-frame-pointer',
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
 command=flags+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-omit-frame-pointer',
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
if a.verify_heap:
 executable=out/'heap-test'
 fnd=a.libntrsystem/'libraries/fnd'
 project=Path(__file__).resolve().parents[1]
 prepare_sdk_heap(fnd/'src/expheap.c',out/'expheap-native.c')
 command=flags+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all',
   '-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-DNNS_FINALROM','-DSDK_FINALROM',
   '-I'+str(fnd/'include'),str(out/'heap-native.c'),str(out/'expheap-native.c'),str(ref/'src/pm_string.c'),
   str(ref/'src/overlay_manager.c'),str(ref/'src/gf_gfx_planes.c'),
   '-I'+str(project/'game/native/include'),'-I'+str(project/'tools/reference_compat'),
   str(project/'game/native/src/native_overlays.c'),
   str(project/'tools/reference_compat/hgss_native_overlay_bridge.c'),
   *[str(fnd/'src'/name) for name in ['heapcommon.c','list.c']],
   str(Path(__file__).with_name('probe_hgss_heap.c')),'-o',str(executable)]
 result=subprocess.run(command,capture_output=True,text=True,timeout=60)
 (out/'heap-build.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 result=subprocess.run([str(executable)],capture_output=True,text=True,timeout=30)
 (out/'heap-run.log').write_text(result.stdout+result.stderr)
 print(result.stdout+result.stderr)
 result.check_returncode()
 # Missing native registrations must stop before application callbacks run.
 # Disable core dumps for this deliberate abort.
 import resource
 def disable_core_dump():resource.setrlimit(resource.RLIMIT_CORE,(0,0))
 rejection=subprocess.run([str(executable),'--reject-unknown-module'],capture_output=True,
                          text=True,timeout=30,preexec_fn=disable_core_dump)
 (out/'overlay-rejection.log').write_text(rejection.stdout+rejection.stderr)
 if rejection.returncode!=-6 or 'Native overlay load failed: id=2147483645 result=2' not in rejection.stderr:
  raise RuntimeError('Unknown overlay was not rejected through the expected fatal path')
 (out/'heap-result.json').write_text(json.dumps({'passed':True,'allocation_cycles':32000,
   'string_cycles':1000,'direct_sdk_alignment_cases':1024,'sanitizers':['address','undefined'],
   'application_lifecycle_cycles':1000,'display_register_boundary':'test register',
   'native_overlay_loading_tested':True,
   'native_overlay_test_scope':'registered test fixture only; no game overlays',
   'registered_module_cycles':1000,
   'unknown_module_fails_before_application_callback':True,
   'fail_on_first_undefined_behavior':True,'os_boundary':'test arena and interrupt hooks',
   'allocator':'original NNS expandable heap with native alignment adaptation',
   'game_sources':['heap.c (native alignment adaptation)','pm_string.c (unchanged)',
                   'overlay_manager.c (unchanged)','gf_gfx_planes.c (unchanged)'],
   'graphics_resource_release_tested':False},indent=2)+'\n')
 if a.build_windows_heap:
  windows_command=[arg.replace('-DSDK_BUILD_LINUX','-DSDK_BUILD_WIN64') for arg in command
                   if arg not in ['-fsanitize=address,undefined','-fno-sanitize-recover=all']]
  windows_command[0]='x86_64-w64-mingw32-gcc'
  windows_command.insert(1,'-I'+str(a.windows_sdl_include.resolve()))
  windows_command.insert(1,'-DSDL_MAIN_HANDLED')
  # COFF unwind sections can retain unused functions despite gc-sections.
  # LTO removes these before link without inventing implementations for them.
  windows_command[1:1]=['-flto','-O1']
  windows_command[-1]=str(out/'heap-test.exe')
  result=subprocess.run(windows_command,capture_output=True,text=True,timeout=60)
  (out/'heap-windows-build.log').write_text(result.stdout+result.stderr)
  result.check_returncode()
  print('Windows heap executable compiled; native Windows execution is a separate check.')
if a.verify_spl:
 executable=out/'spl-test'
 command=flags+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all',
   '-ffunction-sections','-fdata-sections','-Wl,--gc-sections',
   str(a.spl_sources/'spl_manager.c'),str(a.spl_sources/'spl_list.c'),
   str(Path(__file__).with_name('probe_hgss_spl.c')),'-o',str(executable)]
 result=subprocess.run(command,capture_output=True,text=True,timeout=60)
 (out/'spl-build.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 result=subprocess.run([str(executable)],capture_output=True,text=True,timeout=30)
 (out/'spl-run.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 print(result.stdout)
 (out/'spl-result.json').write_text(json.dumps({'passed':True,'cycles':100,
   'emitter_pool_size':20,'particle_pool_size':200,'sanitizers':['address','undefined'],
   'boundary':'malloc and byte fill','rendering_tested':False,'resource_loading_tested':False},indent=2)+'\n')
if a.verify_particles:
 executable=out/'particle-test'
 command=flags+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all',
                '-ffunction-sections','-fdata-sections','-Wl,--gc-sections',
                str(Path(__file__).with_name('probe_hgss_particles.c')),'-o',str(executable)]
 result=subprocess.run(command,capture_output=True,text=True,timeout=60)
 (out/'particle-build.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 result=subprocess.run([str(executable)],capture_output=True,text=True,timeout=30)
 (out/'particle-run.log').write_text(result.stdout+result.stderr)
 result.check_returncode()
 print(result.stdout)
 (out/'particle-result.json').write_text(json.dumps({'passed':True,'slots':16,'cases':2032,
   'sanitizers':['address','undefined'],'scope':'slot allocator alignment and content preservation',
   'rendering_tested':False,'vram_state_runtime_tested':False},indent=2)+'\n')
if a.require_all and any(not module['compiled'] for module in report):
 raise SystemExit('Some HGSS modules failed to compile; see report.json')
