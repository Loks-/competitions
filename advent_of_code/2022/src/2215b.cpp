#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/vector/extract_int.h"
#include "common/vector/intersection.h"
#include "common/vector/read_lines.h"

int main_2215b() {
  auto vs = nvector::ReadLines();
  // int64_t maxx = 4000000;
  vector<int64_t> v0, v1, v2, v3;
  for (auto& s : vs) {
    auto si = nvector::ExtractInt<int64_t>(s, " ,=:");
    int64_t xs = si[0], ys = si[1], xb = si[2], yb = si[3],
            d = Abs(xs - xb) + Abs(ys - yb);
    v0.push_back(xs - ys - d - 1);
    v1.push_back(xs - ys + d + 1);
    v2.push_back(xs + ys - d - 1);
    v3.push_back(xs + ys + d + 1);
  }
  auto vid = nvector::IntersectionVUnsorted(v0, v1);
  auto vis = nvector::IntersectionVUnsorted(v2, v3);
  assert((vid.size() == 1) && (vis.size() == 1));
  cout << ((vid[0] + vis[0]) * 4000000 + (vis[0] - vid[0])) / 2 << endl;
  return 0;
}
