#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REGIONS 1000

typedef struct {
    int x, y;
    int toFit[6];
} Region;

bool regionFits(Region* region) {
    // Each present occupies 9 cells (3x3 grid)
    int requiredArea = 0;
    for (int i = 0; i < 6; i++) {
        requiredArea += region->toFit[i] * 9;
    }

    int availableArea = region->x * region->y;
    return availableArea >= requiredArea;
}

int main() {
    FILE* f = fopen("input.txt", "r");
    if (!f) {
        printf("Cannot open input.txt\n");
        return 1;
    }

    Region regions[MAX_REGIONS];
    int numRegions = 0;

    char line[1024];
    while (fgets(line, sizeof(line), f) && numRegions < MAX_REGIONS) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines
        if (strlen(line) < 4) continue;

        // Check if this is a region line (contains 'x' and ':')
        if (strchr(line, 'x') == NULL || strchr(line, ':') == NULL) continue;

        // Find the colon
        char* colon = strchr(line, ':');
        if (!colon) continue;

        // Parse dimensions
        char dimensions[50];
        int dimLen = colon - line;
        if (dimLen >= sizeof(dimensions)) dimLen = sizeof(dimensions) - 1;
        strncpy(dimensions, line, dimLen);
        dimensions[dimLen] = '\0';

        int x, y;
        if (sscanf(dimensions, "%dx%d", &x, &y) != 2) {
            continue;
        }

        // Parse counts
        char* description = colon + 1;
        int counts[6] = {0};
        int parsed = sscanf(description, "%d %d %d %d %d %d",
                           &counts[0], &counts[1], &counts[2],
                           &counts[3], &counts[4], &counts[5]);

        if (parsed == 6) {
            Region region;
            region.x = x;
            region.y = y;
            memcpy(region.toFit, counts, sizeof(counts));

            regions[numRegions++] = region;
        }
    }

    fclose(f);

    printf("Parsed %d regions\n", numRegions);

    int result = 0;
    for (int i = 0; i < numRegions; i++) {
        if (regionFits(&regions[i])) {
            printf("Region %d (%dx%d): ✓ Fits\n", i + 1, regions[i].x, regions[i].y);
            result++;
        } else {
            printf("Region %d (%dx%d): ✗ Does not fit\n", i + 1, regions[i].x, regions[i].y);
        }
    }

    printf("\nTotal regions that can fit all presents: %d\n", result);

    return 0;
}
