// https://www.hackerrank.com/challenges/sum-numbers-c

#include <stdio.h>

int main_sum_and_difference_of_two_numbers() {
  int i1, i2;
  float f1, f2;
  scanf("%d%d%f%f", &i1, &i2, &f1, &f2);
  printf("%d %d\n%.1f %.1f\n", i1 + i2, i1 - i2, f1 + f2, f1 - f2);
  return 0;
}
