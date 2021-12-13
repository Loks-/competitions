#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2113b() {
  auto vs = nvector::ReadLines();
  unsigned i;
  vector<I2Point> vp;
  for (i = 0; i < vs.size(); ++i) {
    if (vs[i].empty()) break;
    auto ss = Split(vs[i], ',');
    vp.push_back({stoi(ss[0]), stoi(ss[1])});
  }
  for (++i; i < vs.size(); ++i) {
    auto ss = Split(vs[i], ' ');
    int l = stoi(ss[2].substr(2));
    for (auto& p : vp) {
      auto& z = ((ss[2][0] == 'x') ? p.x : p.y);
      if (z > l) z = 2 * l - z;
    }
  }
  vector<vector<unsigned>> vv(6, vector<unsigned>(40, 0));
  for (auto p : vp) vv[p.y][p.x] = 1;
  for (auto& v : vv) {
    for (auto u : v) cout << (u ? '#' : '.');
    cout << endl;
  }
  return 0;
}
