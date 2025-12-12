package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"
	"time"
)

type Machine struct {
	Buttons     [][]int
	Targets     []int
	NumButtons  int
	NumCounters int
}

func parseMachine(line string) *Machine {
	buttonRe := regexp.MustCompile(`\(([0-9,]+)\)`)
	buttonMatches := buttonRe.FindAllStringSubmatch(line, -1)

	var buttons [][]int
	for _, match := range buttonMatches {
		counterStrs := strings.Split(match[1], ",")
		var counters []int
		for _, s := range counterStrs {
			val, _ := strconv.Atoi(s)
			counters = append(counters, val)
		}
		buttons = append(buttons, counters)
	}

	joltageRe := regexp.MustCompile(`\{([0-9,]+)\}`)
	joltageMatch := joltageRe.FindStringSubmatch(line)
	if joltageMatch == nil {
		return nil
	}

	targetStrs := strings.Split(joltageMatch[1], ",")
	var targets []int
	for _, s := range targetStrs {
		val, _ := strconv.Atoi(s)
		targets = append(targets, val)
	}

	return &Machine{
		Buttons:     buttons,
		Targets:     targets,
		NumButtons:  len(buttons),
		NumCounters: len(targets),
	}
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

// Gaussian elimination to row echelon form
func gaussianElimination(matrix [][]int) []int {
	rows := len(matrix)
	if rows == 0 {
		return nil
	}
	cols := len(matrix[0])

	pivotCols := make([]int, 0)
	currentRow := 0

	for col := 0; col < cols-1 && currentRow < rows; col++ {
		// Find pivot row
		pivotRow := -1
		for row := currentRow; row < rows; row++ {
			if matrix[row][col] != 0 {
				if pivotRow == -1 || abs(matrix[row][col]) > abs(matrix[pivotRow][col]) {
					pivotRow = row
				}
			}
		}

		if pivotRow == -1 {
			continue // No pivot in this column
		}

		// Swap rows
		matrix[currentRow], matrix[pivotRow] = matrix[pivotRow], matrix[currentRow]
		pivotCols = append(pivotCols, col)

		// Eliminate below
		for row := currentRow + 1; row < rows; row++ {
			if matrix[row][col] != 0 {
				factor1 := matrix[currentRow][col]
				factor2 := matrix[row][col]

				for c := 0; c < cols; c++ {
					matrix[row][c] = matrix[row][c]*factor1 - matrix[currentRow][c]*factor2
				}
			}
		}

		currentRow++
	}

	return pivotCols
}

func solveMachine(m *Machine) int {
	// Build coefficient matrix [A | b]
	matrix := make([][]int, m.NumCounters)
	for i := range matrix {
		matrix[i] = make([]int, m.NumButtons+1)
	}

	for counterIdx := 0; counterIdx < m.NumCounters; counterIdx++ {
		for buttonIdx, button := range m.Buttons {
			affects := false
			for _, idx := range button {
				if idx == counterIdx {
					affects = true
					break
				}
			}
			if affects {
				matrix[counterIdx][buttonIdx] = 1
			}
		}
		matrix[counterIdx][m.NumButtons] = m.Targets[counterIdx]
	}

	// Perform Gaussian elimination and get pivot columns
	pivotCols := gaussianElimination(matrix)

	// Identify free variables
	isPivot := make([]bool, m.NumButtons)
	for _, col := range pivotCols {
		isPivot[col] = true
	}

	var freeVars []int
	for i := 0; i < m.NumButtons; i++ {
		if !isPivot[i] {
			freeVars = append(freeVars, i)
		}
	}

	numFree := len(freeVars)

	// Calculate search bound
	maxTarget := 0
	for _, t := range m.Targets {
		if t > maxTarget {
			maxTarget = t
		}
	}

	maxVal := 150
	if numFree <= 2 {
		maxVal = maxTarget
	} else if numFree == 3 {
		maxVal = 100
	} else if numFree == 4 {
		maxVal = 50
	} else {
		maxVal = 30
	}

	if maxVal > 250 {
		maxVal = 250
	}

	minPresses := int(1e9)

	// Try all combinations of free variables
	tryAllCombinations(m, matrix, isPivot, freeVars, 0, make([]int, m.NumButtons), maxVal, &minPresses)

	if minPresses == int(1e9) {
		return -1
	}
	return minPresses
}

func tryAllCombinations(m *Machine, matrix [][]int, isPivot []bool, freeVars []int, depth int, presses []int, maxVal int, minPresses *int) {
	if depth == len(freeVars) {
		// Back substitute to find pivot variables
		solution := make([]int, m.NumButtons)
		copy(solution, presses)

		// Process rows from bottom to top
		for rowIdx := m.NumCounters - 1; rowIdx >= 0; rowIdx-- {
			row := matrix[rowIdx]

			// Find the pivot column in this row
			pivotCol := -1
			for col := 0; col < m.NumButtons; col++ {
				if row[col] != 0 {
					pivotCol = col
					break
				}
			}

			if pivotCol == -1 {
				// No pivot - check if row is consistent
				sum := 0
				for col := 0; col < m.NumButtons; col++ {
					sum += row[col] * solution[col]
				}
				if sum != row[m.NumButtons] {
					return // Inconsistent
				}
				continue
			}

			if !isPivot[pivotCol] {
				continue // This shouldn't happen with proper Gaussian elimination
			}

			// Solve for the pivot variable
			rhs := row[m.NumButtons]
			for col := pivotCol + 1; col < m.NumButtons; col++ {
				rhs -= row[col] * solution[col]
			}

			if row[pivotCol] == 0 || rhs%row[pivotCol] != 0 {
				return // No integer solution
			}

			solution[pivotCol] = rhs / row[pivotCol]

			if solution[pivotCol] < 0 {
				return // Negative solution not allowed
			}
		}

		// Verify the solution
		for counterIdx := 0; counterIdx < m.NumCounters; counterIdx++ {
			sum := 0
			for buttonIdx, button := range m.Buttons {
				for _, idx := range button {
					if idx == counterIdx {
						sum += solution[buttonIdx]
						break
					}
				}
			}
			if sum != m.Targets[counterIdx] {
				return // Solution doesn't match targets
			}
		}

		// Calculate total presses
		total := 0
		for _, p := range solution {
			total += p
		}

		if total < *minPresses {
			*minPresses = total
		}

		return
	}

	// Try values for current free variable
	varIdx := freeVars[depth]
	for val := 0; val <= maxVal; val++ {
		presses[varIdx] = val
		tryAllCombinations(m, matrix, isPivot, freeVars, depth+1, presses, maxVal, minPresses)

		// Early termination if we already exceeded current minimum
		if val > 0 && *minPresses < int(1e9) {
			// Estimate lower bound: current assignment + minimum for remaining
			currentSum := 0
			for i := 0; i <= depth; i++ {
				currentSum += presses[freeVars[i]]
			}
			if currentSum >= *minPresses {
				break
			}
		}
	}
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Cannot open input.txt:", err)
		return
	}
	defer file.Close()

	var machines []*Machine
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if m := parseMachine(line); m != nil {
			machines = append(machines, m)
		}
	}

	fmt.Printf("Parsed %d machines\n", len(machines))

	start := time.Now()
	totalPresses := 0

	for i, m := range machines {
		presses := solveMachine(m)
		if presses == -1 {
			fmt.Printf("Machine %d: No solution found (buttons=%d, counters=%d)\n", i+1, m.NumButtons, m.NumCounters)
			// Continue instead of returning to see all failures
			continue
		}
		fmt.Printf("Machine %d: %d presses\n", i+1, presses)
		totalPresses += presses
	}

	elapsed := time.Since(start)

	fmt.Printf("\nTotal minimum button presses: %d\n", totalPresses)
	fmt.Printf("Time: %v\n", elapsed)
}
