// https://www.hackerrank.com/challenges/devu-police

#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/eulers_totient_extended.h"
#include "common/modular/arithmetic.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

using TModularA = modular::TArithmetic_C32U;

int main_devu_vs_police() {
  factorization::table::Primes primes_table(10000);
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n1, k1, n2, k2, n, n3;
    cin >> n1 >> k1 >> n2 >> k2 >> n;
    bool small_power =
        ((n2 == 0) || (n2 == 1) || (k2 == 0) || ((n2 == 2) && (k2 <= 5)) ||
         ((n2 == 3) && (k2 <= 2)) || ((n2 <= 24) && (k2 == 1)));
    n1 = TModularA::PowU(n1, k1, n);
    n3 = EulersTotientExtended(primes_table, n);  // ET
    n2 = (small_power ? PowU(n2, k2) : 24 * n3 + TModularA::PowU(n2, k2, n3));
    cout << TModularA::PowU(n1, n2, n) % n << endl;
  }
  return 0;
}
