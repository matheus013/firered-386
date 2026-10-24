# Gráficos 2D no renderer nativo

O executável próprio SDL3 agora oferece um teste visual que lê recursos locais
e compõe três camadas 2D do título SoulSilver: fundo, logo e créditos.
Os pixels são decodificados por código C++20 autoral, enviados a uma textura RGBA
e exibidos pelo renderer SDL3 com filtro nearest. Não há CPU DS, ROM encapsulada,
ssrecomp, melonDS ou SDK Nintendo nesse caminho.

Isso é um teste de recursos gráficos. Não executa o controlador original do título,
as animações 3D de Lugia/Ho-Oh, transições, áudio, seleção de nova partida ou campanha.
A janela e a legenda indicam explicitamente `NATIVE 2D RESOURCE TEST`.
O executável padrão sem recursos mantém a tela de infraestrutura.

## Formatos implementados

- NARC: leitura por membro com validação de cabeçalho, blocos e limites.
- NCGR: tiles de 8x8 pixels, 4 ou 8 bits por pixel, sem compressão ou bitmap/scanned.
- NCLR: paletas padrão RGB555, conversão para RGBA e transparência do índice zero.
- NSCR: mapas text BG até 256x256, inversão horizontal/vertical e bancos de paleta 4 bpp.
- Composição de camadas com alpha binário e corte para a área visível 256x192.

Compressão LZ, mapas maiores com screen blocks, modos afins, paletas estendidas,
sprites/células e modelos 3D ainda não são suportados. Recursos fora desse escopo
produzem erro; não há fallback para emulação. O leitor é usado sobre dados locais
conhecidos, não como um serviço para processar uploads arbitrários.

## Relação com a base

Os pares são os usados por
[TitleScreenAnim_Load2dBgGfx no pin da base](https://github.com/ClickingMaster/pokeheartgold-single-screen/blob/6984a3d67592157195940049be2e4bbbf933f9ff/src/title_screen.c#L593):

| Camada | NCGR | NSCR | NCLR |
|---|---:|---:|---:|
| Fundo SoulSilver | 36 | 37 | 2 |
| Logo SoulSilver | 1 | 0 | 2 |
| Créditos | 15 | 17 | 2 |

O contêiner é `a/0/4/6`, NARC ID 46. A composição diagnóstica exibe essas camadas
sem fades, brilho, scroll ou ciclo de vida do controlador original.
O layout é explicitamente SoulSilver; não selecionar arquivos HeartGold com esse preset.
Não foi copiada a implementação de gráficos do SDK ou do frontend upstream.

## Executar no Windows

Importe somente o arquivo necessário para uma pasta privada nova:

```powershell
python games/super-heartgold/tools/import_nds_resources.py "D:/jogos/SoulSilver-USA.nds" --output "D:/dados-privados/hg-titulo" --file a/0/4/6
games/super-heartgold/out/windows/SuperHeartGold.exe --title-resources "D:/dados-privados/hg-titulo"
```

O segundo comando usa a saída do helper WSL. No build Visual Studio, use o
executável em `build/bin/Release/`. Feche com Esc; F11 alterna tela cheia.
Esse teste usa uma vista única; os layouts stacked/side não reorganizam suas camadas.
A escala segue `--scale integer|fit`, com uma faixa separada para a legenda.

Para uma execução limitada com captura do renderer:

```powershell
games/super-heartgold/out/windows/SuperHeartGold.exe --title-resources "D:/dados-privados/hg-titulo" --frames 60 --capture "D:/dados-privados/hg-titulo/captura-nova.png"
```

A captura é feita antes do primeiro present e recusa um arquivo existente.
Mantenha os recursos e as capturas do jogo fora do Git. A ROM não é aberta pelo
executável durante essa execução: ele acessa somente o arquivo NARC importado.
Não há leitura ou gravação de saves do jogo.

## Validação — 24/10/2026

- Windows x64, SDL3 3.4.18: execução com renderer software/dummy e com janela real
  Windows/Direct3D 11, ambas com exit 0. A janela real executou 60 frames.
- Capturas 1280x720 inspecionadas; logo, fundo, créditos e legenda legíveis, sem
  sobreposição da legenda à área de jogo. Os dois renderers produziram PNG idêntico,
  SHA-256 `a9d2c382758ff28b74239509fe0ce08a162bdf45a1b74cd48b3adc96ab95a3bf`.
- Seis casos C++ verificam cores, ordem de nibbles, bancos, flips, alpha, cortes,
  cabeçalhos truncados, índices e offsets inválidos e modos não suportados.
- O teste de integração `test_graphics_renderer.py` cria NARC/NCGR/NCLR/NSCR próprios,
  abre o executável real, lê a captura PNG e exige 288 pixels vermelhos e 288 verdes,
  verificando composição e escala inteira. Também testa recusa de sobrescrita e
  falha na inicialização com arquivo truncado.
- Esse teste é incluído na CI nativa com Python/SDL3. Em cross-compilation WSL ele
  é executado separadamente com Python Windows, pois seus caminhos temporários
  precisam pertencer ao mesmo sistema operacional do executável.

As capturas e os recursos originais ficam somente no armazenamento privado.
O relatório público contém metadados em [native-graphics-validation.json](native-graphics-validation.json).

O próximo trabalho é integrar o fluxo real do título e adaptar os serviços
gráficos/3D exigidos por esse fluxo. A primeira área jogável continua pendente.
