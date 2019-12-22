#include "common/stl/base.h"

// Time: O(T)
// Memory: O(1)
int main_006() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    cout << ((n - 1) * n * (n + 1) * (3 * n + 2)) / 12 << endl;
  }
  return 0;
}
