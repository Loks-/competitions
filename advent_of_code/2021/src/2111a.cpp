#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"

#include <queue>

int main_2111a() {
  auto vs = nvector::ReadLines();
  auto v = nvector::TransformV(vs, [](auto& s) {
    vector<unsigned> vt;
    for (auto c : s) vt.push_back(c - '0');
    return vt;
  });

  I2ARectangle b({0, 0}, {9, 9});
  unsigned r = 0;
  for (unsigned im = 0; im < 100; ++im) {
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
        ++r;
        for (auto d : I2NeighborsD8()) {
          auto p1 = p + d;
          if (!b.Inside(p1)) continue;
          if (v[p1.x][p1.y] != 0) {
            if (++v[p1.x][p1.y] == 10) q.push(p1);
          }
        }
      }
    }
  }
  cout << r << endl;
  return 0;
}
