// https://www.hackerrank.com/challenges/sherlock-and-geometry

#include "common/geometry/d2/circle_int_io.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/intersect_segment_icircle.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_sherlock_and_geometry() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    I2Circle c;
    cin >> c;
    auto vp = nvector::Read<I2Point>(3);
    bool b = false;
    for (unsigned i = 0; i < 3; ++i) {
      if (Intersect(I2ClosedSegment(vp[i], vp[(i + 1) % 3]), c)) b = true;
    }
    cout << (b ? "YES" : "NO") << endl;
  }
  return 0;
}
