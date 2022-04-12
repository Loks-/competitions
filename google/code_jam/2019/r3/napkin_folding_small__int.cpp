#include "common/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/reflection/polygon.h"
#include "common/geometry/d2/utils/scale_points.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_napkin_folding_small__int() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    assert(K == 2);
    auto vp = nvector::Read<I2Point>(N);
    auto pl = I2Polygon(ScaleXY(vp, 2));
    auto l = geometry::d2::reflection::LineOfSymmetry(pl);
    cout << "Case #" << it << ": " << (l.Valid() ? "POSSIBLE" : "IMPOSSIBLE")
         << endl;
    if (l.Valid()) {
      for (auto p : {l.p, l.p + l.v}) {
        for (unsigned i = 0; i < 2; ++i) {
          if (p[i] & 1)
            cout << p[i] << "/2 ";
          else
            cout << p[i] / 2 << "/1 ";
        }
      }
      cout << endl;
    }
  }
  return 0;
}
