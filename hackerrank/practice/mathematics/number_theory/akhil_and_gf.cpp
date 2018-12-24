// https://www.hackerrank.com/challenges/akhil-and-gf

#include "common/modular/arithmetic.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/stl/base.h"

using TModularA = TModularArithmetic_C32U;

int main_akhil_and_gf() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, M, M3 = 1, ten = 10;
    cin >> N >> M;
    for (; (M % 3) == 0; M /= 3) M3 *= 3;
    uint64_t r = TModularA::DivSafe(
        TModularA::Sub(TModularA::PowUSafe(10, N, M), 1, M), 9, M);
    uint64_t r3 =
        TModularA::Sub(TModularA::PowUSafe(10, N, 9 * M3), 1, 9 * M3) / 9;
    cout << MergeRemainders<TModularA>(M3, r3, M, r) << endl;
  }
  return 0;
}
