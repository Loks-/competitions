// https://www.hackerrank.com/challenges/conditional-statements-in-c

#include <stdio.h>

int main_conditional_statements_in_c() {
  int n;
  scanf("%d", &n);
  const char *s = 0;
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
  else
    s = "Greater than 9";
  printf("%s\n", s);
  return 0;
}
