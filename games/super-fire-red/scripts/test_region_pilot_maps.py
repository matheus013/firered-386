"""Load and walk the Hoenn and Johto pilot maps through the real New Game loader, in an isolated build.

Never touches output/firered-386-dev.gba: the region branch policy forbids replacing distributed builds.
Data cases check what loads (layout, tilesets, connection, door cell, tile animation; Pallet Town is the
Kanto control). Walk cases hold the D-pad like a player: through every door and back out, up and down the
stairs, into the Route 29 gate, across the Littleroot/Route 101 and New Bark/Route 29 connections, and
around the Hoenn bedroom cell that blocks north/south only.
"""
from pathlib import Path
import json,os,re,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/region-pilot-maps';out.mkdir(parents=True,exist_ok=True)
for old in out.glob('*.png'):old.unlink()
cache=Path(os.environ.get('XDG_CACHE_HOME',Path.home()/'.cache'))/'firered-386'
test=Path(tempfile.mkdtemp(prefix='region-pilot-',dir=cache));(out/'build-path.txt').write_text(str(test)+'\n')
files=subprocess.check_output(['python3',str(game/'scripts/source_base.py'),'--files'])
subprocess.run(['tar','-C',str(game),'-cf','-','-T','-'],input=files,stdout=open(test/'src.tar','wb'),check=True)
subprocess.run(['tar','-xf','src.tar'],cwd=test,check=True);(test/'src.tar').unlink()
subprocess.run(['bash','install.sh',str(test)],cwd=cache/'agbcc',check=True,stdout=subprocess.DEVNULL)

LITTLEROOT,ROUTE101,B1F,B2F,M1F,M2F,BIRCH=range(7)           # gMapGroup_Hoenn
NEWBARK,ROUTE29,P1F,P2F,HOUSE1,HOUSE2,ELM,GATE=range(8)       # gMapGroup_Johto
HOENN,JOHTO='MAP_HOENN_','MAP_JOHTO_'
# (map, spawn x, spawn y, probe x, probe y, steps). No steps = data case.
# Step modes: MAP = hold until the map changes, POS = hold until the player steps, HOLD = hold 48 frames.
CASES=[
 (HOENN+'LITTLEROOT_TOWN',10,8,5,8,[]),
 (HOENN+'ROUTE101',10,8,5,8,[]),
 ('MAP_PALLET_TOWN',10,8,5,8,[]),
 (HOENN+'LITTLEROOT_TOWN',5,9,0,0,[('UP','MAP'),('DOWN','MAP')]),      # Brendan's door, doormat out
 (HOENN+'LITTLEROOT_TOWN',14,9,0,0,[('UP','MAP'),('DOWN','MAP')]),     # May's door
 (HOENN+'LITTLEROOT_TOWN',7,17,0,0,[('UP','MAP'),('DOWN','MAP')]),     # Birch's lab
 (HOENN+'LITTLEROOT_TOWN_BRENDANS_HOUSE_1F',8,3,0,0,[('UP','MAP'),('DOWN','POS'),('UP','MAP')]),
 (HOENN+'LITTLEROOT_TOWN_MAYS_HOUSE_1F',2,3,0,0,[('UP','MAP'),('DOWN','POS'),('UP','MAP')]),
 (HOENN+'LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',1,3,0,0,[('DOWN','HOLD')]),  # (1,4) blocks entry from the north
 (HOENN+'LITTLEROOT_TOWN_BRENDANS_HOUSE_2F',2,4,0,0,[('LEFT','POS'),('UP','HOLD'),('DOWN','HOLD'),('RIGHT','POS')]),
 (HOENN+'LITTLEROOT_TOWN',10,0,0,0,[('UP','MAP'),('DOWN','MAP')]),     # walk north into Route 101 and back
 (JOHTO+'NEW_BARK_TOWN',10,10,10,9,[]),
 (JOHTO+'ROUTE29',34,11,34,10,[]),
 (JOHTO+'NEW_BARK_TOWN',10,10,0,0,[('UP','MAP'),('DOWN','MAP')]),      # Elm's lab
 (JOHTO+'NEW_BARK_TOWN',20,12,0,0,[('UP','MAP'),('DOWN','MAP')]),      # player's house
 (JOHTO+'NEW_BARK_TOWN',4,22,0,0,[('UP','MAP'),('DOWN','MAP')]),       # house 1
 (JOHTO+'NEW_BARK_TOWN',15,22,0,0,[('UP','MAP'),('DOWN','MAP')]),      # house 2, yellow door
 (JOHTO+'NEW_BARK_TOWN_PLAYERS_HOUSE_1F',9,2,0,0,[('RIGHT','MAP')]),   # up-right stairs fire moving east
 (JOHTO+'NEW_BARK_TOWN_PLAYERS_HOUSE_2F',10,2,0,0,[('LEFT','MAP')]),   # down-left stairs fire moving west
 (JOHTO+'ROUTE29',34,11,0,0,[('UP','MAP'),('DOWN','MAP')]),            # Route 46 gate and back
 (JOHTO+'NEW_BARK_TOWN',0,12,0,0,[('LEFT','MAP'),('RIGHT','MAP')]),    # walk west into Route 29 and back
]
KEYS={'UP':'DPAD_UP','DOWN':'DPAD_DOWN','LEFT':'DPAD_LEFT','RIGHT':'DPAD_RIGHT'}
MODES={'MAP':0,'POS':1,'HOLD':2}
spawns=',\n    '.join(f'{{MAP_GROUP({m}), MAP_NUM({m}), {x}, {y}, {px}, {py}}}' for m,x,y,px,py,_ in CASES)
steps=',\n    '.join('{'+', '.join(f'{{{KEYS[k]}, {MODES[mode]}}}' for k,mode in s)+'}' if s else '{{0, 0}}' for *_,s in CASES)
counts=', '.join(str(len(s)) for *_,s in CASES)
hook='''
#include "overworld.h"
#include "fieldmap.h"
#include "palette.h"
#include "script.h"
#include "field_player_avatar.h"
#include "constants/maps.h"
#include "string_util.h"
extern const struct Tileset gTileset_General, gTileset_HoennPilotGeneral, gTileset_HoennPilotPetalburg;
extern const struct Tileset gTileset_JohtoPilotGeneral, gTileset_JohtoPilotNewBarkTown;
void CB2_NewGame(void);
void CB1_Overworld(void);
struct PilotStep { u16 keys; u8 mode; };
const s8 gPilotSpawn[][6] = {
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
EWRAM_DATA static u8 sPilotStep = 0, sPilotPhase = 0, sPilotStartGroup = 0, sPilotStartMap = 0;
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
    const s8 *spawn = gPilotSpawn[gPilotCase - 1];
    s16 px = spawn[4] + MAP_OFFSET, py = spawn[5] + MAP_OFFSET;
    const struct MapLayout *layout = gMapHeader.mapLayout;
    sPilotFrames++;
    if (sPilotFrames == 60) { sPilotFlower = PilotFlowerSum(); gPilotShot = 1; }
    if (sPilotFrames == 76)
    {
        r[0] = gSaveBlock1Ptr->location.mapGroup; r[1] = gSaveBlock1Ptr->location.mapNum;
        r[2] = gMapHeader.mapLayoutId; r[3] = gMapHeader.mapLayoutId >> 8;
        r[4] = (layout->primaryTileset == &gTileset_HoennPilotGeneral) | (layout->secondaryTileset == &gTileset_HoennPilotPetalburg) << 1
             | (layout->primaryTileset == &gTileset_General) << 2
             | (layout->primaryTileset == &gTileset_JohtoPilotGeneral) << 3 | (layout->secondaryTileset == &gTileset_JohtoPilotNewBarkTown) << 4;
        r[7] = gMapHeader.connections ? gMapHeader.connections->count : 0;
        if (r[7]) { r[8] = gMapHeader.connections->connections[0].direction; r[9] = gMapHeader.connections->connections[0].mapGroup; r[10] = gMapHeader.connections->connections[0].mapNum; }
        r[11] = MapGridGetMetatileIdAt(px, py); r[12] = MapGridGetMetatileIdAt(px, py) >> 8;
        r[13] = MapGridGetMetatileBehaviorAt(px, py);
        r[14] = MapGridGetCollisionAt(px, py);
        r[15] = gSaveBlock1Ptr->pos.x; r[16] = gSaveBlock1Ptr->pos.y;
        r[17] = PilotFlowerSum() != sPilotFlower;
        r[24] = MapGridGetCollisionAt(spawn[2] + MAP_OFFSET, spawn[3] + MAP_OFFSET);
        r[25] = gMapHeader.regionMapSectionId;
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
    bool8 moved;
    PlayerGetDestCoords(&x, &y);
    sPilotIdle = field && x == sPilotX && y == sPilotY ? sPilotIdle + 1 : 0;
    sPilotX = x; sPilotY = y;
    sPilotStepFrames++;
    moved = gSaveBlock1Ptr->location.mapGroup != sPilotStartGroup || gSaveBlock1Ptr->location.mapNum != sPilotStartMap;
    if (sPilotPhase == 0)
    {
        if (sPilotIdle < 30) return;
        sPilotStartGroup = gSaveBlock1Ptr->location.mapGroup; sPilotStartMap = gSaveBlock1Ptr->location.mapNum;
        sPilotStartX = x; sPilotStartY = y;
        gPilotKeys = step->keys; sPilotPhase = 1; sPilotStepFrames = 0;
    }
    else if (sPilotPhase == 1)
    {
        if ((step->mode == 0 && (!field || moved))
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
extern const s8 gPilotSpawn[][6];
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
constants=dict(re.findall(r'#define (MAP_\w+|LAYOUT_\w+) +\(?(\d+[^)\n]*)',(test/'include/constants/maps.h').read_text()+(test/'include/constants/layouts.h').read_text()))
runner=test/'region-pilot-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/region_pilot_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'map'),str(len(CASES)),
                       *['0x'+symbols[s] for s in ('gPilotCase','gPilotStage','gPilotResult','gPilotKeys','gPilotShot')]],
                      capture_output=True,text=True,timeout=1800)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
rows={}
for m in re.finditer(r'case=(\d+) stage=(\d+) text=([0-9a-f]+)',result.stdout):
 assert int(m[2])==3,(m[1],'case did not finish',m[2])
 rows[int(m[1])]=bytes.fromhex(m[3])
assert len(rows)==len(CASES),rows.keys()

def same(installed,source,what):
 assert installed.read_bytes()==source.read_bytes(),('installed file differs from its source',what)
# Hoenn: built and installed assets must equal the audited staging output.
stage=root/'.build/region-pilots'
for built,staged in [('graphics/door_anims/hoenn/littleroot.4bpp','hoenn-doors/littleroot.4bpp'),('graphics/door_anims/hoenn/birchs_lab.4bpp','hoenn-doors/birchs_lab.4bpp'),
                     *[(f'data/tilesets/primary/hoenn_pilot/anim/Flower_{i}.4bpp',f'hoenn-animations/Flower_{i}.4bpp') for i in range(3)]]:
 if (stage/staged).exists():same(test/built,stage/staged,built)
for name in ['LittlerootTown','Route101']:
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn'/name/f).exists():same(game/'data/layouts'/('Hoenn_'+name)/f,stage/'hoenn'/name/f,name)
for name in ['LittlerootTown_BrendansHouse_1F','LittlerootTown_BrendansHouse_2F','LittlerootTown_MaysHouse_1F','LittlerootTown_MaysHouse_2F','LittlerootTown_ProfessorBirchsLab']:
 for f in ('map.bin','border.bin'):
  if (stage/'hoenn-interiors'/name/f).exists():same(game/'data/layouts'/('Hoenn_'+name)/f,stage/'hoenn-interiors'/name/f,name)
for folder in ['primary/hoenn_pilot_building','secondary/hoenn_pilot_brendans_mays_house','secondary/hoenn_pilot_lab']:
 for f in ('tiles.png','metatiles.bin','metatile_attributes.bin'):
  if (stage/'hoenn-interior-graphics'/folder/f).exists():same(game/'data/tilesets'/folder/f,stage/'hoenn-interior-graphics'/folder/f,folder)
# Johto: tiles, metatiles, palettes, blocks, animation frames and doors are unchanged HNS_modern files.
hns=root.parent/'map-references/HNS_modern'
if hns.exists():
 for src,dst in [('primary/johto_general','primary/johto_pilot_general'),('secondary/new_bark_town','secondary/johto_pilot_new_bark_town'),
                 ('primary/johto_building','primary/johto_pilot_building'),('secondary/players_house','secondary/johto_pilot_players_house'),
                 ('secondary/house_lab','secondary/johto_pilot_house_lab'),('secondary/gate_standard','secondary/johto_pilot_gate_standard')]:
  for f in ['tiles.png','metatiles.bin',*[f'palettes/{p.name}' for p in (hns/'data/tilesets'/src/'palettes').glob('*.pal')]]:
   same(game/'data/tilesets'/dst/f,hns/'data/tilesets'/src/f,dst+'/'+f)
 for name in ['NewBarkTown','Route29','NewBarkTown_PlayersHouse_1F','NewBarkTown_PlayersHouse_2F','NewBarkTown_House1','NewBarkTown_House2','NewBarkTown_Lab','Gate_Route29_Route46']:
  for f in ('map.bin','border.bin'):same(game/'data/layouts'/('Johto_'+name)/f,hns/'data/layouts'/name/f,name)
 for f in [*[f'flower/{i}.png' for i in range(5)],*[f'water_current_landwatersedge/{i}.png' for i in range(8)]]:
  same(game/'data/tilesets/primary/johto_pilot_general/anim'/f,hns/'data/tilesets/primary/johto_general/anim'/f,f)
 same(game/'graphics/door_anims/johto/new_bark_blue.png',hns/'graphics/door_anims/cerulean.png','blue door')
 same(game/'graphics/door_anims/johto/new_bark_yellow.png',hns/'graphics/door_anims/NewBarkTown_Door_Yellow.png','yellow door')

HG,JG=int(constants['MAP_GROUP_HOENN_LITTLEROOT_TOWN'] if 'MAP_GROUP_HOENN_LITTLEROOT_TOWN' in constants else rows[1][0]),rows[12][0]
UP,DOWN,LEFT,RIGHT=2,1,3,4  # CONNECTION_*
HOENN_TILES,KANTO_TILES,JOHTO_TILES=0b00011,0b00100,0b11000
WARP_DOOR,CAVE_DOOR=0x69,0x60
little,route,pallet,newbark,route29=rows[1],rows[2],rows[3],rows[12],rows[13]
assert (little[0],little[1])==(HG,LITTLEROOT) and (route[0],route[1])==(HG,ROUTE101),'Hoenn maps loaded in the wrong slots'
assert (newbark[0],newbark[1])==(JG,NEWBARK) and (route29[0],route29[1])==(JG,ROUTE29) and JG==HG+1,'Johto maps loaded in the wrong slots'
for name,row,tiles in (('Littleroot',little,HOENN_TILES),('Route101',route,HOENN_TILES),('New Bark',newbark,JOHTO_TILES),('Route29',route29,JOHTO_TILES)):
 assert row[4]==tiles,(name,'wrong tilesets',bin(row[4]))
 assert row[24]==0,(name,'spawn cell is blocked')
 assert row[17],(name,'tile animation did not update VRAM tile 508')
assert pallet[4]==KANTO_TILES,'Pallet Town no longer uses the FireRed tilesets'
assert little[7]==1 and tuple(little[8:11])==(UP,HG,ROUTE101),('Littleroot must connect north to Route101',list(little[7:11]))
assert route[7]==1 and tuple(route[8:11])==(DOWN,HG,LITTLEROOT),('Route101 must connect south to Littleroot',list(route[7:11]))
assert newbark[7]==1 and tuple(newbark[8:11])==(LEFT,JG,ROUTE29),('New Bark must connect west to Route29',list(newbark[7:11]))
assert route29[7]==1 and tuple(route29[8:11])==(RIGHT,JG,NEWBARK),('Route29 must connect east to New Bark',list(route29[7:11]))
assert little[11]|little[12]<<8==0x2C8 and little[13]==WARP_DOOR and little[14],('Littleroot door cell',little[11:15].hex())
assert newbark[11]|newbark[12]<<8==0x2CB and newbark[13]==WARP_DOOR and newbark[14],('New Bark blue door cell',newbark[11:15].hex())
assert route29[13]==CAVE_DOOR,('Route 46 gate entrance should be a step-on warp',route29[11:15].hex())

def walked(case):
 row=rows[case];return [tuple(row[i*4:i*4+4]) for i in range(len(CASES[case-1][5]))]
def at(case,expected):
 got=walked(case)
 for i,(g,want) in enumerate(zip(got,expected)):
  group,num,cells=want
  assert g[0]==group and g[1]==num and (g[2],g[3]) in cells,(CASES[case-1][0],'step',i,'got',g,'expected group',group,'map',num,'in',cells)
# Doors and doormats land on the arrival cell; leaving through a door or step-on stairs steps one cell off it.
at(4,[(HG,B1F,{(8,8)}),(HG,LITTLEROOT,{(5,9)})])
at(5,[(HG,M1F,{(2,8)}),(HG,LITTLEROOT,{(14,9)})])
at(6,[(HG,BIRCH,{(6,12)}),(HG,LITTLEROOT,{(7,17)})])
at(7,[(HG,B2F,{(7,2)}),(HG,B2F,{(7,3)}),(HG,B1F,{(8,3)})])
at(8,[(HG,M2F,{(1,2)}),(HG,M2F,{(1,3)}),(HG,M1F,{(2,3)})])
at(9,[(HG,B2F,{(1,3)})])
at(10,[(HG,B2F,{(1,4)}),(HG,B2F,{(1,4)}),(HG,B2F,{(1,4)}),(HG,B2F,{(2,4)})])
at(11,[(HG,ROUTE101,{(10,19)}),(HG,LITTLEROOT,{(10,0)})])
for case,(num,arrive,back) in {14:(ELM,(6,12),(10,10)),15:(P1F,(9,8),(20,12)),16:(HOUSE1,(4,8),(4,22)),17:(HOUSE2,(4,8),(15,22))}.items():
 at(case,[(JG,num,{arrive}),(JG,NEWBARK,{back})])
at(18,[(JG,P2F,{(9,2)})])   # FireRed stairs: arrive on the stair cell
at(19,[(JG,P1F,{(10,2)})])
at(20,[(JG,GATE,{(7,9)}),(JG,ROUTE29,{(34,11)})])
at(21,[(JG,ROUTE29,{(69,17)}),(JG,NEWBARK,{(0,12)})])
print('PASS: Hoenn and Johto pilots load on their own tilesets with doors, animation and connections; every door, '
      'doormat, staircase and the Route 29 gate warp both ways; both connections cross on foot; the Hoenn bedroom cell '
      'blocks north/south only; assets match their sources; Pallet unchanged.')
(out/'results.json').write_text(json.dumps({'status':'passed','build':str(test),'cases':{str(k):v.hex() for k,v in rows.items()},
                                            'walks':{str(k):walked(k) for k in rows if CASES[k-1][5]}},indent=2)+'\n')
