#include "common/stl/base.h"

int main_digit_blocks() {
  unsigned T, N, B, d;
  uint64_t P;
  cin >> T >> N >> B >> P;
  for (unsigned it = 1; it <= T; ++it) {
    vector<unsigned> vb(N, 0);
    unsigned k1 = 0, k2 = 0;
    for (unsigned i = 0; i < N * B; ++i) {
      cin >> d;
      if (((d == 9) && (k1 < k2)) || (k2 == N)) {
        cout << k1 + 1 << endl;
        ++vb[k1++];
      } else {
        cout << k2 + 1 << endl;
        if (++vb[k2] == B - 1) ++k2;
      }
    }
  }
  return 0;
}
