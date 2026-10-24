"""Prepare an isolated original title decision function and overlay manager.

Downloads are pinned and private. Only TitleScreen_Main is selected from the
large title translation unit; graphics/audio implementations are NOT included.
"""
import argparse
import hashlib
from pathlib import Path
import urllib.request

PIN = '6984a3d67592157195940049be2e4bbbf933f9ff'
FILES = {
    'src/title_screen.c': '138232b1af55084d54408888bf53dcf49dce1316584cf214d06826099781b650',
    'src/overlay_manager.c': '9fccc34260dd4099b54b463b93cc909dd9c0728b13fce216db92a517d63bc029',
    'include/overlay_manager.h': '8ef3461aae09ab92063af9ef29b6094e5b7a3b1f9e1dba83db4aeb068f2762bf',
}


def prepare(destination):
    destination = destination.resolve()
    project = Path(__file__).resolve().parents[1]
    if destination.is_relative_to(project) or any((p/'.git').exists() for p in (destination,*destination.parents)):
        raise ValueError('Title sources require a private build folder outside Git')
    destination.mkdir(parents=True,exist_ok=True)
    for relative, digest in FILES.items():
        target = destination/Path(relative).name
        if target.exists() and hashlib.sha256(target.read_bytes()).hexdigest() == digest:
            continue
        with urllib.request.urlopen(f'https://raw.githubusercontent.com/ClickingMaster/pokeheartgold-single-screen/{PIN}/{relative}',timeout=60) as response:
            payload = response.read()
        if hashlib.sha256(payload).hexdigest() != digest:
            raise ValueError(f'Pinned source differs: {relative}')
        target.write_bytes(payload)
    source = (destination/'title_screen.c').read_text()
    start = source.index('enum TitleScreenMainState {')
    enum = source[start:source.index('};',start)+2]
    start = source.index('static BOOL TitleScreen_Main(OverlayManager *man, int *state) {')
    end = source.index('\nstatic BOOL TitleScreen_Exit(',start)
    function = source[start:end].rstrip()
    wrapper = '''
static const OverlayManagerTemplate native_title = {
    HGTitle_InitCallback, TitleScreen_Main, HGTitle_ExitCallback, FS_OVERLAY_ID_NONE
};
OverlayManager *HGTitle_Create(void) { return OverlayManager_New(&native_title, NULL, HG_HOST_TEST_HEAP); }
BOOL HGTitle_Run(OverlayManager *manager) { return OverlayManager_Run(manager); }
int HGTitle_Phase(OverlayManager *manager) { return manager->proc_state; }
void HGTitle_Destroy(OverlayManager *manager) {
    if (OverlayManager_GetData(manager)) OverlayManager_FreeData(manager);
    OverlayManager_Delete(manager);
}
'''
    selected = '#include "overlay_manager.h"\n#include "title_flow_bridge.h"\n'+enum+'\n'+function+'\n'+wrapper
    (destination/'title_decision.c').write_text(selected)
    print('Private original title decision SHA256:',hashlib.sha256(function.encode()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination',type=Path)
    args = parser.parse_args()
    try:
        prepare(args.destination)
    except (OSError,ValueError) as error:
        parser.exit(1,str(error)+'\n')
