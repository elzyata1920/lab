#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>

using namespace std;

// Базовый класс Liquid (жидкость)
class Liquid {
private:
    char name[50];    // название жидкости
    double density;   // плотность (г/см³)

public:
    // Конструктор по умолчанию
    Liquid() {
        strcpy(name, "Неизвестно");
        density = 1.0;
    }

    // Конструктор инициализации
    Liquid(const char* n, double d) {
        setName(n);
        setDensity(d);
    }

    // Конструктор копирования
    Liquid(const Liquid& other) {
        strcpy(name, other.name);
        density = other.density;
    }

    // Сеттеры с проверкой
    void setName(const char* n) {
        if (n == nullptr || strlen(n) == 0) {
            cerr << "Ошибка: название не может быть пустым!" << endl;
            strcpy(name, "Неизвестно");
        }
        else {
            strncpy(name, n, 49);
            name[49] = '\0';
        }
    }

    void setDensity(double d) {
        if (d <= 0) {
            cerr << "Ошибка: плотность должна быть положительной!" << endl;
            density = 1.0;
        }
        else {
            density = d;
        }
    }

    // Геттеры
    const char* getName() const { return name; }
    double getDensity() const { return density; }

    // Метод переназначения (изменение названия и плотности)
    void reassign(const char* n, double d) {
        setName(n);
        setDensity(d);
    }

    // Изменение плотности
    void changeDensity(double delta) {
        setDensity(density + delta);
    }

    // Преобразование в строку
    virtual string toString() const {
        ostringstream oss;
        oss << "Жидкость: " << name << ", плотность = " << density << " г/см³";
        return oss.str();
    }

    // Вывод на экран
    virtual void display() const {
        cout << toString() << endl;
    }

    // Виртуальный деструктор (для корректного удаления через указатель на базовый класс)
    virtual ~Liquid() {}
};

// Производный класс Alcohol (спирт)
class Alcohol : public Liquid {
private:
    double strength;   // крепость (%, 0..100)

public:
    // Конструктор по умолчанию
    Alcohol() : Liquid() {
        strength = 40.0; // стандартная водка :)
    }

    // Конструктор инициализации
    Alcohol(const char* n, double d, double s) : Liquid(n, d) {
        setStrength(s);
    }

    // Конструктор копирования
    Alcohol(const Alcohol& other) : Liquid(other) {
        strength = other.strength;
    }

    // Сеттер крепости с проверкой
    void setStrength(double s) {
        if (s < 0 || s > 100) {
            cerr << "Ошибка: крепость должна быть в пределах 0..100!" << endl;
            strength = 40.0;
        }
        else {
            strength = s;
        }
    }

    // Геттер крепости
    double getStrength() const { return strength; }

    // Метод переназначения (переопределение)
    void reassign(const char* n, double d, double s) {
        Liquid::reassign(n, d);   // вызов метода базового класса
        setStrength(s);
    }

    // Изменение крепости
    void changeStrength(double delta) {
        setStrength(strength + delta);
    }

    // Переопределение toString()
    string toString() const override {
        ostringstream oss;
        oss << Liquid::toString() << ", крепость = " << strength << "%";
        return oss.str();
    }

    // Переопределение display()
    void display() const override {
        cout << toString() << endl;
    }
};

// Демонстрация принципа подстановки
void demonstrateSubstitution(const Liquid& liq) {
    cout << "=== Принцип подстановки (функция принимает Liquid&) ===" << endl;
    liq.display();   // вызовется нужная версия display() благодаря виртуальности
}

int main() {
    setlocale(LC_ALL, "RU");

    // 1. Создание объектов базового класса
    cout << "1. Создание объектов Liquid:\n";
    Liquid water("Вода", 1.0);
    Liquid oil("Масло", 0.9);
    water.display();
    oil.display();
    cout << endl;

    // 2. Создание объектов производного класса
    cout << "2. Создание объектов Alcohol:\n";
    Alcohol vodka("Водка", 0.95, 40);
    Alcohol ethanol("Этанол", 0.789, 95.5);
    vodka.display();
    ethanol.display();
    cout << endl;

    // 3. Демонстрация методов изменения
    cout << "3. Изменение свойств объектов:\n";
    water.changeDensity(-0.1);
    cout << "Вода после уменьшения плотности на 0.1: " << water.toString() << endl;

    vodka.changeStrength(5);
    cout << "Водка после увеличения крепости на 5%: " << vodka.toString() << endl;

    vodka.reassign("Водка \"Столичная\"", 0.95, 40);
    cout << "Водка после переназначения: " << vodka.toString() << endl;
    cout << endl;

    // 4. Массив объектов базового класса (смешанный)
    cout << "4. Массив указателей на Liquid (смешанные типы):\n";
    Liquid* arr[4];
    arr[0] = &water;
    arr[1] = &oil;
    arr[2] = &vodka;
    arr[3] = &ethanol;

    for (int i = 0; i < 4; ++i) {
        cout << "Элемент " << i << ": ";
        arr[i]->display();   // полиморфный вызов
    }
    cout << endl;

    // 5. Демонстрация принципа подстановки
    cout << "5. Демонстрация принципа подстановки:\n";
    demonstrateSubstitution(water);   // передаётся Liquid
    demonstrateSubstitution(vodka);   // передаётся Alcohol
    cout << endl;

    // 6. Проверка обработки ошибок
    cout << "6. Проверка обработки ошибок:\n";
    Liquid badLiq("", -5.0);   // некорректные данные
    badLiq.display();

    Alcohol badAlc("", -1.0, 150);   // некорректные данные
    badAlc.display();
    cout << endl;

    // 7. Демонстрация конструктора копирования
    cout << "7. Конструктор копирования:\n";
    Alcohol vodkaCopy(vodka);
    vodkaCopy.setStrength(50);
    cout << "Оригинал: " << vodka.toString() << endl;
    cout << "Копия (с изменённой крепостью): " << vodkaCopy.toString() << endl;

    return 0;
}