package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Edge struct {
	dist float64
	i, j int
}

var parent []int

func find(x int) int {
	if parent[x] != x {
		parent[x] = find(parent[x])
	}
	return parent[x]
}

func union(x, y int) bool {
	px, py := find(x), find(y)
	if px != py {
		parent[px] = py
		return true
	}
	return false
}

func main() {
	file, _ := os.Open("input.txt")
	defer file.Close()

	var boxes [][3]int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		parts := strings.Split(scanner.Text(), ",")
		x, _ := strconv.Atoi(parts[0])
		y, _ := strconv.Atoi(parts[1])
		z, _ := strconv.Atoi(parts[2])
		boxes = append(boxes, [3]int{x, y, z})
	}

	n := len(boxes)

	// Build edges
	var edges []Edge
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			dx := boxes[i][0] - boxes[j][0]
			dy := boxes[i][1] - boxes[j][1]
			dz := boxes[i][2] - boxes[j][2]
			dist := math.Sqrt(float64(dx*dx + dy*dy + dz*dz))
			edges = append(edges, Edge{dist, i, j})
		}
	}

	sort.Slice(edges, func(i, j int) bool {
		return edges[i].dist < edges[j].dist
	})

	// Union-Find
	parent = make([]int, n)
	for i := range parent {
		parent[i] = i
	}

	// Continue connecting until all junction boxes are in one circuit
	fmt.Println("Connecting until all junction boxes are in one circuit...")

	components := n
	var lastConnection [2]int
	found := false

	for _, edge := range edges {
		if union(edge.i, edge.j) {
			components--
			if components == 1 {
				lastConnection = [2]int{edge.i, edge.j}
				found = true
				break
			}
		}
	}

	if found {
		i, j := lastConnection[0], lastConnection[1]
		x1, y1, z1 := boxes[i][0], boxes[i][1], boxes[i][2]
		x2, y2, z2 := boxes[j][0], boxes[j][1], boxes[j][2]
		result := x1 * x2
		fmt.Printf("Final connection: box %d (%d,%d,%d) and box %d (%d,%d,%d)\n",
			i, x1, y1, z1, j, x2, y2, z2)
		fmt.Printf("X coordinates: %d * %d = %d\n", x1, x2, result)
		fmt.Printf("\nAnswer: %d\n", result)
	} else {
		fmt.Println("No final connection found!")
	}
}
