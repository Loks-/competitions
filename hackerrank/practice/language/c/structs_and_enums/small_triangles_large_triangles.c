// https://www.hackerrank.com/challenges/small-triangles-large-triangles

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct triangle {
  int a;
  int b;
  int c;
};

typedef struct triangle triangle;

static int sq_area(triangle t) {
  return (t.a + t.b + t.c) * (t.a + t.b - t.c) * (t.a - t.b + t.c) *
         (-t.a + t.b + t.c);
}

static int cmp(const void* p, const void* q) {
  sq_area(*(const triangle*)q) - sq_area(*(const triangle*)p);
}

static void sort_by_area(triangle* tr, int n) {
  qsort((void*)tr, n, sizeof(triangle), cmp);
}

int main_small_triangles_large_triangles() {
  int n;
  scanf("%d", &n);
  triangle* tr = malloc(n * sizeof(triangle));
  for (int i = 0; i < n; i++) {
    scanf("%d%d%d", &tr[i].a, &tr[i].b, &tr[i].c);
  }
  sort_by_area(tr, n);
  for (int i = 0; i < n; i++) {
    printf("%d %d %d\n", tr[i].a, tr[i].b, tr[i].c);
  }
  return 0;
}
