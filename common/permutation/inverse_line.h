#pragma once

#include "common/permutation/base.h"

namespace permutation {
constexpr TLine Inverse(const TLine& line) {
  TLine v(line.size());
  for (unsigned i = 0; i < line.size(); ++i) v[line[i]] = i;
  return v;
}
}  // namespace permutation
