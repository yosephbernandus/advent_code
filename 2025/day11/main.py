#!/usr/bin/env python3

def parse_input(filename):
    """Parse the input file and build a graph as an adjacency list."""
    graph = {}

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            # Split by colon
            parts = line.split(':')
            if len(parts) != 2:
                continue

            source = parts[0].strip()
            destinations = parts[1].strip().split()

            graph[source] = destinations

    return graph

def count_paths(graph, current, target, visited):
    """
    Count all paths from current node to target node using DFS.

    Args:
        graph: Adjacency list representation of the graph
        current: Current node name
        target: Target node name
        visited: Set of visited nodes to avoid cycles

    Returns:
        Number of paths from current to target
    """
    # Base case: reached target
    if current == target:
        return 1

    # Mark current node as visited
    visited.add(current)

    # Get neighbors of current node
    neighbors = graph.get(current, [])

    total_paths = 0
    for neighbor in neighbors:
        # Skip if already visited (avoid cycles)
        if neighbor in visited:
            continue

        # Recursively count paths from neighbor to target
        total_paths += count_paths(graph, neighbor, target, visited)

    # Unmark when backtracking
    visited.remove(current)

    return total_paths

def main():
    # Parse the input
    graph = parse_input("input.txt")

    print(f"Parsed {len(graph)} devices")

    # Count paths from 'you' to 'out'
    visited = set()
    path_count = count_paths(graph, "you", "out", visited)

    print(f"Number of paths from 'you' to 'out': {path_count}")

if __name__ == "__main__":
    main()
