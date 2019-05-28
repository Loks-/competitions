#include "common/geometry/d3/base.h"
#include "common/geometry/d3/plane.h"
#include "common/geometry/d3/point_io.h"
#include "common/geometry/d3/utils/plane_from_points.h"
#include "common/geometry/d3/vector.h"
#include "common/stl/base.h"

#include <cmath>

namespace {}  // namespace

int main_raise_the_roof() {
  struct Point {
    unsigned index;
    // D3Point p;
    geometry::d3::Point<int64_t> p;
  };

  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N;
    cin >> N;
    vector<Point> v(N);
    for (unsigned i = 0; i < N; ++i) {
      v[i].index = i + 1;
      cin >> v[i].p;
    }
    unsigned mi = 0;
    for (unsigned i = 1; i < N; ++i) {
      if (v[mi].p.z < v[i].p.z) mi = i;
    }
    if (mi != 0) swap(v[0], v[mi]);
    mi = N;
    double mvalue = 1.;
    for (unsigned i = 1; i < N; ++i) {
      auto vi = v[0].p - v[i].p;
      assert(!vi.Empty());
      //   vi.Normalize();
      //   if (vi.dz < mvalue) {
      //     mvalue = vi.dz;
      //     mi = i;
      //   }
      double t = double(vi.dz * vi.dz) /
                 (vi.dx * vi.dx + vi.dy * vi.dy + vi.dz * vi.dz);
      if (t < mvalue) {
        mvalue = t;
        mi = i;
      }
    }
    assert(mi < N);
    if (mi != 1) swap(v[1], v[mi]);
    for (unsigned k = 2; k < N; ++k) {
      mi = N;
      for (unsigned i = k; i < N; ++i) {
        auto p = PlaneFromPoints(v[k - 2].p, v[k - 1].p, v[i].p);
        assert(p.Valid() && (p.c != 0));
        if (p.c < 0) p.Flip();
        // cout << k - 2 << "\t" << k - 1 << "\t" << i << endl;
        // for (unsigned j = 0; j < N; ++j) cout << p(v[j].p) << "\t";
        // cout << endl;
        bool ok = true;
        for (unsigned j = k; j < N; ++j) {
          if ((j != i) && (p(v[j].p) > 0)) {
            ok = false;
            break;
          }
        }
        if (ok) {
          mi = i;
          break;
        }
      }
      assert(mi < N);
      if (mi != k) swap(v[k], v[mi]);
    }
    reverse(v.begin(), v.end());
    cout << "Case #" << it << ":";
    for (auto p : v) cout << " " << p.index;
    cout << endl;
  }
  return 0;
}
