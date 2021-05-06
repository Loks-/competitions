#include "common/numeric/utils/concat.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ulog.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_roaring_years() {
  unsigned T;
  cin >> T;
  auto v = nvector::Read<uint64_t>(T);
  auto M = *max_element(v.begin(), v.end());
  vector<uint64_t> vry;
  for (uint64_t i = 1;; ++i) {
    auto i3 = ConcatNumbers(i, ConcatNumbers(i + 1, i + 2));
    vry.push_back(i3);
    if (i3 > M) break;
    for (auto j = i + 3; (i3 <= M); ++j) {
      auto ij = ConcatNumbers(i3, j);
      if (ij / PowU<uint64_t>(10u, ULog10(j) + 1) != i3) break;  // overflow
      i3 = ij;
      vry.push_back(i3);
    }
  }
  sort(vry.begin(), vry.end());

  for (unsigned it = 1; it <= T; ++it) {
    auto k = v[it - 1];
    auto r = *upper_bound(vry.begin(), vry.end(), v[it - 1]);
    auto l = ULog10(k) + 1;
    auto ic = (l & 1) ? PowU<uint64_t>(10u, l / 2) - 1
                      : k / PowU<uint64_t>(10u, l / 2);
    auto rc = ConcatNumbers(ic, ic + 1);
    if (rc <= k) rc = ConcatNumbers(ic + 1, ic + 2);
    cout << "Case #" << it << ": " << min(r, rc) << endl;
  }
  return 0;
}
