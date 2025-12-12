#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DEVICES 1000
#define MAX_CONNECTIONS 20
#define MAX_NAME_LEN 10

typedef struct {
    char name[MAX_NAME_LEN];
    char outputs[MAX_CONNECTIONS][MAX_NAME_LEN];
    int num_outputs;
} Device;

typedef struct {
    Device devices[MAX_DEVICES];
    int count;
} Graph;

int find_device(Graph* graph, const char* name) {
    for (int i = 0; i < graph->count; i++) {
        if (strcmp(graph->devices[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void add_device(Graph* graph, const char* name) {
    if (find_device(graph, name) == -1) {
        strcpy(graph->devices[graph->count].name, name);
        graph->devices[graph->count].num_outputs = 0;
        graph->count++;
    }
}

void add_connection(Graph* graph, const char* from, const char* to) {
    int idx = find_device(graph, from);
    if (idx == -1) {
        add_device(graph, from);
        idx = graph->count - 1;
    }

    Device* device = &graph->devices[idx];
    strcpy(device->outputs[device->num_outputs], to);
    device->num_outputs++;
}

void parse_input(const char* filename, Graph* graph) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Cannot open %s\n", filename);
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        // Find the colon
        char* colon = strchr(line, ':');
        if (!colon) continue;

        // Extract device name
        char device_name[MAX_NAME_LEN];
        int name_len = colon - line;
        if (name_len >= MAX_NAME_LEN) name_len = MAX_NAME_LEN - 1;
        strncpy(device_name, line, name_len);
        device_name[name_len] = '\0';

        // Trim whitespace from device name
        char* end = device_name + strlen(device_name) - 1;
        while (end > device_name && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }

        // Ensure device exists
        add_device(graph, device_name);

        // Parse outputs
        char* outputs = colon + 1;
        char* token = strtok(outputs, " \t");
        while (token != NULL) {
            add_connection(graph, device_name, token);
            token = strtok(NULL, " \t");
        }
    }

    fclose(f);
}

int count_paths(Graph* graph, const char* current, const char* target, bool visited[]) {
    // Base case: reached target
    if (strcmp(current, target) == 0) {
        return 1;
    }

    // Find current device
    int current_idx = find_device(graph, current);
    if (current_idx == -1) {
        return 0;
    }

    // Mark as visited
    visited[current_idx] = true;

    Device* device = &graph->devices[current_idx];
    int total_paths = 0;

    // Explore all outputs
    for (int i = 0; i < device->num_outputs; i++) {
        int next_idx = find_device(graph, device->outputs[i]);

        // Skip if already visited
        if (next_idx != -1 && visited[next_idx]) {
            continue;
        }

        // Recursively count paths
        total_paths += count_paths(graph, device->outputs[i], target, visited);
    }

    // Unmark when backtracking
    visited[current_idx] = false;

    return total_paths;
}

int main() {
    Graph graph;
    graph.count = 0;

    parse_input("input.txt", &graph);

    printf("Parsed %d devices\n", graph.count);

    bool visited[MAX_DEVICES] = {false};
    int path_count = count_paths(&graph, "you", "out", visited);

    printf("Number of paths from 'you' to 'out': %d\n", path_count);

    return 0;
}
