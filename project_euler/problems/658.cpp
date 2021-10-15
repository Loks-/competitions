#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(K)
// Memory: O(K)
int main_658() {
  // unsigned K = 10, N = 100;
  uint64_t K = PowU(10ull, 7), N = PowU(10ull, 12);
  modular::mstatic::Factorial<TModularD> f;
  f.Adjust(K + 1);
  TModularD s = 0, h = TModularD(2).Inverse(), s0 = (K + 1) / 2, s1 = K / 2;
  for (unsigned j = 0; j < K; ++j) {
    auto m = (j > 1) ? (TModularD(j).PowU(N + 1) - 1) / TModularD(j - 1)
                     : TModularD(N * j + 1);
    s += m * (s1 - s0 + 1);
    TModularD ss = f.BinomialCoefficient(K + 1, j + 2),
              sd = s1 -
                   (((K - j) & 1) ? f.BinomialCoefficient(K + 1, j + 1) : 0);
    s0 = (ss - sd) * h;
    s1 = (ss + sd) * h;
  }
  cout << s - 1 << endl;
  return 0;
}
