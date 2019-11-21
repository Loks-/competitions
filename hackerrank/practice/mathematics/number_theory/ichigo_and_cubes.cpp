// https://www.hackerrank.com/challenges/ichigo-and-cubes

#include "common/modular_io.h"
#include "common/stl/base.h"

int main_ichigo_and_cubes() {
  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    TModularD p, q, r;
    cin >> p >> q >> r;
    cout << (p * q + q * r + r * p - 1) / 2 << endl;
  }
  return 0;
}
