#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/long.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/approximate_bisector.h"
#include "common/geometry/d2/utils/half_splitting_line_dl.h"
#include "common/geometry/d2/utils/scale_points.h"
#include "common/numeric/long/signed_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <functional>

using TLine = geometry::d2::LinePV<LongSigned>;

int main_juggle_struggle_part1__bisection() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    std::vector<L2Point> vp = nvector::Read<L2Point>(2 * N), vpt;
    ScaleXY(vp, 2);
    vector<unsigned> vl(2 * N), vm(2 * N);
    vector<vector<unsigned>> vvt(4);

    function<void(unsigned, unsigned, const L2Vector&, const L2Vector&)> Solve =
        [&](unsigned b, unsigned s, const L2Vector& v1,
            const L2Vector& v2) -> void {
      if (s == 0) return;
      if (s == 1) {
        unsigned p0 = vl[b], p1 = vl[b + 1];
        vm[p0] = p1;
        vm[p1] = p0;
        return;
      }
      unsigned e1 = b + s, e2 = b + 2 * s;
      vpt.clear();
      for (unsigned i = b; i < e2; ++i) vpt.push_back(vp[vl[i]]);
      L2Vector v3 = ApproximateBisector(v1, v2);
      auto l3 = HalfSplittingLineDL(vpt, v3);
      for (auto& vt : vvt) vt.clear();
      for (unsigned i = b; i < e2; ++i) {
        vvt[(i < e1 ? 0 : 1) + (l3(vp[vl[i]]).Negative() ? 0 : 2)].push_back(
            vl[i]);
      }
      vvt[1].swap(vvt[3]);
      unsigned k = b;
      std::vector<unsigned> vk(5, b);
      for (unsigned j = 0; j < 4; ++j) {
        vk[j + 1] = vk[j] + vvt[j].size();
        for (unsigned ij : vvt[j]) vl[k++] = ij;
      }
      Solve(vk[0], vk[1] - vk[0], v1, v3);
      Solve(vk[2], vk[3] - vk[2], v3, v2);
    };

    auto v1 = L2Vector(LongSigned(PowU<uint64_t>(10, 10)), LongSigned(1)),
         v2 = v1.RotateHalfPi();
    auto l1 = HalfSplittingLineDL(vp, v1), l2 = HalfSplittingLineDL(vp, v2);
    for (unsigned i = 0; i < 2 * N; ++i)
      vvt[(l1(vp[i]) > 0 ? 0 : 1) + (l2(vp[i]) < 0 ? 0 : 2)].push_back(i);
    vvt[1].swap(vvt[3]);
    unsigned k = 0;
    std::vector<unsigned> vk(5, 0);
    for (unsigned j = 0; j < 4; ++j) {
      vk[j + 1] = vk[j] + vvt[j].size();
      for (unsigned ij : vvt[j]) vl[k++] = ij;
    }
    Solve(vk[0], vk[1] - vk[0], v1, v2);
    Solve(vk[2], vk[3] - vk[2], v2, -v1);

    cout << "Case #" << it << ":";
    for (auto m : vm) cout << " " << m + 1;
    cout << endl;
  }
  return 0;
}
