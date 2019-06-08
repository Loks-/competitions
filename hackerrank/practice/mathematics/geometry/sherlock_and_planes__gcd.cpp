// https://www.hackerrank.com/challenges/sherlock-and-planes

#include "common/linear_algebra/gcd_domain/rank.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/stl/base.h"

int main_sherlock_and_planes__gcd() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    SquareMatrixStaticSize<int64_t, 4> m;
    for (unsigned i = 0; i < 4; ++i) {
      m(i, 0) = 1;
      for (unsigned j = 1; j < 4; ++j) cin >> m(i, j);
    }
    cout << ((la::gcd::Rank(m) < 4) ? "YES" : "NO") << endl;
  }
  return 0;
}
