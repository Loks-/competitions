#include "common/stl/base.h"

#include <string>

int main_008() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    string s;
    cin >> N >> K >> s;
    s = string(K, '0') + s;
    unsigned c = 1, c0 = K, b = 0;
    for (unsigned i = 0; i < N; ++i) {
      unsigned d0 = s[i] - '0', d1 = s[i + K] - '0';
      if (d0 == 0)
        --c0;
      else
        c /= d0;
      if (d1 == 0)
        ++c0;
      else
        c *= d1;
      b = max(b, c0 ? 0 : c);
    }
    cout << b << endl;
  }
  return 0;
}
