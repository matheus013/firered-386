// Johto pilot doors. Source: HNS_modern src/field_door.c (blue reuses its cerulean.png).
static const u8 sJohtoDoorTiles_new_bark_blue[] = INCBIN_U8("graphics/door_anims/johto/new_bark_blue.4bpp");
static const u8 sJohtoDoorPalettes_new_bark_blue[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sJohtoDoorTiles_new_bark_yellow[] = INCBIN_U8("graphics/door_anims/johto/new_bark_yellow.4bpp");
static const u8 sJohtoDoorPalettes_new_bark_yellow[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sJohtoDoorTiles_sliding[] = INCBIN_U8("graphics/door_anims/johto/sliding.4bpp");
static const u8 sJohtoDoorPalettes_sliding[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const struct DoorGraphics sJohtoPilotDoors[] = {
    {0x2CB, DOOR_SOUND_NORMAL, DOOR_SIZE_1x1, sJohtoDoorTiles_new_bark_blue, sJohtoDoorPalettes_new_bark_blue},
    {0x309, DOOR_SOUND_NORMAL, DOOR_SIZE_1x1, sJohtoDoorTiles_new_bark_yellow, sJohtoDoorPalettes_new_bark_yellow},
    {}
};
static const struct DoorGraphics sJohtoPilotDoors_Cherrygrove[] = {
    {0x62, DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sJohtoDoorTiles_sliding, sJohtoDoorPalettes_sliding},
    {}
};
static const struct DoorGraphics sJohtoPilotDoors_Violet[] = {
    {0x62, DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sJohtoDoorTiles_sliding, sJohtoDoorPalettes_sliding},
    {}
};
static const struct DoorGraphics sJohtoPilotDoors_Route32[] = {
    {0x62, DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sJohtoDoorTiles_sliding, sJohtoDoorPalettes_sliding},
    {}
};
static const struct DoorGraphics sJohtoPilotDoors_Azalea[] = {
    {0x62, DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sJohtoDoorTiles_sliding, sJohtoDoorPalettes_sliding},
    {}
};
