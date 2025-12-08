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

# Process the 1000 shortest pairs (regardless of whether they're already connected)
pairs_to_process = min(1000, len(edges))
print(f"Processing {pairs_to_process} shortest pairs")

for i in range(pairs_to_process):
    dist, j, k = edges[i]
    union(j, k)  # Try to connect, even if already connected

# Count component sizes
sizes = sorted(Counter(find(i) for i in range(n)).values(), reverse=True)

print(f"Number of components: {len(sizes)}")
print(f"Top 5 component sizes: {sizes[:5]}")

# If we have fewer than 3 components, pad with size 1 (isolated boxes)
while len(sizes) < 3:
    sizes.append(1)

# Answer
result = sizes[0] * sizes[1] * sizes[2]
print(f"\nAnswer: {result}")
