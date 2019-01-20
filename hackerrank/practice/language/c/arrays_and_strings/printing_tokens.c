// https://www.hackerrank.com/challenges/printing-tokens-

#include <stdio.h>

int main_printing_tokens() {
  char s[1001];
  scanf("%1000[^\n]", s);
  for (unsigned i = 0; s[i]; ++i) {
    if (s[i] == ' ') s[i] = '\n';
  }
  printf("%s\n", s);
  return 0;
}
