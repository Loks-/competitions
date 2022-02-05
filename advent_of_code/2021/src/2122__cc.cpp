#include "common/data_structures/coordinate_compression.h"
#include "common/geometry/d3/axis/rectangle.h"
#include "common/geometry/d3/base.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/make.h"
#include "common/vector/read_lines.h"

// Time: O(N^4)
int main_2122__cc() {
  struct Input {
    unsigned v;
    I3Point p1, p2;
  };

  const int maxv = 50;
  const I3Vector v1(1, 1, 1);
  const I3Point pl = (v1 * maxv).ToPoint();
  const I3ARectagnle rl(-pl, pl);

  auto vs = nvector::ReadLines();
  for (bool ab : {false, true}) {
    vector<vector<int>> vv3(3);
    vector<Input> vi;
    for (auto& s : vs) {
      auto ss = Split(s, ' ');
      unsigned v = (ss[0] == "on" ? 1 : 0);
      auto ss1 = Split(ss[1], ',');
      I3Point p1, p2;
      for (unsigned i = 0; i < 3; ++i) {
        auto ss2 = Split(ss1[i].substr(2), '.');
        p1[i] = stoi(ss2[0]);
        p2[i] = stoi(ss2[2]) + 1;
        vv3[i].push_back(p1[i]);
        vv3[i].push_back(p2[i]);
      }
      if (ab || (rl.Inside(p1) && rl.Inside(p2 - v1)))
        vi.push_back({v, p1, p2});
    }
    vector<ds::CoordinateCompression<int64_t>> vcc(3);
    for (unsigned i = 0; i < 3; ++i) vcc[i].InitUnsorted(vv3[i]);
    auto vvv =
        nvector::Make<unsigned>(vcc[0].Size(), vcc[1].Size(), vcc[2].Size(), 0);
    for (auto& s : vi) {
      auto x0 = vcc[0].GetNew(s.p1.x), x1 = vcc[0].GetNew(s.p2.x);
      auto y0 = vcc[1].GetNew(s.p1.y), y1 = vcc[1].GetNew(s.p2.y);
      auto z0 = vcc[2].GetNew(s.p1.z), z1 = vcc[2].GetNew(s.p2.z);
      for (auto x = x0; x < x1; ++x) {
        for (auto y = y0; y < y1; ++y) {
          for (auto z = z0; z < z1; ++z) vvv[x][y][z] = s.v;
        }
      }
    }
    int64_t r = 0;
    for (unsigned x = 1; x < vcc[0].Size(); ++x) {
      auto dx = vcc[0].GetOld(x) - vcc[0].GetOld(x - 1);
      for (unsigned y = 1; y < vcc[1].Size(); ++y) {
        auto dy = vcc[1].GetOld(y) - vcc[1].GetOld(y - 1);
        for (unsigned z = 1; z < vcc[2].Size(); ++z) {
          auto dz = vcc[2].GetOld(z) - vcc[2].GetOld(z - 1);
          r += vvv[x - 1][y - 1][z - 1] * dx * dy * dz;
        }
      }
    }
    cout << r << endl;
  }
  return 0;
}
