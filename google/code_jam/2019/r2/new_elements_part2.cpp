#include "common/numeric/fraction.h"
#include "common/numeric/utils/smallest_denominator_in_interval.h"
#include "common/stl/base.h"
#include "common/stl/pair_io.h"
#include "common/vector/read.h"

int main_new_elements_part2() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    auto v = ReadVector<pair<int64_t, int64_t>>(n);
    bool impossible = false;
    TIFraction fg(1, 0), fl(0, 1);
    for (unsigned i = 0; i + 1 < n; ++i) {
      int64_t dc = v[i + 1].first - v[i].first;
      int64_t dj = v[i + 1].second - v[i].second;
      if ((dc <= 0) && (dj <= 0)) {
        impossible = true;
        break;
      }
      if ((dc >= 0) && (dj >= 0)) continue;
      if (dc < 0) {
        TIFraction f(-dc, dj);
        if (fl < f) fl = f;
      } else {
        TIFraction f(dc, -dj);
        if (f < fg) fg = f;
      }
    }
    if (!(fl < fg)) impossible = true;
    if (!impossible) {
      auto f = SmallestDenominatorInInterval(fl, fg);
      cout << "Case #" << it << ": " << f.GetD() << " " << f.GetN() << endl;
    } else {
      cout << "Case #" << it << ": "
           << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}
