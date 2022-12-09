#include "common/geometry/d2/base.h"
#include "common/geometry/d2/distance/distance_linf.h"
#include "common/numeric/utils/sign.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

int main_2209() {
  auto vs = nvector::ReadLines();
  for (unsigned N : {2, 10}) {
    vector<I2Point> vp(N), vt;
    for (auto& s : vs) {
      auto ss = Split(s, " ");
      I2Vector v;
      if (ss[0] == "U")
        v.dy = 1;
      else if (ss[0] == "D")
        v.dy = -1;
      else if (ss[0] == "R")
        v.dx = 1;
      else if (ss[0] == "L")
        v.dx = -1;
      int l = stoi(ss[1]);
      for (int i = 0; i < l; ++i) {
        vp[0] += v;
        for (unsigned j = 1; j < N; ++j) {
          auto &h = vp[j - 1], &t = vp[j];
          if (DistanceLInf(h, t) <= 1) continue;
          t.x += Sign(h.x - t.x);
          t.y += Sign(h.y - t.y);
        }
        vt.push_back(vp.back());
      }
    }
    nvector::UniqueUnsorted(vt);
    cout << vt.size() << endl;
  }
  return 0;
}
