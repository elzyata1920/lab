#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

// 1.1 Пустой класс
class EmptyTriangleException {};

// 1.2 Класс с полями (параметры функции)
class TriangleException {
public:
    double a, b, c;
    string message;
    TriangleException(double a_, double b_, double c_, const string& msg)
        : a(a_), b(b_), c(c_), message(msg) {
    }
};

// 1.3 Класс – наследник стандартного исключения invalid_argument
class TriangleInvalidArg : public invalid_argument {
public:
    double a, b, c;
    TriangleInvalidArg(double a_, double b_, double c_, const string& msg)
        : invalid_argument(msg), a(a_), b(b_), c(c_) {
    }
};

// Вспомогательная функция проверки сторон треугольника
void checkTriangle(double a, double b, double c) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw invalid_argument("Стороны должны быть положительными");
    if (a + b <= c || a + c <= b || b + c <= a)
        throw invalid_argument("Нарушено неравенство треугольника");
}

// 1: без спецификации исключений
double perimeter_no_spec(double a, double b, double c) {
    checkTriangle(a, b, c);
    return a + b + c;
}

// 2: со спецификацией throw() – не генерирует исключения
double perimeter_throw(double a, double b, double c) throw() {
    // При ошибке просто возвращаем -1 и печатаем сообщение
    if (a <= 0 || b <= 0 || c <= 0) {
        cerr << "Ошибка (perimeter_throw): стороны должны быть положительными" << endl;
        return -1.0;
    }
    if (a + b <= c || a + c <= b || b + c <= a) {
        cerr << "Ошибка (perimeter_throw): нарушено неравенство треугольника" << endl;
        return -1.0;
    }
    return a + b + c;
}

// 3: со спецификацией стандартного исключения (invalid_argument)
double perimeter_std_except(double a, double b, double c) throw(invalid_argument) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw invalid_argument("Стороны должны быть положительными");
    if (a + b <= c || a + c <= b || b + c <= a)
        throw invalid_argument("Нарушено неравенство треугольника");
    return a + b + c;
}

// 4.1: собственное исключение – пустой класс
double perimeter_custom_empty(double a, double b, double c) throw(EmptyTriangleException) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw EmptyTriangleException();
    if (a + b <= c || a + c <= b || b + c <= a)
        throw EmptyTriangleException();
    return a + b + c;
}

// 4.2: собственное исключение – класс с параметрами
double perimeter_custom_params(double a, double b, double c) throw(TriangleException) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw TriangleException(a, b, c, "Стороны должны быть положительными");
    if (a + b <= c || a + c <= b || b + c <= a)
        throw TriangleException(a, b, c, "Нарушено неравенство треугольника");
    return a + b + c;
}
// 4.3: собственное исключение – наследник invalid_argument
double perimeter_custom_inherit(double a, double b, double c) throw(TriangleInvalidArg) {
    if (a <= 0 || b <= 0 || c <= 0)
        throw TriangleInvalidArg(a, b, c, "Стороны должны быть положительными");
    if (a + b <= c || a + c <= b || b + c <= a)
        throw TriangleInvalidArg(a, b, c, "Нарушено неравенство треугольника");
    return a + b + c;
}

int main() {
    setlocale(LC_ALL, "RU");

    // 1. Без спецификации исключений
    cout << "1. Функция без спецификации исключений:\n";
    try {
        double p = perimeter_no_spec(3, 4, 5);
        cout << "   Периметр (3,4,5) = " << p << endl;
    }
    catch (const invalid_argument& e) {
        cout << "   Поймано исключение: " << e.what() << endl;
    }

    try {
        double p = perimeter_no_spec(1, 2, 3); // 1+2=3 – не треугольник
        cout << "   Периметр (1,2,3) = " << p << endl;
    }
    catch (const invalid_argument& e) {
        cout << "   Поймано исключение: " << e.what() << endl;
    }
    cout << endl;

    // 2. Спецификация throw() – функция не генерирует исключения
    cout << "2. Функция со спецификацией throw():\n";
    double p2 = perimeter_throw(3, 4, 5);
    if (p2 >= 0) cout << "   Периметр (3,4,5) = " << p2 << endl;
    p2 = perimeter_throw(1, 2, 3);
    if (p2 >= 0) cout << "   Периметр (1,2,3) = " << p2 << endl;
    else cout << "   Функция вернула -1 (ошибка)\n";
    cout << endl;

    // 3. Спецификация стандартного исключения invalid_argument
    cout << "3. Функция со спецификацией throw(invalid_argument):\n";
    try {
        double p = perimeter_std_except(3, 4, 5);
        cout << "   Периметр (3,4,5) = " << p << endl;
    }
    catch (const invalid_argument& e) {
        cout << "   Поймано исключение: " << e.what() << endl;
    }

    try {
        double p = perimeter_std_except(-1, 4, 5); // отрицательная сторона
        cout << "   Периметр (-1,4,5) = " << p << endl;
    }
    catch (const invalid_argument& e) {
        cout << "   Поймано исключение: " << e.what() << endl;
    }
    cout << endl;

    // 4.1 Собственное исключение – пустой класс
    cout << "4a. Собственное исключение (пустой класс):\n";
    try {
        double p = perimeter_custom_empty(3, 4, 5);
        cout << "   Периметр (3,4,5) = " << p << endl;
    }
    catch (const EmptyTriangleException&) {
        cout << "   Поймано исключение EmptyTriangleException" << endl;
    }

    try {
        double p = perimeter_custom_empty(0, 4, 5); // нулевая сторона
        cout << "   Периметр (0,4,5) = " << p << endl;
    }
    catch (const EmptyTriangleException&) {
        cout << "   Поймано исключение EmptyTriangleException" << endl;
    }
    cout << endl;

    
    // 4.2 Собственное исключение – класс с параметрами
    cout << "4b. Собственное исключение (класс с параметрами):\n";
    try {
        double p = perimeter_custom_params(3, 4, 5);
        cout << "   Периметр (3,4,5) = " << p << endl;
    }
    catch (const TriangleException& e) {
        cout << "   Поймано исключение: " << e.message << " (стороны: "
            << e.a << ", " << e.b << ", " << e.c << ")" << endl;
    }

    try {
        double p = perimeter_custom_params(1, 2, 3); // не треугольник
        cout << "   Периметр (1,2,3) = " << p << endl;
    }
    catch (const TriangleException& e) {
        cout << "   Поймано исключение: " << e.message << " (стороны: "
            << e.a << ", " << e.b << ", " << e.c << ")" << endl;
    }
    cout << endl;

    // 4.3 Собственное исключение – наследник invalid_argument 
    cout << "4c. Собственное исключение (наследник invalid_argument):\n";
    try {
        double p = perimeter_custom_inherit(3, 4, 5);
        cout << "   Периметр (3,4,5) = " << p << endl;
    }
    catch (const TriangleInvalidArg& e) {
        cout << "   Поймано исключение: " << e.what() << " (стороны: "
            << e.a << ", " << e.b << ", " << e.c << ")" << endl;
    }

    try {
        double p = perimeter_custom_inherit(-2, 3, 4); // отрицательная сторона
        cout << "   Периметр (-2,3,4) = " << p << endl;
    }
    catch (const TriangleInvalidArg& e) {
        cout << "   Поймано исключение: " << e.what() << " (стороны: "
            << e.a << ", " << e.b << ", " << e.c << ")" << endl;
    }
    cout << endl;

    return 0;
}