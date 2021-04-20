#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/half_splitting_line_dl.h"
#include "common/hash.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"
#include "common/vector/read.h"

#include <cmath>
#include <functional>

int main_juggle_struggle_part1__bisection() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    std::vector<D2Point> vp = nvector::Read<D2Point>(2 * N), vpt;
    vector<unsigned> vl(2 * N), vm(2 * N);
    vector<vector<unsigned>> vvt(4);

    function<void(unsigned, unsigned, const D2Vector&, const D2Vector&)> Solve =
        [&](unsigned b, unsigned s, const D2Vector& v1,
            const D2Vector& v2) -> void {
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
      D2Vector v3 = v1 + v2;
      v3.Normalize();
      auto l = HalfSplittingLineDL(vpt, v3);
      for (auto& vt : vvt) vt.clear();
      for (unsigned i = b; i < e2; ++i)
        vvt[(i < e1 ? 0 : 2) + (l(vp[vl[i]]) > 0 ? 0 : 1)].push_back(vl[i]);
      vvt[1].swap(vvt[3]);
      assert(vvt[0].size() == vvt[1].size());
      assert(vvt[2].size() == vvt[3].size());
      unsigned k = b;
      std::vector<unsigned> vk(5, b);
      for (unsigned j = 0; j < 4; ++j) {
        vk[j + 1] = vk[j] + vvt[j].size();
        for (unsigned ij : vvt[j]) vl[k++] = ij;
      }
      Solve(vk[0], vk[1] - vk[0], v3, v2);
      Solve(vk[2], vk[3] - vk[2], v1, v3);
    };

    auto v1 = D2Vector(pow(10.0, 9), 0.25);
    v1.Normalize();
    auto v2 = D2Vector(-v1.dy, v1.dx);
    auto l1 = HalfSplittingLineDL(vp, v1), l2 = HalfSplittingLineDL(vp, v2);
    for (unsigned i = 0; i < 2 * N; ++i)
      vvt[(l1(vp[i]) > 0 ? 0 : 2) + (l2(vp[i]) > 0 ? 0 : 1)].push_back(i);
    vvt[1].swap(vvt[3]);
    assert(vvt[0].size() == vvt[1].size());
    assert(vvt[2].size() == vvt[3].size());
    unsigned k = 0;
    std::vector<unsigned> vk(5, 0);
    for (unsigned j = 0; j < 4; ++j) {
      vk[j + 1] = vk[j] + vvt[j].size();
      for (unsigned ij : vvt[j]) vl[k++] = ij;
    }
    Solve(vk[0], vk[1] - vk[0], v1, -v2);
    Solve(vk[2], vk[3] - vk[2], v2, v1);

    cout << "Case #" << it << ":";
    for (auto m : vm) cout << " " << m + 1;
    cout << endl;
  }
  return 0;
}
