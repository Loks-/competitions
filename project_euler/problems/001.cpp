#include "common/stl/base.h"

int main_001() {
  vector<int64_t> vk{3, 5, 15}, vw{1, 1, -1};
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int64_t n, s = 0;
    cin >> n;
    n -= 1;
    for (unsigned i = 0; i < 3; ++i) {
      int64_t nk = n / vk[i];
      s += (nk * (nk + 1)) / 2 * vk[i] * vw[i];
    }
    cout << s << endl;
  }
  return 0;
}
