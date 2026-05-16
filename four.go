package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)


func clearInput(scanner *bufio.Scanner) {
	scanner.Scan()
}

func isValidSize(N, M int) bool {
	return N > 0 && M > 0 && N*M <= 16
}

func isValidChar(c byte) bool {
	return c == '0' || c == '1'
}

func isValidString(s string, expectedLen int) bool {
	if len(s) != expectedLen {
		return false
	}
	for i := 0; i < len(s); i++ {
		if !isValidChar(s[i]) {
			return false
		}
	}
	return true
}


func inputDimensions(scanner *bufio.Scanner) (int, int, bool) {
	fmt.Print("Введите размеры доски N M: ")
	scanner.Scan()
	line := scanner.Text()
	
	var N, M int
	_, err := fmt.Sscanf(line, "%d %d", &N, &M)
	if err != nil {
		fmt.Println("Ошибка: введите два целых числа!")
		return 0, 0, false
	}
	
	if !isValidSize(N, M) {
		fmt.Println("Ошибка: размеры должны быть положительными, а общее число клеток не более 16")
		fmt.Println("Рекомендуемые размеры: 1x1, 1x2, 2x2, 2x3, 3x3, 3x4, 4x4")
		return 0, 0, false
	}
	
	return N, M, true
}

func inputBoard(scanner *bufio.Scanner, N, M int) ([][]int, bool) {
	board := make([][]int, N)
	for i := 0; i < N; i++ {
		board[i] = make([]int, M)
	}
	
	fmt.Printf("Введите начальную доску (%d строк по %d цифр 0/1):\n", N, M)
	
	for i := 0; i < N; i++ {
		scanner.Scan()
		line := scanner.Text()
		
		if !isValidString(line, M) {
			fmt.Printf("Ошибка: строка %d должна содержать ровно %d цифр (0 или 1)\n", i+1, M)
			fmt.Print("Повторите ввод: ")
			scanner.Scan()
			line = scanner.Text()
			
			if !isValidString(line, M) {
				fmt.Println("Критическая ошибка: ввод прерван")
				return nil, false
			}
		}
		
		for j := 0; j < M; j++ {
			board[i][j] = int(line[j] - '0')
		}
	}
	
	return board, true
}

// ОСНОВНЫЕ ФУНКЦИИ 

func boardToString(board [][]int) string {
	var s strings.Builder
	for i := 0; i < len(board); i++ {
		for j := 0; j < len(board[i]); j++ {
			s.WriteByte(byte(board[i][j] + '0'))
		}
	}
	return s.String()
}

func stringToBoard(s string, N, M int) [][]int {
	board := make([][]int, N)
	for i := 0; i < N; i++ {
		board[i] = make([]int, M)
		for j := 0; j < M; j++ {
			board[i][j] = int(s[i*M+j] - '0')
		}
	}
	return board
}

func makeTarget(N, M, borderValue int) [][]int {
	target := make([][]int, N)
	for i := 0; i < N; i++ {
		target[i] = make([]int, M)
		for j := 0; j < M; j++ {
			isBorder := (i == 0 || i == N-1 || j == 0 || j == M-1)
			if isBorder {
				target[i][j] = borderValue
			} else {
				target[i][j] = 1 - borderValue
			}
		}
	}
	return target
}

func bfsMinMoves(start, target [][]int) (int, [][]int) {
	N, M := len(start), len(start[0])
	startStr := boardToString(start)
	targetStr := boardToString(target)
	
	if startStr == targetStr {
		return 0, start
	}
	
	queue := []string{startStr}
	dist := map[string]int{startStr: 0}
	
	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]
		d := dist[cur]
		
		board := stringToBoard(cur, N, M)
		
		// Горизонтальные пары
		for i := 0; i < N; i++ {
			for j := 0; j < M-1; j++ {
				newBoard := make([][]int, N)
				for k := 0; k < N; k++ {
					newBoard[k] = make([]int, M)
					copy(newBoard[k], board[k])
				}
				newBoard[i][j] ^= 1
				newBoard[i][j+1] ^= 1
				next := boardToString(newBoard)
				
				if _, ok := dist[next]; !ok {
					dist[next] = d + 1
					queue = append(queue, next)
					
					if next == targetStr {
						return d + 1, newBoard
					}
				}
			}
		}
		
		// Вертикальные пары
		for i := 0; i < N-1; i++ {
			for j := 0; j < M; j++ {
				newBoard := make([][]int, N)
				for k := 0; k < N; k++ {
					newBoard[k] = make([]int, M)
					copy(newBoard[k], board[k])
				}
				newBoard[i][j] ^= 1
				newBoard[i+1][j] ^= 1
				next := boardToString(newBoard)
				
				if _, ok := dist[next]; !ok {
					dist[next] = d + 1
					queue = append(queue, next)
					
					if next == targetStr {
						return d + 1, newBoard
					}
				}
			}
		}
	}
	
	return -1, nil
}

// ВЫБОР ЛУЧШЕГО РЕШЕНИЯ 

func selectAndPrintBestSolution(moves0, moves1 int, board0, board1 [][]int) {
	bestMoves := -1
	var bestBoard [][]int
	
	if moves0 != -1 && (bestMoves == -1 || moves0 < bestMoves) {
		bestMoves = moves0
		bestBoard = board0
	}
	if moves1 != -1 && (bestMoves == -1 || moves1 < bestMoves) {
		bestMoves = moves1
		bestBoard = board1
	}
	
	if bestMoves == -1 {
		fmt.Println(-1)
	} else {
		fmt.Printf("\nМинимальное число ходов - %d\n\n", bestMoves)
		for i := 0; i < len(bestBoard); i++ {
			for j := 0; j < len(bestBoard[i]); j++ {
				fmt.Print(bestBoard[i][j])
			}
			fmt.Println()
		}
	}
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	
	var N, M int
	var ok bool
	
	// Ввод размеров
	for {
		N, M, ok = inputDimensions(scanner)
		if ok {
			break
		}
	}
	
	// Ввод доски
	start, ok := inputBoard(scanner, N, M)
	if !ok {
		fmt.Println("Ошибка при вводе доски. Программа завершена.")
		return
	}
	
	// Создание целевых рамок
	target0 := makeTarget(N, M, 0)
	target1 := makeTarget(N, M, 1)
	
	// Поиск решения
	moves0, board0 := bfsMinMoves(start, target0)
	moves1, board1 := bfsMinMoves(start, target1)
	
	// Выбор и вывод лучшего решения
	selectAndPrintBestSolution(moves0, moves1, board0, board1)
}
