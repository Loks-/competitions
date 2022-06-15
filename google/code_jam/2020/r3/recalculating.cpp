#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/axis/utils/union_area1_rectangles.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/stl_hash/point.h"
#include "common/geometry/d2/utils/rotate_pi4s_points.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/stl/hash/vector.h"
#include "common/vector/read.h"

#include <deque>
#include <unordered_map>

using TKey = vector<I2Point>;

int main_recalculating() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int64_t D;
    cin >> N >> D;
    auto vp = nvector::Read<I2Point>(N);
    RotatePi4S(vp);
    sort(vp.begin(), vp.end());
    vector<pair<int64_t, unsigned>> vxo;
    int64_t x = vp[0].x - D;
    vector<pair<int64_t, unsigned>> vy;

    auto Insert = [&](int64_t y, unsigned i) {
      auto p = make_pair(y, i);
      auto it = lower_bound(vy.begin(), vy.end(), p);
      vy.insert(it, p);
    };
    auto Remove = [&](int64_t y, unsigned i) {
      auto it = lower_bound(vy.begin(), vy.end(), make_pair(y, i));
      vy.erase(it);
    };

    deque<unsigned> di;
    TKey vkey;
    unordered_map<TKey, vector<I2ARectangle>> mk2b;
    for (unsigned i1 = 0, i2 = 0; i2 < vp.size();) {
      auto xnew =
          (i1 < vp.size()) ? min(vp[i1].x - D, vp[i2].x + D) : vp[i2].x + D;
      if ((x < xnew) && (vy.size() > 0)) {
        di.push_back(vy[0].second);
        for (unsigned j = 1; j < vy.size(); ++j) {
          if ((vy[j].first != vy[j - 1].first) && (!di.empty())) {
            const auto& p0 = vp[di[0]];
            vkey.clear();
            for (unsigned k = 1; k < di.size(); ++k)
              vkey.push_back((vp[di[k]] - p0).ToPoint());
            sort(vkey.begin(), vkey.end());
            mk2b[vkey].push_back({I2Point(x - p0.x, vy[j - 1].first - p0.y),
                                  I2Point(xnew - p0.x, vy[j].first - p0.y)});
          }
          if (!di.empty() && (vy[j].second == di.front())) {
            di.pop_front();
          } else {
            di.push_back(vy[j].second);
          }
        }
      }
      x = xnew;
      if ((i1 < vp.size()) && (vp[i1].x - D < vp[i2].x + D)) {
        Insert(vp[i1].y - D, i1);
        Insert(vp[i1].y + D, i1);
        ++i1;
      } else {
        Remove(vp[i2].y - D, i2);
        Remove(vp[i2].y + D, i2);
        ++i2;
      }
    }

    int64_t s1 = 0, sa = 0;
    for (auto& mp : mk2b) {
      s1 += UnionArea1(mp.second).first;
      for (auto& r : mp.second) sa += r.Area();
    }
    auto sd = GCD(s1, sa);
    cout << "Case #" << it << ": " << s1 / sd << " " << sa / sd << endl;
  }
  return 0;
}
