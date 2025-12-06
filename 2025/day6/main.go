package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	file, _ := os.Open("input.txt")
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	// Pad to same length
	maxLen := 0
	for _, line := range lines {
		if len(line) > maxLen {
			maxLen = len(line)
		}
	}
	for i := range lines {
		for len(lines[i]) < maxLen {
			lines[i] += " "
		}
	}

	rows := len(lines)
	cols := maxLen

	// Group columns into problems
	var problems [][]int
	var currentCols []int

	for c := 0; c < cols; c++ {
		isSeparator := true
		for r := 0; r < rows; r++ {
			if lines[r][c] != ' ' {
				isSeparator = false
				break
			}
		}

		if isSeparator {
			if len(currentCols) > 0 {
				problems = append(problems, currentCols)
				currentCols = nil
			}
		} else {
			currentCols = append(currentCols, c)
		}
	}
	if len(currentCols) > 0 {
		problems = append(problems, currentCols)
	}

	// Solve each problem
	grandTotal := 0

	for _, problemCols := range problems {
		var numbers []int

		// Extract numbers
		for r := 0; r < rows-1; r++ {
			var sb strings.Builder
			for _, c := range problemCols {
				sb.WriteByte(lines[r][c])
			}
			numStr := strings.TrimSpace(sb.String())
			if numStr != "" {
				num, _ := strconv.Atoi(numStr)
				numbers = append(numbers, num)
			}
		}

		// Extract operator
		var opSb strings.Builder
		for _, c := range problemCols {
			opSb.WriteByte(lines[rows-1][c])
		}
		operator := strings.TrimSpace(opSb.String())

		// Calculate
		result := 0
		if operator == "+" {
			for _, num := range numbers {
				result += num
			}
		} else {
			result = 1
			for _, num := range numbers {
				result *= num
			}
		}

		grandTotal += result
	}

	fmt.Println(grandTotal)
}
