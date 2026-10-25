# Inicializadores de blocos CRC isolados

Validação: 25/10/2026, edição SoulSilver USA pinada. As funções nativas
`initialize_soulsilver_usa_crc_block` cobrem os IDs 23, 24, 26, 27, 30 e 31.
Elas recebem um `SaveContext` dono do armazenamento; não aceitam ponteiros de
bloco desconectados do ID usado para calcular o CRC.

O harness comparou cada bloco com a rotina ARM9 original para quatro padrões
iniciais. Foram **24 comparações de bloco e 652 atualizações de CRC**. A única
substituição é `SaveSubstruct_UpdateCRC`: o serviço sintético calcula CCITT
independentemente sobre o payload alinhado e grava os dois bytes CRC no bloco
vinculado. As rotinas de inicialização originais e os demais cálculos rodam na
imagem ARM9 privada SHA-256
`5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8`.

O ID 24 contém 156 entradas de Ranking; cada nome começa vazio em `0xFFFF` e a
rotina original atualiza o CRC depois de cada entrada e ao final. O ID 27,
Mystery Gift, preserva o payload e atualiza somente seu CRC: a limpeza pertence
ao clear anterior do contexto de save. ID 30 usa a tabela de idioma SoulSilver
USA (inglês = 2) para inicializar flags de Easy Chat e executa duas atualizações
CRC originais. ID 31 habilita notificações Wii no campo correspondente. Os IDs
23 e 26 limpam o payload e calculam o CRC final.

Reprodução local, fora de um build público:

```sh
python3 tools/verify_crc_block_init.py --arm9 /caminho/privado/rc_image_0.bin
```

`ctest` inclui regressões dos seis IDs, preservação dos dois bytes finais do
trailer, IDs rejeitados e ausência de escrita em blocos vizinhos. Esta prova
não monta os `SaveArrayHeader`, footers, cópias de flash nem um save completo.
