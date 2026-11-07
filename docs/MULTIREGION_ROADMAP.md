# Super FireRed: Johto e Hoenn como prioridade

Pesquisa de 07/11/2026. Escopo autorizado: manter Super FireRed e acrescentar os mapas
completos de Johto e Hoenn. A preservação anterior dos mapas deixa de ser uma restrição
à expansão. Não implica remover Kanto ou trocar a engine por Emerald.

## Fontes verificadas no GitHub

| Fonte | Evidência | Uso proposto |
|---|---|---|
| [pret/pokeemerald](https://github.com/pret/pokeemerald) | 518 arquivos `data/maps/*/map.json`; Littleroot possui 3 warps e conexão com Route101 | Fonte principal de Hoenn, incluindo layouts, tilesets e eventos originais |
| [resetes12/HNS_modern](https://github.com/resetes12/HNS_modern) | 957 arquivos de mapas; NewBarkTown possui 8 warps e conexões com Route29 e Route27; Ecruteak e interiores presentes | Candidato principal de Johto já adaptada ao GBA |
| [Marcogn/pokehns-expansion-plus](https://github.com/Marcogn/pokehns-expansion-plus) | 1499 arquivos de mapas, incluindo interiores Ecruteak com sufixo `_hns` | Alternativa mais ligada ao pokeemerald-expansion; avaliar dependências adicionais |
| [Eemeliri/soulgold](https://github.com/Eemeliri/soulgold) | 1106 arquivos de mapas, incluindo Ecruteak e interiores | Alternativa com alterações próprias de Johto |
| [HoboAstronaut/pokeemerald-definitive](https://github.com/HoboAstronaut/pokeemerald-definitive) | README descreve exteriores de Johto em desenvolvimento | Referência multirregional, não prova de Johto completa |

Contagens abrangem TODOS os mapas dos repositórios, não apenas uma região. Não
comprovam completude, qualidade nem funcionamento no Super FireRed. Foram conferidos
metadados e exemplos de mapas, não percorridas campanhas.

Pins consultados:

- pret/pokeemerald: `731ad5bfd6e6f265508d0efcca0ba42f9dcf5881`
- resetes12/HNS_modern: `bf25b6ecd23c009b899da570d62a210f35e01809`
- Marcogn/pokehns-expansion-plus: `85b0a462262cb9e1f7e028f872859b003bda0e58`
- Eemeliri/soulgold: `3f4d90aa8f8bc50054c817d7bd59fcdfcfb4933d`

A API não informou licença SPDX para essas fontes. Antes de incorporar contribuições
de fãs, conferir as condições e créditos de cada recurso; disponibilidade pública não
significa licença irrestrita. Nenhum desses recursos foi incorporado nesta etapa.

## Ordem de implementação

### Isolamento e política de builds

- Branch de trabalho: `codex/firered-johto-hoenn`, criada a partir do `main` local
  em `be492c2`. Nenhum merge ou push para `main` nesta etapa.
- Não gerar builds ROM/PC, releases ou atualizar downloads durante o inventário
  e desenvolvimento inicial. Nesta fase executar apenas validadores de dados.
- Antes da conclusão haverá necessidade de builds locais de validação; somente
  nessa etapa, em diretório isolado, sem substituir builds distribuídas ou saves.
- Publicar a versão final apenas após os critérios abaixo e revisão da integração.
- Não há workflows ativos em `.github/workflows` nesta branch após a retirada
  dos workflows HeartGold. O workflow aninhado do FireRed não é executado pelo GitHub.

| Marco | Entrega verificável | Estado |
|---|---|---|
| M0 | Branch isolada, escopo e fontes fixadas | Concluído |
| M1 | Inventário técnico, classificação regional e matriz de dependências | Iniciado: inventário técnico gerado; classificação e scripts pendentes |
| M2 | Conversão piloto de New Bark/Route29 e Littleroot/Route101 | Iniciado: blocos e bordas convertidos isoladamente; gráficos/eventos pendentes |
| M3 | Geografia completa de Johto, incluindo interiores/cavernas | Pendente |
| M4 | Geografia completa de Hoenn, incluindo interiores/cavernas | Pendente |
| M5 | Transporte, Fly, cura, mapa regional, IDs e save compatíveis | Pendente |
| M6 | Eventos, encontros, desafios e progressão interligados | Pendente |
| M7 | Builds locais isoladas e percurso completo sem teleportes | Pendente |
| M8 | Revisão final, integração em main e publicação | Pendente |

### Evidência inicial

O script `scripts/audit_region_maps.py` gerou os inventários
[Hoenn](map-audits/hoenn-source.json) e [candidato Johto](map-audits/johto-source.json).
Verifica existência dos layouts/borders/blocos, tamanho dos blocos e resolução dos
destinos estáticos. Warps dinâmicos ficam registrados para revisão de scripts.
Resultado: 518/957 mapas inventariados, 13/100 IDs coincidentes com FireRed,
nenhum problema nesses checks limitados. Isso não comprova a compatibilidade dos
tilesets nem a ausência de erros de scripts, índices de warp ou campanha.
Um teste sintético verifica detecção de arquivos ausentes, tamanho incorreto,
destino inexistente, conflito de IDs e aceitação do sentinel MAP_DYNAMIC.

As fontes foram clonadas fora do repositório em `../map-references/`.
Nenhum mapa/asset original foi copiado para o Super FireRed nesta etapa.

### Conversão piloto iniciada

`scripts/prepare_region_pilot.py` converte blocos/bordas para a partição de metatiles
do FireRed sem alterar colisão ou elevação. Emerald usa partição 512; FireRed e
HNS_modern usam 640. Foi executado em NewBarkTown, Route29, LittlerootTown e Route101.
Os arquivos intermediários ficam em `.build/region-pilots/` (ignorados pelo Git);
não são builds, mapas registrados na engine nem mods jogáveis.

Relatórios com hashes e dependências: [Johto piloto](map-audits/johto-pilot.json) e
[Hoenn piloto](map-audits/hoenn-pilot.json). Três testes aprovados: preservação das
64 combinações de colisão/elevação, identidade/sentinel e rejeição de dados
truncados ou IDs que excedem a partição. O conversor não deve ser executado duas
vezes sobre um resultado: lê exclusivamente o checkout fonte e exige saída nova.

Próximo passo: converter metatiles, paletas, atributos e animações dos tilesets
`Johto_General/NewBarkTown` e `General/Petalburg`. Ainda não importar scripts
diretamente: flags/variáveis e constantes pertencem a engines diferentes.

1. Inventariar mapas de cada região: exteriores, interiores, cavernas, andares,
   áreas especiais e mapas auxiliares. Registrar inclusões e exclusões explicitamente.
2. Fazer pilotos New Bark + Route29 e Littleroot + Route101, com ao menos um interior
   e uma conexão de retorno em cada região. Primeiro validar dados; testar em GBA e PC
   na etapa final de builds locais isoladas, respeitando a política acima.
3. Importar geografia completa de Johto, depois Hoenn, por grupos de mapas com
   validação automática de destinos de warps, conexões e arquivos referenciados.
4. Integrar viagens regionais, mapa de navegação, Fly, locais de cura/respawn e saves.
5. Integrar eventos, NPCs, treinadores, encontros e progressão. Mapas caminháveis
   não equivalem a campanhas completas de Gold/Silver/Emerald.
6. Testar percurso sem teleportes, entrada/saída de interiores, Surf, mergulho onde
   adotado, bicicletas, escadas, cavernas, derrota, save/reload e retorno a Kanto.

## Adaptações obrigatórias

- Converter tilesets, paletas, metatiles, atributos de colisão e comportamentos
  Emerald para as convenções FireRed; não copiar `map.bin` isoladamente.
- Resolver IDs de mapas/layouts, grupos, regiões e limites de armazenamento.
- Traduzir constantes de música, clima, gráficos de NPCs, variáveis, flags e scripts.
- Conferir capacidade da ROM e recursos GBA; testar também a build nativa.
- Não deslocar IDs antigos sem estratégia para saves existentes.
- Definir pontos de acesso e ritmo de progressão entre regiões antes de publicar.

## Organização efetuada

Somente `games/super-fire-red` permanece em `games/`. Super Z-A (incluindo ZA-MODS.md),
Super HeartGold/SoulSilver e seus dois workflows foram movidos para
`C:/Users/mathe/Documents/ChatGPT/archived-projects-2026-11-07/`.
Não foram apagadas ROMs, saves externos nem outros worktrees. As remoções locais
de arquivos rastreados são mantidas na branch de expansão; não alteram o main remoto.
