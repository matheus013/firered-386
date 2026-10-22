"""Exercise settings, localization and challenge builds in a disposable ROM."""
from pathlib import Path
import hashlib,json,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[1];out=root/'output/tests/settings';out.mkdir(parents=True,exist_ok=True)
source=Path((root/'output/hack-build-path.txt').read_text().strip())
reference_sha=hashlib.sha256((root/'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
test=Path(tempfile.mkdtemp(prefix='settings-fixture-',dir=Path.home()/'.cache/firered-386'));shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
main=(test/'src/main.c').read_text();hook=(root/'scripts/settings_fixture.c').read_text()
(test/'src/main.c').write_text(main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(SettingsTestBoot);'))
with (test/'src/battle_main.c').open('a') as f:f.write('\nu8 SettingsTestCreateParty(u16 trainer){return CreateNPCTrainerParty(gEnemyParty,trainer);}\n')
with (test/'src/battle_main.c').open('a') as f:f.write('''
u16 SettingsTestOriginalField(u16 trainer,u8 slot,bool8 level) {
 switch(gTrainers[trainer].partyFlags) {
 case 0: return level?gTrainers[trainer].party.NoItemDefaultMoves[slot].lvl:gTrainers[trainer].party.NoItemDefaultMoves[slot].species;
 case F_TRAINER_PARTY_CUSTOM_MOVESET:return level?gTrainers[trainer].party.NoItemCustomMoves[slot].lvl:gTrainers[trainer].party.NoItemCustomMoves[slot].species;
 case F_TRAINER_PARTY_HELD_ITEM:return level?gTrainers[trainer].party.ItemDefaultMoves[slot].lvl:gTrainers[trainer].party.ItemDefaultMoves[slot].species;
 default:return level?gTrainers[trainer].party.ItemCustomMoves[slot].lvl:gTrainers[trainer].party.ItemCustomMoves[slot].species;
 }
}
''')
with (test/'src/wild_encounter.c').open('a') as f:f.write('\nvoid SettingsTestWild(u16 species,u8 level){GenerateWildMon(species,level,0);}\n')
with (test/'src/randomizer.c').open('a') as f:f.write('\nbool8 SettingsTestLegend(u16 species){return IsLegend(species);}\n')
with (test/'src/trainer_tower.c').open('a') as f:f.write('''
u8 SettingsTestTowerParty(u8 course,u8 floor,u8 opponent) {
    u8 type;
    gSaveBlock1Ptr->towerChallengeId=course;
    gSaveBlock1Ptr->trainerTower[course].floorsCleared=floor;
    sTrainerTowerState=AllocZeroed(sizeof(*sTrainerTowerState));
    sTrainerTowerState->floorIdx=floor;
    sTrainerTowerState->data.floors[floor]=*gTrainerTowerFloors[course][floor];
    type=CURR_FLOOR.challengeType;
    VarSet(VAR_TEMP_1,opponent);BuildEnemyParty();FreeTrainerTowerDataStruct();
    return type==CHALLENGE_TYPE_KNOCKOUT?3:1;
}
''')
with (test/'src/localization.c').open('a') as f:f.write('''
bool8 SettingsTestLocale(void) {
    u16 i;
    for(i=0;i<ARRAY_COUNT(sPtLocale);i++) if(L10n_GetText(sPtLocale[i].en)!=sPtLocale[i].pt)return FALSE;
    return TRUE;
}
''')
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'settings-smoke';subprocess.run(['gcc','-O2',str(root/'scripts/settings_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
rom=out/'settings-fixture.gba';shutil.copyfile(test/'pokefirered.gba',rom)
with (out/'emulator.log').open('w') as log:
 result=subprocess.run([str(runner),str(rom),str(out/'settings-test-only.sav'),'0x'+symbols['gHardcoreTestResult'],'0x'+symbols['gSettingsAudit']],stdout=log,stderr=subprocess.STDOUT,timeout=300)
assert hashlib.sha256((root/'output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha,'Production ROM changed during tests'
report={'rom_sha256':reference_sha,'fixture_sha256':hashlib.sha256(rom.read_bytes()).hexdigest(),'status':'passed' if result.returncode==0 else 'failed','exit_code':result.returncode,'scope':'disposable native fixture; regular and Tower parties in both randomizer states; 386-species permutation, randomized capture quotas, pre-League legendary gate, starters, all localization lookups, native save and fresh-instance reload'}
(out/'results.json').write_text(json.dumps(report,indent=2)+'\n');print(report,flush=True)
result.check_returncode()
