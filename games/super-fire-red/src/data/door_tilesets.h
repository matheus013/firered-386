// Exact pairs own their complete door table; no per-ID fallback to Kanto.
extern const struct Tileset gTileset_HoennPilotGeneral;
extern const struct Tileset gTileset_HoennPilotPetalburg;
extern const struct Tileset gTileset_HoennPilotRustboro;
extern const struct Tileset gTileset_HoennPilotMauville;
#include "hoenn_pilot_doors.h"
extern const struct Tileset gTileset_JohtoPilotGeneral;
extern const struct Tileset gTileset_JohtoPilotNewBarkTown;
extern const struct Tileset gTileset_JohtoPilotCherrygroveCity;
extern const struct Tileset gTileset_JohtoPilotVioletCity;
extern const struct Tileset gTileset_JohtoPilotRoute32;
extern const struct Tileset gTileset_JohtoPilotSouth;
extern const struct Tileset gTileset_JohtoPilotAzaleaTown;
extern const struct Tileset gTileset_JohtoPilotGoldenrod;
extern const struct Tileset gTileset_JohtoPilotNorthWest;
extern const struct Tileset gTileset_JohtoPilotEcruteakCity;
extern const struct Tileset gTileset_JohtoPilotOlivineCity;
#include "johto_pilot_doors.h"
static const struct DoorTilesetBinding sDoorTilesetBindings[] = {
    {&gTileset_HoennPilotGeneral, &gTileset_HoennPilotPetalburg, sHoennPilotDoors},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotNewBarkTown, sJohtoPilotDoors},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotCherrygroveCity, sJohtoPilotDoors_Cherrygrove},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotVioletCity, sJohtoPilotDoors_Violet},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotRoute32, sJohtoPilotDoors_Route32},
    {&gTileset_JohtoPilotSouth, &gTileset_JohtoPilotAzaleaTown, sJohtoPilotDoors_Azalea},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotGoldenrod, sJohtoPilotDoors_Goldenrod},
    {&gTileset_HoennPilotGeneral, &gTileset_HoennPilotRustboro, sHoennPilotDoors_Rustboro},
    {&gTileset_JohtoPilotNorthWest, &gTileset_JohtoPilotEcruteakCity, sJohtoPilotDoors_Ecruteak},
    {&gTileset_HoennPilotGeneral, &gTileset_HoennPilotMauville, sHoennPilotDoors_Mauville},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotOlivineCity, sJohtoPilotDoors_Olivine},
    {NULL, NULL, NULL}
};
