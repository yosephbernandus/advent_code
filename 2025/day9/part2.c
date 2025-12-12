#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_POINTS 1000

typedef struct {
    long x, y;
} Point;

typedef struct {
    long x1, y1, x2, y2;
} Edge;

long abs_val(long x) {
    return x < 0 ? -x : x;
}

long min_val(long a, long b) {
    return a < b ? a : b;
}

long max_val(long a, long b) {
    return a > b ? a : b;
}

long rectangleArea(long x1, long y1, long x2, long y2) {
    return (abs_val(x2 - x1) + 1) * (abs_val(y2 - y1) + 1);
}

// Check if rectangles intersect (matching Go algorithm exactly)
bool intersects(long minX, long minY, long maxX, long maxY, Edge* edges, int num_edges) {
    for (int i = 0; i < num_edges; i++) {
        long iMinX = min_val(edges[i].x1, edges[i].x2);
        long iMaxX = max_val(edges[i].x1, edges[i].x2);
        long iMinY = min_val(edges[i].y1, edges[i].y2);
        long iMaxY = max_val(edges[i].y1, edges[i].y2);

        if (minX < iMaxX && maxX > iMinX && minY < iMaxY && maxY > iMinY) {
            return true;
        }
    }
    return false;
}

long manhattanDistance(long x1, long y1, long x2, long y2) {
    return abs_val(x1 - x2) + abs_val(y1 - y2);
}

int main() {
    Point points[MAX_POINTS];
    Edge edges[MAX_POINTS];
    Point redTiles[MAX_POINTS * 2];  // Each edge adds 2 points
    int n = 0;
    int tileCount = 0;

    // Read input
    FILE* f = fopen("input.txt", "r");
    if (!f) {
        printf("Cannot open input.txt\n");
        return 1;
    }

    while (fscanf(f, "%ld,%ld", &points[n].x, &points[n].y) == 2) {
        n++;
    }
    fclose(f);

    printf("Number of red tiles: %d\n", n);

    // Build edges and redTiles array
    for (int fromIdx = 0; fromIdx < n - 1; fromIdx++) {
        edges[fromIdx].x1 = points[fromIdx].x;
        edges[fromIdx].y1 = points[fromIdx].y;
        edges[fromIdx].x2 = points[fromIdx + 1].x;
        edges[fromIdx].y2 = points[fromIdx + 1].y;

        redTiles[tileCount++] = points[fromIdx];
        redTiles[tileCount++] = points[fromIdx + 1];
    }

    // Close the polygon (connect last to first)
    edges[n - 1].x1 = points[n - 1].x;
    edges[n - 1].y1 = points[n - 1].y;
    edges[n - 1].x2 = points[0].x;
    edges[n - 1].y2 = points[0].y;

    // Find maximum rectangle area
    printf("Checking rectangles...\n");
    long max_area = 0;
    int best_i = -1, best_j = -1;

    for (int fTIdx = 0; fTIdx < tileCount - 1; fTIdx++) {
        if (fTIdx % 100 == 0) {
            printf("Progress: %d/%d (current max: %ld)\n", fTIdx, tileCount, max_area);
        }

        for (int tTIdx = fTIdx; tTIdx < tileCount; tTIdx++) {
            Point fromTile = redTiles[fTIdx];
            Point toTile = redTiles[tTIdx];

            long minX = min_val(fromTile.x, toTile.x);
            long maxX = max_val(fromTile.x, toTile.x);
            long minY = min_val(fromTile.y, toTile.y);
            long maxY = max_val(fromTile.y, toTile.y);

            // Optimize with manhattan distance
            long manhattanDist = manhattanDistance(fromTile.x, fromTile.y, toTile.x, toTile.y);
            if (manhattanDist * manhattanDist > max_area) {
                if (!intersects(minX, minY, maxX, maxY, edges, n)) {
                    long area = rectangleArea(fromTile.x, fromTile.y, toTile.x, toTile.y);
                    if (area > max_area) {
                        max_area = area;
                        best_i = fTIdx;
                        best_j = tTIdx;
                    }
                }
            }
        }
    }

    printf("\nMaximum rectangle area: %ld\n", max_area);
    if (best_i >= 0 && best_j >= 0) {
        printf("Best pair: (%ld,%ld) and (%ld,%ld)\n",
               redTiles[best_i].x, redTiles[best_i].y,
               redTiles[best_j].x, redTiles[best_j].y);
    }

    return 0;
}
