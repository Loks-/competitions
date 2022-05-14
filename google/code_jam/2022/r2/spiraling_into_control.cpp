#include "common/stl/base.h"

int main_spiraling_into_control() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    auto Get = [&](unsigned l, unsigned p) {
      return 4 * l * (N - 1) - 4 * l * (l - 1) + p * (N - 1 - 2 * l) + 1;
    };

    cout << "Case #" << it << ": ";
    if ((K < N - 1) || (K & 1)) {
      cout << "IMPOSSIBLE" << endl;
    } else {
      vector<pair<unsigned, unsigned>> vp;
      unsigned p = 0;
      for (unsigned l = 0; l < N / 2; ++l) {
        for (; p < 4; ++p) {
          auto v0 = Get(l, p), v1 = Get(l + 1, p);
          if (K - 2 <= N * N - v1) {
            if (v1 > v0 + 2) vp.push_back({v0 + 1, v1});
            K -= 2;
            break;
          } else {
            K -= (N - 2 * l - 1);
          }
        }
        if (p == 4) p = 0;
      }
      cout << vp.size() << endl;
      for (auto& p : vp) cout << p.first << " " << p.second << endl;
    }
  }
  return 0;
}
