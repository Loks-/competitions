// https://www.hackerrank.com/challenges/lowest-triangle

#include "common/stl/base.h"

int main_minimum_height_triangle() {
  uint64_t a, b;
  cin >> b >> a;
  cout << 1 + (2 * a - 1) / b << endl;
  return 0;
}
