"""SDL original-C text flow integration; all assets are authorial fixtures.

Run with a HG_REFERENCE_TEXT_FLOW=ON executable. Optionally supply
--default-executable to verify that a public build rejects the private mode.
"""

import argparse
from pathlib import Path
import struct
import subprocess
import tempfile

from test_graphics_renderer import png_rgba
from test_text_renderer import narc, message_bank


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('executable', type=Path)
    parser.add_argument('--default-executable', type=Path)
    args = parser.parse_args()
    executable = str(args.executable.resolve())
    with tempfile.TemporaryDirectory(prefix='hg-text-flow-') as temporary:
        root = Path(temporary) / 'ação-ポケモン'
        font_path, text_path = root / 'a/0/1/6', root / 'a/0/2/7'
        font_path.parent.mkdir(parents=True)
        text_path.parent.mkdir(parents=True)
        font = struct.pack('<III4B', 16, 48, 2, 8, 8, 1, 1) + b'\x55' * 16 + b'\xaa' * 16 + bytes([8, 8])
        font_path.write_bytes(narc([font]))
        base = [executable, '--video-driver', 'dummy', '--text-resources', str(root),
                '--message-bank', '0', '--message-id', '0', '--text-flow-self-test']

        def run(units, frames=160, capture_at=None, success=True, extra=()):
            text_path.write_bytes(narc([message_bank(units)]))
            command = base + ['--frames', str(frames)] + list(extra)
            shot = None
            if capture_at is not None:
                shot = root / f'capture-{len(list(root.glob("*.png")))}.png'
                command += ['--capture', str(shot), '--capture-at', str(capture_at)]
            result = subprocess.run(command, capture_output=True, timeout=40)
            assert (result.returncode == 0) == success, result.stdout + result.stderr
            if success:
                assert b'Text flow self-test PASS' in result.stdout + result.stderr
            if shot is None:
                return result
            size, pixels = png_rgba(shot)
            assert size == (1280, 720)
            colors = [tuple(pixels[at:at + 4]) for at in range(0, len(pixels), 4)]
            return colors, result, shot

        first, _, _ = run([1, 2, 0xffff], frames=8, capture_at=0)
        final, _, shot = run([1, 2, 0xffff], frames=8, capture_at=2)
        assert first.count((12, 16, 24, 255)) == 576
        assert first.count((128, 144, 160, 255)) == 0
        assert final.count((12, 16, 24, 255)) == 576
        assert final.count((128, 144, 160, 255)) == 576

        def color_positions(colors, color):
            return {(index % 1280, index // 1280)
                    for index, pixel in enumerate(colors) if pixel == color}

        # Infer the authorial window's screen origin/scale from the existing
        # baseline, rather than duplicating the SDL viewport placement formula.
        baseline = color_positions(first, (12, 16, 24, 255))
        origin_x = min(x for x, _ in baseline)
        origin_y = min(y for _, y in baseline)
        glyph_width = max(x for x, _ in baseline) - origin_x + 1
        glyph_height = max(y for _, y in baseline) - origin_y + 1
        assert glyph_width == glyph_height and glyph_width % 8 == 0
        scale = glyph_width // 8

        def rectangle(x, y, width=8, height=8):
            return {(origin_x + px, origin_y + py)
                    for py in range(y * scale, (y + height) * scale)
                    for px in range(x * scale, (x + width) * scale)}

        # WidthFirstLine measures both 8px glyphs: 224px window, 16px line.
        # Inspect both colors so an implementation positioning only the first
        # glyph correctly cannot pass this regression.
        for command, start_x in ((0x205, 104), (0x206, 208)):
            aligned, _, _ = run([0xfffe, command, 0, 1, 2, 0xffff],
                                frames=8, capture_at=6)
            assert color_positions(aligned, (12, 16, 24, 255)) == rectangle(start_x, 0)
            assert color_positions(aligned, (128, 144, 160, 255)) == rectangle(start_x + 8, 0)

        # Explicit X/Y controls place the first pair at (4,16). The final glyph
        # begins at (220,140), clipping to 4x4 within the 224x144 host window.
        positioned = [0xfffe, 0x203, 1, 4, 0xfffe, 0x204, 1, 16, 1, 2,
                      0xfffe, 0x203, 1, 220, 0xfffe, 0x204, 1, 140, 1, 0xffff]
        clipped, _, _ = run(positioned, frames=14, capture_at=10)
        expected_foreground = rectangle(4, 16) | rectangle(220, 140, 4, 4)
        assert color_positions(clipped, (12, 16, 24, 255)) == expected_foreground
        assert color_positions(clipped, (128, 144, 160, 255)) == rectangle(12, 16)

        # Capture-at is deterministic, and a second write must not replace it.
        repeat, _, _ = run([1, 2, 0xffff], frames=8, capture_at=2)
        assert repeat == final
        before = shot.read_bytes()
        run([1, 2, 0xffff], frames=8, success=False, extra=['--capture', str(shot)])
        assert shot.read_bytes() == before

        before_clear, _, _ = run([1, 0x25bc, 2, 0xffff], capture_at=119)
        after_clear, result, _ = run([1, 0x25bc, 2, 0xffff], capture_at=140)
        assert before_clear.count((12, 16, 24, 255)) >= 576
        assert before_clear.count((128, 144, 160, 255)) == 0
        assert after_clear.count((12, 16, 24, 255)) == 0
        assert after_clear.count((128, 144, 160, 255)) == 576
        assert b'clears=1' in result.stdout + result.stderr
        run([1, 0x25bc, 2, 0xffff], frames=60, success=False)

        after_scroll, result, _ = run([1, 0xe000, 2, 0x25bd, 0xffff], capture_at=150)
        assert after_scroll.count((12, 16, 24, 255)) == 0
        assert after_scroll.count((128, 144, 160, 255)) == 576
        assert b'scrolled=16' in result.stdout + result.stderr

        delayed = [0xfffe, 0x201, 1, 4, 1, 0xffff]
        early, _, _ = run(delayed, frames=30, capture_at=0)
        late, _, _ = run(delayed, frames=30, capture_at=20)
        assert early.count((12, 16, 24, 255)) == 0
        assert late.count((12, 16, 24, 255)) == 576
        for invalid in ([0xfffe, 0x202, 1, 1, 0xffff], [0xfffe, 0x201, 1, 256, 0xffff],
                        [1], [0xfffe, 0x201, 1, 0xffff], [0xfffe, 0x205, 1, 1, 0xffff]):
            run(invalid, frames=2, success=False)
        run([1, 0xffff], frames=2, success=False, extra=['--title-flow'])
        run([1, 0xffff], frames=2, success=False, extra=['--title-resources', str(root)])
        assert subprocess.run([executable, '--text-flow', '--frames', '2'],
                              capture_output=True, timeout=30).returncode != 0
        font_path.write_bytes(b'NARC')
        run([1, 0xffff], frames=2, success=False)
        if args.default_executable:
            result = subprocess.run([str(args.default_executable.resolve()), '--text-flow',
                                     '--text-resources', str(root), '--frames', '2'],
                                    capture_output=True, timeout=30)
            assert result.returncode != 0
            assert b'HG_REFERENCE_TEXT_FLOW' in result.stdout + result.stderr
    print('PASS SDL original C text flow: progressive pixels, alignment, position/clipping, page clear, scroll, delay, input, Unicode and rejection')


if __name__ == '__main__':
    main()
