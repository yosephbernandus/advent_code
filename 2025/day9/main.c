#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POINTS 1000

typedef struct {
    int x, y;
} Point;

int abs_int(int x) {
    return x < 0 ? -x : x;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    Point points[MAX_POINTS];
    int point_count = 0;
    
    if (file) {
        char line[100];
        while (fgets(line, sizeof(line), file) && point_count < MAX_POINTS) {
            // Remove newline
            line[strcspn(line, "\n")] = 0;
            
            // Skip empty lines
            if (strlen(line) == 0) continue;
            
            // Parse "x,y" format
            char *comma = strchr(line, ',');
            if (comma) {
                *comma = '\0';
                points[point_count].x = atoi(line);
                points[point_count].y = atoi(comma + 1);
                point_count++;
            }
        }
        fclose(file);
    }
    
    // If no input, use the example from the problem
    if (point_count == 0) {
        Point example[] = {
            {7, 1}, {11, 1}, {11, 7}, {9, 7},
            {9, 5}, {2, 5}, {2, 3}, {7, 3}
        };
        for (int i = 0; i < 8; i++) {
            points[i] = example[i];
        }
        point_count = 8;
    }
    
    long long max_area = 0;
    
    // Check all pairs of points as opposite corners
    for (int i = 0; i < point_count; i++) {
        for (int j = i + 1; j < point_count; j++) {
            int x1 = points[i].x;
            int y1 = points[i].y;
            int x2 = points[j].x;
            int y2 = points[j].y;
            
            // Calculate rectangle area as number of grid cells
            // Need to add 1 to include both endpoints
            int width = abs_int(x2 - x1) + 1;
            int height = abs_int(y2 - y1) + 1;
            long long area = (long long)width * height;
            
            if (area > max_area) {
                max_area = area;
            }
        }
    }
    
    printf("%lld\n", max_area);
    return 0;
}