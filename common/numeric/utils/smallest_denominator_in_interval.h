#pragma once

#include "common/base.h"
#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/fraction.h"
#include <algorithm>
#include <vector>

inline TIFraction SmallestDenominatorInInterval(const TIFraction& l,
                                                const TIFraction& r) {
  assert(l < r);
  int64_t n = 0, d = 0;
  std::vector<ContinuedFraction> vcl(2, l);
  std::vector<ContinuedFraction> vcr(2, r);
  vcl[1].SplitLast();
  vcr[1].SplitLast();
  for (unsigned i = 0; i < 4; ++i) {
    auto vl = vcl[i % 2].GetVector();
    auto vr = vcr[i / 2].GetVector();
    std::vector<int64_t> vm;
    for (unsigned k = 0;; ++k) {
      if (k == vl.size()) {
        vm.push_back(vr[k] + 1);
        break;
      } else if (k == vr.size()) {
        vm.push_back(vl[k] + 1);
        break;
      } else if (vl[k] != vr[k]) {
        vm.push_back(std::min(vl[k], vr[k]) + 1);
        break;
      } else {
        vm.push_back(vl[k]);
      }
    }
    auto f = ContinuedFraction(vm).ToFraction();
    if ((f < r) && (l < f)) {
      int64_t cd = f.GetD(), cn = f.GetN();
      if ((d == 0) || (cd < d) || ((cd == d) && (cn < n))) {
        n = cn;
        d = cd;
      }
    }
  }
  return {n, d};
}
