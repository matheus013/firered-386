"""Remove an invalid native-only frame dereference at animation sequence edges."""
import hashlib
from pathlib import Path

HASH = '35c33f780ee49752679df1380613c20720d5498df3f806522cf9b92300c7a0ff'


def prepare(source: Path, destination: Path):
    raw = source.read_bytes()
    if hashlib.sha256(raw).hexdigest() != HASH:
        raise ValueError('Animation controller source hash differs')
    text = raw.decode()
    old = '''    #ifdef SDK_PORT
    if( pAnimCtrl->pCurrent->frames == 0 || pAnimCtrl->pActiveCurrent->frames == 0 )
    {
        NNS_G2dStopAnimCtrl( pAnimCtrl );
        return;
    }
    #endif
'''
    if text.count(old) != 1:
        raise ValueError('Sequence edge contract differs')
    # MoveNext_ has already moved pCurrent outside the sequence here. The common
    # stop/reset logic below and subsequent ValidateAnimFrame_ handle this edge.
    destination.write_text(text.replace(old, ''))
