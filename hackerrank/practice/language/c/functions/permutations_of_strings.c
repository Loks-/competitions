// https://www.hackerrank.com/challenges/permutations-of-strings

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap_strings(char **s1, char **s2) {
  char *s = *s1;
  *s1 = *s2;
  *s2 = s;
}

static int next_permutation(int n, char **s) {
  for (int i = n - 2; i >= 0; --i) {
    if (strcmp(s[i], s[i + 1]) < 0) {
      int j = i + 2;
      for (; (j < n) && (strcmp(s[i], s[j]) < 0);) ++j;
      swap_strings(&s[i], &s[j - 1]);
      for (j = 1; j < (n - i + 1) / 2; ++j) swap_strings(&s[i + j], &s[n - j]);
      return 1;
    }
  }
  return 0;
}

int main_permutations_of_strings() {
  char **s;
  int n;
  scanf("%d", &n);
  s = calloc(n, sizeof(char *));
  for (int i = 0; i < n; i++) {
    s[i] = calloc(11, sizeof(char));
    scanf("%s", s[i]);
  }
  do {
    for (int i = 0; i < n; i++) printf("%s%c", s[i], i == n - 1 ? '\n' : ' ');
  } while (next_permutation(n, s));
  for (int i = 0; i < n; i++) free(s[i]);
  free(s);
  return 0;
}
