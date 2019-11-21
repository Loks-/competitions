// https://www.hackerrank.com/challenges/manasa-and-combinatorics

#include "common/modular/static/binomial_coefficient_prime.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<99991>;

int main_manasa_and_combinatorics() {
  const uint64_t p = TModular::GetMod();
  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    uint64_t n;
    cin >> n;
    cout << ModularPrimeBinomialCoefficient<TModular>(3 * n, n) -
                ModularPrimeBinomialCoefficient<TModular>(3 * n, n - 1) * 2 +
                ModularPrimeBinomialCoefficient<TModular>(3 * n, n - 2)
         << endl;
  }
  return 0;
}
