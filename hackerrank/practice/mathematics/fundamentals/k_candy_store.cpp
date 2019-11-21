// https://www.hackerrank.com/challenges/k-candy-store

#include "common/modular/static/binomial_coefficient_table.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_C32<1000000000>;
using TBinomialCoefficient = ModularBinomialCoefficientTable<TModular>;

int main_k_candy_store() {
  TBinomialCoefficient bc;
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> K;
    cout << bc(N + K - 1, K) << endl;
  }
  return 0;
}
