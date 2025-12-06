#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 10000

int main() {
  FILE *file = fopen("input.txt", "r");
  char lines[MAX_ROWS][MAX_COLS];
  int rows = 0;
  int maxCols = 0;

  // Read grid
  while (fgets(lines[rows], MAX_COLS, file)) {
    int len = strlen(lines[rows]);
    if (lines[rows][len - 1] == '\n')
      lines[rows][len - 1] = '\0';
    len = strlen(lines[rows]);
    if (len > maxCols)
      maxCols = len;
    rows++;
  }
  fclose(file);

  // Pad lines
  for (int r = 0; r < rows; r++) {
    int len = strlen(lines[r]);
    for (int i = len; i < maxCols; i++) {
      lines[r][i] = ' ';
    }
    lines[r][maxCols] = '\0';
  }

  long long grandTotal = 0;
  int c = 0;

  while (c < maxCols) {
    // Skip separator columns
    while (c < maxCols) {
      int isSep = 1;
      for (int r = 0; r < rows; r++) {
        if (lines[r][c] != ' ') {
          isSep = 0;
          break;
        }
      }
      if (!isSep)
        break;
      c++;
    }

    if (c >= maxCols)
      break;

    // Find end of problem
    int startCol = c;
    while (c < maxCols) {
      int isSep = 1;
      for (int r = 0; r < rows; r++) {
        if (lines[r][c] != ' ') {
          isSep = 0;
          break;
        }
      }
      if (isSep)
        break;
      c++;
    }
    int endCol = c;

    // Extract numbers and operator
    long long numbers[MAX_ROWS];
    int numCount = 0;
    char operator= ' ';

    for (int r = 0; r < rows - 1; r++) {
      char numStr[100] = "";
      int idx = 0;
      for (int col = startCol; col < endCol; col++) {
        numStr[idx++] = lines[r][col];
      }
      numStr[idx] = '\0';

      // Trim and parse
      char *p = numStr;
      while (isspace(*p))
        p++;
      if (*p && isdigit(*p)) {
        numbers[numCount++] = atoll(p);
      }
    }

    // Get operator
    for (int col = startCol; col < endCol; col++) {
      if (lines[rows - 1][col] == '+' || lines[rows - 1][col] == '*') {
        operator= lines[rows - 1][col];
        break;
      }
    }

    // Calculate
    long long result = 0;
    if (operator== '+') {
      for (int i = 0; i < numCount; i++) {
        result += numbers[i];
      }
    } else {
      result = 1;
      for (int i = 0; i < numCount; i++) {
        result *= numbers[i];
      }
    }

    grandTotal += result;
  }

  printf("%lld\n", grandTotal);
  return 0;
}
