#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <limits>

using namespace std;

// Матрица размерности MxN
void task1() {
    
    int M, N;  
    
    cout << "Введите M (>7) и N (>5): ";
    cin >> M >> N;
    
    if (M < 8) {
        M = 8;
        cout << "M установлено в 8 (минимальное значение)" << endl;
    }
    if (N < 6) {
        N = 6;
        cout << "N установлено в 6 (минимальное значение)" << endl;
    }
    
    
    vector<vector<int>> matrix(M, vector<int>(N));
    
    // Заполнение случайными числами [-30, 50]
    cout << "\nИсходная матрица " << M << "x" << N << ":\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 81 - 30; // -30..50
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
    
    int sum = 0;
    int count = 0;
    vector<int> elements;
    
    // Чётные строки (индексация с 0 -> строки 1,3,5,...)
    // Нечётные столбцы (индексация с 0 -> столбцы 0,2,4,...)
    for (int i = 1; i < M; i += 2) {      // чётные строки
        for (int j = 0; j < N; j += 2) {  // нечётные столбцы
            if (matrix[i][j] >= 0) {
                sum += matrix[i][j];
                count++;
                elements.push_back(matrix[i][j]);
            }
        }
    }
    
    cout << "\nСумма неотрицательных элементов (чётные строки, нечётные столбцы): " << sum << endl;
    cout << "Количество таких элементов: " << count << endl;
    cout << "Массив этих элементов: ";
    for (int val : elements) cout << val << " ";
    cout << endl;
}

// Квадратная матрица
void task2() {
    
    int N;
    cout << "Введите размер квадратной матрицы NxN (>5): ";
    cin >> N;
    if (N < 6) {
        N = 6;
        cout << "N установлено в 6 (минимальное значение)" << endl;
    }
    
    // Выделение динамической матрицы
    int** matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new int[N];
    }
    
    // Заполнение случайными числами [100, 200]
    cout << "\nИсходная матрица " << N << "x" << N << ":\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 101 + 100; // 100..200
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
    
    // Вычисление сумм строк
    int* rowSums = new int[N];
    int maxSum = 0;
    int maxRow = 0;
    
    cout << "\nНомер строки | Сумма \n";
    cout << "------------------------\n";
    for (int i = 0; i < N; i++) {
        int sum = 0;
        for (int j = 0; j < N; j++) {
            sum += matrix[i][j];
        }
        rowSums[i] = sum;
        cout << "     " << i << "      |    " << sum << endl;
        if (sum > maxSum) {
            maxSum = sum;
            maxRow = i;
        }
    }
    
    // Создание динамического массива из строки с максимальной суммой
    int* maxRowArray = new int[N];
    for (int j = 0; j < N; j++) {
        maxRowArray[j] = matrix[maxRow][j];
    }
    
    cout << "\nСтрока с максимальной суммой: " << maxRow << endl;
    cout << "Сумма элементов: " << maxSum << endl;
    cout << "Элементы строки: ";
    for (int j = 0; j < N; j++) {
        cout << maxRowArray[j] << " ";
    }
    cout << endl;
    
    // Освобождение памяти
    delete[] rowSums;
    delete[] maxRowArray;
    for (int i = 0; i < N; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// ======================== Клеточный автомат ========================
const int WIDTH = 60;
const int HEIGHT = 30;

void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// ВЕРСИЯ 1: для вывода с заголовком (3 аргумента)
void printGridWithTitle(bool grid[HEIGHT][WIDTH], int gen, const char* title) {
    clearConsole();
    cout << title << endl;
    cout << "Поколение: " << gen << endl;
    cout << "Живые клетки — 1, мёртвые — .\n\n";

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << (grid[i][j] ? '1' : '.');
        }
        cout << endl;
    }
}

void printGrid(bool grid[HEIGHT][WIDTH], int gen, const char* title) {
    clearConsole();
    cout << title << endl;
    cout << "Поколение: " << gen << endl;
    cout << "Живые клетки — 1, мёртвые — .\n\n";

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << (grid[i][j] ? '1' : '.');
        }
        cout << endl;
    }
}

int countNeighbors(bool grid[HEIGHT][WIDTH], int x, int y) {
    int cnt = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH && grid[nx][ny]) {
                cnt++;
            }
        }
    }
    return cnt;
}

void nextGeneration(bool grid[HEIGHT][WIDTH]) {
    bool newGrid[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = countNeighbors(grid, i, j);
            if (grid[i][j]) {
                newGrid[i][j] = (neighbors == 2 || neighbors == 3);
            } else {
                newGrid[i][j] = (neighbors == 3);
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
}

// ========== Block‑laying switch engine (паровоз) ==========
void initPuffer(bool grid[HEIGHT][WIDTH]) {
    // Очистка поля
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            grid[i][j] = false;

    // Начальная позиция 
    int x = 15, y = 20;

    // Основной двигатель (Switch Engine)
    grid[x][y+2] = grid[x][y+3] = grid[x][y+4] = true;
    grid[x+1][y+1] = grid[x+1][y+4] = true;
    grid[x+2][y] = grid[x+2][y+4] = grid[x+2][y+5] = true;
    grid[x+3][y+1] = grid[x+3][y+3] = true;
    grid[x+4][y+2] = grid[x+4][y+3] = grid[x+4][y+4] = true;
    grid[x+5][y+3] = true;

    // Стабилизаторы (превращают двигатель в паровоз)
    grid[x-1][y+4] = grid[x-1][y+5] = true;
    grid[x][y+5] = true;
    grid[x+6][y+1] = grid[x+6][y+2] = true;
    grid[x+7][y+1] = grid[x+7][y+2] = true;
    grid[x+8][y+1] = true;
    grid[x+1][y+6] = grid[x+2][y+6] = grid[x+3][y+6] = true;
    grid[x+3][y+7] = true;
    grid[x+5][y] = grid[x+5][y+1] = true;
    grid[x+6][y] = true;
}

// Случайная колония (для сравнения)
void initRandom(bool grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            grid[i][j] = (rand() % 100 < 30);
}

// ========== Демонстрация 1: Паровоз ==========
void demoPuffer() {
    bool grid[HEIGHT][WIDTH];
    initPuffer(grid);

    cout << "Демонстрация 1: Block‑laying switch engine (паровоз)\n";
    cout << "Двигается по диагонали, оставляет СЛЕД — блоки 2×2\n";
    cin.get();

    for (int gen = 0; gen < 25; gen++) {
        printGrid(grid, gen, "=== ПАРОВОЗ (движется + след из блоков 2×2) ===");
        nextGeneration(grid);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

// ========== Демонстрация 2: Случайная колония ==========
void demoRandom() {
    bool grid[HEIGHT][WIDTH];
    initRandom(grid);

    cout << "\n\nДемонстрация 2: Случайная колония (для сравнения)\n";
    cout << "Нет направленного движения, только хаос\n";
    cout << "Нажмите Enter...";
    cin.get();

    for (int gen = 0; gen < 100; gen++) {
        printGrid(grid, gen, "=== СЛУЧАЙНАЯ КОЛОНИЯ (хаос) ===");
        nextGeneration(grid);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void task3() {
    cout << "Нажмите Enter...";
    cin.get();
    srand(time(0));
    demoPuffer();
    demoRandom();
    cout << "\nДемонстрация завершена.\n";
}



int main() {
    srand(time(0));
    
    task1();
    task2();
    task3();
    
    return 0;
}
