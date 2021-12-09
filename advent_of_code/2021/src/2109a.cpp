#include "common/geometry/d2/base.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2109a() {
  auto vs = nvector::ReadLines();
  int64_t n = vs.size(), m = vs[0].size();
  auto Get = [&](const I2Point& p) {
    return ((p.x < 0) || (p.x >= n) || (p.y < 0) || (p.y >= m)) ? '9'
                                                                : vs[p.x][p.y];
  };

  unsigned r = 0;
  I2Point p;
  for (p.x = 0; p.x < n; ++p.x) {
    for (p.y = 0; p.y < m; ++p.y) {
      auto c = Get(p);
      if (c == '9') continue;
      bool low_point = true;
      for (auto d : I2NeighborsD4()) {
        if (Get(p + d) <= c) low_point = false;
      }
      if (low_point) r += 1 + (c - '0');
    }
  }
  cout << r << endl;
  return 0;
}
