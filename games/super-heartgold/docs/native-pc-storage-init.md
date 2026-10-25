# PCStorage com nomes de boxes explícitos

`initialize_precleared_pc_storage` produz o bloco bruto de 0x122FC bytes. Recebe
o membro de mensagens 24 já extraído do NARC e decodifica os nomes 6 a 23 com o
decoder de mensagens de produção. O contrato inclui a pré-limpeza do bloco,
540 BoxMons vazios criptografados com seed zero, nomes de até 20 unidades
incluindo EOS, wallpapers padrão, box ativo zero e campos restantes zerados.
Não escolhe nomes, idioma ou arquivo por conta própria.

Os testes C++ usam um banco sintético para conferir limites, criptografia,
espaçamento, offsets, paredes por box e rejeição de entrada truncada ou
empacotada. O script `tools/verify_pc_storage_init.py` compara o bloco nativo
com `PCStorage_Init` da ARM9. A carga do NARC e o decoder são substituídos por
18 nomes do membro privado 24; inicialização de PCStorage e criptografia de
BoxMon vazios executam no ARM9 original. Resultado local: **74492 bytes iguais**,
540 BoxMons e todos os 18 nomes, com guardas intactas.

O NARC privado foi extraído do arquivo local SoulSilver USA e não foi salvo no
repositório. O harness troca apenas o serviço de abrir/ler/liberar mensagens;
não usa nomes simulados nesta comparação.

Para reproduzir com os dados privados já extraídos:

```sh
python3 tools/verify_pc_storage_init.py \
  --arm9 /caminho/privado/rc_image_0.bin \
  --archive /caminho/privado/a/0/2/7
```

O escopo é somente o bloco PCStorage isolado; o boot, demais blocos, footer e
arquivo de save não estão integrados.
