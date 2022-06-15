#pragma once

#include <algorithm>
#include <vector>

namespace nvector {
// Useful for Grundy/Nim value
inline unsigned FirstMissed(std::vector<unsigned>& v) {
  std::sort(v.begin(), v.end());
  unsigned k = 0, l = 0;
  for (; (l < v.size()) && (v[l] <= k); ++l) {
    if (v[l] == k) ++k;
  }
  return k;
}
}  // namespace nvector
