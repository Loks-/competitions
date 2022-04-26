#include "common/stl/base.h"
#include "common/vector/read.h"

int main_pancake_deque() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n, m = 0, s = 0;
    cin >> n;
    auto v = nvector::Read<unsigned>(n);
    for (unsigned i = 0, j = n - 1; i <= j;) {
      auto sij = (v[i] <= v[j]) ? v[i++] : v[j--];
      s += (sij >= m) ? 1 : 0;
      m = max(m, sij);
    }
    cout << "Case #" << it << ": " << s << endl;
  }
  return 0;
}
