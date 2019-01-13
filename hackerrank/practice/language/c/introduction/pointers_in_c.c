// https://www.hackerrank.com/challenges/pointer-in-c

#include <stdio.h>
#include <stdlib.h>

static void update(int *a, int *b) {
  int c = *a + *b;
  int d = abs(*a - *b);
  *a = c;
  *b = d;
}

int main_pointers_in_c() {
  int a, b;
  int *pa = &a, *pb = &b;
  scanf("%d %d", &a, &b);
  update(pa, pb);
  printf("%d\n%d\n", a, b);
  return 0;
}
