"""Compile the PT-BR draft and reviewed overrides into a ROM-only lookup table."""
from pathlib import Path
import ast,collections,json,re,unicodedata,argparse
parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true');args=parser.parse_args()
root=Path(__file__).resolve().parents[2];folder=root/'game/hack-data/localization'
chars={};constants={}
for line in (root/'game/charmap.txt').read_text(encoding='utf-8').splitlines():
 m=re.match(r"^'((?:\\.|[^'])*)'\s*=\s*((?:[0-9A-F]{2}(?:\s+|$))+)",line)
 if m:
  char=m[1].replace("\\'","'").replace('\\"','"');chars[char]=bytes.fromhex(m[2]);continue
 m=re.match(r'^([A-Z_0-9]+)\s*=\s*((?:[0-9A-F]{2}(?:\s+|$))+)',line)
 if m:constants[m[1]]=bytes.fromhex(m[2])
def encode(text):
 out=bytearray();i=0
 while i<len(text):
  if text[i]=='{':
   end=text.index('}',i);tokens=text[i+1:end].split()
   for token in tokens:
    if token in constants:out+=constants[token]
    else:
     n=int(token,16 if token.startswith('0x') else 10);width=max(1,(len(token)-2+1)//2) if token.startswith('0x') else 1 if n<256 else 2 if n<65536 else 4
     out+=n.to_bytes(width,'little')
   i=end+1
  elif text[i] in '\n\r':out+=chars['\\n'];i+=1
  elif text[i]=='\\' and i+1<len(text) and text[i:i+2] in chars:out+=chars[text[i:i+2]];i+=2
  else:out+=chars[text[i]];i+=1
 return bytes(out).split(b'\xff')[0]+b'\xff'
def supported(text):
 out=[]
 for fragment in re.split(r'(\{[^}]+\})',text):
  if fragment.startswith('{') and fragment.endswith('}'):out.append(fragment);continue
  for c in fragment:
   if c in chars or c in '\n\r\\{}':out.append(c)
   else:
    substitute={'–':'-','—':'-','\u00a0':' ','\t':' ','¹':'1','²':'2','³':'3'}.get(c)
    if substitute is None:substitute=''.join(x for x in unicodedata.normalize('NFKD',c) if not unicodedata.combining(x))
    out.append(substitute if substitute and all(x in chars for x in substitute) else '?')
 return ''.join(out)
font_source=(root/'game/src/text.c').read_text(encoding='utf-8')
normal_widths=list(map(int,re.findall(r'\d+',re.search(r'sFontNormalLatinGlyphWidths\[\]\s*=\s*\{(.*?)\};',font_source,re.S)[1])))
def dialog_width(text):
 width=0
 for part in re.split(r'(\{[^}]+\})',supported(text)):
  if part.startswith('{'):
   width+=42 if part in ('{PLAYER}','{RIVAL}') else 96 if part.startswith('{STR_VAR') else 0
  else:
   width+=sum(normal_widths[chars[c][0]] for c in part if c in chars)
 return width

def wrap_dialog(text):
 pages=re.split(r'\\p',text);result=[]
 for page in pages:
  page=page.replace('\\l',' ').replace('\n',' ')
  words=re.findall(r'(?:\{[^}]+\}|[^\s{}])+',page);lines=[];line=[];length=0
  for word in words:
   width=dialog_width(word)
   if line and length+6+width>204:lines.append(' '.join(line));line=[];length=0
   line.append(word);length+=width+(6 if length else 0)
  if line:lines.append(' '.join(line))
  result.append(''.join(('' if i==0 else '\n' if i==1 else '\\l')+line for i,line in enumerate(lines)))
 return '\\p'.join(result)
def fnv(data):
 value=2166136261
 for byte in data[:-1]:value=((value^byte)*16777619)&0xffffffff
 return value
draft=json.loads((folder/'ptbr_draft.json').read_text(encoding='utf-8'))
overrides=json.loads((folder/'ptbr_overrides.json').read_text(encoding='utf-8'))
rows={row['en']:row for row in draft['entries']}
for row in rows.values():row['sources']=[p.replace('\\','/') for p in row['sources']]
protected=set()
# Character names and competitive identifiers must not become ordinary words.
for row in rows.values():
 if any(p in ('src/data/trainers.h','src/trainer_tower_sets.c','src/data/text/nature_names.h') for p in row['sources']):protected.add(row['en'])
ability_source=(root/'game/src/data/text/abilities.h').read_text(encoding='utf-8')
protected.update(ast.literal_eval(m) for m in re.findall(r'_\(\s*("(?:[^"\\]|\\.)*")\s*\)',ability_source[ability_source.index('const u8 gAbilityNames'):]))
for en in protected:
 if en in rows:rows[en]['pt']=en
for file in ('ui_reviewed_ptbr.json','ability_descriptions_ptbr.json','story_reviewed_ptbr.json','battle_reviewed_ptbr.json','services_reviewed_ptbr.json'):
 overrides.update(json.loads((folder/file).read_text(encoding='utf-8')))
moves=json.loads((folder/'move_descriptions_ptbr.json').read_text(encoding='utf-8'))
move_source=(root/'game/src/move_descriptions.c').read_text(encoding='utf-8')
descriptions=dict((name,ast.literal_eval(literal)) for name,literal in re.findall(r'gMoveDescription_(\w+)\[\]\s*=\s*_\(\s*("(?:[^"\\]|\\.)*")\s*\)',move_source))
assert set(moves)==set(descriptions),'All implemented move descriptions must be reviewed'
for name,pt in moves.items():overrides[descriptions[name]]=pt
item_texts=json.loads((folder/'item_descriptions_ptbr.json').read_text(encoding='utf-8'))
items=json.loads((root/'game/src/data/items.json').read_text(encoding='utf-8'))['items']
def wrap_description(text):
 lines=[];line=''
 for word in text.split():
  candidate=(line+' '+word).strip()
  if line and dialog_width(candidate)>198:lines.append(line);line=word
  else:line=candidate
 if line:lines.append(line)
 if len(lines)>3:raise ValueError('Item description exceeds three lines: '+text)
 return '\n'.join(lines)
for item in items:
 if item['itemId'] in item_texts:
  en=item['description_english'].replace('\\n','\n')
  rows.setdefault(en,{'en':en,'pt':en,'sources':['src/data/items.json']})
  overrides[en]=wrap_description(item_texts[item['itemId']])
for en,pt in overrides.items():rows[en]={'en':en,'pt':pt,'sources':rows.get(en,{}).get('sources',[]),'reviewed':True}
accepted={};skipped=[]
direct_pt=set()
for path in (root/'game/src').rglob('*.c'):
 source=path.read_text(encoding='utf-8')
 for match in re.finditer(r'\b\w+Pt\[\]\s*=\s*_\(\s*("(?:[^"\\]|\\.)*")\s*\)',source):
  direct_pt.add(ast.literal_eval(match[1]))
 for block in re.findall(r'\b\w+Pt\[\]\[\d+\]\s*=\s*\{(.*?)\};',source,re.S):
  direct_pt.update(ast.literal_eval(value) for value in re.findall(r'_\(\s*("(?:[^"\\]|\\.)*")\s*\)',block))
def clean(pt,en):
 # SentencePiece word markers (U+2581) leaked into the machine draft; restore plain spacing.
 pt=re.sub(r' {2,}',' ',pt.replace('▁',' '))
 pt=re.sub(r' *\n *','\n',pt)
 for old,new in {'utilizador':'usuário','utilizadores':'usuários','equipa':'equipe','equipas':'equipes','ficheiro':'arquivo','ficheiros':'arquivos','ecrã':'tela','relva':'grama','rapariga':'menina','registar':'registrar','registo':'registro','afectado':'afetado','afectada':'afetada','contacto':'contato','óptimo':'ótimo','óptima':'ótima','protecção':'proteção','controlo':'controle'}.items():
  pt=re.sub(r'\b'+old+r'\b',new,pt,flags=re.I)
 if re.search(r'\bparty\b',en,re.I):pt=re.sub(r'\bfesta\b','equipe',pt,flags=re.I)
 return en[:len(en)-len(en.lstrip(' '))]+pt.strip(' ')+en[len(en.rstrip(' ')):]
wrapped=0
for row in rows.values():
 en,pt=row['en'],row['pt']
 if not row.get('reviewed'):pt=clean(pt,en)
 if en==pt:continue
 if en in direct_pt:continue
 try:
  en_bytes=encode(en)
  if any(p.startswith('data/maps/') for p in row['sources']) and ('\n' in en or '\\l' in en or '\\p' in en):pt=wrap_dialog(pt);wrapped+=1
  pt_bytes=encode(supported(pt))
  if len(pt_bytes)>900:raise ValueError('translation exceeds safe message budget')
  if collections.Counter(re.findall(r'\{[^}]+\}',en))!=collections.Counter(re.findall(r'\{[^}]+\}',pt)):raise ValueError('placeholder mismatch')
  accepted[en_bytes]=(pt_bytes,en,pt,bool(row.get('reviewed')))
 except (KeyError,ValueError,OverflowError) as error:
  if row.get('reviewed'):raise ValueError(f'Invalid reviewed translation: {en!r}: {error}') from error
  skipped.append({'en':en,'reason':str(error)})
entries=sorted(accepted.items(),key=lambda pair:(fnv(pair[0]),pair[0]));code=['// Generated by scripts/generate_locale.py; PT-BR draft with reviewed overrides.']
for i,(en,(pt,*_)) in enumerate(entries):
 code+=['static const u8 sLocaleEn%d[] = {%s};'%(i,','.join(map(str,en))),'static const u8 sLocalePt%d[] = {%s};'%(i,','.join(map(str,pt)))]
code+=['static const struct LocaleEntry sPtLocale[] = {']+[f'    {{0x{fnv(en):08X},sLocaleEn{i},sLocalePt{i}}},' for i,(en,_) in enumerate(entries)]+['};']
def output(path,text):
 if args.check:
  if path.read_text(encoding='utf-8')!=text:raise SystemExit(f'Out of date: {path}')
 else:path.write_text(text,encoding='utf-8',newline='\n')
output(root/'game/src/data/localization_ptbr.h','\n'.join(code)+'\n')
report={'status':'ptbr_beta_reviewed_gameplay_with_machine_draft_dialogue','catalog_strings':len(rows),'translated_entries':len(entries),'reviewed_overrides':sum(v[3] for v in accepted.values()),'reviewed_move_descriptions':len(moves),'reviewed_item_descriptions':len(item_texts),'protected_identifiers':len(protected),'wrapped_map_dialogues':wrapped,'fallback_unchanged':len(rows)-len(entries)-len(skipped),'skipped':skipped,'font_policy':'Unsupported accented glyphs (including a/o with tilde) are transliterated; supported accents, controls and placeholders preserved.','linguistic_review_complete':False}
output(folder/'coverage.json',json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print('Generated',len(entries),'translated entries;',len(skipped),'fallback entries')
