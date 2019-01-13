// https://www.hackerrank.com/challenges/printing-pattern-2

#include <stdio.h>
#include <stdlib.h>

int main_printing_pattern_using_loops() {
  int n;
  scanf("%d", &n);
  for (int i = 1; i < 2 * n; ++i) {
    int x = abs(i - n) + 1;
    for (int j = 1; j < 2 * n; ++j) {
      int y = abs(j - n) + 1;
      printf("%d ", x < y ? y : x);
    }
    printf("\n");
  }
  return 0;
}
