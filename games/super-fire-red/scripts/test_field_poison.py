"""Walk poisoned steps through the real DoPoisonFieldEffect in a disposable ROM: poison stops at 1 HP and is cured."""
from pathlib import Path
import hashlib,json,re,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/field-poison';out.mkdir(parents=True,exist_ok=True)
for old in out.glob('*.png'):old.unlink()
source=Path((game/'output/hack-build-path.txt').read_text().strip())
reference_sha=hashlib.sha256((game/'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
test=Path(tempfile.mkdtemp(prefix='field-poison-',dir=source.parent));shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
hook='''
#include "new_game.h"
#include "pokemon.h"
#include "nuzlocke.h"
#include "field_poison.h"
#include "string_util.h"
EWRAM_DATA volatile u32 gFieldPoisonCase = 0, gFieldPoisonStage = 0;
EWRAM_DATA u8 gFieldPoisonResult[64] = {0};
static void FieldPoisonIdle(void) {}
static void FieldPoisonMon(u8 slot, u16 hp, u32 status)
{
    CreateMon(&gPlayerParty[slot],SPECIES_BULBASAUR,5,31,FALSE,0,OT_ID_PLAYER_ID,0);
    if (hp != 0xFFFF) SetMonData(&gPlayerParty[slot],MON_DATA_HP,&hp);
    SetMonData(&gPlayerParty[slot],MON_DATA_STATUS,&status);
}
// Case 1 Normal: HP 3, HP 1, healthy and HP 2 poisoned. Case 2 Hardcore: lone HP 2. Case 3: HP 0 from an old save.
static void FieldPoisonBoot(void)
{
    static const u8 name[] = _("TEST");
    u8 i;
    if (!gFieldPoisonCase) return;
    SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();
    Nuzlocke_ChooseMode(gFieldPoisonCase == 2 ? GAME_MODE_HARDCORE : GAME_MODE_NORMAL);
    ZeroPlayerPartyMons();
    if (gFieldPoisonCase == 1)
    {
        FieldPoisonMon(0,3,STATUS1_POISON);FieldPoisonMon(1,1,STATUS1_POISON);
        FieldPoisonMon(2,0xFFFF,STATUS1_NONE);FieldPoisonMon(3,2,STATUS1_POISON);
    }
    else
        FieldPoisonMon(0,gFieldPoisonCase == 2 ? 2 : 0,STATUS1_POISON);
    CalculatePlayerPartyCount();
    for (i = 0; i < 4; i++) gFieldPoisonResult[i] = DoPoisonFieldEffect();
    for (i = 0; i < 4; i++)
    {
        u16 hp = GetMonData(&gPlayerParty[i],MON_DATA_HP);
        gFieldPoisonResult[4+i*4] = hp; gFieldPoisonResult[5+i*4] = hp >> 8;
        gFieldPoisonResult[6+i*4] = GetMonData(&gPlayerParty[i],MON_DATA_STATUS);
        gFieldPoisonResult[7+i*4] = GetMonData(&gPlayerParty[i],MON_DATA_MAX_HP);
    }
    gFieldPoisonStage = 3;SetMainCallback2(FieldPoisonIdle);
}
'''
main=(game/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(FieldPoisonBoot);')
assert 'FieldPoisonBoot);' in main
(test/'src/main.c').write_text(main)
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
# The capture-item runner already plays cases 1-3, waits for stage 3 and dumps 64 bytes.
runner=test/'field-poison-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/capture_item_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'poison'),*['0x'+symbols[s] for s in ('gFieldPoisonCase','gFieldPoisonStage','gFieldPoisonResult')]],capture_output=True,text=True,timeout=180)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
rows={}
for m in re.finditer(r'case=(\d+) stage=(\d+) text=([0-9a-f]+)',result.stdout):
 data=bytes.fromhex(m[3]);assert int(m[2])==3,(m[1],'boot hook did not finish')
 rows[int(m[1])]={'returns':list(data[:4]),'party':[(data[4+i*4]|data[5+i*4]<<8,data[6+i*4]) for i in range(4)],'max_hp':[data[7+i*4] for i in range(4)]}
NONE,PSN,FNT,POISON=0,1,2,0x08  # FLDPSN_* and STATUS1_POISON
one=rows[1]
# FNT is the message path: survivors are announced there, but nobody faints.
assert one['returns']==[FNT,FNT,NONE,NONE],one
assert one['party'][0]==(1,0) and one['party'][1]==(1,0) and one['party'][3]==(1,0),one
assert one['party'][2]==(one['max_hp'][2],0),one
two=rows[2]
assert two['returns']==[FNT,NONE,NONE,NONE] and two['party'][0]==(1,0),two
three=rows[3]
assert three['returns']==[FNT]*4 and three['party'][0]==(0,POISON),three
overrides=json.loads((game/'hack-data/localization/ptbr_overrides.json').read_text(encoding='utf-8'))
message=re.search(r'sText_PkmnSurvivedPoison\[\]\s*=\s*_\("((?:[^"\\]|\\.)*)"\)',(game/'src/field_poison.c').read_text())[1]
assert message.replace('\\n','\n') in overrides,('missing PT-BR text',message)
print('PASS: field poison leaves 1 HP and cures (Normal and Hardcore); a healthy Pokemon is untouched; message translated to PT-BR.')
assert hashlib.sha256((game/'output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha
(out/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'status':'passed','cases':{str(k):v for k,v in rows.items()}},ensure_ascii=False,indent=2)+'\n')
