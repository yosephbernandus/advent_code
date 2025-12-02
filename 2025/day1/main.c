#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *file_object = fopen("input.txt", "r");
  if (file_object == NULL) {
    printf("Error opening file\n");
    return 1;
  }

  char lines[1000][100];
  int line_count = 0;
  char buffer[100];

  while (fgets(buffer, sizeof(buffer), file_object)) {
    buffer[strcspn(buffer, "\n")] = 0; // strip newline
    strcpy(lines[line_count], buffer);
    line_count++;
  }

  fclose(file_object);

  int pointer = 50;
  int dial_points = 0;

  for (int i = 0; i < line_count; i++) {
    char dial = toupper(lines[i][0]);
    int number = atoi(&lines[i][1]);

    if (dial == 'L') {
      pointer = (pointer - number) % 100;
      if (pointer < 0) {
        pointer += 100;
      }
    } else {
      pointer = (pointer + number) % 100;
    }

    if (pointer == 0) {
      dial_points++;
    }
  }

  printf("%d\n", dial_points);

  return 0;
}
