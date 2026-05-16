#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ 

void printMatrix(const vector<vector<double>>& A, const vector<double>& B) {
    cout << "\n    ИСХОДНАЯ СИСТЕМА \n";
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            cout << showpos << A[i][j] << "·x" << j+1 << " ";
        }
        cout << noshowpos << "= " << B[i] << endl;
    }
}

// ПРЯМОЙ МЕТОД (LU-РАЗЛОЖЕНИЕ) 

bool luDecomposition(const vector<vector<double>>& A, 
                     const vector<double>& B,  // ← добавить B
                     vector<vector<double>>& L, 
                     vector<vector<double>>& U,
                     vector<double>& X) {
    int n = A.size();
    
    L.assign(n, vector<double>(n, 0.0));
    U.assign(n, vector<double>(n, 0.0));
    
    for (int i = 0; i < n; i++) L[i][i] = 1.0;
    
    cout << "  \nПРЯМОЙ МЕТОД: LU-РАЗЛОЖЕНИЕ\n";
    
    // LU-разложение
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++) sum += L[i][k] * U[k][j];
            U[i][j] = A[i][j] - sum;
        }
        
        for (int j = i + 1; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++) sum += L[j][k] * U[k][i];
            L[j][i] = (A[j][i] - sum) / U[i][i];
        }
    }
    
    // Вывод матриц L и U
    cout << "\n    МАТРИЦА L (нижняя треугольная) \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(12) << setprecision(6) << L[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\n    МАТРИЦА U (верхняя треугольная) \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(12) << setprecision(6) << U[i][j] << " ";
        }
        cout << endl;
    }
    
    // Прямой ход: L·Y = B
    vector<double> Y(n, 0.0);
    cout << "\n    ПРЯМОЙ ХОД (L·Y = B) \n";
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) sum += L[i][j] * Y[j];
        Y[i] = B[i] - sum;  // L[i][i] = 1, поэтому делить не нужно
        cout << "y" << i+1 << " = " << Y[i] << endl;
    }
    
    // Обратный ход: U·X = Y
    cout << "\n    ОБРАТНЫЙ ХОД (U·X = Y) \n";
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) sum += U[i][j] * X[j];
        X[i] = (Y[i] - sum) / U[i][i];
        cout << "x" << i+1 << " = " << X[i] << endl;
    }
    
    return true;
}

// ПРИВЕДЕНИЕ К КАНОНИЧЕСКОМУ ВИДУ 

void toCanonicalForm(const vector<vector<double>>& A, const vector<double>& B,
                     vector<vector<double>>& C, vector<double>& f) {
    int n = A.size();
    C.assign(n, vector<double>(n, 0.0));
    f.assign(n, 0.0);
    
    cout << "  \nКАНОНИЧЕСКИЙ ВИД x = C·x + f\n";
    
    for (int i = 0; i < n; i++) {
        cout << "x" << i+1 << " = ";
        for (int j = 0; j < n; j++) {
            if (i != j) {
                C[i][j] = -A[i][j] / A[i][i];
                cout << showpos << C[i][j] << "·x" << j+1 << " ";
            }
        }
        f[i] = B[i] / A[i][i];
        cout << noshowpos << "+ " << f[i] << endl;
    }
    
    cout << "\n    МАТРИЦА C \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(12) << setprecision(6) << C[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\n    ВЕКТОР f \n";
    for (int i = 0; i < n; i++) {
        cout << "f[" << i << "] = " << f[i] << endl;
    }
}

// ПРОВЕРКА УСЛОВИЯ СХОДИМОСТИ 

bool checkConvergence(const vector<vector<double>>& C) {
    int n = C.size();
    double maxRowSum = 0;
    
    cout << "  \nУСЛОВИЕ СХОДИМОСТИ (НОРМА МАТРИЦЫ C)\n";
    cout << "Достаточное условие: max(сумма |c_ij|) < 1\n\n";
    
    for (int i = 0; i < n; i++) {
        double rowSum = 0;
        for (int j = 0; j < n; j++) {
            rowSum += fabs(C[i][j]);
        }
        maxRowSum = max(maxRowSum, rowSum);
        cout << "Строка " << i+1 << ": сумма |c_" << i+1 << "j| = " << rowSum << endl;
    }
    
    cout << "\n||C|| = " << maxRowSum << endl;
    
    if (maxRowSum < 1) {
        cout << "\nУСЛОВИЕ СХОДИМОСТИ ВЫПОЛНЯЕТСЯ (||C|| < 1)\n";
        return true;
    } else {
        cout << "\nВНИМАНИЕ: Условие сходимости НЕ ГАРАНТИРОВАНО (||C|| >= 1)\n" << endl;
        return false;
    }
}

// ИТЕРАЦИОННЫЙ МЕТОД (ЗЕЙДЕЛЯ) 

void seidelMethod(const vector<vector<double>>& A, const vector<double>& B, 
                  double eps, vector<double>& X) {
    int n = A.size();
    vector<double> X_prev(n, 0.0);
    int iter = 0;
    double maxDiff;
    
    cout << "  ИТЕРАЦИОННЫЙ МЕТОД (ЗЕЙДЕЛЯ)\n";
    cout << "  Точность ε = " << eps << "\n";
    
    // Шапка таблицы (по образцу из методички)
    cout << "-----------------------------------------------------------------------\n";
    cout << "|   n   |    X1         X2        X3        X4        |       εn       |\n";
    cout << "-----------------------------------------------------------------------\n";
    
    do {
        X_prev = X;
        maxDiff = 0;
        
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < n; j++) {
                if (j != i) sum += A[i][j] * X[j];
            }
            X[i] = (B[i] - sum) / A[i][i];
        }
        
        // Вычисляем εn = ||X^(k) - X^(k-1)||
        double epsNorm = 0;
        for (int i = 0; i < n; i++) {
            double diff = fabs(X[i] - X_prev[i]);
            epsNorm += diff;
            if (diff > maxDiff) maxDiff = diff;
        }
        
        // Вывод строки таблицы
        cout << "| " << setw(5) << ++iter << " | ";
        for (int i = 0; i < n; i++) {
            cout << fixed << setw(10) << setprecision(6) << X[i] << " ";
        }
        cout << "| " << scientific << setw(14) << setprecision(2) << epsNorm << " |\n";
        
    } while (maxDiff >= eps && iter < 1000);
    
    cout << "------------------------------------------------------------------------\n";
    
    if (maxDiff < eps) {
        cout << "\nСХОДИМОСТЬ ДОСТИГНУТА за " << iter << " итераций \n";
    } else {
        cout << "\nПРЕВЫШЕНО МАКСИМАЛЬНОЕ ЧИСЛО ИТЕРАЦИЙ \n";
    }
}

// ПРОВЕРКА РЕШЕНИЯ ПОДСТАНОВКОЙ 

void checkSolution(const vector<vector<double>>& A, const vector<double>& B, const vector<double>& X, double eps) {
    int n = A.size();
    double maxResidual = 0;
    
    cout << "  \nПРОВЕРКА РЕШЕНИЯ ПОДСТАНОВКОЙ\n";
    
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += A[i][j] * X[j];
        }
        double residual = fabs(sum - B[i]);
        if (residual > maxResidual) maxResidual = residual;
        
        cout << "Уравнение " << i+1 << ": " << sum << " = " << B[i];
        cout << "   |   погрешность = " << residual;
        if (residual < eps) cout << " \n";
        else cout << " Погрешность превышает ε\n";
    }
    
    cout << "\n    ТОЧНОСТЬ \n";
    cout << "Максимальная погрешность = " << maxResidual << endl;
    cout << "Требуемая точность ε = " << eps << endl;
    
    if (maxResidual < eps) {
        cout << "РЕЗУЛЬТАТ: ДОСТИГНУТА (погрешность < ε)\n";
    } else {
        cout << "РЕЗУЛЬТАТ: НЕ ДОСТИГНУТА (погрешность >= ε)\n";
    }
}

//  ВЫВОД КОРНЕЙ 
void printRoots(const vector<double>& X) {
    cout << "  \nКОРНИ СИСТЕМЫ УРАВНЕНИЙ \n";
    
    for (size_t i = 0; i < X.size(); i++) {
        // Округляем до 6 знаков
        double rounded = round(X[i] * 1e6) / 1e6;
        cout << "x" << i+1 << " = " << fixed << setprecision(6) << X[i];
        if (fabs(X[i] - rounded) > 1e-10) {
            cout << " ≈ " << rounded;
        }
        cout << endl;
    }
}


int main() {
    
    // Данные варианта 11 (M=0.89, N=0.08, P=-1.21)
    vector<vector<double>> A = {
        {0.89, -0.04, 0.21, -1.16},
        {0.25, -1.23, 0.08, -0.09},
        {-0.21, 0.08, 0.8, -0.13},
        {0.15, -1.31, 0.06, -1.21}
    };
    vector<double> B = {-1.24, -1.21, 2.56, 0.89};
    
    // Вывод исходной системы
    printMatrix(A, B);
    
    //  ПРЯМОЙ МЕТОД (LU-разложение) 
    vector<vector<double>> L, U;
    vector<double> X_direct(4, 0);
    luDecomposition(A, B, L, U, X_direct);
    
    // КАНОНИЧЕСКИЙ ВИД
    vector<vector<double>> C;
    vector<double> f;
    toCanonicalForm(A, B, C, f);
    
    // ПРОВЕРКА УСЛОВИЯ СХОДИМОСТИ 
    checkConvergence(C);
    
    // ИТЕРАЦИОННЫЙ МЕТОД 
    double eps = 1e-3;
    vector<double> X_iter(4, 0);  // начальное приближение (0,0,0,0)
    seidelMethod(A, B, eps, X_iter);
    
    // КОРНИ 
    printRoots(X_iter);
    
    // ПРОВЕРКА ПОДСТАНОВКОЙ
    checkSolution(A, B, X_iter, eps);
    
    // Дополнительно: сравнение прямого и итерационного методов
    cout << "  \nСРАВНЕНИЕ МЕТОДОВ\n";
    cout << "Прямой метод (LU):     ";
    for (int i = 0; i < 4; i++) cout << fixed << setprecision(6) << X_direct[i] << " ";
    cout << endl;
    cout << "Итерационный (Зейдель): ";
    for (int i = 0; i < 4; i++) cout << X_iter[i] << " ";
    cout << endl;
    
    return 0;
}
