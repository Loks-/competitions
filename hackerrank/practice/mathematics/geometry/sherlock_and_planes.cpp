// https://www.hackerrank.com/challenges/sherlock-and-planes

#include "common/geometry/d3/point_io.h"
#include "common/geometry/d3/utils/plane_from_points.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_sherlock_and_planes() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    auto v = nvector::Read<I3Point>(4);
    auto p = PlaneFromPoints(v[0], v[1], v[2]);
    cout << ((p(v[3]) == 0) ? "YES" : "NO") << endl;
  }
  return 0;
}
