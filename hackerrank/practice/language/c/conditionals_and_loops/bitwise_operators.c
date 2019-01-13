// https://www.hackerrank.com/challenges/bitwise-operators-in-c

#include <stdio.h>

int main_bitwise_operators() {
  int n, k, r1 = 0, r2 = 0, r3 = 0;
  scanf("%d%d", &n, &k);
  for (int i = 1; i < n; ++i) {
    for (int j = i + 1; j <= n; ++j) {
      if (((i & j) < k) && ((i & j) > r1)) r1 = i & j;
      if (((i | j) < k) && ((i | j) > r2)) r2 = i | j;
      if (((i ^ j) < k) && ((i ^ j) > r3)) r3 = i ^ j;
    }
  }
  printf("%d\n%d\n%d\n", r1, r2, r3);
  return 0;
}
