#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2208() {
  auto vs = nvector::ReadLines();
  int n = vs.size(), m = vs[0].size();
  auto inb = [&](int x, int y) {
    return (0 <= x) && (x < n) && (0 <= y) && (y < m);
  };
  int r1 = 0, r2 = 0;
  vector<vector<int>> vd{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      auto c = vs[i][j];
      bool ok = false;
      vector<int> vl(4, 0);
      for (unsigned d = 0; d < 4; ++d) {
        for (int k = 1;; ++k) {
          auto ii = i + k * vd[d][0], jj = j + k * vd[d][1];
          if (!inb(ii, jj)) {
            ok = true;
            break;
          }
          ++vl[d];
          if (vs[ii][jj] >= c) break;
        }
      }
      if (!ok) continue;
      ++r1;
      r2 = max(r2, vl[0] * vl[1] * vl[2] * vl[3]);
    }
  }
  cout << r1 << endl << r2 << endl;
  return 0;
}
