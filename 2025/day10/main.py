#!/usr/bin/env python3


def parse_machine(line):
    """Parse a machine configuration line."""
    # Extract target lights pattern
    lights_start = line.find("[")
    lights_end = line.find("]")
    lights_str = line[lights_start + 1 : lights_end]
    target = [1 if c == "#" else 0 for c in lights_str]

    # Extract button configurations
    buttons = []
    i = lights_end + 1
    while i < len(line):
        if line[i] == "(":
            # Find matching closing parenthesis
            j = i + 1
            while j < len(line) and line[j] != ")":
                j += 1
            # Parse the button configuration
            button_str = line[i + 1 : j]
            if button_str:
                button = [int(x) for x in button_str.split(",")]
                buttons.append(button)
            i = j + 1
        elif line[i] == "{":
            break  # Reached joltage requirements, stop parsing
        else:
            i += 1

    return target, buttons


def solve_gf2_min(matrix, target):
    """
    Solve a system of linear equations over GF(2) and find the solution
    with minimum hamming weight (minimum number of 1s).

    Returns the minimum number of button presses, or None if no solution exists.
    """
    if not matrix:
        return 0

    m = len(matrix)  # number of lights
    n = len(matrix[0])  # number of buttons

    # Create augmented matrix [A | b]
    aug = [matrix[i][:] + [target[i]] for i in range(m)]

    # Gaussian elimination
    pivot_row = 0
    pivot_cols = []

    for col in range(n):
        # Find pivot in current column
        found_pivot = False
        for row in range(pivot_row, m):
            if aug[row][col] == 1:
                # Swap rows
                aug[pivot_row], aug[row] = aug[row], aug[pivot_row]
                found_pivot = True
                break

        if not found_pivot:
            continue  # This column has no pivot, it's a free variable

        pivot_cols.append(col)

        # Eliminate all other 1s in this column
        for row in range(m):
            if row != pivot_row and aug[row][col] == 1:
                for c in range(n + 1):
                    aug[row][c] ^= aug[pivot_row][c]

        pivot_row += 1

    # Check for inconsistency (no solution)
    for row in range(pivot_row, m):
        if aug[row][n] == 1:
            return None  # No solution exists

    # Identify free variables (buttons that can be 0 or 1)
    free_vars = [i for i in range(n) if i not in pivot_cols]

    # Enumerate all possible assignments to free variables
    # and find the solution with minimum weight
    min_weight = float("inf")

    for mask in range(1 << len(free_vars)):
        solution = [0] * n

        # Set free variables according to current mask
        for i, var_idx in enumerate(free_vars):
            solution[var_idx] = (mask >> i) & 1

        # Back-substitute to find values of pivot variables
        for i in range(len(pivot_cols) - 1, -1, -1):
            col = pivot_cols[i]
            row = i

            # Calculate value for this pivot variable
            val = aug[row][n]
            for j in range(col + 1, n):
                val ^= aug[row][j] * solution[j]

            solution[col] = val

        # Calculate weight (number of button presses)
        weight = sum(solution)
        min_weight = min(min_weight, weight)

    return min_weight


def solve_all_machines(input_text):
    """Solve for all machines and return total button presses."""
    total = 0

    for line_num, line in enumerate(input_text.strip().split("\n"), 1):
        if not line.strip():
            continue

        target, buttons = parse_machine(line)
        n_lights = len(target)
        n_buttons = len(buttons)

        # Build the matrix where each column represents a button
        # and each row represents a light
        matrix = [[0] * n_buttons for _ in range(n_lights)]
        for button_idx, button in enumerate(buttons):
            for light_idx in button:
                if light_idx < n_lights:
                    matrix[light_idx][button_idx] = 1

        min_presses = solve_gf2_min(matrix, target)

        if min_presses is None:
            print(f"Warning: No solution for machine {line_num}")
            return None

        total += min_presses

    return total


if __name__ == "__main__":
    with open("./input.txt", "r") as f:
        input_data = f.read()

    print("Solving actual puzzle...")
    answer = solve_all_machines(input_data)
    print(f"\nAnswer: {answer}")
