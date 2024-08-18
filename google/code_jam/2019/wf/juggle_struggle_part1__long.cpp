#include "common/geometry/d2/angle_long.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/long.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/half_splitting_line_ab.h"
#include "common/hash/combine.h"
#include "common/numeric/long/signed_io.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"
#include "common/vector/read.h"

#include <functional>

using TLine = geometry::d2::LinePV<LongSigned>;

int main_juggle_struggle_part1__long() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    std::vector<L2Point> vp = nvector::Read<L2Point>(2 * N), vpt;
    vector<unsigned> vl = nvector::Enumerate<unsigned>(0, 2 * N), vm(2 * N);
    vector<vector<unsigned>> vvt(4);
    size_t h = 0;

    function<void(unsigned, unsigned)> Solve = [&](unsigned b,
                                                   unsigned s) -> void {
      if (s == 0) return;
      if (s == 1) {
        unsigned p0 = vl[b], p1 = vl[b + 1];
        vm[p0] = p1;
        vm[p1] = p0;
        return;
      }
      unsigned e1 = b + s, e2 = (s == 2 * N) ? e1 : e1 + s;
      vpt.clear();
      for (unsigned i = b; i < e2; ++i) vpt.push_back(vp[vl[i]]);
      nhash::DCombineH(h, s);
      unsigned p0 = h % s, p1 = vl[b + p0],
               p2 = vl[b + HalfSplittingLineAB<LongSigned, L2Angle>(vpt, p0)];
      vm[p1] = p2;
      vm[p2] = p1;
      TLine l(vp[p1], vp[p2]);
      for (auto& vt : vvt) vt.clear();
      for (unsigned i = b; i < e2; ++i) {
        if ((vl[i] == p1) || (vl[i] == p2)) continue;
        vvt[(i < e1 ? 0 : 2) + (l(vp[vl[i]]) > 0 ? 0 : 1)].push_back(vl[i]);
      }
      if (s != 2 * N) vvt[1].swap(vvt[3]);
      unsigned k = b, s1 = vvt[0].size(), s2 = vvt[2].size();
      for (unsigned j = 0; j < 4; ++j) {
        for (unsigned ij : vvt[j]) vl[k++] = ij;
      }
      Solve(b, s1);
      Solve(b + 2 * s1, s2);
    };

    Solve(0, 2 * N);

    cout << "Case #" << it << ":";
    for (auto m : vm) cout << " " << m + 1;
    cout << endl;
  }
  return 0;
}
