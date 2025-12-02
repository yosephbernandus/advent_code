package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	data, _ := os.ReadFile("input.txt")
	inputData := strings.TrimSpace(string(data))

	// Parse ranges
	var ranges [][2]int
	for _, part := range strings.Split(inputData, ",") {
		parts := strings.Split(part, "-")
		start, _ := strconv.Atoi(parts[0])
		end, _ := strconv.Atoi(parts[1])
		ranges = append(ranges, [2]int{start, end})
	}

	total := 0

	// Check each range
	for _, r := range ranges {
		start, end := r[0], r[1]
		minLen := len(strconv.Itoa(start))
		maxLen := len(strconv.Itoa(end))

		// Only check even lengths
		for length := minLen; length <= maxLen; length++ {
			if length%2 != 0 {
				continue
			}

			halfLen := length / 2
			patternStart := 1
			for i := 1; i < halfLen; i++ {
				patternStart *= 10
			}
			patternEnd := patternStart * 10

			// Generate invalid IDs
			for pattern := patternStart; pattern < patternEnd; pattern++ {
				patternStr := strconv.Itoa(pattern)
				invalidIDStr := patternStr + patternStr
				invalidID, _ := strconv.Atoi(invalidIDStr)

				if start <= invalidID && invalidID <= end {
					total += invalidID
				}
			}
		}
	}

	fmt.Println(total)
}
