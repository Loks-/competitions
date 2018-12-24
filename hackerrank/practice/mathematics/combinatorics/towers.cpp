// https://www.hackerrank.com/challenges/towers

#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TMatrix = TSquareMatrixStaticSize<TModular, 15>;

int main_towers() {
  uint64_t n;
  unsigned k, l, s = 15;
  cin >> n >> k;
  TMatrix m;
  for (unsigned i = 1; i < s; ++i) m(i, i - 1) = 1;
  for (unsigned i = 0; i < k; ++i) {
    cin >> l;
    m(0, l - 1) += 1;
  }
  cout << m.PowU(n)(0, 0) * 2 << endl;
  return 0;
}
