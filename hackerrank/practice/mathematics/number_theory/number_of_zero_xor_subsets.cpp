// https://www.hackerrank.com/challenges/number-of-subsets

#include "common/modular/arithmetic.h"
#include "common/stl/base.h"

int main_number_of_zero_xor_subsets() {
  const uint64_t M1 = 1000000007, M2 = M1 - 1;
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N;
    cin >> N;
    uint64_t p = modular::TArithmetic_C32U::PowUSafe(2, N, M2);
    p = modular::TArithmetic_C32U::SubSafe(p, N, M2);
    cout << modular::TArithmetic_P32U::PowU(2, p, M1) << endl;
  }
  return 0;
}
