#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"

#include <queue>

int main_2111() {
  auto vs = nvector::ReadLines();
  auto v = nvector::TransformV(vs, [](auto& s) {
    vector<unsigned> vt;
    for (auto c : s) vt.push_back(c - '0');
    return vt;
  });

  I2ARectangle b({0, 0}, {9, 9});
  unsigned ra = 0, ri;
  for (unsigned im = 1;; ++im) {
    ri = 0;
    queue<I2Point> q;
    I2Point p;
    for (p.x = 0; p.x < 10; ++p.x) {
      for (p.y = 0; p.y < 10; ++p.y) {
        if (++v[p.x][p.y] == 10) q.push(p);
      }
    }
    for (; !q.empty(); q.pop()) {
      p = q.front();
      if (v[p.x][p.y] >= 10) {
        v[p.x][p.y] = 0;
        ++ri;
        for (auto p1 : I2NeighborsD8(p)) {
          if (!b.Inside(p1)) continue;
          if (v[p1.x][p1.y] != 0) {
            if (++v[p1.x][p1.y] == 10) q.push(p1);
          }
        }
      }
    }
    ra += ri;
    if (im == 100) cout << ra << endl;
    if (ri == 100) {
      cout << im << endl;
      break;
    }
  }
  return 0;
}
