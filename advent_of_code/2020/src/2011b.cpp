#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2011b() {
  auto vs = nvector::ReadLines();
  vector<vector<unsigned>> vv;
  for (auto& s : vs) {
    vector<unsigned> v;
    for (auto c : s) v.push_back((c == '.' ? 2 : 0));
    vv.push_back(v);
  }
  auto vvnext = vv;
  int64_t dx = vv[0].size(), dy = vv.size();
  I2ARectangle box({0, 0}, {dx - 1, dy - 1});
  for (;; vv = vvnext) {
    I2Point p(0, 0);
    for (; p.x < dx; ++p.x) {
      for (p.y = 0; p.y < dy; ++p.y) {
        if (vv[p.x][p.y] > 1) continue;
        unsigned n = 0;
        for (auto d : I2NeighborsD8()) {
          for (auto p0 = p + d; box.Inside(p0); p0 += d) {
            auto x = vv[p0.x][p0.y];
            if (x < 2) {
              n += x;
              break;
            }
          }
        }
        if (n == 0) vvnext[p.x][p.y] = 1;
        if (n >= 5) vvnext[p.x][p.y] = 0;
      }
    }
    if (vv == vvnext) break;
  }
  unsigned r = 0;
  for (auto& v : vv) {
    for (auto x : v) {
      if (x == 1) ++r;
    }
  }
  cout << r << endl;
  return 0;
}
