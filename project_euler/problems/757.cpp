#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"
#include "common/vector/unique.h"

// Time: O(N^(1/2) log(N))
// Memory: O(R)
int main_757() {
  uint64_t N = PowU<uint64_t>(10, 14), M = USqrt(N);
  vector<uint64_t> s;
  for (uint64_t e = 1; e * (e + 1) <= M; ++e) {
    for (uint64_t f = e;; f += 1) {
      uint64_t n = e * f * (e + 1) * (f + 1);
      if (n > N) break;
      s.push_back(n);
    }
  }
  nvector::UniqueS(s);
  cout << s.size() << endl;
  return 0;
}
