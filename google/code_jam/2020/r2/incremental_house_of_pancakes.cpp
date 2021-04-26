#include "common/algorithm/binary_search.h"
#include "common/stl/base.h"

int main_incremental_house_of_pancakes() {
  unsigned T;
  uint64_t l, r, k, maxv = (1ull << 31);

  auto f1 = [](uint64_t x) { return (x * (x + 1)) / 2; };
  auto f2 = [&](uint64_t x) { return x * k + x * (x + 1); };

  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> l >> r;
    if (l >= r) {
      k = LowerBoundF<uint64_t, uint64_t>(0, maxv, l - r + 1, f1) - 1;
      l -= f1(k);
    } else {
      k = LowerBoundF<uint64_t, uint64_t>(0, maxv, r - l, f1) - 1;
      r -= f1(k);
      if (k < r) r -= ++k;
    }
    auto j = LowerBoundF<uint64_t, uint64_t>(0, maxv, r + 1, f2) - 1;
    l -= f2(j) - j;
    r -= f2(j);
    k += 2 * j + 1;
    if (k <= l) l -= k++;
    cout << "Case #" << it << ": " << k - 1 << " " << l << " " << r << endl;
  }
  return 0;
}
