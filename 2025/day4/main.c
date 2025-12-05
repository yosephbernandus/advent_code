#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1000

int main() {
  FILE *file = fopen("input.txt", "r");
  char grid[MAX_SIZE][MAX_SIZE];
  int rows = 0;

  // Read grid
  while (fgets(grid[rows], MAX_SIZE, file)) {
    grid[rows][strcspn(grid[rows], "\n")] = 0;
    rows++;
  }
  fclose(file);

  int cols = strlen(grid[0]);

  // 8 directions
  int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  int count = 0;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (grid[r][c] == '@') {
        int adjacentCount = 0;

        for (int d = 0; d < 8; d++) {
          int nr = r + directions[d][0];
          int nc = c + directions[d][1];

          if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (grid[nr][nc] == '@') {
              adjacentCount++;
            }
          }
        }

        if (adjacentCount < 4) {
          count++;
        }
      }
    }
  }

  printf("%d\n", count);
  return 0;
}
