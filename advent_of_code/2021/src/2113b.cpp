#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/make.h"
#include "common/vector/read_from_string.h"
#include "common/vector/read_lines.h"

int main_2113b() {
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
  }
  auto vv = nvector::Make<unsigned>(6, 40);
  for (auto p : vp) vv[p.y][p.x] = 1;
  for (auto& v : vv) {
    for (auto u : v) cout << (u ? '#' : '.');
    cout << endl;
  }
  return 0;
}
