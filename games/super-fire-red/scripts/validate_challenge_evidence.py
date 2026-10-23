"""Check published evidence integrity, native completion and resource/permadeath ledgers."""
from pathlib import Path
import gzip,hashlib,json,collections,argparse
ROOT=Path(__file__).resolve().parents[3]
summary=json.loads((ROOT/'games/super-fire-red/hack-data/challenge_balance/summary.json').read_text(encoding='utf-8'))
assert len(summary['classifications'])==35
parser=argparse.ArgumentParser();parser.add_argument('--reference-rom',type=Path,default=ROOT/'games/super-fire-red/output/firered-386-dev.gba');args=parser.parse_args()
assert summary['rom_sha256']==hashlib.sha256(args.reference_rom.read_bytes()).hexdigest(), 'Use --reference-rom with the v0.4.0 ROM for this historical evidence.'
native=0;blocks=0
def visit(value):
 global native
 if isinstance(value,dict):
  if {'outcome','before','after','completed'}<=value.keys():
   assert value['completed'], 'unfinished native battle'
   assert value['outcome'] in (1,2,3),value['outcome']
   assert all(m['ev_total']<=510 and m['iv_total']<=186 for m in value['before'])
   native+=1
  for child in value.values():visit(child)
 elif isinstance(value,list):
  for child in value:visit(child)
for archive in summary['archives']:
 path=ROOT/archive['file'];packed=path.read_bytes();raw=gzip.decompress(packed)
 assert hashlib.sha256(packed).hexdigest()==archive['sha256']
 assert hashlib.sha256(raw).hexdigest()==archive['uncompressed_sha256']
 data=json.loads(raw)
 assert data['rom_sha256']==summary['rom_sha256']
 # Old fixtures are retained only for traceability, not revalidated as current evidence.
 if archive['dataset'].startswith(('early-','pickup-')) or archive['dataset']=='lab-tutorial':visit(data)
 if archive['dataset']=='lab-tutorial':
  assert len(data['rows'])==12
  assert all(m['ev_total']==0 for r in data['rows'] for a in r['attempts'] for m in a['before'])
 if archive['dataset'].startswith('pickup-'):
  for row in data['rows']:
   for trial in row['trials']:
    used=collections.Counter();dead=set()
    for battle in trial['battles']:
     result=battle['result'];used.update(result['between_items'])
     assert all(m['hp']==0 for m in result['before'] if m['species'] in dead)
     if row['mode']==1:dead.update(m['species'] for m in result['after'] if m['dead'])
    for item,count in used.items():assert count<=data['resource_budget_assumed'][item],(archive['dataset'],item,count)
    if row['mode']==1:assert used['REVIVE']==0
    if trial['won_block']:assert len(trial['battles'])==5 and all(b['result']['outcome']==1 for b in trial['battles'])
    blocks+=1
print(f'PASS: 35 classifications, archive hashes, {native} corrected/advanced native battles, {blocks} League block ledgers; reference ROM hash matches.')
