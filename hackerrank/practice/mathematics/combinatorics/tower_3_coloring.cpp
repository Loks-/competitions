// https://www.hackerrank.com/challenges/tower-3-coloring

#include "common/modular/arithmetic.h"
#include "common/stl/base.h"

int main_tower_3_coloring() {
  const uint64_t M = 1000000007;
  unsigned n;
  cin >> n;
  cout << modular::TArithmetic_P32U::PowU(
              3, modular::TArithmetic_C32U::PowU(3, n, M - 1), M)
       << endl;
  return 0;
}
