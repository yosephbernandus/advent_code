package main

import (
	"bufio"
	"fmt"
	"os"
)

type Pos struct {
	row, col int
}

func main() {
	file, _ := os.Open("input.txt")
	defer file.Close()

	var grid []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		grid = append(grid, scanner.Text())
	}

	// Find S
	var startRow, startCol int
	for r, line := range grid {
		for c, ch := range line {
			if ch == 'S' {
				startRow, startCol = r, c
				break
			}
		}
	}

	// Active beams
	beams := make(map[Pos]bool)
	beams[Pos{startRow, startCol}] = true
	splitCount := 0

	for len(beams) > 0 {
		newBeams := make(map[Pos]bool)

		for pos := range beams {
			nextRow := pos.row + 1

			// Check bounds
			if nextRow >= len(grid) {
				continue
			}
			if pos.col < 0 || pos.col >= len(grid[nextRow]) {
				continue
			}

			// Check next position
			if grid[nextRow][pos.col] == '^' {
				// Split!
				splitCount++
				newBeams[Pos{nextRow, pos.col - 1}] = true
				newBeams[Pos{nextRow, pos.col + 1}] = true
			} else {
				// Continue down
				newBeams[Pos{nextRow, pos.col}] = true
			}
		}

		beams = newBeams
	}

	fmt.Println(splitCount)
}
