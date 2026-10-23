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

// Como as duas telas do DS são arrumadas numa única janela.
enum class ScreenLayout {
    Stacked,    // uma sobre a outra, como no aparelho
    SideBySide, // lado a lado, aproveita telas widescreen
};

enum class ScaleMode {
    Fit,     // maior escala que cabe, mantendo a proporção
    Integer, // maior escala inteira que cabe (pixels nítidos); cai para Fit se nem 1x couber
};

struct ScreenPlacement {
    Rect top;
    Rect bottom;
    float scale = 0.0f;
};

// Calcula onde desenhar as duas telas numa janela de tamanho `window` (em pixels),
// sem distorcer a proporção e centralizando o conjunto (letterbox/pillarbox).
ScreenPlacement place_screens(Size window, ScreenLayout layout, ScaleMode mode, int gap = 0);

} // namespace hg
