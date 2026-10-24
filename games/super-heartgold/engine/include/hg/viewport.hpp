#pragma once

namespace hg {

// Resolução nativa de cada tela do Nintendo DS.
inline constexpr int kDsScreenWidth = 256;
inline constexpr int kDsScreenHeight = 192;

struct Size {
    int w = 0;
    int h = 0;
};

struct Rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    friend bool operator==(const Rect&, const Rect&) = default;
};

// Como as telas do jogo são arrumadas numa única janela.
enum class ScreenLayout {
    Unified,    // modo desktop: a tela de cima (4:3) intacta no centro; o conteúdo da tela de baixo vira HUD nas laterais
    Stacked,    // clássico: uma tela sobre a outra, como no aparelho
    SideBySide, // clássico: lado a lado, aproveita telas widescreen
};

enum class ScaleMode {
    Fit,     // maior escala que cabe, mantendo a proporção
    Integer, // maior escala inteira que cabe (pixels nítidos); cai para Fit se nem 1x couber
};

struct ScreenPlacement {
    Rect top;       // tela de cima; no modo Unified, a vista do jogo
    Rect bottom;    // tela de baixo; vazia (w == 0) no modo Unified
    Rect hud_left;  // só no modo Unified: faixas laterais, da altura da janela, para o HUD.
    Rect hud_right; // Ficam vazias se a janela não for mais larga que 4:3.
    float scale = 0.0f;
};

// Calcula onde desenhar as telas numa janela de tamanho `window` (em pixels),
// sem distorcer a proporção e centralizando o conjunto (letterbox/pillarbox).
ScreenPlacement place_screens(Size window, ScreenLayout layout, ScaleMode mode, int gap = 0);

} // namespace hg
