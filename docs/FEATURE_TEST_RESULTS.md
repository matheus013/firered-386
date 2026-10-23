# Validação das funcionalidades — v0.4.0-dev

Executada em 22/10/2026 com a ROM SHA-256 `881869fbaa37abe13becbb071759ff1225984910fdb410d11818e88b56a0990d`. As sete etapas da suíte de funcionalidades passaram; o binário de produção ficou congelado durante a execução. [Resultados estruturados](../data/feature_tests/features.json).

| Área | Verificação e resultado |
|---|---|
| Cientista | Interação real com o NPC, retrato/painéis, natureza/EV/IV, cancelar/confirmar, sair/reabrir, bloqueios e três curas/quarta recusa. Passou em EN e PT-BR. Imagens revisadas visualmente. |
| Opções e novo save | Botões reais no menu de idioma e no fluxo modo/regras/randomizer; B retorna; Hardcore mantém SET. Passou. |
| Randomizer | Permutação das 386 espécies, faixas e lendários, três iniciais distintos, variáveis do laboratório, coerência com a semente e preservação de níveis. 1.880 Pokémon de treinadores aleatórios e 3.760 verificações de nível nos dois estados. Passou. |
| Captura aleatória | Primeiro elegível, cota já usada, duplicata em nova área sem consumir a cota, próximo elegível e proteção de lendários antes da Liga. Passou no fluxo nativo de geração/cota. |
| Treino dos desafios | Todos os elencos regulares enumerados: 241 Pokémon de desafios por estado do randomizer, com seis IVs 31 e EVs válidos; E4/Campeão também na suíte dedicada. Passou. |
| Torre | 104 adversários criados pelo construtor nativo por estado do randomizer, nos quatro cursos e oito andares; IVs, EVs, espécies e níveis. Passou. Não equivale a terminar as lutas da Torre nesta versão. |
| Localização/save | Todas as 7.771 entradas localizadas, padrões de save antigo, preservação do modo/contadores e gravação nativa com recarga em nova instância. Passou. Não comprova qualidade linguística de todo o texto. |
| Lojas | 23 catálogos × Normal/Hardcore × EN/PT-BR = 92 casos: cancelar, comprar, vender, sair com B/SEE YA, reabrir, estoque exato, Candy=1 no Hardcore, heap e retorno do script. Passou. |
| Regressão | Sete suítes: E4, encontros, Hardcore, treinamento, interface, batalhas de treinadores e lojas. Mais 23 casos de captura e 28 de Pokémon/creche. Passaram. |

A reconstrução a partir da base fixa com o patch cumulativo gerou ROM idêntica. O IPS também é reaplicado à base limpa para conferir igualdade com o `.gba` distribuído. [Regras e limitações](SETTINGS.md).

## Como repetir

Após compilar no Ubuntu/WSL com libmgba-dev e compilador C:

```sh
python3 scripts/run_feature_validation.py
python3 scripts/audit_release.py --version v0.4.0-dev
```

As fixtures são cópias descartáveis, com preparação de mapas/equipes para isolar os casos, controle por botões e inspeção da memória. Testes de dados invocam construtores/funções reais. Não alteram a ROM pública nem o save do jogador. [Log nativo dos dados e recarga](../data/feature_tests/settings-native.log), [regressões](../data/feature_tests/regression.json), [lojas EN](../data/feature_tests/shops-en.json), [lojas PT-BR](../data/feature_tests/shops-ptbr.json).

## Pendências

Campanha completa do começo ao fim **sem teleporte, equipes fornecidas ou recursos injetados** ainda não homologada em Normal nem Hardcore. O relatório de 114 checkpoints é [histórico da v0.3.1-dev](CAMPAIGN_TEST_RESULTS.md) e assistido. Os testes atuais não certificam balanceamento global, todas as traduções em suas telas, trocas/link do VBA ou saves reais de todas as versões anteriores. O português permanece beta; novas mecânicas modernas ainda pendentes continuam listadas no README.

Na primeira execução, duas falhas eram da fixture: registrar captura sem os bits de visto exigidos pela Pokédex e apertar A durante a transição automática da loja, reabrindo Comprar. Os testes foram corrigidos e toda a suíte foi repetida com o mesmo SHA de produção.
