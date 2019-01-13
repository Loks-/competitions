// https://www.hackerrank.com/challenges/functions-in-c

#include <stdio.h>

static int max_of_two(int a, int b) { return a < b ? b : a; }

static int max_of_four(int a, int b, int c, int d) {
  return max_of_two(max_of_two(a, b), max_of_two(c, d));
}

int main_functions_in_c() {
  int a, b, c, d;
  scanf("%d %d %d %d", &a, &b, &c, &d);
  int ans = max_of_four(a, b, c, d);
  printf("%d\n", ans);
  return 0;
}
