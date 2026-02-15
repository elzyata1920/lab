#define _CRT_SECURE_NO_WARNINGS  // Отключаем предупреждения о небезопасных функциях

#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

//Способ 1

class ModelWindow {
private:
    char title[100];
    int x, y;
    int width, height;
    char color[50];
    bool visible;
    bool bordered;

public:
    ModelWindow() {
        strcpy(title, "Новое окно");
        x = y = 100;
        width = 400;
        height = 300;
        strcpy(color, "белый");
        visible = true;
        bordered = true;
    }

    ModelWindow(const char* t, int posX, int posY, int w, int h,
        const char* c, bool vis = true, bool border = true) {
        strncpy(title, t, 99);
        title[99] = '\0';

        // Корректировка позиции и размеров
        if (posX < 0) posX = 0;
        if (posY < 0) posY = 0;
        if (w <= 0) w = 100;
        if (h <= 0) h = 100;

        if (posX + w > SCREEN_WIDTH) {
            posX = SCREEN_WIDTH - w;
            if (posX < 0) {
                posX = 0;
                w = SCREEN_WIDTH;
            }
        }
        if (posY + h > SCREEN_HEIGHT) {
            posY = SCREEN_HEIGHT - h;
            if (posY < 0) {
                posY = 0;
                h = SCREEN_HEIGHT;
            }
        }

        x = posX;
        y = posY;
        width = w;
        height = h;

        strncpy(color, c, 49);
        color[49] = '\0';
        visible = vis;
        bordered = border;
    }

    ModelWindow(const ModelWindow& other) {
        strcpy(title, other.title);
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;
        strcpy(color, other.color);
        visible = other.visible;
        bordered = other.bordered;
    }

    // Методы доступа и операции (как в предыдущем коде)
    bool checkBounds(int newX, int newY, int newWidth, int newHeight) const {
        return (newX >= 0 && newY >= 0 &&
            newX + newWidth <= SCREEN_WIDTH &&
            newY + newHeight <= SCREEN_HEIGHT);
    }

    void setTitle(const char* t) {
        strncpy(title, t, 99);
        title[99] = '\0';
    }

    bool setPosition(int newX, int newY) {
        if (checkBounds(newX, newY, width, height)) {
            x = newX;
            y = newY;
            return true;
        }
        return false;
    }

    bool setSize(int newWidth, int newHeight) {
        if (newWidth <= 0 || newHeight <= 0) return false;
        if (checkBounds(x, y, newWidth, newHeight)) {
            width = newWidth;
            height = newHeight;
            return true;
        }
        return false;
    }

    void setColor(const char* c) {
        strncpy(color, c, 49);
        color[49] = '\0';
    }

    void setVisible(bool vis) { visible = vis; }
    void setBordered(bool border) { bordered = border; }

    bool moveHorizontally(int dx) { return setPosition(x + dx, y); }
    bool moveVertically(int dy) { return setPosition(x, y + dy); }
    bool resizeWidth(int newWidth) { return setSize(newWidth, height); }
    bool resizeHeight(int newHeight) { return setSize(width, newHeight); }

    const char* getTitle() const { return title; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const char* getColor() const { return color; }
    bool isVisible() const { return visible; }
    bool isBordered() const { return bordered; }

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

        if (!checkBounds(x, y, width, height)) {
            cout << "Внимание: окно выходит за границы экрана, корректируем..." << endl;
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x + width > SCREEN_WIDTH) x = SCREEN_WIDTH - width;
            if (y + height > SCREEN_HEIGHT) y = SCREEN_HEIGHT - height;
        }
    }

    void display() const {
        cout << "\n=== Информация об окне ===" << endl;
        cout << "Заголовок: " << title << endl;
        cout << "Позиция: (" << x << ", " << y << ")" << endl;
        cout << "Размеры: " << width << " x " << height << endl;
        cout << "Цвет: " << color << endl;
        cout << "Видимость: " << (visible ? "видимое" : "невидимое") << endl;
        cout << "Рамка: " << (bordered ? "с рамкой" : "без рамки") << endl;
    }

    string toString() const {
        string result = "Окно '";
        result += title;
        result += "' [" + to_string(x) + "," + to_string(y) + "] ";
        result += to_string(width) + "x" + to_string(height);
        result += " цвет: " + string(color);
        result += " " + string(visible ? "видимое" : "скрытое");
        result += " " + string(bordered ? "с рамкой" : "без рамки");
        return result;
    }
};

// Способ 2
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
    ModelWindowOp() {
        strcpy(title, "Новое окно");
        x = y = 100;
        width = 400;
        height = 300;
        strcpy(color, "белый");
        visible = true;
        bordered = true;
    }

    ModelWindowOp(const char* t, int posX, int posY, int w, int h,
        const char* c, bool vis = true, bool border = true) {
        strncpy(title, t, 99);
        title[99] = '\0';

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

        x = posX;
        y = posY;
        width = w;
        height = h;

        strncpy(color, c, 49);
        color[49] = '\0';
        visible = vis;
        bordered = border;
    }

    ModelWindowOp(const ModelWindowOp& other) {
        strcpy(title, other.title);
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;
        strcpy(color, other.color);
        visible = other.visible;
        bordered = other.bordered;
    }

    // Сеттеры
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

    // Перегрузка операций
    ModelWindowOp& operator=(const ModelWindowOp& other) {
        if (this != &other) {
            strcpy(title, other.title);
            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
            strcpy(color, other.color);
            visible = other.visible;
            bordered = other.bordered;
        }
        return *this;
    }

    bool operator==(const ModelWindowOp& other) const {
        return (strcmp(title, other.title) == 0 &&
            x == other.x && y == other.y &&
            width == other.width && height == other.height &&
            strcmp(color, other.color) == 0 &&
            visible == other.visible && bordered == other.bordered);
    }

    bool operator!=(const ModelWindowOp& other) const { return !(*this == other); }

    bool operator<(const ModelWindowOp& other) const {
        return (width * height) < (other.width * other.height);
    }

    bool operator>(const ModelWindowOp& other) const {
        return (width * height) > (other.width * other.height);
    }

    bool operator<=(const ModelWindowOp& other) const { return !(*this > other); }
    bool operator>=(const ModelWindowOp& other) const { return !(*this < other); }

    ModelWindowOp operator+(const ModelWindowOp& other) const {
        int newX = min(x, other.x);
        int newY = min(y, other.y);
        int newWidth = max(x + width, other.x + other.width) - newX;
        int newHeight = max(y + height, other.y + other.height) - newY;

        if (newX < 0) newX = 0;
        if (newY < 0) newY = 0;
        if (newX + newWidth > SCREEN_WIDTH) newWidth = SCREEN_WIDTH - newX;
        if (newY + newHeight > SCREEN_HEIGHT) newHeight = SCREEN_HEIGHT - newY;

        return ModelWindowOp("Объединенное окно", newX, newY, newWidth, newHeight, "серый", true, true);
    }

    ModelWindowOp operator-(const ModelWindowOp& other) const {
        int left = max(x, other.x);
        int top = max(y, other.y);
        int right = min(x + width, other.x + other.width);
        int bottom = min(y + height, other.y + other.height);

        int newWidth = (right > left) ? (right - left) : 0;
        int newHeight = (bottom > top) ? (bottom - top) : 0;

        if (newWidth <= 0 || newHeight <= 0) {
            return ModelWindowOp("Пустое пересечение", 0, 0, 0, 0, "черный", false, false);
        }

        return ModelWindowOp("Пересечение окон", left, top, newWidth, newHeight, "красный", true, true);
    }

    ModelWindowOp operator*(double factor) const {
        if (factor <= 0) return *this;
        int newWidth = static_cast<int>(width * factor);
        int newHeight = static_cast<int>(height * factor);
        if (newWidth > SCREEN_WIDTH) newWidth = SCREEN_WIDTH;
        if (newHeight > SCREEN_HEIGHT) newHeight = SCREEN_HEIGHT;
        if (x + newWidth > SCREEN_WIDTH) newWidth = SCREEN_WIDTH - x;
        if (y + newHeight > SCREEN_HEIGHT) newHeight = SCREEN_HEIGHT - y;

        ModelWindowOp result = *this;
        result.setWidth(newWidth);
        result.setHeight(newHeight);
        return result;
    }

    ModelWindowOp operator/(double factor) const {
        if (factor <= 0) return *this;
        return *this * (1.0 / factor);
    }

    ModelWindowOp& operator+=(const ModelWindowOp& other) { *this = *this + other; return *this; }
    ModelWindowOp& operator-=(const ModelWindowOp& other) { *this = *this - other; return *this; }
    ModelWindowOp& operator*=(double factor) { *this = *this * factor; return *this; }
    ModelWindowOp& operator/=(double factor) { *this = *this / factor; return *this; }

    ModelWindowOp& operator++() { *this *= 1.1; return *this; }
    ModelWindowOp operator++(int) { ModelWindowOp temp = *this; ++(*this); return temp; }
    ModelWindowOp& operator--() { *this /= 1.1; return *this; }
    ModelWindowOp operator--(int) { ModelWindowOp temp = *this; --(*this); return temp; }

    friend ostream& operator<<(ostream& os, const ModelWindowOp& win);
    friend istream& operator>>(istream& is, ModelWindowOp& win);
    friend ModelWindowOp operator*(double factor, const ModelWindowOp& win);
};

ostream& operator<<(ostream& os, const ModelWindowOp& win) {
    os << "Окно '" << win.title << "' [" << win.x << "," << win.y << "] "
        << win.width << "x" << win.height << " цвет: " << win.color
        << " " << (win.visible ? "видимое" : "скрытое")
        << " " << (win.bordered ? "с рамкой" : "без рамки")
        << " (площадь: " << win.width * win.height << ")";
    return os;
}

istream& operator>>(istream& is, ModelWindowOp& win) {
    cout << "Введите заголовок окна: ";
    is.ignore();
    is.getline(win.title, 100);

    cout << "Введите координату X: ";
    is >> win.x;
    cout << "Введите координату Y: ";
    is >> win.y;

    cout << "Введите ширину окна: ";
    is >> win.width;
    cout << "Введите высоту окна: ";
    is >> win.height;

    cout << "Введите цвет окна: ";
    is.ignore();
    is.getline(win.color, 50);

    char choice;
    cout << "Окно видимое? (y/n): ";
    is >> choice;
    win.visible = (choice == 'y' || choice == 'Y');

    cout << "Окно с рамкой? (y/n): ";
    is >> choice;
    win.bordered = (choice == 'y' || choice == 'Y');

    return is;
}

ModelWindowOp operator*(double factor, const ModelWindowOp& win) {
    return win * factor;
}


int main() {
    setlocale(LC_ALL, "RU");

    cout << "Способ 1" << endl;
 
    ModelWindow win1;
    cout << "1. Конструктор по умолчанию:" << endl;
    win1.display();

    ModelWindow win2("Главное окно", 200, 150, 800, 600, "синий");
    cout << "\n2. Конструктор инициализации:" << endl;
    win2.display();

    ModelWindow win3(win2);
    win3.setTitle("Копия главного окна");
    win3.setColor("зеленый");
    cout << "\n3. Конструктор копирования:" << endl;
    win3.display();
    cout << endl;

    // Демонстрация работы с массивом объектов
    cout << "4. Массив объектов ModelWindow:" << endl;
    ModelWindow windows[3];

    windows[0] = ModelWindow("Окно 1", 50, 50, 300, 200, "красный");
    windows[1] = ModelWindow("Окно 2", 400, 300, 500, 400, "желтый");

    cout << "\nВведите данные для третьего окна:" << endl;
    windows[2].read();  // Ввод с клавиатуры

    for (int i = 0; i < 3; i++) {
        cout << "\nОкно " << i + 1 << ":" << endl;
        windows[i].display();
    }

    // Демонстрация операций с окнами
    cout << "\n5. Операции с окнами:" << endl;

    // Перемещение
    cout << "Перемещаем окно 1 на 100 пикселей вправо: ";
    if (windows[0].moveHorizontally(100)) {
        cout << "Успешно!" << endl;
    }
    else {
        cout << "Не удалось!" << endl;
    }

    // Изменение размеров
    cout << "Увеличиваем высоту окна 2 на 100 пикселей: ";
    if (windows[1].resizeHeight(windows[1].getHeight() + 100)) {
        cout << "Успешно!" << endl;
    }
    else {
        cout << "Не удалось!" << endl;
    }

    // Изменение свойств
    cout << "Изменяем свойства окна 3:" << endl;
    windows[2].setColor("фиолетовый");
    windows[2].setVisible(false);
    windows[2].setBordered(false);
    windows[2].display();

    // Демонстрация проверки границ
    cout << "\n6. Проверка границ экрана:" << endl;
    ModelWindow testWin("Тестовое окно", 1800, 1000, 200, 200, "оранжевый");
    cout << "Создано окно вблизи границы: ";
    testWin.display();

    cout << "\nПопытка переместить за границу экрана: ";
    if (testWin.moveHorizontally(200)) {
        cout << "Успешно!" << endl;
    }
    else {
        cout << "Не удалось (выход за границы)!" << endl;
    }

    cout << "Способ 2 " << endl;

    ModelWindowOp opWin1;
    ModelWindowOp opWin2("Окно A", 100, 100, 400, 300, "синий");
    ModelWindowOp opWin3("Окно B", 300, 200, 600, 400, "зеленый");
    ModelWindowOp opWin4(opWin2);  // Конструктор копирования

    cout << "opWin1: " << opWin1 << endl;
    cout << "opWin2: " << opWin2 << endl;
    cout << "opWin3: " << opWin3 << endl;
    cout << "opWin4: " << opWin4 << endl;

    // Вывод объектов с помощью перегруженного оператора <<
    cout << "1. Созданные объекты:" << endl;
    cout << "opWin1: " << opWin1 << endl;
    cout << "opWin2: " << opWin2 << endl;
    cout << "opWin3: " << opWin3 << endl;
    cout << "opWin4: " << opWin4 << endl;

    // Операция присваивания
    cout << "\n2. Операция присваивания (opWin1 = opWin2):" << endl;
    opWin1 = opWin2;
    opWin1.setTitle("Копия окна A");
    cout << "opWin1 после присваивания: " << opWin1 << endl;

    // Операции сравнения
    cout << "\n3. Операции сравнения:" << endl;
    cout << "opWin1 == opWin2: " << (opWin1 == opWin2 ? "true" : "false") << endl;
    cout << "opWin1 != opWin2: " << (opWin1 != opWin2 ? "true" : "false") << endl;
    cout << "opWin2 < opWin3: " << (opWin2 < opWin3 ? "true" : "false") << " (сравнение по площади)" << endl;
    cout << "opWin2 > opWin3: " << (opWin2 > opWin3 ? "true" : "false") << endl;
    cout << "opWin2 <= opWin3: " << (opWin2 <= opWin3 ? "true" : "false") << endl;
    cout << "opWin2 >= opWin3: " << (opWin2 >= opWin3 ? "true" : "false") << endl;

    // Арифметические операции
    cout << "\n4. Арифметические операции:" << endl;

    // Сложение (объединение окон)
    ModelWindowOp sumWin = opWin2 + opWin3;
    cout << "opWin2 + opWin3 (объединение): " << sumWin << endl;

    // Вычитание (пересечение окон)
    ModelWindowOp diffWin = opWin2 - opWin3;
    cout << "opWin2 - opWin3 (пересечение): " << diffWin << endl;

    // Умножение на коэффициент
    ModelWindowOp scaledWin = opWin2 * 1.5;
    cout << "opWin2 * 1.5 (масштабирование): " << scaledWin << endl;

    // Деление на коэффициент
    ModelWindowOp shrunkWin = opWin3 / 2.0;
    cout << "opWin3 / 2.0 (уменьшение): " << shrunkWin << endl;

    // Коммутативное умножение
    ModelWindowOp commWin = 0.5 * opWin2;
    cout << "0.5 * opWin2 (коммутативное умножение): " << commWin << endl;

    // Составные операции присваивания
    cout << "\n5. Составные операции присваивания:" << endl;
    ModelWindowOp compWin = opWin2;

    compWin += opWin3;
    cout << "После compWin += opWin3: " << compWin << endl;

    compWin -= opWin3;
    cout << "После compWin -= opWin3: " << compWin << endl;

    compWin *= 1.2;
    cout << "После compWin *= 1.2: " << compWin << endl;

    compWin /= 1.5;
    cout << "После compWin /= 1.5: " << compWin << endl;

    // Операции инкремента и декремента
    cout << "\n6. Инкремент и декремент:" << endl;
    ModelWindowOp incWin("Инкрементное окно", 200, 200, 200, 200, "красный");

    cout << "Исходное окно: " << incWin << endl;

    // Префиксный инкремент
    ++incWin;
    cout << "После ++incWin: " << incWin << endl;

    // Постфиксный инкремент
    ModelWindowOp temp = incWin++;
    cout << "Результат incWin++: " << temp << endl;
    cout << "incWin после постфиксного инкремента: " << incWin << endl;

    // Префиксный декремент
    --incWin;
    cout << "После --incWin: " << incWin << endl;

    // Постфиксный декремент
    temp = incWin--;
    cout << "Результат incWin--: " << temp << endl;
    cout << "incWin после постфиксного декремента: " << incWin << endl;

    // Работа с массивом объектов
    cout << "\n7. Массив объектов с перегруженными операциями:" << endl;
    ModelWindowOp opArray[3] = {
        ModelWindowOp("Массивное окно 1", 50, 50, 300, 250, "желтый"),
        ModelWindowOp("Массивное окно 2", 200, 150, 400, 350, "голубой"),
        ModelWindowOp("Массивное окно 3", 350, 250, 500, 450, "розовый")
    };

    // Вычисление среднего окна (объединение всех и деление на 3)
    ModelWindowOp avgWindow = opArray[0];
    for (int i = 1; i < 3; i++) {
        avgWindow = avgWindow + opArray[i];
    }
    avgWindow = avgWindow / 3.0;
    avgWindow.setTitle("Среднее окно");
    avgWindow.setColor("серый");

    cout << "Среднее окно массива: " << avgWindow << endl;

    // Ввод объекта с помощью перегруженного оператора >>
    cout << "\n8. Ввод объекта с помощью operator>>:" << endl;
    ModelWindowOp inputWin;
    cin >> inputWin;
    cout << "\nВведенное окно: " << inputWin << endl;

    // Сортировка массива по площади (используя перегруженные операции сравнения)
    cout << "\n9. Сортировка массива окон по площади:" << endl;
    cout << "До сортировки:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  " << opArray[i] << endl;
    }

    // Простая сортировка пузырьком
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2 - i; j++) {
            if (opArray[j] > opArray[j + 1]) {
                ModelWindowOp tempWin = opArray[j];
                opArray[j] = opArray[j + 1];
                opArray[j + 1] = tempWin;
            }
        }
    }

    cout << "\nПосле сортировки (по возрастанию площади):" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  " << opArray[i] << endl;
    }
    cout << "КОНЕЦ" << endl;
    return 0;
}