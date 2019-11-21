// https://www.hackerrank.com/challenges/a-chocolate-fiesta

#include "common/modular_io.h"
#include "common/stl/base.h"

int main_a_chocolate_fiesta() {
  unsigned n, x, s = 0;
  cin >> n;
  for (unsigned i = 0; i < n; ++i) {
    cin >> x;
    s += (x & 1);
  }
  cout << TModularD(2).PowU(s ? n - 1 : n) - 1 << endl;
  return 0;
}
