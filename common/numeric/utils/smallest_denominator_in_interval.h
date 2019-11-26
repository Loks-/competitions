#pragma once

#include "common/base.h"
#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/fraction.h"
#include <algorithm>
#include <vector>

inline TIFraction SmallestDenominatorInInterval(const TIFraction& l,
                                                const TIFraction& r) {
  assert(l < r);
  auto vl = numeric::cf::ContinuedFraction(l).GetVector(),
       vr = numeric::cf::ContinuedFraction(r).GetVector();
  unsigned i = 0, i_end = std::min(vl.size(), vr.size());
  std::vector<int64_t> vm;
  for (; (i < i_end) && (vl[i] == vr[i]); ++i) vm.push_back(vl[i]);
  if (i >= vl.size()) {
    vm.push_back(vr[i] + 1);
  } else if (i >= vr.size()) {
    vm.push_back(vl[i] + 1);
  } else {
    if (vl[i] > vr[i]) vl.swap(vr);
    if ((vr.size() == i + 1) && (vr[i] == vl[i] + 1)) {
      vm.push_back(vl[i]);
      if (vl.size() == i + 1) {
        vm.push_back(2);
      } else if (vl[i + 1] == 1) {
        vm.push_back(1);
        vm.push_back(vl[i + 2] + 1);
      } else if ((vl.size() == i + 2) && (vl[i + 1] == 2)) {
        vm.push_back(1);
        vm.push_back(2);
      } else {
        vm.push_back(2);
      }
    } else {
      vm.push_back(vl[i] + 1);
    }
  }
  return numeric::cf::ContinuedFraction(vm).ToFraction();
}
