#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGES 1000
#define MAX_IDS 10000

typedef struct {
  int start;
  int end;
} Range;

int main() {
  FILE *file = fopen("input.txt", "r");
  char line[100];

  Range ranges[MAX_RANGES];
  int rangeCount = 0;

  int availableIDs[MAX_IDS];
  int idCount = 0;

  bool readingRanges = true;

  // Read file
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;

    // Check for blank line
    if (strlen(line) == 0) {
      readingRanges = false;
      continue;
    }

    if (readingRanges) {
      // Parse range
      int start, end;
      sscanf(line, "%d-%d", &start, &end);
      ranges[rangeCount].start = start;
      ranges[rangeCount].end = end;
      rangeCount++;
    } else {
      // Parse ID
      availableIDs[idCount++] = atoi(line);
    }
  }
  fclose(file);

  // Count fresh IDs
  int freshCount = 0;
  for (int i = 0; i < idCount; i++) {
    int id = availableIDs[i];
    bool isFresh = false;

    for (int j = 0; j < rangeCount; j++) {
      if (id >= ranges[j].start && id <= ranges[j].end) {
        isFresh = true;
        break;
      }
    }

    if (isFresh) {
      freshCount++;
    }
  }

  printf("%d\n", freshCount);
  return 0;
}
