// https://www.hackerrank.com/challenges/recursion-in-c

#include <stdio.h>

static int find_nth_term(int n, int a, int b, int c) {
  switch (n) {
    case 1:
      return a;
    case 2:
      return b;
    case 3:
      return c;
    default:
      return find_nth_term(n - 1, a, b, c) + find_nth_term(n - 2, a, b, c) +
             find_nth_term(n - 3, a, b, c);
  }
}

int main_calculate_the_nth_term() {
  int n, a, b, c;
  scanf("%d %d %d %d", &n, &a, &b, &c);
  printf("%d\n", find_nth_term(n, a, b, c));
  return 0;
}
