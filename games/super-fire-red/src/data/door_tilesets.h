// Register imported door tables here after their tilesets and assets exist.
// Match BOTH tilesets: door palettes depend on the whole map layout.
// A registered table owns all door IDs for that pair; do not fall back per ID
// to Kanto, where the same ID can refer to a completely different door.
// Unregistered pairs keep the original FireRed table.
static const struct DoorTilesetBinding sDoorTilesetBindings[] = {
    {NULL, NULL, NULL}
};
