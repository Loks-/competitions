// https://www.hackerrank.com/challenges/sum-of-digits-of-a-five-digit-number

#include <stdio.h>

int main_sum_of_digits_of_a_five_digit_number() {
  int n, s = 0;
  scanf("%d", &n);
  for (; n; n /= 10) s += n % 10;
  printf("%d\n", s);
  return 0;
}
