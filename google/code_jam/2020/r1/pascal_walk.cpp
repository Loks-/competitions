#include "common/data_structures/compact_intervals_based_set.h"
#include "common/numeric/binomial_coefficient_table.h"
#include "common/stl/base.h"

#include "common/assert_exception.h"

using IBS = ds::CompactIntervalsBasedSet<unsigned>;
using VIBS = vector<IBS>;

int main_pascal_walk() {
  unsigned bct_max_value = (1u << 30);
  numeric::BinomialCoefficientTable bct(bct_max_value);
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    if (N == 1) {
      cout << "Case #" << it << ":" << endl << "1 1" << endl;
      continue;
    }
    uint64_t N2 = N;
    vector<VIBS> v;
    IBS empty;
    v.push_back(VIBS(1));
    v[0][0].InsertBack(1);
    bool found = false;
    unsigned fn = 0, fk = 0;
    for (unsigned n = 1; !found; ++n) {
      v.push_back(VIBS(n + 1));
      auto& vp = v[v.size() - 2];
      auto& vc = v[v.size() - 1];
      for (unsigned k = 0; k <= n; ++k) {
        auto& vk = vc[k];
        if (k > 0) {
          vk.Insert(vp[k - 1]);
          vk.Insert(vc[k - 1]);
        }
        if (k < n) vk.Insert(vp[k]);
        vk.ShiftValues(bct(n, k));
        if (!found && vk.HasKey(N)) {
          found = true;
          fn = n;
          fk = k;
        }
      }
    }
    vector<pair<unsigned, unsigned>> vs;
    for (;;) {
      vs.push_back({fn, fk});
      N -= bct(fn, fk);
      if (N == 0) break;  // fn == 0
      if ((fk < fn) && v[fn - 1][fk].HasKey(N)) {
        fn -= 1;
      } else if (fk && v[fn - 1][fk - 1].HasKey(N)) {
        fn -= 1;
        fk -= 1;
      } else if (fk && v[fn][fk - 1].HasKey(N)) {
        fk -= 1;
      }
    }
    reverse(vs.begin(), vs.end());
    cout << "Case #" << it << ":" << endl;
    for (auto p : vs) cout << p.first + 1 << " " << p.second + 1 << endl;
  }
  return 0;
}
