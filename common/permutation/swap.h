#pragma once

#include "common/permutation/permutation.h"

// Returns list of elements to swap, not positions.
Permutation::TSwaps PermutationToSwaps(const Permutation& p) {
  Permutation::TSwaps swaps;
  for (auto& cycle : p.Cycles()) {
    for (unsigned i = 1; i < cycle.size(); ++i)
      swaps.push_back({cycle[i - 1], cycle[i]});
  }
  return swaps;
}
