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
    TAngle angle_f(1, 0), angle_l(-1, 0);
    for (unsigned i = 0; i < N; ++i) {
      TPoint p1, p2;
      cin >> p1 >> p2;
      if (p1.x == 0) continue;
      if (p1.x > 0) {
        p1.x = -p1.x;
        p2.x = -p2.x;
      }
      auto v0 = x0 - p1, v1 = x1 - p1, v2 = p2 - p1;
      TAngle a0(v0), a1(v1), a2(v2);
      assert(a0 < a1);
      if ((a0 < a2) && (a2 < a1)) {
        vpa.push_back({a0 - a2, TAngle(1, 0), i});
        vpa.push_back({TAngle(1, 0), a1 - a2, i});
      } else {
        vpa.push_back({a0 - a2, a1 - a2, i});
      }
    }
    unsigned cnt1 = 0, cnt2 = 0;
    for (unsigned mask = 0; mask < (1u << N); ++mask) {
      ++cnt2;
      vector<pair<TAngle, TAngle>> vca;
      for (auto sl : vpa) {
        if ((1u << sl.index) & mask)
          vca.push_back({sl.l, sl.r});
        else
          vca.push_back({-sl.r, -sl.l});
      }
      sort(vca.begin(), vca.end(),
           [&](auto pl, auto pr) { return cmp(pl.first, pr.first); });
      TAngle ll(1, 0), le(1, 0);
      for (auto p : vca) {
        if (cmp(ll, p.first)) break;
        if (p.second == le) {
          ++cnt1;
          break;
        }
        if (cmp(ll, p.second)) ll = p.second;
      }
    }
    cout << "Case #" << it << ": " << double(cnt2 - cnt1) / double(cnt2)
         << endl;
  }
  return 0;
}
