#include "common/factorization/primes_generator.h"
#include "common/stl/base.h"

#include <functional>

int main_694() {
  uint64_t N = 1000000, M = 1000000000000000000ull;
  auto vp = GeneratePrimes(N);
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

  Add(M, 0);
  cout << s << endl;
  return 0;
}
