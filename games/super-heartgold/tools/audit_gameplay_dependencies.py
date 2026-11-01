"""Compile gameplay entry modules with native flags and map direct dependencies.

This is a symbol census, not an executable link or a count of missing functions.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess
from probe_hgss_link_dependencies import symbols


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('build','hgss','opening-headers','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--message-headers',type=Path,required=True)
    parser.add_argument('--modules',nargs='+',default=['overlay_36','field_system','field_warp_tasks','battle/battle_setup'])
    args=parser.parse_args()
    args.output.mkdir(parents=True,exist_ok=False)
    entries=json.loads((args.build/'compile_commands.json').read_text())
    entry=next(e for e in entries if e['file'].endswith('/intro_movie_scenes_lib.c'))
    command=entry.get('arguments') or shlex.split(entry['command'])
    filtered=[]
    skip=False
    for token in command:
        if skip: skip=False; continue
        if token in ('-o','-MF','-MT','-MQ'):
            skip=True; continue
        if token in ('-MD','-MMD') or token.endswith('intro_movie_scenes_lib.c'): continue
        filtered.append(token)
    modules=[]
    objects=[]
    for module in args.modules:
        source=args.hgss/'src'/f'{module}.c'
        obj=args.output/(module.replace('/','_')+'.o')
        result=subprocess.run(filtered+['-I'+str(args.opening_headers),'-I'+str(args.message_headers),str(source),'-o',str(obj)],
                          cwd=entry['directory'],capture_output=True,text=True,timeout=120)
        (args.output/(obj.stem+'.log')).write_text(result.stdout+result.stderr)
        modules.append({'module':module,'compiled':result.returncode==0,
                        'sha256':hashlib.sha256(source.read_bytes()).hexdigest()})
        if result.returncode==0: objects.append(obj)
    dependencies=set().union(*(symbols(obj,False) for obj in objects))
    internal=set().union(*(symbols(obj,True) for obj in objects))
    dependencies-=internal
    relocatable=False
    if objects:
        result=subprocess.run(['ld','-r','-o',str(args.output/'gameplay-entry.o'),*map(str,objects)],
                              capture_output=True,text=True,timeout=60)
        (args.output/'relocatable-link.log').write_text(result.stdout+result.stderr)
        result.check_returncode()
        relocatable=True
    runtime=args.build/'standalone/libntr'
    exported=symbols(runtime,True)
    definitions={name:[] for name in dependencies}
    for path in sorted((args.hgss/'src').rglob('*.c')):
        text=path.read_text(errors='replace')
        for name in dependencies:
            if name in text and re.search(r'^[\w\s*]+\b'+re.escape(name)+r'\s*\([^;{}]*\)\s*\{',text,re.M):
                definitions[name].append(str(path.relative_to(args.hgss)))
    report={'modules':modules,'all_compiled':all(m['compiled'] for m in modules),
            'relocatable_link_passed':relocatable,
            'executable_link_tested':False,'gameplay_executed':False,
            'runtime_sha256':hashlib.sha256(runtime.read_bytes()).hexdigest(),
            'direct_undefined_symbols':len(dependencies),
            'present_in_diagnostic':len(dependencies & exported),
            'absent_from_diagnostic':len(dependencies-exported),
            'dependencies':[{'symbol':name,'in_diagnostic':name in exported,
                             'candidate_original_sources':definitions[name]} for name in sorted(dependencies)],
            'limits':'Direct undefined symbols only. Diagnostic may export host adapters. Source candidates are lexical matches; availability does not prove ABI, linking, behavior or missing implementation.'}
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='dependencies'}))
    if not report['all_compiled']: raise SystemExit('One or more gameplay modules failed to compile; see logs')

if __name__=='__main__': main()
