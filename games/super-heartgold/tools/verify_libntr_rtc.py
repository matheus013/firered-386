"""Build a pinned external libntr RTC against our native save calendar (no assets)."""
import argparse
from pathlib import Path
import subprocess
import tempfile
PIN = "380a3d7e60de82aced82075e7b3b9acf5f68b1cd"
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source",type=Path,required=True)
    parser.add_argument("--target",choices=("linux","windows"),default="linux")
    parser.add_argument("--sdl-include",type=Path,help="Directory containing SDL2 headers for target toolchain")
    args=parser.parse_args()
    source=args.source.resolve()
    actual=subprocess.check_output(["git","-C",str(source),"rev-parse","HEAD"],text=True).strip()
    if actual!=PIN: parser.error("libntr revision differs from reviewed pin")
    if subprocess.check_output(["git","-C",str(source),"status","--porcelain","--untracked-files=no"]):
        parser.error("libntr tracked files must be unmodified")
    root=Path(__file__).resolve().parents[1]
    prefix="x86_64-w64-mingw32-" if args.target=="windows" else ""
    define="SDK_BUILD_WIN64" if args.target=="windows" else "SDK_BUILD_LINUX"
    with tempfile.TemporaryDirectory(prefix="hg-libntr-rtc-") as tmp:
        folder=Path(tmp)
        objects=[]
        for index,unit in enumerate((root/"tools/probe_libntr_rtc_bridge.c",source/"libraries/rtc/src/convert.c")):
            obj=folder/f"unit{index}.o"
            subprocess.run([prefix+"gcc","-DSDK_PORT","-DSDK_X86","-DSDK_64BIT","-DSDK_VERSION_MAJOR=4",
                "-D"+define,"-I"+str(source/"include"),
                *(["-I"+str(args.sdl_include)] if args.sdl_include else []),"-c",str(unit),"-o",str(obj)],check=True)
            objects.append(str(obj))
        binary=folder/("probe.exe" if prefix else "probe")
        subprocess.run([prefix+"g++","-std=c++20","-static","-I"+str(root/"game/core/include"),
            str(root/"tools/probe_libntr_rtc.cpp"),str(root/"game/core/src/system_info_init.cpp"),
            *objects,"-o",str(binary)],check=True)
        subprocess.run([str(binary)],check=True,timeout=30)
if __name__=="__main__":main()
