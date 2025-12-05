with open("input.txt", "r") as f:
    content = f.read()

# Split by blank line
parts = content.strip().split("\n\n")
range_lines = parts[0].split("\n")
id_lines = parts[1].split("\n")

# Parse ranges
ranges = []
for line in range_lines:
    start, end = map(int, line.split("-"))
    ranges.append((start, end))

# Parse available IDs
available_ids = [int(line) for line in id_lines]

# Count fresh IDs
fresh_count = 0
for id_val in available_ids:
    is_fresh = False
    for start, end in ranges:
        if start <= id_val <= end:
            is_fresh = True
            break
    if is_fresh:
        fresh_count += 1

print(fresh_count)
