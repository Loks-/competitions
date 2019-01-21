// https://www.hackerrank.com/challenges/sorting-array-of-strings

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int lexicographic_sort(const char* a, const char* b) {
  return strcmp(a, b);
}

static int lexicographic_sort_reverse(const char* a, const char* b) {
  return -lexicographic_sort(a, b);
}

static int distinct_characters(const char* a) {
  int d = 0;
  for (int i = 0; a[i]; ++i) {
    int new_characters = 1;
    for (int j = 0; j < i; ++j) {
      if (a[j] == a[i]) {
        new_characters = 0;
        break;
      }
    }
    d += new_characters;
  }
  return d;
}

static int sort_by_number_of_distinct_characters(const char* a, const char* b) {
  int ld = distinct_characters(a) - distinct_characters(b);
  return ld ? ld : lexicographic_sort(a, b);
}

static int sort_by_length(const char* a, const char* b) {
  int ld = strlen(a) - strlen(b);
  return ld ? ld : lexicographic_sort(a, b);
}

static void string_sort(char** arr, const int len,
                        int (*cmp_func)(const char* a, const char* b)) {
  for (int i = 1; i < len; ++i) {
    for (int j = i; j > 0; --j) {
      if (cmp_func(arr[j - 1], arr[j]) > 0) {
        char* s = arr[j - 1];
        arr[j - 1] = arr[j];
        arr[j] = s;
      } else {
        break;
      }
    }
  }
}

int main_sorting_array_of_strings() {
  int n;
  scanf("%d", &n);

  char** arr;
  arr = (char**)malloc(n * sizeof(char*));

  for (int i = 0; i < n; i++) {
    *(arr + i) = malloc(1024 * sizeof(char));
    scanf("%s", *(arr + i));
    *(arr + i) = realloc(*(arr + i), strlen(*(arr + i)) + 1);
  }

  string_sort(arr, n, lexicographic_sort);
  for (int i = 0; i < n; i++) printf("%s\n", arr[i]);
  printf("\n");

  string_sort(arr, n, lexicographic_sort_reverse);
  for (int i = 0; i < n; i++) printf("%s\n", arr[i]);
  printf("\n");

  string_sort(arr, n, sort_by_length);
  for (int i = 0; i < n; i++) printf("%s\n", arr[i]);
  printf("\n");

  string_sort(arr, n, sort_by_number_of_distinct_characters);
  for (int i = 0; i < n; i++) printf("%s\n", arr[i]);
  printf("\n");

  return 0;
}
