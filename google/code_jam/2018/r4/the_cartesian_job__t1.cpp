#include "common/geometry/d2/base.h"
#include "common/geometry/d2/iangle.h"
#include "common/geometry/d2/point_io.h"
#include "common/stl/base.h"

#include <functional>
#include <iomanip>
#include <unordered_map>

using TPoint = I2Point;
using TAngle = I2Angle;

int main_the_cartesian_job__t1() {
  struct SLaser {
    TAngle l;
    TAngle r;
    unsigned index;
  };
  cout << setprecision(6) << fixed;
  auto cmp = [](const TAngle& l, const TAngle& r) {
    return l.Compare02Pi_i128(r);
  };

  const TPoint x0(0, 0), x1(0, 1000);
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<SLaser> vpa;
    TAngle angle_f(1, 0), angle_l(-1, 0), ll;
    for (unsigned i = 0; i < N; ++i) {
      TPoint p1, p2;
      cin >> p1 >> p2;
      if (p1.x == 0) continue;
      if (p1.x > 0) {
        p1.x = -p1.x;
        p2.x = -p2.x;
      }
      auto v0 = x0 - p1, v1 = x1 - p1, v2 = p2 - p1;
      TAngle a0(v0), a1(v1), a2(v2), a2i(-v2);
      assert(a0 < a1);
      if ((a0 < a2) && (a2 < a1)) {
        auto b0 = a2 - a0, b1 = a1 - a2;
        if (cmp(b1, b0)) swap(b0, b1);
        if (cmp(angle_f, b0)) angle_f = b0;
        if (b0 != b1) vpa.push_back({b0, b1, i});
      } else if ((a0 < a2i) && (a2i < a1)) {
        auto b0 = a0 - a2, b1 = a2 - a1;
        if (cmp(b1, b0)) swap(b0, b1);
        if (cmp(b1, angle_l)) angle_l = b1;
        if (b0 != b1) vpa.push_back({b0, b1, i});
      } else {
        auto b0 = a0 - a2, b1 = a1 - a2;
        if (b0.CompareVS0())
          vpa.push_back({-b1, -b0, i});
        else
          vpa.push_back({b0, b1, i});
      }
    }
    unsigned cnt1 = 0, cnt2 = 0;
    for (unsigned mask = 0; mask < (1u << N); ++mask) {
      ++cnt2;
      vector<pair<TAngle, TAngle>> vca1, vca2;
      for (auto sl : vpa) {
        if ((1u << sl.index) & mask) {
          vca1.push_back({sl.l, sl.r});
        } else {
          vca2.push_back({sl.l, sl.r});
        }
      }
      sort(vca1.begin(), vca1.end(),
           [&](auto pl, auto pr) { return cmp(pl.first, pr.first); });
      sort(vca2.begin(), vca2.end(),
           [&](auto pl, auto pr) { return cmp(pl.first, pr.first); });
      bool f1 = false, f2 = false;
      ll = angle_f;
      for (auto p : vca1) {
        if (cmp(ll, p.first)) break;
        if (cmp(ll, p.second)) ll = p.second;
        if (!cmp(ll, angle_l)) {
          f1 = true;
          break;
        }
      }
      ll = angle_f;
      for (auto p : vca2) {
        if (cmp(ll, p.first)) break;
        if (cmp(ll, p.second)) ll = p.second;
        if (!cmp(ll, angle_l)) {
          f2 = true;
          break;
        }
      }
      if (f1 && f2) ++cnt1;
    }
    cout << "Case #" << it << ": " << double(cnt2 - cnt1) / double(cnt2)
         << endl;
  }
  return 0;
}
