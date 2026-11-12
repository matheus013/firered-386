// Exact pairs own their complete door table; no per-ID fallback to Kanto.
extern const struct Tileset gTileset_HoennPilotGeneral;
extern const struct Tileset gTileset_HoennPilotPetalburg;
#include "hoenn_pilot_doors.h"
extern const struct Tileset gTileset_JohtoPilotGeneral;
extern const struct Tileset gTileset_JohtoPilotNewBarkTown;
extern const struct Tileset gTileset_JohtoPilotCherrygroveCity;
#include "johto_pilot_doors.h"
static const struct DoorTilesetBinding sDoorTilesetBindings[] = {
    {&gTileset_HoennPilotGeneral, &gTileset_HoennPilotPetalburg, sHoennPilotDoors},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotNewBarkTown, sJohtoPilotDoors},
    {&gTileset_JohtoPilotGeneral, &gTileset_JohtoPilotCherrygroveCity, sJohtoPilotDoors_Cherrygrove},
    {NULL, NULL, NULL}
};
