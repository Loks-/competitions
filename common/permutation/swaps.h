#pragma once

#include "common/permutation/base.h"
#include "common/permutation/permutation.h"

namespace permutation {
// Returns list of elements to swap, not positions.
inline TSwaps Swaps(const Permutation& p) {
  TSwaps swaps;
  for (auto& cycle : p.Cycles()) {
    for (unsigned i = 1; i < cycle.size(); ++i)
      swaps.push_back({cycle[i - 1], cycle[i]});
  }
  return swaps;
}
}  // namespace permutation
