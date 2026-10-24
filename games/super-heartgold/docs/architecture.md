# Arquitetura

Super HeartGold é um projeto nativo C++20 com SDL3 e CMake, separado da hack FireRed.
O marco inicial M0 contém uma janela, eventos, desenho de uma tela demonstrativa e testes do framework.
Isso não significa que a engine de HeartGold foi portada ou que a campanha funciona.

## Camadas

- `app`: inicialização, ciclo de execução e composição dos serviços.
- Plataforma SDL3: janela, entrada, apresentação, tempo e, futuramente, áudio.
- Núcleo independente de SDL: estado, regras e serialização, com testes determinísticos.
- Recursos: carregamento com validação de limites e origem documentada.
- Interface: composição adaptável a uma janela; decisões sobre telas DS serão explícitas.

Essas camadas são direção arquitetural; a existência de uma interface não comprova sua implementação.
O shell, framework e testes iniciais podem reaproveitar arquivos autorais do próprio usuário.
Nenhum código ou recurso gráfico/sonoro dos repositórios de referência foi incorporado neste marco.

## Referência obrigatória

A base mandatória, corrigida pelo usuário, é [ClickingMaster/pokeheartgold-single-screen, pin 6984a3d](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
[ctrl-cheeb-del/pokeheartgold](https://github.com/ctrl-cheeb-del/pokeheartgold/tree/5e595ff9dcc9d86150c8d119cb07edd4ad4b95c3) e [pret/pokeheartgold](https://github.com/pret/pokeheartgold/tree/6988158691eb6178cff697cda0428045ddcba5fb) servem para proveniência e comparação.
O fork single-screen adapta interfaces e publica estado para seu frontend; seu build principal continua produzindo ROM DS.
A interface existente é referência para o port seletivo, sujeita à análise por arquivo e a testes.
Não confundir o frontend SDL2/melonDS desse fork com nosso runtime independente C++20/SDL3.

## Limites de hardware

A lógica referenciada depende de serviços DS e precisa de interfaces host com comportamento verificável.
Gráficos 2D incluem tilemaps, sprites, paletas, prioridades e transferências; modelos 3D usam NitroSystem e fixed point.
Áudio envolve sequências, bancos, amostras e serviços originalmente divididos entre ARM9 e ARM7.
Filesystem, heaps, overlays e callbacks precisam ser separados dos endereços físicos e interrupções DS.
SDL3 fornece serviços host; não implementa automaticamente Nitro SDK, GPU DS ou sequenciador do jogo.

## Estado e persistência

Tipos de dados persistidos usarão larguras explícitas e ordem de bytes definida.
Ponteiros, padding de structs e handles host não serão gravados diretamente.
Saves futuros terão cabeçalho com versão, validação de integridade, migração documentada e escrita atômica.
M0 não promete compatibilidade com saves DS ou FireRed.
