#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_3d_printing() {
  unsigned T, L = 1000000;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    auto v0 = nvector::Read<unsigned>(4);
    for (unsigned i = 0; i < 2; ++i) {
      auto vt = nvector::Read<unsigned>(4);
      for (unsigned j = 0; j < 4; ++j) v0[j] = min(v0[j], vt[j]);
    }
    auto s = nvector::Sum(v0);
    cout << "Case #" << it << ":";
    if (s < L) {
      cout << " IMPOSSIBLE";
    } else {
      auto l = L;
      for (auto x : v0) {
        x = min(x, l);
        cout << " " << x;
        l -= x;
      }
    }
    cout << endl;
  }
  return 0;
}
