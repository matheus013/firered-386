"""Test the harness parser without importing/running its ROM build driver."""
import ast
from pathlib import Path
import re
import tempfile
import unittest


class DialogueParserTests(unittest.TestCase):
    def test_labels_bound_the_search(self):
        script = Path(__file__).resolve().parents[1] / 'games/super-fire-red/scripts/test_region_pilot_maps.py'
        tree = ast.parse(script.read_text(encoding='utf-8'))
        node = next(n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name == '_pilot_texts')
        namespace = {'re': re}
        exec(compile(ast.Module(body=[node], type_ignores=[]), str(script), 'exec'), namespace)
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            (path / 'scripts.inc').write_text(
                'NoMessage::\n' + '\tsetvar VAR_HOENN_STORY, 2\n' * 200
                + '\tend\n\nWithMessage::\n\tlock\n\tmsgbox Test_Text_Hello, MSGBOX_DEFAULT\n'
                + '\tend\n\nTest_Text_Hello::\n\t.string "Hello!$"\n', encoding='utf-8')
            self.assertEqual(namespace['_pilot_texts'](path), {'WithMessage': 'Hello!$'})


if __name__ == '__main__':
    unittest.main()
