// https://www.hackerrank.com/challenges/variadic-functions-in-c

#include <stdarg.h>

static int sum(int count, ...) {
  va_list args;
  va_start(args, count);
  int r = va_arg(args, int);
  for (; --count > 0;) {
    int t = va_arg(args, int);
    r += t;
  }
  va_end(args);
  return r;
}

static int min(int count, ...) {
  va_list args;
  va_start(args, count);
  int r = va_arg(args, int);
  for (; --count > 0;) {
    int t = va_arg(args, int);
    if (r > t) r = t;
  }
  va_end(args);
  return r;
}

static int max(int count, ...) {
  va_list args;
  va_start(args, count);
  int r = va_arg(args, int);
  for (; --count > 0;) {
    int t = va_arg(args, int);
    if (r < t) r = t;
  }
  va_end(args);
  return r;
}

int main_variadic_functions_in_c() { return 0; }
