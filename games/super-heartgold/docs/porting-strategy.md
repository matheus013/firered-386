# Estratégia de portabilidade

## Base e evidência

A base mandatória é [ClickingMaster/pokeheartgold-single-screen no pin 6984a3d](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
A correção do usuário substitui a escolha anterior de ctrl como base principal.
[ctrl-cheeb-del, pin 5e595ff](https://github.com/ctrl-cheeb-del/pokeheartgold/tree/5e595ff9dcc9d86150c8d119cb07edd4ad4b95c3) e [pret, pin 6988158](https://github.com/pret/pokeheartgold/tree/6988158691eb6178cff697cda0428045ddcba5fb) são proveniência e comparação.

O [README](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/README.md) descreve `SINGLE_SCREEN`: comandos de batalha, bolsa e equipe são reorganizados para uma tela e seu estado é exposto ao frontend.
Também identifica SoulSilver USA como configuração testada pelo setup; isso não comprova HeartGold nativo funcionando aqui.
O [Makefile](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/Makefile) continua usando ARM9 e produzindo `.nds`; ARM7, SDK e formatos DS continuam relevantes.
O [frontend original](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/frontend/main.cpp) usa SDL2, componentes melonDS e interfaces de recompilação externas.
Portanto esse código não constitui sozinho um runtime independente SDL3 para nosso projeto.
As adaptações de layout, publicação de estado e regras são candidatas a estudo e port seletivo.
Não foi copiado código ou asset desse fork para M0.

## Portar por unidades

1. Inventariar chamadas externas, structs, recursos e origem de cada módulo no pin fixado.
2. Registrar permissões aplicáveis antes de incorporar código ou recursos.
3. Selecionar lógica real e pequena, com poucas dependências e testes de comportamento.
4. Substituir serviços DS por interfaces host explícitas e validar um formato com fixtures próprias/autorizadas.
5. Integrar exploração, interface single-screen, batalha e áudio numa fatia jogável.

Não há decisão antecipada de reescrever tudo nem promessa de recompilar todo o upstream sem adaptação.
O projeto nativo não será apresentado como wrapper de emulador.
Rotinas ARM, acesso a registradores, DMA e sincronização ARM9/ARM7 exigem substituições fundamentadas.
Overlays precisam de ciclo de vida host explícito, sem depender de posições fixas de memória.

## ABI e formatos

Os tipos DS e casts de ponteiros precisam de auditoria em 64 bits.
`unsigned long` tem 64 bits no macOS ARM64 e 32 no Windows x86-64; ponteiros têm 64 nos dois.
Usar larguras fixas, IDs/offsets serializados e limites verificados; não truncar ponteiros para `u32`.
Separar structs de arquivo do runtime e testar padding, bitfields, alinhamento e fixed point.
Gráficos 2D/3D, sequências de áudio, NARC, heaps e overlays dependem de serviços ausentes em SDL3.

## Permissões e proveniência

A [LICENSE raiz](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/LICENSE) contém texto MIT padrão com copyright ClickingMaster.
A [frontend/LICENSE](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/frontend/LICENSE) acrescenta crédito nominal a ClickingMaster em README ou tela de créditos.
O README limita a oferta às mudanças single-screen/frontend e distingue o código upstream e as licenças de ferramentas.
Não presumir que um autor relicenciou automaticamente código original, SDK ou assets de terceiros.
Os pins inspecionados de ctrl/pret não têm licença geral na raiz; isso não demonstra autorização de redistribuição.
Cada incorporação exige decisão por escopo, preservação de avisos e cumprimento das obrigações aplicáveis.
O validador de assets verifica estrutura e proveniência declarada; não prova titularidade ou autorização.
M0 usa somente código autoral e recursos próprios/autorizados; ROM, BIOS e SDK proprietário não entram no repositório.
Respeitar a política publicada de não enviar trabalho gerado por IA a pret.
