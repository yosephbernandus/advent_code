package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// Machine represents a machine configuration
type Machine struct {
	target  []int
	buttons [][]int
}

// parseMachine parses a line into a machine configuration
func parseMachine(line string) Machine {
	// Extract target lights pattern
	start := strings.Index(line, "[")
	end := strings.Index(line, "]")
	lightsStr := line[start+1 : end]

	target := make([]int, len(lightsStr))
	for i, c := range lightsStr {
		if c == '#' {
			target[i] = 1
		} else {
			target[i] = 0
		}
	}

	// Extract button configurations
	buttons := [][]int{}
	i := end + 1
	for i < len(line) {
		if line[i] == '(' {
			// Find matching closing parenthesis
			j := i + 1
			for j < len(line) && line[j] != ')' {
				j++
			}
			// Parse the button configuration
			buttonStr := line[i+1 : j]
			if buttonStr != "" {
				parts := strings.Split(buttonStr, ",")
				button := make([]int, len(parts))
				for k, part := range parts {
					num, _ := strconv.Atoi(part)
					button[k] = num
				}
				buttons = append(buttons, button)
			}
			i = j + 1
		} else if line[i] == '{' {
			break // Reached joltage requirements
		} else {
			i++
		}
	}

	return Machine{target: target, buttons: buttons}
}

// solveGF2Min solves the system over GF(2) and finds minimum weight solution
func solveGF2Min(matrix [][]int, target []int) int {
	if len(matrix) == 0 {
		return 0
	}

	m := len(matrix)    // number of lights
	n := len(matrix[0]) // number of buttons

	// Create augmented matrix [A | b]
	aug := make([][]int, m)
	for i := 0; i < m; i++ {
		aug[i] = make([]int, n+1)
		copy(aug[i], matrix[i])
		aug[i][n] = target[i]
	}

	// Gaussian elimination
	pivotRow := 0
	pivotCols := []int{}

	for col := 0; col < n; col++ {
		// Find pivot in current column
		foundPivot := false
		for row := pivotRow; row < m; row++ {
			if aug[row][col] == 1 {
				// Swap rows
				aug[pivotRow], aug[row] = aug[row], aug[pivotRow]
				foundPivot = true
				break
			}
		}

		if !foundPivot {
			continue // This column has no pivot, it's a free variable
		}

		pivotCols = append(pivotCols, col)

		// Eliminate all other 1s in this column
		for row := 0; row < m; row++ {
			if row != pivotRow && aug[row][col] == 1 {
				for c := 0; c <= n; c++ {
					aug[row][c] ^= aug[pivotRow][c]
				}
			}
		}

		pivotRow++
	}

	// Check for inconsistency (no solution)
	for row := pivotRow; row < m; row++ {
		if aug[row][n] == 1 {
			return -1 // No solution exists
		}
	}

	// Identify free variables
	isPivot := make(map[int]bool)
	for _, col := range pivotCols {
		isPivot[col] = true
	}
	freeVars := []int{}
	for i := 0; i < n; i++ {
		if !isPivot[i] {
			freeVars = append(freeVars, i)
		}
	}

	// Enumerate all possible assignments to free variables
	minWeight := 1<<31 - 1

	numFreeVars := len(freeVars)
	for mask := 0; mask < (1 << numFreeVars); mask++ {
		solution := make([]int, n)

		// Set free variables according to current mask
		for i, varIdx := range freeVars {
			if (mask>>i)&1 == 1 {
				solution[varIdx] = 1
			}
		}

		// Back-substitute to find values of pivot variables
		for i := len(pivotCols) - 1; i >= 0; i-- {
			col := pivotCols[i]
			row := i

			// Calculate value for this pivot variable
			val := aug[row][n]
			for j := col + 1; j < n; j++ {
				val ^= aug[row][j] * solution[j]
			}

			solution[col] = val
		}

		// Calculate weight (number of button presses)
		weight := 0
		for _, v := range solution {
			weight += v
		}
		if weight < minWeight {
			minWeight = weight
		}
	}

	return minWeight
}

// solveAllMachines solves all machines and returns total button presses
func solveAllMachines(input string) int {
	total := 0
	lines := strings.Split(strings.TrimSpace(input), "\n")

	for _, line := range lines {
		line = strings.TrimSpace(line)
		if line == "" {
			continue
		}

		machine := parseMachine(line)
		nLights := len(machine.target)
		nButtons := len(machine.buttons)

		// Build the matrix
		matrix := make([][]int, nLights)
		for i := 0; i < nLights; i++ {
			matrix[i] = make([]int, nButtons)
		}

		for buttonIdx, button := range machine.buttons {
			for _, lightIdx := range button {
				if lightIdx < nLights {
					matrix[lightIdx][buttonIdx] = 1
				}
			}
		}

		minPresses := solveGF2Min(matrix, machine.target)
		if minPresses == -1 {
			fmt.Printf("Warning: No solution found\n")
			return -1
		}

		total += minPresses
	}

	return total
}

func main() {
	// Test with examples
	examples := `[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}`

	fmt.Println("Testing with examples...")
	result := solveAllMachines(examples)
	fmt.Printf("Example result: %d (expected: 7)\n\n", result)

	// Read actual input
	data, err := os.ReadFile("input.txt")
	if err != nil {
		fmt.Printf("Error reading input file: %v\n", err)
		fmt.Println("Please save your input to input.txt and run again.")
		return
	}

	fmt.Println("Solving actual puzzle...")
	answer := solveAllMachines(string(data))
	fmt.Printf("\nAnswer: %d\n", answer)
}
