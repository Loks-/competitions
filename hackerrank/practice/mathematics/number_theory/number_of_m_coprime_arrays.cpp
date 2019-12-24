// https://www.hackerrank.com/challenges/number-of-m-coprime-arrays

#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/primes_powers.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

#include <numeric>

using TMatrix = SquareMatrixStaticSize<TModularD, 2>;

int main_number_of_m_coprime_arrays() {
  PrimesTable primes_table(1000000);
  TMatrix m;
  m(0, 0) = m(0, 1) = 1;

  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, M;
    cin >> N >> M;
    vector<unsigned> fp = PrimesPowers(primes_table, M);
    TModularD r = 1;
    for (unsigned q : fp) {
      m(1, 0) = q;
      r *= m.PowU(N + 1)(0, 0);
    }
    cout << r << endl;
  }
  return 0;
}
