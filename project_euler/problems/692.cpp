#include "common/stl/base.h"

// Time: O(log(N))
// Memory: O(1)
int main_692() {
  uint64_t N = 23416728348467685;
  uint64_t f0 = 1, f1 = 1, f2, g0 = 1, g1 = 1, g2;
  while (f0 < N) {
    f2 = f1;
    f1 = f0;
    f0 = f1 + f2;
    g2 = g1;
    g1 = g0;
    g0 = g2 + g1 + f1;
  }
  cout << g0 << endl;
  return 0;
}
