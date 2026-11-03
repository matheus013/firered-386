"""Regression: filtering handler names must never renumber script opcodes."""
import unittest
from verify_map_events import command_table


class CommandTableTests(unittest.TestCase):
    def test_nonstandard_handler_keeps_its_slot(self):
        source = 'gScriptCmdTable[] = { ScrCmd_A, OtherHandler, /* note */ ScrCmd_B, // note\n };'
        self.assertEqual(command_table(source), ['ScrCmd_A', 'OtherHandler', 'ScrCmd_B'])

    def test_unknown_initializer_is_rejected(self):
        with self.assertRaises(ValueError):
            command_table('gScriptCmdTable[] = { [3] = Handler, };')


if __name__ == '__main__':
    unittest.main()
