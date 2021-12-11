#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2109b() {
  auto vs = nvector::ReadLines();
  int64_t n = vs.size(), m = vs[0].size();
  I2ARectangle b({0, 0}, {n - 1, m - 1});
  auto Get = [&](const I2Point& p) { return b.Inside(p) ? vs[p.x][p.y] : '9'; };

  I2Point p;
  unordered_map<int64_t, unsigned> um;
  for (p.x = 0; p.x < n; ++p.x) {
    for (p.y = 0; p.y < m; ++p.y) {
      if (Get(p) == '9') continue;
      I2Point p0 = p;
      for (bool low_point = false; !low_point;) {
        low_point = true;
        auto c = Get(p0);
        for (auto d : I2NeighborsD4()) {
          if (Get(p0 + d) < c) {
            low_point = false;
            p0 += d;
            break;
          }
        }
      }
      um[p0.x * m + p0.y] += 1;
    }
  }
  vector<unsigned> v;
  for (auto p : um) v.push_back(p.second);
  sort(v.begin(), v.end());
  reverse(v.begin(), v.end());
  cout << v[0] * v[1] * v[2] << endl;
  return 0;
}
