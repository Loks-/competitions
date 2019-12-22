#include "common/factorization/primes_generator.h"
#include "common/stl/base.h"

#include <functional>

int main_634__slow() {
  uint64_t N = 3000000000, M = 9000000000000000000ull;
  auto vp = GeneratePrimes(N);
  cout << "Primes created. Size = " << vp.size() << endl;
  uint64_t s = 0;

  std::function<void(uint64_t, unsigned, unsigned)> Add = [&](
      uint64_t k, unsigned i, unsigned mask) -> void {
    if (k == 0) return;
    if (mask == 0) ++s;
    for (; i < vp.size(); ++i) {
      uint64_t p = vp[i], kp = k / (p * p);
      if (kp == 0) break;
      Add(kp, i + 1, mask & 2);
      kp /= p;
      Add(kp, i + 1, mask & 1);
      kp /= p;
      if (kp == 0) {
        if ((mask & 1) == 0) {
          auto it = lower_bound(
              vp.begin() + i + 1, vp.end(), k,
              [](uint64_t _p, uint64_t _k) { return _p * _p <= _k / _p; });
          s += (it - vp.begin()) - i - 1;
        }
        if ((mask & 2) == 0) {
          auto it = lower_bound(
              vp.begin() + i + 1, vp.end(), k,
              [](uint64_t _p, uint64_t _k) { return _p * _p <= _k; });
          s += (it - vp.begin()) - i - 1;
        }
        break;
      }
      Add(kp, i + 1, mask & 2);
      kp /= p;
      Add(kp, i + 1, 0);
      kp /= p;
      Add(kp, i + 1, (mask == 3) ? 4 : 0);
      for (kp /= p; kp; kp /= p) Add(kp, i + 1, 0);
    }
  };

  Add(M, 0, 3);
  cout << s << endl;
  return 0;
}
