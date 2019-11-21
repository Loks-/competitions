// https://www.hackerrank.com/challenges/number-of-m-coprime-arrays

#include "common/factorization/factorization.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular.h"
#include "common/stl/base.h"

#include <numeric>

using TMatrix = SquareMatrixStaticSize<TModularD, 2>;

int main_number_of_m_coprime_arrays__factorization() {
  TMatrix m;
  m(0, 0) = m(0, 1) = 1;

  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, M;
    cin >> N >> M;
    TFactorization fp = Factorize(M);
    TModularD r = 1;
    for (auto q : fp) {
      m(1, 0) = q.power;
      r *= m.PowU(N + 1)(0, 0);
    }
    cout << r.Get() << endl;
  }
  return 0;
}
