# Servidor experimental VBA Link 1.72 LAN

**Somente servidor para o protocolo 1.72. Não é emulador e não é compatível com o servidor interno 1.8 LAN/WiFi.** Consulte `../AUDIT.md` e `../README.md` para o trabalho principal destinado ao 1.8.

Windows x64:

```powershell
.\bin\vbalink-server.exe --players 2
```

O padrão aceita apenas conexões locais. Para uma sessão LAN, acrescente `--bind 0.0.0.0` ou um endereço IPv4 local específico. Não foram alteradas regras do firewall. Portas padrão: 6478 para dois jogadores, 6479 para três e 6480 para quatro.

Há opções `--port`, `--timeout-ms` e `--help`. Prazo padrão zero permite espera sem limite. Nenhuma conexão criptografada ou autenticação foi adicionada. A porta selecionada deve ser usada apenas no ambiente de teste pretendido.

Compilação no Linux/WSL com GCC e MinGW-w64:

```sh
bash build.sh
python3 tests/test_protocol.py bin/vbalink-server-linux
```

No Windows com Python, `python tests/test_protocol.py bin/vbalink-server.exe`. Os testes de protocolo são sintéticos; clientes e partidas reais ainda precisam validação. Não há alegação de ganho medido de velocidade.

Implementação nova sob GPL-2.0-or-later, baseada no formato de protocolo do servidor VBA Link original de denopqrihg. Consulte `COPYING`.
