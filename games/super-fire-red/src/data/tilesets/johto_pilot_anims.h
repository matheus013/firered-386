// Johto pilot (New Bark Town, Route 29). Source: HNS_modern TilesetAnim_JohtoGeneral, same schedule.
static const u16 sJohtoFlower0[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/flower/0.4bpp");
static const u16 sJohtoFlower1[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/flower/1.4bpp");
static const u16 sJohtoFlower2[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/flower/2.4bpp");
static const u16 sJohtoFlower3[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/flower/3.4bpp");
static const u16 sJohtoFlower4[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/flower/4.4bpp");
static const u16 sJohtoWater0[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/0.4bpp");
static const u16 sJohtoWater1[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/1.4bpp");
static const u16 sJohtoWater2[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/2.4bpp");
static const u16 sJohtoWater3[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/3.4bpp");
static const u16 sJohtoWater4[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/4.4bpp");
static const u16 sJohtoWater5[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/5.4bpp");
static const u16 sJohtoWater6[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/6.4bpp");
static const u16 sJohtoWater7[] = INCBIN_U16("data/tilesets/primary/johto_pilot_general/anim/water_current_landwatersedge/7.4bpp");
static const u16 *const sJohtoFlower[] = {sJohtoFlower0, sJohtoFlower1, sJohtoFlower2, sJohtoFlower3, sJohtoFlower4};
static const u16 *const sJohtoWater[] = {sJohtoWater0, sJohtoWater1, sJohtoWater2, sJohtoWater3, sJohtoWater4, sJohtoWater5, sJohtoWater6, sJohtoWater7};
static void TilesetAnim_JohtoPilot(u16 timer)
{
    // Sand/water edge: first 18 tiles of each water frame. The HNS land/water edge animation is disabled
    // and its waterfall tiles are not used by the pilot maps.
    if (timer % 16 == 0)
        AppendTilesetAnimToBuffer(sJohtoWater[(timer / 16) % ARRAY_COUNT(sJohtoWater)], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP(416)), 18 * TILE_SIZE_4BPP);
    if (timer % 16 == 2)
        AppendTilesetAnimToBuffer(sJohtoFlower[(timer >> 4) % ARRAY_COUNT(sJohtoFlower)], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP(508)), 4 * TILE_SIZE_4BPP);
}

void InitTilesetAnim_JohtoPilot(void)
{
    sPrimaryTilesetAnimCounter = 0;
    sPrimaryTilesetAnimCounterMax = 256;
    sPrimaryTilesetAnimCallback = TilesetAnim_JohtoPilot;
}
