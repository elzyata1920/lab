#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

//  Версия для C++11 (рекурсия) 
// Базовый случай (один аргумент)
template<typename Func, typename T>
void forEach11(Func&& func, T&& arg) {
    func(forward<T>(arg));
}

// Рекурсивный случай (несколько аргументов)
template<typename Func, typename First, typename... Rest>
void forEach11(Func&& func, First&& first, Rest&&... rest) {
    func(forward<First>(first));
    forEach11(forward<Func>(func), forward<Rest>(rest)...);
}

//  Версия для C++17 (fold expression) 
template<typename Func, typename... Args>
void forEach17(Func&& func, Args&&... args) {
    (func(forward<Args>(args)), ...);
}

int main() {
    setlocale(LC_ALL, "RU");

    // Функция для вывода (лямбда)
    auto print = [](const auto& x) {
        cout << x << " ";
        };

    // Функция для возведения в квадрат (изменяет аргументы)
    auto square = [](auto& x) {
        x = x * x;
        };

    cout << "=== Версия C++11 (рекурсия) ===\n";
    forEach11(print, 10, 3.14, "привет", 'X');
    cout << endl;

    int a = 2, b = 3, c = 4;
    forEach11(square, a, b, c);
    cout << "После square: a=" << a << ", b=" << b << ", c=" << c << endl;

    cout << "\n=== Версия C++17 (fold expression) ===\n";
    forEach17(print, 100, 200, 300, 400);
    cout << endl;

    int x = 5, y = 6, z = 7;
    forEach17(square, x, y, z);
    cout << "После square: x=" << x << ", y=" << y << ", z=" << z << endl;

    return 0;
}