// https://www.hackerrank.com/challenges/rectangular-game

#include "common/stl/base.h"

int main_rectangular_game() {
  unsigned n;
  cin >> n;
  uint64_t a, b, ma = 1000000, mb = ma;
  for (unsigned i = 0; i < n; ++i) {
    cin >> a >> b;
    ma = min(ma, a);
    mb = min(mb, b);
  }
  cout << ma * mb << endl;
  return 0;
}
