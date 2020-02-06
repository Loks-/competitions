// https://www.hackerrank.com/challenges/points-on-rectangle

#include "common/geometry/d2/arectangle.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/box.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_points_on_a_rectangle() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    auto v = ReadVector<I2Point>(N);
    auto r = Box(v);
    bool good = true;
    for (auto& p : v) {
      if (r.StrictInside(p)) good = false;
    }
    cout << (good ? "YES" : "NO") << endl;
  }
  return 0;
}
