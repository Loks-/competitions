#include "common/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/reflection/polygon.h"
#include "common/numeric/fraction_io.h"
#include "common/stl/base.h"

using TPoint = geometry::d2::Point<TIFraction>;
using TPolygon = geometry::d2::Polygon<TIFraction>;

int main_napkin_folding_small() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    assert(K == 2);
    vector<TPoint> vp(N);
    for (unsigned i = 0; i < N; ++i) {
      int64_t x, y;
      cin >> x >> y;
      vp[i] = TPoint(TIFraction(x), TIFraction(y));
    }
    auto pl = TPolygon(vp);
    auto l = geometry::d2::reflection::LineOfSymmetry(pl);
    cout << "Case #" << it << ": " << (l.Valid() ? "POSSIBLE" : "IMPOSSIBLE")
         << endl;
    if (l.Valid()) {
      for (auto p : {l.p, l.p + l.v}) cout << p.x << " " << p.y << " ";
      cout << endl;
    }
  }
  return 0;
}
