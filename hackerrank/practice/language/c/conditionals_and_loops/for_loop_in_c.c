// https://www.hackerrank.com/challenges/for-loop-in-c

#include <stdio.h>

int main_for_loop_in_c() {
  int a, b;
  scanf("%d%d", &a, &b);
  const char *s = 0;
  for (int n = a; n <= b; ++n) {
    if (n == 1)
      s = "one";
    else if (n == 2)
      s = "two";
    else if (n == 3)
      s = "three";
    else if (n == 4)
      s = "four";
    else if (n == 5)
      s = "five";
    else if (n == 6)
      s = "six";
    else if (n == 7)
      s = "seven";
    else if (n == 8)
      s = "eight";
    else if (n == 9)
      s = "nine";
    else if ((n % 2) == 0)
      s = "even";
    else
      s = "odd";
    printf("%s\n", s);
  }
  return 0;
}
