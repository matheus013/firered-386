"""Build a reviewable doubles pass from the pinned vanilla trainer data.

Uses the CURRENT engine's learnsets/types. Rerun and review after the modern engine
lands; this is not a claim to have implemented modern moves or competitive EVs.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT / "game"
BASE = "ce6406d8c2eaf46d543c8a2606de0c3cb1951982"
PARTY_RE = re.compile(r"static const struct (TrainerMon\w+) (sParty_\w+)\[\] = \{(.*?)\};", re.S)
TRAINER_RE = re.compile(r"    \[(TRAINER_\w+)\] = \{(.*?)\n    \},", re.S)
LAB = {"sParty_RivalOaksLabSquirtle", "sParty_RivalOaksLabBulbasaur", "sParty_RivalOaksLabCharmander"}
BANNED = set("NONE SELF_DESTRUCT EXPLOSION EARTHQUAKE SURF MAGNITUDE FISSURE HORN_DRILL GUILLOTINE SHEER_COLD DOUBLE_TEAM MINIMIZE METRONOME ASSIST MIMIC SKETCH MIRROR_MOVE SLEEP_TALK SNORE DREAM_EATER NIGHTMARE RETURN FRUSTRATION HIDDEN_POWER HYPER_BEAM SOLAR_BEAM FOCUS_PUNCH COUNTER MIRROR_COAT BIDE FLAIL REVERSAL ENDEAVOR PRESENT FUTURE_SIGHT DOOM_DESIRE LAST_RESORT DIG DIVE FLY BOUNCE RAZOR_WIND SKULL_BASH".split())
SUPPORT = "ICY_WIND THUNDER_WAVE STRING_SHOT ROCK_TOMB REFLECT LIGHT_SCREEN HELPING_HAND SLEEP_POWDER HYPNOSIS STUN_SPORE SPORE WILL_O_WISP TOXIC LEECH_SEED SCREECH TAIL_WHIP LEER GROWL POISON_POWDER SUPERSONIC CONFUSE_RAY".split()
SECONDARY = "PROTECT DETECT RECOVER SYNTHESIS MOONLIGHT SOFT_BOILED MILK_DRINK FAKE_OUT QUICK_ATTACK EXTREME_SPEED MACH_PUNCH".split()
PHYSICAL_TYPES = set("NORMAL FIGHTING FLYING POISON GROUND ROCK BUG GHOST STEEL".split())

# Keep the gyms' themes and aces. Levels remain at or below the original ace.
BOSSES = {
    "LeaderBrock": ("ROCK", 14, "GEODUDE ONIX RHYHORN OMANYTE KABUTO NOSEPASS"),
    "LeaderMisty": ("WATER", 21, "POLIWAG PSYDUCK HORSEA STARYU GOLDEEN STARMIE"),
    "LeaderLtSurge": ("ELECTRIC", 24, "VOLTORB MAGNEMITE PIKACHU CHINCHOU ELECTRIKE RAICHU"),
    "LeaderErika": ("GRASS", 29, "TANGELA WEEPINBELL GLOOM PARASECT EXEGGCUTE VILEPLUME"),
    "LeaderKoga": ("POISON", 43, "KOFFING GOLBAT MUK ARIADOS VENOMOTH WEEZING"),
    "LeaderSabrina": ("PSYCHIC", 43, "MR_MIME KADABRA HYPNO SLOWBRO GIRAFARIG ALAKAZAM"),
    "LeaderBlaine": ("FIRE", 47, "NINETALES RAPIDASH MAGMAR CAMERUPT FLAREON ARCANINE"),
    "LeaderGiovanni": ("GROUND", 50, "DUGTRIO SANDSLASH NIDOQUEEN NIDOKING RHYHORN RHYDON"),
    "EliteFourLorelei": ("ICE", 54, "DEWGONG CLOYSTER JYNX PILOSWINE GLALIE LAPRAS"),
    "EliteFourBruno": ("FIGHTING", 56, "HITMONTOP HITMONCHAN HARIYAMA HERACROSS HITMONLEE MACHAMP"),
    "EliteFourAgatha": ("GHOST", 58, "GENGAR MISDREAVUS DUSCLOPS BANETTE SABLEYE HAUNTER"),
    "EliteFourLance": ("DRAGON", 60, "DRAGONAIR KINGDRA FLYGON ALTARIA SALAMENCE DRAGONITE"),
    "EliteFourLorelei2": ("ICE", 66, "DEWGONG CLOYSTER JYNX PILOSWINE GLALIE LAPRAS"),
    "EliteFourBruno2": ("FIGHTING", 68, "HITMONTOP HITMONCHAN HARIYAMA HERACROSS HITMONLEE MACHAMP"),
    "EliteFourAgatha2": ("GHOST", 70, "GENGAR MISDREAVUS DUSCLOPS BANETTE SABLEYE HAUNTER"),
    "EliteFourLance2": ("DRAGON", 72, "KINGDRA FLYGON ALTARIA SALAMENCE DRAGONAIR DRAGONITE"),
}

# Small companion pools preserve trainer identity when a second member is needed.
POOLS = {
    "BUG_CATCHER": "WEEDLE CATERPIE SPINARAK VENONAT",
    "FISHERMAN": "GOLDEEN POLIWAG HORSEA TENTACOOL",
    "SWIMMER": "HORSEA PSYDUCK POLIWAG TENTACOOL",
    "HIKER": "GEODUDE MACHOP ONIX SANDSHREW",
    "BLACK_BELT": "MACHOP MANKEY MAKUHITA MEDITITE",
    "BIRD_KEEPER": "PIDGEY SPEAROW DODUO NATU",
    "PSYCHIC": "DROWZEE EXEGGCUTE NATU SLOWPOKE",
    "CHANNELER": "GASTLY MISDREAVUS SHUPPET DUSKULL",
    "ENGINEER": "MAGNEMITE VOLTORB PIKACHU ELECTRIKE",
    "SCIENTIST": "MAGNEMITE VOLTORB KOFFING GRIMER",
    "ROCKET": "EKANS ZUBAT KOFFING RATTATA",
    "BURGLAR": "GROWLITHE VULPIX PONYTA KOFFING",
    "BIKER": "KOFFING GRIMER EKANS ZUBAT",
    "CUE_BALL": "MANKEY MACHOP KOFFING GRIMER",
    "BEAUTY": "VULPIX BELLSPROUT ODDISH MEOWTH",
    "LASS": "ODDISH NIDORAN_F JIGGLYPUFF PIDGEY",
    "PICNICKER": "ODDISH NIDORAN_F BELLSPROUT PIKACHU",
    "GAMER": "POLIWAG GROWLITHE BELLSPROUT VOLTORB",
    "JUGGLER": "DROWZEE MR_MIME VOLTORB KADABRA",
    "TAMER": "EKANS SANDSHREW NIDORINO GROWLITHE",
    "POKEMANIAC": "SLOWPOKE CUBONE RHYHORN LICKITUNG",
    "SAILOR": "MACHOP POLIWAG TENTACOOL HORSEA",
    "AROMA_LADY": "ODDISH BELLSPROUT HOPPIP EXEGGCUTE",
    "CRUSH": "MACHOP MANKEY MAKUHITA MEDITITE",
}


def read(path):
    return (GAME / path).read_text(encoding="utf-8")


def original(path):
    return subprocess.check_output(["git", "-C", str(GAME), "show", f"{BASE}:{path}"], encoding="utf-8")


def field(body, name):
    match = re.search(r"\." + name + r"\s*=\s*([^,\n]+)", body)
    return match[1].strip() if match else None


class Data:
    def __init__(self):
        self.species = {}
        for name, body in re.findall(r"\[SPECIES_(\w+)\]\s*=\s*\{\n(.*?)\n    \}", read("src/data/pokemon/species_info.h"), re.S):
            types = re.search(r"\.types = \{TYPE_(\w+), TYPE_(\w+)\}", body)
            if types:
                self.species[name] = {"types": set(types.groups()), "attack": int(field(body, "baseAttack")), "special": int(field(body, "baseSpAttack"))}
                abilities = re.search(r"\.abilities = \{(.*?)\}", body)
                self.species[name]["abilities"] = set(re.findall(r"ABILITY_(\w+)", abilities[1])) if abilities else set()
        sets = {}
        for name, body in re.findall(r"static const u16 (\w+)\[\] = \{(.*?)\n\};", read("src/data/pokemon/level_up_learnsets.h"), re.S):
            sets[name] = [(int(lvl), move) for lvl, move in re.findall(r"LEVEL_UP_MOVE\((\d+), MOVE_(\w+)\)", body)]
        self.learn = {name: sets[ptr] for name, ptr in re.findall(r"\[SPECIES_(\w+)\] = (\w+)", read("src/data/pokemon/level_up_learnset_pointers.h"))}
        self.tm = {}
        for name, body in re.findall(r"\[SPECIES_(\w+)\]\s*= TMHM_LEARNSET\((.*?)\),", read("src/data/pokemon/tmhm_learnsets.h"), re.S):
            self.tm[name] = set(re.findall(r"TMHM\((?:TM|HM)\d+_(\w+)\)", body))
        self.moves = {}
        for name, body in re.findall(r"\[MOVE_(\w+)\]\s*=\s*\{(.*?)\n    \}", read("src/data/battle_moves.h"), re.S):
            self.moves[name] = {"power": int(field(body, "power")), "type": field(body, "type").removeprefix("TYPE_"), "accuracy": int(field(body, "accuracy")), "target": field(body, "target")}

    def legal(self, species, level):
        if species == "SMEARGLE":
            # Sketch permits these ordinary moves; use a bounded support palette.
            return set("TACKLE FAKE_OUT ICY_WIND SPORE HELPING_HAND PROTECT SEISMIC_TOSS".split())
        natural = {move for lvl, move in self.learn[species] if lvl <= level}
        return natural | self.tm.get(species, set())

    def candidates(self, species, level):
        cap = 60 if level <= 15 else 80 if level <= 27 else 100 if level <= 44 else 120
        legal = self.legal(species, level)
        if level <= 15:
            natural = {m for lvl, m in self.learn[species] if lvl <= level}
            legal &= natural | {"ROCK_TOMB", "WATER_PULSE", "BULLET_SEED"}
        if level < 30:
            legal.discard("TOXIC")
        return {m for m in legal if m not in BANNED and m in self.moves
                and self.moves[m]["power"] <= cap
                and self.moves[m]["target"] != "MOVE_TARGET_FOES_AND_ALLY"}

    def attack_score(self, species, move):
        data, info = self.moves[move], self.species[species]
        stat = info["attack"] if data["type"] in PHYSICAL_TYPES else info["special"]
        return data["power"] * (data["accuracy"] or 100) / 100 * (1.5 if data["type"] in info["types"] else 1) * (stat + 80) / 160


def parse():
    parties_text = original("src/data/trainer_parties.h")
    actual_start = parties_text.index("// Start of actual trainer data")
    parties = {}
    for match in PARTY_RE.finditer(parties_text, actual_start):
        if ".species" not in match[3]:
            continue  # Unused one-line placeholders are not campaign teams.
        team = []
        for body in re.findall(r"\{(.*?)\}", match[3], re.S):
            species = field(body, "species")
            if species:
                team.append({"species": species.removeprefix("SPECIES_"), "level": int(field(body, "lvl")), "iv": int(field(body, "iv") or 0)})
        if not team:
            raise ValueError(f"Failed to parse {match[2]}")
        parties[match[2]] = team
    trainers_text = original("src/data/trainers.h")
    trainers = {}
    for match in TRAINER_RE.finditer(trainers_text):
        party = re.search(r"\.party = \w+\((\w+)\)", match[2])
        if party and party[1] in parties:
            trainers[party[1]] = {"id": match[1], "class": field(match[2], "trainerClass"), "name": field(match[2], "trainerName")}
    if set(parties) != set(trainers):
        raise ValueError(f"Unmapped parties: {set(parties) - set(trainers)}")
    return parties_text, trainers_text, parties, trainers


def companion(data, trainer, team, salt):
    pool = "NIDORAN_M PIDGEY RATTATA SPEAROW"
    for key, candidates in POOLS.items():
        if key in trainer["class"]:
            pool = candidates
            break
    choices = [s for s in pool.split() if s not in {m["species"] for m in team}]
    if not choices:
        choices = pool.split()
    species = choices[salt % len(choices)]
    level = max(m["level"] for m in team)
    # Use the matching evolutionary stage at later points rather than level-50 basics.
    stages = {"PIDGEY": (18, "PIDGEOTTO", 36, "PIDGEOT"), "RATTATA": (20, "RATICATE", 101, "RATICATE"),
              "SPEAROW": (20, "FEAROW", 101, "FEAROW"), "POLIWAG": (25, "POLIWHIRL", 101, "POLIWHIRL"),
              "GOLDEEN": (33, "SEAKING", 101, "SEAKING"), "HORSEA": (32, "SEADRA", 101, "SEADRA"),
              "GEODUDE": (25, "GRAVELER", 37, "GOLEM"), "MACHOP": (28, "MACHOKE", 37, "MACHAMP"),
              "KOFFING": (35, "WEEZING", 101, "WEEZING"), "GRIMER": (38, "MUK", 101, "MUK"),
              "EKANS": (22, "ARBOK", 101, "ARBOK"), "ZUBAT": (22, "GOLBAT", 101, "GOLBAT"),
              "GASTLY": (25, "HAUNTER", 37, "GENGAR"), "DROWZEE": (26, "HYPNO", 101, "HYPNO"),
              "MAGNEMITE": (30, "MAGNETON", 101, "MAGNETON"), "VOLTORB": (30, "ELECTRODE", 101, "ELECTRODE"),
              "TENTACOOL": (30, "TENTACRUEL", 101, "TENTACRUEL"), "ELECTRIKE": (26, "MANECTRIC", 101, "MANECTRIC"),
              "WEEDLE": (10, "BEEDRILL", 101, "BEEDRILL"), "CATERPIE": (10, "BUTTERFREE", 101, "BUTTERFREE")}
    if species in stages:
        a, first, b, second = stages[species]
        if level >= b: species = second
        elif level >= a: species = first
    return {"species": species, "level": level, "iv": 0}


def moveset(data, mon, support=None, boss=False, weather=None):
    species, level = mon["species"], mon["level"]
    legal = data.candidates(species, level)
    def score(move):
        value = data.attack_score(species, move)
        move_type = data.moves[move]["type"]
        if weather == "RAIN_DANCE":
            value *= 1.5 if move_type == "WATER" else 0.5 if move_type == "FIRE" else 1
            if move == "THUNDER": value *= 100 / 70
        if weather == "SUNNY_DAY":
            value *= 1.5 if move_type == "FIRE" else 0.5 if move_type == "WATER" else 1
        return value
    attacks = sorted((m for m in legal if data.moves[m]["power"] > 0), key=lambda m: (-score(m), m))
    selected = []
    if attacks:
        stab = [m for m in attacks if data.moves[m]["type"] in data.species[species]["types"]]
        selected.append((stab or attacks)[0])
    if support and support not in selected:
        selected.append(support)
    # Coverage precedes redundant attacks of the same type.
    for move in attacks:
        if move not in selected and not any(data.moves[m]["power"] and data.moves[m]["type"] == data.moves[move]["type"] for m in selected):
            selected.append(move)
        if len(selected) >= (3 if support else 2): break
    utilities = SECONDARY if level >= 25 or boss else ["FAKE_OUT", "QUICK_ATTACK"]
    for move in utilities:
        if len(selected) >= 4: break
        if move in legal and move not in selected: selected.append(move)
    for move in attacks + SUPPORT:
        if len(selected) >= 4: break
        if move in legal and move not in selected: selected.append(move)
    # Transform specialists retain a functional move instead of four empty slots.
    if not selected and "TRANSFORM" in legal: selected.append("TRANSFORM")
    return (selected + ["NONE"] * 4)[:4]


def generate(data, originals, trainers):
    result = {}
    for party, old_team in originals.items():
        if party in LAB:
            continue
        salt = int(hashlib.sha256(party.encode()).hexdigest()[:8], 16)
        key = party.removeprefix("sParty_")
        boss = trainers[party]["class"] in {"TRAINER_CLASS_LEADER", "TRAINER_CLASS_ELITE_FOUR", "TRAINER_CLASS_CHAMPION"}
        if key in BOSSES:
            theme, cap, roster = BOSSES[key]
            team = [{"species": s, "level": cap - (2 if i < 4 else 1 if i == 4 else 0), "iv": 180} for i, s in enumerate(roster.split())]
        else:
            theme = None
            team = [dict(m) for m in old_team]
        # Cocoon-only and splash-only slots cannot contribute to coordinated doubles.
        for i, mon in enumerate(team):
            if mon["species"] in {"METAPOD", "KAKUNA"}:
                mon["species"] = ("BUTTERFREE" if mon["level"] >= 10 else "CATERPIE") if mon["species"] == "METAPOD" else ("BEEDRILL" if mon["level"] >= 10 else "WEEDLE")
            if mon["species"] == "MAGIKARP":
                mon["species"] = "GYARADOS" if mon["level"] >= 25 and i == len(team) - 1 else ("POLIWAG" if i % 2 else "GOLDEEN")
        if len(team) < 2:
            team.append(companion(data, trainers[party], team, salt))
        # Place an actual team-support move on an opening member when legal.
        support_pair = None
        for move in SUPPORT:
            eligible = [i for i, m in enumerate(team) if move in data.candidates(m["species"], m["level"])]
            if eligible:
                support_pair = (eligible[salt % len(eligible)], move)
                break
        if support_pair is None:
            # Replace one unsupported/redundant slot rather than merely labelling
            # two identical attackers as a coordinated team.
            for attempt in range(16):
                candidate = companion(data, trainers[party], team, salt + attempt)
                candidate["level"] = team[-1]["level"]
                options = [m for m in SUPPORT if m in data.candidates(candidate["species"], candidate["level"])]
                if options:
                    team[-1] = candidate
                    support_pair = (len(team) - 1, options[0])
                    break
        weather = None
        if min(m["level"] for m in team) >= 25:
            types = [data.species[m["species"]]["types"] for m in team]
            if all("WATER" in t for t in types): weather = "RAIN_DANCE"
            elif all(t & {"FIRE", "GRASS"} for t in types): weather = "SUNNY_DAY"
            elif all("ICE" in t for t in types): weather = "HAIL"
            elif all(t & {"ROCK", "GROUND", "STEEL"} for t in types): weather = "SANDSTORM"
            if any(data.species[m["species"]]["abilities"] & {"CLOUD_NINE", "AIR_LOCK"} for m in team):
                weather = None
            if weather:
                eligible = [i for i, m in enumerate(team) if weather in data.candidates(m["species"], m["level"])]
                if eligible: support_pair = (eligible[salt % len(eligible)], weather)
                else: weather = None
        if support_pair:
            index, support = support_pair
            team[0], team[index] = team[index], team[0]
            strategy = f"{support}: suporte de abertura + pressão e cobertura do parceiro"
        else:
            support = None
            strategy = "pressão ofensiva complementar; sem suporte legal disponível nesta base/nível"
        for i, mon in enumerate(team):
            mon["moves"] = moveset(data, mon, support if i == 0 else None, boss, weather)
            mon["iv"] = max(mon["iv"], 80 if mon["level"] < 16 else 120 if mon["level"] < 30 else 180 if mon["level"] < 50 else 220)
            mon["item"] = "ORAN_BERRY" if mon["level"] < 20 else "SITRUS_BERRY" if mon["level"] < 45 else ("LUM_BERRY" if i == 0 else "LEFTOVERS" if i % 3 == 0 else "SITRUS_BERRY")
        result[party] = {**trainers[party], "theme": theme, "strategy": strategy, "team": team}
    return result


def validate(data, results):
    errors = []
    for party, entry in results.items():
        team = entry["team"]
        if not 2 <= len(team) <= 6: errors.append(f"{party}: invalid size")
        if entry["class"] == "TRAINER_CLASS_LEADER" and len(team) != 6: errors.append(f"{party}: leader needs six")
        for mon in team:
            if mon["species"] not in data.species: errors.append(f"{party}: invalid species")
            if entry["theme"] and entry["theme"] not in data.species[mon["species"]]["types"]: errors.append(f"{party}: off-theme")
            if not 1 <= mon["level"] <= 100: errors.append(f"{party}: invalid level")
            nonempty = [m for m in mon["moves"] if m != "NONE"]
            if len(set(nonempty)) != len(nonempty): errors.append(f"{party}: duplicate move")
            if not nonempty: errors.append(f"{party}: no moves")
            for move in nonempty:
                if move not in data.legal(mon["species"], mon["level"]): errors.append(f"{party}: illegal {mon['species']}/{move}")
                if data.moves[move]["target"] == "MOVE_TARGET_FOES_AND_ALLY": errors.append(f"{party}: unsafe friendly fire")
    if errors: raise ValueError("\n".join(errors))


def render_party(name, entry):
    rows = [f"// {entry['strategy']}", f"static const struct TrainerMonItemCustomMoves {name}[] = {{"]
    for mon in entry["team"]:
        rows.extend(["    {", f"        .iv = {mon['iv']},", f"        .lvl = {mon['level']},", f"        .species = SPECIES_{mon['species']},", f"        .heldItem = ITEM_{mon['item']},", "        .moves = {" + ", ".join("MOVE_" + m for m in mon["moves"]) + "},", "    },"])
    return "\n".join(rows + ["};"])


def tower_teams(data):
    text = original("src/trainer_tower_sets.c")
    records = []
    block_re = re.compile(r"(\.mons = \{\n)(.*?)(\n\t{3}\})", re.S)
    mon_re = re.compile(r"\{\n(.*?)\n\t{4}\}", re.S)

    def replace_block(match):
        before = text[:match.start()]
        names = re.findall(r'\.name = _\("([^"\n]*)"\)', before)
        floors = re.findall(r"static const struct TrainerTowerFloor (\w+) =", before)
        name = names[-1] if names else "unknown"
        floor = floors[-1]
        team = []

        def replace_mon(mon_match):
            body = mon_match[1]
            species = field(body, "species").removeprefix("SPECIES_")
            legal = data.candidates(species, 50)
            support = next((m for m in SUPPORT if m in legal), None)
            mon = {"species": species, "level": 50, "moves": [], "item": "SITRUS_BERRY"}
            mon["moves"] = moveset(data, mon, support, True)
            body = re.sub(r"\.moves = \{[^}]+\}", ".moves = {" + ", ".join("MOVE_" + m for m in mon["moves"]) + "}", body)
            body = re.sub(r"\.heldItem = ITEM_\w+", ".heldItem = ITEM_SITRUS_BERRY", body)
            team.append(mon)
            return "{\n" + body + "\n\t\t\t\t}"

        body = mon_re.sub(replace_mon, match[2])
        if len(team) != 6:
            raise ValueError(f"Tower parse failed: {floor}/{name}, {len(team)} slots")
        entry = {"class": "TRAINER_CLASS_TOWER", "theme": None, "team": team}
        validate(data, {floor + "/" + name: entry})
        records.append({"floor": floor, "name": name, "team": team})
        return match[1] + body + match[3]

    return block_re.sub(replace_block, text), records


def outputs():
    data = Data()
    parties_text, trainers_text, originals, trainers = parse()
    results = generate(data, originals, trainers)
    validate(data, results)
    tower_text, tower = tower_teams(data)
    parties_text = PARTY_RE.sub(lambda m: render_party(m[2], results[m[2]]) if m[2] in results else m[0], parties_text)

    def replace_trainer(match):
        body = match[2]
        party = re.search(r"\.party = \w+\((\w+)\)", body)
        if not party or party[1] not in results: return match[0]
        body = re.sub(r"\.doubleBattle = \w+", ".doubleBattle = TRUE", body)
        body = re.sub(r"\.aiFlags = [^,]+", ".aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_VIABILITY | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_DOUBLE_BATTLE | AI_SCRIPT_HP_AWARE", body)
        body = re.sub(r"\.party = \w+\((\w+)\)", r".party = ITEM_CUSTOM_MOVES(\1)", body)
        return f"    [{match[1]}] = {{{body}\n    }},"

    trainers_text = TRAINER_RE.sub(replace_trainer, trainers_text)
    report = ["# Revisão dos treinadores para duplas", "", f"{len(results)} equipes regulares revisadas; três variantes da primeira luta preservadas em singles. Também revisados {len(tower)} elencos internos da Torre de Treinadores.", "", "Base de golpes, tipos e regras: motor atual de geração III. Necessário revisar após a migração moderna. As equipes regulares abaixo não têm EVs/naturezas competitivos configurados nem comprovam decisões perfeitas da IA. Dados externos de e-Reader não fazem parte desta revisão. Na Torre, a seleção de golpes usa nível 50 como referência, e os adversários continuam escalando ao nível da equipe do jogador; os EVs, IVs e naturezas originais foram preservados.", "", "## Critérios", "", "- Duas a seis criaturas; líderes e Elite Four com seis.", "- Suporte na abertura quando disponível, golpes legais por nível/TM e cobertura de tipos.", "- Sem ataques que atinjam o aliado, OHKO ou spam de evasão.", "- Níveis regulares preservados; berries e IVs progressivos, sem lendários adicionados.", "- Líderes e Elite Four respeitam seu tipo de especialidade.", ""]
    no_support = []
    for name, entry in results.items():
        report.extend([f"## {entry['id']}", "", entry["strategy"] + ".", "", "| Pokémon | Nível | Item | Golpes |", "|---|---:|---|---|"])
        for m in entry["team"]:
            report.append(f"| {m['species']} | {m['level']} | {m['item']} | {', '.join(x for x in m['moves'] if x != 'NONE')} |")
        report.append("")
        if "sem suporte" in entry["strategy"]: no_support.append(name)
    for entry in tower:
        report.extend([f"## Torre: {entry['floor']} / {entry['name']}", "", "| Pokémon | Item | Golpes |", "|---|---|---|"])
        for mon in entry["team"]:
            report.append(f"| {mon['species']} | {mon['item']} | {', '.join(m for m in mon['moves'] if m != 'NONE')} |")
        report.append("")
    payload = {"base": BASE, "engine": "current-gen3", "single_exceptions": sorted(LAB), "teams": results, "tower": tower, "no_support": no_support}
    return {GAME / "src/data/trainer_parties.h": parties_text, GAME / "src/data/trainers.h": trainers_text,
            ROOT / "data/trainer_balance.json": json.dumps(payload, ensure_ascii=False, indent=2) + "\n",
            ROOT / "TRAINER_REVIEW.md": "\n".join(report), GAME / "src/trainer_tower_sets.c": tower_text}, results, no_support


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="Validate generated files without changing them")
    args = parser.parse_args()
    generated, results, no_support = outputs()
    for path, content in generated.items():
        if args.check:
            if not path.exists() or path.read_text(encoding="utf-8") != content:
                raise SystemExit(f"Out of date: {path}")
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8", newline="\n")
    print(f"PASS: {len(results)} teams; 3 lab singles; {len(no_support)} teams without legal support moves")


if __name__ == "__main__":
    main()
