#include "common/geometry/d2/base.h"
#include "common/geometry/d2/hash/point.h"
#include "common/geometry/d2/utils/sign.h"
#include "common/hash.h"
#include "common/stl/base.h"
#include "common/vector/extract_int.h"
#include "common/vector/read_lines.h"

#include <unordered_set>

int main_2214() {
  auto vs = nvector::ReadLines();
  int64_t maxy = 0, count = 0, x0 = 500;
  unordered_set<I2Point, DHash<I2Point>> ps;
  auto Test = [&](const I2Point& p) { return ps.find(p) == ps.end(); };

  for (auto& s : vs) {
    auto ss = nvector::ExtractInt<int64_t>(s, " ,");
    I2Point p(ss[0], ss[1]);
    maxy = max(maxy, p.y);
    for (unsigned i = 2; i < ss.size(); i += 2) {
      I2Point p2(ss[i], ss[i + 1]);
      maxy = max(maxy, p2.y);
      for (auto d = Sign(p2 - p); p != p2; p += d) ps.insert(p);
    }
    ps.insert(p);
  }
  for (int64_t x = x0 - (maxy + 2); x <= x0 + (maxy + 2); ++x)
    ps.insert(I2Point(x, maxy + 2));
  bool first = true;
  vector<I2Vector> vd{{0, 1}, {-1, 1}, {1, 1}, {0, 0}};
  for (;;) {
    I2Point p(500, 0);
    for (I2Vector d;;) {
      for (auto dt : vd) {
        if (Test(p + (d = dt))) break;
      }
      if (d.dy == 0) break;
      p += d;
    }
    if (first && (p.y > maxy)) {
      cout << count << endl;
      first = false;
    }
    ++count;
    ps.insert(p);
    if (p.y == 0) break;
  }
  cout << count << endl;
  return 0;
}
