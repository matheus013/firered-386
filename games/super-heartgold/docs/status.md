# Estado geral e estimativa de conclusão

Avaliação de engenharia em 24/10/2026: **aproximadamente 10% do port completo de
HG/SS para Windows**, com faixa ampla de **5% a 15%**. Para o projeto ampliado com
Hoenn e 24 insígnias, **menos de 5%**. São estimativas de esforço e risco dos marcos,
não porcentagens medidas de código, testes ou campanha.

O objetivo estrito é executar o jogo completo em uma engine host, sem CPU DS
interpretada e sem uma ROM encapsulada como produto. Abrir uma janela, mostrar o
título ou executar unidades isoladas não conclui esse objetivo. A campanha
validada na engine própria permanece em **0%: nenhuma área jogável foi integrada**.

## Evidência disponível

| Área | Validado | Pendente principal |
|---|---|---|
| Plataforma Windows | C++20, SDL3, x64, viewport, entrada, clock, CI | Testes extensos de dispositivos e distribuição |
| Recursos | Importação privada, arquivos host, NARC, NCGR/NCLR/NSCR, LZ10/LZ11, mensagens, fontes e impressor C incremental privado | Comandos restantes de texto, sprites, modelos e animações |
| Código original | Scheduler, filesystem e overlay manager em provas nativas; decisão do título ligada à janela | Init/exit completos, serviços de runtime e restante da engine |
| Check-save | Decisão original controlada; leitor/inspetor dos blocos principais, CRC e seleção redundante testados | Registros extras, estado do jogo e carga de partida real integrados ao launcher |
| Gráficos/áudio | Camadas 2D estáticas e efeitos host | 3D, câmera, animações e áudio original |
| Gameplay | 26 inicializadores parciais de blocos comprovados; ainda sem área jogável | Outros 16 blocos, fluxo de nova partida, exploração, scripts, interação e batalhas |
| Campanha/pós-jogo | Sem validação nativa integrada | Johto, Kanto e pós-jogo; Hoenn depois |

A referência AOT híbrida é útil para observar o jogo original. Como ainda contém
interpretação ARM7/fallback ARM9, seu título/menu funcionando não é contado como
campanha ou engine host concluída.

## Como interpretar os 10%

A infraestrutura é reutilizável e já elimina obstáculos iniciais, mas a maior
parte do risco está nos serviços gráficos/3D, formatos, ABI, save e integração
do gameplay. Portanto a aparência do título tem peso pequeno na estimativa total.
Mais testes de uma unidade aumentam confiança nessa unidade, não a porcentagem
de campanha portada. A faixa será revista quando houver uma primeira área
jogável, uma batalha completa e save/load integrados; não é uma previsão de data.

## Próximos critérios de aceite

1. Definir o armazenamento host e preservar os layouts de dados sem usar `sizeof`
   de structs x64 como se fossem os formatos DS.
2. Ampliar a validação principal de save já implementada para registros extras e
   interpretação do estado do jogo. CRC e seleção redundante foram confirmados;
   ainda não existem partidas reais carregadas e validadas no port.
3. Integrar o caminho de partida nova. A base pula o menu quando não há save;
   não devemos fabricar uma opção Continue funcional antes da persistência existir.
4. Adaptar os serviços necessários à introdução e à primeira área, incluindo 3D.
5. Demonstrar exploração, interação, batalha e persistência antes de ampliar a campanha.

As evidências detalhadas estão em [native-resources.md](native-resources.md),
[native-graphics.md](native-graphics.md), [native-title-flow.md](native-title-flow.md)
e [checksave-decision.md](checksave-decision.md). A validação principal independente
está em [native-save-validation.md](native-save-validation.md).
Mensagens e fonte estão em [native-text.md](native-text.md). A renderização de
uma fala estática ainda não acrescenta uma área jogável à campanha validada.
O novo [impressor privado](native-text-flow.md) executa texto incremental,
pausas, confirmação e rolagem. Os
[26 blocos parciais](newgame-dependency-audit.md) também não constituem uma
partida: os outros inicializadores e o fluxo dos overlays continuam pendentes.
