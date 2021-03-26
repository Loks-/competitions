#include "common/stl/base.h"
#include "common/vector/read.h"

int main_thermometers() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int64_t K;
    cin >> K >> N;
    auto vx = ReadVector<int64_t>(N), vt = ReadVector<int64_t>(N);
    for (unsigned i = 0; i < N; ++i) vx.push_back(vx[i] + K);
    vector<unsigned> vd(N, N);
    unsigned r = 2 * N;
    for (unsigned k = 0; k < N; ++k) {
      int64_t a = 1, b = 0, l = ((k == 0) ? vx[N - 1] - K : vx[k - 1]),
              h = vx[k];
      for (unsigned i = 0; i < N; ++i) {
        a = -a;
        b = 2 * vx[i + k] - b;
        if (a > 0) {
          l = max(l, vx[i + k] - b);
          h = min(h, vx[i + k + 1] - b);
        } else {
          l = max(l, -vx[i + k + 1] + b);
          h = min(h, -vx[i + k] + b);
        }
        if (l >= h) {
          vd[k] = i;
          break;
        }
      }
      if ((k == 0) && (vd[0] == N)) {
        if (((a == 1) && (b == K)) ||
            ((a == -1) && (2 * l < (b - K)) && ((b - K) < 2 * h)))
          r = N;
      }
    }
    for (unsigned k = 0; k < N; ++k) {
      unsigned i = 0, l = k;
      for (; l < k + N; ++i) l += vd[l % N];
      r = min(r, N + i);
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
