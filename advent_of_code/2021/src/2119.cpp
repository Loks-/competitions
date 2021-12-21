#include "common/geometry/d3/base.h"
#include "common/geometry/d3/distance/distance_l0.h"
#include "common/geometry/d3/distance/distance_l1.h"
#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/intersection.h"
#include "common/vector/read_lines.h"

int main_2119() {
  auto vs = nvector::ReadLines();
  vector<vector<I3Point>> vvp;
  for (auto& s : vs) {
    if (s.empty()) continue;
    if (s[1] == '-') {
      vvp.push_back({});
      continue;
    }
    auto ss = Split(s, ',');
    vvp.back().push_back({stoi(ss[0]), stoi(ss[1]), stoi(ss[2])});
  }

  vector<vector<vector<I3Point>>> vsrp(vvp.size());
  for (unsigned is = 0; is < vvp.size(); ++is) {
    vsrp[is].resize(24);
    vector<unsigned> vp{0, 1, 2};
    for (unsigned ip = 0; ip < 6; ++ip) {
      for (unsigned bm = 0; bm < 4; ++bm) {
        unsigned index = ip * 4 + bm;
        auto& vip = vsrp[is][index];
        for (auto& p : vvp[is]) {
          I3Point pnew;
          for (unsigned d = 0; d < 3; ++d)
            pnew[d] = p[vp[d]] * (1 - int(2 * ((bm >> d) & 1)));
          vip.push_back(pnew);
        }
        sort(vip.begin(), vip.end());
      }
      next_permutation(vp.begin(), vp.end());
    }
  }
  vector<vector<vector<I3Vector>>> vsrv(vsrp.size());
  for (unsigned i = 0; i < vsrp.size(); ++i) {
    vsrv[i].resize(vsrp[i].size());
    for (unsigned j = 0; j < vsrp[i].size(); ++j) {
      auto& vp = vsrp[i][j];
      auto& vv = vsrv[i][j];
      for (auto& p1 : vp) {
        for (auto& p2 : vp) {
          if (p1 == p2) continue;
          vv.push_back(p1 - p2);
        }
      }
      sort(vv.begin(), vv.end());
    }
  }

  vector<I3Point> vfp = vsrp[0][0];
  vector<I3Vector> vfv = vsrv[0][0];
  vector<unsigned> vused(vsrp.size(), 0);
  vused[0] = 1;
  vector<I3Point> vloc(1, {0, 0, 0});
  for (unsigned u = 1; u < vused.size(); ++u) {
    unsigned best_match = 0, best_index1 = 0, best_index2 = 0, best_is = 0;
    for (unsigned i1 = 0; i1 < vused.size(); ++i1) {
      if (vused[i1]) continue;
      for (unsigned i2 = 0; i2 < vsrv[i1].size(); ++i2) {
        auto c = nvector::Intersection(vfv, vsrv[i1][i2]);
        if (best_match < c) {
          best_match = c;
          best_index1 = i1;
          best_index2 = i2;
        }
      }
    }
    best_match = 0;
    I3Vector best_shift;
    for (unsigned is = 0; is < 2; ++is) {
      auto vp = vsrp[best_index1][best_index2];
      if (is == 1) {
        for (auto& p : vp) p = -p;
        reverse(vp.begin(), vp.end());
      }
      for (auto p1 : vp) {
        for (auto p2 : vfp) {
          auto shift = p2 - p1;
          vector<I3Point> vtemp;
          for (auto p : vp) vtemp.push_back(p + shift);
          auto c = nvector::Intersection(vfp, vtemp);
          if (best_match < c) {
            best_match = c;
            best_is = is;
            best_shift = shift;
          }
        }
      }
    }
    vloc.push_back(best_shift.ToPoint());
    auto vp = vsrp[best_index1][best_index2];
    if (best_is == 1) {
      for (auto& p : vp) p = -p;
    }
    for (auto& p : vp) p = p + best_shift;
    vector<I3Point> new_points;
    for (auto& p : vp) {
      if (!binary_search(vfp.begin(), vfp.end(), p)) new_points.push_back(p);
    }
    for (auto p1 : new_points) {
      for (auto p2 : vfp) {
        if (DistanceL0(p1, p2) > 2000) continue;
        vfv.push_back(p1 - p2);
        vfv.push_back(p2 - p1);
      }
      vfp.push_back(p1);
    }
    sort(vfp.begin(), vfp.end());
    sort(vfv.begin(), vfv.end());
    vused[best_index1] = 1;
  }
  cout << vfp.size() << endl;
  int64_t max_dist = 0;
  for (auto p1 : vloc) {
    for (auto p2 : vloc) max_dist = max(max_dist, DistanceL1(p1, p2));
  }
  cout << max_dist << endl;
  return 0;
}
