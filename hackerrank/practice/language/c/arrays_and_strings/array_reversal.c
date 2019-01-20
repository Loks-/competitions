// https://www.hackerrank.com/challenges/reverse-array-c

#include <stdio.h>
#include <stdlib.h>

int main_array_reversal() {
  int n;
  scanf("%d", &n);
  int *arr = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; ++i) scanf("%d", arr + i);
  for (int i = n; i > 0; --i) printf("%d ", arr[i - 1]);
  free(arr);
  return 0;
}
