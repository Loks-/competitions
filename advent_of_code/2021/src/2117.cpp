#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/base.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2117() {
  auto s = nvector::ReadLines()[0];
  auto ss = Split(s, ' ');
  auto sx = ss[2].substr(2, ss[2].size() - 3), sy = ss[3].substr(2);
  auto ssx = Split(sx, '.'), ssy = Split(sy, '.');
  int64_t x1 = stoi(ssx[0]), x2 = stoi(ssx[2]), y1 = stoi(ssy[0]),
          y2 = stoi(ssy[2]);
  I2ARectangle box({x1, y1}, {x2, y2});
  int64_t maxy = y1 - 1;
  unsigned r = 0;
  for (int64_t dx = 0; dx <= x2; ++dx) {
    for (int64_t dy = y1; dy <= -y1; ++dy) {
      I2Vector v(dx, dy);
      for (I2Point p(dx, dy); (p.x <= x2) && (p.y >= y1); p += v) {
        if (box.Inside(p)) {
          ++r;
          maxy = max(maxy, dy);
          break;
        }
        if (v.dx > 0) v.dx -= 1;
        v.dy -= 1;
      }
    }
  }
  cout << y1 * (y1 + 1) / 2 << endl;
  cout << r << endl;
  return 0;
}
