// https://www.hackerrank.com/challenges/mehta-and-his-laziness

#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/factorize_extended.h"
#include "common/numeric/fraction_io.h"
#include "common/stl/base.h"

int main_mehta_and_his_laziness() {
  PrimesTable primes_table(1000);
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    auto f = FactorizeExtended(primes_table, N);
    if ((f.size() == 0) || (f[0].prime != 2) || (f[0].power < 2) || (N == 4)) {
      cout << 0 << endl;
    } else {
      bool square = true;
      unsigned p = 1, q = 1;
      for (unsigned i = 0; i < f.size(); ++i) {
        square = square && ((f[i].power & 1) == 0);
        p *= (((i > 0) ? 1 : 0) + f[i].power / 2);
        q *= (1 + f[i].power);
      }
      cout << TIFraction(square ? p - 1 : p, q - 1) << endl;
    }
  }
  return 0;
}
