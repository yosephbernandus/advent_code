#include <stdio.h>
#include <string.h>

int main() {
  FILE *file = fopen("input.txt", "r");
  char bank[1000];
  int total = 0;

  while (fgets(bank, sizeof(bank), file)) {
    // Remove newline
    bank[strcspn(bank, "\n")] = 0;

    int maxJoltage = 0;
    int n = strlen(bank);

    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        // Form 2-digit number
        int joltage = (bank[i] - '0') * 10 + (bank[j] - '0');
        if (joltage > maxJoltage) {
          maxJoltage = joltage;
        }
      }
    }

    total += maxJoltage;
  }

  fclose(file);
  printf("%d\n", total);
  return 0;
}
