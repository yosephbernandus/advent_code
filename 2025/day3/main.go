package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

func main() {
	file, _ := os.Open("input.txt")
	defer file.Close()

	scanner := bufio.NewScanner(file)
	total := 0

	for scanner.Scan() {
		bank := scanner.Text()
		maxJoltage := 0
		n := len(bank)

		for i := 0; i < n; i++ {
			for j := i + 1; j < n; j++ {
				joltageStr := string(bank[i]) + string(bank[j])
				joltage, _ := strconv.Atoi(joltageStr)
				if joltage > maxJoltage {
					maxJoltage = joltage
				}
			}
		}

		total += maxJoltage
	}

	fmt.Println(total)
}
