#pragma once

#include "permutation.h"
#include <vector>

std::vector<unsigned> PermutationFixedPoints(const Permutation& p) {
  std::vector<unsigned> output;
  const Permutation::TLine& l = p.Line();
  for (unsigned i = 0; i < l.size(); ++i) {
    if (l[i] == i) output.push_back(i);
  }
  return output;
}
