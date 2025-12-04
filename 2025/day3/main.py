with open("input.txt", "r") as f:
    lines = [line.strip() for line in f]

total = 0

for bank in lines:
    max_joltage = 0
    n = len(bank)

    # Try all pairs
    for i in range(n):
        for j in range(i + 1, n):
            joltage = int(bank[i] + bank[j])
            max_joltage = max(max_joltage, joltage)

    total += max_joltage

print(total)
