#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(K)
// Memory: O(K)
int main_657() {
  // unsigned K = 3, N = 4;
  uint64_t K = PowU(10ull, 7), N = PowU(10ull, 12);
  modular::mstatic::Factorial<TModularD> f;
  TModularD s = 0, one = 1;
  for (unsigned j = 0; j < K; ++j) {
    auto m = (j > 1) ? (TModularD(j).PowU(N + 1) - 1) / TModularD(j - 1)
                     : TModularD(N * j + 1);
    s += m * (((j ^ K) & 1) ? one : -one) * f.BinomialCoefficient(K, j);
  }
  cout << s << endl;
  return 0;
}
