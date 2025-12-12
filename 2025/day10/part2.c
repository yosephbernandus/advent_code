#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_BUTTONS 20
#define MAX_COUNTERS 20
#define MAX_MACHINES 300

typedef struct {
    int buttons[MAX_BUTTONS][MAX_COUNTERS];  // buttons[i] contains indices that button i affects
    int button_sizes[MAX_BUTTONS];           // number of counters each button affects
    int targets[MAX_COUNTERS];
    int num_buttons;
    int num_counters;
} Machine;

int abs_val(int x) {
    return x < 0 ? -x : x;
}

// Parse a machine from input line
bool parse_machine(const char* line, Machine* m) {
    m->num_buttons = 0;
    m->num_counters = 0;

    // Find joltage requirements in {curly braces}
    const char* joltage_start = strchr(line, '{');
    if (!joltage_start) return false;

    // Parse joltage requirements
    const char* p = joltage_start + 1;
    int count = 0;
    while (*p && *p != '}') {
        if (*p >= '0' && *p <= '9') {
            m->targets[count++] = atoi(p);
            while (*p >= '0' && *p <= '9') p++;
        } else {
            p++;
        }
    }
    m->num_counters = count;

    // Parse buttons - find all (parentheses)
    p = line;
    while (*p && p < joltage_start) {
        if (*p == '(') {
            p++;
            int btn_size = 0;

            // Parse the counter indices
            while (*p && *p != ')') {
                if (*p >= '0' && *p <= '9') {
                    m->buttons[m->num_buttons][btn_size++] = atoi(p);
                    while (*p >= '0' && *p <= '9') p++;
                } else {
                    p++;
                }
            }
            m->button_sizes[m->num_buttons] = btn_size;
            m->num_buttons++;
        }
        p++;
    }

    return m->num_buttons > 0 && m->num_counters > 0;
}

// Gaussian elimination to row echelon form, returns pivot columns
int gaussian_elimination(int matrix[][MAX_BUTTONS + 1], int rows, int cols, int pivot_cols[]) {
    int num_pivots = 0;
    int current_row = 0;

    for (int col = 0; col < cols - 1 && current_row < rows; col++) {
        // Find pivot row
        int pivot_row = -1;
        for (int row = current_row; row < rows; row++) {
            if (matrix[row][col] != 0) {
                if (pivot_row == -1 || abs_val(matrix[row][col]) > abs_val(matrix[pivot_row][col])) {
                    pivot_row = row;
                }
            }
        }

        if (pivot_row == -1) {
            continue; // No pivot in this column
        }

        // Swap rows
        if (pivot_row != current_row) {
            for (int c = 0; c < cols; c++) {
                int temp = matrix[current_row][c];
                matrix[current_row][c] = matrix[pivot_row][c];
                matrix[pivot_row][c] = temp;
            }
        }

        pivot_cols[num_pivots++] = col;

        // Eliminate below
        for (int row = current_row + 1; row < rows; row++) {
            if (matrix[row][col] != 0) {
                int factor1 = matrix[current_row][col];
                int factor2 = matrix[row][col];

                for (int c = 0; c < cols; c++) {
                    matrix[row][c] = matrix[row][c] * factor1 - matrix[current_row][c] * factor2;
                }
            }
        }

        current_row++;
    }

    return num_pivots;
}

// Forward declaration
void try_all_combinations(Machine* m, int matrix[][MAX_BUTTONS + 1], bool is_pivot[],
                         int free_vars[], int num_free, int depth, int presses[],
                         int max_val, int* min_presses);

int solve_machine(Machine* m) {
    // Build coefficient matrix [A | b]
    int matrix[MAX_COUNTERS][MAX_BUTTONS + 1];
    memset(matrix, 0, sizeof(matrix));

    for (int counter_idx = 0; counter_idx < m->num_counters; counter_idx++) {
        for (int button_idx = 0; button_idx < m->num_buttons; button_idx++) {
            // Check if this button affects this counter
            bool affects = false;
            for (int i = 0; i < m->button_sizes[button_idx]; i++) {
                if (m->buttons[button_idx][i] == counter_idx) {
                    affects = true;
                    break;
                }
            }
            if (affects) {
                matrix[counter_idx][button_idx] = 1;
            }
        }
        matrix[counter_idx][m->num_buttons] = m->targets[counter_idx];
    }

    // Perform Gaussian elimination and get pivot columns
    int pivot_cols[MAX_BUTTONS];
    int num_pivots = gaussian_elimination(matrix, m->num_counters, m->num_buttons + 1, pivot_cols);

    // Identify free variables
    bool is_pivot[MAX_BUTTONS] = {false};
    for (int i = 0; i < num_pivots; i++) {
        is_pivot[pivot_cols[i]] = true;
    }

    int free_vars[MAX_BUTTONS];
    int num_free = 0;
    for (int i = 0; i < m->num_buttons; i++) {
        if (!is_pivot[i]) {
            free_vars[num_free++] = i;
        }
    }

    // Calculate search bound
    int max_target = 0;
    for (int i = 0; i < m->num_counters; i++) {
        if (m->targets[i] > max_target) {
            max_target = m->targets[i];
        }
    }

    int max_val = 150;
    if (num_free <= 2) {
        max_val = max_target;
    } else if (num_free == 3) {
        max_val = 100;
    } else if (num_free == 4) {
        max_val = 50;
    } else {
        max_val = 30;
    }

    if (max_val > 250) {
        max_val = 250;
    }

    int min_presses = INT_MAX;
    int presses[MAX_BUTTONS] = {0};

    // Try all combinations of free variables
    try_all_combinations(m, matrix, is_pivot, free_vars, num_free, 0, presses, max_val, &min_presses);

    return min_presses == INT_MAX ? -1 : min_presses;
}

void try_all_combinations(Machine* m, int matrix[][MAX_BUTTONS + 1], bool is_pivot[],
                         int free_vars[], int num_free, int depth, int presses[],
                         int max_val, int* min_presses) {
    if (depth == num_free) {
        // Back substitute to find pivot variables
        int solution[MAX_BUTTONS];
        memcpy(solution, presses, sizeof(int) * m->num_buttons);

        // Process rows from bottom to top
        for (int row_idx = m->num_counters - 1; row_idx >= 0; row_idx--) {
            int* row = matrix[row_idx];

            // Find the pivot column in this row
            int pivot_col = -1;
            for (int col = 0; col < m->num_buttons; col++) {
                if (row[col] != 0) {
                    pivot_col = col;
                    break;
                }
            }

            if (pivot_col == -1) {
                // No pivot - check if row is consistent
                int sum = 0;
                for (int col = 0; col < m->num_buttons; col++) {
                    sum += row[col] * solution[col];
                }
                if (sum != row[m->num_buttons]) {
                    return; // Inconsistent
                }
                continue;
            }

            if (!is_pivot[pivot_col]) {
                continue;
            }

            // Solve for the pivot variable
            int rhs = row[m->num_buttons];
            for (int col = pivot_col + 1; col < m->num_buttons; col++) {
                rhs -= row[col] * solution[col];
            }

            if (row[pivot_col] == 0 || rhs % row[pivot_col] != 0) {
                return; // No integer solution
            }

            solution[pivot_col] = rhs / row[pivot_col];

            if (solution[pivot_col] < 0) {
                return; // Negative solution not allowed
            }
        }

        // Verify the solution
        for (int counter_idx = 0; counter_idx < m->num_counters; counter_idx++) {
            int sum = 0;
            for (int button_idx = 0; button_idx < m->num_buttons; button_idx++) {
                for (int i = 0; i < m->button_sizes[button_idx]; i++) {
                    if (m->buttons[button_idx][i] == counter_idx) {
                        sum += solution[button_idx];
                        break;
                    }
                }
            }
            if (sum != m->targets[counter_idx]) {
                return; // Solution doesn't match targets
            }
        }

        // Calculate total presses
        int total = 0;
        for (int i = 0; i < m->num_buttons; i++) {
            total += solution[i];
        }

        if (total < *min_presses) {
            *min_presses = total;
        }

        return;
    }

    // Try values for current free variable
    int var_idx = free_vars[depth];
    for (int val = 0; val <= max_val; val++) {
        presses[var_idx] = val;
        try_all_combinations(m, matrix, is_pivot, free_vars, num_free, depth + 1, presses, max_val, min_presses);

        // Early termination if we already exceeded current minimum
        if (val > 0 && *min_presses < INT_MAX) {
            int current_sum = 0;
            for (int i = 0; i <= depth; i++) {
                current_sum += presses[free_vars[i]];
            }
            if (current_sum >= *min_presses) {
                break;
            }
        }
    }
}

int main() {
    FILE* f = fopen("input.txt", "r");
    if (!f) {
        printf("Cannot open input.txt\n");
        return 1;
    }

    Machine machines[MAX_MACHINES];
    int num_machines = 0;

    char line[2048];
    while (fgets(line, sizeof(line), f) && num_machines < MAX_MACHINES) {
        if (parse_machine(line, &machines[num_machines])) {
            num_machines++;
        }
    }
    fclose(f);

    printf("Parsed %d machines\n", num_machines);

    int total_presses = 0;
    bool all_solved = true;

    for (int i = 0; i < num_machines; i++) {
        int presses = solve_machine(&machines[i]);
        if (presses == -1) {
            printf("Machine %d: No solution found (buttons=%d, counters=%d)\n",
                   i + 1, machines[i].num_buttons, machines[i].num_counters);
            all_solved = false;
            continue;
        }
        printf("Machine %d: %d presses\n", i + 1, presses);
        total_presses += presses;
    }

    if (all_solved) {
        printf("\nTotal minimum button presses: %d\n", total_presses);
    } else {
        printf("\nSome machines could not be solved\n");
    }

    return 0;
}
