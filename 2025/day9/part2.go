package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

type Point struct {
	x, y int64
}

type Edge struct {
	x1, y1, x2, y2 int64
}

func abs(x int64) int64 {
	if x < 0 {
		return -x
	}
	return x
}

func min(a, b int64) int64 {
	if a < b {
		return a
	}
	return b
}

func max(a, b int64) int64 {
	if a > b {
		return a
	}
	return b
}

func rectangleArea(x1, y1, x2, y2 int64) int64 {
	return (abs(x2-x1) + 1) * (abs(y2-y1) + 1)
}

func intersects(minX, minY, maxX, maxY int64, edges []Edge) bool {
	for _, edge := range edges {
		iMinX := min(edge.x1, edge.x2)
		iMaxX := max(edge.x1, edge.x2)
		iMinY := min(edge.y1, edge.y2)
		iMaxY := max(edge.y1, edge.y2)

		if minX < iMaxX && maxX > iMinX && minY < iMaxY && maxY > iMinY {
			return true
		}
	}
	return false
}

func manhattanDistance(x1, y1, x2, y2 int64) int64 {
	return abs(x1-x2) + abs(y1-y2)
}

func main() {
	// Read input
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Cannot open input.txt")
		return
	}
	defer file.Close()

	var points []Point
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.Split(line, ",")
		x, _ := strconv.ParseInt(parts[0], 10, 64)
		y, _ := strconv.ParseInt(parts[1], 10, 64)
		points = append(points, Point{x, y})
	}

	fmt.Printf("Number of red tiles: %d\n", len(points))

	// Build edges and redTiles array
	var edges []Edge
	var redTiles []Point

	for i := 0; i < len(points)-1; i++ {
		edges = append(edges, Edge{
			x1: points[i].x,
			y1: points[i].y,
			x2: points[i+1].x,
			y2: points[i+1].y,
		})
		redTiles = append(redTiles, points[i], points[i+1])
	}

	// Close the polygon
	edges = append(edges, Edge{
		x1: points[len(points)-1].x,
		y1: points[len(points)-1].y,
		x2: points[0].x,
		y2: points[0].y,
	})

	// Find maximum rectangle area
	fmt.Println("Checking rectangles...")
	start := time.Now()

	var maxArea int64 = 0
	bestI, bestJ := -1, -1

	for fTIdx := 0; fTIdx < len(redTiles)-1; fTIdx++ {
		if fTIdx%100 == 0 {
			fmt.Printf("Progress: %d/%d (current max: %d)\n", fTIdx, len(redTiles), maxArea)
		}

		for tTIdx := fTIdx; tTIdx < len(redTiles); tTIdx++ {
			fromTile := redTiles[fTIdx]
			toTile := redTiles[tTIdx]

			minX := min(fromTile.x, toTile.x)
			maxX := max(fromTile.x, toTile.x)
			minY := min(fromTile.y, toTile.y)
			maxY := max(fromTile.y, toTile.y)

			// Optimize with manhattan distance
			manhattanDist := manhattanDistance(fromTile.x, fromTile.y, toTile.x, toTile.y)
			if manhattanDist*manhattanDist > maxArea {
				if !intersects(minX, minY, maxX, maxY, edges) {
					area := rectangleArea(fromTile.x, fromTile.y, toTile.x, toTile.y)
					if area > maxArea {
						maxArea = area
						bestI = fTIdx
						bestJ = tTIdx
					}
				}
			}
		}
	}

	elapsed := time.Since(start)

	fmt.Printf("\nMaximum rectangle area: %d\n", maxArea)
	if bestI >= 0 && bestJ >= 0 {
		fmt.Printf("Best pair: (%d,%d) and (%d,%d)\n",
			redTiles[bestI].x, redTiles[bestI].y,
			redTiles[bestJ].x, redTiles[bestJ].y)
	}
	fmt.Printf("Time: %v\n", elapsed)
}
