// https://www.hackerrank.com/challenges/hello-world-c

#include <stdio.h>

int main_hello_world_in_c() {
  char s[100];
  scanf("%99[^\n]", s);
  printf("Hello, World!\n%s\n", s);
  return 0;
}
