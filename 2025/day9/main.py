with open("input.txt", "r") as f:
    lines = f.read().strip().split("\n")

# Parse coordinates
points = []
for line in lines:
    if line.strip():
        x, y = map(int, line.split(","))
        points.append((x, y))

# If no input, use the example from the problem
if not points:
    points = [(7, 1), (11, 1), (11, 7), (9, 7), (9, 5), (2, 5), (2, 3), (7, 3)]

max_area = 0

# Check all pairs of points as opposite corners
for i in range(len(points)):
    for j in range(i + 1, len(points)):
        x1, y1 = points[i]
        x2, y2 = points[j]

        # Calculate rectangle area as number of grid cells
        # Need to add 1 to include both endpoints
        width = abs(x2 - x1) + 1
        height = abs(y2 - y1) + 1
        area = width * height

        if area > max_area:
            max_area = area

print(max_area)
