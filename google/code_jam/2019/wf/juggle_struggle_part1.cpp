#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/iangle.h"
#include "common/geometry/d2/point_io.h"
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
    auto vp = nvector::Read<I2Point>(2 * N);
    vector<unsigned> vl = nvector::Enumerate<unsigned>(0, 2 * N), vm(2 * N);
    vector<pair<I2Angle, unsigned>> vpai;
    vector<vector<unsigned>> vvt(4);

    function<void(unsigned, unsigned, unsigned)> Solve = [&](
        unsigned b1, unsigned e1, unsigned e2) -> void {
      if (e2 == b1) return;
      // cout << "Solve(" << b1 << ", " << e1 << ", " << e2 << ")" << endl;
      // for (unsigned i = b1; i < e2; ++i)
      //   cout << " " << vl[i];
      // cout << endl;
      unsigned bi = b1;
      for (unsigned i = b1 + 1; i < e2; ++i) {
        if (CompareXY(vp[vl[i]], vp[vl[bi]])) bi = i;
      }
      unsigned fi = vl[bi];
      vpai.clear();
      for (unsigned i = b1; i < e2; ++i) {
        if (i == bi) continue;
        vpai.push_back({I2Angle(vp[vl[i]] - vp[fi]), i});
      }
      unsigned mi = vpai.size() / 2;
      nth_element(vpai.begin(), vpai.begin() + mi, vpai.end());
      // for (auto p : vpai)
      //   cout << " " << vl[p.second];
      // cout << endl;
      unsigned lim = vl[vpai[mi].second];
      vm[fi] = lim;
      vm[lim] = fi;
      for (auto& vt : vvt) vt.clear();
      for (unsigned j = 0; j < mi; ++j) {
        unsigned ij1 = vpai[j].second;
        vvt[ij1 < e1 ? 0 : 2].push_back(vl[ij1]);
        unsigned ij2 = vpai[j + mi + 1].second;
        vvt[ij2 < e1 ? 1 : 3].push_back(vl[ij2]);
      }
      if (e1 != 2 * N) vvt[1].swap(vvt[3]);
      unsigned k = b1;
      for (unsigned j = 0; j < 4; ++j) {
        // cout << "vvt[j].size() = " << vvt[j].size() << endl;
        for (unsigned ij : vvt[j]) vl[k++] = ij;
      }
      unsigned p1 = b1 + vvt[0].size(), p2 = p1 + vvt[1].size(),
               p3 = p2 + vvt[2].size();
      Solve(b1, p1, p2);
      Solve(p2, p3, k);
    };

    Solve(0, 2 * N, 2 * N);

    cout << "Case #" << it << ":";
    for (auto m : vm) cout << " " << m + 1;
    cout << endl;
  }
  return 0;
}
