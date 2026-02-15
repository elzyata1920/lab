#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>

using namespace std;

// Шаблонная функция сортировки подсчётом (только для целочисленных типов)
template<typename T>
void sortCount(T* arr, int size) {
    static_assert(is_integral<T>::value,
        "Counting sort требует целочисленный тип");

    if (size <= 0) return;

    // Находим минимальное и максимальное значения
    T minVal = arr[0];
    T maxVal = arr[0];
    for (int i = 1; i < size; ++i) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    // Размер диапазона значений
    size_t range = static_cast<size_t>(maxVal - minVal + 1);

    // Вектор счётчиков
    vector<size_t> count(range, 0);

    // Подсчёт количества каждого элемента
    for (int i = 0; i < size; ++i) {
        count[static_cast<size_t>(arr[i] - minVal)]++;
    }

    // Запись отсортированных элементов обратно в массив
    size_t index = 0;
    for (size_t i = 0; i < range; ++i) {
        for (size_t j = 0; j < count[i]; ++j) {
            arr[index++] = static_cast<T>(i + minVal);
        }
    }
}

int main() {
    setlocale(LC_ALL, "RU");

    cout << "=== Сортировка подсчётом для массива int ===\n";
    int arrInt[] = { 5, 2, 8, 2, 1, 0, 4, 4, 9, 3, -1, -3 };
    int sizeInt = sizeof(arrInt) / sizeof(arrInt[0]);

    cout << "Исходный массив: ";
    for (int i = 0; i < sizeInt; ++i)
        cout << arrInt[i] << " ";
    cout << endl;

    sortCount(arrInt, sizeInt);

    cout << "Отсортированный: ";
    for (int i = 0; i < sizeInt; ++i)
        cout << arrInt[i] << " ";
    cout << endl;

    cout << "\n=== Сортировка подсчётом для массива unsigned char ===\n";
    unsigned char arrChar[] = { 5, 2, 8, 2, 1, 0, 4, 4, 9, 3 };
    int sizeChar = sizeof(arrChar) / sizeof(arrChar[0]);

    cout << "Исходный массив: ";
    for (int i = 0; i < sizeChar; ++i)
        cout << static_cast<int>(arrChar[i]) << " ";
    cout << endl;

    sortCount(arrChar, sizeChar);

    cout << "Отсортированный: ";
    for (int i = 0; i < sizeChar; ++i)
        cout << static_cast<int>(arrChar[i]) << " ";
    cout << endl;

    return 0;
}