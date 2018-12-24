// https://www.hackerrank.com/challenges/strange-grid

#include "common/stl/base.h"

int main_strange_grid_again() {
  uint64_t r, c;
  cin >> r >> c;
  cout << 10 * ((r - 1) / 2) + 2 * (c - 1) + (1 - r % 2) << endl;
  return 0;
}
