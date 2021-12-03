#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2001b() {
  unsigned n, m = 2020;
  cin >> n;
  auto v = nvector::Read<unsigned>(n);
  vector<unsigned> v2(m + 1, 0);
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = i + 1; j < n; ++j) {
      auto s = v[i] + v[j];
      if (s <= m) v2[s] = v[i] * v[j];
    }
  }
  for (auto x : v) {
    if (x >= m) continue;
    if (v2[m - x] > 0) {
      cout << x * v2[m - x] << endl;
      break;
    }
  }
  return 0;
}
