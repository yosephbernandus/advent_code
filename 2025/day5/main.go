package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Range struct {
	start int
	end   int
}

func main() {
	data, _ := os.ReadFile("input.txt")
	content := string(data)

	// Split by blank line
	parts := strings.Split(strings.TrimSpace(content), "\n\n")
	rangeLines := strings.Split(parts[0], "\n")
	idLines := strings.Split(parts[1], "\n")

	// Parse ranges
	var ranges []Range
	for _, line := range rangeLines {
		parts := strings.Split(line, "-")
		start, _ := strconv.Atoi(parts[0])
		end, _ := strconv.Atoi(parts[1])
		ranges = append(ranges, Range{start, end})
	}

	// Parse available IDs
	var availableIDs []int
	for _, line := range idLines {
		id, _ := strconv.Atoi(line)
		availableIDs = append(availableIDs, id)
	}

	// Count fresh IDs
	freshCount := 0
	for _, id := range availableIDs {
		isFresh := false
		for _, r := range ranges {
			if id >= r.start && id <= r.end {
				isFresh = true
				break
			}
		}
		if isFresh {
			freshCount++
		}
	}

	fmt.Println(freshCount)
}
