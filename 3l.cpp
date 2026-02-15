#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <sstream>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class ModelWindow {
private:
    char title[100];
    int x, y;
    int width, height;
    char color[50];
    bool visible;
    bool bordered;

public:
    // Конструктор без аргументов (по умолчанию)
    ModelWindow() {
        strcpy(title, "Новое окно");
        x = 100; y = 100;
        width = 400; height = 300;
        strcpy(color, "белый");
        visible = true;
        bordered = true;
    }

    // Конструктор инициализации
    ModelWindow(const char* t, int posX, int posY, int w, int h,
        const char* c, bool vis = true, bool border = true) {
        strncpy(title, t, 99);
        title[99] = '\0';

        // Корректировка координат и размеров, чтобы окно не выходило за экран
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

        x = posX; y = posY;
        width = w; height = h;

        strncpy(color, c, 49);
        color[49] = '\0';
        visible = vis;
        bordered = border;
    }

    // Конструктор копирования
    ModelWindow(const ModelWindow& other) {
        strcpy(title, other.title);
        x = other.x; y = other.y;
        width = other.width; height = other.height;
        strcpy(color, other.color);
        visible = other.visible;
        bordered = other.bordered;
    }

    // Проверка границ
    bool checkBounds(int newX, int newY, int newWidth, int newHeight) const {
        return (newX >= 0 && newY >= 0 &&
            newX + newWidth <= SCREEN_WIDTH &&
            newY + newHeight <= SCREEN_HEIGHT);
    }

    // Установка заголовка
    void setTitle(const char* t) {
        strncpy(title, t, 99);
        title[99] = '\0';
    }

    // Установка позиции с проверкой
    bool setPosition(int newX, int newY) {
        if (checkBounds(newX, newY, width, height)) {
            x = newX;
            y = newY;
            return true;
        }
        return false;
    }

    // Установка размеров с проверкой
    bool setSize(int newWidth, int newHeight) {
        if (newWidth <= 0 || newHeight <= 0) return false;
        if (checkBounds(x, y, newWidth, newHeight)) {
            width = newWidth;
            height = newHeight;
            return true;
        }
        return false;
    }

    // Изменение цвета
    void setColor(const char* c) {
        strncpy(color, c, 49);
        color[49] = '\0';
    }

    void setVisible(bool vis) { visible = vis; }
    void setBordered(bool border) { bordered = border; }

    // Перемещение
    bool moveHorizontally(int dx) { return setPosition(x + dx, y); }
    bool moveVertically(int dy) { return setPosition(x, y + dy); }
    bool resizeWidth(int newWidth) { return setSize(newWidth, height); }
    bool resizeHeight(int newHeight) { return setSize(width, newHeight); }

    // Геттеры
    const char* getTitle() const { return title; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const char* getColor() const { return color; }
    bool isVisible() const { return visible; }
    bool isBordered() const { return bordered; }

    // Ввод с клавиатуры
    void read() {
        cout << "Введите заголовок окна: ";
        cin.ignore();
        cin.getline(title, 100);

        cout << "Введите координату X (0-" << SCREEN_WIDTH << "): ";
        cin >> x;
        cout << "Введите координату Y (0-" << SCREEN_HEIGHT << "): ";
        cin >> y;

        cout << "Введите ширину окна: ";
        cin >> width;
        cout << "Введите высоту окна: ";
        cin >> height;

        cout << "Введите цвет окна: ";
        cin.ignore();
        cin.getline(color, 50);

        char choice;
        cout << "Окно видимое? (y/n): ";
        cin >> choice;
        visible = (choice == 'y' || choice == 'Y');

        cout << "Окно с рамкой? (y/n): ";
        cin >> choice;
        bordered = (choice == 'y' || choice == 'Y');

        // Корректировка после ввода
        if (!checkBounds(x, y, width, height)) {
            cout << "Внимание: окно выходит за границы экрана, корректируем...\n";
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x + width > SCREEN_WIDTH)  x = SCREEN_WIDTH - width;
            if (y + height > SCREEN_HEIGHT) y = SCREEN_HEIGHT - height;
        }
    }

    // Вывод на экран
    void display() const {
        cout << "\n=== Информация об окне ===\n"
            << "Заголовок: " << title << '\n'
            << "Позиция: (" << x << ", " << y << ")\n"
            << "Размеры: " << width << " x " << height << '\n'
            << "Цвет: " << color << '\n'
            << "Видимость: " << (visible ? "видимое" : "невидимое") << '\n'
            << "Рамка: " << (bordered ? "с рамкой" : "без рамки") << '\n';
    }

    // Преобразование в строку
    string toString() const {
        ostringstream oss;
        oss << "Окно '" << title << "' [" << x << "," << y << "] "
            << width << "x" << height << " цвет: " << color << " "
            << (visible ? "видимое" : "скрытое") << " "
            << (bordered ? "с рамкой" : "без рамки");
        return oss.str();
    }
};

int main() {
    setlocale(LC_ALL, "RU");

    // 1. Конструктор без аргументов
    cout << "1. Конструктор по умолчанию:\n";
    ModelWindow w1;
    w1.display();

    // 2. Конструктор инициализации
    cout << "\n2. Конструктор инициализации:\n";
    ModelWindow w2("Главное окно", 200, 150, 800, 600, "синий", true, true);
    w2.display();

    // 3. Конструктор копирования
    cout << "\n3. Конструктор копирования (копия w2 с изменёнными полями):\n";
    ModelWindow w3(w2);
    w3.setTitle("Копия главного окна");
    w3.setColor("зелёный");
    w3.display();

    // 4. Массив объектов
    cout << "\n4. Массив объектов ModelWindow:\n";
    ModelWindow arr[3] = {
        ModelWindow("Окно A", 50, 50, 300, 200, "красный"),
        ModelWindow("Окно B", 400, 300, 500, 400, "жёлтый"),
        ModelWindow()   // по умолчанию
    };
    for (int i = 0; i < 3; ++i) {
        cout << "\nЭлемент " << i << ": " << arr[i].toString() << endl;
    }

    // 5. Демонстрация методов перемещения и изменения размеров
    cout << "\n5. Демонстрация операций с окном w2:\n";
    cout << "Перемещаем на 50 вправо: " << (w2.moveHorizontally(50) ? "успешно" : "не удалось") << endl;
    cout << "Увеличиваем ширину до 900: " << (w2.resizeWidth(900) ? "успешно" : "не удалось") << endl;
    w2.display();

    // 6. Проверка границ (попытка выйти за экран)
    cout << "\n6. Проверка границ:\n";
    ModelWindow edge("На границе", 1850, 1000, 200, 200, "оранжевый");
    cout << "Исходное окно: " << edge.toString() << endl;
    cout << "Попытка переместить на 100 вправо: " << (edge.moveHorizontally(100) ? "успешно" : "не удалось") << endl;
    edge.display();

    // 7. Ввод с клавиатуры
    cout << "\n7. Ввод нового окна с клавиатуры:\n";
    ModelWindow inputWin;
    inputWin.read();
    cout << "Введённое окно:" << inputWin.toString() << endl;

    return 0;
}