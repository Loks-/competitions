#include "common/geometry/d3/point.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"
#include "common/vector/sum.h"

int main_2122a() {
  auto vs = nvector::ReadLines();
  int minx = -50, maxx = 50, l = maxx - minx + 1;
  vector<vector<vector<unsigned>>> vvv(
      l, vector<vector<unsigned>>(l, vector<unsigned>(l, 0)));
  for (auto& s : vs) {
    auto ss = Split(s, ' ');
    unsigned v = (ss[0] == "on" ? 1 : 0);
    auto ss1 = Split(ss[1], ',');
    vector<int> v3;
    for (unsigned i = 0; i < 3; ++i) {
      auto ss2 = Split(ss1[i].substr(2), '.');
      v3.push_back(stoi(ss2[0]));
      v3.push_back(stoi(ss2[2]));
    }
    bool ok = true;
    for (auto x : v3) ok = ok && (x >= minx) && (x <= maxx);
    if (!ok) continue;
    I3Point p1(v3[0], v3[2], v3[4]), p2(v3[1], v3[3], v3[5]);
    for (int x = p1.x; x <= p2.x; ++x) {
      for (int y = p1.y; y <= p2.y; ++y) {
        for (int z = p1.z; z <= p2.z; ++z) {
          vvv[x - minx][y - minx][z - minx] = v;
        }
      }
    }
  }
  cout << nvector::SumVVV(vvv) << endl;
  return 0;
}
