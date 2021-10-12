#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(K)
// Memory: O(K)
int main_743() {
  // uint64_t K = 4, N = 20;
  uint64_t K = PowU(10ull, 8), N = PowU(10ull, 16);
  assert((N % K) == 0);
  modular::mstatic::Factorial<TModularD> f;
  f.Adjust(K);
  TModularD s, m2n = TModularD(2).PowU(N / K), m = m2n.PowU(K),
               mm = m2n.PowS(-2);
  for (unsigned i = 0; i <= K / 2; ++i) {
    s += m * f.Get(K) * f.GetI(i) * f.GetI(i) * f.GetI(K - 2 * i);
    m *= mm;
  }
  cout << s << endl;
  return 0;
}
