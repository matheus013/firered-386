# Misc: validação nativa

Bloco 9, 736 bytes. O teste verify_misc_init.py compara a implementação C++
com o ARM9 privado, validado por SHA-256. Oito snapshots e quatro padrões de
memória inicial: 32 comparações passaram. Apenas a cópia do RTC foi substituída;
os helpers e a conversão EasyChat originais executaram. Guardas e limites de
escrita foram conferidos. A validação de calendário é compartilhada com SysInfo.

Data em 0x230, hora em 0x240. O original preenche oito bytes de rivalName com FF,
deixando os oito seguintes zero. Greeting: banco 4, mensagem 0, palavra 0x501,
segundo campo FFFF. Dez chaves de chunks extras iniciam em FFFFFFFF.

Linux ASan/UBSan: 30/30 suites. Windows: 29/29 suites.
40/42 inicializadores isolados comparados; não há nova partida integrada,
save completo ou campanha validada.
