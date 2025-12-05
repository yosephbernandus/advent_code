with open("input.txt", "r") as f:
    grid = [line.strip() for line in f]

rows = len(grid)
cols = len(grid[0]) if rows > 0 else 0

# 8 directions (including diagonals)
directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]

count = 0

for r in range(rows):
    for c in range(cols):
        if grid[r][c] == "@":
            # Count adjacent '@' symbols
            adjacent_count = 0
            for dr, dc in directions:
                nr, nc = r + dr, c + dc
                if 0 <= nr < rows and 0 <= nc < cols:
                    if grid[nr][nc] == "@":
                        adjacent_count += 1

            # Accessible if < 4 adjacent rolls
            if adjacent_count < 4:
                count += 1

print(count)
