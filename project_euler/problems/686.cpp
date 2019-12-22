#include "common/stl/base.h"
#include <cmath>

// Time: O(N / (log(x2) - log(x1)))
// Memory: O(1)
int main_686() {
  double x1 = 1.23, x2 = 1.24;
  uint64_t N = 678910, k = 0;
  double l2 = log10(2.), lx1 = log10(x1), lx2 = log10(x2), t;
  for (;; ++k) {
    double l = modf(k * l2, &t);
    if ((l > lx1) && (l < lx2)) {
      if (--N == 0) break;
    }
  }
  std::cout << k << std::endl;
  return 0;
}
