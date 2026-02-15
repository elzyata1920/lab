#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 100;

class Array {
protected:
    unsigned char* data;
    int size;

public:
    Array() : data(nullptr), size(0) {}

    Array(int n, unsigned char initValue = 0) : size(n) {
        if (n < 0 || n > MAX_SIZE) {
            cerr << "Ошибка: недопустимый размер массива (0.." << MAX_SIZE << ")" << endl;
            size = 0;
            data = nullptr;
            return;
        }
        if (n > 0) {
            data = new unsigned char[n];
            for (int i = 0; i < n; ++i)
                data[i] = initValue;
        }
        else {
            data = nullptr;
        }
    }

    Array(const Array& other) : size(other.size) {
        if (other.data) {
            data = new unsigned char[size];
            memcpy(data, other.data, size);
        }
        else {
            data = nullptr;
        }
    }

    virtual ~Array() {
        delete[] data;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            if (other.data) {
                data = new unsigned char[size];
                memcpy(data, other.data, size);
            }
            else {
                data = nullptr;
            }
        }
        return *this;
    }

    unsigned char& operator[](int index) {
        if (index < 0 || index >= size) {
            cerr << "Ошибка: индекс " << index << " вне диапазона [0, " << size - 1 << "]" << endl;
            static unsigned char dummy = 0;
            return dummy;
        }
        return data[index];
    }

    const unsigned char& operator[](int index) const {
        if (index < 0 || index >= size) {
            cerr << "Ошибка: индекс " << index << " вне диапазона [0, " << size - 1 << "]" << endl;
            static unsigned char dummy = 0;
            return dummy;
        }
        return data[index];
    }

    int getSize() const { return size; }
    static int getMaxSize() { return MAX_SIZE; }

    // Виртуальная функция, возвращающая указатель на новый объект Array (динамическое выделение)
    virtual Array* add(const Array& other) const {
        int resultSize = max(size, other.size);
        Array* result = new Array(resultSize, 0);
        for (int i = 0; i < resultSize; ++i) {
            unsigned char val1 = (i < size) ? data[i] : 0;
            unsigned char val2 = (i < other.size) ? other.data[i] : 0;
            (*result)[i] = val1 + val2;
        }
        return result;
    }

    // Перегрузка оператора + для удобства, но возвращает указатель
    Array* operator+(const Array& other) const {
        return add(other);
    }

    virtual string toString() const {
        ostringstream oss;
        oss << "Array[" << size << "]: ";
        for (int i = 0; i < size; ++i)
            oss << (int)data[i] << " ";
        return oss.str();
    }

    virtual void display() const {
        cout << toString() << endl;
    }
};

class Decimal : public Array {
public:
    Decimal() : Array() {}
    Decimal(int n, unsigned char initValue = 0) : Array(n, initValue) {
        for (int i = 0; i < size; ++i)
            if (data[i] > 9) data[i] = initValue % 10;
    }
    Decimal(const Decimal& other) : Array(other) {}

    Array* add(const Array& other) const override {
        const Decimal* otherDec = dynamic_cast<const Decimal*>(&other);
        if (!otherDec) {
            cerr << "Ошибка: Decimal можно складывать только с Decimal!" << endl;
            return new Array(); // пустой результат
        }

        int resultSize = max(size, otherDec->size);
        Decimal* result = new Decimal(resultSize, 0);
        int carry = 0;

        for (int i = 0; i < resultSize; ++i) {
            unsigned char val1 = (i < size) ? data[i] : 0;
            unsigned char val2 = (i < otherDec->size) ? otherDec->data[i] : 0;
            int sum = val1 + val2 + carry;
            (*result)[i] = sum % 10;
            carry = sum / 10;
        }

        if (carry > 0 && resultSize < MAX_SIZE) {
            Decimal* newResult = new Decimal(resultSize + 1, 0);
            for (int i = 0; i < resultSize; ++i)
                (*newResult)[i] = (*result)[i];
            (*newResult)[resultSize] = carry;
            delete result;
            return newResult;
        }
        return result;
    }

    string toString() const override {
        ostringstream oss;
        oss << "Decimal[" << size << "]: ";
        for (int i = size - 1; i >= 0; --i)
            oss << (int)data[i];
        return oss.str();
    }
};

class BitString : public Array {
public:
    BitString() : Array() {}
    BitString(int n, unsigned char initValue = 0) : Array(n, initValue) {
        for (int i = 0; i < size; ++i)
            if (data[i] > 1) data[i] = initValue % 2;
    }
    BitString(const BitString& other) : Array(other) {}

    Array* add(const Array& other) const override {
        const BitString* otherBits = dynamic_cast<const BitString*>(&other);
        if (!otherBits) {
            cerr << "Ошибка: BitString можно складывать только с BitString!" << endl;
            return new Array();
        }

        int resultSize = max(size, otherBits->size);
        BitString* result = new BitString(resultSize, 0);
        int carry = 0;

        for (int i = 0; i < resultSize; ++i) {
            unsigned char val1 = (i < size) ? data[i] : 0;
            unsigned char val2 = (i < otherBits->size) ? otherBits->data[i] : 0;
            int sum = val1 + val2 + carry;
            (*result)[i] = sum % 2;
            carry = sum / 2;
        }

        if (carry > 0 && resultSize < MAX_SIZE) {
            BitString* newResult = new BitString(resultSize + 1, 0);
            for (int i = 0; i < resultSize; ++i)
                (*newResult)[i] = (*result)[i];
            (*newResult)[resultSize] = carry;
            delete result;
            return newResult;
        }
        return result;
    }

    string toString() const override {
        ostringstream oss;
        oss << "BitString[" << size << "]: ";
        for (int i = size - 1; i >= 0; --i)
            oss << (int)data[i];
        oss << " (binary)";
        return oss.str();
    }
};

void demonstrateVirtualAdd(Array* a1, Array* a2) {
    cout << "Вызов add() через указатель Array*:" << endl;
    Array* result = a1->add(*a2);
    result->display();
    delete result; // важно освободить память
}

int main() {
    setlocale(LC_ALL, "RU");


    // Базовые массивы
    Array arr1(5, 3), arr2(3, 7);
    for (int i = 0; i < arr1.getSize(); ++i) arr1[i] = i + 1;
    for (int i = 0; i < arr2.getSize(); ++i) arr2[i] = i + 5;
    cout << "Array объекты:\n";
    arr1.display();
    arr2.display();
    cout << endl;

    // Decimal
    Decimal dec1(4, 0); dec1[0] = 5; dec1[1] = 3; dec1[2] = 8; dec1[3] = 1;
    Decimal dec2(3, 0); dec2[0] = 9; dec2[1] = 7; dec2[2] = 2;
    cout << "Decimal объекты:\n";
    dec1.display();
    dec2.display();
    cout << endl;

    // BitString
    BitString bits1(4, 0); bits1[0] = 1; bits1[1] = 0; bits1[2] = 1; bits1[3] = 1;
    BitString bits2(3, 0); bits2[0] = 1; bits2[1] = 1; bits2[2] = 0;
    cout << "BitString объекты:\n";
    bits1.display();
    bits2.display();
    cout << endl;

    // Сложение Decimal + Decimal (возвращает указатель)
    cout << "Сложение Decimal + Decimal:\n";
    Array* decSum = dec1.add(dec2); // или dec1 + dec2
    decSum->display();
    delete decSum;
    cout << endl;

    // Сложение BitString + BitString
    cout << "Сложение BitString + BitString:\n";
    Array* bitsSum = bits1.add(bits2);
    bitsSum->display();
    delete bitsSum;
    cout << endl;

    // Через указатели на базовый класс
    cout << "Через указатели на Array:\n";
    demonstrateVirtualAdd(&dec1, &dec2);
    demonstrateVirtualAdd(&bits1, &bits2);
    cout << endl;

    // Попытка сложения разных типов (должна выдать ошибку)
    cout << "Попытка сложения Decimal + BitString (разные типы):\n";
    Array* wrongSum = dec1.add(bits1);
    wrongSum->display(); // пустой массив
    delete wrongSum;
    cout << endl;

    // Массив указателей на базовый класс
    cout << "Массив указателей Array* с объектами разных типов:\n";
    Array* objects[4] = { &arr1, &dec1, &bits1, &dec2 };
    for (int i = 0; i < 4; ++i) {
        cout << "Объект " << i << ": ";
        objects[i]->display();
    }
    cout << endl;

    // Проверка индексации
    cout << "Проверка индексации:\n";
    cout << "dec1[0] = " << (int)dec1[0] << endl;
    cout << "bits1[2] = " << (int)bits1[2] << endl;
    cout << "Попытка выхода за границу dec1[10]: ";
    unsigned char val = dec1[10];
    cout << (int)val << endl;

    return 0;
}