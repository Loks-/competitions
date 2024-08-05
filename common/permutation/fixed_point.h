#pragma once

#include "common/permutation/base.h"

#include <vector>

namespace permutation {
constexpr std::vector<unsigned> FixedPoints(const TLine& line) {
  std::vector<unsigned> output;
  for (unsigned i = 0; i < line.size(); ++i) {
    if (line[i] == i) output.push_back(i);
  }
  return output;
}
}  // namespace permutation
