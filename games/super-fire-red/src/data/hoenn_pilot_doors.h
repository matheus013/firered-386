// Hoenn pilot doors. Source: pret/pokeemerald.
static const u8 sHoennDoorTiles_Littleroot[] = INCBIN_U8("graphics/door_anims/hoenn/littleroot.4bpp");
static const u8 sHoennDoorPalettes_Littleroot[] = {11, 11, 7, 7, 7, 7, 7, 7};
static const u8 sHoennDoorTiles_BirchsLab[] = INCBIN_U8("graphics/door_anims/hoenn/birchs_lab.4bpp");
static const u8 sHoennDoorPalettes_BirchsLab[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const struct DoorGraphics sHoennPilotDoors[] = {
    {0x2C8, DOOR_SOUND_NORMAL, DOOR_SIZE_1x2, sHoennDoorTiles_Littleroot, sHoennDoorPalettes_Littleroot},
    {0x2C9, DOOR_SOUND_NORMAL, DOOR_SIZE_1x2, sHoennDoorTiles_BirchsLab, sHoennDoorPalettes_BirchsLab},
    {}
};
