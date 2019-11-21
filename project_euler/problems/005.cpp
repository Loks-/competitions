#include "common/numeric/utils/lcm.h"
#include "common/stl/base.h"

int main_005() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n, m = 1;
    cin >> n;
    for (uint64_t i = 2; i <= n; ++i) m = LCM(m, i);
    cout << m << endl;
  }
  return 0;
}
