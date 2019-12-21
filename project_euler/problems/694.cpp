#include "common/factorization/primes_list.h"
#include "common/stl/base.h"

#include <functional>

int main_694() {
  uint64_t N = 1000000, M = 1000000000000000000ull;
  factorization::PrimesList primes_list(N);
  uint64_t s = M;

  std::function<void(uint64_t k, const TFactorization& f, unsigned fi)> Add =
      [&](uint64_t k, const TFactorization& f, unsigned fi) -> void {
    if (fi == f.size()) {
      s += M / k;
    } else {
      for (; k <= M; k *= f[fi].prime) {
        Add(k, f, fi + 1);
        if (k > M / f[fi].prime) break;  // Avoid overflow
      }
    }
  };

  for (uint64_t i = 2; i <= N; ++i) {
    auto f = primes_list.FactorizeTable(i);
    bool has_power_2plus = false;
    for (auto p : f) {
      if (p.power > 1) has_power_2plus = true;
    }
    if (has_power_2plus) continue;
    Add(i * i * i, f, 0);
  }
  cout << s << endl;
  return 0;
}
