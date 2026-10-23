# Assets

Conteúdo **próprio** do jogo: arte, áudio, fontes e textos feitos para o Super HeartGold ou com licença
compatível. **Nunca** coloque aqui nada extraído de ROM (gráficos, músicas, textos, mapas, modelos), nem ROM,
BIOS ou firmware.

## Pipeline (esqueleto)

```
assets/
  manifest.json      lista de todos os assets, com autor, licença e origem
  source/            arquivos-fonte editáveis (ainda vazio)
```

1. Todo arquivo em `assets/source/` precisa ter uma entrada em `manifest.json`.
2. Toda entrada precisa de `path`, `author`, `license` e `origin`. `origin` só aceita `original`
   (feito para o projeto) ou `third-party` (com licença que permita redistribuição).
3. `tools/check_assets.py` valida as regras acima e roda no CTest (`assets.manifest`).

Ainda não há conversão (empacotamento, atlas, compressão). Ela vem com o sistema de recursos (M2 no roadmap).

## Por que não usar os dados originais

Os dados de HG/SS pertencem à Nintendo/Game Freak/Creatures e as decompilações não têm licença
(ver `docs/porting-strategy.md`). O jogo precisa rodar sem ROM; por isso todo o conteúdo tem que nascer aqui.
