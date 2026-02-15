#include <iostream>
#include <string>

using namespace std;

struct Pair {
    int first;   // числитель (целое положительное)
    int second;  // знаменатель (целое положительное)

    // Метод инициализации с проверкой корректности
    bool init(int f, int s) {
        if (f < 0 || s <= 0) {
            cout << "Ошибка: числитель должен быть >= 0, знаменатель > 0!" << endl;
            return false;
        }
        first = f;
        second = s;
        return true;
    }

    // Ввод с клавиатуры
    bool read() {
        cout << "Введите числитель (неотрицательное целое): ";
        cin >> first;
        cout << "Введите знаменатель (положительное целое): ";
        cin >> second;

        // Проверка корректности
        if (first < 0 || second <= 0) {
            cout << "Ошибка: числитель должен быть >= 0, знаменатель > 0!" << endl;
            return false;
        }
        return true;
    }

    // Вывод на экран
    void display() const {
        cout << "Дробь: " << first << "/" << second;
    }

    // Метод выделения целой части дроби
    bool ipart(int& result) const {
        if (second == 0) {
            cout << "Ошибка: деление на ноль!" << endl;
            return false;
        }

        result = first / second;  // целочисленное деление
        return true;
    }

    // Дополнительный метод: вывод дроби в смешанном виде
    void displayMixed() const {
        if (second == 0) {
            cout << "Ошибка: знаменатель равен нулю!";
            return;
        }

        int whole = first / second;
        int remainder = first % second;

        if (whole > 0 && remainder > 0) {
            cout << whole << " целых " << remainder << "/" << second;
        }
        else if (whole > 0) {
            cout << whole;
        }
        else {
            cout << first << "/" << second;
        }
    }
};

// Внешняя функция для создания структуры
bool make_Pair(int first, int second, Pair& result) {
    if (first < 0 || second <= 0) {
        cout << "Ошибка создания дроби: числитель должен быть >= 0, знаменатель > 0!" << endl;
        return false;
    }

    result.first = first;
    result.second = second;
    return true;
}

int main() {
    setlocale(LC_ALL, "RU");
    cout << "=== Лабораторная работа №1. Структура-пара. Задание №3 ===\n\n";

    cout << "1. Создание объекта с помощью метода init():\n";
    Pair p1;
    if (p1.init(7, 3)) {
        cout << "Объект p1: ";
        p1.display();

        int int_part;
        if (p1.ipart(int_part)) {
            cout << "\nЦелая часть дроби: " << int_part << endl;
        }

        cout << "Дробь в смешанном виде: ";
        p1.displayMixed();
        cout << endl;
    }
    cout << endl;

    cout << "2. Создание объекта с помощью функции make_Pair():\n";
    Pair p2;
    if (make_Pair(15, 4, p2)) {
        cout << "Объект p2: ";
        p2.display();

        int int_part;
        if (p2.ipart(int_part)) {
            cout << "\nЦелая часть дроби: " << int_part << endl;
        }

        cout << "Дробь в смешанном виде: ";
        p2.displayMixed();
        cout << endl;
    }
    cout << endl;

    cout << "3. Создание объекта с помощью метода read() (ввод с клавиатуры):\n";
    Pair p3;
    if (p3.read()) {
        cout << "Объект p3: ";
        p3.display();

        int int_part;
        if (p3.ipart(int_part)) {
            cout << "\nЦелая часть дроби: " << int_part << endl;
        }

        cout << "Дробь в смешанном виде: ";
        p3.displayMixed();
        cout << endl;
    }
    cout << endl;

    cout << "4. Демонстрация работы с массивом объектов:\n";
    const int SIZE = 4;
    Pair arr[SIZE];

    // Инициализация массива разными способами
    arr[0].init(8, 3);      // 8/3 = 2 целых 2/3
    make_Pair(12, 4, arr[1]); // 12/4 = 3
    arr[2].init(5, 7);      // 5/7 = 0 целых 5/7
    arr[3].init(17, 5);     // 17/5 = 3 целых 2/5

    cout << "\nМассив объектов дробей:\n";
    for (int i = 0; i < SIZE; i++) {
        cout << "arr[" << i << "] = ";
        arr[i].display();

        int int_part;
        if (arr[i].ipart(int_part)) {
            cout << " -> Целая часть: " << int_part;
        }

        cout << " -> Смешанный вид: ";
        arr[i].displayMixed();
        cout << endl;
    }
    cout << endl;

    cout << "5. Демонстрация различных случаев выделения целой части:\n";
    Pair test_cases[6];

    // Тестовые случаи
    test_cases[0].init(10, 3);      // 10/3 = 3 целых 1/3
    test_cases[1].init(0, 5);       // 0/5 = 0
    test_cases[2].init(9, 9);       // 9/9 = 1
    test_cases[3].init(1, 2);       // 1/2 = 0 целых 1/2
    test_cases[4].init(100, 33);    // 100/33 = 3 целых 1/33
    test_cases[5].init(16, 8);      // 16/8 = 2

    cout << "\nРезультаты выделения целой части:\n";
    for (int i = 0; i < 6; i++) {
        int int_part;
        if (test_cases[i].ipart(int_part)) {
            cout << test_cases[i].first << "/" << test_cases[i].second
                << " -> целая часть: " << int_part << endl;
        }
    }
    cout << endl;

    cout << "6. Проверка ошибочных случаев:\n";

    Pair error1;
    if (!error1.init(-5, 3)) {
        cout << "Правильно: отрицательный числитель недопустим\n";
    }

    Pair error2;
    if (!error2.init(5, 0)) {
        cout << "Правильно: нулевой знаменатель недопустим\n";
    }

    Pair error3;
    if (!error3.init(5, -2)) {
        cout << "Правильно: отрицательный знаменатель недопустим\n";
    }

    // Проверка метода ipart с нулевым знаменателем
    Pair error4;
    error4.first = 5;  // напрямую, чтобы обойти проверку init
    error4.second = 0;
    int dummy;
    if (!error4.ipart(dummy)) {
        cout << "Правильно: метод ipart обнаружил деление на ноль\n";
    }

    cout << "\n=== Демонстрация завершена ===\n";

    return 0;
}