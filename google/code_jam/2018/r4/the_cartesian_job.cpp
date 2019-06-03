#include "common/geometry/d2/base.h"
#include "common/geometry/d2/iangle.h"
#include "common/geometry/d2/point_io.h"
#include "common/stl/base.h"

#include <functional>
#include <iomanip>
#include <unordered_map>

using TPoint = I2Point;
using TAngle = I2Angle;

int main_the_cartesian_job() {
  cout << setprecision(6) << fixed;
  auto cmp = [](const TAngle& l, const TAngle& r) {
    return l.Compare02Pi_i128(r);
  };

  const TPoint x0(0, 0), x1(0, 1000);
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<pair<TAngle, TAngle>> vpa;
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
      TAngle a0(v0), a1(v1), a2(v2), a2i(-v2);
      assert(a0 < a1);
      if ((a0 < a2) && (a2 < a1)) {
        auto b0 = a2 - a0, b1 = a1 - a2;
        if (cmp(b1, b0)) swap(b0, b1);
        if (cmp(angle_f, b0)) angle_f = b0;
        if (b0 != b1) vpa.push_back({b0, b1});
      } else if ((a0 < a2i) && (a2i < a1)) {
        auto b0 = a0 - a2, b1 = a2 - a1;
        if (cmp(b1, b0)) swap(b0, b1);
        if (cmp(b1, angle_l)) angle_l = b1;
        if (b0 != b1) vpa.push_back({b0, b1});
      } else {
        auto b0 = a0 - a2, b1 = a1 - a2;
        if (b0.CompareVS0())
          vpa.push_back({-b1, -b0});
        else
          vpa.push_back({b0, b1});
      }
    }
    assert((angle_f.dx > 0) && (angle_f.dy >= 0));
    assert((angle_l.dx < 0) && (angle_f.dy >= 0));
    assert(cmp(angle_f, angle_l));
    vector<TAngle> va{angle_f, angle_l};
    for (auto p : vpa) {
      assert(p.first.dy >= 0);
      assert(p.second.dy >= 0);
      assert(cmp(p.first, p.second));
      va.push_back(p.first);
      va.push_back(p.second);
    }
    sort(va.begin(), va.end(), cmp);
    auto va_it = unique(va.begin(), va.end());
    va.resize(va_it - va.begin());
    auto it_f = lower_bound(va.begin(), va.end(), angle_f, cmp);
    auto it_l = lower_bound(va.begin(), va.end(), angle_l, cmp);
    unsigned l = it_l - it_f;
    assert(l > 0);
    vector<pair<unsigned, unsigned>> vpu;
    for (auto p : vpa) {
      auto it1 = lower_bound(va.begin(), va.end(), p.first, cmp);
      auto it2 = lower_bound(va.begin(), va.end(), p.second, cmp);
      assert(it1 < it2);
      if (it1 < it_f) it1 = it_f;
      if (it_l < it2) it2 = it_l;
      if (it1 < it2) vpu.push_back({it1 - it_f, it2 - it_f});
    }
    sort(vpu.begin(), vpu.end());

    std::unordered_map<uint64_t, double> cache;
    std::function<double(unsigned, unsigned, unsigned)> SolveR =
        [&](unsigned i, unsigned l0, unsigned l1) -> double {
      if (l1 < l0) swap(l0, l1);
      uint64_t one = 1ull;
      uint64_t h = i + (one << 15) * l0 + (one << 30) * l1;
      auto itc = cache.find(h);
      if (itc != cache.end()) return itc->second;
      // cerr << "SolveR(" << i << ", " << l0 << ", " << l1 << ")" << endl;
      if (l0 >= l) return (cache[h] = 0.);
      if (i >= vpu.size()) return (cache[h] = 1.);
      if (vpu[i].first > l0) return (cache[h] = 1.);
      return (cache[h] = 0.5 * (SolveR(i + 1, max(l0, vpu[i].second), l1) +
                                SolveR(i + 1, l0, max(l1, vpu[i].second))));
    };

    cout << "Case #" << it << ": " << SolveR(0, 0, 0) << endl;
  }
  return 0;
}
