#include "common/stl/base.h"
#include <cmath>

int main_686() {
  double x1 = 1.23, x2 = 1.24;
  uint64_t k = 678910, n = 0;
  double l2 = log10(2.), lx1 = log10(x1), lx2 = log10(x2), t;
  for (;; ++n) {
    double l = modf(n * l2, &t);
    if ((l > lx1) && (l < lx2)) {
      if (--k == 0) break;
    }
  }
  std::cout << n << std::endl;
  return 0;
}
