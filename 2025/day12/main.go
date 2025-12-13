package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Region struct {
	X, Y  int
	ToFit [6]int
}

func parseInput(filename string) ([]Region, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var regions []Region
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())

		// Skip empty lines and shape definitions
		if len(line) < 4 || !strings.Contains(line, "x") || !strings.Contains(line, ":") {
			continue
		}

		// Parse region: "WxH: count0 count1 count2 count3 count4 count5"
		parts := strings.Split(line, ":")
		if len(parts) != 2 {
			continue
		}

		// Parse dimensions
		dimensions := strings.TrimSpace(parts[0])
		dimParts := strings.Split(dimensions, "x")
		if len(dimParts) != 2 {
			continue
		}

		x, err1 := strconv.Atoi(dimParts[0])
		y, err2 := strconv.Atoi(dimParts[1])
		if err1 != nil || err2 != nil {
			continue
		}

		// Parse counts
		description := strings.TrimSpace(parts[1])
		countStrs := strings.Fields(description)

		var toFit [6]int
		for i, cs := range countStrs {
			if i >= 6 {
				break
			}
			val, err := strconv.Atoi(cs)
			if err == nil {
				toFit[i] = val
			}
		}

		regions = append(regions, Region{X: x, Y: y, ToFit: toFit})
	}

	return regions, scanner.Err()
}

func regionFits(region Region) bool {
	// Each present occupies 9 cells (3x3 grid)
	requiredArea := 0
	for _, count := range region.ToFit {
		requiredArea += count * 9
	}

	availableArea := region.X * region.Y
	return availableArea >= requiredArea
}

func main() {
	regions, err := parseInput("input.txt")
	if err != nil {
		fmt.Println("Error reading input:", err)
		return
	}

	fmt.Printf("Parsed %d regions\n", len(regions))

	result := 0
	for i, region := range regions {
		if regionFits(region) {
			fmt.Printf("Region %d (%dx%d): ✓ Fits\n", i+1, region.X, region.Y)
			result++
		} else {
			fmt.Printf("Region %d (%dx%d): ✗ Does not fit\n", i+1, region.X, region.Y)
		}
	}

	fmt.Printf("\nTotal regions that can fit all presents: %d\n", result)
}
