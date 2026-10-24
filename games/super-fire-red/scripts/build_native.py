"""Compile the shared game C sources for a 32-bit desktop host, without ARM emulation."""
from pathlib import Path
import argparse, concurrent.futures, hashlib, os, re, shutil, subprocess, sys

ROOT=Path(__file__).resolve().parents[3]
parser=argparse.ArgumentParser();parser.add_argument('--platform',choices=['linux','windows'],default='linux');args=parser.parse_args()

cache=Path(os.environ.get('XDG_CACHE_HOME',Path.home()/'.cache'))/'firered-386'/('native-'+args.platform)
source=cache/'source';objects=cache/'objects';logs=ROOT/'games/super-fire-red/output/native-logs'/args.platform
cache.mkdir(parents=True,exist_ok=True);objects.mkdir(exist_ok=True);logs.mkdir(parents=True,exist_ok=True)
gba=Path((ROOT/'games/super-fire-red/output/hack-build-path.txt').read_text(encoding='utf-8').strip())
assert (gba/'tools/preproc/preproc').exists(),'Run make gba first'
if source.exists():
    assert source.parent==cache and source.name=='source'
    shutil.rmtree(source)
shutil.copytree(gba,source,ignore=shutil.ignore_patterns('build','.git','tools/agbcc','*.elf','*.map','*.sym'))
for p in source.rglob('*'):
    if p.is_file() and p.suffix in {'.c','.h','.inc','.s','.ld','.txt','.json'}:
        data=p.read_bytes()
        if b'\r\n' in data:p.write_bytes(data.replace(b'\r\n',b'\n'))
patch=cache/'port.patch';patch.write_bytes((ROOT/'games/super-fire-red/native/port.patch').read_bytes().replace(b'\r\n',b'\n'))
subprocess.run(['git','apply','--check',str(patch)],cwd=source,check=True)
subprocess.run(['git','apply',str(patch)],cwd=source,check=True)
shutil.copytree(ROOT/'games/super-fire-red/native/runtime',source,dirs_exist_ok=True)

def preserve_apcs_struct_alignment(path):
    # agbcc uses the old APCS ABI: unpacked aggregates have minimum 4-byte
    # alignment, including structs containing only bytes/halfwords. x86's ABI
    # differs; preserve save layouts and the binary data tables assembled below.
    content=path.read_text(encoding='utf-8')
    mask=re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*',
                lambda m:' '*len(m[0]), content, flags=re.S)
    insertions=[]
    for match in re.finditer(r'\b(struct|union)(\s+(?:[A-Za-z_]\w*\s*)?)\{',mask):
        depth=1;end=match.end()
        while end<len(mask) and depth:
            if mask[end]=='{':depth+=1
            elif mask[end]=='}':depth-=1
            end+=1
        suffix=mask[end:mask.find(';',end)+1]
        if 'packed' not in suffix:
            insertions.append(match.start()+len(match[1]))
    for offset in reversed(insertions):
        content=content[:offset]+' __attribute__((aligned(4)))'+content[offset:]
    path.write_text(content,encoding='utf-8',newline='\n')

for folder in ['src','include']:
    for p in (source/folder).rglob('*'):
        if p.is_file() and p.suffix in {'.c','.h'}:preserve_apcs_struct_alignment(p)

def asm_text(text):
    # These files contain game data and script bytecode, never ARM instructions.
    lines=[]
    for line in text.splitlines():
        quoted=False;escaped=False;end=len(line)
        for i,c in enumerate(line):
            if c=='"' and not escaped:quoted=not quoted
            if c=='@' and not quoted and not escaped:end=i;break
            escaped=(c=='\\' and not escaped)
        line=line[:end]
        if args.platform=='windows':line=re.sub(r'(\.section\s+[^,]+),.*',r'\1',line)
        if args.platform=='windows' and re.match(r'\s*\.(size|type)\b',line):continue
        line=re.sub(r'\(MAX_COINS \+ 1\) - (\d+)',r'(MAX_COINS+1)-\1',line)
        line=line.replace('.4byte','.long').replace('.2byte','.short')
        line=re.sub(r'\.word\b','.long',line)
        line=re.sub(r'\.hword\b','.short',line)
        line=re.sub(r'\.align\s+(\d+)',lambda m:'.balign '+str(1<<int(m[1])),line)
        if re.match(r'\s*\.(?:thumb|thumb_func|arm|syntax|fpu|arch)\b',line):continue
        lines.append(line)
    return '\n'.join(lines)+'\n'

for folder in ['asm','data','constants','sound']:
    for p in (source/folder).rglob('*'):
        if p.suffix not in {'.inc','.s'}:continue
        p.write_text(asm_text(p.read_text(encoding='utf-8')),newline='\n')

windows=args.platform=='windows'
sdk=cache.parent/'sdl2-mingw/SDL2-2.30.11/i686-w64-mingw32'
if windows and not (sdk/'bin/SDL2.dll').exists():
    raise SystemExit('Install the pinned SDL2 MinGW SDK first (games/super-fire-red/scripts/setup_native.sh).')
cc='i686-w64-mingw32-gcc' if windows else 'gcc'
defines=['-DNONMATCHING','-DPORTABLE','-DPLATFORM_SDL2','-DRENDERER_EASY_DRAW','-DFIRERED_DESKTOP','-DFIRERED=1','-DREVISION=0','-DENGLISH=1','-DMODERN=1','-DUBFIX']
includes=['-iquote','include','-I'+str(sdk/'include' if windows else '/usr/include/SDL2'),'-D_REENTRANT']
if windows:defines+=['-DSDL_MAIN_HANDLED']
flags=['-m32','-std=gnu99','-O2','-fcommon','-fno-pie','-fno-builtin','-fno-strict-aliasing','-ffunction-sections','-fdata-sections','-Wno-implicit-function-declaration','-Wno-int-conversion','-Wno-incompatible-pointer-types','-Wno-attributes']
if windows:flags+=['-mno-ms-bitfields']
preproc=source/'tools/preproc/preproc'

def run(command, data=None):
    r=subprocess.run(command,cwd=source,input=data,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    if r.returncode:raise RuntimeError(' '.join(map(str,command))+'\n'+r.stderr.decode(errors='replace')[-12000:])
    return r.stdout

def compile_c(p):
    name=p.relative_to(source).as_posix();tag=name.replace('/','_');obj=objects/(tag+'.o');stamp=objects/(tag+'.sha256')
    try:
        expanded=run([cc,*flags,*includes,*defines,'-E',name])
        converted=run([str(preproc),'-i',name,'charmap.txt'],expanded)
        digest=hashlib.sha256(converted+' '.join([cc,*flags]).encode()).hexdigest()
        if not obj.exists() or not stamp.exists() or stamp.read_text(encoding='utf-8')!=digest:
            run([cc,*flags,'-x','cpp-output','-c','-o',str(obj),'-'],converted)
            stamp.write_text(digest)
        return obj,None
    except Exception as error:
        (logs/(tag+'.log')).write_text(str(error));return obj,str(error)

files=sorted((source/'src').glob('*.c'))+sorted((source/'src/platform').glob('*.c'))
errors=[];compiled=[]
with concurrent.futures.ThreadPoolExecutor(max_workers=6) as pool:
    for obj,error in pool.map(compile_c,files):
        if error:errors.append(error)
        else:compiled.append(obj)
print('Native C:',len(compiled),'compiled;',len(errors),'errors',flush=True)
if errors:
    print('\n\n'.join(errors[:6]));raise SystemExit(1)

specials=source/'native_special_ids.inc'
names=re.findall(r'^\s+def_special\s+(\w+)',(source/'data/specials.inc').read_text(encoding='utf-8'),re.M)
specials.write_text(''.join(f'.set SPECIAL_{name}, {i}\n' for i,name in enumerate(names)))
asm_files=sorted((source/'data').glob('*.s'))+sorted((source/'sound/songs').rglob('*.s'))
for p in asm_files:
    name=p.relative_to(source).as_posix();tag=name.replace('/','_');obj=objects/(tag+'.o')
    try:
        if name.startswith('sound/'):
            converted=asm_text(p.read_text(encoding='utf-8')).encode()
        else:
            first=run([str(preproc),name,'charmap.txt'])
            second=run([cc,'-E','-x','c','-Iinclude','-DPORTABLE','-DUBFIX','-'],first)
            third=run([str(preproc),'-ie',name,'charmap.txt'],second)
            converted=b'.include "native_special_ids.inc"\n'+asm_text(third.decode()).encode()
        stamp=objects/(tag+'.sha256');digest=hashlib.sha256(converted).hexdigest()
        # Included script files also change; assembling is cheap compared with C.
        run([cc,'-m32','-c','-x','assembler','-Wa,-I.,-Isound,--defsym,FIRERED=1,--defsym,REVISION=0,--defsym,ENGLISH=1,--defsym,MODERN=1','-o',str(obj),'-'],converted)
        if windows:run(['i686-w64-mingw32-objcopy','--prefix-symbols=_',str(obj)])
        compiled.append(obj);stamp.write_text(digest)
    except Exception as error:
        (logs/(tag+'.log')).write_text(str(error));errors.append(str(error))
print('Native data:',len(asm_files)-len(errors),'assembled;',len(errors),'errors',flush=True)
if errors:print('\n\n'.join(errors[:3]));raise SystemExit(1)
out=ROOT/'games/super-fire-red/output/pc'/args.platform;out.mkdir(parents=True,exist_ok=True)
exe='firered-386.exe' if windows else 'firered-386'
response=cache/'objects.rsp';response.write_text('\n'.join(str(p) for p in compiled))
link=[cc,'-m32','-Wl,--gc-sections','-o',str(cache/exe),'@'+str(response)]
link+=['-L'+str(sdk/'lib'),'-lSDL2','-lxinput9_1_0','-lm'] if windows else ['-no-pie','-L/usr/lib/i386-linux-gnu','-lSDL2','-lm']
try:run(link)
except Exception as error:(logs/'link.log').write_text(str(error));print(str(error));raise SystemExit(1)
shutil.copy2(cache/exe,out/exe)
if windows:
    shutil.copy2(sdk/'bin/SDL2.dll',out/'SDL2.dll')
    shutil.copy2(sdk.parent/'LICENSE.txt',out/'SDL2-LICENSE.txt')
shutil.copytree(ROOT/'games/super-fire-red/native/licenses',out/'licenses',dirs_exist_ok=True)
print('Built native PC game:',out/exe,flush=True)
