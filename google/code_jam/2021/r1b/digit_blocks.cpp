#include "common/stl/base.h"

int main_digit_blocks() {
  unsigned T, N, B, d;
  uint64_t P;
  cin >> T >> N >> B >> P;

  vector<vector<vector<double>>> v(
      N * B + 1, vector<vector<double>>(N + 1, vector<double>(N + 1, 0.)));
  // i - left elements
  for (unsigned i = 1; i <= N * B; ++i) {
    // j - columns height B - 1
    for (unsigned j = 0, je = min(i, N); j <= je; ++j) {
      unsigned ia = i - j, kb = (ia + B - 1) / B;
      // k - columns height less than B - 1
      for (unsigned k = kb, ke = min(ia / 2, N - j); k <= ke; ++k) {
        double s = 0;
        for (unsigned d = 0; d < 10; ++d) {
          double b = 0.;
          if (j > 0) b = max(b, d + v[i - 1][j - 1][k]);
          if ((k > 0) && (ia <= (k - 1) * B + 2))
            b = max(b, d / 10.0 + v[i - 1][j + 1][k - 1]);
          if (ia > 2 * k) b = max(b, v[i - 1][j][k]);
          s += b;
        }
        v[i][j][k] = s / 10;
      }
    }
  }

  for (unsigned it = 1; it <= T; ++it) {
    vector<unsigned> vb(N, 0);
    unsigned j = 0, k = N, p1 = 1;
    for (unsigned i = N * B; i > 0; --i) {
      unsigned ia = i - j, ia2 = B * k - ia;
      cin >> d;
      double s1 = (j > 0) ? d + v[i - 1][j - 1][k] : -1.;
      double s2 = ((k > 0) && (ia <= (k - 1) * B + 2))
                      ? d / 10.0 + v[i - 1][j + 1][k - 1]
                      : -1.;
      double s3 = (ia > 2 * k) ? v[i - 1][j][k] : -1.;
      if ((s1 > s2) && (s1 > s3)) {
        cout << p1 << endl;
        ++p1;
        --j;
      } else if (s2 > s3) {
        cout << p1 + j << endl;
        ++j;
        --k;
      } else {
        cout << p1 + j + ia2 / (B - 2) << endl;
      }
    }
  }
  return 0;
}
