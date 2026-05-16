#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

// Функция рисования треугольника Серпинского
// x, y - координаты левого нижнего угла
// size - размер стороны в пикселях
// depth - глубина рекурсии (0 - только целый треугольник)
// grid - массив символов (2D), где ' ' - пусто, '#' - закрашено
void drawSierpinski(int x, int y, int size, int depth, vector<vector<char>>& grid) {
    if (depth == 0) {
        // Рисуем заполненный треугольник
        for (int i = 0; i < size; i++) {
            for (int j = 0; j <= i; j++) {
                grid[y - i][x + j] = '#';
            }
        }
        return;
    }
    
    int newSize = size / 2;
    
    // Три меньших треугольника (без центрального)
    // Нижний левый
    drawSierpinski(x, y, newSize, depth - 1, grid);
    // Нижний правый
    drawSierpinski(x + newSize, y, newSize, depth - 1, grid);
    // Верхний
    drawSierpinski(x + newSize / 2, y - newSize, newSize, depth - 1, grid);
    
    // Центральный треугольник не рисуется (остаётся пустым)
}

int main() {
    cout << "ФРАКТАЛ: ТРЕУГОЛЬНИК СЕРПИНСКОГО" << endl;
    cout << "Вариант 2" << endl;
    
    int depth;
    cout << "Введите глубину рекурсии (0 - 7, рекомендуемая 4-5 для консоли): ";
    cin >> depth;
    
    if (depth < 0) depth = 0;
    if (depth > 7) {
        cout << "Глубина 7 — максимальная для консоли. Установлено 7." << endl;
        depth = 7;
    }
    
    // Размер стороны треугольника в ячейках
    int size = pow(2, depth);
    int width = size * 2;
    int height = size;
    
    // Создаём сетку для рисования (все пробелы)
    vector<vector<char>> grid(height, vector<char>(width, ' '));
    
    // Рисуем треугольник Серпинского
    drawSierpinski(0, height - 1, size, depth, grid);
    
    // Выводим результат
    cout << "\nТреугольник Серпинского (глубина = " << depth << "):\n" << endl;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
    
    cout << "Примечание: '#' — закрашенная область, пробел — пустота." << endl;
    cout << "Глубина " << depth << " = " << pow(3, depth) << " маленьких треугольников" << endl;
    
    return 0;
}
