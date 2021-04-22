#include "common/geometry/d2/long.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/half_splitting_line_dl.h"
#include "common/geometry/d2/utils/scale.h"
#include "common/hash.h"
#include "common/numeric/long/signed_io.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"
#include "common/vector/read.h"

#include <functional>

int main_juggle_struggle_part1__rsplit_long() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    std::vector<L2Point> vp = nvector::Read<L2Point>(2 * N), vpt;
    ScaleXY(vp, 2);
    vector<unsigned> vl = nvector::Enumerate<unsigned>(0, 2 * N), vm(2 * N);
    vector<vector<unsigned>> vvt(5);
    size_t h = 0;

    function<void(unsigned, unsigned)> Solve = [&](unsigned b,
                                                   unsigned s) -> void {
      if (s == 0) return;
      unsigned e1 = b + s, e2 = (s == 2 * N) ? e1 : e1 + s;
      vpt.clear();
      for (unsigned i = b; i < e2; ++i) vpt.push_back(vp[vl[i]]);
      h = HashCombine(h, s);
      unsigned p1 = b + h % s, p2 = (e1 == e2) ? b + (h + 1) % s : e1 + h % s;
      auto l = HalfSplittingLineDL(vpt, vp[vl[p2]] - vp[vl[p1]]);
      for (auto& vt : vvt) vt.clear();
      for (unsigned i = b; i < e2; ++i) {
        auto lp = l(vp[vl[i]]);
        vvt[(lp == 0) ? 4 : (i < e1 ? 0 : 2) + (lp.Negative() ? 0 : 1)]
            .push_back(vl[i]);
      }
      if (s != 2 * N) vvt[1].swap(vvt[3]);
      unsigned k = b, s1 = vvt[0].size(), s2 = vvt[2].size();
      for (unsigned j = 0; j < 4; ++j) {
        for (unsigned ij : vvt[j]) vl[k++] = ij;
      }
      if (!vvt[4].empty()) {
        vm[vvt[4][0]] = vvt[4][1];
        vm[vvt[4][1]] = vvt[4][0];
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
