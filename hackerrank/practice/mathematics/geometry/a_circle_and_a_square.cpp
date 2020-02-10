// https://www.hackerrank.com/challenges/a-circle-and-a-square

#include "common/geometry/d2/circle_io.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/square_from_diagonal.h"
#include "common/stl/base.h"

int main_a_circle_and_a_square() {
  int w, h, x1, y1, x3, y3;
  I2Circle c;
  I2Point pa, pc;
  cin >> w >> h >> c >> pa >> pc;
  I2Point pa2 = (I2Vector(pa) * 2).ToPoint(),
          pc2 = (I2Vector(pc) * 2).ToPoint();
  auto s = SquareFromDiagonal(pa2, pc2);
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      I2Point p(x, y), p2(2 * x, 2 * y);
      cout << ((c.Inside(p) || s.Inside(p2)) ? "#" : ".");
    }
    cout << endl;
  }
  return 0;
}
