// https://www.hackerrank.com/challenges/diagonal-difference

#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/utils/read.h"
#include "common/stl/base.h"

int main_diagonal_difference() {
  unsigned n;
  cin >> n;
  auto m = la::ReadMatrix<int>(n);
  int d = 0;
  for (unsigned i = 0; i < n; ++i) d += m(i, i) - m(i, n - i - 1);
  cout << abs(d) << endl;
  return 0;
}
