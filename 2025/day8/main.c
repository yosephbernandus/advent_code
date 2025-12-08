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

    // Process the 1000 shortest pairs
    int pairs_to_process = 1000;
    if (pairs_to_process > num_edges) {
        pairs_to_process = num_edges;
    }
    printf("Processing %d shortest pairs\n", pairs_to_process);

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

    // Process pairs
    for (int i = 0; i < pairs_to_process; i++) {
        union_sets(parent, edges[i].i, edges[i].j);
    }

    // Count component sizes
    int *component_sizes = malloc(n * sizeof(int));
    int *component_count = malloc(n * sizeof(int));
    if (!component_sizes || !component_count) {
        printf("Memory allocation failed for component arrays\n");
        free(points);
        free(edges);
        free(parent);
        return 1;
    }

    // Initialize component counts
    for (int i = 0; i < n; i++) {
        component_count[i] = 0;
    }

    // Count components
    for (int i = 0; i < n; i++) {
        int root = find(parent, i);
        component_count[root]++;
    }

    // Extract non-zero component sizes
    int num_components = 0;
    for (int i = 0; i < n; i++) {
        if (component_count[i] > 0) {
            component_sizes[num_components++] = component_count[i];
        }
    }

    // Sort component sizes in descending order
    qsort(component_sizes, num_components, sizeof(int), compare_ints);

    printf("Number of components: %d\n", num_components);
    printf("Top 5 component sizes: ");
    for (int i = 0; i < 5 && i < num_components; i++) {
        printf("%d ", component_sizes[i]);
    }
    printf("\n");

    // Calculate result
    if (num_components >= 3) {
        long long result = (long long)component_sizes[0] * 
                          component_sizes[1] * 
                          component_sizes[2];
        printf("Answer: %d * %d * %d = %lld\n", 
               component_sizes[0], component_sizes[1], 
               component_sizes[2], result);
    } else {
        printf("Not enough components!\n");
    }

    // Clean up
    free(points);
    free(edges);
    free(parent);
    free(component_sizes);
    free(component_count);

    return 0;
}