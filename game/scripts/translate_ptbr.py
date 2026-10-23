"""Create a resumable, local machine-translation draft of game text.

Uses the separately installed Argos en->pt model through CTranslate2. No text is
sent to a translation service. This draft still requires linguistic review.
"""
from pathlib import Path
import ast,json,re,time
import ctranslate2,sentencepiece
root=Path(__file__).resolve().parents[2];out=root/'game/hack-data/localization';out.mkdir(parents=True,exist_ok=True)
model=Path.home()/'.local/share/argos-translate/packages/translate-en_pt-1_9'
translator=ctranslate2.Translator(str(model/'model'),device='cpu',compute_type='int8',inter_threads=1,intra_threads=4)
tokenizer=sentencepiece.SentencePieceProcessor(model_file=str(model/'sentencepiece.model'))
cache_path=out/'mt_cache.json';cache=json.loads(cache_path.read_text(encoding='utf-8')) if cache_path.exists() else {}
literal=r'"(?:[^"\\]|\\.)*"'
def decode(value):
 try:return ''.join(ast.literal_eval(x) for x in re.findall(literal,value))
 except (SyntaxError,ValueError):return ''
catalog={}
excluded=['training_screen.c','species_names.h','move_names.h','trainer_class_names.h','pokedex_orders.h']
for path in [*(root/'game/src').rglob('*.c'),*(root/'game/src').rglob('*.h'),*(root/'game/data').rglob('*.inc')]:
 if path.name in excluded:continue
 source=path.read_text(encoding='utf-8',errors='replace');strings=[]
 if path.suffix=='.inc':
  pending=[]
  for line in source.splitlines()+['']:
   m=re.match(r'\s*\.string\s+('+literal+r')',line)
   if m:pending.append(decode(m[1]))
   elif pending:strings.append(''.join(pending));pending=[]
 else:strings=[decode(m[1]) for m in re.finditer(r'_\(\s*((?:'+literal+r'\s*)+)\)',source)]
 for text in strings:
  text=text.removesuffix('$')
  if re.search(r'[A-Za-z]{2}',text) and 1<len(text)<3000:catalog.setdefault(text,[]).append(str(path.relative_to(root/'game')))
proper={'POKéMON','POKEMON','OAK','BILL','CELIO','KANTO','JOHTO','HOENN','SEVII','RED','GREEN','BLUE','GARY','DAISY','LORELEI','BRUNO','AGATHA','LANCE','BROCK','MISTY','SABRINA','BLAINE','GIOVANNI','ERIKA','KOGA','LT. SURGE','TEAM ROCKET','S.S. ANNE'}
for path in [root/'game/src/data/text/species_names.h',root/'game/src/data/text/move_names.h']:
 if path.exists():proper.update(decode(m[1]) for m in re.finditer(r'_\(\s*('+literal+r')\s*\)',path.read_text(encoding='utf-8')))
proper.discard('');protected='|'.join(re.escape(x) for x in sorted(proper,key=len,reverse=True))
splitter=re.compile(r'(\{[^}]+\}|\\[lpn]|[\n\r]|\$|(?<![A-Za-z])(?:'+protected+r')(?![A-Za-z]))')
def parts(text):return splitter.split(text)
needed=sorted({part for text in catalog for i,part in enumerate(parts(text)) if i%2==0 and re.search(r'[A-Za-z]{2}',part) and part not in cache})
print('Catalog strings:',len(catalog),'new translation fragments:',len(needed),flush=True)
start=time.time()
for offset in range(0,len(needed),48):
 batch=needed[offset:offset+48]
 results=translator.translate_batch([tokenizer.encode(text,out_type=str) for text in batch],beam_size=2,max_batch_size=48,max_decoding_length=250)
 for text,result in zip(batch,results):cache[text]=tokenizer.decode(result.hypotheses[0])
 if offset%480==0 or offset+48>=len(needed):
  cache_path.write_text(json.dumps(cache,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
  print('Translated',min(offset+48,len(needed)),'/',len(needed),'elapsed',round(time.time()-start),'seconds',flush=True)
rows=[]
for text,paths in catalog.items():
 def translated(part):
  if part not in cache:return part
  return re.match(r'^\s*',part)[0]+cache[part].strip()+re.search(r'\s*$',part)[0]
 pt=''.join(translated(part) if i%2==0 else part for i,part in enumerate(parts(text)))
 rows.append({'en':text,'pt':pt,'sources':paths})
(out/'ptbr_draft.json').write_text(json.dumps({'status':'machine_translation_draft','model':'Argos translate-en_pt-1_9','source':'https://github.com/argosopentech/argos-translate','entries':rows},ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('Draft complete:',len(rows),'strings',flush=True)
