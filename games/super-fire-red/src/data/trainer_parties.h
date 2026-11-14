// Trainer party data exists for all of the RS trainer classes, but
// are only filled with one of the following placeholder pokemon.
// The actual FRLG trainer party data starts after these.
#define DUMMY_TRAINER_MON           \
    {                               \
        .lvl = 5,                   \
        .species = SPECIES_EKANS,   \
    }

#define DUMMY_TRAINER_MON_IV        \
    {                               \
        .iv = 100,                  \
        .lvl = 5,                   \
        .species = SPECIES_EKANS,   \
    }

// Copy of Swimmer Male Finn's party
#define DUMMY_TRAINER_STARMIE       \
    {                               \
        .lvl = 38,                  \
        .species = SPECIES_STARMIE, \
    }

static const struct TrainerMonNoItemDefaultMoves sParty_AquaLeader[] = {DUMMY_TRAINER_MON};
// Region geography (TRAINER_JOHTO_ANTHONY in TRAINER_AQUA_GRUNT_M): HNS_modern TRAINER_ANTHONY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_AquaGruntM[] = {
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_GEODUDE,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_MACHOP,
    }
};
// Region geography (TRAINER_JOHTO_DANIEL in TRAINER_AQUA_GRUNT_F): HNS_modern TRAINER_DANIEL, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_AquaGruntF[] = {
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_ONIX,
    }
};
// Region geography (TRAINER_JOHTO_RUSSELL in TRAINER_RS_AROMA_LADY): HNS_modern TRAINER_RUSSELL, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSAromaLady[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_GEODUDE,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_CUBONE,
    }
};
// Region geography (TRAINER_JOHTO_BILL in TRAINER_RS_RUIN_MANIAC): HNS_modern TRAINER_BILL, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSRuinManiac[] = {
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_GROWLITHE,
    }
};
// Region geography (TRAINER_JOHTO_LARRY in TRAINER_INTERVIEWER): HNS_modern TRAINER_LARRY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_Interviewer[] = {
    {
    .iv = 0,
    .lvl = 70,
    .species = SPECIES_LARVITAR,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_RSTuberF[] = {DUMMY_TRAINER_STARMIE};
static const struct TrainerMonNoItemDefaultMoves sParty_RSTuberM[] = {DUMMY_TRAINER_STARMIE};
static const struct TrainerMonNoItemDefaultMoves sParty_RSCooltrainerM[] = {DUMMY_TRAINER_MON_IV};
static const struct TrainerMonNoItemDefaultMoves sParty_RSCooltrainerF[] = {DUMMY_TRAINER_MON_IV};
// Region geography (TRAINER_JOHTO_RAY in TRAINER_HEX_MANIAC): HNS_modern TRAINER_RAY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HexManiac[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_VULPIX,
    }
};
// Region geography (TRAINER_JOHTO_WAYNE in TRAINER_RS_LADY): HNS_modern TRAINER_WAYNE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSLady[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_PARAS,
    },
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_ODDISH,
    }
};
// Region geography (TRAINER_JOHTO_SAMUEL in TRAINER_RS_BEAUTY): HNS_modern TRAINER_SAMUEL, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSBeauty[] = {
    {
    .iv = 0,
    .lvl = 72,
    .species = SPECIES_TEDDIURSA,
    },
    {
    .iv = 0,
    .lvl = 70,
    .species = SPECIES_SANDSHREW,
    },
    {
    .iv = 0,
    .lvl = 72,
    .species = SPECIES_SPEAROW,
    }
};
// Region geography (TRAINER_JOHTO_IAN in TRAINER_RICH_BOY): HNS_modern TRAINER_IAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RichBoy[] = {
    {
    .iv = 0,
    .lvl = 72,
    .species = SPECIES_MANKEY,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_DIGLETT,
    }
};
// Region geography (TRAINER_JOHTO_TODD in TRAINER_RS_POKEMANIAC): HNS_modern TRAINER_TODD, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSPokemaniac[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_AZUMARILL,
    }
};
// Region geography (TRAINER_JOHTO_GINA in TRAINER_RS_SWIMMER_M): HNS_modern TRAINER_GINA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSSwimmerM[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_SKIPLOOM,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_BULBASAUR,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_RSBlackBelt[] = {DUMMY_TRAINER_MON_IV};
// Region geography (TRAINER_JOHTO_KEITH in TRAINER_GUITARIST): HNS_modern TRAINER_KEITH, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_Guitarist[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_GROWLITHE,
    }
};
// Region geography (TRAINER_JOHTO_BRANDON in TRAINER_KINDLER): HNS_modern TRAINER_BRANDON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_Kindler[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_SNUBBULL,
    }
};
// Region geography (TRAINER_JOHTO_IRENE in TRAINER_RS_CAMPER): HNS_modern TRAINER_IRENE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSCamper[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_GOLDEEN,
    },
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_SEAKING,
    }
};
// Region geography (TRAINER_JOHTO_JENN in TRAINER_BUG_MANIAC): HNS_modern TRAINER_JENN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_BugManiac[] = {
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_STARYU,
    },
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_STARMIE,
    }
};
// Region geography (TRAINER_JOHTO_KATE in TRAINER_RS_PSYCHIC_M): HNS_modern TRAINER_KATE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSPsychicM[] = {
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_SHELLDER,
    },
    {
    .iv = 0,
    .lvl = 88,
    .species = SPECIES_CLOYSTER,
    }
};
// Region geography (TRAINER_JOHTO_KIM in TRAINER_RS_PSYCHIC_F): HNS_modern TRAINER_KIM, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSPsychicF[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_VULPIX,
    }
};
// Region geography (TRAINER_JOHTO_BRYAN in TRAINER_RS_GENTLEMAN): HNS_modern TRAINER_BRYAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSGentleman[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_PIDGEY,
    },
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_PIDGEOTTO,
    },
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_SPEAROW,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_EliteFourSidney[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_EliteFourPhoebe[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderRoxanne[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderBrawly[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderTateLiza[] = {DUMMY_TRAINER_MON};
// Region geography (TRAINER_JOHTO_BROOKE in TRAINER_SCHOOL_KID_M): HNS_modern TRAINER_BROOKE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_SchoolKidM[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_PIKACHU,
    }
};
// Region geography (TRAINER_JOHTO_ELLIOT in TRAINER_SCHOOL_KID_F): HNS_modern TRAINER_ELLIOT, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_SchoolKidF[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_SANDSHREW,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_MARILL,
    }
};
// Region geography (TRAINER_JOHTO_IVAN in TRAINER_SR_AND_JR): HNS_modern TRAINER_IVAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_SrAndJr[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_EEVEE,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_DIGLETT,
    }
};
// Region geography (TRAINER_JOHTO_WALT in TRAINER_POKEFAN_M): HNS_modern TRAINER_WALT, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_PokefanM[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MAGMAR,
    },
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MAGMAR,
    }
};
// Region geography (TRAINER_JOHTO_IRWIN in TRAINER_POKEFAN_F): HNS_modern TRAINER_IRWIN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_PokefanF[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_VOLTORB,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_PINECO,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_VOLTORB,
    }
};
// Region geography (TRAINER_JOHTO_DIRK in TRAINER_EXPERT_M): HNS_modern TRAINER_DIRK, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_ExpertM[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_GROWLITHE,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_HOUNDOUR,
    }
};
// Region geography (TRAINER_JOHTO_ARNIE in TRAINER_EXPERT_F): HNS_modern TRAINER_ARNIE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_ExpertF[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_YANMA,
    },
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_VENONAT,
    }
};
// Region geography (TRAINER_JOHTO_MARK in TRAINER_RS_YOUNGSTER): HNS_modern TRAINER_MARK, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSYoungster[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_ABRA,
    },
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_ABRA,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_KADABRA,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_RSChampion[] = {DUMMY_TRAINER_MON};
// Region geography (TRAINER_JOHTO_ALAN in TRAINER_RS_FISHERMAN): HNS_modern TRAINER_ALAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSFisherman[] = {
    {
    .iv = 0,
    .lvl = 80,
    .species = SPECIES_TANGELA,
    },
    {
    .iv = 0,
    .lvl = 80,
    .species = SPECIES_GROWLITHE,
    }
};
// Region geography (TRAINER_HOENN_HALEY in TRAINER_CYCLING_TRIATHLETE_M): pokeemerald TRAINER_HALEY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_CyclingTriathleteM[] = {
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_LOTAD,
    },
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_SHROOMISH,
    }
};
// Region geography (TRAINER_HOENN_IVAN in TRAINER_CYCLING_TRIATHLETE_F): pokeemerald TRAINER_IVAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_CyclingTriathleteF[] = {
    {
    .iv = 0,
    .lvl = 65,
    .species = SPECIES_MAGIKARP,
    },
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_MAGIKARP,
    },
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_MAGIKARP,
    }
};
// Region geography (TRAINER_HOENN_GINA_AND_MIA in TRAINER_RUNNING_TRIATHLETE_M): pokeemerald TRAINER_GINA_AND_MIA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RunningTriathleteM[] = {
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_SEEDOT,
    },
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_LOTAD,
    }
};
// Region geography (TRAINER_HOENN_WINSTON in TRAINER_RUNNING_TRIATHLETE_F): pokeemerald TRAINER_WINSTON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RunningTriathleteF[] = {
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_ZIGZAGOON,
    }
};
// Region geography (TRAINER_HOENN_CINDY in TRAINER_SWIMMING_TRIATHLETE_M): pokeemerald TRAINER_CINDY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_SwimmingTriathleteM[] = {
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_ZIGZAGOON,
    }
};
// Region geography (TRAINER_HOENN_BILLY in TRAINER_SWIMMING_TRIATHLETE_F): pokeemerald TRAINER_BILLY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_SwimmingTriathleteF[] = {
    {
    .iv = 0,
    .lvl = 65,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_SEEDOT,
    }
};
// Region geography (TRAINER_HOENN_DARIAN in TRAINER_DRAGON_TAMER): pokeemerald TRAINER_DARIAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_DragonTamer[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_MAGIKARP,
    }
};
// Region geography (TRAINER_HOENN_LYLE in TRAINER_RS_BIRD_KEEPER): pokeemerald TRAINER_LYLE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSBirdKeeper[] = {
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_WURMPLE,
    },
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_WURMPLE,
    },
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_WURMPLE,
    },
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_WURMPLE,
    }
};
// Region geography (TRAINER_HOENN_JAMES in TRAINER_NINJA_BOY): pokeemerald TRAINER_JAMES, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_NinjaBoy[] = {
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_NINCADA,
    },
    {
    .iv = 0,
    .lvl = 66,
    .species = SPECIES_NINCADA,
    }
};
// Region geography (TRAINER_JOHTO_ANN_AND_ANNE in TRAINER_BATTLE_GIRL): HNS_modern TRAINER_ANN_AND_ANNE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_BattleGirl[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_CLEFAIRY,
    },
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_FURRET,
    }
};
// Region geography (TRAINER_JOHTO_GREG in TRAINER_PARASOL_LADY): HNS_modern TRAINER_GREG, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_ParasolLady[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_STANTLER,
    }
};
// Region geography (TRAINER_HOENN_JOEY in TRAINER_RS_SWIMMER_F): pokeemerald TRAINER_JOEY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSSwimmerF[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_MACHOP,
    }
};
// Region geography (TRAINER_HOENN_JOSE in TRAINER_RS_PICNICKER): pokeemerald TRAINER_JOSE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSPicnicker[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_WURMPLE,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_NINCADA,
    }
};
// Region geography (TRAINER_HOENN_CLARK in TRAINER_RS_TWINS): pokeemerald TRAINER_CLARK, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSTwins[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_GEODUDE,
    }
};
// Region geography (TRAINER_HOENN_JANICE in TRAINER_RS_SAILOR): pokeemerald TRAINER_JANICE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSSailor[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_MARILL,
    }
};
// Region geography (TRAINER_HOENN_KAREN in TRAINER_BOARDER_M): pokeemerald TRAINER_KAREN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_BoarderM[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_SHROOMISH,
    }
};
// Region geography (TRAINER_HOENN_JERRY in TRAINER_BOARDER_F): pokeemerald TRAINER_JERRY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_BoarderF[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_RALTS,
    }
};
// Region geography (TRAINER_HOENN_SARAH in TRAINER_COLLECTOR): pokeemerald TRAINER_SARAH, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_Collector[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_LOTAD,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_ZIGZAGOON,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_Wally[] = {DUMMY_TRAINER_MON};
// Region pilot: resetes12/HNS_modern sParty_Rival_Cyndaquil5, levels +40.
static const struct TrainerMonItemCustomMoves sParty_Brendan[] = {
    {
    .iv = 100,
    .lvl = 85,
    .species = SPECIES_URSARING,
    .heldItem = ITEM_CHESTO_BERRY,
    .moves = {MOVE_SLASH, MOVE_FAINT_ATTACK, MOVE_ROCK_SMASH, MOVE_REST},
    },
    {
    .iv = 100,
    .lvl = 85,
    .species = SPECIES_GOLBAT,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_TOXIC, MOVE_DOUBLE_TEAM, MOVE_CONFUSE_RAY, MOVE_WING_ATTACK},
    },
    {
    .iv = 100,
    .lvl = 86,
    .species = SPECIES_OCTILLERY,
    .heldItem = ITEM_QUICK_CLAW,
    .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_PSYBEAM, MOVE_HYPER_BEAM},
    },
    {
    .iv = 100,
    .lvl = 86,
    .species = SPECIES_VICTREEBEL,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_GROWTH},
    },
    {
    .iv = 100,
    .lvl = 87,
    .species = SPECIES_TYPHLOSION,
    .heldItem = ITEM_CHARCOAL,
    .moves = {MOVE_FLAMETHROWER, MOVE_QUICK_ATTACK, MOVE_IRON_TAIL, MOVE_THUNDER_PUNCH},
    },
    {
    .iv = 100,
    .lvl = 88,
    .species = SPECIES_TYRANITAR,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_FIRE_PUNCH},
    }
};
// Region pilot: resetes12/HNS_modern sParty_Rival_Totodile5, levels +40.
static const struct TrainerMonItemCustomMoves sParty_Brendan2[] = {
    {
    .iv = 100,
    .lvl = 85,
    .species = SPECIES_URSARING,
    .heldItem = ITEM_CHESTO_BERRY,
    .moves = {MOVE_SLASH, MOVE_FAINT_ATTACK, MOVE_ROCK_SMASH, MOVE_REST},
    },
    {
    .iv = 100,
    .lvl = 85,
    .species = SPECIES_GOLBAT,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_TOXIC, MOVE_DOUBLE_TEAM, MOVE_CONFUSE_RAY, MOVE_WING_ATTACK},
    },
    {
    .iv = 100,
    .lvl = 86,
    .species = SPECIES_VICTREEBEL,
    .heldItem = ITEM_QUICK_CLAW,
    .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_GROWTH},
    },
    {
    .iv = 100,
    .lvl = 86,
    .species = SPECIES_HOUNDOOM,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_FLAMETHROWER, MOVE_IRON_TAIL, MOVE_CRUNCH, MOVE_DOUBLE_TEAM},
    },
    {
    .iv = 100,
    .lvl = 87,
    .species = SPECIES_FERALIGATR,
    .heldItem = ITEM_MYSTIC_WATER,
    .moves = {MOVE_ICE_PUNCH, MOVE_SURF, MOVE_SLASH, MOVE_BITE},
    },
    {
    .iv = 100,
    .lvl = 88,
    .species = SPECIES_TYRANITAR,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_FIRE_PUNCH},
    }
};
// Region pilot: resetes12/HNS_modern sParty_Rival_Chikorita5, levels +40.
static const struct TrainerMonItemCustomMoves sParty_Brendan3[] = {
    {
    .iv = 100,
    .lvl = 85,
    .species = SPECIES_URSARING,
    .heldItem = ITEM_CHESTO_BERRY,
    .moves = {MOVE_SLASH, MOVE_FAINT_ATTACK, MOVE_ROCK_SMASH, MOVE_REST},
    },
    {
    .iv = 100,
    .lvl = 85,
    .species = SPECIES_GOLBAT,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_TOXIC, MOVE_DOUBLE_TEAM, MOVE_CONFUSE_RAY, MOVE_WING_ATTACK},
    },
    {
    .iv = 100,
    .lvl = 86,
    .species = SPECIES_OCTILLERY,
    .heldItem = ITEM_QUICK_CLAW,
    .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_PSYBEAM, MOVE_HYPER_BEAM},
    },
    {
    .iv = 100,
    .lvl = 86,
    .species = SPECIES_HOUNDOOM,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_FLAMETHROWER, MOVE_IRON_TAIL, MOVE_CRUNCH, MOVE_SUNNY_DAY},
    },
    {
    .iv = 100,
    .lvl = 87,
    .species = SPECIES_MEGANIUM,
    .heldItem = ITEM_MIRACLE_SEED,
    .moves = {MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_BODY_SLAM, MOVE_SYNTHESIS},
    },
    {
    .iv = 100,
    .lvl = 88,
    .species = SPECIES_TYRANITAR,
    .heldItem = ITEM_NONE,
    .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_FIRE_PUNCH},
    }
};
// Region pilot: pokeemerald sParty_MayLilycoveMudkip, levels +55.
static const struct TrainerMonNoItemDefaultMoves sParty_May[] = {
    {
    .iv = 150,
    .lvl = 86,
    .species = SPECIES_TROPIUS,
    },
    {
    .iv = 150,
    .lvl = 87,
    .species = SPECIES_SLUGMA,
    },
    {
    .iv = 150,
    .lvl = 87,
    .species = SPECIES_PELIPPER,
    },
    {
    .iv = 200,
    .lvl = 89,
    .species = SPECIES_GROVYLE,
    }
};
// Region pilot: pokeemerald sParty_MayLilycoveTreecko, levels +55.
static const struct TrainerMonNoItemDefaultMoves sParty_May2[] = {
    {
    .iv = 150,
    .lvl = 86,
    .species = SPECIES_TROPIUS,
    },
    {
    .iv = 150,
    .lvl = 87,
    .species = SPECIES_PELIPPER,
    },
    {
    .iv = 150,
    .lvl = 87,
    .species = SPECIES_LUDICOLO,
    },
    {
    .iv = 200,
    .lvl = 89,
    .species = SPECIES_COMBUSKEN,
    }
};
// Region pilot: pokeemerald sParty_MayLilycoveTorchic, levels +55.
static const struct TrainerMonNoItemDefaultMoves sParty_May3[] = {
    {
    .iv = 150,
    .lvl = 86,
    .species = SPECIES_TROPIUS,
    },
    {
    .iv = 150,
    .lvl = 87,
    .species = SPECIES_LUDICOLO,
    },
    {
    .iv = 150,
    .lvl = 87,
    .species = SPECIES_SLUGMA,
    },
    {
    .iv = 200,
    .lvl = 89,
    .species = SPECIES_MARSHTOMP,
    }
};
// Region geography (TRAINER_HOENN_DAWSON in TRAINER_RS_PKMN_BREEDER_M): pokeemerald TRAINER_DAWSON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSPkmnBreederM[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_POOCHYENA,
    }
};
// Region geography (TRAINER_HOENN_JOHNSON in TRAINER_RS_PKMN_BREEDER_F): pokeemerald TRAINER_JOHNSON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_RSPkmnBreederF[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_SHROOMISH,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_LOTAD,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_RSPkmnRangerM[] = {DUMMY_TRAINER_MON_IV};
static const struct TrainerMonNoItemDefaultMoves sParty_RSPkmnRangerF[] = {DUMMY_TRAINER_MON_IV};
static const struct TrainerMonNoItemDefaultMoves sParty_MagmaLeader[] = {DUMMY_TRAINER_MON};
// Region geography (TRAINER_HOENN_DEVAN in TRAINER_MAGMA_GRUNT_M): pokeemerald TRAINER_DEVAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_MagmaGruntM[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_GEODUDE,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_GEODUDE,
    }
};
// Region geography (TRAINER_HOENN_MIKE in TRAINER_MAMGA_GRUNT_F): pokeemerald TRAINER_MIKE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_MagmaGruntF[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_GEODUDE,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_GEODUDE,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_MACHOP,
    }
};
static const struct TrainerMonNoItemDefaultMoves sParty_RSLass[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_RSBugCatcher[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_RSHiker[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_RSYoungCouple[] = {DUMMY_TRAINER_MON, DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_OldCouple[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_RSSisAndBro[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_AquaAdminMatt[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_AquaAdminShelly[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_MagmaAdminTabitha[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_MagmaAdminCourtney[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderWattson[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderFlannery[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderNorman[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderWinona[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_LeaderWallace[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_EliteFourGlacia[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_EliteFourDrake[] = {DUMMY_TRAINER_MON};

// Start of actual trainer data
// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterBen[] = {
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_EKANS,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_ROCK_TOMB, MOVE_BITE, MOVE_WRAP},
    },
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_RATTATA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_QUICK_ATTACK, MOVE_BITE, MOVE_TACKLE, MOVE_TAIL_WHIP},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterCalvin[] = {
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_SPEAROW,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterJosh[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ZUBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_AIR_CUTTER},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterTimmy[] = {
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_EKANS,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_SANDSHREW,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterJoey[] = {
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_RATTATA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_TAIL_WHIP, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_SPEAROW,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_SECRET_POWER, MOVE_WING_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterDan[] = {
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_STRENGTH, MOVE_WATER_PULSE, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterChad[] = {
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_SANDSHREW,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_EKANS,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterTyler[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_LEER, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterEddie[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterDillon[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_POISON, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_AIR_CUTTER},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterYasu[] = {
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterDave[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_LEER, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterBen2[] = {
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ACID, MOVE_ROCK_TOMB, MOVE_CRUNCH, MOVE_STRENGTH},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherRick[] = {
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_POISON_STING, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherDoug[] = {
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_POISON_STING, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_FURY_ATTACK, MOVE_STRING_SHOT},
    },
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherSammy[] = {
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_POISON_STING, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_VENONAT,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_CONFUSION, MOVE_TACKLE, MOVE_SUPERSONIC, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherColton[] = {
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_TACKLE, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherGreg[] = {
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_GUST, MOVE_CONFUSION},
    },
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_FURY_CUTTER, MOVE_FURY_ATTACK},
    },
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherJames[] = {
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_GUST, MOVE_CONFUSION},
    },
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherKent[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_POISON_STING, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_BRICK_BREAK, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherRobby[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_TACKLE, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_SHADOW_BALL, MOVE_GIGA_DRAIN, MOVE_BUG_BITE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherCale[] = {
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_POISON_STING, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_GUST, MOVE_CONFUSION, MOVE_TACKLE},
    },
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_FURY_CUTTER, MOVE_FURY_ATTACK},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherKeigo[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_TACKLE, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherElijah[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AIR_SLASH, MOVE_STRING_SHOT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_BRICK_BREAK, MOVE_PROTECT, MOVE_VENOSHOCK},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcher2[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_STRING_SHOT, MOVE_SHADOW_BALL, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_VENONAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_GIGA_DRAIN, MOVE_SECRET_POWER, MOVE_THIEF},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherBrent[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_BRICK_BREAK, MOVE_PROTECT, MOVE_VENOSHOCK},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherConner[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_POISON_STING, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_VENONAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SECRET_POWER},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassJanice[] = {
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_NIDORAN_F,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_TAIL_WHIP, MOVE_WATER_PULSE, MOVE_SCRATCH},
    },
    {
        .iv = 80,
        .lvl = 12,
        .species = SPECIES_PIDGEY,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_TACKLE, MOVE_GUST, MOVE_NONE, MOVE_NONE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassSally[] = {
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_NIDORAN_F,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_TAIL_WHIP, MOVE_WATER_PULSE, MOVE_SCRATCH},
    },
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_RATTATA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_QUICK_ATTACK, MOVE_BITE, MOVE_TACKLE, MOVE_TAIL_WHIP},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassRobin[] = {
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassCrissy[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassMiriam[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_BELLSPROUT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_REFLECT, MOVE_FACADE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ODDISH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_ACID, MOVE_MEGA_DRAIN, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassIris[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_STRENGTH},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ODDISH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_ACID, MOVE_MEGA_DRAIN, MOVE_FACADE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassReli[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_TAIL_WHIP, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassAli[] = {
    {
        .iv = 80,
        .lvl = 15,
        .species = SPECIES_BELLSPROUT,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_VINE_WHIP, MOVE_SLEEP_POWDER, MOVE_WRAP, MOVE_BULLET_SEED},
    },
    {
        .iv = 80,
        .lvl = 15,
        .species = SPECIES_ODDISH,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ACID, MOVE_MEGA_DRAIN, MOVE_BULLET_SEED, MOVE_ABSORB},
    },
    {
        .iv = 80,
        .lvl = 15,
        .species = SPECIES_PIDGEY,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_QUICK_ATTACK, MOVE_GUST, MOVE_TACKLE, MOVE_NONE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Lass2[] = {
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_TAIL_WHIP, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// STUN_SPORE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassHaley[] = {
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_ODDISH,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_STUN_SPORE, MOVE_ACID, MOVE_MEGA_DRAIN},
    },
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_PIDGEY,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_ODDISH,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_ACID, MOVE_MEGA_DRAIN, MOVE_FACADE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassAnn[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TAIL_WHIP, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassDawn[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassPaige[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_THUNDERBOLT},
    },
};

// GROWL: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassAndrea[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_GROWL, MOVE_THUNDERBOLT, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassMegan[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_MEOWTH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassJulia[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassKay[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassLisa[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONBLAST},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorEdmond[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorTrevor[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_TENTACOOL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_WATERFALL, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorLeonard[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TAIL_WHIP, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorDuncan[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUBBLE_BEAM, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_TENTACOOL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorHuey[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_TENTACOOL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_STARYU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_BRINE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorDylan[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUBBLE_BEAM, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUBBLE_BEAM, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUBBLE_BEAM, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorPhillip[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_THUNDERBOLT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SailorDwayne[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperLiam[] = {
    {
        .iv = 80,
        .lvl = 13,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_TACKLE, MOVE_ROLLOUT, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_SANDSHREW,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_SCRATCH, MOVE_FURY_CUTTER, MOVE_ROLLOUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperShane[] = {
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_EKANS,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_RATTATA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperEthan[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_FACADE, MOVE_SECRET_POWER, MOVE_ACID},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperRicky[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_WARTORTLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_STRENGTH, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperJeff[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_SECRET_POWER, MOVE_PLUCK},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_Camper2[] = {DUMMY_TRAINER_MON};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperChris[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_CHARMELEON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_DRAGON_CLAW, MOVE_PROTECT, MOVE_FIRE_FANG},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperDrew[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_PROTECT, MOVE_CRUSH_CLAW},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ACID, MOVE_CRUNCH, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerDiana[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_GOLDEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_FACADE, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerNancy[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerIsabelle[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ODDISH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_ACID, MOVE_MEGA_DRAIN},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerKelsey[] = {
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_TAIL_WHIP, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerAlicia[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_REFLECT, MOVE_FACADE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_FACADE, MOVE_SECRET_POWER, MOVE_VINE_WHIP},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_FACADE, MOVE_SECRET_POWER, MOVE_ACID},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_FACADE, MOVE_SECRET_POWER, MOVE_ACID},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerCaitlin[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerHeidi[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerCarol[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerSofia[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerMartha[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SEED_BOMB},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerTina[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerHannah[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacMark[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_RUN, MOVE_ROCK_TOMB, MOVE_HAMMER_ARM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacHerman[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_CUBONE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ZEN_HEADBUTT, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacCooper[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacSteve[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_CHARMELEON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_ROCK_TOMB, MOVE_DRAGON_CLAW, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacWinston[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacDawson[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_LAPRAS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_HYDRO_PUMP, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacAshton[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_SLOWPOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdJovan[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_SPARK},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_SPARK, MOVE_CHARGE_BEAM},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdMiguel[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE, MOVE_ROCK_TOMB, MOVE_FACADE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_SPARK, MOVE_CHARGE_BEAM},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE, MOVE_SHADOW_BALL, MOVE_CLEAR_SMOG, MOVE_FACADE},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdAidan[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_CLEAR_SMOG},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_PROTECT, MOVE_SPARK},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_PROTECT, MOVE_SPARK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdGlenn[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FACADE, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FACADE, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdLeslie[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerd1[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_TRI_ATTACK, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_KOFFING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerd2[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_FACADE, MOVE_PROTECT, MOVE_SPARK},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_CLEAR_SMOG},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_FACADE, MOVE_PROTECT, MOVE_SPARK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerd3[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_SHOCK_WAVE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdErik[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SUNNY_DAY, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdAvery[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SUNNY_DAY, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdDerek[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdZac[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SUNNY_DAY, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerMarcos[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_SMACK_DOWN},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_SMACK_DOWN},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerFranklin[] = {
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 19,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_SMACK_DOWN},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerNob[] = {
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 16,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerWayne[] = {
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerAlan[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerBrice[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerClark[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerTrent[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerDudley[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerAllen[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_SANDSTORM, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerEric[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerLenny[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerOliver[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_SANDSTORM, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_ROCK_SLIDE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerLucas[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerJared[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerMalik[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerErnest[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerAlex[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerLao[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_Biker1[] = {DUMMY_TRAINER_MON};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerHideo[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_TOXIC, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerRuben[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerBilly[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerNikolas[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_LIGHT_SCREEN, MOVE_SWIFT, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_SHOCK_WAVE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerJaxon[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerWilliam[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerLukas[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerIsaac[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerGerald[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Burglar1[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Burglar2[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Burglar3[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_VULPIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_EXTRASENSORY, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FLAME_WHEEL},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BurglarQuinn[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BurglarRamon[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BurglarDusty[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BurglarArnie[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SUNNY_DAY, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Burglar4[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SUNNY_DAY, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BurglarSimon[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_TOXIC, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BurglarLewis[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_EngineerBaily[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_SHOCK_WAVE},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_EngineerBraxton[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_EngineerBernie[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_SPARK},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_FACADE, MOVE_SPARK, MOVE_THUNDER_SHOCK},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_FACADE, MOVE_SPARK, MOVE_THUNDER_SHOCK},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanDale[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_GOLDEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_TENTACOOL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_GOLDEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanBarny[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_STARYU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_BRINE},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_TENTACOOL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanNed[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_POLIWHIRL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_GOLDEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_GOLDEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanChip[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanHank[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanElliot[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUBBLE_BEAM, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_GOLDEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_POLIWHIRL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanRonald[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanClaude[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanWade[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_CRUNCH, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanNolan[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanAndrew[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_RAIN_DANCE, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleLuis[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_LEER, MOVE_FACADE, MOVE_WATER_PULSE},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_WATER_PULSE, MOVE_WATER_GUN},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleRichard[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleReece[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleMatthew[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleDouglas[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleDavid[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleTony[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_DRAGON_BREATH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleAxle[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_STARYU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_BRINE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleBarry[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleDean[] = {
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_HYDRO_PUMP, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleDarrin[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_DRAGON_BREATH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleSpencer[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_DRAGON_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleJack[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleJerome[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleRoland[] = {
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallKoji[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallLuke[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallCamron[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallRaul[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallIsaiah[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallZeek[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_THRASH, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallJamal[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallCorey[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallChase[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerHugo[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_POLIWAG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_HYPNOSIS, MOVE_FACADE, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_WATER_PULSE, MOVE_WATER_GUN},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerJasper[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_REFLECT, MOVE_FACADE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_FACADE, MOVE_SECRET_POWER, MOVE_ACID},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerDirk[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_SPARK},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_SPARK, MOVE_CHARGE_BEAM},
    },
};

// HELPING_HAND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerDarian[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAME_WHEEL, MOVE_HELPING_HAND, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_VULPIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_INCINERATE, MOVE_FACADE, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerStan[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_POLIWHIRL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_POLIWHIRL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_POLIWHIRL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_Gamer1[] = {DUMMY_TRAINER_MON};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerRich[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_VULPIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_EXTRASENSORY, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_CRUNCH, MOVE_PROTECT, MOVE_FIRE_FANG},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyBridget[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONBLAST},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONBLAST},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyTamia[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyLori[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_EXEGGCUTE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyLola[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautySheila[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleTiffany[] = {
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleNora[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleMelissa[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyGrace[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyOlivia[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyLauren[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleAnya[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleAlice[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleConnie[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleShirley[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_DRAGON_BREATH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicJohan[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MR_MIME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_DAZZLING_GLEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicTyron[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MR_MIME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_DAZZLING_GLEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicCameron[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicPreston[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RockerRandall[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_THUNDER_WAVE, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_PROTECT, MOVE_SPARK},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_VOLTORB,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHOCK_WAVE, MOVE_SWIFT, MOVE_PROTECT, MOVE_SPARK},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RockerLuca[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_LIGHT_SCREEN, MOVE_SWIFT, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_SHOCK_WAVE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerDalton[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MR_MIME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_DAZZLING_GLEAM, MOVE_PROTECT, MOVE_THUNDERBOLT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerNelson[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerKirk[] = {
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerShawn[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerGregory[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_ROCK_TOMB, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_MR_MIME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_DAZZLING_GLEAM, MOVE_PROTECT, MOVE_THUNDERBOLT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerEdward[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_MR_MIME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_DAZZLING_GLEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 64,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 64,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerKayden[] = {
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerNate[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerPhil[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_CRUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerEdgar[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_CRUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerJason[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerCole[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_TAUROS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_THRASH},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerVincent[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_LIGHT_SCREEN, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerJohn[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_TAUROS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_THRASH},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperSebastian[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperPerry[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperRobert[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperDonald[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_XATU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_FARFETCHD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_SECRET_POWER},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperBenny[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperEdwin[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FARFETCHD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// GROWL: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperChester[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_GROWL, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperWilton[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperRamiro[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_XATU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperJacob[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperRoger[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperReed[] = {
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperKeith[] = {
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_FARFETCHD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FACADE, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// GROWL: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperCarter[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_GROWL, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperMitch[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperBeck[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperMarlon[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltKoichi[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltMike[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltHideki[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltAaron[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HARIYAMA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_DETECT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltHitoshi[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltAtsushi[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltKiyo[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltTakashi[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltDaisuke[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_RivalOaksLabSquirtle[] = {
    {
        .iv = 0,
        .lvl = 5,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_RivalOaksLabBulbasaur[] = {
    {
        .iv = 0,
        .lvl = 5,
        .species = SPECIES_BULBASAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_RivalOaksLabCharmander[] = {
    {
        .iv = 0,
        .lvl = 5,
        .species = SPECIES_CHARMANDER,
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalRoute22EarlySquirtle[] = {
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_SQUIRTLE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_WATER_PULSE, MOVE_TAIL_WHIP, MOVE_BITE, MOVE_WATER_GUN},
    },
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_PIDGEY,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_QUICK_ATTACK, MOVE_GUST, MOVE_TACKLE, MOVE_NONE},
    },
};

// LEECH_SEED: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalRoute22EarlyBulbasaur[] = {
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_BULBASAUR,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_RAZOR_LEAF, MOVE_LEECH_SEED, MOVE_TACKLE, MOVE_VINE_WHIP},
    },
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_PIDGEY,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_QUICK_ATTACK, MOVE_GUST, MOVE_TACKLE, MOVE_NONE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalRoute22EarlyCharmander[] = {
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_CHARMANDER,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_EMBER, MOVE_ROCK_TOMB, MOVE_DRAGON_BREATH, MOVE_SCRATCH},
    },
    {
        .iv = 80,
        .lvl = 14,
        .species = SPECIES_PIDGEY,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_QUICK_ATTACK, MOVE_GUST, MOVE_TACKLE, MOVE_NONE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalCeruleanSquirtle[] = {
    {
        .iv = 120,
        .lvl = 24,
        .species = SPECIES_KADABRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CONFUSION, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PSYCHO_CUT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_WARTORTLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_STRENGTH, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalCeruleanBulbasaur[] = {
    {
        .iv = 120,
        .lvl = 24,
        .species = SPECIES_KADABRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CONFUSION, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PSYCHO_CUT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_IVYSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_STRENGTH, MOVE_PROTECT, MOVE_SEED_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalCeruleanCharmander[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_CHARMELEON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_FANG, MOVE_ROCK_TOMB, MOVE_DRAGON_CLAW, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 24,
        .species = SPECIES_KADABRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CONFUSION, MOVE_SHADOW_BALL, MOVE_PSYCHO_CUT, MOVE_SHOCK_WAVE},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistTed[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_LIGHT_SCREEN, MOVE_SWIFT, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistConnor[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistJerry[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistJose[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_LIGHT_SCREEN, MOVE_SWIFT, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistRodney[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistBeau[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistTaylor[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistJoshua[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistParker[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistEd[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistTravis[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistBraydon[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistIvan[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BossGiovanni[] = {
    {
        .iv = 250,
        .lvl = 44,
        .species = SPECIES_KANGASKHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 250,
        .lvl = 39,
        .species = SPECIES_RHYHORN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_RUN, MOVE_ROCK_TOMB, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 250,
        .lvl = 40,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BossGiovanni2[] = {
    {
        .iv = 250,
        .lvl = 54,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_ROCK_TOMB, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 250,
        .lvl = 53,
        .species = SPECIES_KANGASKHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 250,
        .lvl = 54,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_HAMMER_ARM},
    },
    {
        .iv = 250,
        .lvl = 58,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
};

// Gligar abre com imunidade a Earthquake de Donphan; Flygon mantÃ©m a opÃ§Ã£o segura e Nidoqueen oferece Helping Hand/cobertura.
static const struct TrainerMonItemCustomMoves sParty_LeaderGiovanni[] = {
    {
        .iv = 255,
        .lvl = 74,
        .species = SPECIES_GLIGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_AERIAL_ACE, MOVE_SCREECH, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 74,
        .species = SPECIES_DONPHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_BODY_SLAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 74,
        .species = SPECIES_FLYGON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTHQUAKE, MOVE_DRAGON_CLAW, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 74,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HELPING_HAND, MOVE_ICE_BEAM, MOVE_THUNDERBOLT, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 75,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BONEMERANG, MOVE_ROCK_SLIDE, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 76,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_MEGAHORN, MOVE_PROTECT},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ZUBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_AIR_CUTTER},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt2[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_SANDSHREW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ZUBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_SHADOW_BALL, MOVE_AIR_CUTTER, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt3[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_EKANS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ACID, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_ZUBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_SHADOW_BALL, MOVE_AIR_CUTTER, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt4[] = {
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_SANDSHREW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 20,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt5[] = {
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_DROWZEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYBEAM, MOVE_SHADOW_BALL, MOVE_BRICK_BREAK, MOVE_CONFUSION},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt6[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ACID, MOVE_ROCK_TOMB, MOVE_CRUNCH, MOVE_STRENGTH},
    },
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AERIAL_ACE, MOVE_SHADOW_BALL, MOVE_AIR_CUTTER, MOVE_FACADE},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt7[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_POISON, MOVE_HYPNOSIS, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt8[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt9[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt10[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt11[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt12[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt13[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt14[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt15[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt16[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_CRUNCH},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt17[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_CRUNCH},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt18[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt19[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt20[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt21[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt22[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt23[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_THRASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt24[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt25[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt26[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt27[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_THRASH, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt28[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt29[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt30[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt31[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt32[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt33[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_SANDSTORM, MOVE_THRASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_THRASH, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt34[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt35[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt36[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt37[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BELCH},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt38[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt39[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt40[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt41[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_THRASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_THRASH, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerSamuel[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_SANDSTORM, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerGeorge[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_HYDRO_PUMP, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerColby[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CRABHAMMER, MOVE_HAMMER_ARM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerPaul[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CRABHAMMER, MOVE_ROCK_TOMB, MOVE_HAMMER_ARM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_HYDRO_PUMP, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerRolando[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_ROCK_TOMB, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerGilbert[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_TAUROS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_THRASH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerOwen[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerBerke[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_ICY_WIND, MOVE_AQUA_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CRABHAMMER, MOVE_HAMMER_ARM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_HYDRO_PUMP, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerYuji[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_SANDSTORM, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerWarren[] = {
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_THRASH},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_MEGAHORN},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
    {
        .iv = 220,
        .lvl = 72,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_THRASH},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerMary[] = {
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONBLAST},
    },
    {
        .iv = 180,
        .lvl = 39,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerCaroline[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_SUNNY_DAY, MOVE_ENERGY_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_X_SCISSOR, MOVE_ENERGY_BALL, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerAlexa[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_ICY_WIND, MOVE_AQUA_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_SPIT_UP},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CHANSEY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SOFT_BOILED},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerShannon[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_POISON_JAB, MOVE_STRING_SHOT, MOVE_BUG_BITE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_PSYCHIC},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_ENERGY_BALL, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_VENOMOTH,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_BUG_BUZZ, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_LEECH_LIFE},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_VENOMOTH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_LEECH_LIFE},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerNaomi[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_HYPNOSIS, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerBrooke[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_TANGELA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ENERGY_BALL, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerAustina[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_HAMMER_ARM},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerJulie[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// Helping Hand + Surf/Ice Beam na abertura; quatro tipos Gelo, sono e controle de velocidade; Tyranitar como alternativa de pressão por areia
static const struct TrainerMonItemCustomMoves sParty_EliteFourLorelei[] = {
    {
        .iv = 255,
        .lvl = 78,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HELPING_HAND, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_DETECT},
    },
    {
        .iv = 255,
        .lvl = 78,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_SPIKES, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 78,
        .species = SPECIES_PILOSWINE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_MUD_SHOT, MOVE_ROCK_SLIDE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 78,
        .species = SPECIES_JYNX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_PSYCHIC, MOVE_LOVELY_KISS, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 79,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 80,
        .species = SPECIES_LAPRAS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDERBOLT, MOVE_PROTECT},
    },
};

// Hariyama + Kyogre na abertura com Fake Out; Ludicolo aproveita chuva, Slowbro paralisa; Groudon oferece modo de sol e cobertura separado, sem Trick Room
static const struct TrainerMonItemCustomMoves sParty_EliteFourBruno[] = {
    {
        .iv = 255,
        .lvl = 81,
        .species = SPECIES_HARIYAMA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 81,
        .species = SPECIES_KYOGRE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 81,
        .species = SPECIES_LUDICOLO,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_GIGA_DRAIN, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 81,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SURF, MOVE_THUNDER_WAVE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 82,
        .species = SPECIES_GROUDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MUD_SHOT, MOVE_ROCK_SLIDE, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 83,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_BODY_SLAM, MOVE_PROTECT},
    },
};

// Mawile + Gengar: Intimidate e Fake Tears apoiam dano especial; três Fantasmas, queimadura/paralisia, cobertura elétrica/água e Tyranitar como alternativa de areia
static const struct TrainerMonItemCustomMoves sParty_EliteFourAgatha[] = {
    {
        .iv = 255,
        .lvl = 84,
        .species = SPECIES_MAWILE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FAKE_TEARS, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 84,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SHADOW_BALL, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 84,
        .species = SPECIES_DUSCLOPS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_NIGHT_SHADE, MOVE_SHADOW_BALL, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 84,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_PAIN_SPLIT},
    },
    {
        .iv = 255,
        .lvl = 85,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_TAUNT, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 86,
        .species = SPECIES_LANTURN,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_THUNDERBOLT, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
};

// Hitmontop + Kyogre: Intimidate e Helping Hand; Claydol usa telas, Raichu paralisa; Rayquaza fecha com dano misto e Air Lock, que também suspende a chuva aliada
static const struct TrainerMonItemCustomMoves sParty_EliteFourLance[] = {
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HELPING_HAND, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_DETECT},
    },
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_KYOGRE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SHADOW_BALL, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_CLAYDOL,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_REFLECT, MOVE_LIGHT_SCREEN, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_LIGHT_SCREEN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 89,
        .species = SPECIES_RAYQUAZA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRAGON_CLAW, MOVE_FLAMETHROWER, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
};

// Thunder Wave e Rock Tomb controlam velocidade; Omanyte pressiona Ground/Rock e Kabuto cobre Water/Ground com Absorb.
static const struct TrainerMonItemCustomMoves sParty_LeaderBrock[] = {
    {
        .iv = 255,
        .lvl = 16,
        .species = SPECIES_NOSEPASS,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_THUNDER_WAVE, MOVE_ROCK_TOMB, MOVE_TACKLE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 16,
        .species = SPECIES_OMANYTE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_WATER_PULSE, MOVE_ROCK_TOMB, MOVE_PROTECT, MOVE_WITHDRAW},
    },
    {
        .iv = 255,
        .lvl = 16,
        .species = SPECIES_GEODUDE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_THROW, MOVE_ROCK_TOMB, MOVE_TACKLE, MOVE_DEFENSE_CURL},
    },
    {
        .iv = 255,
        .lvl = 16,
        .species = SPECIES_KABUTO,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ABSORB, MOVE_ROCK_TOMB, MOVE_SCRATCH, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 17,
        .species = SPECIES_RHYHORN,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_PROTECT, MOVE_HORN_ATTACK, MOVE_TAIL_WHIP},
    },
    {
        .iv = 255,
        .lvl = 18,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_ROCK_TOMB, MOVE_TACKLE, MOVE_PROTECT},
    },
};

// Poliwhirl abre com chuva ou Hypnosis; Fake Out de Lombre protege a preparaÃ§Ã£o e Swift Swim aproveita a chuva.
static const struct TrainerMonItemCustomMoves sParty_LeaderMisty[] = {
    {
        .iv = 255,
        .lvl = 26,
        .species = SPECIES_POLIWHIRL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_RAIN_DANCE, MOVE_WATER_PULSE, MOVE_HYPNOSIS, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 26,
        .species = SPECIES_LOMBRE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_RAZOR_LEAF, MOVE_WATER_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 26,
        .species = SPECIES_HORSEA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATER_PULSE, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAIN_DANCE},
    },
    {
        .iv = 255,
        .lvl = 26,
        .species = SPECIES_PSYDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATER_PULSE, MOVE_CONFUSION, MOVE_LIGHT_SCREEN, MOVE_HYPNOSIS},
    },
    {
        .iv = 255,
        .lvl = 27,
        .species = SPECIES_STARYU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATER_PULSE, MOVE_THUNDER_WAVE, MOVE_LIGHT_SCREEN, MOVE_SWIFT},
    },
    {
        .iv = 255,
        .lvl = 28,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATER_PULSE, MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_RECOVER},
    },
};

// Electrode prepara chuva e Lanturn usa Surf/Thunder; Volt Absorb, telas e cobertura reduzem a dependÃªncia de um Ãºnico tipo ofensivo.
static const struct TrainerMonItemCustomMoves sParty_LeaderLtSurge[] = {
    {
        .iv = 255,
        .lvl = 34,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_RAIN_DANCE, MOVE_THUNDER, MOVE_TAUNT, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = 255,
        .lvl = 34,
        .species = SPECIES_LANTURN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SURF, MOVE_THUNDER, MOVE_ICE_BEAM, MOVE_THUNDER_WAVE},
    },
    {
        .iv = 255,
        .lvl = 34,
        .species = SPECIES_MANECTRIC,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_BITE, MOVE_ROAR, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 34,
        .species = SPECIES_ELECTABUZZ,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_PSYCHIC, MOVE_LIGHT_SCREEN, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 255,
        .lvl = 35,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_REFLECT, MOVE_THUNDER_WAVE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 36,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_BRICK_BREAK, MOVE_THUNDER_WAVE, MOVE_QUICK_ATTACK},
    },
};

// Tangela prepara sol ou paralisia e Shiftry usa Fake Out; Chlorophyll, Solar Beam e recuperaÃ§Ã£o aproveitam o clima.
static const struct TrainerMonItemCustomMoves sParty_LeaderErika[] = {
    {
        .iv = 255,
        .lvl = 42,
        .species = SPECIES_TANGELA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SUNNY_DAY, MOVE_STUN_SPORE, MOVE_GIGA_DRAIN, MOVE_REFLECT},
    },
    {
        .iv = 255,
        .lvl = 42,
        .species = SPECIES_SHIFTRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_THIEF, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 42,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_RAZOR_LEAF, MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 42,
        .species = SPECIES_BELLOSSOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SUNNY_DAY, MOVE_SOLAR_BEAM, MOVE_MOONLIGHT, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 43,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SOLAR_BEAM, MOVE_REFLECT, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 44,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_MOONLIGHT},
    },
};

// Nidoqueen usa Earthquake ao lado de Gengar imune; Helping Hand, sono, queimadura e trÃªs parceiros imunes a Ground permitem alternar funÃ§Ãµes.
static const struct TrainerMonItemCustomMoves sParty_LeaderKoga[] = {
    {
        .iv = 255,
        .lvl = 50,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_ICE_BEAM, MOVE_HELPING_HAND},
    },
    {
        .iv = 255,
        .lvl = 50,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_PSYCHIC, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 50,
        .species = SPECIES_CROBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAUNT, MOVE_SLUDGE_BOMB, MOVE_AERIAL_ACE, MOVE_BITE},
    },
    {
        .iv = 255,
        .lvl = 50,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 51,
        .species = SPECIES_VENOMOTH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLEEP_POWDER, MOVE_PSYCHIC, MOVE_GIGA_DRAIN, MOVE_STUN_SPORE},
    },
    {
        .iv = 255,
        .lvl = 52,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_THUNDERBOLT, MOVE_FLAMETHROWER, MOVE_WILL_O_WISP},
    },
};

// Ninetales prepara sol para Heat Wave de Torkoal/Arcanine; Helping Hand de Flareon amplia a pressÃ£o em ambos os adversÃ¡rios.
static const struct TrainerMonItemCustomMoves sParty_LeaderBlaine[] = {
    {
        .iv = 255,
        .lvl = 66,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SUNNY_DAY, MOVE_FLAMETHROWER, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 66,
        .species = SPECIES_TORKOAL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HEAT_WAVE, MOVE_BODY_SLAM, MOVE_AMNESIA, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 66,
        .species = SPECIES_FLAREON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HELPING_HAND, MOVE_FLAMETHROWER, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 255,
        .lvl = 66,
        .species = SPECIES_MAGMAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_PSYCHIC, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 67,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_DOUBLE_KICK, MOVE_HYPNOSIS, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 68,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HEAT_WAVE, MOVE_EXTREME_SPEED, MOVE_CRUNCH, MOVE_HELPING_HAND},
    },
};

// Fake Out e telas de Mr. Mime apoiam Espeon; Helping Hand combina com ataques especiais e Surf de Slowbro.
static const struct TrainerMonItemCustomMoves sParty_LeaderSabrina[] = {
    {
        .iv = 255,
        .lvl = 58,
        .species = SPECIES_MR_MIME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_PSYCHIC, MOVE_REFLECT, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = 255,
        .lvl = 58,
        .species = SPECIES_ESPEON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HELPING_HAND, MOVE_PSYCHIC, MOVE_BITE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 58,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_THUNDER_WAVE, MOVE_LIGHT_SCREEN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 58,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SURF, MOVE_PSYCHIC, MOVE_YAWN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 59,
        .species = SPECIES_GIRAFARIG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 60,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_CALM_MIND, MOVE_RECOVER, MOVE_SHOCK_WAVE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanThomas[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FIRE_FANG},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanArthur[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TAIL_WHIP, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ICE_BEAM},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanTucker[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_BODY_SLAM},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanNorton[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_REFLECT, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// HELPING_HAND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanWalter[] = {
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAME_WHEEL, MOVE_HELPING_HAND, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 22,
        .species = SPECIES_PONYTA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAME_WHEEL, MOVE_STRENGTH, MOVE_FACADE, MOVE_SECRET_POWER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalSsAnneSquirtle[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_KADABRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 33,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_WARTORTLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalSsAnneBulbasaur[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_KADABRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 33,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_SEED_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalSsAnneCharmander[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_CHARMELEON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_ROCK_TOMB, MOVE_DRAGON_CLAW, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_KADABRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_CONFUSION},
    },
    {
        .iv = 180,
        .lvl = 33,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalPokemonTowerSquirtle[] = {
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalPokemonTowerBulbasaur[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_CRUNCH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalPokemonTowerCharmander[] = {
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_ROCK_TOMB, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 46,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_CRUNCH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalSilphSquirtle[] = {
    {
        .iv = 220,
        .lvl = 58,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalSilphBulbasaur[] = {
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 58,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalSilphCharmander[] = {
    {
        .iv = 220,
        .lvl = 58,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_ROCK_TOMB, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalRoute22LateSquirtle[] = {
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 250,
        .lvl = 78,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalRoute22LateBulbasaur[] = {
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 250,
        .lvl = 78,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RivalRoute22LateCharmander[] = {
    {
        .iv = 250,
        .lvl = 78,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_ROCK_TOMB, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_MEGAHORN},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 220,
        .lvl = 73,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChampionFirstSquirtle[] = {
    {
        .iv = 255,
        .lvl = 92,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_MEGAHORN},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 255,
        .lvl = 90,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChampionFirstBulbasaur[] = {
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 255,
        .lvl = 90,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 255,
        .lvl = 92,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChampionFirstCharmander[] = {
    {
        .iv = 255,
        .lvl = 92,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_ROCK_TOMB, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 87,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_MEGAHORN},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 255,
        .lvl = 90,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 255,
        .lvl = 88,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerPatricia[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerCarly[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerHope[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_BANETTE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerPaula[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_DUSCLOPS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerLaurel[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerJody[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_DUSCLOPS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerTammy[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerRuth[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_DUSCLOPS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerKarina[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerJanae[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerAngelica[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerEmilia[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerJennifer[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler1[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler2[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler3[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_DUSKULL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_PSYCHIC},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler4[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_SHUPPET,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler5[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_DUSKULL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_PSYCHIC},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler6[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler7[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_DUSKULL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_Channeler8[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_SHUPPET,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerAmanda[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerStacy[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_BANETTE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PHANTOM_FORCE, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChannelerTasha[] = {
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_HEX},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerJeremy[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerAlma[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerSusie[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerValerie[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_PSYCHIC},
    },
};

// SCREECH: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerGwen[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_SCREECH, MOVE_THUNDERBOLT, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerVirgil[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperFlint[] = {
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_EKANS,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_FACADE},
    },
    {
        .iv = 120,
        .lvl = 18,
        .species = SPECIES_RATTATA,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_FACADE, MOVE_SHADOW_BALL, MOVE_QUICK_ATTACK, MOVE_SECRET_POWER},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerMissy[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerIrene[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerDana[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerAriana[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_REFLECT, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerLeah[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_WEEPINBELL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_REFLECT, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperJustin[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerYazmin[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_TANGELA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ENERGY_BALL, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_RAZOR_LEAF},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerKindra[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SUNNY_DAY, MOVE_GIGA_DRAIN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONBLAST},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_PROTECT, MOVE_MOONBLAST},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerBecky[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerCelia[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_REFLECT, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanBrooks[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_BODY_SLAM},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanLamar[] = {
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_PONYTA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FIRE_FANG},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TwinsEliAnne[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_BODY_SLAM},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CoolCoupleRayTyra[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungCoupleGiaJes[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ICE_BEAM},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TwinsKiriJan[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_HEAT_WAVE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinRonMya[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungCoupleLeaJed[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FLAME_WHEEL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SisAndBroLiaLuc[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SisAndBroLilIan[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_BugCatcher3[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_BugCatcher4[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_BugCatcher5[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_BugCatcher6[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_BugCatcher7[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_BugCatcher8[] = {DUMMY_TRAINER_MON};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterBen3[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterBen4[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterChad2[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_ROCK_TOMB, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ACID, MOVE_CRUNCH, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassReli2[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_STING, MOVE_TAIL_WHIP, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassReli3[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterTimmy2[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ACID, MOVE_ROCK_TOMB, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_POISON, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_AIR_CUTTER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterTimmy3[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterTimmy4[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterChad3[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_CRUNCH},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassJanice2[] = {
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassJanice3[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterChad4[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_BELCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerFranklin2[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_GRAVELER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_PkmnProfProfOak[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_PlayerBrendan[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_PlayerMay[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_PlayerRed[] = {DUMMY_TRAINER_MON};
static const struct TrainerMonNoItemDefaultMoves sParty_PlayerLeaf[] = {DUMMY_TRAINER_MON};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt42[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HOUNDOOM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SUNNY_DAY, MOVE_FOUL_PLAY, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HOUNDOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_FOUL_PLAY, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicJaclyn[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_XATU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlSharon[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_THRASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_THRASH, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TuberAmira[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLEEP_POWDER, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnBreederAlize[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_AZUMARILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerNicolas[] = {
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerMadeline[] = {
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_AromaLadyNikki[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacStanly[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_SANDSTORM, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LadyJacki[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_JUMPLUFF,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SUNNY_DAY, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_JUMPLUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PainterDaisy[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_SMEARGLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_ICY_WIND, MOVE_SEISMIC_TOSS, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_SPIT_UP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerGoon[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerGoon2[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerGoon3[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_Biker2[] = {DUMMY_TRAINER_MON};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherAnthony[] = {
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_TACKLE, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_TACKLE, MOVE_STRING_SHOT, MOVE_NONE},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherCharlie[] = {
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_CATERPIE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_STRING_SHOT, MOVE_TACKLE, MOVE_NONE},
    },
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_GUST, MOVE_CONFUSION, MOVE_TACKLE},
    },
    {
        .iv = 80,
        .lvl = 10,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_ORAN_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_GUST, MOVE_CONFUSION, MOVE_TACKLE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TwinsEliAnne2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_CLEFAIRY,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DISARMING_VOICE, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_HYPER_VOICE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterJohnson[] = {
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_SPIT_UP, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 61,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerRicardo[] = {
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_KOFFING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_KOFFING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FLAMETHROWER, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerJaren[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_GRIMER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_FACADE, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt43[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_SANDSTORM, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_THRASH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt44[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt45[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt46[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_CROSS_POISON},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt47[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt48[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketAdmin[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BELCH},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// HYPNOSIS: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketAdmin2[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HOUNDOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_FOUL_PLAY, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ScientistGideon[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PORYGON2,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TRI_ATTACK, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleAmara[] = {
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleMaria[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_DRAGON_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleAbigail[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_LIGHT_SCREEN, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleFinn[] = {
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_HYDRO_PUMP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleGarrett[] = {
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_SHELLDER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanTommy[] = {
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlTanya[] = {
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltShea[] = {
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltHugh[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperBryce[] = {
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerClaire[] = {
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 48,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinMikKia[] = {
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_THRASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_AromaLadyViolet[] = {
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 180,
        .lvl = 49,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TuberAlexis[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_RAZOR_SHELL, MOVE_HAMMER_ARM, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_RAZOR_SHELL, MOVE_HAMMER_ARM, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TwinsJoyMeg[] = {
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_REFLECT, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 50,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleTisha[] = {
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_RAZOR_SHELL, MOVE_ROCK_TOMB, MOVE_HAMMER_ARM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 51,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PainterCelina[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_SMEARGLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_ICY_WIND, MOVE_SEISMIC_TOSS, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PainterRayna[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_SMEARGLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_ICY_WIND, MOVE_SEISMIC_TOSS, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LadyGillian[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_AMPHAROS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_DRAGON_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_AMPHAROS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_AMPHAROS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterDestin[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleToby[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt49[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BELCH},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt50[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_TOXIC, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TeamRocketGrunt51[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_SPIT_UP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperMilo[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperChaz[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperHarold[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_NOCTOWL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_UPROAR, MOVE_REFLECT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_NOCTOWL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_UPROAR, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_MOONBLAST},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanTylor[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_QWILFISH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_RAIN_DANCE, MOVE_POISON_JAB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_QWILFISH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_POISON_JAB, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleMymo[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_KINGLER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CRABHAMMER, MOVE_HAMMER_ARM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleNicole[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_LIGHT_SCREEN, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_AZUMARILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SisAndBroAvaGeb[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_AromaLadyRose[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SUNKERN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SEED_BOMB, MOVE_SUNNY_DAY, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SUNFLORA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleSamir[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleDenise[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_LANTURN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_THUNDER, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_LANTURN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_THUNDER, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TwinsMiuMia[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerEarl[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacFoster[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacLarry[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerDaryl[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_SUDOWOODO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_WOOD_HAMMER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacHector[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_KANGASKHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicDario[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_REFLECT, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_GIRAFARIG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicRodette[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_XATU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_AromaLadyMiah[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_BELLOSSOM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SUNNY_DAY, MOVE_MOONBLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_BELLOSSOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_MOONBLAST, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungCoupleEveJon[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_LIGHT_SCREEN, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerMason[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FORRETRESS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FORRETRESS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlCyndy[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_DETECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlJocelyn[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_DETECT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerEvan[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_URSARING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_HIGH_HORSEPOWER, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacMark2[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_RUN, MOVE_ROCK_TOMB, MOVE_HAMMER_ARM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerLogan[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_SUNNY_DAY, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerJackson[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_SUNNY_DAY, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_TANGELA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ENERGY_BALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerBeth[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerKatelyn[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_CHANSEY,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerLeroy[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_KANGASKHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_URSARING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_HIGH_HORSEPOWER, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerMichelle[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_ICY_WIND, MOVE_AQUA_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FLARE_BLITZ, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GIRAFARIG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CoolCoupleLexNya[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_TAUROS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_MILTANK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BODY_SLAM, MOVE_HIGH_HORSEPOWER, MOVE_PROTECT, MOVE_MILK_DRINK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacBrandon[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_SPIT_UP},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacBenjamin[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_SANDSTORM, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PainterEdna[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_SMEARGLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_ICY_WIND, MOVE_SEISMIC_TOSS, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GentlemanClifford[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// SCREECH: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LadySelphy[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FACADE, MOVE_SCREECH, MOVE_THUNDERBOLT, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacLawson[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_SANDSTORM, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_ROCK_TOMB},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_THRASH},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicLaura[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_XATU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_XATU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_XATU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnBreederBethany[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_CHANSEY,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnBreederAllison[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_REFLECT, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherGarret[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_ARIADOS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_POISON_JAB, MOVE_STRING_SHOT, MOVE_BUG_BITE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_THRASH},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherJonah[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_POISON_JAB, MOVE_STRING_SHOT, MOVE_BUG_BITE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_YANMA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_DETECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_YANMA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_DETECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_POISON_JAB, MOVE_BUG_BITE, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherVance[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VENOMOTH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_SLEEP_POWDER, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VENOMOTH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_LEECH_LIFE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterNash[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterCordell[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_FARFETCHD,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BRAVE_BIRD, MOVE_LEER, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_FARFETCHD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRAVE_BIRD, MOVE_FACADE, MOVE_PROTECT, MOVE_SECRET_POWER},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassDalia[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_JUMPLUFF,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SUNNY_DAY, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_JUMPLUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_JUMPLUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_JUMPLUFF,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_GIGA_DRAIN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassJoana[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_GRANBULL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PLAY_ROUGH, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperRiley[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PINSIR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_ROCK_TOMB, MOVE_SUPERPOWER, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_THRASH},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerMarcy[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_SUNNY_DAY, MOVE_ENERGY_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_ENERGY_BALL, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_PARASECT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_X_SCISSOR, MOVE_ENERGY_BALL, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacLayton[] = {
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 220,
        .lvl = 86,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerKelsey2[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TAIL_WHIP, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ICE_BEAM},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerKelsey3[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerKelsey4[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperRicky2[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_WARTORTLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperRicky3[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperRicky4[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperJeff2[] = {
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperJeff3[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperJeff4[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerIsabelle2[] = {
    {
        .iv = 120,
        .lvl = 23,
        .species = SPECIES_GLOOM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_FACADE, MOVE_SECRET_POWER},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerIsabelle3[] = {
    {
        .iv = 180,
        .lvl = 37,
        .species = SPECIES_NIDORINA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_TOXIC, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerIsabelle4[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_REFLECT, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterYasu2[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterYasu3[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_EngineerBernie2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SANDSTORM, MOVE_FLASH_CANNON, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_SHOCK_WAVE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MAGNETON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_PROTECT, MOVE_SHOCK_WAVE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerDarian2[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperChris2[] = {
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_GROWLITHE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 34,
        .species = SPECIES_CHARMELEON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_DRAGON_CLAW, MOVE_PROTECT, MOVE_FIRE_FANG},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperChris3[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_HEAT_WAVE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CamperChris4[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_SUNNY_DAY, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerAlicia2[] = {
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_JIGGLYPUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 35,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerAlicia3[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SPIT_UP, MOVE_REFLECT, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerAlicia4[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLEEP_POWDER, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerJeremy2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BRICK_BREAK, MOVE_STRENGTH, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacMark3[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_DRILL_RUN, MOVE_PROTECT, MOVE_MEGAHORN},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacHerman2[] = {
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 42,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_ZEN_HEADBUTT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacHerman3[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_MAROWAK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STOMPING_TANTRUM, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerTrent2[] = {
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_GOLEM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_TOMB, MOVE_SANDSTORM, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 44,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_IRON_TAIL, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassMegan2[] = {
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 32,
        .species = SPECIES_NIDORINO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_ICE_BEAM},
    },
    {
        .iv = 120,
        .lvl = 28,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 30,
        .species = SPECIES_PIDGEOTTO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_LassMegan3[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_POISON_JAB},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 64,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SuperNerdGlenn2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_SLUDGE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_GamerRich2[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SUNNY_DAY, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAMETHROWER, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerJaren2[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_MUK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_SLUDGE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanElliot2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_RAZOR_SHELL},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_DRAGON_BREATH, MOVE_PROTECT, MOVE_BUBBLE_BEAM},
    },
};

// LIGHT_SCREEN: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RockerLuca2[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_LIGHT_SCREEN, MOVE_SWIFT, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautySheila2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_REFLECT, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperRobert2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperRobert3[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerSusie2[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerSusie3[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerSusie4[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_CRUNCH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerLukas2[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperBenny2[] = {
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperBenny3[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperMarlon2[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperMarlon3[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_DRILL_PECK, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BeautyGrace2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_WIGGLYTUFF,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_REFLECT, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// GROWL: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperChester2[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_GROWL, MOVE_FACADE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// GROWL: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperChester3[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THRASH, MOVE_GROWL, MOVE_DRILL_PECK, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_DRILL_PECK, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_DODRIO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_DRILL_PECK, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerBecky2[] = {
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 45,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerBecky3[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerBecky4[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinRonMya2[] = {
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 47,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinRonMya3[] = {
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinRonMya4[] = {
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// TOXIC: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerRuben2[] = {
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BELCH, MOVE_TOXIC, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 69,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallCamron2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BikerJaxon2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallIsaiah2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallCorey2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperJacob2[] = {
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AERIAL_ACE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 180,
        .lvl = 40,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_TAKE_DOWN, MOVE_AERIAL_ACE, MOVE_PROTECT, MOVE_FACADE},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperJacob3[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleAlice2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_MEGAHORN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleDarrin2[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_RAIN_DANCE, MOVE_DRAGON_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerMissy2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_MEGAHORN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PicnickerMissy3[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_MEGAHORN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_FishermanWade2[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_MEGAHORN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_WATER_PULSE},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleJack2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SisAndBroLilIan2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SisAndBroLilIan3[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_RAIN_DANCE, MOVE_DRAGON_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_RECOVER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleFinn2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_STARMIE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_HYDRO_PUMP, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_GOLDUCK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlSharon2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlSharon3[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlTanya2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlTanya3[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGA_KICK, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltShea2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltShea3[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltHugh2[] = {
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 74,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BlackBeltHugh3[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinMikKia2[] = {
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushKinMikKia3[] = {
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_CROSS_CHOP},
    },
};

// SLEEP_POWDER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TuberAmira2[] = {
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLEEP_POWDER, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 220,
        .lvl = 67,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TwinsJoyMeg2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_REFLECT, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PainterRayna2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_SMEARGLE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_ICY_WIND, MOVE_SEISMIC_TOSS, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// TAIL_WHIP: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungsterDestin2[] = {
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_RATICATE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_TAIL_WHIP, MOVE_CRUNCH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// THUNDER_WAVE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnBreederAlize2[] = {
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_CLEFABLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_MOONBLAST, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_AZUMARILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_AQUA_TAIL},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungCoupleGiaJes2[] = {
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 43,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_FLAMETHROWER},
    },
};

// SANDSTORM: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_YoungCoupleGiaJes3[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_NIDOKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SANDSTORM, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_NIDOQUEEN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_EARTH_POWER, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SUPERPOWER},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperMilo2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_XATU,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HURRICANE, MOVE_FACADE, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

// LEER: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperChaz2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_LEER, MOVE_TAKE_DOWN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_FEAROW,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_DRILL_PECK, MOVE_TAKE_DOWN, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BirdKeeperHarold2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_NOCTOWL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_UPROAR, MOVE_REFLECT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_NOCTOWL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_UPROAR, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_MOONBLAST},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleNicole2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_AZUMARILL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_PLAY_ROUGH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_HYDRO_PUMP},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicJaclyn2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_XATU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleSamir2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_TENTACRUEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_HikerEarl2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HIGH_HORSEPOWER, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_RuinManiacLarry2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PokemaniacHector2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_KANGASKHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicDario2[] = {
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_REFLECT, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_GIRAFARIG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FACADE},
    },
};

// REFLECT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PsychicRodette2[] = {
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_REFLECT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_XATU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_SHADOW_BALL},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_HYPNO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_PSYSHOCK},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_JugglerMason2[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_FORRETRESS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_ROCK_TOMB, MOVE_DOUBLE_EDGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_FORRETRESS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_STRENGTH},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_ELECTRODE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_THUNDERBOLT, MOVE_SWIFT, MOVE_PROTECT, MOVE_THUNDER},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerNicolas2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_VICTREEBEL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerMadeline2[] = {
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SUNNY_DAY, MOVE_SLUDGE_BOMB, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CrushGirlCyndy2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_OUTRAGE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_STRENGTH, MOVE_PROTECT, MOVE_DETECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_TamerEvan2[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_LICKITUNG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_SANDSLASH,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SAND_TOMB, MOVE_STRENGTH, MOVE_PROTECT, MOVE_BRICK_BREAK},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_URSARING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_HIGH_HORSEPOWER, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// SUNNY_DAY: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerJackson2[] = {
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_SUNNY_DAY, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_TANGELA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ENERGY_BALL, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_PkmnRangerKatelyn2[] = {
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_CHANSEY,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_FIRE_BLAST, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_VILEPLUME,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_MOONLIGHT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerLeroy2[] = {
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_STONE_EDGE, MOVE_ROCK_TOMB, MOVE_DRILL_RUN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_WATERFALL, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_KANGASKHAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_STRENGTH, MOVE_OUTRAGE, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 76,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_MACH_PUNCH},
    },
    {
        .iv = 220,
        .lvl = 79,
        .species = SPECIES_URSARING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THRASH, MOVE_HIGH_HORSEPOWER, MOVE_PROTECT, MOVE_STRENGTH},
    },
};

// ICY_WIND: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CooltrainerMichelle2[] = {
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_ICY_WIND, MOVE_AQUA_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 77,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FACADE, MOVE_THUNDERBOLT, MOVE_PROTECT, MOVE_FAKE_OUT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_NINETALES,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_RAPIDASH,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FLARE_BLITZ, MOVE_DOUBLE_EDGE, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
    {
        .iv = 220,
        .lvl = 80,
        .species = SPECIES_GIRAFARIG,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_STRENGTH, MOVE_PROTECT, MOVE_FACADE},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CoolCoupleLexNya2[] = {
    {
        .iv = 220,
        .lvl = 82,
        .species = SPECIES_TAUROS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 82,
        .species = SPECIES_MILTANK,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BODY_SLAM, MOVE_HIGH_HORSEPOWER, MOVE_PROTECT, MOVE_MILK_DRINK},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherColton2[] = {
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AIR_SLASH, MOVE_STRING_SHOT, MOVE_SHADOW_BALL, MOVE_PROTECT},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_WEEDLE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BITE, MOVE_POISON_STING, MOVE_STRING_SHOT, MOVE_NONE},
    },
    {
        .iv = 120,
        .lvl = 25,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AIR_SLASH, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_GIGA_DRAIN},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherColton3[] = {
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_JAB, MOVE_STRING_SHOT, MOVE_BUG_BITE, MOVE_PROTECT},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_PSYCHIC},
    },
    {
        .iv = 180,
        .lvl = 38,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_PSYCHIC},
    },
};

// STRING_SHOT: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_BugCatcherColton4[] = {
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_STRING_SHOT, MOVE_AIR_SLASH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 78,
        .species = SPECIES_BEEDRILL,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_POISON_JAB, MOVE_BUG_BITE, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
    {
        .iv = 220,
        .lvl = 75,
        .species = SPECIES_BUTTERFREE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_PSYCHIC},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleMatthew2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_POLIWRATH,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerMaleTony2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_DRAGON_PULSE, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_DRAGON_PULSE, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// RAIN_DANCE: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_SwimmerFemaleMelissa2[] = {
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_SEAKING,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WATERFALL, MOVE_RAIN_DANCE, MOVE_MEGAHORN, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 71,
        .species = SPECIES_POLITOED,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_PROTECT, MOVE_WATERFALL},
    },
};

// Helping Hand + Surf/Ice Beam na abertura; quatro tipos Gelo, sono e controle de velocidade; Tyranitar como alternativa de pressão por areia
static const struct TrainerMonItemCustomMoves sParty_EliteFourLorelei2[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HELPING_HAND, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_DETECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_SPIKES, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_PILOSWINE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_MUD_SHOT, MOVE_ROCK_SLIDE, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_JYNX,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_PSYCHIC, MOVE_LOVELY_KISS, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_BRICK_BREAK, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_LAPRAS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDERBOLT, MOVE_PROTECT},
    },
};

// Hariyama + Kyogre na abertura com Fake Out; Ludicolo aproveita chuva, Slowbro paralisa; Groudon oferece modo de sol e cobertura separado, sem Trick Room
static const struct TrainerMonItemCustomMoves sParty_EliteFourBruno2[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_HARIYAMA,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FAKE_OUT, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_KYOGRE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_LUDICOLO,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_GIGA_DRAIN, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_SURF, MOVE_THUNDER_WAVE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_GROUDON,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_MUD_SHOT, MOVE_ROCK_SLIDE, MOVE_FLAMETHROWER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_BODY_SLAM, MOVE_PROTECT},
    },
};

// Mawile + Gengar: Intimidate e Fake Tears apoiam dano especial; três Fantasmas, queimadura/paralisia, cobertura elétrica/água e Tyranitar como alternativa de areia
static const struct TrainerMonItemCustomMoves sParty_EliteFourAgatha2[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_MAWILE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FAKE_TEARS, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SHADOW_BALL, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_DUSCLOPS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_NIGHT_SHADE, MOVE_SHADOW_BALL, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_PAIN_SPLIT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_TAUNT, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_LANTURN,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_THUNDERBOLT, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
};

// Hitmontop + Kyogre: Intimidate e Helping Hand; Claydol usa telas, Raichu paralisa; Rayquaza fecha com dano misto e Air Lock, que também suspende a chuva aliada
static const struct TrainerMonItemCustomMoves sParty_EliteFourLance2[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_HELPING_HAND, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE, MOVE_DETECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_KYOGRE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDER, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_SHADOW_BALL, MOVE_WILL_O_WISP, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_CLAYDOL,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_REFLECT, MOVE_LIGHT_SCREEN, MOVE_PSYCHIC, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_RAICHU,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_LIGHT_SCREEN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_RAYQUAZA,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_DRAGON_CLAW, MOVE_FLAMETHROWER, MOVE_ROCK_SLIDE, MOVE_PROTECT},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChampionRematchSquirtle[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_WAVE_CRASH, MOVE_ROCK_TOMB, MOVE_ICE_BEAM, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CRUNCH, MOVE_STONE_EDGE, MOVE_PROTECT, MOVE_THRASH},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_THRASH},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChampionRematchBulbasaur[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CRUNCH, MOVE_ROCK_TOMB, MOVE_THRASH, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_MEGAHORN, MOVE_PROTECT, MOVE_THRASH},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_PROTECT, MOVE_EXTREME_SPEED},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PETAL_DANCE, MOVE_SLUDGE_BOMB, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_ChampionRematchCharmander[] = {
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_CLOSE_COMBAT, MOVE_ROCK_TOMB, MOVE_MEGAHORN, MOVE_PROTECT},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_PROTECT, MOVE_RECOVER},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CRUNCH, MOVE_STONE_EDGE, MOVE_PROTECT, MOVE_THRASH},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_LEFTOVERS,
        .moves = {MOVE_WOOD_HAMMER, MOVE_PSYCHIC, MOVE_PROTECT, MOVE_SYNTHESIS},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_AQUA_TAIL, MOVE_THRASH, MOVE_PROTECT, MOVE_WATERFALL},
    },
    {
        .iv = 255,
        .lvl = 100,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLARE_BLITZ, MOVE_AIR_SLASH, MOVE_PROTECT, MOVE_FIRE_BLAST},
    },
};

// ROCK_TOMB: suporte de abertura + pressão e cobertura do parceiro
static const struct TrainerMonItemCustomMoves sParty_CueBallPaxton[] = {
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_MUK,
        .heldItem = ITEM_LUM_BERRY,
        .moves = {MOVE_GUNK_SHOT, MOVE_ROCK_TOMB, MOVE_STRENGTH, MOVE_PROTECT},
    },
    {
        .iv = 220,
        .lvl = 68,
        .species = SPECIES_WEEZING,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_BELCH, MOVE_FIRE_BLAST, MOVE_PROTECT, MOVE_SLUDGE_BOMB},
    },
};

// Region geography: pokeemerald TRAINER_CALVIN_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennCalvin[] = {
    {
    .iv = 0,
    .lvl = 65,
    .species = SPECIES_POOCHYENA,
    }
};

// Region geography: pokeemerald TRAINER_RICK, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennRick[] = {
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_WURMPLE,
    },
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_WURMPLE,
    }
};

// Region geography: pokeemerald TRAINER_TIANA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennTiana[] = {
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_SHROOMISH,
    }
};

// Region geography: pokeemerald TRAINER_ALLEN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennAllen[] = {
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_TAILLOW,
    }
};

// Region geography: pokeemerald TRAINER_DAISY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDaisy[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_SHROOMISH,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ROSELIA,
    }
};

// Region geography: pokeemerald TRAINER_AMY_AND_LIV_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennAmyAndLiv[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_PLUSLE,
    },
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MINUN,
    }
};

// Region geography: pokeemerald TRAINER_ANDREW, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennAndrew[] = {
    {
    .iv = 0,
    .lvl = 65,
    .species = SPECIES_MAGIKARP,
    },
    {
    .iv = 0,
    .lvl = 70,
    .species = SPECIES_TENTACOOL,
    },
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MAGIKARP,
    }
};

// Region geography: pokeemerald TRAINER_MIGUEL_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennMiguel[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_SKITTY,
    }
};

// Region geography: pokeemerald TRAINER_RHETT, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennRhett[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MAKUHITA,
    }
};

// Region geography: pokeemerald TRAINER_MARCOS, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennMarcos[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_VOLTORB,
    }
};

// Region geography: pokeemerald TRAINER_ISABELLE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennIsabelle[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MARILL,
    }
};

// Region geography: pokeemerald TRAINER_PETE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennPete[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_TENTACOOL,
    }
};

// Region geography: HNS_modern TRAINER_JOEY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoJoey[] = {
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_RATTATA,
    }
};

// Region geography: HNS_modern TRAINER_MIKEY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoMikey[] = {
    {
    .iv = 0,
    .lvl = 62,
    .species = SPECIES_HOOTHOOT,
    },
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_SENTRET,
    }
};

// Region geography: HNS_modern TRAINER_DON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoDon[] = {
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_LEDYBA,
    },
    {
    .iv = 0,
    .lvl = 63,
    .species = SPECIES_SPINARAK,
    }
};

// Region geography: HNS_modern TRAINER_WADE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoWade[] = {
    {
    .iv = 0,
    .lvl = 64,
    .species = SPECIES_WEEDLE,
    },
    {
    .iv = 0,
    .lvl = 65,
    .species = SPECIES_PINECO,
    }
};

// Region geography: HNS_modern TRAINER_ALBERT, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoAlbert[] = {
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_SENTRET,
    },
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_ZUBAT,
    }
};

// Region geography: HNS_modern TRAINER_LIZ, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoLiz[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_NIDORAN_F,
    }
};

// Region geography: HNS_modern TRAINER_ROLAND, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoRoland[] = {
    {
    .iv = 0,
    .lvl = 69,
    .species = SPECIES_NIDORAN_M,
    }
};

// Region geography: HNS_modern TRAINER_RALPH, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoRalph[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_GOLDEEN,
    }
};

// Region geography: HNS_modern TRAINER_HENRY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoHenry[] = {
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_MARILL,
    },
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_POLIWAG,
    }
};

// Region geography: HNS_modern TRAINER_JUSTIN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoJustin[] = {
    {
    .iv = 0,
    .lvl = 67,
    .species = SPECIES_TENTACOOL,
    }
};

// Region geography: HNS_modern TRAINER_GORDON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoGordon[] = {
    {
    .iv = 0,
    .lvl = 70,
    .species = SPECIES_WOOPER,
    }
};

// Region geography: HNS_modern TRAINER_PETER, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoPeter[] = {
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_SPEAROW,
    },
    {
    .iv = 0,
    .lvl = 68,
    .species = SPECIES_NATU,
    }
};

// Region geography: HNS_modern TRAINER_HARRY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoHarry[] = {
    {
    .iv = 0,
    .lvl = 83,
    .species = SPECIES_QUAGSIRE,
    }
};

// Region geography: HNS_modern TRAINER_DANA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoDana[] = {
    {
    .iv = 0,
    .lvl = 79,
    .species = SPECIES_FLAAFFY,
    },
    {
    .iv = 0,
    .lvl = 80,
    .species = SPECIES_PSYDUCK,
    }
};

// Region geography: HNS_modern TRAINER_OLIVIA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoOlivia[] = {
    {
    .iv = 0,
    .lvl = 81,
    .species = SPECIES_CORSOLA,
    }
};

// Region geography: HNS_modern TRAINER_TOBY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoToby[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_DODUO,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_DODUO,
    }
};

// Region geography: HNS_modern TRAINER_CHAD, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoChad[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_MR_MIME,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_MAGNEMITE,
    }
};

// Region geography: HNS_modern TRAINER_VALERIE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoValerie[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_SKIPLOOM,
    },
    {
    .iv = 0,
    .lvl = 81,
    .species = SPECIES_MILTANK,
    }
};

// Region geography: HNS_modern TRAINER_NORMAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoNorman[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_SLOWPOKE,
    },
    {
    .iv = 0,
    .lvl = 83,
    .species = SPECIES_SLOWPOKE,
    }
};

// Region geography: HNS_modern TRAINER_RUTH, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoRuth[] = {
    {
    .iv = 0,
    .lvl = 83,
    .species = SPECIES_PIKACHU,
    }
};

// Region geography: HNS_modern TRAINER_DEREK, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoDerek[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_PIKACHU,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_PONYTA,
    }
};

// Region geography: HNS_modern TRAINER_EUGENE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoEugene[] = {
    {
    .iv = 0,
    .lvl = 80,
    .species = SPECIES_POLIWHIRL,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_TAUROS,
    }
};

// Region geography: HNS_modern TRAINER_JAMIE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoJamie[] = {
    {
    .iv = 0,
    .lvl = 80,
    .species = SPECIES_UMBREON,
    }
};

// Region geography: pokeemerald TRAINER_DYLAN_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDylan[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_DODUO,
    }
};

// Region geography: pokeemerald TRAINER_LYDIA_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennLydia[] = {
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_WINGULL,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_SHROOMISH,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_MARILL,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_ROSELIA,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_SKITTY,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_GOLDEEN,
    }
};

// Region geography: pokeemerald TRAINER_ISAAC_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennIsaac[] = {
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_WHISMUR,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_ARON,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_POOCHYENA,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_TAILLOW,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_MAKUHITA,
    }
};

// Region geography: pokeemerald TRAINER_MARIA_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennMaria[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_DODUO,
    }
};

// Region geography: pokeemerald TRAINER_DEREK, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDerek[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_DUSTOX,
    },
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_BEAUTIFLY,
    }
};

// Region geography: pokeemerald TRAINER_ANNA_AND_MEG_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennAnnaAndMeg[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_MAKUHITA,
    }
};

// Region geography: pokeemerald TRAINER_BRANDI, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBrandi[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_RALTS,
    }
};

// Region geography: pokeemerald TRAINER_AISHA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennAisha[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_MEDITITE,
    }
};

// Region geography: pokeemerald TRAINER_MELINA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennMelina[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_DODUO,
    }
};

// Region geography: HNS_modern TRAINER_TULLY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoTully[] = {
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_QWILFISH,
    }
};

// Region geography: HNS_modern TRAINER_BENJAMIN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoBenjamin[] = {
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_GRAVELER,
    },
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_DUGTRIO,
    }
};

// Region geography: HNS_modern TRAINER_SHANE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoShane[] = {
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_NIDORINO,
    },
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_NIDOKING,
    }
};

// Region geography: pokeemerald TRAINER_EDWARD, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennEdward[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_ABRA,
    }
};

// Region geography: pokeemerald TRAINER_JACLYN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennJaclyn[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_ABRA,
    }
};

// Region geography: pokeemerald TRAINER_EDWIN_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennEdwin[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_LOMBRE,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_NUZLEAF,
    }
};

// Region geography: pokeemerald TRAINER_DALE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDale[] = {
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_TENTACOOL,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_WAILMER,
    },
    {
    .iv = 0,
    .lvl = 71,
    .species = SPECIES_TENTACOOL,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_WAILMER,
    }
};

// Region geography: pokeemerald TRAINER_TIMMY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennTimmy[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_ARON,
    },
    {
    .iv = 0,
    .lvl = 73,
    .species = SPECIES_ELECTRIKE,
    }
};

// Region geography: pokeemerald TRAINER_ISABEL_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennIsabel[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_PLUSLE,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_MINUN,
    }
};

// Region geography: pokeemerald TRAINER_KALEB, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennKaleb[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_MINUN,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_PLUSLE,
    }
};

// Region geography: pokeemerald TRAINER_ALYSSA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennAlyssa[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MAGNEMITE,
    }
};

// Region geography: pokeemerald TRAINER_JOSEPH, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennJoseph[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ELECTRIKE,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_VOLTORB,
    }
};

// Region geography: HNS_modern TRAINER_SPENCER, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoSpencer[] = {
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_SANDSLASH,
    },
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_GOLBAT,
    }
};

// Region geography: HNS_modern TRAINER_TIFFANY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoTiffany[] = {
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_CLEFABLE,
    },
    {
    .iv = 0,
    .lvl = 84,
    .species = SPECIES_WIGGLYTUFF,
    }
};

// Region geography: HNS_modern TRAINER_BEN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoBen[] = {
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_SLOWBRO,
    },
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_WARTORTLE,
    }
};

// Region geography: HNS_modern TRAINER_MARVIN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoMarvin[] = {
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_GYARADOS,
    },
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_GYARADOS,
    }
};

// Region geography: HNS_modern TRAINER_RON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoRon[] = {
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_NIDOKING,
    },
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_CHARMELEON,
    }
};

// Region geography: HNS_modern TRAINER_BRENT, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoBrent[] = {
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_LICKITUNG,
    },
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_IVYSAUR,
    }
};

// Region geography: HNS_modern TRAINER_ANDRE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoAndre[] = {
    {
    .iv = 0,
    .lvl = 87,
    .species = SPECIES_GYARADOS,
    }
};

// Region geography: HNS_modern TRAINER_RAYMOND, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoRaymond[] = {
    {
    .iv = 0,
    .lvl = 88,
    .species = SPECIES_VAPOREON,
    }
};

// Region geography: HNS_modern TRAINER_LOIS, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoLois[] = {
    {
    .iv = 0,
    .lvl = 88,
    .species = SPECIES_SKIPLOOM,
    },
    {
    .iv = 0,
    .lvl = 87,
    .species = SPECIES_NINETALES,
    }
};

// Region geography: HNS_modern TRAINER_AARON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_JohtoAaron[] = {
    {
    .iv = 0,
    .lvl = 87,
    .species = SPECIES_IVYSAUR,
    },
    {
    .iv = 0,
    .lvl = 87,
    .species = SPECIES_CHARMELEON,
    },
    {
    .iv = 0,
    .lvl = 87,
    .species = SPECIES_WARTORTLE,
    }
};

// Region geography: pokeemerald TRAINER_PERRY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennPerry[] = {
    {
    .iv = 0,
    .lvl = 86,
    .species = SPECIES_WINGULL,
    }
};

// Region geography: pokeemerald TRAINER_ROSE_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennRose[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ROSELIA,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_SHROOMISH,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ROSELIA,
    }
};

// Region geography: pokeemerald TRAINER_WADE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennWade[] = {
    {
    .iv = 0,
    .lvl = 76,
    .species = SPECIES_TENTACOOL,
    }
};

// Region geography: pokeemerald TRAINER_CHESTER, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennChester[] = {
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_TAILLOW,
    },
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_SWELLOW,
    }
};

// Region geography: pokeemerald TRAINER_BARNY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBarny[] = {
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_TENTACOOL,
    },
    {
    .iv = 0,
    .lvl = 85,
    .species = SPECIES_CARVANHA,
    }
};

// Region geography: pokeemerald TRAINER_DALTON_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDalton[] = {
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_MAGNEMITE,
    },
    {
    .iv = 0,
    .lvl = 75,
    .species = SPECIES_WHISMUR,
    }
};

// Region geography: pokeemerald TRAINER_DEANDRE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDeandre[] = {
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ZIGZAGOON,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ARON,
    },
    {
    .iv = 0,
    .lvl = 74,
    .species = SPECIES_ELECTRIKE,
    }
};

// Region geography: pokeemerald TRAINER_HEIDI, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennHeidi[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_SANDSHREW,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_BALTOY,
    }
};

// Region geography: pokeemerald TRAINER_DREW, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDrew[] = {
    {
    .iv = 0,
    .lvl = 83,
    .species = SPECIES_SANDSHREW,
    }
};

// Region geography: pokeemerald TRAINER_DUSTY_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDusty[] = {
    {
    .iv = 0,
    .lvl = 83,
    .species = SPECIES_SANDSLASH,
    }
};

// Region geography: pokeemerald TRAINER_BEAU, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBeau[] = {
    {
    .iv = 0,
    .lvl = 81,
    .species = SPECIES_BALTOY,
    },
    {
    .iv = 0,
    .lvl = 81,
    .species = SPECIES_SANDSHREW,
    },
    {
    .iv = 0,
    .lvl = 81,
    .species = SPECIES_BALTOY,
    }
};

// Region geography: pokeemerald TRAINER_BECKY, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBecky[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_SANDSHREW,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_MARILL,
    }
};

// Region geography: pokeemerald TRAINER_IRENE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennIrene[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_SHROOMISH,
    },
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_MARILL,
    }
};

// Region geography: pokeemerald TRAINER_TRAVIS, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennTravis[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_SANDSHREW,
    }
};

// Region geography: pokeemerald TRAINER_DAISUKE, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennDaisuke[] = {
    {
    .iv = 0,
    .lvl = 79,
    .species = SPECIES_MACHOP,
    }
};

// Region geography: pokeemerald TRAINER_BROOKE_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBrooke[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_WINGULL,
    },
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_NUMEL,
    },
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_ROSELIA,
    }
};

// Region geography: pokeemerald TRAINER_WILTON_1, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennWilton[] = {
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_ELECTRIKE,
    },
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_WAILMER,
    },
    {
    .iv = 0,
    .lvl = 77,
    .species = SPECIES_MAKUHITA,
    }
};

// Region geography: pokeemerald TRAINER_TYRON, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennTyron[] = {
    {
    .iv = 0,
    .lvl = 79,
    .species = SPECIES_SANDSHREW,
    }
};

// Region geography: pokeemerald TRAINER_CELINA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennCelina[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_ROSELIA,
    }
};

// Region geography: pokeemerald TRAINER_BIANCA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBianca[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_SHROOMISH,
    }
};

// Region geography: pokeemerald TRAINER_HAYDEN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennHayden[] = {
    {
    .iv = 0,
    .lvl = 78,
    .species = SPECIES_NUMEL,
    }
};

// Region geography: pokeemerald TRAINER_BRYAN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBryan[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_SANDSHREW,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_SANDSLASH,
    }
};

// Region geography: pokeemerald TRAINER_CELIA, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennCelia[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_MARILL,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_LOMBRE,
    }
};

// Region geography: pokeemerald TRAINER_BRANDEN, levels +60.
static const struct TrainerMonNoItemDefaultMoves sParty_HoennBranden[] = {
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_TAILLOW,
    },
    {
    .iv = 0,
    .lvl = 82,
    .species = SPECIES_NUZLEAF,
    }
};
