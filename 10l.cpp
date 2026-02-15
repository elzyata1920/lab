#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <type_traits>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    // Constexpr лямбда для вычисления наименьшего общего кратного (НОК)
    auto lcm = [](auto a, auto b) constexpr {
        using T = decltype(a);
        static_assert(is_integral_v<T>,
            "НОК можно вычислить только для целых типов");

        if (a == 0 || b == 0) return T{ 0 };

        // Внутренняя constexpr лямбда для НОД (алгоритм Евклида)
        auto gcd = [](auto x, auto y) constexpr {
            while (y != 0) {
                auto t = x % y;
                x = y;
                y = t;
            }
            return x;
            };

        T g = gcd(a, b);
        return (a / g) * b;
        };

    // Тесты во время компиляции (значения вычисляются на этапе компиляции)
    constexpr auto res1 = lcm(12, 18);   // 36
    constexpr auto res2 = lcm(7, 13);    // 91
    constexpr auto res3 = lcm(0, 5);     // 0
    constexpr auto res4 = lcm(21, 6);    // 42

    cout << "LCM(12, 18) = " << res1 << endl;
    cout << "LCM(7, 13)  = " << res2 << endl;
    cout << "LCM(0, 5)   = " << res3 << endl;
    cout << "LCM(21, 6)  = " << res4 << endl;

    // Проверка во время выполнения
    int a = 100, b = 75;
    cout << "LCM(100, 75) = " << lcm(a, b) << endl;

    // Работа с разными целыми типами
    unsigned int u = 15, v = 20;
    cout << "LCM(15u, 20u) = " << lcm(u, v) << endl;

    return 0;
}