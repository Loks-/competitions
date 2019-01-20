// https://www.hackerrank.com/challenges/1d-arrays-in-c

#include <stdio.h>

int main_1d_arrays_in_c() {
  int n, s = 0, t;
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", &t);
    s += t;
  }
  printf("%d\n", s);
  return 0;
}
