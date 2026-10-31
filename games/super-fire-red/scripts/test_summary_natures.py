"""Open the real summary screen for all 25 natures and check the stat colors on screen, disposable ROM."""
from pathlib import Path
import hashlib,json,re,shutil,subprocess,tempfile
from png_util import read_png
root=Path(__file__).resolve().parents[3];game=root/'games/super-fire-red'
out=game/'output/tests/summary-natures';out.mkdir(parents=True,exist_ok=True)
for old in out.glob('*.png'):old.unlink()
source=Path((game/'output/hack-build-path.txt').read_text().strip())
reference_sha=hashlib.sha256((game/'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
test=Path(tempfile.mkdtemp(prefix='summary-natures-',dir=source.parent));shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
hook='''
#include "new_game.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "string_util.h"
EWRAM_DATA volatile u32 gSummaryNatureCase = 0;
static void SummaryNatureIdle(void) {}
static void SummaryNatureBoot(void)
{
    static const u8 name[] = _("TEST");
    u8 slot;
    if (!gSummaryNatureCase) return;
    SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();
    // Personality picks the nature; set N covers natures 5(N-1)..5(N-1)+4.
    for (slot = 0; slot < 5; slot++)
        CreateMon(&gPlayerParty[slot],SPECIES_MEWTWO,50,31,TRUE,(gSummaryNatureCase-1)*5+slot,OT_ID_PLAYER_ID,0);
    CalculatePlayerPartyCount();
    ShowPokemonSummaryScreen(gPlayerParty,0,4,SummaryNatureIdle,PSS_MODE_NORMAL);
}
'''
main=(game/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(SummaryNatureBoot);')
assert 'SummaryNatureBoot);' in main
(test/'src/main.c').write_text(main)
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'summary-nature-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(game/'scripts/summary_nature_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
with (out/'emulator.log').open('w') as log:
 subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'summary'),'0x'+symbols['gSummaryNatureCase']],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=300)
reference=json.loads((game/'hack-data/nature_reference.json').read_text())
ids={name:int(value) for name,value in re.findall(r'#define NATURE_(\w+)\s+(\d+)',(game/'include/constants/pokemon.h').read_text())}
screen_rows=['attack','defense','spatk','spdef','speed'] # summary order: ATK, DEF, SP. ATK, SP. DEF, SPEED
for raised,row in zip(reference['stats'],reference['grid']):
 for lowered,nature in zip(reference['stats'],row):
  green=set() if raised==lowered else {screen_rows.index(raised)};red=set() if raised==lowered else {screen_rows.index(lowered)}
  pixels=read_png(out/f'summary-nature-{ids[nature]:02d}.png')
  found={color:{(y-38)//13 for y in range(38,103) for x in range(160,240) if pixels[y][x]==value} for color,value in (('green',(4,18,6)),('red',(26,4,4)))}
  assert (found['green'],found['red'])==(green,red),(nature,'green rows',found['green'],'red rows',found['red'],'expected',green,red)
print('PASS: summary stats page colors only the raised (green) and lowered (red) stat for all 25 natures.')
assert hashlib.sha256((game/'output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha
(out/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'status':'passed','natures':25},indent=2)+'\n')
