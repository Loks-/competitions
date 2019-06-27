// https://www.hackerrank.com/challenges/jim-beam

#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/segment_intersect.h"
#include "common/stl/base.h"

int main_jim_beam() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    I2Point p0(0, 0), p1, p2, pm;
    cin >> p1 >> p2 >> pm;
    I2ClosedSegment s1(p0, pm), s2(p1, p2);
    std::cout << (Intersect(s1, s2) ? "NO" : "YES") << endl;
  }
  return 0;
}
