#include "common/factorization/table/pi.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(N log(N))
// Memory: O(N)
int main_609() {
  uint64_t N = PowU<uint64_t>(10, 8);
  factorization::table::Pi pi(N);
  std::vector<uint64_t> vs(N + 1, 0);
  for (unsigned i = 2; i <= N; ++i) {
    unsigned c = (pi.IsPrime(i) ? 0 : 1), j = pi.Get(i);
    for (; j > 0; j = pi.Get(j)) {
      c += (pi.IsPrime(j) ? 0 : 1);
      ++vs[c];
    }
  }
  TModularD r = 1;
  for (uint64_t v : vs) {
    if (v) r *= v;
  }
  cout << r << endl;
  return 0;
}
