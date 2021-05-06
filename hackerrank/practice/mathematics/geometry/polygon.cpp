// https://www.hackerrank.com/challenges/polygon

#include "common/geometry/d2/axis/polygon.h"
#include "common/geometry/d2/axis/count_point_set_polygon.h"
#include "common/geometry/d2/axis/points_set.h"
#include "common/geometry/d2/point_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_polygon() {
  unsigned N, Q, M;
  cin >> N >> Q;
  auto vp = nvector::Read<I2Point>(N);
  I2APointsSet ps(vp);
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    cin >> M;
    auto vq = nvector::Read<I2Point>(M);
    auto ap = I2APolygon(vq);
    cout << Count(ps, ap) << endl;
  }
  return 0;
}
