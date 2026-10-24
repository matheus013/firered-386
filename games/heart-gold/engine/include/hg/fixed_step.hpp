#pragma once

#include <cstdint>

namespace hg {

// Taxa de quadros do Nintendo DS: 33.513.982 Hz / (355 * 263 * 6) ≈ 59,8261 Hz.
inline constexpr std::int64_t kDsFrameNanoseconds = 16'715'113;

// Passo fixo da lógica do jogo, independente da taxa de atualização do monitor.
// A velocidade multiplica quantos passos rodam por tempo real (1x, 2x, 4x...);
// a lógica de cada passo nunca muda, então as mecânicas originais são preservadas.
class FixedStep {
public:
    explicit FixedStep(std::int64_t step_ns = kDsFrameNanoseconds, int max_steps_per_update = 8);

    void set_speed(int multiplier);
    int speed() const { return m_speed; }

    // Acumula o tempo real decorrido e devolve quantos passos de lógica executar agora.
    // Limita o total para não entrar em espiral quando a máquina não acompanha.
    int advance(std::int64_t elapsed_ns);

    std::int64_t step_ns() const { return m_step_ns; }

private:
    std::int64_t m_step_ns;
    int m_max_steps;
    int m_speed = 1;
    std::int64_t m_accumulator = 0;
};

} // namespace hg
