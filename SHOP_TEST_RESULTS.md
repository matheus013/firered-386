# Regressão de lojas — v0.3.0-dev

Verificada em 21/10/2026, com ROMs descartáveis na biblioteca do mGBA. Nenhum save do usuário foi aberto ou modificado.

## Falha e correção

O estoque ampliado de Rare Candy era copiado para o heap. Ao retornar de compra/venda ao mapa, FireRed executa `MoveSaveBlocks_ResetHeap`, que destrói esse heap. A loja mantinha uma referência inválida para reconstruir o menu e liberá-la ao sair. O código anterior da loja reproduziu falha de opcode ao cancelar e sair de Viridian, antes mesmo de exigir uma compra.

Agora o estoque mantém o ponteiro para a lista original na ROM. O Rare Candy adicional é acessado como uma entrada virtual no Hardcore, sem alocação persistente no heap. A saída não tenta liberar um estoque destruído. A tela de TMs também deixa de formatar o Rare Candy como número de TM.

## Cobertura

Todos os 23 catálogos usados pelo comando `pokemart` foram extraídos automaticamente dos scripts dos mapas. Cada catálogo foi aberto no mapa correspondente em Normal e Hardcore: **46 casos, todos aprovados**. Inclui os cinco balcões de Celadon, a loja da Torre e os quatro estágios de Two Island.

Cada caso usa os botões da interface real para cancelar uma compra, comprar um item, vender uma Potion, sair por B e por SEE YA e reabrir a loja. Confere preço/quantidade, estoque completo preservado, retomada do script e integridade do heap após três ciclos de abertura/saída. No Hardcore, percorre o estoque e compra o Rare Candy adicional por ₽1, inclusive no balcão de TMs.

A fixture chama a rotina normal da loja com o estoque verdadeiro e usa um script de atendimento de teste, para não depender de requisitos de história de cada vendedor. Não é um teste completo das quests de acesso. Máquinas de bebida, troca de moedas/prêmios e vendedores por diálogo não usam o comando `pokemart` nem esse estoque e não estão incluídos nestes 46 casos. O VBA Link não foi automatizado neste teste.

| Catálogo | Normal | Hardcore |
|---|---|---|
| `CeladonCity_DepartmentStore_2F_Items` | Passou | Passou |
| `CeladonCity_DepartmentStore_2F_TMs` | Passou | Passou |
| `CeladonCity_DepartmentStore_4F_Items` | Passou | Passou |
| `CeladonCity_DepartmentStore_5F_XItems` | Passou | Passou |
| `CeladonCity_DepartmentStore_5F_Vitamins` | Passou | Passou |
| `CeruleanCity_Mart_Items` | Passou | Passou |
| `CinnabarIsland_Mart_Items` | Passou | Passou |
| `FourIsland_Mart_Items` | Passou | Passou |
| `FuchsiaCity_Mart_Items` | Passou | Passou |
| `IndigoPlateau_PokemonCenter_1F_Items` | Passou | Passou |
| `LavenderTown_Mart_Items` | Passou | Passou |
| `PewterCity_Mart_Items` | Passou | Passou |
| `SaffronCity_Mart_Items` | Passou | Passou |
| `SevenIsland_Mart_Items` | Passou | Passou |
| `SixIsland_Mart_Items` | Passou | Passou |
| `ThreeIsland_Mart_Items` | Passou | Passou |
| `TrainerTower_Lobby_Mart_Items` | Passou | Passou |
| `TwoIsland_Items_ShopInitial` | Passou | Passou |
| `TwoIsland_Items_ShopExpanded1` | Passou | Passou |
| `TwoIsland_Items_ShopExpanded2` | Passou | Passou |
| `TwoIsland_Items_ShopExpanded3` | Passou | Passou |
| `VermilionCity_Mart_Items` | Passou | Passou |
| `ViridianCity_Mart_Items` | Passou | Passou |

Repetir após compilar: `python3 scripts/test_shops.py` no Ubuntu/WSL com `libmgba-dev`. Logs/capturas ficam em `output/tests/shops/`. Resumo versionado: [data/shop_test_results.json](data/shop_test_results.json).
