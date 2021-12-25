#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/stl_hash/point.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2109() {
  auto vs = nvector::ReadLines();
  int64_t n = vs.size(), m = vs[0].size();
  I2ARectangle b({0, 0}, {n - 1, m - 1});
  auto Get = [&](const I2Point& p) { return b.Inside(p) ? vs[p.x][p.y] : '9'; };

  unordered_map<I2Point, unsigned> um;
  for (I2Point p; p.x < n; ++p.x) {
    for (p.y = 0; p.y < m; ++p.y) {
      if (Get(p) == '9') continue;
      I2Point p0 = p;
      for (bool low_point = false; !low_point;) {
        low_point = true;
        auto c = Get(p0);
        for (auto p1 : I2NeighborsD4(p0)) {
          if (Get(p1) < c) {
            low_point = false;
            p0 = p1;
            break;
          }
        }
      }
      um[p0] += 1;
    }
  }
  unsigned r = 0;
  vector<unsigned> v;
  for (auto p : um) {
    r += Get(p.first) - '0' + 1;
    v.push_back(p.second);
  }
  sort(v.begin(), v.end());
  reverse(v.begin(), v.end());
  cout << r << endl << v[0] * v[1] * v[2] << endl;
  return 0;
}
