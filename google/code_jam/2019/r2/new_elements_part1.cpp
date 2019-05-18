#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/stl/hash/pair.h"
#include "common/stl/pair_io.h"
#include "common/vector/read.h"

#include <unordered_set>

int main_new_elements_part1() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    auto v = ReadVector<pair<int64_t, int64_t>>(n);
    unordered_set<pair<int64_t, int64_t>> us;
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned j = i + 1; j < n; ++j) {
        int64_t dc = v[i].first - v[j].first;
        int64_t dj = v[i].second - v[j].second;
        if (dc * dj >= 0) continue;
        if (dc < 0)
          dc = -dc;
        else
          dj = -dj;
        int64_t dd = GCD(dc, dj);
        us.insert({dc / dd, dj / dd});
      }
    }
    cout << "Case #" << it << ": " << us.size() + 1 << endl;
  }
  return 0;
}
