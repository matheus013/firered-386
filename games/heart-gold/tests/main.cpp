#include "test.hpp"

#include <cstring>

// Uso: hg_tests [prefixo]. Sem argumento, roda todos os testes.
int main(int argc, char** argv)
{
    const char* filter = argc > 1 ? argv[1] : "";
    int ran = 0;
    int failed_cases = 0;
    for (const auto& c : hg::test::registry()) {
        if (std::strncmp(c.name, filter, std::strlen(filter)) != 0) {
            continue;
        }
        const int before = hg::test::failures();
        c.fn();
        ++ran;
        const bool ok = hg::test::failures() == before;
        failed_cases += ok ? 0 : 1;
        std::printf("[%s] %s\n", ok ? " OK " : "FAIL", c.name);
    }
    std::printf("%d teste(s), %d com falha\n", ran, failed_cases);
    return (ran > 0 && failed_cases == 0) ? 0 : 1;
}
