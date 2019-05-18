#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/stl/hash/pair.h"

#include <unordered_set>
#include <utility>

int main_new_elements_part1() {
  struct CJ {
    int64_t c, j;
  };

  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    vector<CJ> v(n);
    for (unsigned i = 0; i < n; ++i) {
      cin >> v[i].c >> v[i].j;
    }
    unordered_set<pair<int64_t, int64_t>> us;
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned j = i + 1; j < n; ++j) {
        int64_t dc = v[i].c - v[j].c;
        int64_t dj = v[i].j - v[j].j;
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
