#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DEVICES 1000
#define MAX_CONNECTIONS 20
#define MAX_NAME_LEN 10
#define MAX_MEMO 100000

typedef struct {
    char name[MAX_NAME_LEN];
    char outputs[MAX_CONNECTIONS][MAX_NAME_LEN];
    int num_outputs;
} Device;

typedef struct {
    Device devices[MAX_DEVICES];
    int count;
} Graph;

typedef struct {
    char device[MAX_NAME_LEN];
    bool fft_visited;
    bool dac_visited;
} State;

typedef struct {
    State state;
    long long value;
} MemoEntry;

typedef struct {
    MemoEntry entries[MAX_MEMO];
    int count;
} Memo;

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
        line[strcspn(line, "\n")] = 0;

        char* colon = strchr(line, ':');
        if (!colon) continue;

        char device_name[MAX_NAME_LEN];
        int name_len = colon - line;
        if (name_len >= MAX_NAME_LEN) name_len = MAX_NAME_LEN - 1;
        strncpy(device_name, line, name_len);
        device_name[name_len] = '\0';

        char* end = device_name + strlen(device_name) - 1;
        while (end > device_name && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }

        add_device(graph, device_name);

        char* outputs = colon + 1;
        char* token = strtok(outputs, " \t");
        while (token != NULL) {
            add_connection(graph, device_name, token);
            token = strtok(NULL, " \t");
        }
    }

    fclose(f);
}

bool state_equal(State* s1, State* s2) {
    return strcmp(s1->device, s2->device) == 0 &&
           s1->fft_visited == s2->fft_visited &&
           s1->dac_visited == s2->dac_visited;
}

long long memo_get(Memo* memo, State* state, bool* found) {
    for (int i = 0; i < memo->count; i++) {
        if (state_equal(&memo->entries[i].state, state)) {
            *found = true;
            return memo->entries[i].value;
        }
    }
    *found = false;
    return 0;
}

void memo_set(Memo* memo, State* state, long long value) {
    if (memo->count < MAX_MEMO) {
        memo->entries[memo->count].state = *state;
        memo->entries[memo->count].value = value;
        memo->count++;
    }
}

long long dfs(Graph* graph, const char* device, bool fft, bool dac, Memo* memo) {
    // Base case: reached "out"
    if (strcmp(device, "out") == 0) {
        return (fft && dac) ? 1 : 0;
    }

    // Create state for memoization
    State state;
    strcpy(state.device, device);
    state.fft_visited = fft;
    state.dac_visited = dac;

    // Check memo
    bool found;
    long long cached = memo_get(memo, &state, &found);
    if (found) {
        return cached;
    }

    long long paths_count = 0;

    // Update flags based on current device
    bool next_fft = fft || (strcmp(device, "fft") == 0);
    bool next_dac = dac || (strcmp(device, "dac") == 0);

    // Find current device and explore neighbors
    int device_idx = find_device(graph, device);
    if (device_idx != -1) {
        Device* dev = &graph->devices[device_idx];
        for (int i = 0; i < dev->num_outputs; i++) {
            paths_count += dfs(graph, dev->outputs[i], next_fft, next_dac, memo);
        }
    }

    // Store in memo
    memo_set(memo, &state, paths_count);
    return paths_count;
}

int main() {
    Graph graph;
    graph.count = 0;

    parse_input("input.txt", &graph);

    printf("Parsed %d devices\n", graph.count);

    Memo memo = {0};
    long long path_count = dfs(&graph, "svr", false, false, &memo);

    printf("Paths from 'svr' to 'out' visiting both 'dac' and 'fft': %lld\n", path_count);

    return 0;
}
