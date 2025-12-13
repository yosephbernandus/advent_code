package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Device struct {
	Name    string
	Outputs []string
}

type State struct {
	device  string
	fft_visited bool
	dac_visited bool
}

func parseInput(filename string) (map[string][]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	graph := make(map[string][]string)
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}

		from, outputs, found := strings.Cut(line, ":")
		if !found {
			continue
		}

		from = strings.TrimSpace(from)
		outputList := strings.Fields(strings.TrimSpace(outputs))

		graph[from] = outputList
	}

	return graph, scanner.Err()
}

// DFS with memoization and state tracking for fft and dac visits
func dfs(device string, fft bool, dac bool, graph map[string][]string, memo map[State]int64) int64 {
	// Base case: reached "out"
	if device == "out" {
		if fft && dac {
			return 1 // Valid path: visited both fft and dac
		}
		return 0 // Invalid: didn't visit both
	}

	// Create state key for memoization
	state := State{device: device, fft_visited: fft, dac_visited: dac}

	// Check memo
	if val, ok := memo[state]; ok {
		return val
	}

	pathsCount := int64(0)

	// Update flags based on current device
	nextFft := fft || (device == "fft")
	nextDac := dac || (device == "dac")

	// Explore neighbors
	if neighbors, ok := graph[device]; ok {
		for _, nextDevice := range neighbors {
			pathsCount += dfs(nextDevice, nextFft, nextDac, graph, memo)
		}
	}

	// Store in memo
	memo[state] = pathsCount
	return pathsCount
}

func main() {
	graph, err := parseInput("input.txt")
	if err != nil {
		fmt.Println("Error reading input:", err)
		return
	}

	fmt.Printf("Parsed %d devices\n", len(graph))

	memo := make(map[State]int64)
	pathCount := dfs("svr", false, false, graph, memo)

	fmt.Printf("Paths from 'svr' to 'out' visiting both 'dac' and 'fft': %d\n", pathCount)
}
