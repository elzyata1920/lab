#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <algorithm>   // для min, max

using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class ModelWindowOp {
private:
    char title[100];
    int x, y;
    int width, height;
    char color[50];
    bool visible;
    bool bordered;

    bool checkBounds(int newX, int newY, int newWidth, int newHeight) const {
        return (newX >= 0 && newY >= 0 &&
            newX + newWidth <= SCREEN_WIDTH &&
            newY + newHeight <= SCREEN_HEIGHT);
    }

public:
    // Конструктор без аргументов
    ModelWindowOp() {
        strcpy(title, "Новое окно");
        x = 100; y = 100;
        width = 400; height = 300;
        strcpy(color, "белый");
        visible = true;
        bordered = true;
    }

    // Конструктор инициализации
    ModelWindowOp(const char* t, int posX, int posY, int w, int h,
        const char* c, bool vis = true, bool border = true) {
        strncpy(title, t, 99); title[99] = '\0';
        if (posX < 0) posX = 0;
        if (posY < 0) posY = 0;
        if (w <= 0) w = 100;
        if (h <= 0) h = 100;
        if (posX + w > SCREEN_WIDTH) {
            posX = SCREEN_WIDTH - w;
            if (posX < 0) { posX = 0; w = SCREEN_WIDTH; }
        }
        if (posY + h > SCREEN_HEIGHT) {
            posY = SCREEN_HEIGHT - h;
            if (posY < 0) { posY = 0; h = SCREEN_HEIGHT; }
        }
        x = posX; y = posY; width = w; height = h;
        strncpy(color, c, 49); color[49] = '\0';
        visible = vis; bordered = border;
    }

    // Конструктор копирования
    ModelWindowOp(const ModelWindowOp& other) {
        strcpy(title, other.title);
        x = other.x; y = other.y; width = other.width; height = other.height;
        strcpy(color, other.color);
        visible = other.visible; bordered = other.bordered;
    }

    // Сеттеры (с проверкой)
    void setTitle(const char* t) { strncpy(title, t, 99); title[99] = '\0'; }
    void setX(int newX) { if (checkBounds(newX, y, width, height)) x = newX; }
    void setY(int newY) { if (checkBounds(x, newY, width, height)) y = newY; }
    void setWidth(int newWidth) { if (newWidth > 0 && checkBounds(x, y, newWidth, height)) width = newWidth; }
    void setHeight(int newHeight) { if (newHeight > 0 && checkBounds(x, y, width, newHeight)) height = newHeight; }
    void setColor(const char* c) { strncpy(color, c, 49); color[49] = '\0'; }
    void setVisible(bool vis) { visible = vis; }
    void setBordered(bool border) { bordered = border; }

    // Геттеры
    const char* getTitle() const { return title; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const char* getColor() const { return color; }
    bool getVisible() const { return visible; }
    bool getBordered() const { return bordered; }

    // ---------- Перегрузка операций ----------

    // Операция присваивания
    ModelWindowOp& operator=(const ModelWindowOp& other) {
        if (this != &other) {
            strcpy(title, other.title);
            x = other.x; y = other.y;
            width = other.width; height = other.height;
            strcpy(color, other.color);
            visible = other.visible;
            bordered = other.bordered;
        }
        return *this;
    }

    // Сравнение на равенство (все поля)
    bool operator==(const ModelWindowOp& other) const {
        return (strcmp(title, other.title) == 0 &&
            x == other.x && y == other.y &&
            width == other.width && height == other.height &&
            strcmp(color, other.color) == 0 &&
            visible == other.visible && bordered == other.bordered);
    }

    bool operator!=(const ModelWindowOp& other) const {
        return !(*this == other);
    }

    // Сравнение по площади окна
    bool operator<(const ModelWindowOp& other) const {
        return (width * height) < (other.width * other.height);
    }
    bool operator>(const ModelWindowOp& other) const {
        return (width * height) > (other.width * other.height);
    }
    bool operator<=(const ModelWindowOp& other) const {
        return !(*this > other);
    }
    bool operator>=(const ModelWindowOp& other) const {
        return !(*this < other);
    }

    // Сложение (объединение окон)
    ModelWindowOp operator+(const ModelWindowOp& other) const {
        int newX = min(x, other.x);
        int newY = min(y, other.y);
        int newWidth = max(x + width, other.x + other.width) - newX;
        int newHeight = max(y + height, other.y + other.height) - newY;

        if (newX < 0) newX = 0;
        if (newY < 0) newY = 0;
        if (newX + newWidth > SCREEN_WIDTH)  newWidth = SCREEN_WIDTH - newX;
        if (newY + newHeight > SCREEN_HEIGHT) newHeight = SCREEN_HEIGHT - newY;

        return ModelWindowOp("Объединённое окно", newX, newY,
            newWidth, newHeight, "серый", true, true);
    }

    // Вычитание (пересечение окон)
    ModelWindowOp operator-(const ModelWindowOp& other) const {
        int left = max(x, other.x);
        int top = max(y, other.y);
        int right = min(x + width, other.x + other.width);
        int bottom = min(y + height, other.y + other.height);

        int newWidth = (right > left) ? (right - left) : 0;
        int newHeight = (bottom > top) ? (bottom - top) : 0;

        if (newWidth <= 0 || newHeight <= 0) {
            return ModelWindowOp("Пустое пересечение", 0, 0, 0, 0, "чёрный", false, false);
        }
        return ModelWindowOp("Пересечение окон", left, top,
            newWidth, newHeight, "красный", true, true);
    }

    // Умножение на коэффициент (масштабирование)
    ModelWindowOp operator*(double factor) const {
        if (factor <= 0) return *this;
        int newWidth = static_cast<int>(width * factor);
        int newHeight = static_cast<int>(height * factor);

        if (newWidth > SCREEN_WIDTH)  newWidth = SCREEN_WIDTH;
        if (newHeight > SCREEN_HEIGHT) newHeight = SCREEN_HEIGHT;
        if (x + newWidth > SCREEN_WIDTH)  newWidth = SCREEN_WIDTH - x;
        if (y + newHeight > SCREEN_HEIGHT) newHeight = SCREEN_HEIGHT - y;

        ModelWindowOp result = *this;
        result.setWidth(newWidth);
        result.setHeight(newHeight);
        return result;
    }

    // Деление на коэффициент (уменьшение)
    ModelWindowOp operator/(double factor) const {
        if (factor <= 0) return *this;
        return *this * (1.0 / factor);
    }

    // Составные операции присваивания
    ModelWindowOp& operator+=(const ModelWindowOp& other) {
        *this = *this + other;
        return *this;
    }
    ModelWindowOp& operator-=(const ModelWindowOp& other) {
        *this = *this - other;
        return *this;
    }
    ModelWindowOp& operator*=(double factor) {
        *this = *this * factor;
        return *this;
    }
    ModelWindowOp& operator/=(double factor) {
        *this = *this / factor;
        return *this;
    }

    // Инкремент / декремент (увеличение/уменьшение размеров на 10%)
    ModelWindowOp& operator++() {    // префиксный
        *this *= 1.1;
        return *this;
    }
    ModelWindowOp operator++(int) {  // постфиксный
        ModelWindowOp temp = *this;
        ++(*this);
        return temp;
    }
    ModelWindowOp& operator--() {
        *this /= 1.1;
        return *this;
    }
    ModelWindowOp operator--(int) {
        ModelWindowOp temp = *this;
        --(*this);
        return temp;
    }

    // Дружественные функции для ввода/вывода
    friend ostream& operator<<(ostream& os, const ModelWindowOp& win);
    friend istream& operator>>(istream& is, ModelWindowOp& win);
    friend ModelWindowOp operator*(double factor, const ModelWindowOp& win); // коммутативность
};

// Реализация дружественных операторов
ostream& operator<<(ostream& os, const ModelWindowOp& win) {
    os << "Окно '" << win.title << "' [" << win.x << "," << win.y << "] "
        << win.width << "x" << win.height << " цвет: " << win.color << " "
        << (win.visible ? "видимое" : "скрытое") << " "
        << (win.bordered ? "с рамкой" : "без рамки")
        << " (площадь: " << win.width * win.height << ")";
    return os;
}

istream& operator>>(istream& is, ModelWindowOp& win) {
    cout << "Введите заголовок окна: ";
    is.ignore();
    is.getline(win.title, 100);

    cout << "Введите координату X: "; is >> win.x;
    cout << "Введите координату Y: "; is >> win.y;
    cout << "Введите ширину окна: ";  is >> win.width;
    cout << "Введите высоту окна: ";  is >> win.height;

    cout << "Введите цвет окна: ";
    is.ignore();
    is.getline(win.color, 50);

    char choice;
    cout << "Окно видимое? (y/n): "; cin >> choice;
    win.visible = (choice == 'y' || choice == 'Y');
    cout << "Окно с рамкой? (y/n): "; cin >> choice;
    win.bordered = (choice == 'y' || choice == 'Y');

    return is;
}

ModelWindowOp operator*(double factor, const ModelWindowOp& win) {
    return win * factor;   // коммутативность
}

int main() {
    setlocale(LC_ALL, "RU");

    // Создание объектов
    ModelWindowOp op1;
    ModelWindowOp op2("Окно X", 100, 100, 400, 300, "синий");
    ModelWindowOp op3("Окно Y", 300, 200, 600, 400, "зелёный");
    ModelWindowOp op4(op2);   // конструктор копирования

    cout << "Созданные объекты:\n";
    cout << "op1: " << op1 << endl;
    cout << "op2: " << op2 << endl;
    cout << "op3: " << op3 << endl;
    cout << "op4 (копия op2): " << op4 << endl;

    // Операция присваивания
    cout << "\n1. Присваивание (op1 = op2):\n";
    op1 = op2;
    op1.setTitle("Копия X");
    cout << "op1 после присваивания: " << op1 << endl;

    // Операции сравнения
    cout << "\n2. Сравнение:\n";
    cout << "op2 == op4? " << (op2 == op4 ? "да" : "нет") << endl;
    cout << "op2 != op3? " << (op2 != op3 ? "да" : "нет") << endl;
    cout << "op2 < op3 (по площади)? " << (op2 < op3 ? "да" : "нет") << endl;
    cout << "op2 > op3? " << (op2 > op3 ? "да" : "нет") << endl;

    // Арифметические операции
    cout << "\n3. Арифметические операции:\n";
    ModelWindowOp sum = op2 + op3;
    cout << "op2 + op3 (объединение): " << sum << endl;

    ModelWindowOp diff = op2 - op3;
    cout << "op2 - op3 (пересечение): " << diff << endl;

    ModelWindowOp scaled = op2 * 1.5;
    cout << "op2 * 1.5 (масштабирование): " << scaled << endl;

    ModelWindowOp shrunk = op3 / 2.0;
    cout << "op3 / 2.0 (уменьшение): " << shrunk << endl;

    // Коммутативное умножение
    ModelWindowOp comm = 0.5 * op2;
    cout << "0.5 * op2: " << comm << endl;

    // Составные операции присваивания
    cout << "\n4. Составные операции:\n";
    ModelWindowOp comp = op2;
    comp += op3;
    cout << "comp после += op3: " << comp << endl;
    comp -= op3;
    cout << "comp после -= op3: " << comp << endl;
    comp *= 1.2;
    cout << "comp после *= 1.2: " << comp << endl;
    comp /= 1.5;
    cout << "comp после /= 1.5: " << comp << endl;

    // Инкремент / декремент
    cout << "\n5. Инкремент и декремент:\n";
    ModelWindowOp inc("Inc", 200, 200, 200, 200, "красный");
    cout << "Исходное: " << inc << endl;
    ++inc;
    cout << "После ++inc: " << inc << endl;
    inc++;
    cout << "После inc++: " << inc << endl;
    --inc;
    cout << "После --inc: " << inc << endl;
    inc--;
    cout << "После inc--: " << inc << endl;

    // Массив объектов и сортировка
    cout << "\n6. Массив объектов и сортировка по площади:\n";
    ModelWindowOp arr[3] = {
        ModelWindowOp("M1", 10, 10, 300, 250, "жёлтый"),
        ModelWindowOp("M2", 50, 50, 400, 350, "голубой"),
        ModelWindowOp("M3", 100, 100, 500, 450, "розовый")
    };
    cout << "До сортировки:\n";
    for (int i = 0; i < 3; ++i)
        cout << "  " << arr[i] << endl;

    // Сортировка пузырьком по возрастанию площади (используем operator>)
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2 - i; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);

    cout << "После сортировки по возрастанию площади:\n";
    for (int i = 0; i < 3; ++i)
        cout << "  " << arr[i] << endl;

    // Ввод с клавиатуры через перегруженный оператор >>
    cout << "\n7. Ввод объекта с помощью operator>>:\n";
    ModelWindowOp inputWin;
    cin >> inputWin;
    cout << "Вы ввели: " << inputWin << endl;


    return 0;
}