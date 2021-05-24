#include "common/geometry/d2/distance/distance_l1.h"
#include "common/geometry/d2/point.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/algorithm/assignment.h"
#include "common/linear_algebra/matrix.h"

#include <string>

int main_retiling() {
  unsigned T, R, C, F, S;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C >> F >> S;
    auto vs = nvector::Read<string>(R);
    auto ve = nvector::Read<string>(R);
    std::vector<int> vr;
    for (unsigned ic = 0; ic < 2; ++ic) {
      char c = (ic == 0) ? 'M' : 'G';
      vector<I2Point> vps, vpe;
      for (unsigned i = 0; i < R; ++i) {
        for (unsigned j = 0; j < C; ++j) {
          if (vs[i][j] == c) vps.push_back({i, j});
          if (ve[i][j] == c) vpe.push_back({i, j});
        }
      }
      if (vps.size() > vpe.size()) vps.swap(vpe);
      unsigned n = vps.size(), m = vpe.size();
      la::Matrix<int64_t> a(n, m);
      for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < m; ++j) 
          a(i, j) = std::min<int64_t>(2 * F, S * DistanceL1(vps[i], vpe[j]));
      }
      auto r = alg::Assignment(a, 1 << 30);
      // cerr << "A size = " << vps.size() << " " << vpe.size() << " " << r  << endl;
      vr.push_back(r + F * (m - n));
    }
    cout << "Case #" << it << ": " << min(vr[0], vr[1]) << endl;
  }
  return 0;
}
