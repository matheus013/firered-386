"""Load the Hoenn pilot exteriors through the real New Game map loader, in an isolated build from source.

Never touches output/firered-386-dev.gba: the region branch policy forbids replacing distributed builds.
Case 1 Littleroot, case 2 Route101, case 3 Pallet Town (control: Kanto keeps its own tilesets).
"""
from pathlib import Path
import json,os,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/hoenn-pilot-maps';out.mkdir(parents=True,exist_ok=True)
for old in out.glob('*.png'):old.unlink()
cache=Path(os.environ.get('XDG_CACHE_HOME',Path.home()/'.cache'))/'firered-386'
test=Path(tempfile.mkdtemp(prefix='hoenn-pilot-',dir=cache));(out/'build-path.txt').write_text(str(test)+'\n')
files=subprocess.check_output(['python3',str(game/'scripts/source_base.py'),'--files'])
subprocess.run(['tar','-C',str(game),'-cf','-','-T','-'],input=files,stdout=open(test/'src.tar','wb'),check=True)
subprocess.run(['tar','-xf','src.tar'],cwd=test,check=True);(test/'src.tar').unlink()
subprocess.run(['bash','install.sh',str(test)],cwd=cache/'agbcc',check=True,stdout=subprocess.DEVNULL)
hook='''
#include "overworld.h"
#include "fieldmap.h"
#include "constants/maps.h"
#include "constants/layouts.h"
#include "constants/metatile_behaviors.h"
#include "string_util.h"
extern const struct Tileset gTileset_General, gTileset_HoennPilotGeneral, gTileset_HoennPilotPetalburg;
void CB2_NewGame(void);
void CB1_Overworld(void);
EWRAM_DATA volatile u32 gPilotCase = 0, gPilotStage = 0;
EWRAM_DATA u8 gPilotResult[64] = {0};
EWRAM_DATA static u32 sPilotFrames = 0, sPilotFlower = 0;
static u32 PilotFlowerSum(void)
{
    const u32 *tile = (const u32 *)(BG_VRAM + 508 * 32);
    u32 i, sum = 0;
    for (i = 0; i < 32; i++) sum = sum * 31 + tile[i];
    return sum;
}
void PilotCheck(void)
{
    u8 *r = gPilotResult;
    s16 dx = 5 + MAP_OFFSET, dy = 8 + MAP_OFFSET;
    if (gPilotStage == 0 || gPilotStage == 3 || gMain.callback1 != CB1_Overworld) return;
    sPilotFrames++;
    if (sPilotFrames == 60) { sPilotFlower = PilotFlowerSum(); gPilotStage = 2; }
    if (sPilotFrames == 76)
    {
        r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
        r[2] = gMapHeader.mapLayoutId; r[3] = gMapHeader.mapLayoutId >> 8;
        r[4] = gMapHeader.mapLayout->primaryTileset == &gTileset_HoennPilotGeneral;
        r[5] = gMapHeader.mapLayout->secondaryTileset == &gTileset_HoennPilotPetalburg;
        r[6] = gMapHeader.mapLayout->primaryTileset == &gTileset_General;
        r[7] = gMapHeader.connections ? gMapHeader.connections->count : 0;
        if (r[7]) { r[8] = gMapHeader.connections->connections[0].direction; r[9] = gMapHeader.connections->connections[0].mapGroup; r[10] = gMapHeader.connections->connections[0].mapNum; }
        r[11] = MapGridGetMetatileIdAt(dx, dy); r[12] = MapGridGetMetatileIdAt(dx, dy) >> 8;
        r[13] = MapGridGetMetatileBehaviorAt(dx, dy) == MB_WARP_DOOR;
        r[14] = MapGridGetCollisionAt(dx, dy);
        r[15] = gSaveBlock1Ptr->pos.x; r[16] = gSaveBlock1Ptr->pos.y;
        r[17] = PilotFlowerSum() != sPilotFlower;
        r[18] = MAP_GROUP(MAP_HOENN_LITTLEROOT_TOWN); r[19] = MAP_NUM(MAP_HOENN_LITTLEROOT_TOWN);
        r[20] = MAP_GROUP(MAP_HOENN_ROUTE101); r[21] = MAP_NUM(MAP_HOENN_ROUTE101);
        r[22] = LAYOUT_HOENN_LITTLEROOT_TOWN & 0xFF; r[23] = LAYOUT_HOENN_LITTLEROOT_TOWN >> 8;
        r[24] = MapGridGetCollisionAt(10 + MAP_OFFSET, 8 + MAP_OFFSET);
        gPilotStage = 3;
    }
}
static void PilotBoot(void)
{
    static const u8 name[] = _("TEST");
    if (!gPilotCase) return;
    gPilotStage = 1; sPilotFrames = 0;
    SetSaveBlocksPointers();
    // NewGameInitData copies the rival name: a blank save has no EOS and the copy overruns.
    StringCopy(gSaveBlock2Ptr->playerName, name); StringCopy(gSaveBlock1Ptr->rivalName, name);
    SetMainCallback2(CB2_NewGame);
}
'''
main=(game/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(PilotBoot);')
main=main.replace('#include "global.h"','#include "global.h"\nvoid PilotCheck(void);',1).replace('    ReadKeys();','    PilotCheck();\n    ReadKeys();')
assert 'PilotCheck();' in main and 'PilotBoot);' in main
(test/'src/main.c').write_text(main)
# Same entry as test_shops.py: right after NewGameInitData, warp straight onto the map under test.
setup='''
extern volatile u32 gPilotCase;
void PilotSetup(void)
{
    if (gPilotCase == 1) SetWarpDestination(MAP_GROUP(MAP_HOENN_LITTLEROOT_TOWN), MAP_NUM(MAP_HOENN_LITTLEROOT_TOWN), -1, 10, 8);
    else if (gPilotCase == 2) SetWarpDestination(MAP_GROUP(MAP_HOENN_ROUTE101), MAP_NUM(MAP_HOENN_ROUTE101), -1, 10, 8);
    else SetWarpDestination(MAP_GROUP(MAP_PALLET_TOWN), MAP_NUM(MAP_PALLET_TOWN), -1, 10, 8);
    WarpIntoMap();
}
'''
world=(game/'src/overworld.c').read_text()
anchor='    NewGameInitData();\n    ResetInitialPlayerAvatarState();'
assert anchor in world
world=world.replace(anchor,'    NewGameInitData();\n    PilotSetup();\n    ResetInitialPlayerAvatarState();').replace('#include "global.h"','#include "global.h"\nvoid PilotSetup(void);',1)
(test/'src/overworld.c').write_text(world+setup)
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
# The capture-item runner plays cases 1-3, screenshots at stage 2 and dumps 64 bytes at stage 3.
runner=test/'hoenn-pilot-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/capture_item_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'map'),*['0x'+symbols[s] for s in ('gPilotCase','gPilotStage','gPilotResult')]],capture_output=True,text=True,timeout=300)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
rows={}
import re
for m in re.finditer(r'case=(\d+) stage=(\d+) text=([0-9a-f]+)',result.stdout):
 assert int(m[2])==3,(m[1],'map did not reach the overworld',m[2])
 rows[int(m[1])]=bytes.fromhex(m[3])
# Each built asset must equal the audited staging output, so the installed files keep their provenance.
stage=root/'.build/region-pilots'
for built,staged in [('graphics/door_anims/hoenn/littleroot.4bpp','hoenn-doors/littleroot.4bpp'),('graphics/door_anims/hoenn/birchs_lab.4bpp','hoenn-doors/birchs_lab.4bpp'),
                     *[(f'data/tilesets/primary/hoenn_pilot/anim/Flower_{i}.4bpp',f'hoenn-animations/Flower_{i}.4bpp') for i in range(3)]]:
 if (stage/staged).exists():assert (test/built).read_bytes()==(stage/staged).read_bytes(),('built asset differs from audited staging',built)
for name in ('Hoenn_LittlerootTown','Hoenn_Route101'):
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn'/name.removeprefix('Hoenn_')/f).exists():assert (game/'data/layouts'/name/f).read_bytes()==(stage/'hoenn'/name.removeprefix('Hoenn_')/f).read_bytes(),(name,f)
little,route,pallet=rows[1],rows[2],rows[3]
UP,DOWN=2,1  # CONNECTION_NORTH / CONNECTION_SOUTH
assert (little[0],little[1])==(little[18],little[19]) and (route[0],route[1])==(little[20],little[21]),'loaded the wrong map'
assert little[2]|little[3]<<8==little[22]|little[23]<<8,'wrong layout'
for name,row in (('Littleroot',little),('Route101',route)):
 assert row[4] and row[5] and not row[6],(name,'not using the Hoenn pilot tilesets')
 assert (row[15],row[16])==(10,8),(name,'player not at the spawn cell',row[15],row[16])  # map coords, no MAP_OFFSET
 assert row[24]==0,(name,'spawn cell is blocked')
 assert row[17],(name,'flower animation did not update VRAM tile 508')
assert little[7]==1 and (little[8],little[9],little[10])==(UP,little[20],little[21]),('Littleroot must connect north to Route101',list(little[7:11]))
assert route[7]==1 and (route[8],route[9],route[10])==(DOWN,little[18],little[19]),('Route101 must connect south to Littleroot',list(route[7:11]))
assert little[11]|little[12]<<8==0x2C8 and little[13] and little[14],('Littleroot door cell',little[11:15].hex())
assert pallet[6] and not pallet[4] and not pallet[5],'Pallet Town no longer uses the FireRed tilesets'
print('PASS: Littleroot and Route101 load from New Game on the Hoenn pilot tilesets, connect to each other, animate flowers and keep audited assets; Pallet Town unchanged.')
(out/'results.json').write_text(json.dumps({'status':'passed','build':str(test),'cases':{str(k):v.hex() for k,v in rows.items()}},indent=2)+'\n')
