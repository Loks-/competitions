// https://www.hackerrank.com/challenges/grid-lines

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_grid_lines() {
  TFactorial f;

  auto G = [&](uint64_t n, uint64_t m, uint64_t k, uint64_t dx, uint64_t dy,
               uint64_t l) {
    return (f.BinomialCoefficient(l, k - 1) * n * m -
            f.BinomialCoefficient(l + 1, k) * (n * dy + m * dx + dx * dy) *
                (k - 1) +
            f.BinomialCoefficient(l + 2, k + 1) * dx * dy * k * (k - 1)) *
           2;
  };

  auto F = [&](uint64_t n, uint64_t m, uint64_t k) {
    TModular s =
        f.BinomialCoefficient(m, k) * n + f.BinomialCoefficient(n, k) * m;
    for (uint64_t dx = 1; dx * (k - 1) < n; ++dx) {
      for (uint64_t dy = 1; dy * (k - 1) < m; ++dy) {
        if (GCD(dx, dy) != 1) continue;
        s += G(n, m, k, dx, dy, min((n - 1) / dx, (m - 1) / dy)) -
             G(n, m, k, dx, dy, k - 2);
      }
    }
    return s;
  };

  uint64_t n, m, k;
  cin >> n >> m >> k;
  cout << F(n + 1, m + 1, k) - F(n - 1, m - 1, k) << endl;
  return 0;
}
