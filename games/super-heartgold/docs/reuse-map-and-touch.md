# Reaproveitamento para o primeiro mapa — 02/11/2026

Mantida a rota nativa de fontes originais + serviços host. O ssrecomp é referência
externa de comportamento; seu dispatcher ARM e core melonDS não foram integrados.

## pokeheartgold-single-screen: carregador de matrizes

Compilado e executado o `src/map_matrix.c` original, sem editar seu corpo.
Os adaptadores de teste fornecem heap, leitura de um arquivo privado e seleção
explícita da matriz; não substituem o futuro filesystem/MapHeader do jogo.
A associação do quarto inicial (mapa 64) à matriz 72 é conferida no arquivo
original `src/data/map_headers.h` antes da compilação.

Três recursos da referência privada: matriz geral 0 (47×17), 71 (1×1) e quarto
inicial 72 (1×1). Em cada plataforma passaram 64 recargas por caso, 192 no total.
Conteúdo de nomes, headers, altitudes e modelos comparado integralmente com
decodificador independente; regiões não usadas devem estar limpas; contagem de
alocações volta a zero ao liberar. Antes de cada carga, memória recebe 0xA5.

- [Linux](hgss-map-matrix-linux-test.json): ASan/UBSan; instrumentação de globais
  desativada conforme a fronteira SDK do diagnóstico.
- [Windows](hgss-map-matrix-windows-test.json): EXE x64 executado por interop WSL,
  sem sanitizadores; resultados iguais. Corrigida a assinatura do adaptador para
  usar `s32`, compatível com o header em ambas as plataformas. O teste console
  usa SDL_MAIN_HANDLED e desliga tabelas de unwind para descartar funções de mapa
  não usadas no diagnóstico; não são configurações propostas para o jogo inteiro.

Dados vêm dos arquivos da referência pinada, não de comparação com a ROM nesta
rodada. Modelos 3D não foram desenhados. Colisão, eventos, FieldSystem, entrada
da partida e mapa controlável ainda não foram executados. Este resultado valida
o carregador e não comprova a integração de um mapa jogável.

## ssrecomp: referência de coordenadas single screen

`verify_touch_reference.py` compila diretamente `frontend/layout.h` de uma cópia
externa, confere o pin e os bytes do arquivo e compara `LayoutTouch` com nossa
ponte `reference_compat/single_screen_touch.h`.

Passaram [64.896 casos](hgss-touch-reference-test.json), cobrindo seis seleções
de layout, oito tamanhos de janela, ambas as posições de troca, bordas, pontos
internos e externos. Também se exige ausência de toque quando a tela está oculta
ou o botão não está pressionado. Execução Linux com ASan/UBSan.

Não testa composição merged, imagens, eventos SDL, conversão HiDPI nem gameplay.
O modo merged aqui só fornece o retângulo básico de `LayoutRects`; sua composição
específica depende de outro componente. A lógica nativa existente já coincide;
nenhuma mudança no runtime foi necessária para fazer o teste passar. Código do
ssrecomp permanece externo, sob a licença de seu repositório, sem cópia ao produto.

## Reproduzir

```sh
python3 tools/verify_map_matrix.py --hgss "$HGSS" --probe "$PROBE" \
  --stack "$STACK" --output "$NOVO_TESTE_LINUX"
python3 tools/verify_map_matrix.py --hgss "$HGSS" --probe "$PROBE" \
  --stack "$STACK" --output "$NOVO_TESTE_WINDOWS" --target windows \
  --sdl-include "$WINDOWS_DEPS/include"
python3 tools/verify_touch_reference.py --reference "$SSRECOMP" --output "$NOVO_TESTE_TOQUE"
```

Windows exige MinGW e interop WSL ativo; SDL2/SDL_thread.h precisa existir sob o
diretório de include informado. Saídas são novas e privadas. A geração de índices
da matriz valida a sequência dos nomes dos arquivos originais; não distribui os
headers gerados nem os recursos. Linux precisa de GCC/G++, SDL2 headers e sanitizadores.

## Referências e próxima integração

- [Carregador original](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/map_matrix.c).
- [Layout e toque do ssrecomp](https://github.com/ClickingMaster/ssrecomp/blob/3146fb2d4d88522b12b4bf25dee74285120975e2/frontend/layout.h).

Próximo bloqueio de gameplay: ligar carregamento de matriz, modelos/eventos e
estado inicial ao FieldSystem. Os percentuais de aceite funcional permanecem
inalterados; não houve boot, exploração, batalha ou save/retomada.
