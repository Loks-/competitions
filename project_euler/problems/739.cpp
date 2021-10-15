#include "common/modular/static/catalan.h"
#include "common/modular/static/fibonacci.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(N)
// Memory: O(N)
int main_739() {
  // unsigned N = 20;
  uint64_t N = PowU(10ull, 8);
  modular::mstatic::Catalan<TModularD> c;
  modular::mstatic::Fibonacci<TModularD> f(1, 3);
  TModularD s;
  f.Adjust(N);
  for (unsigned k = 1; k < N; ++k)
    s += f.Get(k) * c.GetTriangle(N - 2, N - k - 1);
  cout << s << endl;
  return 0;
}
