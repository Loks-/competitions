// https://www.hackerrank.com/challenges/frequency-of-digits-1

#include <stdio.h>
#include <string.h>

int main_digit_frequency() {
  char s[1001];
  scanf("%s", s);
  int cnt[256];
  memset(cnt, 0, sizeof(cnt));
  for (unsigned i = 0; s[i]; ++i) ++cnt[s[i]];
  for (char i = '0'; i <= '9'; ++i) printf("%d ", cnt[i]);
  return 0;
}
