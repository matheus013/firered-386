# Inicialização de Frontier

O inicializador nativo gera o bloco 19 de SoulSilver USA: 5672 bytes (0x1628).
O rank Wi-Fi começa em 1; quatro mensagens da Torre recebem os templates
originais; os demais dados e flags começam zerados.

`tools/verify_frontier_init.py` valida o SHA-256 da imagem ARM9 privada e o
descritor exato, executa o original sem substituir serviços, compara cada byte
do bloco e da cópia global em 0x021D2AFC. Quatro padrões de memória inicial
passaram. Guardas, limites de escrita e 1418 escritas de quatro bytes na cópia
por caso foram conferidos. São 5672 escritas globais nos quatro casos.

A API retorna um valor independente. Não cria uma variável global: carregar e
confirmar a cópia de trabalho continuará sob responsabilidade do contexto da
partida. Um teste verifica independência entre cópia salva e cópia modificada.

Linux ASan/UBSan: 31/31 suites. Windows: 30/30 suites.
41/42 inicializadores isolados estão comparados ao ARM9. Nova partida completa,
arquivo de save e campanha ainda não estão integrados.

## Dependência restante: FriendRoster

O bloco 25 tem 0x8C0 bytes. A referência limpa o bloco, inicializa 32 entradas
em 0x1C0 com stride 0x38 e então chama Comm_EnsureDwcUserData (0x0203A01C).
Esse serviço verifica o checksum da conta, cria os dados DWC usando o código
0x4144414A quando inválidos e limpa a flag dirty, recalculando o checksum.
Depois copia o bloco inteiro para 0x021D2230. O cabeçalho da conta tem 64 bytes.

Ainda é necessário portar ou definir explicitamente o provedor desses dados.
Substituir esse serviço por bytes zerados não valida a criação da conta e não
será contabilizado como port completo de FriendRoster. Nenhuma alteração de
rede ou criação de conta real foi executada nesta investigação.
