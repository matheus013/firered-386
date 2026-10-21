# Progressão e treinamento — v0.2.0-dev

A curva dos treinadores foi elevada por etapa, preservando história, caminhos e primeira luta no laboratório. Os parâmetros reproduzíveis ficam em `data/progression_curve.json`; as equipes completas em TRAINER_REVIEW.md e data/trainer_balance.json.

| Desafio | Maior nível |
|---|---:|
| Brock / Misty / Surge / Erika | 18 / 28 / 36 / 44 |
| Koga / Sabrina / Blaine / Giovanni | 52 / 60 / 68 / 76 |
| Lorelei / Bruno / Agatha / Lance | 80 / 83 / 86 / 89 |
| Campeão, primeira Liga | 92 |
| E4 e Campeão, revanche | Todos os integrantes no 100 |

Rotas e áreas têm faixas de níveis para evitar saltos indevidos em treinadores reutilizados. Rivais e Giovanni da história têm limites próprios. Evoluções por nível foram atualizadas segundo o novo nível; evoluções por pedra adequadas entram nas equipes avançadas. Líderes usam seis integrantes e especialidade preservada. A E4 mantém suas adaptações de campeões, e o Campeão recebe IVs 31, natureza/habilidade fixas e EVs por função. A Torre usa seu escalonamento original, sem adotar estes níveis fixos.

EXP e Rare Candy respeitam o teto nos dois modos. Após os ginásios, o teto acompanha cada membro da Liga ainda não vencido e chega a 100 após o Hall da Fama. No Normal, Pokémon adquiridos acima do teto continuam utilizáveis, sem ganhar EXP até o progresso alcançá-los. No Hardcore, continuam indisponíveis enquanto acima do teto, sem que isso os marque como perdidos.

## Cientista dos Centros

Os 19 Centros têm um NPC com interface gratuita de natureza, EVs e IVs. No Hardcore, fica disponível desde o começo; no Normal, após a Liga. Não há alteração na geometria dos mapas.

- Natureza: escolher entre 25 efeitos, com bônus/penalidade mostrados.
- EVs: 0–252 por atributo, máximo de 510 no total; setas ajustam, L zera e R maximiza dentro do saldo.
- IVs: 0–31 por atributo, com os mesmos controles de zerar/maximizar.
- A revisa e confirma; B volta ou cancela sem aplicar alterações pendentes.
- Ovos, Pokémon perdidos e Pokémon indisponíveis pelo teto do Hardcore não podem ser editados.
- Atributos são recalculados sem ganhar HP nem reviver. HP atual é limitado ao novo máximo se este diminuir.
- EVs ganhos em batalha continuam ativos; a interface permite corrigir/remover esses ganhos.

O ajuste de natureza usa um campo reservado do Pokémon e mantém personalidade, shiny, gênero e habilidade. Seu efeito nos atributos e a natureza exibida mudam; preferências de sabor que consultam diretamente a personalidade continuam usando a natureza original. Tamanho do Pokémon, checksum, armazenamento no PC e tamanho do save são preservados.

## Economia e cura do Hardcore

Lojas comuns acrescentam Rare Candy ao estoque por ₽1 exclusivamente no Hardcore. Revenda rende ₽0. Preços e estoques do Normal são preservados. Máquinas, prêmios por moedas e decoração não recebem essa alteração.

Cada Centro admite três curas completas pela enfermeira durante toda a partida. O diálogo mostra os usos restantes; recusar não gasta. A quarta cura é bloqueada. Os 19 locais têm contadores independentes salvos. Um Hardcore de versão anterior começa com zero usos registrados após atualizar. Normal mantém curas ilimitadas.

O contador abrange a enfermeira do Centro. Camas, outros curandeiros e recuperação pelo PC não são limitados por ele. Não há autosave nem proteção contra recarregar saves anteriores. O editor de atributos não recupera HP e, portanto, não fornece curas adicionais.

## Limites da calibração

Em v0.3.0-dev, as 386 espécies receberam encontros diretos, com níveis/faixas por etapa e lendários aleatórios travados após a Liga. Consulte ENCOUNTERS.md e SPECIES_LOCATIONS.md. Itens de evolução ainda precisam de revisão, mas a captura direta permite obter suas evoluções. A cota do Hardcore ainda limita a equipe disponível; o rival do laboratório permanece simples e as demais lutas são duplas. Níveis e treinamento maiores aumentam os números dos desafios, mas não medem sozinhos dificuldade estratégica ou chance de vitória. Não foi concluído um teste completo da campanha.

Os testes usam ROMs descartáveis no mGBA: auditam dados efetivos das equipes, turnos de batalha, regras/recarga do save e a interface de treinamento. As equipes artificiais usadas na automação da Liga têm HP elevado para permitir observar turnos; esse teste não simula uma equipe legal do jogador nem avalia balanceamento competitivo.
