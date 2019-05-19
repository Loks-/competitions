#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/fraction.h"
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
      int64_t c = 0, j = 0;
      vector<ContinuedFraction> vcl(2, fl);
      vector<ContinuedFraction> vcr(2, fg);
      vcl[1].SplitLast();
      vcr[1].SplitLast();
      for (unsigned i = 0; i < 4; ++i) {
        auto vl = vcl[i % 2].GetVector();
        auto vr = vcr[i / 2].GetVector();
        vector<int64_t> vm;
        for (unsigned k = 0;; ++k) {
          if (k == vl.size()) {
            vm.push_back(vr[k] + 1);
            break;
          } else if (k == vr.size()) {
            vm.push_back(vl[k] + 1);
            break;
          } else if (vl[k] != vr[k]) {
            vm.push_back(min(vl[k], vr[k]) + 1);
            break;
          } else {
            vm.push_back(vl[k]);
          }
        }
        auto f = ContinuedFraction(vm).ToFraction();
        if ((f < fg) && (fl < f)) {
          int64_t cc = f.GetD(), cj = f.GetN();
          if ((c == 0) || (cc < c) || ((cc == c) && (cj < j))) {
            c = cc;
            j = cj;
          }
        }
      }
      assert(c > 0);
      cout << "Case #" << it << ": " << c << " " << j << endl;
    } else {
      cout << "Case #" << it << ": "
           << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}
