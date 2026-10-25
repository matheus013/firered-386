# Inicialização RTC de SysInfo

`initialize_system_info` cria os 92 bytes do bloco 0 a partir de um
`RtcSnapshot` explícito. Valida a data gregoriana no intervalo 2000–2099, hora,
minuto, segundo e weekday, e calcula dias/segundos desde 2000-01-01. O weekday é
copiado como informado pelo RTC; não é recalculado. MAC, aniversário e demais
campos dependentes de identidade/serviços ficam zerados.

O teste privado executou `SysInfo_Init` e as conversões de calendário originais
na ARM9 pinada. Apenas `GF_RTC_CopyDateTime` foi substituída por uma cópia
controlada dos sete valores do snapshot. **32 comparações passaram** (oito
datas × quatro padrões de memória), incluindo anos bissextos e os limites de
2099. O hash ARM9 foi
`5cccd60d47cad39a651cb326144d3116eb473540096c00d4e1a959656ee445f8`.

```sh
python3 tools/verify_system_info_init.py --arm9 /caminho/privado/rc_image_0.bin
```

Isto valida apenas o inicializador bruto SysInfo, não a captura de relógio do
sistema, a identidade local, o restante do save ou o fluxo de nova partida.
