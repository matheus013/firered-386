"""Catch a wild Pokemon through the real battle script and check the held-item message, disposable ROM."""
from pathlib import Path
import hashlib,json,re,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/capture-item';out.mkdir(parents=True,exist_ok=True)
for old in out.glob('*.png'):old.unlink()
source=Path((game/'output/hack-build-path.txt').read_text().strip())
reference_sha=hashlib.sha256((game/'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
test=Path(tempfile.mkdtemp(prefix='capture-item-',dir=source.parent));shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
hook='''
#include "new_game.h"
#include "pokemon.h"
#include "battle.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "game_settings.h"
#include "overworld.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/maps.h"
extern void CB2_InitBattle(void);
extern void HandleTurnActionSelectionState(void);
extern void RunBattleScriptCommands(void);
EWRAM_DATA volatile u32 gCaptureItemCase = 0, gCaptureItemStage = 0;
EWRAM_DATA u8 gCaptureItemText[64] = {0};
void CaptureItemCheck(void)
{
    if (gCaptureItemStage == 0 && gMain.inBattle && gBattleMainFunc == HandleTurnActionSelectionState && gBattleControllerExecFlags == 0)
    {
        gBattlerAttacker = 0; gBattlerTarget = 1; gLastUsedItem = ITEM_POKE_BALL;
        gBattlescriptCurrInstr = BattleScript_SuccessBallThrow; gBattleMainFunc = RunBattleScriptCommands; gCaptureItemStage = 1;
    }
    if (gCaptureItemStage == 1 && gBattlescriptCurrInstr == BattleScript_CaughtMonHeldItem + 3)
    {
        memcpy(gCaptureItemText, gDisplayedStringBattle, sizeof(gCaptureItemText)); gCaptureItemStage = 2;
    }
    if (gCaptureItemStage >= 1 && gCaptureItemStage < 3 && gBattlescriptCurrInstr[0] == 0xF1) gCaptureItemStage = 3;
}
static void CaptureItemIdle(void) {}
static void CaptureItemBoot(void)
{
    static const u8 name[] = _("TEST");
    u16 item;
    if (!gCaptureItemCase) return;
    SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();
    gSaveBlock2Ptr->optionsTextSpeed=2;gSaveBlock2Ptr->optionsBattleSceneOff=TRUE;
    if (gCaptureItemCase == 3) GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);
    gMapHeader=*Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1));
    CreateMon(&gPlayerParty[0],SPECIES_BULBASAUR,18,31,FALSE,0,OT_ID_PLAYER_ID,0);CalculatePlayerPartyCount();
    ZeroEnemyPartyMons();CreateMon(&gEnemyParty[0],SPECIES_PIKACHU,5,0,FALSE,0,OT_ID_PLAYER_ID,0);
    item = gCaptureItemCase == 2 ? ITEM_NONE : ITEM_LIGHT_BALL;SetMonData(&gEnemyParty[0],MON_DATA_HELD_ITEM,&item);
    gBattleTypeFlags=0;gMain.savedCallback=CaptureItemIdle;SetMainCallback2(CB2_InitBattle);
}
'''
main=(game/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(CaptureItemBoot);')
main=main.replace('#include "global.h"','#include "global.h"\nvoid CaptureItemCheck(void);',1).replace('    ReadKeys();','    CaptureItemCheck();\n    ReadKeys();')
assert 'CaptureItemCheck();' in main and 'CaptureItemBoot);' in main
(test/'src/main.c').write_text(main)
p=test/'src/battle_main.c';p.write_text(p.read_text().replace('static void HandleTurnActionSelectionState(void)','void HandleTurnActionSelectionState(void)'))
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'capture-item-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/capture_item_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'capture'),*['0x'+symbols[s] for s in ('gCaptureItemCase','gCaptureItemStage','gCaptureItemText')]],capture_output=True,text=True,timeout=180)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
chars={}
for line in (game/'charmap.txt').read_text(encoding='utf-8').splitlines():
 m=re.match(r"^'((?:\\.|[^'])*)'\s*=\s*([0-9A-F]{2})\s*$",line)
 if m:chars.setdefault(int(m[2],16),m[1].replace("\\'","'"))
def decode(data):
 text=''
 for byte in data:
  if byte==0xFF:break
  text+='\n' if byte==0xFE else '\\p' if byte==0xFB else chars.get(byte,f'<{byte:02x}>')
 return text
rows={int(m[1]):(int(m[2]),decode(bytes.fromhex(m[3])) if int(m[3],16) else '') for m in re.finditer(r'case=(\d+) stage=(\d+) text=([0-9a-f]+)',result.stdout)}
expected={1:'PIKACHU was holding\nLIGHT BALL!\\p',2:'',3:'PIKACHU estava segurando\nLIGHT BALL!\\p'}
for case,text in expected.items():
 stage,shown=rows[case]
 assert stage==3,(case,'script did not reach the Pokedex step',stage)
 assert shown==text,(case,shown,text)
print('PASS: caught Pokemon holding an item shows the item (EN and PT-BR); without an item no message is shown.')
assert hashlib.sha256((game/'output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha
(out/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'status':'passed','cases':{str(k):v for k,v in rows.items()}},ensure_ascii=False,indent=2)+'\n')
