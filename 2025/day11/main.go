package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Graph map[string][]string

func parseInput(filename string) (Graph, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	graph := make(Graph)
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}

		parts := strings.Split(line, ":")
		if len(parts) != 2 {
			continue
		}

		source := strings.TrimSpace(parts[0])
		destinations := strings.Fields(strings.TrimSpace(parts[1]))

		graph[source] = destinations
	}

	return graph, scanner.Err()
}

func countPaths(graph Graph, current string, target string, visited map[string]bool) int {
	// Base case: reached target
	if current == target {
		return 1
	}

	// Mark current node as visited
	visited[current] = true
	defer func() { delete(visited, current) }() // Unmark when backtracking

	// Get neighbors of current node
	neighbors, exists := graph[current]
	if !exists {
		return 0
	}

	totalPaths := 0
	for _, neighbor := range neighbors {
		// Skip if already visited (avoid cycles)
		if visited[neighbor] {
			continue
		}

		// Recursively count paths from neighbor to target
		totalPaths += countPaths(graph, neighbor, target, visited)
	}

	return totalPaths
}

func main() {
	graph, err := parseInput("input.txt")
	if err != nil {
		fmt.Println("Error reading input:", err)
		return
	}

	visited := make(map[string]bool)
	pathCount := countPaths(graph, "you", "out", visited)

	fmt.Printf("Number of paths from 'you' to 'out': %d\n", pathCount)
}
