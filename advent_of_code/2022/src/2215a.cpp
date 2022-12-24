#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/vector/extract_int.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

#include <unordered_set>

int main_2215a() {
  auto vs = nvector::ReadLines();
  int64_t y0 = 2000000;
  vector<int64_t> blocked;
  unordered_set<int64_t> ony;
  for (auto& s : vs) {
    auto si = nvector::ExtractInt<int64_t>(s, " ,=:");
    int64_t xs = si[0], ys = si[1], xb = si[2], yb = si[3],
            d = Abs(xs - xb) + Abs(ys - yb), da = d - Abs(ys - y0);
    for (int64_t x = xs - da; x <= xs + da; ++x) blocked.push_back(x);
    if (yb == y0) ony.insert(xb);
  }
  nvector::UniqueUnsorted(blocked);
  cout << blocked.size() - ony.size() << endl;
  return 0;
}
