#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/fraction.h"
#include "common/stl/base.h"

int main_new_elements_part2() {
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
    bool impossible = false;
    TIFraction fl(1, 0), fr(0, 1);
    for (unsigned i = 0; i + 1 < n; ++i) {
      int64_t dc = v[i + 1].c - v[i].c;
      int64_t dj = v[i + 1].j - v[i].j;
      if ((dc <= 0) && (dj <= 0)) {
        impossible = true;
        break;
      }
      if ((dc >= 0) && (dj >= 0)) continue;
      if (dc < 0) {
        TIFraction f(-dc, dj);
        if (fr < f) fr = f;
      } else {
        TIFraction f(dc, -dj);
        if (f < fl) fl = f;
      }
    }
    if (!(fr < fl)) impossible = true;
    if (!impossible) {
      int64_t c = 0, j = 0;
      if ((fr.GetN() == 0) && (fl.GetD() == 0)) {
        c = j = 1;
      } else if (fr.GetN() == 0) {
        c = fl.GetD() / fl.GetN() + 1;
        j = 1;
      } else if (fl.GetD() == 0) {
        c = 1;
        j = fr.GetN() / fr.GetD() + 1;
      } else {
        vector<ContinuedFraction> vcl(2, fr);
        vector<ContinuedFraction> vcr(2, fl);
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
          if ((f < fl) && (fr < f)) {
            int64_t cc = f.GetD(), cj = f.GetN();
            for (--cj; fr < TIFraction(cj, cc);) --cj;
            ++cj;
            if ((c == 0) || (cc < c) || ((cc == c) && (cj < j))) {
              c = cc;
              j = cj;
            }
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
