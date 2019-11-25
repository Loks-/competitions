// https://www.hackerrank.com/challenges/highway-construction

#include "common/modular.h"
#include "common/modular/static/sum_of_powers.h"
#include "common/stl/base.h"

using TModular = TModular_P32<1000000009>;

int main_highway_construction() {
  modular::mstatic::SumOfPowers<TModular> sum_of_powers;
  unsigned T, K;
  cin >> T;
  for (unsigned i = 0; i < T; ++i) {
    uint64_t N;
    cin >> N >> K;
    cout << ((N <= 2) ? uint64_t(0)
                      : (sum_of_powers.Sum(N - 1, K) - TModular(1)).Get())
         << endl;
  }
  return 0;
}
