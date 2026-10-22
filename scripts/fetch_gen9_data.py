"""Pin and download PokeAPI's version-specific data, never execute remote code."""
from pathlib import Path
import hashlib,json,urllib.request
ROOT=Path(__file__).resolve().parents[1];DEST=ROOT/'data/upstream/pokeapi';DEST.mkdir(parents=True,exist_ok=True)
def fetch(url):
 req=urllib.request.Request(url,headers={'User-Agent':'firered-386-data-import'})
 with urllib.request.urlopen(req,timeout=120) as response:return response.read()
manifest=DEST/'manifest.json'
if manifest.exists():commit=json.loads(manifest.read_text())['commit']
else:commit=json.loads(fetch('https://api.github.com/repos/PokeAPI/pokeapi/commits/master'))['sha']
files={}
for name in ['moves','move_names','pokemon_moves','version_groups','versions','machines','pokemon','pokemon_species','move_changelog']:
 url=f'https://raw.githubusercontent.com/PokeAPI/pokeapi/{commit}/data/v2/csv/{name}.csv';path=DEST/(name+'.csv')
 if not path.exists():path.write_bytes(fetch(url))
 files[path.name]={'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'url':url,'bytes':path.stat().st_size}
 print(path.name,path.stat().st_size,flush=True)
manifest.write_text(json.dumps({'repository':'https://github.com/PokeAPI/pokeapi','commit':commit,'files':files},indent=2)+'\n')
