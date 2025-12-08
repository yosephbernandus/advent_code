#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 200
#define MAX_COLS 200
#define MAX_BEAMS 100000

typedef struct {
  int row;
  int col;
} Beam;

int main() {
  FILE *file = fopen("input.txt", "r");
  char grid[MAX_ROWS][MAX_COLS];
  int rows = 0;

  while (fgets(grid[rows], MAX_COLS, file)) {
    grid[rows][strcspn(grid[rows], "\n")] = 0;
    rows++;
  }
  fclose(file);

  // Find S
  int startRow = 0, startCol = 0;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < strlen(grid[r]); c++) {
      if (grid[r][c] == 'S') {
        startRow = r;
        startCol = c;
        break;
      }
    }
  }

  // Active beams
  Beam beams[MAX_BEAMS];
  int beamCount = 1;
  beams[0].row = startRow;
  beams[0].col = startCol;

  int splitCount = 0;

  while (beamCount > 0) {
    Beam newBeams[MAX_BEAMS];
    int newCount = 0;

    for (int i = 0; i < beamCount; i++) {
      int nextRow = beams[i].row + 1;
      int col = beams[i].col;

      // Check bounds
      if (nextRow >= rows)
        continue;
      if (col < 0 || col >= strlen(grid[nextRow]))
        continue;

      // Check next position
      if (grid[nextRow][col] == '^') {
        // Split!
        splitCount++;
        newBeams[newCount].row = nextRow;
        newBeams[newCount].col = col - 1;
        newCount++;
        newBeams[newCount].row = nextRow;
        newBeams[newCount].col = col + 1;
        newCount++;
      } else {
        // Continue down
        newBeams[newCount].row = nextRow;
        newBeams[newCount].col = col;
        newCount++;
      }
    }

    // Remove duplicates
    beamCount = 0;
    for (int i = 0; i < newCount; i++) {
      int isDup = 0;
      for (int j = 0; j < beamCount; j++) {
        if (beams[j].row == newBeams[i].row &&
            beams[j].col == newBeams[i].col) {
          isDup = 1;
          break;
        }
      }
      if (!isDup) {
        beams[beamCount++] = newBeams[i];
      }
    }
  }

  printf("%d\n", splitCount);
  return 0;
}
