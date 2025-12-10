package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Point struct {
	x, y int
}

func main() {
	// Read input
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	var points []Point
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}

		parts := strings.Split(line, ",")
		if len(parts) != 2 {
			continue
		}

		x, err1 := strconv.Atoi(parts[0])
		y, err2 := strconv.Atoi(parts[1])
		if err1 != nil || err2 != nil {
			continue
		}

		points = append(points, Point{x, y})
	}

	// If no input, use the example from the problem
	if len(points) == 0 {
		points = []Point{
			{7, 1}, {11, 1}, {11, 7}, {9, 7},
			{9, 5}, {2, 5}, {2, 3}, {7, 3},
		}
	}

	maxArea := 0

	// Check all pairs of points as opposite corners
	for i := 0; i < len(points); i++ {
		for j := i + 1; j < len(points); j++ {
			x1, y1 := points[i].x, points[i].y
			x2, y2 := points[j].x, points[j].y

			// Calculate rectangle area as number of grid cells
			// Need to add 1 to include both endpoints
			width := abs(x2-x1) + 1
			height := abs(y2-y1) + 1
			area := width * height

			if area > maxArea {
				maxArea = area
			}
		}
	}

	fmt.Println(maxArea)
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}
