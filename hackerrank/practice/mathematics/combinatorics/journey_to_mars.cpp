// https://www.hackerrank.com/challenges/ajourney

#include "common/modular/arithmetic.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

using TModularA = TModularArithmetic_C32U;

#include <cmath>

int main_journey_to_mars() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> N >> K;
    unsigned P10K = PowU(10u, K);
    long double P2l = (N - 1) * log10l(2.0L);
    uint64_t l1 = uint64_t(P10K * expl(logl(10.) * (P2l - uint64_t(P2l)))) / 10;
    uint64_t l2 = TModularA::PowU(2, N - 1, P10K);
    cout << l1 + l2 << endl;
  }
  return 0;
}
