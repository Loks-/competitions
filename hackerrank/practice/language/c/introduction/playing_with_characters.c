// https://www.hackerrank.com/challenges/playing-with-characters

#include <stdio.h>

int main_playing_with_characters() {
  char c, s1[100], s2[100];
  scanf("%c\n%s\n%99[^\n]", &c, s1, s2);
  printf("%c\n%s\n%s\n", c, s1, s2);
  return 0;
}
