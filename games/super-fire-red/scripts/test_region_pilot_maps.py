"""Load and walk the Hoenn pilot maps through the real New Game loader, in an isolated build from source.

Never touches output/firered-386-dev.gba: the region branch policy forbids replacing distributed builds.
Cases 1-3 check loaded data (Littleroot, Route101, Pallet Town as the Kanto control). The other cases
hold the D-pad like a player: through the Littleroot doors, back out by the doormats, up and down the
house stairs, and around the bedroom cell that blocks north/south only.
"""
from pathlib import Path
import json,os,re,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/hoenn-pilot-maps';out.mkdir(parents=True,exist_ok=True)
for old in out.glob('*.png'):old.unlink()
cache=Path(os.environ.get('XDG_CACHE_HOME',Path.home()/'.cache'))/'firered-386'
test=Path(tempfile.mkdtemp(prefix='hoenn-pilot-',dir=cache));(out/'build-path.txt').write_text(str(test)+'\n')
files=subprocess.check_output(['python3',str(game/'scripts/source_base.py'),'--files'])
subprocess.run(['tar','-C',str(game),'-cf','-','-T','-'],input=files,stdout=open(test/'src.tar','wb'),check=True)
subprocess.run(['tar','-xf','src.tar'],cwd=test,check=True);(test/'src.tar').unlink()
subprocess.run(['bash','install.sh',str(test)],cwd=cache/'agbcc',check=True,stdout=subprocess.DEVNULL)

HOENN=['LITTLEROOT_TOWN','ROUTE101','LITTLEROOT_TOWN_BRENDANS_HOUSE_1F','LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',
       'LITTLEROOT_TOWN_MAYS_HOUSE_1F','LITTLEROOT_TOWN_MAYS_HOUSE_2F','LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB']
LITTLEROOT,ROUTE,B1F,B2F,M1F,M2F,LAB=range(7)  # map numbers inside gMapGroup_Hoenn
# Step modes: MAP = hold until a warp starts, POS = hold until the player steps, HOLD = hold 48 frames.
CASES=[
 ('MAP_HOENN_LITTLEROOT_TOWN',10,8,[]),
 ('MAP_HOENN_ROUTE101',10,8,[]),
 ('MAP_PALLET_TOWN',10,8,[]),
 ('MAP_HOENN_LITTLEROOT_TOWN',5,9,[('UP','MAP'),('DOWN','MAP')]),      # Brendan's door, doormat out
 ('MAP_HOENN_LITTLEROOT_TOWN',14,9,[('UP','MAP'),('DOWN','MAP')]),     # May's door
 ('MAP_HOENN_LITTLEROOT_TOWN',7,17,[('UP','MAP'),('DOWN','MAP')]),     # Birch's lab
 ('MAP_HOENN_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F',8,3,[('UP','MAP'),('DOWN','POS'),('UP','MAP')]),
 ('MAP_HOENN_LITTLEROOT_TOWN_MAYS_HOUSE_1F',2,3,[('UP','MAP'),('DOWN','POS'),('UP','MAP')]),
 ('MAP_HOENN_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',1,3,[('DOWN','HOLD')]),  # (1,4) blocks entry from the north
 ('MAP_HOENN_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',2,4,[('LEFT','POS'),('UP','HOLD'),('DOWN','HOLD'),('RIGHT','POS')]),
]
KEYS={'UP':'DPAD_UP','DOWN':'DPAD_DOWN','LEFT':'DPAD_LEFT','RIGHT':'DPAD_RIGHT'}
MODES={'MAP':0,'POS':1,'HOLD':2}
spawns=',\n    '.join(f'{{MAP_GROUP({m}), MAP_NUM({m}), {x}, {y}}}' for m,x,y,_ in CASES)
steps=',\n    '.join('{'+', '.join(f'{{{KEYS[k]}, {MODES[mode]}}}' for k,mode in s)+'}' if s else '{{0, 0}}' for *_,s in CASES)
counts=', '.join(str(len(s)) for *_,s in CASES)
hook='''
#include "overworld.h"
#include "fieldmap.h"
#include "palette.h"
#include "script.h"
#include "field_player_avatar.h"
#include "constants/maps.h"
#include "constants/layouts.h"
#include "constants/metatile_behaviors.h"
#include "string_util.h"
extern const struct Tileset gTileset_General, gTileset_HoennPilotGeneral, gTileset_HoennPilotPetalburg;
void CB2_NewGame(void);
void CB1_Overworld(void);
struct PilotStep { u16 keys; u8 mode; };
const s8 gPilotSpawn[][4] = {
    SPAWNS
};
static const struct PilotStep sPilotSteps[][4] = {
    STEPS
};
static const u8 sPilotStepCount[] = {COUNTS};
EWRAM_DATA volatile u32 gPilotCase = 0, gPilotStage = 0, gPilotShot = 0;
EWRAM_DATA volatile u16 gPilotKeys = 0;
EWRAM_DATA u8 gPilotResult[64] = {0};
EWRAM_DATA static u32 sPilotFrames = 0, sPilotFlower = 0, sPilotIdle = 0, sPilotStepFrames = 0;
EWRAM_DATA static u8 sPilotStep = 0, sPilotPhase = 0, sPilotStartMap = 0;
EWRAM_DATA static s16 sPilotX = 0, sPilotY = 0, sPilotStartX = 0, sPilotStartY = 0;
static u32 PilotFlowerSum(void)
{
    const u32 *tile = (const u32 *)(BG_VRAM + 508 * 32);
    u32 i, sum = 0;
    for (i = 0; i < 32; i++) sum = sum * 31 + tile[i];
    return sum;
}
static void PilotDataCase(void)
{
    u8 *r = gPilotResult;
    s16 dx = 5 + MAP_OFFSET, dy = 8 + MAP_OFFSET;
    sPilotFrames++;
    if (sPilotFrames == 60) { sPilotFlower = PilotFlowerSum(); gPilotShot = 1; }
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
// Hold one D-pad key per step, then wait until the player is idle and record map and position.
static void PilotWalkCase(void)
{
    const struct PilotStep *step = &sPilotSteps[gPilotCase - 1][sPilotStep];
    u8 *r = gPilotResult + sPilotStep * 4;
    s16 x, y;
    bool8 field = gMain.callback1 == CB1_Overworld && !ArePlayerFieldControlsLocked() && !gPaletteFade.active;
    PlayerGetDestCoords(&x, &y);
    sPilotIdle = field && x == sPilotX && y == sPilotY ? sPilotIdle + 1 : 0;
    sPilotX = x; sPilotY = y;
    sPilotStepFrames++;
    if (sPilotPhase == 0)
    {
        if (sPilotIdle < 30) return;
        sPilotStartMap = gSaveBlock1Ptr->location.mapNum; sPilotStartX = x; sPilotStartY = y;
        gPilotKeys = step->keys; sPilotPhase = 1; sPilotStepFrames = 0;
    }
    else if (sPilotPhase == 1)
    {
        if ((step->mode == 0 && (!field || gSaveBlock1Ptr->location.mapNum != sPilotStartMap))
         || (step->mode == 1 && (x != sPilotStartX || y != sPilotStartY))
         || (step->mode == 2 && sPilotStepFrames >= 48)
         || sPilotStepFrames >= 600)
        {
            gPilotKeys = 0; sPilotPhase = 2; sPilotIdle = 0; sPilotStepFrames = 0;
        }
    }
    else if (sPilotIdle >= 30 || sPilotStepFrames >= 900)
    {
        r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
        r[2] = x - MAP_OFFSET; r[3] = y - MAP_OFFSET;
        gPilotShot = sPilotStep + 1;
        sPilotPhase = 0; sPilotIdle = 0;
        if (++sPilotStep >= sPilotStepCount[gPilotCase - 1])
            gPilotStage = 3;
    }
}
void PilotCheck(void)
{
    if (gPilotStage == 0 || gPilotStage == 3) return;
    if (gMain.callback1 != CB1_Overworld && sPilotFrames == 0 && sPilotPhase == 0 && sPilotStep == 0) return;
    if (sPilotStepCount[gPilotCase - 1] == 0) PilotDataCase();
    else PilotWalkCase();
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
'''.replace('SPAWNS',spawns).replace('STEPS',steps).replace('COUNTS',counts)
main=(game/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(PilotBoot);')
main=main.replace('#include "global.h"','#include "global.h"\nvoid PilotCheck(void);',1).replace('    ReadKeys();','    PilotCheck();\n    ReadKeys();')
assert 'PilotCheck();' in main and 'PilotBoot);' in main
(test/'src/main.c').write_text(main)
# Same entry as test_shops.py: right after NewGameInitData, warp straight onto the map under test.
setup='''
extern volatile u32 gPilotCase;
extern const s8 gPilotSpawn[][4];
void PilotSetup(void)
{
    const s8 *spawn = gPilotSpawn[gPilotCase - 1];
    SetWarpDestination(spawn[0], spawn[1], -1, spawn[2], spawn[3]);
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
runner=test/'hoenn-pilot-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/hoenn_pilot_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'map'),str(len(CASES)),
                       *['0x'+symbols[s] for s in ('gPilotCase','gPilotStage','gPilotResult','gPilotKeys','gPilotShot')]],
                      capture_output=True,text=True,timeout=900)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
rows={}
for m in re.finditer(r'case=(\d+) stage=(\d+) text=([0-9a-f]+)',result.stdout):
 assert int(m[2])==3,(m[1],'case did not finish',m[2])
 rows[int(m[1])]=bytes.fromhex(m[3])
assert len(rows)==len(CASES),rows.keys()

# Each built asset must equal the audited staging output, so the installed files keep their provenance.
stage=root/'.build/region-pilots'
for built,staged in [('graphics/door_anims/hoenn/littleroot.4bpp','hoenn-doors/littleroot.4bpp'),('graphics/door_anims/hoenn/birchs_lab.4bpp','hoenn-doors/birchs_lab.4bpp'),
                     *[(f'data/tilesets/primary/hoenn_pilot/anim/Flower_{i}.4bpp',f'hoenn-animations/Flower_{i}.4bpp') for i in range(3)]]:
 if (stage/staged).exists():assert (test/built).read_bytes()==(stage/staged).read_bytes(),('built asset differs from audited staging',built)
for name in ['LittlerootTown','Route101']:
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn'/name/f).exists():assert (game/'data/layouts'/('Hoenn_'+name)/f).read_bytes()==(stage/'hoenn'/name/f).read_bytes(),(name,f)
for name in ['LittlerootTown_BrendansHouse_1F','LittlerootTown_BrendansHouse_2F','LittlerootTown_MaysHouse_1F','LittlerootTown_MaysHouse_2F','LittlerootTown_ProfessorBirchsLab']:
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn-interiors'/name/f).exists():assert (game/'data/layouts'/('Hoenn_'+name)/f).read_bytes()==(stage/'hoenn-interiors'/name/f).read_bytes(),(name,f)
for folder in ['primary/hoenn_pilot_building','secondary/hoenn_pilot_brendans_mays_house','secondary/hoenn_pilot_lab']:
 for f in ('tiles.png','metatiles.bin','metatile_attributes.bin'):
  if (stage/'hoenn-interior-graphics'/folder/f).exists():assert (game/'data/tilesets'/folder/f).read_bytes()==(stage/'hoenn-interior-graphics'/folder/f).read_bytes(),(folder,f)

little,route,pallet=rows[1],rows[2],rows[3]
UP,DOWN=2,1  # CONNECTION_NORTH / CONNECTION_SOUTH
GROUP=little[18]
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

def walked(case):
 row=rows[case];return [tuple(row[i*4:i*4+4]) for i in range(len(CASES[case-1][3]))]
def at(case,expected):
 got=walked(case)
 for i,(g,want) in enumerate(zip(got,expected)):
  num,cells=want
  assert g[0]==GROUP and g[1]==num and (g[2],g[3]) in cells,(CASES[case-1][0],'step',i,'got map',g[1],'at',(g[2],g[3]),'expected map',num,'in',cells)
# Doors and doormats land on the arrival cell; leaving through a door or the stairs steps one cell off it.
at(4,[(B1F,{(8,8)}),(LITTLEROOT,{(5,9)})])
at(5,[(M1F,{(2,8)}),(LITTLEROOT,{(14,9)})])
at(6,[(LAB,{(6,12)}),(LITTLEROOT,{(7,17)})])
at(7,[(B2F,{(7,2)}),(B2F,{(7,3)}),(B1F,{(8,3)})])
at(8,[(M2F,{(1,2)}),(M2F,{(1,3)}),(M1F,{(2,3)})])
at(9,[(B2F,{(1,3)})])
at(10,[(B2F,{(1,4)}),(B2F,{(1,4)}),(B2F,{(1,4)}),(B2F,{(2,4)})])
print('PASS: Littleroot and Route101 load on the Hoenn pilot tilesets, connect and animate; all three doors, '
      'doormats and both staircases warp both ways; the bedroom cell blocks north/south only; assets match staging; Pallet unchanged.')
(out/'results.json').write_text(json.dumps({'status':'passed','build':str(test),'cases':{str(k):v.hex() for k,v in rows.items()},
                                            'walks':{str(k):walked(k) for k in range(4,len(CASES)+1)}},indent=2)+'\n')
