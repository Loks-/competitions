// https://www.hackerrank.com/challenges/minimum-mst-graph

#include "common/stl/base.h"

int main_minimum_mst_graph() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n, m, s, m0;
    cin >> n >> m >> s;
    m0 = ((n - 1) * (n - 2)) / 2 + 1;
    uint64_t k, kt;
    if (m <= m0) {
      k = s + (m - n + 1);
    } else {
      uint64_t ec = s - n + 2;
      k = s + m0 - (n - 1) + ec * (m - m0);
      uint64_t c0 = s / (n - 1), l1 = s % (n - 1), l0 = n - l1,
               ll = (l0 * (l0 - 1)) / 2;
      ll = min(ll, m);
      kt = c0 * ll + (c0 + 1) * (m - ll);
      k = min(k, kt);
      ll = m0 - 1;
      kt = c0 * ll + (s - (n - 2) * c0) * (m - ll);
      k = min(k, kt);
    }
    cout << k << endl;
  }
  return 0;
}
