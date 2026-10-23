"""Exercise every actual Pokemart catalog in Normal/Hardcore via the game UI."""
from pathlib import Path
import argparse,hashlib,json,re,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
parser=argparse.ArgumentParser();parser.add_argument('--before-fix',action='store_true');parser.add_argument('--case',type=int);parser.add_argument('--language',choices=['en','ptbr'],default='en');args=parser.parse_args()
out=root/'games/super-fire-red/output/tests'/('shops' if args.language=='en' else 'shops-ptbr');out.mkdir(parents=True,exist_ok=True)
reference_sha=hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()
source=Path((root/'games/super-fire-red/output/hack-build-path.txt').read_text().strip())
test=Path(tempfile.mkdtemp(prefix='shop-fixture-',dir=Path.home()/'.cache/firered-386'));shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
catalogs=[]
for path in sorted((game/'data/maps').glob('*/scripts.inc')):
 text=path.read_text()
 for stock in re.findall(r'^\s*pokemart\s+(\w+)',text,re.M):
  m=json.loads((path.parent/'map.json').read_text())
  warp=m['warp_events'][0] if m['warp_events'] else {'x':8,'y':6}
  catalogs.append({'map':m['id'],'stock':stock,'x':warp['x'],'y':max(1,warp['y']-1)})
assert len(catalogs)==23
(out/'catalogs.json').write_text(json.dumps(catalogs,indent=2)+'\n')
hook='''
#include "new_game.h"
#include "load_save.h"
#include "overworld.h"
#include "nuzlocke.h"
#include "game_settings.h"
#include "pokemon.h"
#include "string_util.h"
#include "event_data.h"
#include "item.h"
#include "money.h"
#include "script.h"
#include "shop.h"
#include "constants/items.h"
#include "constants/maps.h"
extern const u8 ShopTest_Event[];
extern bool32 CheckHeap(void);
extern u32 ShopTestStockInfo(void);
EWRAM_DATA u32 gShopTestCase=0,gShopTestMode=0,gShopTestCommand=0,gShopTestMoney=0,gShopTestDone=0,gShopTestQty=0,gShopTestStock=0,gShopTestCount=0,gShopTestPrice=0,gShopTestClosed=0;
static EWRAM_DATA bool8 sShopWasOpen=FALSE;
'''
hook+='\n'.join('extern const u16 '+c['stock']+'[];' for c in catalogs)+'\n'
hook+='static const u16 *const sShopStocks[]={'+','.join(c['stock'] for c in catalogs)+'};\n'
hook+='static const u16 sShopMaps[]={'+','.join(c['map'] for c in catalogs)+'};\n'
hook+='static const u8 sShopPos[][2]={'+','.join('{%d,%d}'%(c['x'],c['y']) for c in catalogs)+'};\n'
hook+='''
void ShopTestSetup(void)
{
    Nuzlocke_ChooseMode(gShopTestMode ? GAME_MODE_HARDCORE : GAME_MODE_NORMAL);
    FlagSet(FLAG_SYS_POKEDEX_GET);
    VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_MART,1);
    gSaveBlock2Ptr->optionsTextSpeed=2;
    SetMoney(&gSaveBlock1Ptr->money,900000);
    AddBagItem(ITEM_POTION,5);
    CreateMon(&gPlayerParty[0],SPECIES_BULBASAUR,5,15,FALSE,0,OT_ID_PLAYER_ID,0);
    CalculatePlayerPartyCount();
    SetWarpDestination(MAP_GROUP(sShopMaps[gShopTestCase]),MAP_NUM(sShopMaps[gShopTestCase]),-1,sShopPos[gShopTestCase][0],sShopPos[gShopTestCase][1]);
    WarpIntoMap();
}
void ShopTestOpen(void) {CreatePokemartMenu(sShopStocks[gShopTestCase]);}
const u16 *ShopTestExpectedStock(void) {return sShopStocks[gShopTestCase];}
static void ShopTestIdle(void) {}
static void ShopTestPoll(void)
{
    static const u8 name[] = _("TEST");
    if(gShopTestCommand==1){
        gShopTestCommand=0;SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);SetMainCallback2(CB2_NewGame);
    }
    if(gShopTestCommand==2){gShopTestCommand=0;gShopTestDone=0;sShopWasOpen=TRUE;ScriptContext_SetupScript(ShopTest_Event);}
    if(gShopTestMoney || gPlayerPartyCount){
        gShopTestMoney=GetMoney(&gSaveBlock1Ptr->money);
        gShopTestQty=BagGetQuantityByItemId(sShopStocks[gShopTestCase][0]);
        gShopTestPrice=ItemId_GetPrice(sShopStocks[gShopTestCase][0]);
        gShopTestStock=ShopTestStockInfo();
    }
    if(sShopWasOpen && !ScriptContext_IsEnabled() && !ArePlayerFieldControlsLocked()){
        gShopTestDone=CheckHeap()?1:2;gShopTestClosed++;sShopWasOpen=FALSE;
    }
}
static void ShopTestBoot(void){SetMainCallback2(ShopTestIdle);}
'''
hook=hook.replace('    FlagSet(FLAG_SYS_POKEDEX_GET);','    GameSettings_SetLanguage('+('GAME_LANGUAGE_PTBR' if args.language=='ptbr' else 'GAME_LANGUAGE_EN')+');\n    FlagSet(FLAG_SYS_POKEDEX_GET);')
main=(game/'src/main.c').read_text().replace('#include "global.h"','#include "global.h"\nstatic void ShopTestPoll(void);')
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(ShopTestBoot);').replace('    ReadKeys();','    ShopTestPoll();\n    ReadKeys();')
(test/'src/main.c').write_text(main)
world=(game/'src/overworld.c').read_text().replace('    NewGameInitData();\n    ResetInitialPlayerAvatarState();','    NewGameInitData();\n    ShopTestSetup();\n    ResetInitialPlayerAvatarState();').replace('#include "global.h"','#include "global.h"\nextern void ShopTestSetup(void);')
(test/'src/overworld.c').write_text(world)
scripts=(game/'data/event_scripts.s').read_text()+'\nShopTest_Event::\n\tlockall\n\tcallnative ShopTestOpen\n\twaitstate\n\treleaseall\n\tend\n'
(test/'data/event_scripts.s').write_text(scripts)
shop=(root/'.build/shop-before-fix.c' if args.before_fix else game/'src/shop.c').read_text()
access='sShopData.itemList[i]' if args.before_fix else 'GetShopItemId(i)'
shop+='''
extern u32 gShopTestCount;
extern u32 gShopTestMode;
extern const u16 *ShopTestExpectedStock(void);
u32 ShopTestStockInfo(void) {
    u16 i, expectedCount=0;
    bool8 hasCandy=FALSE, addCandy;
    const u16 *expected=ShopTestExpectedStock();
    while(expected[expectedCount]) {if(expected[expectedCount]==ITEM_RARE_CANDY) hasCandy=TRUE;expectedCount++;}
    addCandy=gShopTestMode && !hasCandy;
    gShopTestCount=sShopData.itemCount;
    if(sShopData.itemCount!=expectedCount+addCandy) return 0;
    for(i=0;i<expectedCount;i++) if('''+access+'''!=expected[i]) return 0;
    if(addCandy && '''+access+'''!=ITEM_RARE_CANDY) return 0;
    return 1;
}
'''
(test/'src/shop.c').write_text(shop)
with (out/('before-fix-build.log' if args.before_fix else 'build.log')).open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'shop-smoke';subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'games/super-fire-red/scripts/shop_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
names=('gShopTestCase','gShopTestMode','gShopTestCommand','gShopTestMoney','gShopTestDone','gShopTestQty','gShopTestStock','gTasks','Task_ShopMenu','Task_BuyMenu','Task_BagMenu_HandleInput')
cases=[args.case] if args.case is not None else range(len(catalogs))
passed=[]
for i in cases:
 for mode in (0,1):
  label=('before-fix-' if args.before_fix else '')+str(i)+'-'+('hardcore' if mode else 'normal')
  addresses=['0x'+symbols[s] for s in names]
  addresses += [str(i),str(mode),'0x'+symbols['gShopTestCount'],'0x'+symbols['gShopTestPrice'],'0x'+symbols['gShopTestClosed']]
  result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/label),*addresses],capture_output=True,text=True,timeout=90)
  (out/(label+'.log')).write_text(result.stdout+result.stderr)
  assert result.returncode==0,(catalogs[i]['stock'],label,result.stdout,result.stderr[-1500:])
  print(result.stdout.strip(),catalogs[i]['stock'],flush=True)
  passed.append({'catalog':catalogs[i]['stock'],'map':catalogs[i]['map'],'mode':'Hardcore' if mode else 'Normal','passed':True})
if args.case is None:
 assert hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha,'Production ROM changed during tests'
 (out/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'language':args.language,'cases':passed,'count':len(passed),'flow':'cancel; buy; sell; quit B/SEE YA; reopen; Hardcore Candy=1; exact stock; heap integrity and script resume'},indent=2)+'\n')
 print(f'PASS: all {len(catalogs)} shop catalogs, both modes ({len(passed)} cases).')
