package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	fileObject, err := os.Open("input.txt")
	if err != nil {
		panic(err)
	}
	defer fileObject.Close()

	var lines []string
	scanner := bufio.NewScanner(fileObject)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		lines = append(lines, line)
	}

	pointer := 50
	dialPoints := 0

	for _, line := range lines {
		dial := string(line[0])
		dial = strings.ToUpper(dial)
		number, _ := strconv.Atoi(line[1:])

		if dial == "L" {
			pointer = (pointer - number) % 100
			if pointer < 0 {
				pointer += 100
			}
		} else {
			pointer = (pointer + number) % 100
		}

		if pointer == 0 {
			dialPoints++
		}
	}

	fmt.Println(dialPoints)
}
