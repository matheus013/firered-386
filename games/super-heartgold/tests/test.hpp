#pragma once

// Framework de testes mínimo, sem dependências externas.
// TEST_CASE registra um teste; CHECK e CHECK_EQ registram falhas sem abortar o teste.

#include <cstdio>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace hg::test {

struct Case {
    const char* name;
    std::function<void()> fn;
};

inline std::vector<Case>& registry()
{
    static std::vector<Case> cases;
    return cases;
}

inline int& failures()
{
    static int count = 0;
    return count;
}

struct Registrar {
    Registrar(const char* name, std::function<void()> fn) { registry().push_back({name, std::move(fn)}); }
};

inline void fail(const char* file, int line, const std::string& what)
{
    ++failures();
    std::fprintf(stderr, "  %s:%d: falhou: %s\n", file, line, what.c_str());
}

template <typename A, typename B>
void check_eq(const A& a, const B& b, const char* expr_a, const char* expr_b, const char* file, int line)
{
    if (!(a == b)) {
        std::ostringstream os;
        os << expr_a << " == " << expr_b << " (" << a << " != " << b << ")";
        fail(file, line, os.str());
    }
}

} // namespace hg::test

#define HG_TEST_CONCAT2(a, b) a##b
#define HG_TEST_CONCAT(a, b) HG_TEST_CONCAT2(a, b)

#define TEST_CASE(name)                                                                                      \
    static void HG_TEST_CONCAT(hg_test_fn_, __LINE__)();                                                      \
    static const ::hg::test::Registrar HG_TEST_CONCAT(hg_test_reg_, __LINE__)(name,                           \
                                                                             HG_TEST_CONCAT(hg_test_fn_, __LINE__)); \
    static void HG_TEST_CONCAT(hg_test_fn_, __LINE__)()

#define CHECK(expr)                                                                                          \
    do {                                                                                                     \
        if (!(expr)) ::hg::test::fail(__FILE__, __LINE__, #expr);                                            \
    } while (0)

#define CHECK_EQ(a, b) ::hg::test::check_eq((a), (b), #a, #b, __FILE__, __LINE__)
