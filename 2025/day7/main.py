# with open("input.txt", "r") as f:
#     grid = [line.rstrip("\n") for line in f]
#
# # Find S
# start_row, start_col = None, None
# for r in range(len(grid)):
#     for c in range(len(grid[r])):
#         if grid[r][c] == "S":
#             start_row, start_col = r, c
#             break
#
# # Active beams: set of (row, col)
# beams = {(start_row, start_col)}
# split_count = 0
#
# while beams:
#     new_beams = set()
#
#     for row, col in beams:
#         next_row = row + 1
#
#         # Check if out of bounds (bottom)
#         if next_row >= len(grid):
#             continue
#
#         # Check if out of bounds (left/right)
#         if col < 0 or col >= len(grid[next_row]):
#             continue
#
#         # Check what's at the next position
#         if grid[next_row][col] == "^":
#             # Hit a splitter!
#             split_count += 1
#             # Create two new beams at left and right
#             new_beams.add((next_row, col - 1))
#             new_beams.add((next_row, col + 1))
#         else:  # '.' or 'S'
#             # Continue downward
#             new_beams.add((next_row, col))
#
#     beams = new_beams
#
# print(split_count)


with open("input.txt", "r") as f:
    grid = [line.rstrip("\n") for line in f]

# Find S
start_row, start_col = None, None
for r in range(len(grid)):
    for c in range(len(grid[r])):
        if grid[r][c] == "S":
            start_row, start_col = r, c
            break

memo = {}


def count_timelines(row, col):
    # Out of bounds = timeline exits here
    if row >= len(grid) or col < 0 or col >= len(grid[0]):
        return 1

    # Memoization
    if (row, col) in memo:
        return memo[(row, col)]

    next_row = row + 1

    # Exiting at bottom
    if next_row >= len(grid):
        return 1

    # Out of bounds check for next position
    if col < 0 or col >= len(grid[next_row]):
        return 1

    # Check what's at next position
    if grid[next_row][col] == "^":
        # Splitter - particle takes both paths
        left = count_timelines(next_row, col - 1)
        right = count_timelines(next_row, col + 1)
        result = left + right
    else:
        # Continue down
        result = count_timelines(next_row, col)

    memo[(row, col)] = result
    return result


print(count_timelines(start_row, start_col))
