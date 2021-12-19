#include "common/geometry/d3/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector_io.h"
#include "common/numeric/utils/abs.h"
#include "common/stl/full.h"
#include "common/stl/pair_io.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"
#include "common/vector/read_all.h"
#include "common/vector/read_from_line.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"
#include "common/vector/write.h"

int main_2119() {
  auto vs = nvector::ReadLines();
  vector<vector<vector<int64_t>>> vvv;
  for (auto& s : vs) {
    if (s.empty()) continue;
    if (s[1] == '-') {
      vvv.push_back({});
      continue;
    }
    auto ss = Split(s, ',');
    vvv.back().push_back({stoi(ss[0]), stoi(ss[1]), stoi(ss[2])});
  }
  //   cout << vs.size() << endl;
  //   cout << vvv.size() << endl;
  //   cout << vvv[0].size() << endl;
  vector<vector<vector<I3Point>>> vsrp(vvv.size());
  for (unsigned is = 0; is < vvv.size(); ++is) {
    vsrp[is].resize(48);
    vector<unsigned> vp{0, 1, 2};
    for (unsigned ip = 0; ip < 6; ++ip) {
      for (int bm = 0; bm < 8; ++bm) {
        unsigned index = ip * 8 + bm;
        auto& vip = vsrp[is][index];
        for (auto& v : vvv[is]) {
          int64_t x = v[vp[0]] * (1 - 2 * ((bm >> 0) & 1));
          int64_t y = v[vp[1]] * (1 - 2 * ((bm >> 1) & 1));
          int64_t z = v[vp[2]] * (1 - 2 * ((bm >> 2) & 1));
          vip.push_back(I3Point(x, y, z));
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

  auto CommonP = [](const vector<I3Point>& v1, const vector<I3Point>& v2) {
    unsigned c = 0, i = 0, j = 0;
    for (; (i < v1.size()) && (j < v2.size());) {
      if (v1[i] < v2[j]) {
        ++i;
      } else if (v2[j] < v1[i]) {
        ++j;
      } else {
        ++c;
        ++i;
        ++j;
      }
    }
    return c;
  };

  auto CommonV = [](const vector<I3Vector>& v1, const vector<I3Vector>& v2) {
    unsigned c = 0, i = 0, j = 0;
    for (; (i < v1.size()) && (j < v2.size());) {
      if (v1[i] < v2[j]) {
        ++i;
      } else if (v2[j] < v1[i]) {
        ++j;
      } else {
        ++c;
        ++i;
        ++j;
      }
    }
    return c;
  };

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
        auto c = CommonV(vfv, vsrv[i1][i2]);
        if (best_match < c) {
          best_match = c;
          best_index1 = i1;
          best_index2 = i2;
        }
      }
    }
    cout << "Match " << best_index1 << " " << best_index2
         << " with commons: " << best_match << endl;
    best_match = 0;
    I3Vector best_shift;
    for (unsigned is = 0; is < 2; ++is) {
      auto vp = vsrp[best_index1][best_index2];
      if (is == 1) {
        for (auto& p : vp) {
          p.x = -p.x;
          p.y = -p.y;
          p.z = -p.z;
        }
        sort(vp.begin(), vp.end());
      }
      for (auto p1 : vp) {
        for (auto p2 : vfp) {
          auto shift = p2 - p1;
          vector<I3Point> vtemp;
          for (auto p : vp) vtemp.push_back(p + shift);
          auto c = CommonP(vfp, vtemp);
          if (best_match < c) {
            best_match = c;
            best_is = is;
            best_shift = shift;
          }
        }
      }
    }
    cout << "\t " << best_is << " [" << best_shift << "]\t" << best_match
         << endl;
    auto vp = vsrp[best_index1][best_index2];
    if (best_is == 1) {
      for (auto& p : vp) {
        p.x = -p.x;
        p.y = -p.y;
        p.z = -p.z;
      }
    }
    for (auto& p : vp) p = p + best_shift;
    vloc.push_back({best_shift.dx, best_shift.dy, best_shift.dz});
    vector<I3Point> new_points;
    for (auto& p : vp) {
      if (!binary_search(vfp.begin(), vfp.end(), p)) new_points.push_back(p);
    }
    cout << "\tAdding new points: " << new_points.size() << endl;
    for (auto p1 : new_points) {
      for (auto p2 : vfp) {
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
    for (auto p2 : vloc) {
      auto v = p2 - p1;
      auto d = Abs(v.dx) + Abs(v.dy) + Abs(v.dz);
      max_dist = max(max_dist, d);
    }
  }
  cout << max_dist << endl;
  return 0;
}
