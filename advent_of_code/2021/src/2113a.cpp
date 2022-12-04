#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/point.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_from_string.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

int main_2113a() {
  auto vs = nvector::ReadLines();
  unsigned i;
  vector<I2Point> vp;
  for (i = 0; i < vs.size(); ++i) {
    if (vs[i].empty()) break;
    auto v = nvector::ReadFromString<int>(vs[i], ",");
    vp.push_back({v[0], v[1]});
  }
  for (++i; i < vs.size(); ++i) {
    auto ss = Split(vs[i], " =");
    int l = stoi(ss[3]);
    for (auto& p : vp) {
      auto& z = ((ss[2][0] == 'x') ? p.x : p.y);
      if (z > l) z = 2 * l - z;
    }
    break;
  }
  sort(vp.begin(), vp.end(), CompareXY<int64_t>);
  nvector::Unique(vp);
  cout << vp.size() << endl;
  return 0;
}
