# Bloco FriendGroup

`initialize_friend_group_block` cria o bloco de 264 bytes do ID 14, com seis
grupos vazios. Cada grupo tem 44 bytes; os primeiros code units de `groupName`
e `leaderName` recebem EOS (`0xFFFF`) e os demais campos começam zerados.

A prova privada executou `Save_FriendGroup_Init` e a função de tamanho originais
na ARM9. **Quatro padrões de entrada** produziram os mesmos 264 bytes. O hook
também conferiu que o único efeito fora do bloco e da pilha foi atribuir o
ponteiro do bloco a `sFriendGroupSave` (`0x021D2AF0`).

```sh
python3 tools/verify_friend_group_init.py --arm9 /caminho/privado/rc_image_0.bin
```

A função host retorna bytes e não cria um global equivalente. A integração deve
manter a instância ativa de FriendGroup ligada explicitamente aos consumidores
que usam esse estado. O bloco isolado não inicia a partida nem substitui o
gerenciamento do save.
