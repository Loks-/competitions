// https://www.hackerrank.com/challenges/cheese-and-random-toppings

#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/factorize_extended.h"
#include "common/modular/proxy/binomial_coefficient_prime.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/stl/base.h"

int main_cheese_and_random_toppings() {
  unsigned T, N, R, M;
  PrimesTable primes_table(50);
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> R >> M;
    uint64_t r = 0, m = 1;
    TFactorization vf = FactorizeExtended(primes_table, M);
    for (auto pf : vf) {
      uint64_t rf =
          modular::proxy::BinomialCoefficientPrime(N, R, unsigned(pf.prime));
      r = MergeRemainders<modular::TArithmetic_P32U>(m, r, pf.prime, rf);
      m *= pf.prime;
    }
    cout << r << endl;
  }
  return 0;
}
