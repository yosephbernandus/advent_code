package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	file, _ := os.Open("input.txt")
	defer file.Close()

	var grid []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		grid = append(grid, scanner.Text())
	}

	rows := len(grid)
	if rows == 0 {
		fmt.Println(0)
		return
	}
	cols := len(grid[0])

	// 8 directions
	directions := [][2]int{
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1}, {0, 1},
		{1, -1}, {1, 0}, {1, 1},
	}

	count := 0

	for r := 0; r < rows; r++ {
		for c := 0; c < cols; c++ {
			if grid[r][c] == '@' {
				adjacentCount := 0

				for _, dir := range directions {
					nr := r + dir[0]
					nc := c + dir[1]

					if nr >= 0 && nr < rows && nc >= 0 && nc < cols {
						if grid[nr][nc] == '@' {
							adjacentCount++
						}
					}
				}

				if adjacentCount < 4 {
					count++
				}
			}
		}
	}

	fmt.Println(count)
}
