// https://www.hackerrank.com/challenges/polygons

#include "common/modular/static/factorial_extended_prime.h"
#include "common/modular_io.h"
#include "common/numeric/utils/binomial_coefficient_prime_power.h"
#include "common/stl/base.h"

using TModular = TModular_P32<1000003>;

// R = BC(n-3, k) * BC(n+k-1, k) / (k + 1)
int main_polygons() {
  unsigned T, n, k, p = TModular::GetMod();
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> n >> k;
    cout << (((k > n - 3) ||
              (BinomialCoefficientPrimePower(n - 3, k, p) +
                   BinomialCoefficientPrimePower(n + k - 1, k, p) >
               BinomialCoefficientPrimePower(k + 1, 1, p)))
                 ? TModular(0)
                 : modular::mstatic::BinomialCoefficientExtendedPrime<TModular>(
                       n - 3, k) *
                       modular::mstatic::BinomialCoefficientExtendedPrime<
                           TModular>(n + k - 1, k) *
                       modular::mstatic::BinomialCoefficientExtendedPrime<
                           TModular>(k + 1, 1, true))
         << endl;
  }
  return 0;
}
