#include "common/factorization/primes_generator.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/stl/base.h"

#include <functional>

// Time: O(N^(1/3))
// Memory: O(N^(1/3))
int main_694() {
  uint64_t N = PowU<uint64_t>(10, 18), M = UCbrt(N);
  auto vp = GeneratePrimes(M);
  uint64_t s = 0;

  std::function<void(uint64_t, unsigned)> Add = [&](uint64_t k,
                                                    unsigned i) -> void {
    s += k;
    for (; i < vp.size(); ++i) {
      uint64_t p = vp[i], kp = k / (p * p * p);
      if (kp == 0) break;
      for (; kp; kp /= p) Add(kp, i + 1);
    }
  };

  Add(N, 0);
  cout << s << endl;
  return 0;
}
