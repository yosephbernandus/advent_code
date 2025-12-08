import math
from collections import Counter

with open("input.txt", "r") as f:
    boxes = [tuple(map(int, line.strip().split(","))) for line in f if line.strip()]

n = len(boxes)
print(f"Number of boxes: {n}")

# Calculate all edges with distances
edges = []
for i in range(n):
    for j in range(i + 1, n):
        dx = boxes[i][0] - boxes[j][0]
        dy = boxes[i][1] - boxes[j][1]
        dz = boxes[i][2] - boxes[j][2]
        dist = math.sqrt(dx * dx + dy * dy + dz * dz)
        edges.append((dist, i, j))

edges.sort()

# Union-Find
parent = list(range(n))


def find(x):
    if parent[x] != x:
        parent[x] = find(parent[x])
    return parent[x]


def union(x, y):
    px, py = find(x), find(y)
    if px != py:
        parent[px] = py
        return True
    return False


# Make exactly 1000 shortest connections (or as many as possible)
target_connections = 1000

print(f"Target connections: {target_connections}")

# Continue connecting until all junction boxes are in one circuit
print("Connecting until all junction boxes are in one circuit...")

last_connection = None
components = n

for dist, i, j in edges:
    if union(i, j):
        components -= 1
        if components == 1:
            last_connection = (i, j)
            break

if last_connection:
    i, j = last_connection
    x1, y1, z1 = boxes[i]
    x2, y2, z2 = boxes[j]
    result = x1 * x2
    print(f"Final connection: box {i} ({x1},{y1},{z1}) and box {j} ({x2},{y2},{z2})")
    print(f"X coordinates: {x1} * {x2} = {result}")
else:
    print("No final connection found!")
    result = 0

print(f"\nAnswer: {result}")
