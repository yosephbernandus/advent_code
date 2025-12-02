#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_digits(long long n) {
  if (n == 0)
    return 1;
  int count = 0;
  while (n > 0) {
    count++;
    n /= 10;
  }
  return count;
}

long long power(int base, int exp) {
  long long result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

int main() {
  FILE *file = fopen("input.txt", "r");
  if (!file) {
    printf("Error opening file\n");
    return 1;
  }

  char buffer[100000];
  fgets(buffer, sizeof(buffer), file);
  fclose(file);

  // Parse ranges
  long long ranges[1000][2];
  int range_count = 0;

  char *token = strtok(buffer, ",");
  while (token != NULL) {
    long long start, end;
    sscanf(token, "%lld-%lld", &start, &end);
    ranges[range_count][0] = start;
    ranges[range_count][1] = end;
    range_count++;
    token = strtok(NULL, ",");
  }

  long long total = 0;

  // Check each range
  for (int i = 0; i < range_count; i++) {
    long long start = ranges[i][0];
    long long end = ranges[i][1];
    int min_len = count_digits(start);
    int max_len = count_digits(end);

    // Only check even lengths
    for (int length = min_len; length <= max_len; length++) {
      if (length % 2 != 0)
        continue;

      int half_len = length / 2;
      long long pattern_start = power(10, half_len - 1);
      long long pattern_end = power(10, half_len);

      // Generate invalid IDs
      for (long long pattern = pattern_start; pattern < pattern_end;
           pattern++) {
        char pattern_str[50];
        sprintf(pattern_str, "%lld%lld", pattern, pattern);
        long long invalid_id = atoll(pattern_str);

        if (start <= invalid_id && invalid_id <= end) {
          total += invalid_id;
        }
      }
    }
  }

  printf("%lld\n", total);
  return 0;
}
