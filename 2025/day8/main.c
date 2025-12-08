#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    int x, y, z;
} Point;

typedef struct {
    double dist;
    int i, j;
} Edge;

int compare_edges(const void *a, const void *b) {
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;
    if (ea->dist < eb->dist) return -1;
    if (ea->dist > eb->dist) return 1;
    return 0;
}

int compare_ints(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ib - ia; // For descending order
}

int find(int *parent, int x) {
    if (parent[x] != x) {
        parent[x] = find(parent, parent[x]);
    }
    return parent[x];
}

void union_sets(int *parent, int x, int y) {
    int root_x = find(parent, x);
    int root_y = find(parent, y);
    if (root_x != root_y) {
        parent[root_y] = root_x;
    }
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        printf("Error opening input.txt\n");
        return 1;
    }

    // Count lines first to allocate memory
    int line_count = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 1) { // Skip empty lines
            line_count++;
        }
    }
    rewind(file);

    // Allocate memory for points
    Point *points = malloc(line_count * sizeof(Point));
    if (!points) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    // Read points
    int n = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 1) {
            sscanf(line, "%d,%d,%d", &points[n].x, &points[n].y, &points[n].z);
            n++;
        }
    }
    fclose(file);

    printf("Number of boxes: %d\n", n);

    // Calculate number of edges
    int num_edges = n * (n - 1) / 2;
    Edge *edges = malloc(num_edges * sizeof(Edge));
    if (!edges) {
        printf("Memory allocation failed for edges\n");
        free(points);
        return 1;
    }

    // Calculate all pairwise distances
    printf("Calculating distances...\n");
    int edge_idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double dz = points[i].z - points[j].z;
            edges[edge_idx].dist = sqrt(dx*dx + dy*dy + dz*dz);
            edges[edge_idx].i = i;
            edges[edge_idx].j = j;
            edge_idx++;
        }
    }

    // Sort edges by distance
    printf("Sorting distances...\n");
    qsort(edges, num_edges, sizeof(Edge), compare_edges);

    // Continue connecting until all junction boxes are in one circuit
    printf("Connecting until all junction boxes are in one circuit...\n");

    // Union-Find initialization
    int *parent = malloc(n * sizeof(int));
    if (!parent) {
        printf("Memory allocation failed for parent array\n");
        free(points);
        free(edges);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    int components = n;
    int last_i = -1, last_j = -1;
    int found = 0;

    for (int i = 0; i < num_edges; i++) {
        int root_i = find(parent, edges[i].i);
        int root_j = find(parent, edges[i].j);
        
        if (root_i != root_j) {
            union_sets(parent, edges[i].i, edges[i].j);
            components--;
            if (components == 1) {
                last_i = edges[i].i;
                last_j = edges[i].j;
                found = 1;
                break;
            }
        }
    }

    if (found) {
        int x1 = points[last_i].x;
        int y1 = points[last_i].y;
        int z1 = points[last_i].z;
        int x2 = points[last_j].x;
        int y2 = points[last_j].y;
        int z2 = points[last_j].z;
        
        long long result = (long long)x1 * x2;
        printf("Final connection: box %d (%d,%d,%d) and box %d (%d,%d,%d)\n",
               last_i, x1, y1, z1, last_j, x2, y2, z2);
        printf("X coordinates: %d * %d = %lld\n", x1, x2, result);
        printf("\nAnswer: %lld\n", result);
    } else {
        printf("No final connection found!\n");
    }

    // Clean up
    free(points);
    free(edges);
    free(parent);

    return 0;
}