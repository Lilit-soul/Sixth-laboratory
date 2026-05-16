package main

import (
	"fmt"
	"math"
)

// Печать матрицы и вектора
func printMatrix(A [][]float64, B []float64) {
	fmt.Println("\n=== ИСХОДНАЯ СИСТЕМА ===")
	for i := 0; i < len(A); i++ {
		for j := 0; j < len(A[i]); j++ {
			if A[i][j] >= 0 {
				fmt.Printf("+%.2f·x%d ", A[i][j], j+1)
			} else {
				fmt.Printf("%.2f·x%d ", A[i][j], j+1)
			}
		}
		fmt.Printf("= %.2f\n", B[i])
	}
}

// Прямой метод: LU-разложение
func luDecomposition(A [][]float64, B []float64) ([]float64, [][]float64, [][]float64) {
	n := len(A)
	L := make([][]float64, n)
	U := make([][]float64, n)
	for i := 0; i < n; i++ {
		L[i] = make([]float64, n)
		U[i] = make([]float64, n)
		L[i][i] = 1.0
	}

	// LU-разложение
	for i := 0; i < n; i++ {
		for j := i; j < n; j++ {
			sum := 0.0
			for k := 0; k < i; k++ {
				sum += L[i][k] * U[k][j]
			}
			U[i][j] = A[i][j] - sum
		}
		for j := i + 1; j < n; j++ {
			sum := 0.0
			for k := 0; k < i; k++ {
				sum += L[j][k] * U[k][i]
			}
			L[j][i] = (A[j][i] - sum) / U[i][i]
		}
	}

	// Прямой ход: L·Y = B
	Y := make([]float64, n)
	for i := 0; i < n; i++ {
		sum := 0.0
		for j := 0; j < i; j++ {
			sum += L[i][j] * Y[j]
		}
		Y[i] = B[i] - sum
	}

	// Обратный ход: U·X = Y
	X := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		sum := 0.0
		for j := i + 1; j < n; j++ {
			sum += U[i][j] * X[j]
		}
		X[i] = (Y[i] - sum) / U[i][i]
	}

	return X, L, U
}

// Канонический вид x = C·x + f
func toCanonicalForm(A [][]float64, B []float64) ([][]float64, []float64) {
	n := len(A)
	C := make([][]float64, n)
	f := make([]float64, n)
	for i := 0; i < n; i++ {
		C[i] = make([]float64, n)
		f[i] = B[i] / A[i][i]
		for j := 0; j < n; j++ {
			if i != j {
				C[i][j] = -A[i][j] / A[i][i]
			}
		}
	}
	return C, f
}

// Норма матрицы (максимум суммы строк)
func matrixNorm(C [][]float64) float64 {
	n := len(C)
	maxNorm := 0.0
	for i := 0; i < n; i++ {
		rowSum := 0.0
		for j := 0; j < n; j++ {
			rowSum += math.Abs(C[i][j])
		}
		if rowSum > maxNorm {
			maxNorm = rowSum
		}
	}
	return maxNorm
}

// Метод Зейделя
func seidelMethod(A [][]float64, B []float64, eps float64) ([]float64, int) {
	n := len(A)
	X := make([]float64, n)
	iter := 0
	var maxDiff float64

	for {
		X_prev := make([]float64, n)
		copy(X_prev, X)
		maxDiff = 0

		for i := 0; i < n; i++ {
			sum := 0.0
			for j := 0; j < n; j++ {
				if j != i {
					sum += A[i][j] * X[j]
				}
			}
			X[i] = (B[i] - sum) / A[i][i]
		}

		epsNorm := 0.0
		for i := 0; i < n; i++ {
			diff := math.Abs(X[i] - X_prev[i])
			epsNorm += diff
			if diff > maxDiff {
				maxDiff = diff
			}
		}

		iter++

		// Вывод таблицы
		if iter == 1 {
			fmt.Println("\n=== МЕТОД ЗЕЙДЕЛЯ ===")
			fmt.Println("Точность ε =", eps)
			fmt.Println("+------+-------------------------------------------+----------------+")
			fmt.Println("|  n   |   x1        x2        x3        x4        |      εn        |")
			fmt.Println("+------+-------------------------------------------+----------------+")
		}
		fmt.Printf("| %4d | %10.6f %10.6f %10.6f %10.6f | %14.2e |\n",
			iter, X[0], X[1], X[2], X[3], epsNorm)

		if maxDiff < eps || iter >= 1000 {
			fmt.Println("+------+-------------------------------------------+----------------+")
			break
		}
	}

	return X, iter
}

// Проверка решения подстановкой
func checkSolution(A [][]float64, B []float64, X []float64, eps float64) {
	n := len(A)
	maxResidual := 0.0

	fmt.Println("\n=== ПРОВЕРКА РЕШЕНИЯ ПОДСТАНОВКОЙ ===")

	for i := 0; i < n; i++ {
		sum := 0.0
		for j := 0; j < n; j++ {
			sum += A[i][j] * X[j]
		}
		residual := math.Abs(sum - B[i])
		if residual > maxResidual {
			maxResidual = residual
		}
		fmt.Printf("Уравнение %d: %.6f = %.2f | погрешность = %.2e", i+1, sum, B[i], residual)
		if residual < eps {
			fmt.Println(" ✓")
		} else {
			fmt.Println(" ✗")
		}
	}

	fmt.Printf("\n=== ТОЧНОСТЬ ===\n")
	fmt.Printf("Максимальная погрешность = %.2e\n", maxResidual)
	fmt.Printf("Требуемая точность ε = %.0e\n", eps)

	if maxResidual < eps {
		fmt.Println("РЕЗУЛЬТАТ: ДОСТИГНУТА (погрешность < ε)")
	} else {
		fmt.Println("РЕЗУЛЬТАТ: НЕ ДОСТИГНУТА (погрешность >= ε)")
	}
}

func main() {
	// Данные варианта 11
	A := [][]float64{
		{0.89, -0.04, 0.21, -1.16},
		{0.25, -1.23, 0.08, -0.09},
		{-0.21, 0.08, 0.8, -0.13},
		{0.15, -1.31, 0.06, -1.21},
	}
	B := []float64{-1.24, -1.21, 2.56, 0.89}

	// Вывод исходной системы
	printMatrix(A, B)

	// Прямой метод (LU-разложение)
	X_direct, L, U := luDecomposition(A, B)

	fmt.Println("\n=== МАТРИЦА L ===")
	for i := 0; i < len(L); i++ {
		for j := 0; j < len(L[i]); j++ {
			fmt.Printf("%12.6f ", L[i][j])
		}
		fmt.Println()
	}

	fmt.Println("\n=== МАТРИЦА U ===")
	for i := 0; i < len(U); i++ {
		for j := 0; j < len(U[i]); j++ {
			fmt.Printf("%12.6f ", U[i][j])
		}
		fmt.Println()
	}

	fmt.Printf("\n=== КОРНИ (прямой метод) ===\n")
	fmt.Printf("x1 = %.6f\nx2 = %.6f\nx3 = %.6f\nx4 = %.6f\n", X_direct[0], X_direct[1], X_direct[2], X_direct[3])

	// Канонический вид
	C, f := toCanonicalForm(A, B)

	fmt.Println("\n=== КАНОНИЧЕСКИЙ ВИД x = C·x + f ===")
	for i := 0; i < len(C); i++ {
		fmt.Printf("x%d = ", i+1)
		for j := 0; j < len(C[i]); j++ {
			if j != i {
				if C[i][j] >= 0 {
					fmt.Printf("+%.4f·x%d ", C[i][j], j+1)
				} else {
					fmt.Printf("%.4f·x%d ", C[i][j], j+1)
				}
			}
		}
		fmt.Printf("+ %.4f\n", f[i])
	}

	// Норма матрицы C
	normC := matrixNorm(C)
	fmt.Printf("\n=== НОРМА МАТРИЦЫ C ===\n")
	fmt.Printf("||C|| = %.6f\n", normC)
	if normC < 1 {
		fmt.Println("УСЛОВИЕ СХОДИМОСТИ ВЫПОЛНЯЕТСЯ (||C|| < 1)")
	} else {
		fmt.Println("ВНИМАНИЕ: Условие сходимости НЕ ГАРАНТИРОВАНО (||C|| >= 1)")
	}

	// Итерационный метод (Зейдель)
	eps := 1e-3
	X_iter, iter := seidelMethod(A, B, eps)

	fmt.Printf("\n=== КОРНИ СИСТЕМЫ (ε = %.0e) ===\n", eps)
	fmt.Printf("x1 = %.8f\nx2 = %.8f\nx3 = %.8f\nx4 = %.8f\n", X_iter[0], X_iter[1], X_iter[2], X_iter[3])
	fmt.Printf("Сходимость достигнута за %d итераций\n", iter)

	// Проверка решения
	checkSolution(A, B, X_iter, eps)

	// Сравнение методов
	fmt.Println("\n=== СРАВНЕНИЕ МЕТОДОВ ===")
	fmt.Printf("Прямой метод (LU):     %.6f %.6f %.6f %.6f\n", X_direct[0], X_direct[1], X_direct[2], X_direct[3])
	fmt.Printf("Итерационный (Зейдель): %.6f %.6f %.6f %.6f\n", X_iter[0], X_iter[1], X_iter[2], X_iter[3])
}
