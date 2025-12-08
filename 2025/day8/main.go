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

	// Process the 1000 shortest pairs
	pairsToProcess := 1000
	if pairsToProcess > len(edges) {
		pairsToProcess = len(edges)
	}

	for i := 0; i < pairsToProcess; i++ {
		union(edges[i].i, edges[i].j) // Try to connect, even if already connected
	}

	// Count sizes
	compSize := make(map[int]int)
	for i := 0; i < n; i++ {
		compSize[find(i)]++
	}

	var sizes []int
	for _, size := range compSize {
		sizes = append(sizes, size)
	}
	sort.Sort(sort.Reverse(sort.IntSlice(sizes)))

	fmt.Println(sizes[0] * sizes[1] * sizes[2])
}
