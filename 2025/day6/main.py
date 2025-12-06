with open("input.txt", "r") as f:
    lines = [line.rstrip("\n") for line in f]

# Pad lines to same length
max_len = max(len(line) for line in lines) if lines else 0
lines = [line.ljust(max_len) for line in lines]

rows = len(lines)
cols = max_len

# Group columns into problems
problems = []
current_cols = []

for c in range(cols):
    # Check if column is all spaces
    is_separator = all(lines[r][c] == " " for r in range(rows))

    if is_separator:
        if current_cols:
            problems.append(current_cols)
            current_cols = []
    else:
        current_cols.append(c)

if current_cols:
    problems.append(current_cols)

# Solve each problem
grand_total = 0

for problem_cols in problems:
    numbers = []

    # Extract numbers (all rows except last)
    for r in range(rows - 1):
        num_str = "".join(lines[r][c] for c in problem_cols).strip()
        if num_str:
            numbers.append(int(num_str))

    # Extract operator (last row)
    operator = "".join(lines[rows - 1][c] for c in problem_cols).strip()

    # Calculate result
    if operator == "+":
        result = sum(numbers)
    else:  # operator == '*'
        result = 1
        for num in numbers:
            result *= num

    grand_total += result

print(grand_total)
