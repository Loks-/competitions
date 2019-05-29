#include "common/geometry/d3/base.h"
#include "common/geometry/d3/plane_pn.h"
#include "common/geometry/d3/point_io.h"
#include "common/geometry/d3/utils/plane_pn_from_points.h"
#include "common/geometry/d3/vector.h"
#include "common/stl/base.h"

int main_raise_the_roof__pn() {
  struct Point {
    unsigned index;
    I3Point p;
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
    swap(v[0], v[mi]);
    mi = N;
    double mvalue = 1.;
    for (unsigned i = 1; i < N; ++i) {
      auto vi = v[0].p - v[i].p;
      assert(!vi.Empty());
      double t = double(vi.dz * vi.dz) /
                 (vi.dx * vi.dx + vi.dy * vi.dy + vi.dz * vi.dz);
      if (t < mvalue) {
        mvalue = t;
        mi = i;
      }
    }
    assert(mi < N);
    swap(v[1], v[mi]);
    for (unsigned k = 2; k < N; ++k) {
      mi = N;
      for (unsigned i = k; i < N; ++i) {
        auto p = PlanePNFromPoints(v[k - 2].p, v[k - 1].p, v[i].p);
        assert(p.Valid() && (p.n.dz != 0));
        if (p.n.dz < 0) p.SetOppositeNormal();
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
      swap(v[k], v[mi]);
    }
    reverse(v.begin(), v.end());
    cout << "Case #" << it << ":";
    for (auto p : v) cout << " " << p.index;
    cout << endl;
  }
  return 0;
}
