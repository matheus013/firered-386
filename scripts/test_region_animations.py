import unittest
from prepare_hoenn_animations import required_tracks, read_tracks


class AnimationTests(unittest.TestCase):
    def test_source_sequence_and_timing(self):
        source = 'void InitTilesetAnim_General(void) { sPrimaryTilesetAnimCounterMax = 256; }\n'
        source += 'void InitTilesetAnim_Petalburg(void) { sSecondaryTilesetAnimCallback = NULL; }\n'
        for phase, name in enumerate(('Flower', 'Water', 'SandWaterEdge', 'Waterfall', 'LandWaterEdge')):
            prefix = 'gTilesetAnims_General_' + name
            source += f'const u16 *const {prefix}[] = {{{prefix}_Frame0, {prefix}_Frame1, {prefix}_Frame0}};\n'
            for frame in (0, 1):
                source += f'{prefix}_Frame{frame}[] = INCGFX_U16("anim/{name}/{frame}.png", ".4bpp");\n'
            source += f'if (timer % 16 == {phase})\n QueueAnimTiles_General_{name}(timer / 16);\n'
            source += f'AppendTilesetAnimToBuffer({prefix}[i], (u16 *)(BG_VRAM + TILE_OFFSET_4BPP(508)), 4 * TILE_SIZE_4BPP);\n'
        tracks = read_tracks(source)
        self.assertEqual(len(tracks), 5)
        self.assertEqual(tracks[0]['sequence'], ['0', '1', '0'])
        self.assertEqual(tracks[4]['phase'], 4)
        self.assertEqual(tracks[0]['paths']['1'], 'anim/Flower/1.png')
        with self.assertRaises(ValueError):
            read_tracks(source.replace('timer % 16 == 0', 'timer % 8 == 0'))

    def test_half_open_dma_ranges(self):
        tracks = [dict(start=508, count=4)]
        for tile in (508, 509, 510, 511):
            self.assertEqual(required_tracks(tracks, {tile}), tracks)
        self.assertEqual(required_tracks(tracks, {507, 512}), [])

    def test_unrecognized_source_rejected(self):
        with self.assertRaises(ValueError):
            read_tracks('')


if __name__ == '__main__':
    unittest.main()
