# Seleção dos bancos de save

`game/core/src/save_selection.cpp` implementa em C++20 a política de seleção dos
dois bancos principais de HG/SS. A entrada contém somente validade e contador já
obtidos por um validador separado, para os chunks normal e PC de cada banco.
Essa função não lê arquivos, verifica CRC, valida os demais registros, recupera
dados nem escreve saves.

O resultado distingue `NoSave`, `Corrupt`, `Recovered` e `Good`, e informa banco,
contador e `selected_chunks_valid`. `Recovered` descreve a escolha de fallback da
política; não indica que um arquivo tenha sido reparado.

## Regras e limites

- Empate de contadores favorece o banco 0.
- O contador 0 sucede `UINT32_MAX`; os demais pares usam comparação numérica.
  Não se aplica uma comparação modular genérica.
- Contadores de chunks inválidos são tratados como zero, como no original.
- A seleção considera a consistência entre os chunks normal e PC por banco.
- A asserção original para o único par válido no mesmo banco com contadores
  diferentes torna-se `Corrupt`, evitando interromper o aplicativo.

Existe uma particularidade original preservada para auditoria: com dois chunks
normais válidos e um PC válido, um contador normal zero pode coincidir com o zero
artificial de um PC inválido. O original então pode selecionar esse banco como
`Good`. O resultado expõe `selected_chunks_valid = false`; um consumidor que
pretenda carregar dados **deve rejeitar essa seleção**. O status isolado não é
evidência de integridade de um save real.

## Verificação reproduzível contra o original

Base de referência: `ClickingMaster/pokeheartgold-single-screen`, commit
`6984a3d67592157195940049be2e4bbbf933f9ff`.

SHA-256 de `src/save.c`:
`4eed1844a349b1b761f51ee41b53922c1edf0095de2d88c8ed1a31108e545584`.

O verificador exige uma cópia local desse fonte e um compilador host GCC/Clang
C++20. Ele confere o hash, extrai as quatro funções originais de comparação e
seleção, fornece fixtures de IO/validade e compara o resultado com nossa função.
Não acessa saves. Os fontes extraídos e executáveis ficam somente em diretório
temporário fora de qualquer repositório Git e são apagados ao encerrar.

Exemplo no WSL, a partir da raiz do monorepo:

```bash
python3 games/super-heartgold/tools/verify_save_selection_reference.py \
  --source /root/.cache/super-heartgold/reference/src/save.c
```

Para outro compilador, use `--compiler clang++`. `--project` permite indicar
explicitamente a pasta `games/super-heartgold`.

São 38.416 combinações: as 16 máscaras de validade dos quatro chunks, cruzadas
com os contadores `0`, `1`, `2`, `0x7fffffff`, `0x80000000`, `0xfffffffe` e
`0xffffffff` em cada chunk. A comparação inclui status, banco, contador e a
validade dos chunks selecionados; a asserção original citada é normalizada para
`Corrupt`. O teste cobre esse conjunto finito, sem representar validação de toda
combinação possível de 32 bits ou de uma campanha jogável.

Os testes unitários `tests/save_selection_tests.cpp` verificam também as saídas
sem banco, a prioridade de empate, fallback, wrap e a independência de contadores
fornecidos para chunks inválidos. Passaram com GCC e AddressSanitizer/UBSan.

Saída observada em 24/10/2026, GCC no WSL:

```text
Exact original selection differential: 38416 cases passed
Original assertions normalized to Corrupt: 4116
Original selections with an invalid chosen chunk, explicitly exposed: 609
```

Os 609 casos são fixtures sintéticas dessa grade, não ocorrências em saves de
usuários. Demonstram por que a validade dos chunks escolhidos precisa de uma
verificação adicional antes de carregar dados.
