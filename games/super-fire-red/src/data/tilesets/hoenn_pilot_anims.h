// Hoenn pilot exteriors only. Source: pret/pokeemerald.
static const u16 sHoennFlower0[] = INCBIN_U16("data/tilesets/primary/hoenn_pilot/anim/Flower_0.4bpp");
static const u16 sHoennFlower1[] = INCBIN_U16("data/tilesets/primary/hoenn_pilot/anim/Flower_1.4bpp");
static const u16 sHoennFlower2[] = INCBIN_U16("data/tilesets/primary/hoenn_pilot/anim/Flower_2.4bpp");
static const u16 *const sHoennFlower[] = {sHoennFlower0, sHoennFlower1, sHoennFlower0, sHoennFlower2};
static void TilesetAnim_HoennPilot(u16 timer)
{
    if (timer % 16 == 0)
        AppendTilesetAnimToBuffer(sHoennFlower[(timer / 16) % ARRAY_COUNT(sHoennFlower)], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP(508)), 4 * TILE_SIZE_4BPP);
}
void InitTilesetAnim_HoennPilot(void)
{
    sPrimaryTilesetAnimCounter = 0;
    sPrimaryTilesetAnimCounterMax = 256;
    sPrimaryTilesetAnimCallback = TilesetAnim_HoennPilot;
}
void InitTilesetAnim_HoennPetalburgPilot(void)
{
    sSecondaryTilesetAnimCounter = 0;
    sSecondaryTilesetAnimCounterMax = sPrimaryTilesetAnimCounterMax;
    sSecondaryTilesetAnimCallback = NULL;
}
