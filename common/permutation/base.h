#pragma once

#include <utility>
#include <vector>

namespace permutation {
using TLine = std::vector<unsigned>;
using TCycle = std::vector<unsigned>;
using TCycles = std::vector<TCycle>;
using TSwap = std::pair<unsigned, unsigned>;
using TSwaps = std::vector<TSwap>;
}  // namespace permutation
