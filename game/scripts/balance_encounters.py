"""Deterministic 386-species distribution; nominal slot odds, habitat and power gates."""
from pathlib import Path
from collections import Counter, defaultdict
import argparse
import hashlib
import json
import re
import struct
import subprocess
from source_base import base_bytes

ROOT = Path(__file__).resolve().parents[2]
GAME = ROOT/'game'
BASE = 'ce6406d8c2eaf46d543c8a2606de0c3cb1951982'
LEGENDS = dict(
    ARTICUNO=('FOUR_ISLAND_ICEFALL_CAVE_1F','land_mons',11),
    ZAPDOS=('SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE','land_mons',10),
    MOLTRES=('MT_EMBER_RUBY_PATH_B1F','land_mons',11),
    MEWTWO=('CERULEAN_CAVE_B1F','land_mons',11),
    MEW=('FIVE_ISLAND_LOST_CAVE_ROOM10','land_mons',11),
    RAIKOU=('FIVE_ISLAND_MEADOW','land_mons',11),
    ENTEI=('MT_EMBER_RUBY_PATH_B2F','land_mons',11),
    SUICUNE=('FOUR_ISLAND_ICEFALL_CAVE_BACK','water_mons',4),
    LUGIA=('SIX_ISLAND_OUTCAST_ISLAND','water_mons',4),
    HO_OH=('SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE','land_mons',11),
    CELEBI=('SIX_ISLAND_PATTERN_BUSH','land_mons',11),
    REGIROCK=('SEVEN_ISLAND_SEVAULT_CANYON','land_mons',11),
    REGICE=('FOUR_ISLAND_ICEFALL_CAVE_B1F','land_mons',11),
    REGISTEEL=('SIX_ISLAND_RUIN_VALLEY','land_mons',11),
    LATIAS=('FIVE_ISLAND_RESORT_GORGEOUS','water_mons',4),
    LATIOS=('FIVE_ISLAND_MEMORIAL_PILLAR','land_mons',11),
    KYOGRE=('FIVE_ISLAND_WATER_LABYRINTH','water_mons',4),
    GROUDON=('MT_EMBER_RUBY_PATH_B3F','land_mons',11),
    RAYQUAZA=('CERULEAN_CAVE_2F','land_mons',11),
    JIRACHI=('FIVE_ISLAND_LOST_CAVE_ROOM14','land_mons',11),
    DEOXYS=('SIX_ISLAND_ALTERING_CAVE','land_mons',11),
)
AQUATIC = set('MAGIKARP GYARADOS FEEBAS MILOTIC GOLDEEN SEAKING CARVANHA SHARPEDO REMORAID OCTILLERY QWILFISH CHINCHOU LANTURN HORSEA SEADRA KINGDRA TENTACOOL TENTACRUEL WAILMER WAILORD CLAMPERL HUNTAIL GOREBYSS LUVDISC RELICANTH SHELLDER CLOYSTER BARBOACH WHISCASH CORPHISH CRAWDAUNT KRABBY KINGLER STARYU STARMIE DRATINI DRAGONAIR'.split())
STARTERS = set('BULBASAUR CHARMANDER SQUIRTLE CHIKORITA CYNDAQUIL TOTODILE TREECKO TORCHIC MUDKIP'.split())
RARE_STARTERS = STARTERS | set('IVYSAUR VENUSAUR CHARMELEON CHARIZARD WARTORTLE BLASTOISE BAYLEEF MEGANIUM QUILAVA TYPHLOSION CROCONAW FERALIGATR GROVYLE SCEPTILE COMBUSKEN BLAZIKEN MARSHTOMP SWAMPERT'.split())
LEVELS = {1:(3,12),2:(17,25),3:(27,33),4:(35,41),5:(44,50),6:(54,59),7:(62,67),8:(70,75),9:(84,94)}
OPENING = {
    'ROUTE1':'PIDGEY RATTATA SENTRET ZIGZAGOON TAILLOW POOCHYENA HOOTHOOT SPEAROW SUNKERN WURMPLE PICHU HOPPIP',
    'ROUTE2':'PIDGEY RATTATA CATERPIE WEEDLE LOTAD SEEDOT BELLSPROUT ODDISH RALTS SURSKIT NIDORAN_F NIDORAN_M',
    'ROUTE22':'MANKEY SPEAROW NIDORAN_M NIDORAN_F MARILL WOOPER PHANPY AZURILL TYROGUE MAKUHITA RALTS TRAPINCH',
    'VIRIDIAN_FOREST':'CATERPIE WEEDLE WURMPLE PIKACHU SHROOMISH PARAS METAPOD KAKUNA SILCOON CASCOON PINECO SPINARAK',
}


def read(path): return path.read_text(encoding='utf-8-sig')
def digest(text): return int(hashlib.sha256(text.encode()).hexdigest()[:8],16)


class Catalog:
    def __init__(self):
        dex = read(GAME/'include/constants/pokedex.h')
        # The first enum ends at Deoxys; ignore subsequent regional orders.
        self.names = re.findall(r'\bNATIONAL_DEX_(\w+)\s*,',dex)
        self.names = self.names[1:self.names.index('DEOXYS')+1]
        assert len(self.names)==386 and self.names[0]=='BULBASAUR'
        self.info={}
        for name, body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{\n(.*?)\n    \}',read(GAME/'src/data/pokemon/species_info.h'),re.S):
            if name not in self.names: continue
            stats=[int(re.search(r'\.base'+s+r'\s*=\s*(\d+)',body)[1]) for s in ('HP','Attack','Defense','Speed','SpAttack','SpDefense')]
            self.info[name]={'bst':sum(stats),'types':set(re.findall(r'TYPE_(\w+)',re.search(r'\.types = \{([^}]+)',body)[1]))}
        self.evo_level={}
        for body in re.findall(r'\[SPECIES_\w+\]\s*=\s*\{(.*?)(?=\n\s*\[SPECIES_|\n\};)',read(GAME/'src/data/pokemon/evolution.h'),re.S):
            for level, species in re.findall(r'\{EVO_LEVEL,\s*(\d+),\s*SPECIES_(\w+)\}',body):
                self.evo_level[species]=int(level)
        maps=[json.loads(read(p)) for p in (GAME/'data/maps').glob('*/map.json')]
        self.maps={m['id']:m for m in maps}
        self.layouts={l['id']:l for l in json.loads(read(GAME/'data/layouts/layouts.json'))['layouts'] if 'id' in l}
        attrs={name:[v[0] for v in struct.iter_unpack('<I',(GAME/path).read_bytes())] for name,path in re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)',read(GAME/'src/data/tilesets/metatiles.h'))}
        self.tilesets={name:attrs[re.search(r'\.metatileAttributes = (\w+)',body)[1]] for name,body in re.findall(r'const struct Tileset (\w+) =\s*\{(.*?)\};',read(GAME/'src/data/tilesets/headers.h'),re.S)}

    def has_land(self, map_id):
        layout=self.layouts[self.maps[map_id]['layout']]
        for block, in struct.iter_unpack('<H',(GAME/layout['blockdata_filepath']).read_bytes()):
            tile=block&0x3ff
            attrs=self.tilesets[layout['primary_tileset']][tile] if tile<640 else self.tilesets[layout['secondary_tileset']][tile-640]
            if (attrs>>24)&7==1: return True
        return False

    def minimum(self,species):
        if species in LEGENDS: return 9
        bst=self.info[species]['bst']
        stage=8 if bst>=600 else 7 if bst>=550 else 6 if bst>=500 else 5 if bst>=450 else 4 if bst>=400 else 3 if bst>=350 else 1
        if species in STARTERS: stage=max(stage,2)
        if species in {'DRATINI','LARVITAR','BAGON','BELDUM','DITTO','SMEARGLE','WYNAUT'}:stage=max(stage,4)
        if species in {'WOBBUFFET','CHANSEY','SHEDINJA'}:stage=max(stage,5)
        evo=self.evo_level.get(species,1)
        while stage<9 and LEVELS[stage][1]<evo:stage+=1
        return stage

    def max_weight(self,species):
        if species in LEGENDS or self.info[species]['bst']>=600 or species in {'CHANSEY','BLISSEY'}:return 1
        if species in {'DITTO','SMEARGLE'}:return 4
        if species in RARE_STARTERS or species in {'DRATINI','DRAGONAIR','LARVITAR','BAGON','BELDUM','GYARADOS','MILOTIC','KINGDRA','SNORLAX','HERACROSS','PINSIR','SCYTHER','SKARMORY','LAPRAS','OMANYTE','KABUTO','LILEEP','ANORITH'}:return 5
        return 10 if self.info[species]['bst']>=500 else 20 if self.info[species]['bst']>=450 else 100


def map_stage(name):
    n=name.removeprefix('MAP_')
    if n.startswith(('CERULEAN_CAVE','FOUR_ISLAND','FIVE_ISLAND','SIX_ISLAND','SEVEN_ISLAND','MT_EMBER_RUBY_PATH')): return 9
    if n.startswith(('ONE_ISLAND','TWO_ISLAND','THREE_ISLAND','MT_EMBER')): return 8
    if n.startswith(('VICTORY_ROAD',)):return 8
    if n.startswith(('POKEMON_MANSION','CINNABAR')):return 7
    if n.startswith(('SEAFOAM','POWER_PLANT')):return 6
    if n.startswith(('POKEMON_TOWER','SAFARI_ZONE','FUCHSIA')):return 5
    if n.startswith(('ROCK_TUNNEL','LAVENDER','CELADON')):return 4
    if n.startswith(('DIGLETTS','SSANNE','VERMILION')):return 3
    if n.startswith(('MT_MOON','CERULEAN_CITY')):return 2
    if n in {'VIRIDIAN_FOREST','PALLET_TOWN','VIRIDIAN_CITY'}:return 1
    if n.startswith('ROUTE'):
        route=int(re.search(r'ROUTE(\d+)',n)[1])
        if route in {1,2,22}:return 1
        if route in {3,4,24,25}:return 2
        if route in {5,6,11}:return 3
        if route in {7,8,9,10}:return 4
        if 12<=route<=18:return 5
        if 19<=route<=21:return 6
        if route==23:return 8
    raise ValueError('Unclassified map: '+name)


def method_stage(map_id,field,index):
    stage=map_stage(map_id)
    if field=='water_mons':stage=max(stage,6) # Surf arrives after Koga.
    if field=='rock_smash_mons':stage=max(stage,8) # Guaranteed access after Kindle Road.
    if field=='fishing_mons':stage=max(stage,3 if index<2 else 5)
    return stage


def habitat(cat,species,slot):
    types=cat.info[species]['types']; method=slot['field']; name=slot['map']
    if method in {'water_mons','fishing_mons'}:
        return 100 if 'WATER' in types or species in AQUATIC else None
    if species in AQUATIC:return None
    if method=='rock_smash_mons':return 100 if types&{'ROCK','GROUND','BUG'} else None
    if 'POKEMON_TOWER' in name:return 150 if types&{'GHOST','DARK','PSYCHIC'} else None
    if species in {'ZUBAT','GOLBAT','CROBAT'} and any(x in name for x in ('CAVE','MOON','TUNNEL')):return 120
    if species in {'CLEFFA','CLEFAIRY','CLEFABLE'} and 'MOON' in name:return 120
    if any(x in name for x in ('SEAFOAM','ICEFALL')) and not types&{'ICE','WATER','ROCK'}:return None
    if 'POWER_PLANT' in name and not types&{'ELECTRIC','STEEL','POISON'}:return None
    if 'EMBER' in name and not types&{'FIRE','GROUND','ROCK','FIGHTING'}:return None
    if 'MOON' in name and not types&{'ROCK','GROUND','DARK','STEEL','GHOST','BUG'}:return None
    wanted=set()
    for fragment, theme in [('FOREST','BUG GRASS'),('BUSH','BUG GRASS'),('BOND','BUG GRASS'),('CAVE','ROCK GROUND DARK'),('TUNNEL','ROCK GROUND'),('MOON','ROCK GROUND'),('MANSION','FIRE POISON DARK'),('POWER_PLANT','ELECTRIC STEEL'),('SEAFOAM','ICE WATER'),('ICEFALL','ICE WATER'),('EMBER','FIRE GROUND'),('RUIN','PSYCHIC STEEL'),('SEVAULT','GROUND ROCK DRAGON'),('BEACH','WATER GRASS'),('SAFARI','NORMAL GRASS GROUND'),('LOST','GHOST DARK PSYCHIC')]:
        if fragment in name:wanted.update(theme.split())
    return 65 if types&wanted else 30 if not wanted else 0


def generate():
    cat=Catalog()
    original=json.loads(base_bytes('src/data/wild_encounters.json'))
    group=original['wild_encounter_groups'][0]
    fields={f['type']:f for f in group['fields']}
    active={};inactive=[]
    for row in group['encounters']:
        if not row['base_label'].endswith('_FireRed'):continue
        if row['map'] in active:inactive.append(row);continue
        active[row['map']]=row
    slots=[];pinned=set(); placements={}
    for map_id,row in active.items():
        # The ship departs before Surf/advanced rods become normally available.
        # Keep its baseline tables, but never use them as an availability proof.
        if map_id=='MAP_SSANNE_EXTERIOR':continue
        if 'TANOBY_RUINS_' in map_id:continue # Runtime Unown form selection uses these chambers.
        for field,table in row.items():
            if field not in fields:continue
            if field=='land_mons' and not cat.has_land(map_id):continue
            for i,mon in enumerate(table['mons']):
                slot={'map':map_id,'field':field,'index':i,'weight':fields[field]['encounter_rates'][i], 'stage':method_stage(map_id,field,i), 'mon':mon}
                slots.append(slot)
    bykey={(s['map'],s['field'],s['index']):s for s in slots}

    def assign(species,slot):
        level=LEVELS[slot['stage']]
        lo=max(level[0],cat.evo_level.get(species,1))
        hi=max(lo,level[1])
        if species in LEGENDS:lo,hi=(90,95) if cat.info[species]['bst']>=650 else (85,90)
        # Keep the first route's opening levels close to the untouched lab battle.
        if slot['map']=='MAP_ROUTE1':lo,hi=max(3,cat.evo_level.get(species,1)),max(6,cat.evo_level.get(species,1))
        elif slot['map'] in {'MAP_ROUTE2','MAP_ROUTE22'} and slot['field']=='land_mons':lo,hi=5,12
        elif slot['map']=='MAP_VIRIDIAN_FOREST':lo,hi=8,15
        mon=slot['mon'];mon.update(species='SPECIES_'+species,min_level=lo,max_level=hi)

    for species,(map_name,field,i) in LEGENDS.items():
        slot=bykey[('MAP_'+map_name,field,i)]
        assert slot['stage']==9 and slot['weight']==1
        assign(species,slot);pinned.add(id(slot));placements[species]=slot
    for map_name, roster in OPENING.items():
        for i,species in enumerate(roster.split()):
            slot=bykey[('MAP_'+map_name,'land_mons',i)]
            assert cat.minimum(species)==1,(map_name,species)
            assign(species,slot);pinned.add(id(slot));placements.setdefault(species,slot)
    # Every ordinary species receives at least one physical, non-event slot.
    # Place the strongest first, choosing an appropriate earliest stage/habitat.
    map_used=Counter()
    ordinary=[s for s in cat.names if s not in LEGENDS and s!='UNOWN']
    for species in sorted((s for s in ordinary if s not in placements),key=lambda s:(-cat.minimum(s),-cat.info[s]['bst'],s)):
        minimum=cat.minimum(species)
        desired=4 if minimum>=6 else 5 if species in STARTERS or minimum>=4 else 10 if minimum>=2 else 20
        options=[]
        for slot in slots:
            if id(slot) in pinned or slot['stage']<minimum or slot['weight']>cat.max_weight(species):continue
            fit=habitat(cat,species,slot)
            if fit is None:continue
            # Route 1 remains ordinary early wildlife, without starter gifts.
            if slot['map']=='MAP_ROUTE1' and (minimum>1 or cat.info[species]['bst']>320):continue
            score=(slot['stage']-minimum)*160-fit+abs(slot['weight']-desired)*3+map_used[slot['map']]*7
            options.append((score,digest(species+slot['map']+slot['field']+str(slot['index'])),slot))
        assert options, 'No eligible habitat for '+species
        slot=min(options,key=lambda option:option[:2])[2]
        assign(species,slot);pinned.add(id(slot));placements[species]=slot;map_used[slot['map']]+=1
    for slot in slots:
        if id(slot) in pinned:continue
        old=slot['mon']['species'].removeprefix('SPECIES_')
        allowed=[s for s in ordinary if cat.minimum(s)<=slot['stage'] and slot['weight']<=cat.max_weight(s) and habitat(cat,s,slot) is not None]
        if old in allowed and cat.minimum(old)>=slot['stage']-2:
            species=old
        else:
            # Prefer habitat and power near the local stage, with seeded variety.
            candidates=sorted(allowed,key=lambda s:(-(habitat(cat,s,slot) or 0)+max(0,slot['stage']-cat.minimum(s)-1)*20,digest(s+slot['map']+str(slot['index'])+slot['field'])))
            species=candidates[digest(slot['map']+slot['field']+str(slot['index']))%min(8,len(candidates))]
        assign(species,slot)
    # Unown stays in Tanoby at its original level and form weights.
    for row in active.values():
        if 'TANOBY_RUINS_' in row['map']:
            for mon in row['land_mons']['mons']: mon['min_level']=mon['max_level']=50
    report=[]
    for number,species in enumerate(cat.names,1):
        found=[]
        for slot in slots:
            if slot['mon']['species']=='SPECIES_'+species:
                found.append({k:slot[k] for k in ('map','field','index','weight','stage')}|{'min_level':slot['mon']['min_level'],'max_level':slot['mon']['max_level']})
                assert slot['stage']>=cat.minimum(species),(species,slot)
                assert slot['weight']<=cat.max_weight(species),(species,slot)
        if species=='UNOWN':found=[{'map':m,'field':'land_mons','weight':100,'stage':9,'min_level':50,'max_level':50} for m in active if 'TANOBY_RUINS_' in m]
        assert found,species
        if species in LEGENDS:assert all(x['stage']==9 and x['weight']==1 for x in found)
        report.append({'national_dex':number,'species':species,'bst':cat.info[species]['bst'],'minimum_stage':cat.minimum(species),'encounters':found})
    payload={'version':'v0.3.0-dev','species_count':386,'legendary_mythical_count':len(LEGENDS),'scope':'direct wild capture in Normal; Hardcore quotas remain','species':report}
    return original,payload


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true');args=parser.parse_args()
    wild,report=generate()
    outputs={GAME/'src/data/wild_encounters.json':wild,ROOT/'game/hack-data/encounter_distribution.json':report}
    for path,value in outputs.items():
        content=json.dumps(value,ensure_ascii=False,indent=2)+'\n'
        if args.check:assert read(path)==content,'Out of date: '+str(path)
        else:path.write_text(content,encoding='utf-8',newline='\n')
    guard='// Generated by scripts/balance_encounters.py.\nstatic bool8 IsPostgameWildLegend(u16 species)\n{\n    switch (species)\n    {\n'+''.join('    case SPECIES_'+s+':\n' for s in LEGENDS)+'        return TRUE;\n    default:\n        return FALSE;\n    }\n}\n'
    path=GAME/'src/data/legendary_encounters.h'
    if args.check:assert read(path)==guard
    else:path.write_text(guard,encoding='utf-8',newline='\n')
    print('PASS: 386/386 direct wild species; 21 legendary/mythical species in postgame 1% slots; no trade/event dependency.')


if __name__=='__main__':main()
