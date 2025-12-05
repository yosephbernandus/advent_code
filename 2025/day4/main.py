# with open("input.txt", "r") as f:
#     grid = [line.strip() for line in f]
#
# rows = len(grid)
# cols = len(grid[0]) if rows > 0 else 0
#
# # 8 directions (including diagonals)
# directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
#
# count = 0
#
# for r in range(rows):
#     for c in range(cols):
#         if grid[r][c] == "@":
#             # Count adjacent '@' symbols
#             adjacent_count = 0
#             for dr, dc in directions:
#                 nr, nc = r + dr, c + dc
#                 if 0 <= nr < rows and 0 <= nc < cols:
#                     if grid[nr][nc] == "@":
#                         adjacent_count += 1
#
#             # Accessible if < 4 adjacent rolls
#             if adjacent_count < 4:
#                 count += 1
#
# print(count)


## part 2
with open("input.txt", "r") as f:
    grid = [list(line.strip()) for line in f]

rows = len(grid)
cols = len(grid[0]) if rows > 0 else 0

directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]


def find_accessible(grid, rows, cols):
    """Find all accessible rolls (< 4 adjacent rolls)"""
    accessible = []
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == "@":
                adjacent_count = 0
                for dr, dc in directions:
                    nr, nc = r + dr, c + dc
                    if 0 <= nr < rows and 0 <= nc < cols:
                        if grid[nr][nc] == "@":
                            adjacent_count += 1

                if adjacent_count < 4:
                    accessible.append((r, c))
    return accessible


total_removed = 0

while True:
    # Find accessible rolls
    accessible = find_accessible(grid, rows, cols)

    if not accessible:
        break

    # Remove them all
    for r, c in accessible:
        grid[r][c] = "."

    total_removed += len(accessible)

print(total_removed)
