#include "common/stl/base.h"
#include "common/vector/read.h"

int main_d1000000() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    auto v = nvector::Read<unsigned>(n);
    sort(v.begin(), v.end());
    unsigned k = 0;
    for (auto x : v) {
      if (x > k) ++k;
    }
    cout << "Case #" << it << ": " << k << endl;
  }
  return 0;
}
