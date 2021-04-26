// https://www.hackerrank.com/challenges/clique

#include "common/algorithm/binary_search.h"
#include "common/stl/base.h"

int main_clique() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n, m;
    cin >> n >> m;
    auto f = [&](unsigned k) {
      unsigned x0 = n / k, x1 = x0 + 1, l1 = n % k, l0 = k - l1;
      return n * (n - 1) - l0 * x0 * (x0 - 1) - l1 * x1 * (x1 - 1);
    };
    cout << LowerBoundF(1u, n + 1, 2 * m, f) << endl;
  }
  return 0;
}
