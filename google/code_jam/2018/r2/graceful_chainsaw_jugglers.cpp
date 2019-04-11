#include "common/stl/base.h"

int main_graceful_chainsaw_jugglers() {
  unsigned maxc = 501;
  unsigned l = 34;
  vector<vector<unsigned>> vc(maxc, vector<unsigned>(maxc, 0)), vn = vc;
  for (unsigned i = 0; i < l; ++i) {
    for (unsigned j = 0; j < l; ++j) {
      if ((i == 0) && (j == 0)) continue;
      if ((i * (i + 1) * (j + 1) >= 2 * maxc) ||
          ((i + 1) * j * (j + 1) >= 2 * maxc))
        continue;
      for (unsigned ic = 0; ic < maxc; ++ic) {
        for (unsigned jc = 0; jc < maxc; ++jc) {
          vn[ic][jc] = ((i <= ic) && (j <= jc))
                           ? max(vc[ic][jc], 1 + vc[ic - i][jc - j])
                           : vc[ic][jc];
        }
      }
      vn.swap(vc);
    }
  }
  for (unsigned ic = 0; ic < maxc; ++ic) {
    for (unsigned jc = 0; jc < maxc; ++jc) {
      if (ic) vc[ic][jc] = max(vc[ic][jc], vc[ic - 1][jc]);
      if (jc) vc[ic][jc] = max(vc[ic][jc], vc[ic][jc - 1]);
    }
  }
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned r, b;
    cin >> r >> b;
    cout << "Case #" << it << ": " << vc[r][b] << endl;
  }
  return 0;
}
