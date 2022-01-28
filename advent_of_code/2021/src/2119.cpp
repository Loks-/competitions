#include "common/geometry/d3/affine_transformation.h"
#include "common/geometry/d3/base.h"
#include "common/geometry/d3/distance/distance_l1.h"
#include "common/geometry/d3/distance/distance_l2.h"
#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/intersection.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

#include <queue>

int main_2119() {
  const unsigned min_points = 12,
                 min_pairs = (min_points * (min_points - 1)) / 2;
  auto vs = nvector::ReadLines();
  vector<vector<I3Point>> vvp;
  for (auto& s : vs) {
    if (s.empty()) continue;
    if (s[1] == '-') {
      vvp.push_back({});
    } else {
      auto ss = Split(s, ',');
      vvp.back().push_back({stoi(ss[0]), stoi(ss[1]), stoi(ss[2])});
    }
  }

  vector<vector<int64_t>> vsh(vvp.size());
  vector<vector<vector<int64_t>>> vph(vvp.size());
  for (unsigned i = 0; i < vvp.size(); ++i) {
    auto& vp = vvp[i];
    auto& sh = vsh[i];
    auto& ph = vph[i];
    ph.resize(vp.size());
    for (unsigned j1 = 0; j1 < vp.size(); ++j1) {
      for (unsigned j2 = j1 + 1; j2 < vp.size(); ++j2) {
        auto h = SquaredDistanceL2(vp[j1], vp[j2]);
        sh.push_back(h);
        ph[j1].push_back(h);
        ph[j2].push_back(h);
      }
    }
    sort(sh.begin(), sh.end());
    for (auto& vh : ph) sort(vh.begin(), vh.end());
  }

  vector<unsigned> vfound(vvp.size());
  vfound[0] = 1;
  vector<I3AffineTransformation> vat(vvp.size());
  queue<unsigned> q;
  for (q.push(0); !q.empty(); q.pop()) {
    auto i1 = q.front();
    for (unsigned i2 = 0; i2 < vsh.size(); ++i2) {
      if (vfound[i2]) continue;
      if (nvector::Intersection(vsh[i1], vsh[i2]) >= min_pairs) {
        vfound[i2] = 1;
        q.push(i2);
        auto &ph1 = vph[i1], &ph2 = vph[i2];
        vector<pair<unsigned, unsigned>> vp;
        for (unsigned j1 = 0; j1 < ph1.size(); ++j1) {
          for (unsigned j2 = 0; j2 < ph2.size(); ++j2) {
            if (nvector::Intersection(ph1[j1], ph2[j2]) >= min_points - 1) {
              vp.push_back({j1, j2});
              break;
            }
          }
        }
        for (unsigned j1 = 0; j1 < vp.size(); ++j1) {
          for (unsigned j2 = j1 + 1; j2 < vp.size(); ++j2) {
            unsigned k1 = vp[j1].first, k2 = vp[j2].first;
            auto d1 = vvp[i1][k1] - vvp[i1][k2];
            vector<int64_t> vd(0);
            for (unsigned l = 0; l < 3; ++l) vd.push_back(Abs(d1[l]));
            sort(vd.begin(), vd.end());
            if ((vd[0] == 0) || (vd[1] == vd[0]) || (vd[2] == vd[1])) continue;
            // restore transformation
            unsigned l1 = vp[j1].second, l2 = vp[j2].second;
            auto d2 = vvp[i2][l1] - vvp[i2][l2];
            I3AffineTransformation at;
            for (unsigned m1 = 0; m1 < 3; ++m1) {
              for (unsigned m2 = 0; m2 < 3; ++m2) {
                auto x1 = d1[m1], x2 = d2[m2];
                at.Get(m1, m2) = (x1 == x2) ? 1 : (x1 == -x2) ? -1 : 0;
              }
            }
            auto d3 = vvp[i1][k1] - at(vvp[i2][l1]);
            for (unsigned m = 0; m < 3; ++m) at.Get(m, 3) = d3[m];
            vat[i2] = vat[i1] * at;
            j1 = j2 = vp.size();  // break
          }
        }
      }
    }
  }

  vector<I3Point> vfp, vfs;
  for (unsigned i = 0; i < vvp.size(); ++i) {
    auto& at = vat[i];
    for (auto& p : vvp[i]) vfp.push_back(at(p));
    vfs.push_back({at.Get(0, 3), at.Get(1, 3), at.Get(2, 3)});
  }
  nvector::UniqueUnsorted(vfp);
  cout << vfp.size() << endl;

  int64_t max_dist = 0;
  for (auto& p1 : vfs) {
    for (auto& p2 : vfs) max_dist = max(max_dist, DistanceL1(p1, p2));
  }
  cout << max_dist << endl;
  return 0;
}
