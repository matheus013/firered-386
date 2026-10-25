# Mailbox e Safari com pré-clear e identidade explícitos

Implementação de 25/10/2026. Duas APIs separadas de `PartialNewGameBlocks`:

```cpp
initialize_precleared_mailbox(hg::GameIdentity{language, version});
initialize_precleared_safari(hg::GameIdentity{language, version});
```

Cada função cria um bloco bruto DS zerado e aplica os efeitos do inicializador
correspondente. Isso reproduz **pré-clear do chamador + inicializador**, não a
chamada isolada original sobre bytes arbitrários. `GameIdentity` recebe dois
bytes explícitos; aceitar um valor não significa validar uma edição, idioma ou
localização jogável. Não há defaults ocultos, RNG, RTC, identidade da máquina,
globais, acesso a arquivos ou escrita de save nessas funções.

## Contratos dos blocos

Mailbox (ID13, `0x460` bytes) tem 20 Mail de 56 bytes. Cada slot recebe idioma,
versão, tipo `MAIL_NONE`, nome e ícones vazios e três mensagens sem banco/EC words.
O campo `msg_no` de cada mensagem fica zero **por causa do pré-clear**. O original
`MailMsg_Init` não grava esse campo; não se trata de padding. São 120 bytes
preservados quando o inicializador isolado roda sobre memória não zerada.

Safari (ID35, `0x5FC` bytes) tem dois conjuntos de áreas zerados e um líder não
vinculado, com idioma/versão explícitos, nome EOS e demais campos zerados. Os
quatro bytes finais de level/configuração/unlockTime ficam zero pelo pré-clear,
pois o inicializador original não os altera. Não são escolhidas áreas padrão:
a seleção de áreas feita após Oak é uma etapa diferente, dependente de RNG.

Referência fixa:
[`ClickingMaster/pokeheartgold-single-screen`, pin `6984a3d67592157195940049be2e4bbbf933f9ff`](https://github.com/ClickingMaster/pokeheartgold-single-screen/tree/6984a3d67592157195940049be2e4bbbf933f9ff).
Fontes relevantes: `src/save.c:799–812`, `src/mail.c:43–59,275–280`,
`src/mail_message.c:30–36`, `src/safari_zone.c:32–38,282–290` e seus headers.

## Prova privada ARM9

```sh
python3 tools/verify_identity_init.py --arm9 /caminho/privado/rc_image_0.bin
```

É necessário um compilador C++20 nativo compatível com GNU (`--compiler`, padrão
`c++`) e Unicorn opcional. A ferramenta compila o mesmo C++ de produção num
harness temporário fora do Git. Somente metadados e hashes são impressos.
Nenhuma ROM é extraída, nenhum save é aberto e o arquivo ARM9 não é modificado.

A imagem ARM9 precisa ter SHA-256
`5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8` e no
máximo 4 MiB. As funções de tamanho e inicialização são obtidas dos descritores
13 e 35 da tabela `0x020F64C4`, depois de verificar o hash. Os literais dos
helpers originais são conferidos contra os endereços de idioma `0x020F5670` e
versão `0x020F566C`.

Para cada caso, esses dois bytes são configurados **somente na RAM sintética**
antes de ativar os hooks. As identidades testadas são `(0,0)`, `(1,7)`, `(2,8)`,
`(3,7)`, `(4,8)`, `(5,7)`, `(7,8)` e `(255,255)`; os extremos testam o contrato
de bytes e não representam uma promessa de suporte a essas edições.

Cada identidade e bloco é executado com quatro preenchimentos prévios: zero,
`0xA5`, `0xFF` e `0x5A`. Primeiro roda o inicializador sem pré-clear como controle
negativo: os 120 bytes de `msg_no` ou os quatro bytes finais do Safari precisam
preservar o preenchimento. Depois, uma primitiva original de memset executa o
clear delimitado e o inicializador ARM9 roda novamente. Todos os bytes devem
coincidir com o C++ nativo.

Cada chamada original tem orçamento de 100.000 instruções. Guardas de 16 bytes
antes/depois precisam permanecer intactas. Um hook proíbe qualquer escrita fora
do bloco-alvo e da stack sintética, incluindo nos globais de identidade; esses
dois bytes também são conferidos após a execução. Compilação e harness possuem
timeouts de 120 e 30 segundos, respectivamente.

Resultado local: **64 comparações passaram** (8 identidades × 2 blocos × 4
padrões), com 48 controles negativos não nulos demonstrando a necessidade do
pré-clear. Hash agregado da saída sintética do C++:
`bfd6e28a955495be0e8dc5c0995eac522a0c31c5673306ea669bc4c0ff5c89aa`.
Os três testes C++ passaram com `-Wall -Wextra -Werror`, AddressSanitizer e
UndefinedBehaviorSanitizer.

Os 26 blocos sem contexto permanecem separados dessas APIs. Isso não inicializa
todos os blocos, escolhe o idioma do jogo, inicia Oak ou valida uma campanha.
