#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(K^2 * log(H))
// Memory: O(K)
int main_767__slow() {
  // uint64_t H = 16, K = 3, N = 9;
  uint64_t H = 16, K = PowU(10ull, 5), N = PowU(10ull, 16);
  assert((N % K) == 0);
  TModularD m2 = 2, m2a = m2.PowU(N / K) - m2, s = 0;
  modular::mstatic::Factorial<TModularD> f;
  vector<TModularD> va(K + 1);
  for (unsigned l = 0; l <= K; ++l) {
    if ((l & (l - 1)) == 0) cout << "\tl = " << l << endl;
    for (unsigned i = 0; i <= l; ++i)
      va[l] += f.BinomialCoefficient(l, i).PowU(H);
    s += va[l] * f.BinomialCoefficient(K, l) * m2a.PowU(K - l);
  }
  cout << s << endl;
  return 0;
}
