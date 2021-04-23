// https://www.hackerrank.com/challenges/a-circle-and-a-square

#include "common/geometry/d2/circle_int_io.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/scale_point.h"
#include "common/geometry/d2/utils/square_from_diagonal.h"
#include "common/stl/base.h"

int main_a_circle_and_a_square() {
  int w, h;
  I2Circle c;
  I2Point pa, pc;
  cin >> w >> h >> c >> pa >> pc;
  auto s = SquareFromDiagonal(ScaleXY(pa, 2), ScaleXY(pc, 2));
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      I2Point p(x, y);
      cout << ((c.Inside(p) || s.Inside(ScaleXY(p, 2))) ? "#" : ".");
    }
    cout << endl;
  }
  return 0;
}
