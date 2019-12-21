#include "common/factorization/primes_generator.h"
#include "common/stl/base.h"

#include <functional>

int main_694() {
  uint64_t N = 2000000, M = 1000000000000000000ull;
  auto vp = GeneratePrimes(N);
  uint64_t s = 0;

  std::function<void(uint64_t, unsigned)> Add = [&](uint64_t k,
                                                    unsigned i) -> void {
    uint64_t p = vp[i], l = p * p * p;
    if (k > M / l) {
      s += M / k;
    } else {
      Add(k, i + 1);
      for (; k <= M / l; k *= p) Add(k * l, i + 1);
    }
  };

  Add(1, 0);
  cout << s << endl;
  return 0;
}
