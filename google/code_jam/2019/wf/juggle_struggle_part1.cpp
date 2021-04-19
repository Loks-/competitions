#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/half_splitting_line.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"
#include "common/vector/enumerate.h"
#include "common/vector/read.h"

#include <functional>

int main_juggle_struggle_part1() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    std::vector<I2Point> vp = nvector::Read<I2Point>(2 * N), vpt;
    vector<unsigned> vl = nvector::Enumerate<unsigned>(0, 2 * N), vm(2 * N);
    vector<pair<I2Angle, unsigned>> vpai;
    vector<vector<unsigned>> vvt(4);

    function<void(unsigned, unsigned, unsigned)> Solve = [&](
        unsigned b1, unsigned e1, unsigned e2) -> void {
      if (e2 == b1) return;
      vpt.clear();
      for (unsigned i = b1; i < e2; ++i) vpt.push_back(vp[vl[i]]);
      auto p = HalfSplittingLineAB(vpt);
      unsigned p1 = vl[b1 + p.first], p2 = vl[b1 + p.second];
      vm[p1] = p2;
      vm[p2] = p1;
      I2LinePV l(vp[p1], vp[p2]);
      for (auto& vt : vvt) vt.clear();
      for (unsigned i = b1; i < e2; ++i) {
        if ((vl[i] == p1) || (vl[i] == p2)) continue;
        vvt[(i < e1 ? 0 : 2) + (l(vp[vl[i]]) > 0 ? 0 : 1)].push_back(vl[i]);
      }
      if (e1 != 2 * N) vvt[1].swap(vvt[3]);
      unsigned k = b1;
      for (unsigned j = 0; j < 4; ++j) {
        for (unsigned ij : vvt[j]) vl[k++] = ij;
      }
      unsigned lp1 = b1 + vvt[0].size(), lp2 = lp1 + vvt[1].size(),
               lp3 = lp2 + vvt[2].size();
      Solve(b1, lp1, lp2);
      Solve(lp2, lp3, k);
    };

    Solve(0, 2 * N, 2 * N);

    cout << "Case #" << it << ":";
    for (auto m : vm) cout << " " << m + 1;
    cout << endl;
  }
  return 0;
}
