#include "common/data_structures/coordinate_compression.h"
#include "common/geometry/d3/point.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2122b() {
  struct Input {
    unsigned v;
    I3Point p1, p2;
  };

  auto vs = nvector::ReadLines();
  vector<vector<int>> vv3(3);
  vector<Input> vi;
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
    for (unsigned i = 0; i < 3; ++i) {
      vv3[i].push_back(v3[2 * i]);
      vv3[i].push_back(v3[2 * i + 1] + 1);
    }
    I3Point p1(v3[0], v3[2], v3[4]), p2(v3[1] + 1, v3[3] + 1, v3[5] + 1);
    vi.push_back({v, p1, p2});
  }
  vector<ds::CoordinateCompression<int64_t>> vcc(3);
  for (unsigned i = 0; i < 3; ++i) vcc[i].InitUnsorted(vv3[i]);
  vector<vector<vector<unsigned>>> vvv(
      vcc[0].Size(), vector<vector<unsigned>>(
                         vcc[1].Size(), vector<unsigned>(vcc[0].Size(), 0)));
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
  return 0;
}
