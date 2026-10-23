"""Check compiled PT-BR coverage, source keys, control tokens and text budgets."""
from pathlib import Path
import ast,collections,hashlib,json,re,runpy,sys,warnings
warnings.filterwarnings('ignore',category=SyntaxWarning)
root=Path(__file__).resolve().parents[3]
sys.argv=['generate_locale.py','--check'];locale=runpy.run_path(str(root/'games/super-fire-red/scripts/generate_locale.py'))
accepted=locale['accepted'];encode=locale['encode'];supported=locale['supported']
literal=r'"(?:[^"\\]|\\.)*"';corpus=set()
def decode(value):return ''.join(ast.literal_eval(x) for x in re.findall(literal,value)).removesuffix('$')
for path in (root/'games/super-fire-red/src').rglob('*'):
 if path.suffix not in ('.c','.h') or path.name=='localization_ptbr.h':continue
 corpus.update(decode(m) for m in re.findall(r'_\(\s*((?:'+literal+r'\s*)+)\)',path.read_text(encoding='utf-8')))
for path in (root/'games/super-fire-red/data').rglob('*'):
 if path.suffix not in ('.inc','.s'):continue
 pending=[]
 for line in path.read_text(encoding='utf-8').splitlines()+['']:
  m=re.match(r'\s*\.string\s+('+literal+r')',line)
  if m:pending.append(ast.literal_eval(m[1]))
  elif pending:corpus.add(''.join(pending).removesuffix('$'));pending=[]
items=json.loads((root/'games/super-fire-red/src/data/items.json').read_text(encoding='utf-8'))['items']
abilities=[ast.literal_eval(m) for m in re.findall(r'static const u8 \w+Description\[\]\s*=\s*_\(\s*('+literal+r')\s*\)',locale['ability_source'])]
assert all(en in locale['overrides'] for en in abilities),'An ability description has no reviewed translation'
corpus.update(i['description_english'].replace('\\n','\n') for i in items)
missing=[];checks=0
for file in ('ui_reviewed_ptbr.json','ability_descriptions_ptbr.json','story_reviewed_ptbr.json','battle_reviewed_ptbr.json','services_reviewed_ptbr.json'):
 for en,pt in json.loads((root/'games/super-fire-red/hack-data/localization'/file).read_text(encoding='utf-8')).items():
  if en not in corpus:missing.append({'file':file,'en':en})
  assert collections.Counter(re.findall(r'\{[^}]+\}',en))==collections.Counter(re.findall(r'\{[^}]+\}',pt)),en
  key=encode(en)
  # More specific review files intentionally override earlier wording.
  assert key in accepted or en==pt or en in locale['direct_pt'],en
  checks+=1
for key,(pt,en,text,reviewed) in accepted.items():
 assert b'\xff' not in key[:-1] and b'\xff' not in pt[:-1]
 assert '▁' not in text
 assert collections.Counter(re.findall(r'\{[^}]+\}',en))==collections.Counter(re.findall(r'\{[^}]+\}',text)),en
 assert len(pt)<=900
 if 'src/battle_message.c' in locale['rows'][en]['sources']:
  # Reserve room for expanded names/stat buffers in the 300-byte display area.
  expanded=len(pt)+27*len(re.findall(r'\{B_(?:ATK|DEF|EFF|SCR_ACTIVE|ACTIVE)_NAME_WITH_PREFIX\}',text))+28*len(re.findall(r'\{B_BUFF[123]\}',text))
  assert expanded<300,('battle message budget',en,expanded)
for item in items:
 if item['itemId'] not in locale['item_texts']:continue
 key=encode(item['description_english'].replace('\\n','\n'))
 assert key in accepted,item['itemId']
 pt=accepted[key][2]
 assert len(pt.splitlines())<=3 and all(locale['dialog_width'](line)<=198 for line in pt.splitlines()),item['itemId']
report={'status':'passed' if not missing else 'failed','rom_sha256':hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest(),
 'reviewed_keys_checked':checks,'compiled_entries_checked':len(accepted),'item_descriptions_checked':len(locale['item_texts']),'ability_definitions_checked':len(abilities),
 'missing_source_keys':missing,'scope':'source-key coverage, compiled lookup entries, placeholder preservation and item text budgets; move width tested natively in test_category_ui.py',
 'linguistic_review_complete':False}
out=root/'games/super-fire-red/output/tests/localization';out.mkdir(parents=True,exist_ok=True)
(out/'results.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps(report,ensure_ascii=True,indent=2))
assert not missing,'Reviewed keys must match actual game strings'
