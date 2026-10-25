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
                        [1], [0xfffe, 0x201, 1, 0xffff]):
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
    print('PASS SDL original C text flow: progressive pixels, page clear, scroll, delay, input, Unicode and rejection')


if __name__ == '__main__':
    main()
