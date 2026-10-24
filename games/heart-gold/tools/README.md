# Ferramentas

Ferramentas de build e validação. Rodam na máquina de desenvolvimento ou no CI, **nunca** dentro do jogo.

| Ferramenta | O que faz |
|---|---|
| `check_assets.py` | Valida `assets/manifest.json` (autor, licença, origem) e recusa arquivos fora do manifesto ou com extensão de dados de ROM. Roda no CTest como `assets.manifest` quando há Python 3. |

Planejado: conversores de arte e áudio próprios para os formatos do jogo e validadores dos dados de região.
