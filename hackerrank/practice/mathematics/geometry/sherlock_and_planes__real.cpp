// https://www.hackerrank.com/challenges/sherlock-and-planes

#include "common/linear_algebra/matrix_static_size.h"
#include "common/linear_algebra/real/det.h"
#include "common/stl/base.h"

#include <cmath>

int main_sherlock_and_planes__real() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    DSquareMatrixStaticSize<4> m;
    for (unsigned i = 0; i < 4; ++i) {
      m(i, 0) = 1.0;
      for (unsigned j = 1; j < 4; ++j) cin >> m(i, j);
    }
    cout << ((fabs(la::real::Det(m)) < 0.5) ? "YES" : "NO") << endl;
  }
  return 0;
}
