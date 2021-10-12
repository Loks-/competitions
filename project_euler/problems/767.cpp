#include "common/modular/static/convolution.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(K * log(K))
// Memory: O(K)
int main_767() {
  // uint64_t H = 16, K = 3, N = 9;
  uint64_t H = 16, K = PowU(10ull, 5), N = PowU(10ull, 16);
  assert((N % K) == 0);
  TModularD m2 = 2, m2a = m2.PowU(N / K) - m2, s = 0;
  modular::mstatic::Factorial<TModularD> f;
  f.Adjust(K + 1);
  vector<TModularD> va(K + 1);
  for (unsigned l = 0; l <= K; ++l) va[l] = f.GetI(l).PowU(H);
  auto vb = modular::mstatic::Convolution<TModularD, (1u << 18)>(va);
  for (unsigned l = 0; l <= K; ++l)
    s += vb[l] * f.Get(l).PowU(H) * f.BinomialCoefficient(K, l) *
         m2a.PowU(K - l);
  cout << s << endl;
  return 0;
}
