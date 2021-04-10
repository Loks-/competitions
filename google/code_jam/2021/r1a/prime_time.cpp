#include "common/stl/base.h"
#include "common/stl/pair_io.h"
#include "common/vector/read.h"

#include <functional>

int main_prime_time() {
  unsigned T, M;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> M;
    auto v = nvector::Read<pair<uint64_t, uint64_t>>(M);
    uint64_t s = 0, best = 0, s0 = 0;
    for (auto p : v) s += p.first * p.second;
    for (uint64_t st = s; st; st /= v.back().first) s0 += v.back().first;
    uint64_t s1 = (s0 >= s) ? 1 : (s - s0);

    std::function<void(uint64_t, uint64_t, uint64_t)> Solve = [&](
        uint64_t cs, uint64_t cm, uint64_t ci) -> void {
      if (cm + cs == s) best = std::max(best, cm);
      if (ci >= M) return;
      uint64_t p = v[ci].first;
      if (cm > (s - cs - p) / p) return;
      if (s / cm == s1 / cm) return;
      for (unsigned j = 0; j <= v[ci].second; ++j) {
        Solve(cs, cm, ci + 1);
        if (cm > s / p) break;
        cs += p;
        cm *= p;
      }
    };

    Solve(0, 1, 0);
    cout << "Case #" << it << ": " << best << endl;
  }
  return 0;
}
