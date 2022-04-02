#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

int main_twisty_little_passages() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N, K, R0, R1, P0, P1;
    double hn = 0, hd = 0;
    cin >> N >> K;
    auto th = USqrt(N) + 1;
    for (unsigned i = 0; i < K; ++i) {
      if (i & 1) {
        cout << "W" << endl;
        cin >> R1 >> P1;
        if (P1 >= th) {
          hn += P0;
          hd += double(P0) / P1;
        }
      } else {
        if (i != 0) cout << "T " << (R0 % N) + 1 << endl;
        cin >> R0 >> P0;
        if (P0 < th) {
          hn += P0;
          hd += 1;
        }
      }
    }
    cout << "E " << uint64_t(N * hn / hd / 2. + 0.5) << endl;
  }
  return 0;
}
