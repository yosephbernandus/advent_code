#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LIGHTS 20
#define MAX_BUTTONS 20
#define MAX_BUTTON_LIGHTS 20
#define MAX_LINE_LENGTH 1024

typedef struct {
    int target[MAX_LIGHTS];
    int n_lights;
    int buttons[MAX_BUTTONS][MAX_BUTTON_LIGHTS];
    int button_sizes[MAX_BUTTONS];
    int n_buttons;
} Machine;

// Parse a machine configuration from a line
Machine parse_machine(const char *line) {
    Machine m = {0};

    // Find target lights pattern between [ and ]
    const char *start = strchr(line, '[');
    const char *end = strchr(line, ']');

    if (start && end) {
        start++;
        m.n_lights = end - start;
        for (int i = 0; i < m.n_lights; i++) {
            m.target[i] = (start[i] == '#') ? 1 : 0;
        }
    }

    // Parse button configurations between ( and )
    const char *pos = end + 1;
    m.n_buttons = 0;

    while (*pos && m.n_buttons < MAX_BUTTONS) {
        if (*pos == '(') {
            pos++;
            const char *close = strchr(pos, ')');
            if (!close) break;

            // Parse comma-separated numbers
            int btn_idx = m.n_buttons;
            m.button_sizes[btn_idx] = 0;

            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, pos, close - pos);
            buffer[close - pos] = '\0';

            char *token = strtok(buffer, ",");
            while (token && m.button_sizes[btn_idx] < MAX_BUTTON_LIGHTS) {
                m.buttons[btn_idx][m.button_sizes[btn_idx]++] = atoi(token);
                token = strtok(NULL, ",");
            }

            m.n_buttons++;
            pos = close + 1;
        } else if (*pos == '{') {
            break;  // Reached joltage requirements
        } else {
            pos++;
        }
    }

    return m;
}

// Solve the system over GF(2) and find minimum weight solution
int solve_gf2_min(int matrix[MAX_LIGHTS][MAX_BUTTONS], int target[MAX_LIGHTS], int m, int n) {
    if (m == 0 || n == 0) return 0;

    // Create augmented matrix [A | b]
    int aug[MAX_LIGHTS][MAX_BUTTONS + 1];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = matrix[i][j];
        }
        aug[i][n] = target[i];
    }

    // Gaussian elimination
    int pivot_row = 0;
    int pivot_cols[MAX_BUTTONS];
    int n_pivot_cols = 0;

    for (int col = 0; col < n; col++) {
        // Find pivot in current column
        int found_pivot = 0;
        for (int row = pivot_row; row < m; row++) {
            if (aug[row][col] == 1) {
                // Swap rows
                for (int c = 0; c <= n; c++) {
                    int tmp = aug[pivot_row][c];
                    aug[pivot_row][c] = aug[row][c];
                    aug[row][c] = tmp;
                }
                found_pivot = 1;
                break;
            }
        }

        if (!found_pivot) continue;

        pivot_cols[n_pivot_cols++] = col;

        // Eliminate all other 1s in this column
        for (int row = 0; row < m; row++) {
            if (row != pivot_row && aug[row][col] == 1) {
                for (int c = 0; c <= n; c++) {
                    aug[row][c] ^= aug[pivot_row][c];
                }
            }
        }

        pivot_row++;
    }

    // Check for inconsistency (no solution)
    for (int row = pivot_row; row < m; row++) {
        if (aug[row][n] == 1) {
            return -1;
        }
    }

    // Identify free variables
    int is_pivot[MAX_BUTTONS] = {0};
    for (int i = 0; i < n_pivot_cols; i++) {
        is_pivot[pivot_cols[i]] = 1;
    }

    int free_vars[MAX_BUTTONS];
    int n_free_vars = 0;
    for (int i = 0; i < n; i++) {
        if (!is_pivot[i]) {
            free_vars[n_free_vars++] = i;
        }
    }

    // Enumerate all possible assignments to free variables
    int min_weight = INT_MAX;

    int num_combinations = 1 << n_free_vars;
    for (int mask = 0; mask < num_combinations; mask++) {
        int solution[MAX_BUTTONS] = {0};

        // Set free variables according to current mask
        for (int i = 0; i < n_free_vars; i++) {
            if ((mask >> i) & 1) {
                solution[free_vars[i]] = 1;
            }
        }

        // Back-substitute to find values of pivot variables
        for (int i = n_pivot_cols - 1; i >= 0; i--) {
            int col = pivot_cols[i];
            int row = i;

            // Calculate value for this pivot variable
            int val = aug[row][n];
            for (int j = col + 1; j < n; j++) {
                val ^= aug[row][j] * solution[j];
            }

            solution[col] = val;
        }

        // Calculate weight (number of button presses)
        int weight = 0;
        for (int i = 0; i < n; i++) {
            weight += solution[i];
        }

        if (weight < min_weight) {
            min_weight = weight;
        }
    }

    return min_weight;
}

// Solve all machines and return total button presses
int solve_all_machines(const char *input) {
    int total = 0;
    char line[MAX_LINE_LENGTH];

    const char *pos = input;
    while (*pos) {
        // Read a line
        int i = 0;
        while (*pos && *pos != '\n' && i < MAX_LINE_LENGTH - 1) {
            line[i++] = *pos++;
        }
        line[i] = '\0';
        if (*pos == '\n') pos++;

        if (strlen(line) == 0) continue;

        // Parse machine
        Machine machine = parse_machine(line);

        // Build the matrix
        int matrix[MAX_LIGHTS][MAX_BUTTONS] = {0};
        for (int btn_idx = 0; btn_idx < machine.n_buttons; btn_idx++) {
            for (int i = 0; i < machine.button_sizes[btn_idx]; i++) {
                int light_idx = machine.buttons[btn_idx][i];
                if (light_idx < machine.n_lights) {
                    matrix[light_idx][btn_idx] = 1;
                }
            }
        }

        int min_presses = solve_gf2_min(matrix, machine.target,
                                        machine.n_lights, machine.n_buttons);

        if (min_presses == -1) {
            printf("Warning: No solution found\n");
            return -1;
        }

        total += min_presses;
    }

    return total;
}

int main() {
    // Test with examples
    const char *examples =
        "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
        "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}";

    printf("Testing with examples...\n");
    int result = solve_all_machines(examples);
    printf("Example result: %d (expected: 7)\n\n", result);

    // Read actual input
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Error reading input file\n");
        printf("Please save your input to input.txt and run again.\n");
        return 1;
    }

    // Read entire file into memory
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *input_data = malloc(file_size + 1);
    if (!input_data) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return 1;
    }

    fread(input_data, 1, file_size, fp);
    input_data[file_size] = '\0';
    fclose(fp);

    printf("Solving actual puzzle...\n");
    int answer = solve_all_machines(input_data);
    printf("\nAnswer: %d\n", answer);

    free(input_data);
    return 0;
}
