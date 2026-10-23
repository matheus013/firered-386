# Avaliação do VBA Link

## Fontes e identificação

- Página original: https://www.vbalink.info/download-gba-emulator.htm
- Arquivo original 1.72: https://www.vbalink.info/downloads/V172lsrc.zip
- Cópia (removida do repositório; está no histórico do git): `emulator/downloads/V172lsrc.zip`; SHA-256 `BBA595FCE888E2AF151D99B4351DE4F16AA2CF8671AEBFE08A0E37B3BBAD944B`.
- Código completo consultado para 1.8: https://github.com/luckytyphlosion/vba-link
- Commit: `a4f4ff94a9e5ba651b6258b3b4d8319813341577`.

O arquivo original 1.72 contém arquivos de substituição, não uma árvore completa moderna. O repositório 1.8 é um port Linux de terceiros e se apresenta como projeto descontinuado. Sua identificação como 1.8 não comprova equivalência com o executável Windows LAN/WiFi do usuário. A documentação original distingue a conexão 1.72 da conexão 1.8: https://www.vbalink.info/vba-link-lan.htm

## Achados no transporte 1.8 e mudanças realizadas

Código: [`emulator/vba-link/src/GBALink.cpp`](../emulator/vba-link/src/GBALink.cpp); novo auxiliar: [`LinkTransport.h`](../emulator/vba-link/src/LinkTransport.h).

| Problema observado | Consequência possível | Correção desta entrega |
| --- | --- | --- |
| Laços somam o retorno de `recv` sem tratar zero/erro | Laço infinito ou deslocamento negativo após desconexão | Leitura completa com EOF, erros e prazo explícito |
| Leitura solicita 16/256 bytes mesmo após receber parte de um pacote | Escrita além do espaço restante; consumo do pacote seguinte | Receber apenas o restante; separar handshake e frames |
| Comprimento do frame é um `char` não validado | Comprimento negativo ou inesperado | Interpretar como byte sem sinal; validar 4–12 bytes pares e capacidade |
| Leitura pode consumir vários frames e descartar o excedente | Dessincronização entre participantes | Consumir exatamente um frame por operação |
| Retorno de `send` é ignorado | Mensagens truncadas em escritas parciais | Completar escrita com prazo e sinalizar falha |
| `tv_usec = linktimeout * 1000` | `timeval` inválido para tempos de um segundo ou mais | Usar resto da divisão por 1000 |
| `FD_ZERO` dentro do laço de sockets do servidor | Apenas o último socket permanece no conjunto | Limpar conjunto antes do laço |
| `TCP_NODELAY` recebe `bool` de um byte | Opção pode ser rejeitada pelo sistema | Usar `int` e tamanho correspondente |
| Erro de `pthread_create` testado com `< 0` | Falha positiva passa despercebida | Testar retorno diferente de zero |
| `select` reutiliza o prazo alterado no laço de aceitação | Espera ativa após expirar o primeiro prazo | Reinicializar prazo a cada iteração |
| Handshake e tamanho de dados não validados antes de decodificar | IDs fora do intervalo e leitura de dados antigos | Validar IDs, número de participantes e tamanho esperado |

O pacote de desconexão de um ramo do servidor também anunciava quatro bytes e enviava doze: agora envia quatro. O laço de fechamento desse ramo inclui o último cliente. O buffer local da thread de aceitação passa a ser inicializado.

As correções reduzem classes concretas de falhas; não estabelecem compatibilidade completa nem melhoria medida de velocidade. Em falhas, a sessão é marcada como desconectada: não há reconexão automática nem garantia de recuperação da partida.

## Validação

[`emulator/tests/test-18.sh`](../emulator/tests/test-18.sh) produz registros em `emulator/tests/test-results/`:

- 265 cenários do auxiliar: todos os comprimentos permitidos; 251 comprimentos inválidos; fragmentação; junção de frames; capacidade insuficiente; EOF; prazo; handshake junto da mensagem seguinte; escrita de 1 MiB com buffer pequeno; conexão encerrada.
- Integração com os métodos reais `lclient::CheckConn`, `lclient::Recv`, `lclient::Send` e `lserver::Recv`: 2/3/4 participantes, valores decodificados, frames acumulados, tamanho de resposta, EOF e rejeição de mensagem curta.
- Ambos com AddressSanitizer e UndefinedBehaviorSanitizer, sem diagnóstico nas execuções registradas.
- Compilação do arquivo completo `GBALink.cpp` em C++17, sem `-fpermissive`. Permanecem avisos de código antigo, registrados em `compile18.txt`.

Não foram executados jogos, trocas, batalhas reais, RFU, interface gráfica, testes pela internet ou o emulador Windows 1.8 completo. Os testes usam sockets locais; não medem latência de rede real. O teste do auxiliar usa sockets Unix com semântica de fluxo; os métodos da integração também são isolados por sockets locais.

## Pendências relevantes

1. **Windows 1.8 completo:** o port baixado usa APIs POSIX; os arquivos históricos Windows dependem de MFC e ferramentas antigas. A correção não gera um `.exe` 1.8 pronto para jogar.
2. **RFU / adaptador wireless:** não alterado nem validado. O transporte TCP testado não cobre toda a emulação RFU.
3. **Ciclo de vida e concorrência:** continuam globais compartilhadas e código antigo de threads/memória compartilhada; revisar encerramento e sincronização antes de considerar a base estável.
4. **Conexão inicial:** o uso histórico de `fcntl(F_GETFL, ...)`, tratamento de `connect`, cancelamento durante a espera de participantes e resolução de host ainda precisam revisão. A espera pelo frame de início foi limitada a uma hora nesta primeira correção.
5. **Erros de envio e encerramento:** a marcação de desconexão foi adicionada, mas o fluxo completo de interface/reconexão ainda não foi redesenhado.
6. **Emulação e saves:** CPU/PPU/áudio, precisão dos timings, compatibilidade da hackrom e integridade dos saves não foram auditados integralmente.
7. **Avisos herdados:** há conversões de `NULL` para inteiros e potencial truncamento/estouro no nome do log; isso continua visível no registro de compilação e precisa uma rodada própria.

## Servidor experimental 1.72

Antes da confirmação da versão usada, foi criado `emulator/improved-server/` (removido do repositório; está no histórico do git), separado do cliente. Corrige no servidor 1.72 fragmentação, escritas parciais, desconexões, dados não inicializados e a dependência de thread/evento; abre apenas a porta escolhida e permite definir prazo. O bind padrão é localhost; LAN exige escolha explícita.

Foi compilado para Windows x64 e Linux. Sete métodos de teste, com subcasos para 2–4 jogadores, passaram em cinco variantes: Windows, Linux, ambas com envios forçados de um byte e Linux com sanitizadores. Isso valida o modelo de protocolo dos testes, não partidas com clientes reais. O cliente original 1.72 ainda tem problemas próprios de buffers e encerramento que o servidor separado não resolve.

O 1.8 usa frames de 8/10/12 bytes onde o protocolo 1.72 usa 8/12/16. Os produtos não são intercambiáveis.

## Licença

Preservados `COPYING`, autores e comentários do código de origem. Os auxiliares e testes novos são publicados com identificador `GPL-2.0-or-later`. O manifesto registra hashes dos artefatos desta entrega. Nenhuma ROM comercial está incluída no pacote do emulador.
