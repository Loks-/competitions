#include "common/factorization/factorization.h"
#include "common/factorization/utils/divisors.h"
#include "common/stl/base.h"

int main_009() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, best = 0;
    cin >> N;
    if ((N % 2) == 0) {
      N /= 2;
      auto vdn = GetDivisors(Factorize(N));
      for (uint64_t k : vdn) {
        if (k == N) continue;
        uint64_t l = N / k;
        auto vdl = GetDivisors(Factorize(l));
        for (uint64_t a : vdl) {
          if (a * a >= l) continue;
          uint64_t b = l / a - a;
          if (b >= a) continue;
          uint64_t abc =
              k * k * k * (a * a - b * b) * 2 * a * b * (a * a + b * b);
          best = max(best, abc);
        }
      }
    }
    cout << (best ? int64_t(best) : int64_t(-1)) << endl;
  }
  return 0;
}
